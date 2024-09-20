R 数据可视化
<a name="c7ujg"></a>
## R-waffle包简介
使用R绘制华夫饼图最简单的方法是使用waffle包进行绘制(当然，也可以使用ggplot2的`geom_tile()`和`geom_point()`函数绘制)。这里主要介绍waffle包`waffle()`、`geom_pictogram()`、`geom_waffle()`和`scale_label_pictogram()` 绘图函数，详细如下：

- `waffle()`：绘制方形华夫饼图
- `geom_waffle()`：绘制方形华夫饼图图层(用法和ggplot2相似)
- `geom_pictogram()`：绘制类似图形文字的集合图层。
- `scale_label_pictogram()`：与`geom_pictogram()`一起使用，使用Font Awesome字体映射标签。

接下来，通过各种例子介绍上述介绍的绘图函数。
<a name="C3ymb"></a>
## R-waffle包样例介绍
<a name="G4p0V"></a>
### 1、`waffle()`：
基本例子：
```r
library(tidyverse)
library(ggtext)
library(hrbrthemes)
library(wesanderson)
library(LaCroixColoR)
library(RColorBrewer)
library(waffle)

parts <- data.frame(
  names = LETTERS[1:4],
  vals = c(80, 30, 20, 10)
)
waffle_plot <- waffle(parts,rows = 8)+
  theme_ipsum(base_family = "Roboto Condensed",grid = "") +
  labs(
  title = "Example of <span style='color:#D20F26'>waffle::waffle() function</span>",
  subtitle = "processed charts with <span style='color:#1A73E8'>waffle()</span>",
  caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme(plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                      size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
        plot.caption = element_markdown(face = 'bold',size = 12),
        axis.text.x = element_blank(),
        axis.text.y = element_blank())
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623996826678-3a91082e-99a8-4ee7-a332-d8386ac3864b.png#clientId=u3ca8b34d-0e22-4&from=paste&id=u43b0186d&originHeight=629&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u0ec19c07-9290-4d84-a74f-bd31c1f388e)<br />Example01 of waffle()<br />当然还可以通过设置`use_glyph`参数，使用fontawesome 图标进行绘制，如下：
```r
waffle(parts,rows = 8,
                      colors = c("#9DC8C8", "#58C9B9", "#519D9E","#D1B6E1"),
                      use_glyph = "file-medical",
                      size=5,legend_pos = "bottom")+
····

```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623996826498-1eaf12cb-7806-4d0f-b996-98918fee9012.webp#clientId=u3ca8b34d-0e22-4&from=paste&id=u39e6bf93&originHeight=621&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u610d7868-b9d1-4f87-9365-f7959cb0d11)<br />Example02 of waffle()
<a name="AiJOa"></a>
### 2、`geom_waffle()`：
```r
df <- data.frame(
  parts = factor(rep(month.abb[1:3], 3), levels=month.abb[1:3]),
  vals = c(10, 20, 30, 6, 14, 40, 30, 20, 10)
)
df_count <- df %>% count(parts, wt = vals)
plot01 <- ggplot(data = df_count,aes(fill = parts, values = n)) +
  geom_waffle(n_rows = 20, size = .5, colour = "black", flip = TRUE) +
  #ggsci::scale_fill_lancet(labels = c("Fruit", "Sammich", "Pizza"))+
  scale_fill_manual(values = lacroix_palette(type = "paired"),
                    labels = c("Fruit", "Sammich", "Pizza")) +
  guides(fill=guide_legend(nrow=1,byrow=TRUE,reverse = TRUE,title=NULL))+
  labs(
    title = "Example of <span style='color:#D20F26'>waffle::geom_waffle function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_waffle()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_ipsum(base_family = "Roboto Condensed",grid="") +
  theme_enhance_waffle()+
  theme(plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
        plot.caption = element_markdown(face = 'bold',size = 12),
        legend.position = 'bottom',)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623996826625-7ea2db83-fd6d-4006-bbf7-e21c0c604f56.webp#clientId=u3ca8b34d-0e22-4&from=paste&id=u4947caa3&originHeight=878&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ue3a4b086-6e62-4be6-aa3e-bf7b66520d6)<br />Example01 of geom_waffle<br />「Waffle Bar Charts」
```r
test02 <-  storms %>% filter(year >= 2010) %>% count(year, status)
ggplot(test02, aes(fill = status, values = n)) +
  geom_waffle(color = "white", size = .25, n_rows = 10, flip = TRUE) +
  facet_wrap(~year, nrow = 1, strip.position = "bottom") +
  scale_x_discrete() + 
  scale_y_continuous(labels = function(x) x * 10,expand = c(0,0)) +
  scale_fill_manual(values = lacroix_palette(type = "paired"),
                    name=NULL) +
  coord_equal() +
  labs(
    title = "Example of <span style='color:#D20F26'>waffle::geom_pictogram function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_pictogram()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>",
    x = "Year",
    y = "Count") +
  theme_minimal(base_family = "Roboto Condensed") +
  theme(plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
        plot.caption = element_markdown(face = 'bold',size = 12),
        legend.position = 'bottom',
        panel.grid = element_blank(),
       axis.ticks.y = element_line()) +
  guides(fill = guide_legend(reverse = TRUE))

```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623996826725-05022fb2-9150-4954-828d-b007ddaac16a.webp#clientId=u3ca8b34d-0e22-4&from=paste&id=ub4d80a5f&originHeight=878&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u68189ec8-0ace-4d34-8e25-7da5c86a896)<br />Example of geom_waffle()<br />通过此例子，就可以通过使用不同图方形个数表示不同类别的个数，当然，也可以使用不同样式图标进行表示。
<a name="veyTY"></a>
### 3、`geom_pictogram()`和`scale_label_pictogram()`
还还可以通过`geom_pictogram()`、`scale_label_pictogram()`绘图函数使用fontawesome字体图标进行绘制，例子如下：<br />[fa 字体.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1623997007426-3f96edaf-4a06-48c8-bf89-82d06f9df3e0.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1623997007426-3f96edaf-4a06-48c8-bf89-82d06f9df3e0.zip%22%2C%22name%22%3A%22fa%20%E5%AD%97%E4%BD%93.zip%22%2C%22size%22%3A269455%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22u1561ceec-4f85-4fc8-a511-549514ff92e%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22ucfcbf36a%22%2C%22card%22%3A%22file%22%7D)
```r
#导入字体：比较重要的一步
library(showtext)
font_add(family = "FontAwesome5Free-Solid", regular = "E:\\Download Fonts\\fa-solid-900.ttf")
font_add(family = "FontAwesome5Free-Regular", regular = "E:\\Download Fonts\\fa-regular-400.ttf")
font_add(family = "FontAwesome5Brands-Regular", regular = "E:\\Download Fonts\\fa-brands-400.ttf")
font_add(family = "Roboto Condensed",regular = "E:\\Download Fonts\\RobotoCondensed-Regular.ttf")

plot02 <- ggplot(data = df_count) +
  geom_pictogram(aes(label = parts, values = n,colour = parts),n_rows = 10,
                 flip = TRUE, make_proportional = TRUE) +
  scale_colour_manual(values = lacroix_palette(type = "paired"),
                      labels = c("Fruit", "Sammich", "Pizza")) +
  scale_label_pictogram(
    values = c("apple-alt", "bread-slice", "pizza-slice"),
    labels = c("Fruit", "Sammich", "Pizza")
  ) +
  guides(fill=guide_legend(nrow=1,byrow=TRUE,reverse = TRUE,title=NULL))+
  labs(
    title = "Example of <span style='color:#D20F26'>waffle::geom_pictogram()  function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_pictogram()</span>",
    caption = "Visualization by <span style='color:#0057FF'>DataCharm</span>") +
  theme_ipsum(grid="") +
  theme_enhance_waffle()+
  theme(plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",face = 'bold',
                                      size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
        plot.caption = element_markdown(face = 'bold',size = 12),
        legend.position = 'bottom',)

```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623996826787-b2c287eb-4464-4473-87e3-bd0655ea0717.webp#clientId=u3ca8b34d-0e22-4&from=paste&id=ua370d0e8&originHeight=878&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u2f147c49-a512-4e23-906a-8b5e3200449)<br />Example Of geom_pictogram
