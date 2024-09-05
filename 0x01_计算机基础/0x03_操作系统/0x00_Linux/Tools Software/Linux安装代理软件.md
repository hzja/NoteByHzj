在Linux操作系统中，会因为网络原因导致下载失败，需要调节好网络进行操作。推荐使用Proton VPN。但是这里由于经济原因，使用ProxyChains代理软件走宿主机的代理进行使用。安装命令如下：
```bash
sudo apt install tor
sudo apt install proxychains4
```
配置proxychains4，首先安装vim编辑器
```bash
sudo apt install-y vim
```
打开proxychains4配置文件
```bash
sudo vim/etc/proxychains4.conf
```
将宿主机的IP按照如下格式添加到配置文件的最后一行，注释掉socks4<br />![DM_20230722144149_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1690008600139-78cbd5f1-6081-4357-a1ee-d626c5e98f4a.png#averageHue=%23300a24&clientId=u198abcb3-d1f2-4&from=ui&id=ub01a237f&originHeight=486&originWidth=1081&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1579212&status=done&style=none&taskId=u916fcb96-4453-4951-89f1-5028e2fd8fd&title=)
