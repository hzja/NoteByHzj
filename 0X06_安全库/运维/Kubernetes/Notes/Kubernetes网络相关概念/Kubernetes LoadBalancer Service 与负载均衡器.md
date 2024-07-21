Kubernetes<br />介绍过一些 Ingress 使用，比如 Ingress SSL 透传、Ingress 的多租户。从 Demo 看起来是创建 Ingress 之后，就能从集群外访问服务了。实际上除了 Ingress 的作用以外，还有 Kubernetes Service 和负载均衡器（Load Balancer）参与（当 Service 类型为 LoadBalancer 时）。<br />来介绍了 Kubernetes LoadBalancer Service 和两个比较典型的负载均衡器的工作原理。
<a name="aT3AL"></a>
## LoadBalancer Service
Service 是 Kubernetes 中的资源类型，用来将一组 Pod 的应用作为网络服务公开。每个 Pod 都有自己的 IP，但是这个 IP 的生命周期与 Pod 生命周期一致，也就是说 Pod 销毁后这个 IP 也就无效了（也可能被分配给其他的 Pod 使用）。而 Service 的 IP（ClusterIP） 则是在创建之后便不会改变，Service 与 Pod 之前通过 userspace 代理、iptables 和 ipvs 代理 等手段关联。<br />LoadBalancer 是 Service 四种类型中的一种，其他三种是 ClusterIP、NodePort、ExternalName。<br />LoadBalancer 的工作需要搭配第三方的负载均衡器来完成。当安装 Ingress 控制器时，会创建一个类型为 LoadBalancer 的 Service。新创建的 Service 的 EXTERNAL-IP 状态是 pending，假如没有负载均衡器的话，会一直处于 pending 状态：
```
NAME                          TYPE           CLUSTER-IP      EXTERNAL-IP   PORT(S)        AGE
fsm-ingress-pipy-controller   LoadBalancer   10.43.188.208   <pending>     80:30508/TCP   8s
```
在 Demo 中用的是 K3s（默认提供了负载均衡器），这个 Service 会获得 EXTERNAL-IP，192.168.1.12和 192.168.1.13 是集群中两个节点的 IP，当使用该 IP 地址访问时，流量会进入到 Ingress 控制器的 pod，然后路由到配置对应的 pod 中。
```
NAME                          TYPE           CLUSTER-IP      EXTERNAL-IP   PORT(S) 
fsm-ingress-pipy-controller   LoadBalancer   10.43.188.208   192.168.1.12,192.168.1.13   80:30508/TCP   47s
```
如何实现的呢？<br />K3s 服务端的 controller 运行时会监听集群中 Service 的变更，如果 Service 类型是 LoadBalancer 便为其创建一个 Daemonset ，传入 Service 的 ClusterIP、Protocol、Port 等信息。剩下的工作就由这个 Daemonset 来完成了。<br />而这个 Daemonset 使用的是 klipper-lb 的镜像。
<a name="KmfaK"></a>
## Klipper LB
klipper-lb 是 K3s 的一个开源负载均衡器的实现，实现很简单：使用主机端口（`pod.spec.containers.ports.hostPort` 与 Service 的端口号相同）来接收流量，并使用 iptables 将接收的流量转发到 Service的 ClusterIP。<br />Pod 启动时使用通过环境变量注入的 Service 的信息，创建 iptables 的 NAT 规则。<br />![klipper-lb](https://cdn.nlark.com/yuque/0/2022/png/396745/1664811582457-3a4f3479-3d28-4e36-af5a-565ebf15b189.png#clientId=u1fd34490-b207-4&from=paste&id=u88017dd7&originHeight=682&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucdd44691-fba0-4789-a04f-f1934f61460&title=klipper-lb "klipper-lb")<br />代码量很少：
```bash
#!/bin/sh
set -e -x

trap exit TERM INT

for dest_ip in ${DEST_IPS}
do
    if echo ${dest_ip} | grep -Eq ":" 
    then
        if [ `cat /proc/sys/net/ipv6/conf/all/forwarding` != 1 ]; then
            exit 1
        fi
        ip6tables -t nat -I PREROUTING ! -s ${dest_ip}/128 -p ${DEST_PROTO} --dport ${SRC_PORT} -j DNAT --to [${dest_ip}]:${DEST_PORT}
        ip6tables -t nat -I POSTROUTING -d ${dest_ip}/128 -p ${DEST_PROTO} -j MASQUERADE
    else
        if [ `cat /proc/sys/net/ipv4/ip_forward` != 1 ]; then
            exit 1
        fi
        iptables -t nat -I PREROUTING ! -s ${dest_ip}/32 -p ${DEST_PROTO} --dport ${SRC_PORT} -j DNAT --to ${dest_ip}:${DEST_PORT}
        iptables -t nat -I POSTROUTING -d ${dest_ip}/32 -p ${DEST_PROTO} -j MASQUERADE
    fi
done

if [ ! -e /pause ]; then
    mkfifo /pause
fi
</pause
```
那除了 klipper-lb 以外，介绍下另一种负载均衡器 metallb。
<a name="AZTWt"></a>
## Metal LB
MetalLB 是裸机 Kubernetes 集群的负载均衡器实现，使用标准路由协议。<br />**注意：** MetalLB 目前还是 beta 阶段。

- 在 Kubernetes 集群中使用 MetalLB 作为 LoadBalancer（上）- Layer2
- 在 Kubernetes 集群中使用 MetalLB 作为 LoadBalancer（下）- BGP

关于 Metal LB，在上面的文章介绍过。其有两种工作模式，这里通过 BGP 的工作模式来进行说明。<br />与 klipper-lb 不同的时，需要预先配置好可用的 IP 地址段。当创建 LoadBalancer 类型的 Service时，metallb 会从地址池中为其分配 IP 地址。<br />以 Daemonset 方式运行在各个节点的 speaker 组件会监听 Service 的变更，如果该 Service 已经分配了 IP 地址（`service.status.loadBalancer.ingress`），speaker 在完成一些列的校验工作后会向路由器声明该地址。<br />路由接收到 speaker 的广播后，会更新路由表：使用 speaker 的节点进行该 IP 的路由。<br />当请求该 IP 地址时，因为无法找到 IP 地址对应的 MAC 地址（没有 ARP 广播），便将请求转给路由器。路由器根据路由表中的记录进行路由，转发到对应的节点上。后续的流程，就是 iptables/kubeproxy 的工作了。<br />![metal-lb-bgp](https://cdn.nlark.com/yuque/0/2022/png/396745/1664811582488-57f432a8-addc-40a0-a76a-9ab731b12e60.png#clientId=u1fd34490-b207-4&from=paste&id=u9bd2fb6c&originHeight=518&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0127dd8f-d76e-4d80-a5a6-53972b969ce&title=metal-lb-bgp "metal-lb-bgp")<br />可能你也看出来，如果目标 pod 不在该节点上 ，还会再有一次转发。这种情况可以通过设置 Service 的 `externalTrafficPolicy` 来避免。<br />如果 `service.sepc.externalTrafficPolicy` 设置为 `Local`，只有 Service 的 Endpoints 所在的节点，才会参与广播和路由；否则，所有的节点都会参与广播和路由。
