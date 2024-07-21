Docker
<a name="vq22B"></a>
## 多阶段构建DockerFile
```dockerfile
FROM node:16-alpine3.11 as builder
WORKDIR '/build'
COPY myaccount ./myaccount
COPY resources ./resources
COPY third_party ./third_party

WORKDIR '/build/myaccount'

RUN npm install
RUN npm rebuild node-sass
RUN npm run build

RUN ls /build/myaccount/dist

FROM nginx
EXPOSE 80
COPY --from=builder /build/myaccount/dist /usr/share/nginx/html
```
<a name="oFI4X"></a>
## `multi-stage build` 的支持
在使用docker 进行多阶段构建镜像时，有如下报错；查看资料是docker 17.05 才开始支持多阶段构建。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631846657760-40ad42e5-3a6d-4889-9bda-f74bb33a917a.png#clientId=u4c42db6a-d943-4&from=paste&height=108&id=uc8e0422a&originHeight=324&originWidth=3323&originalType=binary&ratio=1&size=323584&status=done&style=none&taskId=u3b0858ef-d4cf-4ec4-b4ab-7af1ee692c2&width=1107.6666666666667)
```bash
ERROR: Service 'myaccount-service' failed to build : Error parsing reference: "node:16-alpine3.11 as builder" is not a valid repository/tag: invalid reference format
```
<a name="wK3oH"></a>
## 解决
<a name="ouHoX"></a>
### 卸载低版本Docker
```bash
yum remove docker
yum remove docker-common
yum remove docker-client
```
<a name="XebKX"></a>
### 安装最新版Docker
```bash
curl -fsSL https://get.docker.com/ | sh
systemctl restart docker
systemctl enable docker
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631845834654-b1e86a62-daa1-43ca-b161-cc3008dfff3b.png#clientId=u379e538a-dbaf-4&from=paste&height=593&id=u48a94710&originHeight=1780&originWidth=3323&originalType=binary&ratio=1&size=2084850&status=done&style=none&taskId=u55029d67-95c0-4932-b072-bc11e6bfa21&width=1107.6666666666667)
<a name="vEa7g"></a>
### 之后再执行构建命令发现可以成功执行
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631846748111-c7503579-e3b0-49fd-988d-51b104e50825.png#clientId=u4c42db6a-d943-4&from=paste&height=505&id=uff668226&originHeight=1516&originWidth=3323&originalType=binary&ratio=1&size=1785758&status=done&style=none&taskId=u44fb82ed-d65b-4bd0-9488-6333c505afb&width=1107.6666666666667)
