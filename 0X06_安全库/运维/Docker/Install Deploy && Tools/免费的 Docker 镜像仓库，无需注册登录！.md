Docker<br />CI 工作流将构建生成的镜像，推送到镜像仓库。dockerhub 是常用镜像存储仓库。一个构建步骤可以推送映像，而其他分布式步骤可以拉取。挑战在于大多数注册中心都需要身份验证才能推送和拉取。<br />ttl.sh 是一个免费使用、不需要注册登录、短暂、开源的 Docker 镜像仓库<br />[https://ttl.sh/ ](https://ttl.sh/ )<br />[https://github.com/replicatedhq/ttl.sh](https://github.com/replicatedhq/ttl.sh)
<a name="p3InA"></a>
### **如何使用**
下面通过将 busybox 重新打 tag 后 push 做示例：
```bash
# docker pull busybox
# docker tag busybox ttl.sh/busybox
# docker push  ttl.sh/busybox
# docker pull ttl.sh/busybox
```
镜像有时间限制，默认为 1 小时，最长为 24 小时（有效时间标签：5m, :1600s, :4h, :1d）<br />上面示例推送的镜像没有加时间后缀，所以默认保存 1 小时。<br />下面通过生成 uuid 的方式，并且增加 5 分钟的保留时间：
```bash
# IMAGE_NAME=$(uuidgen)
# docker tag busybox ttl.sh/${IMAGE_NAME}:5m
# docker push ttl.sh/${IMAGE_NAME}:5m
The push refers to repository [ttl.sh/ede2d864-ff1c-48d0-b0c3-2fd902833432]
```
拉取镜像速度取决于 Cloudflare。<br />CloudFlare 的 是一家美国公司，为各种产品提供内容交付网络 (CDN) 和 DNS 服务，以优化网站和应用程序的性能、可交付性和在线安全性。
<a name="ahIga"></a>
### **总结**
这对于开发环境使用会更好，不推荐用于生产，因为由于匿名，传上去的镜像，很可能被其他人篡改，如果直接运行，会有很大的安全隐患！<br />所以在 push 镜像时，通过uuidgen命令生成将随机的 UUID 添加到镜像名称中，来减少碰撞的几率，降低可发现性。
