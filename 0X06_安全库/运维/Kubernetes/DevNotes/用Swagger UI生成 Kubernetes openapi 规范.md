Kubernetes<br />当在开发与 kubernetes API 服务器通信的软件时，实际上是正在寻找详细的 kubernetes API 规范。当然这里不是在谈论主要揭示核心对象模型的官方 kubernetes API 参考。可能会对特定的 API 路径、可能的标头、查询参数和响应感兴趣。<br />通常，可以在openapi规范文档中找到所有这些信息，可以通过Swagger UI、ReDoc或其他此类工具查看该文档。<br />[https://swagger.io/tools/swagger-ui/https://github.com/Redocly/redochttps://swagger.io/specification/](https://swagger.io/tools/swagger-ui/https://github.com/Redocly/redochttps://swagger.io/specification/)<br />所以下一个合乎逻辑的步骤是在谷歌上搜索“kubernetes swagger”或“kubernetes openapi spec”之类的东西，虽然其中一些搜索结果可以引导朝着正确的方向前进，但不会对正在寻找的 Swagger UI 感到满意。<br />原因是**由于自定义资源定义，每个 Kubernetes API 服务器的规范实际上是不同的，因此暴露了不同的路径和模型。**<br />这需要 kubernetes API 服务器实际生成它自己的 openapi 规范，将在下面执行此操作：<br />首先确保连接到 kubernetes API 服务器作为当前的 kube 上下文。可以通过 仔细检查`kubectl config current-context`。然后，可以为 kubernetes API 服务器打开一个反向代理：
```bash
kubectl proxy --port=8080
```
这节省了一堆现在全部由 kubectl 处理的身份验证配置。在新的终端窗口中运行以下命令以保持反向代理处于活动状态（或在后台运行反向代理）。通过以下命令为 kubernetes API Server 保存 Swagger 文件：
```bash
curl localhost:8080/openapi/v2 > k8s-swagger.json
```
作为最后一步，现在只需要使用生成的 Swagger json 作为输入来启动一个 Swagger  Server。将为此使用 docker 容器：
```bash
docker run \    --rm \    -p 80:8080 \    -e SWAGGER_JSON=/k8s-swagger.json \    -v $(pwd)/k8s-swagger.json:/k8s-swagger.json \    swaggerapi/swagger-ui
```
现在访问http://localhost，将获得一个 Swagger UI，其中包括安装在集群上的所有自定义资源的模型和路径！<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642174754322-fc4b5b73-3659-4c06-b09a-f08e02219f6c.webp#clientId=uf9858aae-0067-4&from=paste&id=u09698d84&originHeight=369&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf6c9d76e-5d06-4d09-8b3f-e4677bedd87&title=)
