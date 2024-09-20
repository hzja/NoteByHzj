<a name="plA8P"></a>
### 报错信息
```
Failed to execute goal org.apache.maven.plugins:maven-deploy-plugin:2.7:deploy (default-deploy) on project admin: Deployment failed: repository element was not specified in the POM inside distributionManagement element or in -DaltDeploymentRepository=id::layout::url parameter -> [Help 1]
```
<a name="Wkz4A"></a>
### 基础配置
[语雀内容](https://www.yuque.com/fcant/operation/sbtsz5?inner=HdudB&view=doc_embed)
<a name="CEbUc"></a>
### 解决办法
多模块项目中需要在每个模块配置`<distributionManagement>`
