Python R<br />汇总一下有关Venn Diagram(韦恩图) 的绘制方法，主要内容包括：

- Venn Diagram(韦恩图)的简介
- Venn Diagram(韦恩图)的R绘制方法
- Venn Diagram(韦恩图)的Python绘制方法
<a name="p0Aza"></a>
## Venn Diagram(韦恩图)的简介
Venn Diagram(韦恩图)，或叫Venn图、文氏图、温氏图，是在所谓的集合论（或者类的理论）数学分支中表示集合或者类的一种草图，主要用于显示元素集合重叠区域的图示，如下图所示：<br />![2021-05-31-10-36-43-088207.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622428915386-9c889cba-f358-43a8-aeee-548c5cb64d3f.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=uaf62e24f&originHeight=1009&originWidth=1080&originalType=binary&size=156262&status=done&style=shadow&taskId=ued923d9c-6248-4bd6-b12b-8474982e305)<br />Venn Diagram Example<br />那么，如何使用R或者Python实现Venn Diagram(韦恩图)的高效绘制呢？首先介绍R绘制的方法。
<a name="HXjc9"></a>
## Venn Diagram(韦恩图)的R绘制方法
<a name="Y4uds"></a>
### R-ggvenn包绘制
使用R绘制Venn Diagram图，首先想到的肯定是ggplot2,而`ggvenn`包作为ggplot2的拓展包且有`geom_*()`，这里就最先介绍。ggvenn包主要使用`ggvenn()`函数和`geom_venn()`绘图函数(ggplot2图层语法类似)绘制。官网：[https://github.com/yanlinlin82/ggvenn](https://github.com/yanlinlin82/ggvenn)<br />首先介绍下其主要的参数设置：<br />1. For filling:

- `fill_color`(填充颜色)：默认是 c("blue", "yellow", "green", "red")
- `fill_alpha`(透明度)：默认是0.5

2. For stroke:

- `stroke_color`(线条颜色)：默认是"black"
- `stroke_alpha`(线条透明度)：默认是1
- `stroke_size`(线条宽度)：默认是1
- `stroke_linetype`：默认是"solid"

3. For set name:

- `set_name_color`(文本名颜色)：默认是"black"
- `set_name_size`(文本名大小)：默认是6

4. For text:

- `text_color`(文本颜色)：默认是"black"
- `text_size`(文本大小)：默认是4.

以上各参数为`ggvenn()`和`geom_venn()`绘图函数的共同参数，其他额外参数可自行查阅~<br />接下来，结合实例进行解释说明~<br />「样例一」:`ggvenn()`函数绘制
```r
# 样例数据
a <- list(`Set 1` = c(1, 3, 5, 7, 9),
          `Set 2` = c(1, 5, 9, 13),
          `Set 3` = c(1, 2, 8, 9),
          `Set 4` = c(6, 7, 10, 12))
#可视化绘制
opar <- par(family = "Roboto Condensed")
ggvenn(a,fill_color=mypal,fill_alpha = .7,stroke_linetype = "longdash",set_name_size = 8,
      text_size=5) 
```
![2021-05-31-10-36-43-254782.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622428958756-4ca027d5-5798-4447-8765-6b70c058dc46.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=u89ee723f&originHeight=712&originWidth=874&originalType=binary&size=46459&status=done&style=shadow&taskId=u21106c4c-b0f0-41a9-8e63-bb7da1e9f28)<br />Example Of ggvenn()<br />「样例二」:`geom_venn()`函数绘制
```r
# 生成样例数据
d <- tibble(value   = c(1, 2, 3, 5, 6, 7, 8, 9, 10, 12, 13),
            `Set 1` = c(T, F, T, T, F, T, F, T, F,  F,  F),
            `Set 2` = c(T, F, F, T, F, F, F, T, F,  F,  T),
            `Set 3` = c(T, T, F, F, F, F, T, T, F,  F,  F),
            `Set 4` = c(F, F, F, F, T, T, F, F, T,  T,  F))
# 可视化绘制
ggvenn_4 <- ggplot(d, aes(A = `Set 1`, B = `Set 2`, C = `Set 3`, D = `Set 4`)) +
  geom_venn(fill_color=mypal,fill_alpha = .7,stroke_linetype = "longdash",set_name_size = 8,
            text_size=5) + 
  theme_void()+
  coord_fixed() +
  labs(title = "Example of ggvenn:: geom_venn function",
       subtitle = "processed charts with geom_venn()",
       caption = "Visualization by DataCharm") +
  theme(plot.title = element_text(hjust = 0.5,vjust = .5,color = "black",face = 'bold',
                                  size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_text(hjust = 0,vjust = .5,size=15),
       plot.caption = element_text(face = 'bold',size = 12))
```
![2021-05-31-10-36-43-360478.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622428978530-0b0d0d7e-4627-430b-97c6-dcc06fcc9b5b.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=u57e3cd75&originHeight=839&originWidth=902&originalType=binary&size=22915&status=done&style=shadow&taskId=ubc8a5932-cf3b-4762-b05c-20d9e68436a)<br />Example Of geom_venn()<br />这里分别使用了`ggvenn()` 和 `geom_venn()` 函数绘制了韦恩图，更多细节，可参考官网进行理解~
<a name="y1Kxn"></a>
### R-ggVennDiagram包绘制
R-ggVennDiagram包和ggvenn包一样也是ggplot2的拓展包，其可以支持2~7维的韦恩图绘制，这里直接通过使用`ggVennDiagram()`绘制韦恩图进行解释。<br />「样例」：
```r
library(ggVennDiagram)
# 样例数据
genes <- paste("gene",1:1000,sep="")
set.seed(20210419)
x <- list(A=sample(genes,300),
          B=sample(genes,525),
          C=sample(genes,440),
          D=sample(genes,350))
# 可视化绘制
library(ggplot2)
ggVennDiagram(x, category.names = c("Stage 1","Stage 2","Stage 3", "Stage4"),
              size=1,lty="longdash",color="gray60") + 
  scale_fill_gradient(name="Count",low="#EC7D85",high = "#182F6F") +
  hrbrthemes::theme_ipsum(base_family = "sans") +
  labs(title = "Example of ggVennDiagram:: ggVennDiagram function",
       subtitle = "processed charts with ggVennDiagram()",
       caption = "Visualization by DataCharm") +
  theme(plot.title = element_text(hjust = 0.5,vjust = .5,color = "black",face = 'bold',
                                  size = 20, margin = margin(t = 1, b = 12)),
        plot.subtitle = element_text(hjust = 0,vjust = .5,size=15),
        plot.caption = element_text(face = 'bold',size = 12),
        axis.text.x = element_blank(),
        axis.text.y = element_blank(),
        axis.title.x = element_blank(),
        axis.title.y = element_blank())
```
![2021-05-31-10-36-43-529026.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622429020383-ccd3ce8f-ef38-4963-8516-a803b50db94a.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=ub8990682&originHeight=863&originWidth=1080&originalType=binary&size=192314&status=done&style=shadow&taskId=u8ef9ba03-5a55-4879-a638-ab105fa9ad8)<br />Example of ggVennDiagram
<a name="yC209"></a>
## Venn Diagram(韦恩图)的Python绘制方法
要想使用Python绘制韦恩图(这里主要介绍基于matplotlib的绘制方法，交互式后期统一介绍)，这里介绍一个非常便捷的方法-matplotlib-venn 绘制。安装方式如下：
<a name="OAKHC"></a>
### 「安装」
```bash
easy_install matplotlib-venn
```
<a name="Z7pGy"></a>
### 「官网」
[https://github.com/konstantint/matplotlib-venn](https://github.com/konstantint/matplotlib-venn)
<a name="C5pmG"></a>
### 「样例一」：两个集合
```python
from matplotlib_venn import venn2, venn2_circles, venn2_unweighted
from matplotlib_venn import venn3, venn3_circles
import matplotlib.pyplot as plt

# 样例数据
Group1 = ['a','b','c','e','f','g','i','p','q']
Group2 = ['b','e','f','h','k','q','r','s','t','u','v','z']
Group3 = ['c','e','g','h','j','k','o','q','z']

#可视化绘制
plt.rcParams['font.family'] = ["Times New Roman"]
fig, ax = plt.subplots(figsize=(5,3),dpi=110)
vee2 = venn2([set(Group1), set(Group2)],set_labels=("Group1", "Group2"),
      set_colors=("#0073C2FF", "#EFC000FF"), alpha = 0.8,ax=ax)
venn2_circles([set(Group1), set(Group2)], linestyle="--", linewidth=2, color="black",ax=ax)

# 定制化设置:设置字体属性
for text in vee2.set_labels:
    text.set_fontsize(15);
for text in vee2.subset_labels:  
    text.set_fontsize(16)
    text.set_fontweight("bold")
ax.text(.8,-.1,'\nVisualization by DataCharm',transform = ax.transAxes,
        ha='center', va='center',fontsize = 8,color='black')
plt.title("Example Of venn2() and venn2_circles()",size=15,fontweight="bold",
         backgroundcolor="#868686FF",color="black",style="italic")
```
![2021-05-31-10-36-43-688409.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622429040356-131d4c7b-7c06-470c-8911-f22224a807ee.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=u353380ae&originHeight=884&originWidth=1080&originalType=binary&size=101401&status=done&style=shadow&taskId=u83efbdc9-2a39-4760-9253-c78414aba26)<br />Example Of venn2()
<a name="w9rjX"></a>
### 「样例二」：三个集合
```python
fig, ax = plt.subplots(figsize=(5,3),dpi=110)
vd3=venn3([set(Group1),set(Group2),set(Group3)],
          set_labels=("Group1","Group2","Group3"),
          set_colors=('#0073C2FF','#EFC000FF',"#CD534CFF"), 
          alpha = 0.8,ax=ax)
venn3_circles([set(Group1), set(Group2),set(Group3)], linestyle="--", linewidth=2, color="black",ax=ax)
# 定制化设置
for text in vd3.set_labels:
    text.set_fontsize(15);
    text.set_fontweight("bold")
for text in vd3.subset_labels:
    text.set_fontsize(15)
    text.set_fontweight("bold")
ax.text(.8,-.1,'\nVisualization by DataCharm',transform = ax.transAxes,
        ha='center', va='center',fontsize = 9,color='black')
plt.title("Example Of venn3() and venn3_circles()",fontweight="bold",fontsize=15,
          pad=30,backgroundcolor="#868686FF",color="black",style="italic")
```
![2021-05-31-10-36-43-907231.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622429053424-c89d08a9-73ee-4bf1-b4e0-02327c322e5e.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=uaf011246&originHeight=985&originWidth=1080&originalType=binary&size=125664&status=done&style=shadow&taskId=uea496924-de53-43a7-bcfa-25f49b83742)<br />Example Of venn3()<br />当然，还可以通过如下代码定义”圈“的样式：
```python
···
c3 = venn3_circles([set(Group1), set(Group2),set(Group3)], linestyle="--", linewidth=2, color="black",ax=ax)
···
# 对圆进行设置
c3[1].set_lw(7)
c3[1].set_ls(":")
c3[1].set_color("#7AA6DCFF")
···
```
![2021-05-31-10-36-44-096640.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622429063712-95391c90-2a2a-4fe2-ab7e-8db91c4d5070.png#clientId=u3ad6dd50-f7a2-4&from=ui&id=u82b6617c&originHeight=985&originWidth=1080&originalType=binary&size=110416&status=done&style=shadow&taskId=u8af5a1a9-2234-4722-9707-7b6c9487c50)

 
