使用一个开源工具 [**Documate**](https://github.com/AirCodeLabs/documate) 快速让 [**VitePress**](https://vitepress.dev/) 文档站拥有 AI 对话能力，基于文档内容来解答用户问题。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1697462928399-52ece76b-d7f3-4ab8-873f-01192e1948de.gif#averageHue=%23fdfdfd&clientId=u9e19cb1d-2ebb-4&from=paste&id=uf42eeaa3&originHeight=615&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf6a095ea-79ed-4ff9-b38c-2c5dcfb8d53&title=)<br />通过简单的配置，就可以将能力接入到网站之中。有了大模型的加持，用户上来几乎可以不读文档，直接与 AI 对话就可以完成自己的需求，对技术类产品文档来说，价值很高。支持流式输出，回答速度很快，并且开源，随时可以贡献和调整代码。
<a name="IM3HG"></a>
## 技术原理
仔细阅读了这个项目的源码，整体实现还是比较简单和清晰的。它由三部分构成：<br />第一部分：一个将文档提交到数据库的 CLI 工具，只需要在项目中配置下 documate.json 就行了。<br />document.json配置文件非常简单，只有三个配置项：

- root 描述使用项目的哪个目录下的文件生成内容，默认是根目录。
- include 描述哪些是需要处理的文档文件，默认是 Markdown 文本。
- backend 指定上传文档内容到后端保存的接口，OpenAI 根据这些内容来提供回答。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928427-b7753a1c-de68-4ddb-a65a-8b801a27cfcc.png#averageHue=%23f7f7f7&clientId=u9e19cb1d-2ebb-4&from=paste&id=u69339302&originHeight=357&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uded77a9e-934d-4bf2-8716-9ca678220fa&title=)<br />第二部分：一个封装好的拿来即用的问答 UI 组件，直接 import 组件就可以使用，目前提供了 Vue 组件以及不依赖任何框架的原生 JavaScript 的 UI。看 issue 规划，React 版本社区已经在支持中了，相信很快就会发布。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928459-54b79569-eb97-46ac-98be-f0276dfe2968.png#averageHue=%23f4f2ef&clientId=u9e19cb1d-2ebb-4&from=paste&id=uda0bc09a&originHeight=496&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae967c28-d4eb-4a6a-8312-12decfab8e4&title=)<br />第三部分：一个提供问答服务、可一键完成部署的 AI Ask Server，可以在 AirCode 直接完成部署，也可以在自己的服务器上部署。<br />问答服务有了，UI 组件有了，数据也有了，那就可以尝试着玩耍了，整个配置过程大概可以在 15min 内完成。
<a name="JEBx7"></a>
## 如何接入
如果想创建一个全新的 VitePress 项目并包含 AI 对话能力，可以使用下面的命令：
```bash
npm create documate@latest --template vitepress
```
创建完成后可直接跳到第 3 步「构建上传和搜索后端 API」继续配置。<br />如果要给已有的 VitePress 项目添加 AI 对话能力，则按照以下步骤进行。
<a name="Raq5T"></a>
### 1、初始化
在 VitePress 项目根目录下使用以下命令进行初始化：
```bash
npx @documate/documate init --framework vue
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928410-b5084060-f748-4029-b429-e77c38cc33a0.png#averageHue=%230a0a0a&clientId=u9e19cb1d-2ebb-4&from=paste&id=u68939405&originHeight=358&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub7a050c0-735a-4f11-bc98-c043a04276d&title=)<br />该命令会创建一个 documate.json 配置文件。
```json
{
  "root": ".",
  "include": [
    "**/*.md"
  ],
  "backend": ""
}
```
并且添加了一个 documate:upload 命令用于上传文档生成知识库，后面会介绍到具体用法。
```json
{
  "scripts": {
    "docs:dev": "vitepress dev",
    "docs:build": "vitepress build",
    "docs:preview": "vitepress preview",
    "documate:upload": "documate upload"
  },
  "dependencies": {
    "@documate/vue": "^0.2.3"
  },
  "devDependencies": {
    "@documate/documate": "^0.1.0"
  }
}
```
<a name="KMWFf"></a>
### 2、给项目添加 UI 入口
在文件 .vitepress/theme/index.js 中添加如下代码，如果没有则需要先手动创建这个文件。VitePress 在 [**Extending the Default Theme**](https://vitepress.dev/guide/extending-default-theme) 文档中介绍了如何定制自己的主题。
```javascript
import { h } from 'vue'
import DefaultTheme from 'vitepress/theme'

// Load component and style
import Documate from '@documate/vue'
import '@documate/vue/dist/style.css'

export default {
  ...DefaultTheme,
  // Add Documate UI to the slot
  Layout: h(DefaultTheme.Layout, null, {
    'nav-bar-content-before': () => h(Documate, {
      endpoint: '',
    }),
  }),
}
```
上面的代码会在导航栏中添加一个 AI 对话框的 UI。在本地使用 `npm run docs:dev` 启动服务后，可以在左上角找到 Ask AI 的按钮。如果没有看到 Ask AI 按钮，可以检查下上面的代码是否都正确添加，并确保从 @documate/vue/dist/style.css 导入了 CSS 样式文件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928394-0463860d-8b23-4149-a878-5673b8a1cab6.png#averageHue=%23f4c04e&clientId=u9e19cb1d-2ebb-4&from=paste&id=ue64745c6&originHeight=621&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9f0992f1-7089-4b08-8ef0-ab4229cbb31&title=)<br />至此，已经完成了 UI 的接入，接下来为对话框添加回答问题的接口能力。
<a name="LPPS4"></a>
### 3、构建上传和搜索后端 API
Documate 的后端代码用于上传文档内容生成知识库，以及接收用户的问题并返回流式的回答。<br />进入 [**GitHub 中的 backend 文件夹**](https://github.com/AirCodeLabs/documate/tree/main/backend)，点击其中的「Deploy to AirCode」，快速复制并部署一份自己的后端代码。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928765-a386201f-7f38-4b55-8219-eae36f4eaaa8.png#averageHue=%23fdfdfd&clientId=u9e19cb1d-2ebb-4&from=paste&id=u6203e763&originHeight=274&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua16daacb-90fb-4be1-a4a5-a213974bf39&title=)<br />如果是第一次使用 AirCode（一个在线编写和部署 Node.js 应用的平台），会被重定向到登录页面。建议选择 GitHub 登录，会更快一些。<br />创建应用之后，在 Envrionments 标签页中设置 OPENAI_API_KEY 环境变量为 OpenAI Key 值。可以在 [**OpenAI 控制台**](https://platform.openai.com/account/api-keys) 中获取到 API Key。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928825-e7583fb4-082f-4d10-92b7-402349e1a966.png#averageHue=%23fefefe&clientId=u9e19cb1d-2ebb-4&from=paste&id=ued4d409b&originHeight=292&originWidth=916&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1367287e-9459-4083-b019-5dddedf4854&title=)<br />点击顶部栏上的「Deploy」按钮，将所有的函数部署到线上，部署成功后可以得到每个函数的调用 URL。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928820-b15ef244-050d-4505-8c18-55031ceb4883.png#averageHue=%23b6b6b5&clientId=u9e19cb1d-2ebb-4&from=paste&id=u27a988e5&originHeight=686&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u894ea73b-aa74-46ff-82bd-21df7b36054&title=)<br />这里会使用到 upload.js 和 ask.js两个函数，一个用于文档内容上传，另一个用于回答问题。
<a name="UUj5f"></a>
### 4、设置 API 接口
在 AirCode Dashboard 中，选择部署后的 upload.js 文件，复制 URL 并添加到 documate.json 的 backend 字段中：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928891-a32ca4ae-709e-4153-b43c-09ede9ff4b4d.png#averageHue=%23fdfcfc&clientId=u9e19cb1d-2ebb-4&from=paste&id=u589c360f&originHeight=564&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udcbafe89-2b8b-4f37-a96f-4b4fb7b4893&title=)
```json
// documate.json
{
  "root": ".",
  "include": [ "**/*.md" ],
  "backend": "替换为你的 upload.js 的 URL"
}
```
同上操作，在 AirCode 中选择已部署的 ask.js 文件，复制调用 URL，修改 .vitepress/theme/index.js 文件 endpoint 值。
```javascript
// .vitepress/theme/index.js
import { h } from 'vue'
import DefaultTheme from 'vitepress/theme'
import Documate from '@documate/vue'
import '@documate/vue/dist/style.css'

export default {
  ...DefaultTheme,
  Layout: h(DefaultTheme.Layout, null, {
    'nav-bar-content-before': () => h(Documate, {
        // Replace the URL with your own one
        endpoint: '替换为你的 ask.js 的 URL',
      },
    ),
  }),
}
```
<a name="vem0w"></a>
### 5、运行项目
通过下面的命令将内容上传到后端，生成文档知识库：
```bash
npm run documate:upload
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697462928983-c92a7b0e-8ddb-444e-8ca7-c7edc1c93dd7.png#averageHue=%23060606&clientId=u9e19cb1d-2ebb-4&from=paste&id=uce2d40eb&originHeight=451&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua60ece10-04aa-448d-b4c2-d90595f6521&title=)<br />命令完成后，本地启动项目，点击左上角 Ask AI 按钮，在弹出对话框后输入问题就可以得到基于文档内容的回答。
```bash
npm run docs:dev
```
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1697462929136-a7900e29-c2f6-41c7-bad3-be19b2778e18.gif#averageHue=%23fdfdfd&clientId=u9e19cb1d-2ebb-4&from=paste&id=uf4e40a42&originHeight=615&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74e98c81-6aa8-4846-a7eb-c9952064e22&title=)<br />更多的使用和配置方式可以参考 [**Documate 项目的 GitHub**](https://github.com/AirCodeLabs/documate)。
<a name="brtCU"></a>
## 参考资料
Documate: [_https://github.com/AirCodeLabs/documate_](https://github.com/AirCodeLabs/documate)<br />VitePress: [_https://vitepress.dev/_](https://vitepress.dev/)<br />Extending the Default Theme: [_https://vitepress.dev/guide/extending-default-theme_](https://vitepress.dev/guide/extending-default-theme)<br />GitHub 中的 backend 文件夹: [_https://github.com/AirCodeLabs/documate/tree/main/backend_](https://github.com/AirCodeLabs/documate/tree/main/backend)<br />OpenAI 控制台: [_https://platform.openai.com/account/api-keys_](https://platform.openai.com/account/api-keys)<br />Documate 项目的 GitHub: [_https://github.com/AirCodeLabs/documate_](https://github.com/AirCodeLabs/documate)
