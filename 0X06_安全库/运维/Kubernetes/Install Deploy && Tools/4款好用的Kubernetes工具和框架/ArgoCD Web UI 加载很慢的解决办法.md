KubernetesArgoCD<br />Argo CD 是用于 Kubernetes 的强大 GitOps 持续交付工具。<br />[https://argo-cd.readthedocs.io/en/stable/](https://argo-cd.readthedocs.io/en/stable/)<br />但是，在管理大量应用程序时， ArgoCD Web UI 非常慢。怎样才能加快 Web UI 的速度？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657376740528-f59c0fea-64c4-477a-9771-7f4c41eca2df.png#clientId=u20a32e7d-af82-4&from=paste&id=ub39a3bea&originHeight=464&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u97a0551c-296f-4b81-b33d-284a5d18683&title=)<br />阿尔戈用户界面
<a name="qy4ZL"></a>
## 启用 Gzip 压缩
加速它的最简单方法是压缩它的所有资源。在 argocd-server 上启用 Gzip 压缩是实现这一目标的最简单和最有效的方法。<br />argocd-server 是一个 gRPC/REST 服务器，它公开了 Web UI 使用的 API。可以通过启用 GZIP 压缩`--enable-gzip`选项进行配置
```
argocd-server [flags]
    --enable-gzip       Enable GZIP compression
```
如果您的 ArgoCD 是使用社区维护的 Argo CD Chart 安装的，[https://github.com/argoproj/argo-helm/tree/main/charts/argo-cd](https://github.com/argoproj/argo-helm/tree/main/charts/argo-cd)<br />请使用以下配置启用 Gzip 压缩。
```yaml
server:
  extraArgs:
  - --enable-gzip
```
<a name="BKWu3"></a>
## 总结
**启用 Gzip 压缩以加速 ArgoCD Web UI。gzip 压缩将显着减少 UI 加载的数据量。**
<a name="Yw7Jc"></a>
## 参考
argo-cd PR 壮举：在 api 服务器中支持 gzip 压缩 #4218：[https://github.com/argoproj/argo-cd/pull/4218](https://github.com/argoproj/argo-cd/pull/4218)<br />argocd server 选项：[https://argo-cd.readthedocs.io/en/stable/operator-manual/server-commands/argocd-server/](https://argo-cd.readthedocs.io/en/stable/operator-manual/server-commands/argocd-server/)
