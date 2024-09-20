数据可视化 R
<a name="JzD9G"></a>
## R-ggCyberPunk包简介
R-ggCyberPunk包作为基于ggplot2、专门绘制赛博朋克风的第三方包，其提供的绘制函数有限，主要包括以下几个：

- `geom_glowing_area()`：绘制赛博朋克风发光区域图(area plot)。
- `geom_glowing_line()`：绘制赛博朋克风发光线图(line plot )。
- `geom_linesaber()`:绘制赛博朋克风激光剑外观线图(line plot)。
- `theme_cyberpunk()`:绘制赛博朋克风绘图主题。

更多参数信息可参考：[R-ggCyberPunk介绍](https://github.com/delabj/ggCyberPunk)
<a name="ofOmu"></a>
## R-ggCyberPunk包实例演示
这一部分就列举使用几个 R-ggCyberPunk包绘制赛博朋克风的可视化作品，如下：
<a name="V9KDq"></a>
### 「`geom_glowing_area()`」：
```r
library(tidyverse)
library(ggtext)
library(hrbrthemes)
library(ggCyberPunk)

df = data.frame(A=c(1,4,4,6,7,5,1),
               B=c(4,3,5,7,5,6,7),
               Time=c(1,2,3,4,5,6,7)) %>%
# 将数据准换成DF类型
pivot_longer(cols = c(A,B),names_to = "group", values_to = "count")

#可视化绘制
plot01 <- ggplot(data = df,aes(x=Time, y = count, color = group, fill= group))+
     geom_glowing_area()+
     scale_fill_cyberpunk("zune")+
     scale_color_cyberpunk("zune")+
    labs(
    title = "Example of <span style='color:#D20F26'>ggCyberPunk::geom_glowing_area function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_glowing_area()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_cyberpunk(font = "Roboto Condensed")+
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(hjust = 1,face = 'bold',size = 12)
      )
```
![2021-08-27-16-32-35-837256.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053465059-d6fe6013-3253-46c0-9fad-e86f895a76e2.png#clientId=u65dd1664-666a-4&from=ui&id=u37e1b6c1&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=129306&status=done&style=none&taskId=uddd0ab8b-b6aa-499b-9e79-364364fed9c)<br />Example Of geom_glowing_area()with zune<br />注意：这里`scale_fill_cyberpunk()` 中可设置`main `, `zune`,` cotton candy`, `laser sword` 四种选项。效果分别如下：<br />「`main`」:<br />![2021-08-27-16-32-36-017257.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053464956-0a9861c0-af7c-41ca-9764-434d1aaa3110.png#clientId=u65dd1664-666a-4&from=ui&id=gD9a9&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=135725&status=done&style=none&taskId=ud7a7a978-a4aa-4ce0-b1fa-1db2f666baf)<br />Example Of geom_glowing_area()with main<br />「`cotton candy`」:<br />![2021-08-27-16-32-36-190256.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053464970-d0bd6cca-77ed-4563-a12a-fa119e1f09b3.png#clientId=u65dd1664-666a-4&from=ui&id=lZglz&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=135450&status=done&style=none&taskId=u6ac74943-61b2-4e26-b8b1-e190c68a972)<br />Example Of geom_glowing_area()with cotton candy<br />「`laser sword`」:<br />![2021-08-27-16-32-36-333256.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053485007-e672c163-b3a8-4167-8423-985116a37079.png#clientId=u65dd1664-666a-4&from=ui&id=u4a77daa1&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=133457&status=done&style=none&taskId=uc7a9f31c-fd33-435b-b637-62a3bccfccb)<br />Example Of geom_glowing_area()with laser sword
<a name="FD1WE"></a>
### 「`geom_glowing_line()`」：
```r
df = data.frame(A=c(1,4,4,6,7,5,1),
               B=c(4,3,5,7,5,6,7),
               Time=c(1,2,3,4,5,6,7)) %>%
pivot_longer(cols = c(A,B),names_to = "group", values_to = "count")


plot2 <- ggplot(data = df,aes(x=Time, y = count, color = group, fill= group))+
     geom_glowing_line()+
     scale_color_cyberpunk()+
     labs(
    title = "Example of <span style='color:#D20F26'>ggCyberPunk::geom_glowing_line function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_glowing_line()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_cyberpunk(font = "Roboto Condensed")+
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(hjust = 1,face = 'bold',size = 12)
      )
```
![2021-08-27-16-32-36-934258.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053494696-af6d6359-daab-4979-9fef-c838cea81c0d.png#clientId=u65dd1664-666a-4&from=ui&id=ub73bcf2e&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=125816&status=done&style=none&taskId=ufa6f9e61-1897-486b-9a6e-3cbfc8ca226)<br />Example Of geom_glowing_line()
<a name="swEwg"></a>
### 「`geom_linesaber()`」：
```r
df = data.frame(A=c(1,4,4,6,7,5,1),
               B=c(4,3,5,7,5,6,7),
               Time=c(1,2,3,4,5,6,7)) %>%
pivot_longer(cols = c(A,B),names_to = "group", values_to = "count")

plot3 <- ggplot(data = df,aes(x=Time, y = count, color = group, fill= group))+
     geom_linesaber()+
     theme_cyberpunk()+
     scale_color_cyberpunk(reverse = T)+
    labs(
    title = "Example of <span style='color:#D20F26'>ggCyberPunk::geom_linesaber function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_linesaber()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_cyberpunk(font = "Roboto Condensed")+
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(hjust = 1,face = 'bold',size = 12)
      )
```
![2021-08-27-16-32-37-406265.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053506414-c0fe3650-46f5-4835-9ef2-c12f0b5d96e5.png#clientId=u65dd1664-666a-4&from=ui&id=ucf21ba9a&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=130676&status=done&style=none&taskId=u963ec0dc-2635-42ad-88e9-3fc5bc24a98)<br />Example Of geom_linesaber()<br />以上就是简单介绍了R-ggCyberPunk包三个绘图函数，接下里，使用改包主题对常见图表进行朋克风格图表绘制，如下：
<a name="arDKl"></a>
### 「geom_boxplot」：箱线图
```r
plot4 <- ggplot(mpg, aes(class, hwy,fill=factor(drv))) + 
  geom_boxplot() +
  scale_fill_cyberpunk("zune",name="")+
    labs(
    title = "Example of <span style='color:#D20F26'>ggCyberPunk::scale_fill_cyberpunk function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>scale_fill_cyberpunk()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_cyberpunk(font = "Roboto Condensed")+
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(hjust = 1,face = 'bold',size = 12)
      )
```
![2021-08-27-16-32-37-735254.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630053506405-3a8865ed-1365-46c4-8305-e6be22308021.png#clientId=u65dd1664-666a-4&from=ui&id=aAGDg&originHeight=810&originWidth=1080&originalType=binary&ratio=1&size=64677&status=done&style=none&taskId=ud95355be-9f53-4ece-993e-a4bd58736ff)<br />Example Of ggplot2::geom_boxplot() with theme_cyberpunk()<br />当然，还可以设置其他颜色风格的样式。如果不喜欢R绘制类似的图表，可以试试python的可视化库－mplcyberpunk，更多关于mplcyberpunk库的详细内容大家可参考：Python－[mplcyberpunk库](https://github.com/dhaitz/mplcyberpunk)
<a name="US5Fo"></a>
## 参考资料
R-ggCyberPunk介绍：[https://github.com/delabj/ggCyberPunk](https://github.com/delabj/ggCyberPunk)<br />python－mplcyberpunk库：[https://github.com/dhaitz/mplcyberpunk](https://github.com/dhaitz/mplcyberpunk)
