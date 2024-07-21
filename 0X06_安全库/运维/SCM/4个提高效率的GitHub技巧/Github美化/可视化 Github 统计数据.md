<a name="MxyiY"></a>
### 统计卡片生成
将这行代码复制到 markdown 文件中，更改 `?username=` 的值为你的 GitHub 用户名，即可生成 GitHub 统计卡片。
```markdown
[![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=anuraghazra)](https://github.com/anuraghazra/github-readme-stats)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867982056-cbb8c90f-52e2-4fd0-9ea0-fa2177bca0a9.png#averageHue=%23fefdfd&clientId=uca56e9e1-d449-4&from=paste&id=ub9da5571&originHeight=195&originWidth=450&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue4d83518-c486-44e1-839d-af6b7877577&title=)
<a name="iAbnS"></a>
### 更多配置项
<a name="uRjos"></a>
#### 隐藏指定统计
想要隐藏指定统计信息，可以调用参数 `?hide=`，其值用 , 分隔。<br />选项：`&hide=stars,commits,prs,issues,contribs`
```markdown
![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=anuraghazra&hide=contribs,prs)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867981928-d49f589c-f2d9-4a7c-8a5d-85bbff8588e6.png#averageHue=%23fefdfd&clientId=uca56e9e1-d449-4&from=paste&id=u30c66932&originHeight=150&originWidth=450&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u48224bc8-016c-45bd-b641-162d7a01cda&title=)
<a name="U1wD0"></a>
#### 将私人项目贡献添加到总提交计数中
可以使用参数 `?count_private=true` 把私人贡献计数添加到总提交计数中。
```markdown
![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=anuraghazra&count_private=true)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867982030-6328ce40-86b8-43c9-acf3-afd6ef0d9549.png#averageHue=%23fefdfd&clientId=uca56e9e1-d449-4&from=paste&id=ubc24e593&originHeight=195&originWidth=450&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua38a5468-9e60-4e13-82b0-74e9685d377&title=)
<a name="SepFA"></a>
#### 显示图标
如果想要显示图标，可以调用 `show_icons=true` 参数，像这样：
```markdown
![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=anuraghazra&show_icons=true)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867982038-0398d452-e3d6-4e4d-99fc-3ccc1e445592.png#averageHue=%23fefdfd&clientId=uca56e9e1-d449-4&from=paste&id=uffcb3fbf&originHeight=170&originWidth=467&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3ad97e77-a06f-4ec7-ba3f-0ea5299be2d&title=)
<a name="xFe57"></a>
### 主题
可以通过现有的主题进行卡片个性化，省去手动自定义的麻烦。通过调用 `?theme=THEME_NAME` 参数，像这样：<br />选项：dark, radical, merko, gruvbox, tokyonight, onedark, cobalt, synthwave, highcontrast, dracula
```markdown
![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=anuraghazra&show_icons=true&theme=radical)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867982181-a6ce454f-774d-41d6-9a08-51ad1bd028a3.png#averageHue=%23162620&clientId=uca56e9e1-d449-4&from=paste&id=u8231a3e2&originHeight=860&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2b21cd57-d081-4d24-934c-accf57702af&title=)
<a name="ts5Dy"></a>
### 自定义
可以通过使用 URL 参数的方式，为 Stats Card 或 Repo Card 自定义样式。<br />常用选项：

- title_color - 卡片标题颜色 （十六进制色码）
- text_color - 内容文本颜色 （十六进制色码）
- icon_color - 图标颜色（如果可用）（十六进制色码）
- bg_color - 卡片背景颜色 （十六进制色码）**或者** 以 _angle,start,end_ 的形式渐变
- hide_border - 隐藏卡的边框 (布尔值)
- theme - 主题名称，从 ../themes/README.md 中选择
- cache_seconds - 手动设置缓存头 （最小值: 1800，最大值: 86400）
- locale - 在卡片中设置语言 (例如 cn, de, es, 等等)
<a name="yPeox"></a>
### 热门语言卡片
热门语言卡片显示了 GitHub 用户常用的编程语言。注意：热门语言并不表示技能水平或类似的水平，它是用来衡量用户在 github 上拥有最多代码的语言的一项指标，它是 github-readme-stats 的新特性。
```markdown
[![Top Langs](https://github-readme-stats.vercel.app/api/top-langs/?username=anuraghazra)](https://github.com/anuraghazra/github-readme-stats)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675867982327-d202db5f-d5d5-4a8e-bb2c-53d4a1aa8329.png#averageHue=%23faf9f7&clientId=uca56e9e1-d449-4&from=paste&id=ufb871b96&originHeight=285&originWidth=300&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u854dd086-6a3d-4822-aa78-c4d8747dd40&title=)
<a name="g5mMo"></a>
### 项目地址
[https://github.com/anuraghazra/github-readme-stats](https://github.com/anuraghazra/github-readme-stats)
