Kubernetes<br />Popeye 是一种实用程序，可扫描实时 Kubernetes 集群并报告已部署资源和配置的潜在问题。它根据部署的内容为集群消毒。通过扫描集群，它可以检测错误配置并确保最佳实践到位，从而防止未来出现问题。它旨在减少在运行 Kubernetes 集群时面临的认知过载。此外，如果集群使用metric-server，它会报告潜在的资源分配过多/不足，并在集群容量不足时尝试警告。<br />Popeye 是一个只读工具，它不会以任何方式改变任何 Kubernetes 资源！Popeye 会扫描集群以寻找最佳实践和潜在问题。目前，Popeye 只查看节点、命名空间、pod 和服务。消毒器的目的是发现错误配置，即端口不匹配、无效或未使用的资源、指标利用率、探针、容器映像、RBAC 规则、裸露资源等。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640583357968-2752996a-1ee0-45e7-866c-c9c76e6040b4.webp#clientId=u3bc113fb-8a42-4&from=paste&id=u0f8e7b20&originHeight=484&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8874f48e-22b7-42bd-9848-df5090a26cc&title=)
<a name="yRQ7Y"></a>
## 安装
Popeye 适合 Kubernetes 1.13+，可以将 Popeye 容器化并作为一次性或 CronJob 直接在 Kubernetes 集群中运行。它可在 Linux、MacOS 和 Windows 平台上使用。<br />下面在linux演示Popeye的使用：
```bash
wget https://github.com/derailed/popeye/releases/download/v0.9.8/popeye_Linux_x86_64.tar.gztar -zxf popeye_Linux_x86_64.tar.gz
tar -zxf popeye_Linux_x86_64.tar.gz
```
<a name="Xzcvt"></a>
## 检查版本
```bash
# ./popeye version
 ___     ___ _____   _____                       K          .-'-.     
| _ \___| _ \ __\ \ / / __|                       8     __|      `\  
|  _/ _ \  _/ _| \ V /| _|                         s   `-,-`--._   `\
|_| \___/_| |___| |_| |___|                       []  .->'  a     `|-'
  Biffs`em and Buffs`em!                            `=/ (__/_       /  
                                                      \_,    `    _)  
                                                         `----;  |     
Version:   0.9.8
Commit:    6db27f04407b337f6743faf4f382a61991aa5f31
Date:      2021-11-02T21:26:28Z
Logs:
```
<a name="fhrjQ"></a>
## 执行扫描，默认扫描所有namespace
```bash
# ./popeye
# ....
STATEFULSETS (5 SCANNED)                                                       💥 2 😱 3 🔊 0 ✅ 0 0٪
┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅
  · default/dubbo-monitor..........................................................................💥
    😱 [POP-500] Zero scale detected.
    🐳 dubbo-monitor
      💥 [POP-100] Untagged docker image in use.
      😱 [POP-106] No resources requests/limits defined.
  · default/mongodb................................................................................😱
    🐳 mongodb
      😱 [POP-106] No resources requests/limits defined.
  · default/redis..................................................................................💥
    🐳 init-redis
      💥 [POP-100] Untagged docker image in use.
      😱 [POP-106] No resources requests/limits defined.
    🐳 redis
      😱 [POP-106] No resources requests/limits defined.
  · default/redis-test.............................................................................😱
    😱 [POP-500] Zero scale detected.
    🐳 redis
      😱 [POP-106] No resources requests/limits defined.
  · default/zookeeper-one..........................................................................😱
    🐳 zookeeper-one
      😱 [POP-106] No resources requests/limits defined.


SUMMARY
┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅
Your cluster score: 74 -- C
```
可以看到当前集群评分为74：C<br />可以用`-n`参数指定namespace。<br />输出格式默认输出到控制台，也可以输出json、html、yaml等格式，可以将输出内容push到第三方服务，S3桶。<br />输出html格式到文件：
```bash
./popeye --out html --save  --output-file log.html
/tmp/popeye/log.html
```
然后在浏览器中打开。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640595480130-2960b6c0-870c-4345-8412-ece3a98e63da.png#clientId=u11e963e3-67d8-4&from=paste&height=594&id=ueb622fa9&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=339584&status=done&style=none&taskId=u8fbf2258-1415-4cba-a8c4-92794db09a3&title=&width=1280)<br />了解Popeye更多信息，请移步：[https://github.com/derailed/popeye](https://github.com/derailed/popeye)
