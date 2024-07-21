<a name="DwvYt"></a>
## 1、查看镜像
```bash
➜ ~ docker images
REPOSITORY TAG IMAGE ID CREATED SIZE
springboot 1.0.0 fab5182e4cc3 About a minute ago 531MB
registry.cn-hangzhou.aliyuncs.com/xx/springboot 1.0.0 fab5182e4cc3 About a minute ago 531MB
```
这里有两个镜像，镜像ID均一致。(第二个镜像是根据第一个镜像，进行docker tag而来的)。
<a name="MaWSh"></a>
## 2、删除镜像
```bash
➜ ~ docker rmi fab5182e4cc3
Error response from daemon: conflict: unable to delete fab5182e4cc3 (must be forced) - image is referenced in multiple repositories
```
报错信息image is referenced in multiple repositories。 问题看下来应该是有镜像ID指向了两个responsitory，所以没法删除
<a name="vbfrG"></a>
## 3、用repository和tag的方式来删除
```bash
➜ ~ docker rmi springboot:1.0.0
Untagged: springboot:1.0.0
```
再次查看镜像，发现springboot那个镜像已经被删除了
```bash
➜ ~ docker images
REPOSITORY TAG IMAGE ID CREATED SIZE
registry.cn-hangzhou.aliyuncs.com/xx/springboot 1.0.0 fab5182e4cc3 About an hour ago 531MB
```
还剩一个镜像了，可以使用docker rmi 镜像ID的方式删除了
