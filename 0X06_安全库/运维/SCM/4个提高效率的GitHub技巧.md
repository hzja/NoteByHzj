Git Github<br />如果将代码托管在 GitHub 上，那么可以充分利用 GitHub Actions。借助 GitHub Actions，可以完成代码测试和检查，无需手动运行这些任务。<br />[https://github.com/features/actions](https://github.com/features/actions)
<a name="eyo4Z"></a>
## 1、利用 GitHub Actions 审计网页
这一操作集成了谷歌提供的实用网页审计工具 Lighthouse，可以检测性能、可访问性、最佳实践、搜索引擎优化和渐进式 Web 应用程序。<br />[https://developers.google.com/web/tools/lighthouse/](https://developers.google.com/web/tools/lighthouse/)<br />![2021-08-04-18-22-51-578395.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628072706039-3c8df367-3b54-4401-b12e-fc598d48749e.png#clientId=ud978da0e-64fe-4&from=ui&id=u327b361d&originHeight=405&originWidth=1080&originalType=binary&ratio=1&size=122878&status=done&style=shadow&taskId=ub838db1e-15e3-406b-9f0b-0d81798b361)<br />Github Lighthouse Action<br />目前，该操作会打印出 5 个分数（满分 100），并上传 HTML 和 JSON 版本的报告。<br />在下一个版本中，该操作将允许指定每项测试的阈值，如果不满足条件，可以有选择性地停止这个步骤。<br />![2021-08-04-18-22-51-851440.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628072721351-8a6529ec-2c26-4894-b8f0-5d2d6edc98b7.png#clientId=ud978da0e-64fe-4&from=ui&id=u3d78e861&originHeight=684&originWidth=1080&originalType=binary&ratio=1&size=159949&status=done&style=shadow&taskId=u4ee7ce24-c32c-48b1-ba19-811bae40cdf)<br />Lighthouse 审计报告示例
<a name="hcaR8"></a>
### 用法
下面的工作流在 jarv.is 上运行一个 Lighthouse 审计，该步骤会在输出中显示 5 个分数，并上传结果 .html 和 .json的版本，以供下载（如上所示）。<br />[https://jarv.is/](https://jarv.is/)<br />workflow.yml文件：
```yaml
name: Audit live site
on: pushjobs:
  audit:
    runs-on: ubuntu-latest
    steps:
    - name: Audit live URL
      uses: jakejarvis/lighthouse-action@master
      with:
        url: 'https://jarv.is/'
    - name: Upload results as an artifact
      uses: actions/upload-artifact@master
      with:
        name: report
        path: './report'
```
Lighthouse 在构建渐进式 Web 应用时特别有用。该项目的灵感来自 GoogleChromeLabs/lighthousebot。<br />[https://medium.com/better-programming/everything-you-need-to-know-about-pwas-8e41a7e745aa](https://medium.com/better-programming/everything-you-need-to-know-about-pwas-8e41a7e745aa)[https://github.com/GoogleChromeLabs/lighthousebot](https://github.com/GoogleChromeLabs/lighthousebot)
<a name="TCDp2"></a>
## 2、利用 GitHub Actions 运行 SSH 命令
该操作将通过 SSH 在 $HOST 上将提供的参数作为命令运行。如果想在每次提交或推送之后在自己的私有服务器上运行命令，那么它会非常有用。<br />![2021-08-04-18-22-51-976478.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628072772659-acfbeb1a-0d6b-4d3e-a9c2-1c2607b3f210.png#clientId=ud978da0e-64fe-4&from=ui&id=u16332aa7&originHeight=534&originWidth=606&originalType=binary&ratio=1&size=46711&status=done&style=none&taskId=u716c3dcf-ef7e-4b1d-82e1-69613294398)<br />SSH GitHub Action
<a name="wIgQ0"></a>
### 用法
要使用这个操作，只需要在.github/main.workflow文件中添加以下几行：
```javascript
action "Run deploy script" {
  uses = "maddox/actions/ssh@master"
  args = "/opt/deploy/run"
  secrets = [
    "PRIVATE_KEY",
    "HOST",
    "USER"
  ]
}
```
<a name="Ix9Ii"></a>
### 所需的参数
所使用的参数就是要通过 SSH 在服务器上运行的命令。
<a name="M17gK"></a>
### 示例

- `args = "/opt/deploy/run"`
- `args = "touch ~/.reload"`
<a name="c4uUD"></a>
### 所需的私密信息
要使用这项操作，需要提供以下私密信息：

- PRIVATE_KEY：SSH 私钥；
- HOST：该操作将通过 SSH 连接并运行命令的主机，如your.site.com；
- USER：SSH 命令将其和私钥一起用于身份验证的用户。

要了解更多细节，请查看 GitHub 库。<br />[https://github.com/maddox/actions/tree/master/ssh](https://github.com/maddox/actions/tree/master/ssh)
<a name="la8Vf"></a>
## 3、利用 GitHub Actions 检测密钥泄漏
将 gitleaks 作为一个 GitHub Action，用于审计 Git 提交中的秘密。如果使用.env文件，该操作会在无意中发布了私密信息时通知你。<br />[https://github.com/zricethezav/gitleaks](https://github.com/zricethezav/gitleaks)<br />![2021-08-04-18-22-52-195394.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628072759122-d7662fb1-0d1e-4acf-9ac9-e4fc4e376ff4.png#clientId=ud978da0e-64fe-4&from=ui&id=ue7a0c1d1&originHeight=414&originWidth=1080&originalType=binary&ratio=1&size=104123&status=done&style=shadow&taskId=u34550bd4-3ec8-4df7-a6be-57adf3b042a)<br />Gitleaks-action
<a name="BaMF6"></a>
### 用法
```javascript

workflow "gitleaks my commits" {
  on = "push"
  resolves = ["gitleaks"]
}action "gitleaks" {
  uses = "eshork/gitleaks-action@master"
}
```
要了解更多信息，请移步 zricethezav/gitleaks。<br />[https://github.com/zricethezav/gitleaks](https://github.com/zricethezav/gitleaks)
<a name="S1KX3"></a>
## 4、利用 GitHub Action 运行 ESLint
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1628072557715-abb3ad3d-50eb-4f1b-bf17-fdd600bed3d1.png#clientId=ud978da0e-64fe-4&from=paste&id=u82f58cb4&originHeight=378&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ud64ddc7c-984e-4bcc-ae9e-d5fb669918d)Eslint Action<br />该操作在指定的 JavaScript 文件上执行 ESLint 代码检查工具，而不需要任何前期的操作 / 构建步骤或 Docker。<br />[https://eslint.org/](https://eslint.org/)<br />要执行操作，本地必须运行 ESLint。它将使用与本地相同的规则。要了解更多信息，请查看 ESLint 入门指南。<br />[https://eslint.org/docs/user-guide/getting-started#installation-and-usage](https://eslint.org/docs/user-guide/getting-started#installation-and-usage)
<a name="VnyZS"></a>
### 用法
将下面的任何一个例子添加到文件.github/main.workflow。<br />下面是一个使用该操作的示例：
```javascript
workflow "New workflow" {
  on = "push"
  resolves = ["ESLint"]
}action "ESLint" {
  uses = "stefanoeb/eslint-action@master"
}
```
在默认情况下，它会对项目中的所有文件运行 ESLint。但是，可以使用args 指定要检查的文件，如下所示：
```javascript
workflow "New workflow" {
  on = "push"
  resolves = ["ESLint"]
}action "ESLint" {
  uses = "stefanoeb/eslint-action@master"
  args = "index.js src/**.js"
}
```
如果之前没有安装必要的模块，那么该操作会自动运行`yarn install` 或 `npm install` 。
