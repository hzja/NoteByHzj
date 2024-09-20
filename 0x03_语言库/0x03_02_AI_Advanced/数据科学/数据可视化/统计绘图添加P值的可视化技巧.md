数据科学<br />在一些常见的统计图表中经常需要在一些图表中添加P值，汇总一下关于统计图表中P值的添加方法。今天推文的主要内容如下：

- P值简单介绍
- 可视化绘制中P值绘制
<a name="KXYbD"></a>
## P值简单介绍
P值是指在一个概率模型中，统计摘要（如两组样本均值差）与实际观测数据相同，或甚至更大这一事件发生的概率。换言之，是检验假设零假设成立或表现更严重的可能性。P值若与选定显著性水平(0.05或0.01)相比更小，则零假设会被否定而不可接受。然而这并不直接表明原假设正确。P值是一个服从正态分布的随机变量，在实际使用中因样本等各种因素存在不确定性.在许多研究领域，0.05的P值通常被认为是可接受错误的边界水平。
<a name="DQVNM"></a>
## 可视化绘制中P值绘制
在可视化图表中添加P值，使其更好的表现图表含义是在绘制图表是需要考虑的。这里使用R-ggpubr和R-ggsignif包进行P值添加及定制化操作。
<a name="oPndC"></a>
### R-ggpubr 添加P值
在使用ggpubr包进行P值添加之前，需导入R-rstatix包进行必要的统计操作(T检验等)，这里直接通过例子进行解释说明。
<a name="n77Yl"></a>
#### 「简单例子」：
```python
# 导入必要的包
library(tidyverse)
library(ggtext)
library(hrbrthemes)
library(ggpubr)
library(rstatix)
library(ggsci)

# 导入数据
df <- ToothGrowth
df$dose <- as.factor(df$dose)

# 进行T-test
stat.test <- df %>% rstatix::t_test(len ~ supp) %>% rstatix::add_significance()

# 进行可视化绘制
stat.test <- stat.test %>% rstatix::add_xy_position(x = "supp")
bxp <- ggpubr::ggboxplot(df, x = "supp", y = "len", fill = "supp",
                         palette="jco",
                         ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")) +

  labs(
    title = "Example of <span style='color:#D20F26'> add P values in ggpubr</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>ggpubr::ggboxplot + rstatix::t_test</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>")+
  # 添加P值
  stat_pvalue_manual(stat.test, label = "p") +
  scale_y_continuous(expand = expansion(mult = c(0.05, 0.1)))+
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-36-281402.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352979047-6d1baa6d-8c68-4b69-8d7c-9ef9d42f3df1.png#align=left&display=inline&height=810&id=u40c3cb2b&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />boxplot with P value<br />此外，还可以通过显示P值的显著性水平(p.signif)
```python
+
stat_pvalue_manual(stat.test, label = "p.signif")
+
```
![2021-05-07-09-47-36-427189.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352969249-4b90b98e-52f9-4703-8af0-62f9c9fec021.png#align=left&display=inline&height=810&id=udabbd78a&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />boxplot with P value in different form
<a name="xoPWh"></a>
#### 「分组数据例子」：
如果面对分组数据时，那么可通过如下代码进行组与组数据直接的显著性比较，还是使用上面的数据，只不过使用`group_by`进行分组操作：
```python
# 分组计算P值
stat.test <- df %>% group_by(dose) %>%rstatix::t_test(len ~ supp) %>% rstatix::adjust_pvalue() %>%
                    rstatix::add_significance("p.adj")

#可视化绘制
stat.test <- stat.test %>% add_xy_position(x = "supp")
bxp2 <- ggboxplot(df, x = "supp", y = "len", fill = "supp",palette = "jco",
                 facet.by = "dose",
                 ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")) +
   labs(
    title = "Example of <span style='color:#D20F26'> add P.adj values in ggpubr</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>ggpubr::ggboxplot + rstatix::t_test</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>")+
   stat_pvalue_manual(stat.test, label = "p.adj") +
   scale_y_continuous(expand = expansion(mult = c(0.05, 0.10))) +
   theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))                 
```
![2021-05-07-09-47-36-559548.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352962575-10236407-f610-4fba-8274-42cc90598b1f.png#align=left&display=inline&height=810&id=u1fdcc555&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add P Values in group data<br />不喜欢ggsci包的颜色配色，可以使用黑灰色系进行颜色设置，修改成如下代码即可：
```python
ggboxplot(df, x = "supp", y = "len", fill = "supp",palette = c("gray80","gray20"),
                 facet.by = "dose",
                 ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed"))
```
设置`palette = c("gray80","gray20")`即可。
<a name="XvXnE"></a>
#### 「定义P值样式」：
如果觉得P值的样式比较单一，也可以自定义P值样式：
```python
#定义P值显示条件
stat.test$custom.label <- ifelse(stat.test$p.adj <=.05,stat.test$p.adj,"ns")
# 可视化绘制

stat.test <- stat.test %>% add_xy_position(x = "supp")
bxp4 <- ggboxplot(df, x = "supp", y = "len", fill = "supp",palette = "jco",
                 facet.by = "dose",
                 ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")) +
   labs(
    title = "Example of <span style='color:#D20F26'> add P.custom values in ggpubr</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>ggpubr::ggboxplot + rstatix::t_test</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>")+
   stat_pvalue_manual(stat.test, label = "custom.label") +
   scale_y_continuous(expand = expansion(mult = c(0.05, 0.10))) +
   theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-36-694207.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352951664-946f4150-df9d-4320-843a-5bd9444e26f2.png#align=left&display=inline&height=810&id=u9bb5070c&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Set P Value form
<a name="uPfSQ"></a>
#### 设置P值位科学计数法:
```python
# 添加科学计数法一列
+
stat.test$p.scient <- format(stat.test$p.adj, scientific = TRUE)
+ ····
stat_pvalue_manual(stat.test, label = "p.scient")+
# 其他同上
```
![2021-05-07-09-47-36-867249.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352937421-f91deff0-ef7b-49e4-9e07-88e9f7d44da9.png#align=left&display=inline&height=810&id=u32cf575f&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add P Value in scientific form<br />还可以绘制如下可视化结果：
```python
# 计算P值
stat.test <- df %>%
  t_test(len ~ supp, paired = TRUE) %>%add_significance()
# 可视化绘制
stat.test <- stat.test %>% add_xy_position(x = "supp") 

ggpaired(df, x = "supp", y = "len", fill = "supp",palette = "jco",
                  line.color = "gray", line.size = 0.4,
                  ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")) +
 labs(
    title = "Example of <span style='color:#D20F26'> add P.signif values in ggpubr</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>ggpubr::ggpaired + rstatix::t_test</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>")+
   stat_pvalue_manual(stat.test, label = "{p}{p.signif}") +
   scale_y_continuous(expand = expansion(mult = c(0.05, 0.10))) +
   theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-37-018092.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352922664-1fbb29db-c3f3-4907-b026-dd4579a2b39d.png#align=left&display=inline&height=810&id=u86c5b1c5&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add P Value in ggpaired example
<a name="kfX2D"></a>
#### 「柱形图P值添加」：
统计计算如下：
```python
# 统计计算
stat.test <- df %>% t_test(len ~ dose)%>% add_xy_position(fun = "mean_sd", x = "dose")
#可视化绘制
bp_p <- ggbarplot(df, x = "dose", y = "len", add = "mean_sd", fill = "dose", palette = "jco",
               ggtheme= hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")) +
   stat_pvalue_manual(stat.test, label = "p.adj.signif", tip.length = 0.01)+
   
   labs(
    title = "Example of <span style='color:#D20F26'> ggpubr::ggbarplot with p.adj.signif</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>ggpubr::ggbarplot + rstatix::t_test</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>")+
   theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-37-123861.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352898205-489c2c3b-24a9-40c2-a0f2-b385574db73e.png#align=left&display=inline&height=810&id=u1b5bc3d3&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add P Values in ggbarplot<br />具体的P值样式修改可参看上面代码。以上就是使用R-ggpubr包快速绘制P值的方法介绍，借助了R-rstatix包进行完成，下面就介绍一种更简单的绘制P值的方法。
<a name="BHLAG"></a>
### R-ggsignif添加P值
R-ggsignif 包可是专门为绘制P值的第三方包，其实用也较为简单，接下来通过三个小例子解释一下：
<a name="k7QSH"></a>
#### 「样例一」：
```python
ggplot(mpg, aes(class, hwy)) +
  geom_boxplot() +
  geom_signif(
    comparisons = list(c("2seater", "midsize"), c("minivan", "suv")),
    textsize = 6,map_signif_level = function(p) sprintf("P = %.2g", p)
  ) +
  ylim(NA, 48) +
   labs(
    title = "Example of <span style='color:#D20F26'>ggsignif::geom_signif function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_signif()</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>") +
  hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")  +
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-37-234569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352884182-a8c2abee-2bb0-4655-909d-c44c1d9d3746.png#align=left&display=inline&height=810&id=ue1744dc1&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add and Custom P Values in geom_signif()<br />注意：
```python
geom_signif(
    comparisons = list(c("2seater", "midsize"), c("minivan", "suv")),
    textsize = 6,map_signif_level = function(p) sprintf("P = %.2g", p)
)
```
添加P值并修改P值样式。
<a name="qnAAS"></a>
#### 「样例二」：
```python
ggplot(iris, aes(Species, Sepal.Width)) +
  geom_boxplot(aes(fill=Species)) +
  geom_signif(
        comparisons = list(c("versicolor", "setosa"),c("versicolor","virginica")),
        textsize = 6,map_signif_level = function(p) sprintf("P = %.3g", p),
        y_position = c(4.5, 4.))+
  scale_fill_jco()+
  ylim(NA, 5) +
   labs(
    title = "Example of <span style='color:#D20F26'>ggsignif::geom_signif function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_signif()</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>") +
  hrbrthemes::theme_ipsum(base_family = "Roboto Condensed")  +
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-37-999033.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352870969-e8761a10-6780-4ada-83cd-89beede9029f.png#align=left&display=inline&height=810&id=u5ddb12d0&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add and Custom P Values in geom_signif()<br />通过：
```python
y_position = c(4.5, 4.)
```
设置P值的具体添加位置。
<a name="VIv2t"></a>
#### 「样例三」：
```python
test_data<- data.frame(
  Group = c("S1", "S1", "S2", "S2"),
  Sub = c("A", "B", "A", "B"),
  Value = c(2, 5, 7, 8)
)
# 可视化绘制
ggplot(test_data, aes(x = Group, y = Value)) +
  geom_bar(aes(fill = Sub), stat = "identity", position = "dodge", width = .6) +
  geom_signif(
    y_position = c(5.3, 8.3), xmin = c(0.8, 1.8), xmax = c(1.2, 2.2),
    annotation = c("**", "NS")) +
  geom_signif(
    comparisons = list(c("S1", "S2")),size=.7,
    y_position = 9.3, vjust = 0.2)+ 
  scale_fill_grey() +
  labs(
    title = "Example of <span style='color:#D20F26'>ggsignif::geom_signif function</span>",
    subtitle = "processed charts with <span style='color:#1A73E8'>geom_signif() in geom_bar</span>",
    caption = "Visualization by <span style='color:#DD6449'>DataCharm</span>") +
  hrbrthemes::theme_ipsum(base_family = "Roboto Condensed") +
  theme(
    plot.title = element_markdown(hjust = 0.5,vjust = .5,color = "black",
                                  size = 20, margin = margin(t = 1, b = 12)),
    plot.subtitle = element_markdown(hjust = 0,vjust = .5,size=15),
    plot.caption = element_markdown(face = 'bold',size = 12))
```
![2021-05-07-09-47-38-090652.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620352854770-0c3ed174-d45e-4094-b4ad-ae95c2273c92.png#align=left&display=inline&height=810&id=u5d5695d0&originHeight=810&originWidth=1080&size=2629534&status=done&style=shadow&width=1080)<br />Add P Vlaus on geom_bar() in geom_signif()<br />通过：
```python
y_position = c(5.3, 8.3), xmin = c(0.8, 1.8), xmax = c(1.2, 2.2),
    annotation = c("**", "NS")

comparisons = list(c("S1", "S2")),size=.7,
    y_position = 9.3, vjust = 0.2
```
设置P值显示样式和样式(粗细、位置等)。<br />是不是觉得使用R-ggsignif包绘制P值更加方便些呢，更多属性设置和其他用法，可以去ggsigni包官网进行查阅。
