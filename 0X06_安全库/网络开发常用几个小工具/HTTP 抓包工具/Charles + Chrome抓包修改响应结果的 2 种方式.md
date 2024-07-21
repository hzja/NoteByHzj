抓包Charles<br />在工作中，经常需要针对某个接口修改其响应值，以此改变 App 或浏览器实际的渲染结果<br />这里以 Charles + Chrome 浏览器为例，讲解修改响应结果常用的 2 种方式
<a name="GvlHA"></a>
## 1、断点 Breakpoints
这种方式很简单，具体操作步骤如下<br />具体步骤如下：
<a name="oUvH3"></a>
### 1-1  打开 Charles 并选中 Windows Proxy
Windows Proxy 相当于开启整个系统通过 Charles 作为代理，所以能正常抓取浏览器的数据包<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028417-58d3f578-2cc9-42f6-9ca9-b495afc8efad.png#clientId=uf7827c94-8f87-4&from=paste&id=u4bf2939a&originHeight=1072&originWidth=914&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0016e793-87a9-451b-9a56-d3964bb7b00&title=)
<a name="p0DZA"></a>
### 1-2  抓包并对目标接口开启断点
正常利用 Charles 进行抓包，选中目标接口（以登录接口为例），右键开启断点<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028425-dd08b227-04c6-4e47-a804-e978a9ba3d4a.png#clientId=uf7827c94-8f87-4&from=paste&id=ufb3da6ec&originHeight=1089&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubbb0462a-e5c7-48c6-8ddb-d59d839f277&title=)
<a name="ITY1e"></a>
### 1-3  再次调用接口，修改响应值
再次抓取到登录接口时，Charles 会在请求、响应时断点展示，可以对「 **请求头、请求体、响应头、响应体** 」进行二次编辑<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028412-f709fc0a-48a4-4351-b46e-19c044a99ac3.png#clientId=uf7827c94-8f87-4&from=paste&id=u111c9c40&originHeight=620&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4cc7ad73-390a-4317-a2dc-4e70dc4711b&title=)<br />在编辑窗口修改响应结果后，点击右下角的「 **Execute ** 」按钮就可以将修改后的结果返回给前端进行渲染<br />这样，就完成了针对某个接口响应结果的修改
<a name="GyUu9"></a>
## 2、Map Local Settings
第一种方式虽然操作简单，但是如果接口设置的超时时间很短，在断点窗口中修改响应结果不一定来得及<br />Map Local Settings 相当于将本地文件数据作为响应值进行返回<br />具体操作步骤如下：
<a name="yTWI4"></a>
### 2-1 正常抓包并保存响应值并进行二次编辑
使用 Charles 正常抓包，右键选择登录接口将响应数据以 JSON 格式保存到本地，然后对响应结果文件进行二次编辑<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028409-9b177a9a-5482-44a0-8192-534dd24268a0.png#clientId=uf7827c94-8f87-4&from=paste&id=u1dc04b82&originHeight=464&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6563595-f6f7-49cd-b2ad-7140b99da60&title=)
<a name="q3RBJ"></a>
### 2-2 查看同接口OPTIONS 请求方式的响应头（可选）
为了避免 CORS 跨域异常，如果登录接口会同时发送一个 OPTIONS 请求，需要查看该请求的响应头<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028420-99667bb7-2e53-45bc-ba84-1f89ed243835.png#clientId=uf7827c94-8f87-4&from=paste&id=uc1de33d9&originHeight=834&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u00465e86-b6ad-41e7-bd3f-2f551c4f3e4&title=)<br />然后重写请求头，将上面 4 项追加到响应头中去（具体配置参考2.3）
<a name="LkvRB"></a>
### 2-3 Rewrite Settings（可选）
操作路径：Tools - Rewrite - Enable Rewrite<br />点击 Add 按钮，在右侧编辑区域添加对应的 Location 及 Rewrite Rule<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028809-cad2b0ea-1970-47bf-aa42-bbed9dd470b8.png#clientId=uf7827c94-8f87-4&from=paste&id=ue9e6e306&originHeight=475&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9740e49f-5370-4374-9421-711efe98456&title=)<br />需要注意的是，Rewrite Rule 追加响应体时，应该勾选上 Response 和 Replace first
<a name="NDPgr"></a>
### 2-5  配置 Map Local Settings 并开启
操作路径：Tools - Map Local - Enable Map Local<br />点击 Add 按钮，新增一个本地响应匹配信息链，将接口地址与本地响应文件的完整路径输入进去<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666438028778-b1320d94-85b4-473d-8e7c-138a4c0ee449.png#clientId=uf7827c94-8f87-4&from=paste&id=ud47c5d50&originHeight=526&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9f31b2d7-cf7c-4004-93a9-124e1b02a90&title=)
<a name="EzWjO"></a>
### 2-6 测试一下
经过上面配置后，重新调用登录接口，本地数据会作为响应值返回回去
