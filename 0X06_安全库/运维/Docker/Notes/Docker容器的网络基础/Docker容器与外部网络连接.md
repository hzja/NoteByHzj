Docker 网络
<a name="UJ26N"></a>
## 1、查看网络数据转发是否开启
```bash
[root@iZuligp6e1dyzfZ ~]# sysctl net.ipv4.conf.all.forwarding
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894147253-57cad3a6-46d5-475e-ad2e-52720e5daf32.png#height=33&id=YyK64&originHeight=92&originWidth=1344&originalType=binary&size=15866&status=done&style=none&width=488.72727272727275)
<a name="kdDlz"></a>
## 2、查看系统外部网络连接设置
```
[root@iZuligp6e1dyzfZ ~]# iptables -t filter -L -n
[root@iZuligp6e1dyzfZ ~]# iptables -L -n
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894227317-cd9d6d30-23a7-457d-9b88-54c37151e492.png#height=485&id=osKN8&originHeight=1335&originWidth=2482&originalType=binary&size=658528&status=done&style=none&width=902.5454545454545)
<a name="AQNDB"></a>
## 3、允许端口映射访问
在启动镜像时使用-p配置端口映射,在系统网络过滤器iptables配置
```bash
[root@iZuligp6e1dyzfZ ~]# docker port zk01
[root@iZuligp6e1dyzfZ ~]# iptables -I DOCKER -s 10.211.55.3 -d 172.17.0.7 -p TCP --dport 80 -j DROP
[root@iZuligp6e1dyzfZ ~]# iptables -L -n
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894315964-bdd601fa-71d7-4343-b9d2-b145b05d02f5.png#height=478&id=FKD9L&originHeight=1315&originWidth=2449&originalType=binary&size=699175&status=done&style=none&width=890.5454545454545)
<a name="PYhHF"></a>
## 4、限制IP访问容器
