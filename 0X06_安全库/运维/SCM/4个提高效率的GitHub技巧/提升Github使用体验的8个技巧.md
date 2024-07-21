Github
<a name="z8g34"></a>
## 1、简介
作为全球最大的开源及私有软件项目托管社区平台，Github可以显著地帮助从事编程相关工作的人员提升自己的技术水平。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666853334939-259ed6e8-1728-42f4-a5e9-cbb5c72004b6.png#averageHue=%23eeeeee&clientId=u1a25a137-0b8d-4&from=paste&id=uc523a841&originHeight=397&originWidth=719&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9aafd1b2-4f97-4dc4-829a-110a549bdbe&title=)<br />Github为了使得其使用更加便捷，时常推出一些实用的功能，来学习其中方便日常编写Markdown文档、创建issues等操作的几则小技巧。
<a name="psRu5"></a>
## 2、Github日常使用小技巧
<a name="IkkFD"></a>
### 2.1 在Markdown中绘制在线地图
Github支持使用Markdown编写的文档或内容中利用特殊的语法插入geoJSON或topoJSON数据，从而直接渲染交互式在线地图：

- geoJSON示例

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666853334922-376a4468-3ff0-4384-80fc-89bff6f8d78c.png#averageHue=%23f3f3f1&clientId=u1a25a137-0b8d-4&from=paste&id=u5f67c0eb&originHeight=643&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uae34ab90-1659-4735-aaff-2e274fd2f28&title=)<br />下面是`geoJSON`渲染示例
```json
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "type": "Polygon",
        "coordinates": [
          [
            [
              106.58472061157227,
              29.57106827738255
            ],
            [
              106.58025741577148,
              29.567335671068733
            ],
            [
              106.57304763793945,
              29.5653946612635
            ],
            [
              106.56137466430664,
              29.566738441255648
            ],
            [
              106.5538215637207,
              29.569873858372727
            ],
            [
              106.54541015625,
              29.569724555003123
            ],
            [
              106.53871536254883,
              29.565842589914105
            ],
            [
              106.53751373291016,
              29.562259105087506
            ],
            [
              106.53064727783203,
              29.554942428835226
            ],
            [
              106.53545379638672,
              29.54568383423715
            ],
            [
              106.55193328857422,
              29.551955878093022
            ],
            [
              106.5596580505371,
              29.55091056448027
            ],
            [
              106.57115936279295,
              29.54986524005526
            ],
            [
              106.57888412475586,
              29.55329983685018
            ],
            [
              106.58557891845703,
              29.561960475613336
            ],
            [
              106.58472061157227,
              29.57106827738255
            ]
          ]
        ]
      }
    }
  ]
}
```

- topoJSON示例

下面是`topoJSON`渲染示例
```json
{
 "type": "Topology",
 "objects": {
   "collection": {
     "type": "GeometryCollection",
     "geometries": [
       {
         "type": "Polygon",
         "arcs": [
           [0]
         ]
       }
     ]
   }
 },
 "arcs": [
   [
     [2008, 9999],
     [-2008, -1221],
     [1261, -8778],
     [7285, 1408],
     [1453, 5797],
     [-7991, 2794]
   ]
 ],
 "transform": {
   "scale": [
     0.0000160691117939919,
     0.00001612860192965454
   ],
   "translate": [
     106.52584075927734,
     29.47517125618036
   ]
 },
 "bbox": [
   106.52584075927734,
   29.47517125618036,
   106.68651580810547,
   29.636441146874976
 ]
}
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666853334945-9242fb3e-2acf-46c7-89b0-ddc25170af9f.png#averageHue=%23f3f3f1&clientId=u1a25a137-0b8d-4&from=paste&id=u1fab840d&originHeight=650&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8cb7847f-77de-4e39-a281-34eea9576f3&title=)
<a name="G5tyl"></a>
### 2.2 Markdown中令图片根据主题自动切换
Github在其markdown语法中，添加了可以随着用户对网站主题的切换，而自动变换到相应图片地址的功能，具体用法可参考（[https://github.blog/changelog/2022-05-19-specify-theme-context-for-images-in-markdown-beta/](https://github.blog/changelog/2022-05-19-specify-theme-context-for-images-in-markdown-beta/)）：<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1666853335133-8de74fb9-a246-4a44-b45e-4dad5985fd3a.gif#averageHue=%23f1efed&clientId=u1a25a137-0b8d-4&from=paste&id=ud9173eb6&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5677496e-5d68-4910-9d87-a771589f0ef&title=)
<a name="Eo3jq"></a>
### 2.3 根据粘贴板链接快速生成Markdown超链接内容
当你刚刚复制了一段链接地址后，回到Github的编辑窗口中选中某段文字后粘贴，就会自动快捷生成以这段文字为显示内容的超链接：<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1666853334894-3b5893d6-a498-4a72-8803-64feb975a64f.gif#averageHue=%23fefefe&clientId=u1a25a137-0b8d-4&from=paste&id=u95cdd856&originHeight=467&originWidth=572&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u419e3d8e-a1d0-4686-b8e2-d5711d48ebc&title=)
<a name="eF6mr"></a>
### 2.4 快捷生成引用块、有序列表及无序列表
选中目标内容后，按下快捷键ctrl+shift+.可以快速生成引用块；按下快捷键ctrl+shift+7可以快速生成有序列表；按下快捷键ctrl+shift+8可以快速生成无序列表：<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1666853335733-dc48db39-805e-4320-9907-b3e29a856d91.gif#averageHue=%23b2a89f&clientId=u1a25a137-0b8d-4&from=paste&id=uae0ee540&originHeight=438&originWidth=1078&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u880b4e9c-5f1c-4850-9035-67923eeedd5&title=)
<a name="JvLV5"></a>
### 2.5 为文档生成脚注
可以在Markdown文档中利用[^序号]的语法完成脚注内容的插入，使得阅读文档时点击插入处即可自动滚动到相应的页脚中：<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1666853336051-a25ed1ed-db6f-4f2f-9d5c-93ad37ec475c.gif#averageHue=%23f4f8f9&clientId=u1a25a137-0b8d-4&from=paste&id=ued358029&originHeight=674&originWidth=863&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud3161572-c8ab-4640-acef-1d03ed98b20&title=)
<a name="n1MEh"></a>
### 2.6 以在线vscode的方式浏览仓库
当你正在浏览某个仓库时，双击键盘上的`.`键，稍等片刻后，Github会刷新页面以在线vscode的形式展示此仓库内容，从而方便浏览仓库内文件，以pandas官方仓库为例：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666853336069-2fb36632-3dc3-4cfd-87a1-f74a6b140e29.png#averageHue=%23ded7bc&clientId=u1a25a137-0b8d-4&from=paste&id=u174b22bf&originHeight=522&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2f8e7670-d753-4acb-839b-6ce3256a92b&title=)
<a name="po8HB"></a>
### 2.7 使用国内镜像加速仓库clone
由于一些众所周知的原因，在国内clone来自Github的远程仓库时，速度十分捉急，而目前只需要在目标仓库的clone地址前添加前缀https://github.91chi.fun/再进行clone，速度就会快上许多，譬如：
```bash
git clone https://github.91chi.fun/https://github.com/pandas-dev/pandas.git
```
<a name="zvBuQ"></a>
### 2.8 状态风格引用块
最后要介绍的这个特性是Github最近几天放出的实验性质功能，目前可获得的情报中，可以利用下面的语法方式，创建出带有状态风格的引用块，从而使得文档内容更加别致，大家也可以在（[https://github.com/github/feedback/discussions/16925](https://github.com/github/feedback/discussions/16925)）中关注其最新消息：
```markdown
> **Note**<br>
> This is a note

> **Warning**<br>
> This is a warning
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666853336376-536aaf81-d2d5-47c0-9e69-ca3a1fbf84ba.png#averageHue=%23fefefd&clientId=u1a25a137-0b8d-4&from=paste&id=ub3c62583&originHeight=315&originWidth=459&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uea8c4f40-00a6-470e-ad68-8acc91ddece&title=)
