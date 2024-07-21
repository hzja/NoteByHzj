Python Pandas
<a name="U8U6g"></a>
## 1. 概述
先简单介绍一下什么是表格条件格式可视化，以常用的Excel为例说明。<br />在Excel菜单栏里，默认（选择）开始菜单，在中间部位有个条件格式控件，里面就是关于表格条件格式的方方面面。主要包含突出显示单元格规则、最前/最后规则、数据条、色阶、图标集以及规则管理等。<br />![2021-08-07-11-02-47-790293.png](./img/1628305698674-f3fc072e-3863-4802-9ce1-9178eb9cf1f9.png)<br />条件格式<br />基于以上，其实可以通过函数方式进行多种条件的综合，让Excel表格可视化丰富多彩，比如以下截图展示的就是色阶效果！<br />![2021-08-07-11-02-47-882351.png](./img/1628305711694-2f63b962-c96a-485e-baf5-533bc017778c.png)<br />色阶案例，数据截止8月5日<br />在上图中，对每列单独进行条件格式-色阶设置，绿色->红色 代表数值从小到大，可以很直观的快速感受数值表现。<br />所谓 表格条件格式可视化，就是对表格的数据按照一定的条件进行可视化的展示（这里的可视化更多是指单元格背景色、字体颜色以及文本格式显示等）。<br />那么，Pandas作为表格化的数据处理工具，可以如何实现表格条件格式可视化呢？！<br />那就是通过：`df.style`
<a name="RjvUb"></a>
## 2. 突出显示单元格
在Excel条件格式中，突出显示单元格规则提供的是大于、小于、等于以及重复值等内置样式，不过在Pandas中这些需要通过函数方法来实现，放在后续介绍。这里介绍Pandas突出显示缺失值、最大值、最小值、区间值的函数方法以及Excel实现这些操作的自定义操作。
<a name="nlavV"></a>
### 2.1. 高亮缺失值
`df.style.highlight_null()`
```python
Signature:
df.style.highlight_null(
    null_color: 'str' = 'red',
    subset: 'Subset | None' = None,
    props: 'str | None' = None,
) -> 'Styler'
Docstring:
Highlight missing values with a style.
```
`null_color`用于指定高亮的背景色，默认是红色<br />`subset`用于指定操作的列或行<br />`props`用于突出显示CSS属性（后面案例中会涉及到）<br />![2021-08-07-11-02-47-977296.png](./img/1628305776015-7c8a7672-941d-4721-bde9-5dd120a33633.png)<br />比如，可以指定高亮的背景色为橙色（颜色可以是英文名称）<br />![2021-08-07-11-02-48-098346.png](./img/1628305776041-44269149-574b-4382-aed1-9adc02d79365.png)<br />比如，可以指定高亮的背景色为紫红色（颜色可以是16进制）<br />![2021-08-07-11-02-48-204290.png](./img/1628305776049-cf645540-58c7-4559-807b-628dd7e0e558.png)
<a name="rUNmm"></a>
### 2.2. 高亮最大值
`df.style.highlight_max()`
```python
Signature:
df.style.highlight_max(
    subset: 'Subset | None' = None,
    color: 'str' = 'yellow',
    axis: 'Axis | None' = 0,
    props: 'str | None' = None,
) -> 'Styler'
Docstring:
Highlight the maximum with a style.
```
`subset`用于指定操作的列或行<br />`color`用于指定颜色，默认是黄色<br />`axis`用于指定行最大、列最大或全部，默认是列方向最大<br />![2021-08-07-11-02-48-293303.png](./img/1628305808954-e49a8528-93c7-425b-8fe6-704bd28b5252.png)<br />这里可以发现对于中文也有列最大高亮，为了避免出现这种情况，有两种方法：<br />①将这一列设置为索引(这里不做演示），<br />②采用`subset`指定<br />![2021-08-07-11-02-48-381290.png](./img/1628305808969-04b4d69d-f43d-42ff-ae71-cfc9a59b6fce.png)<br />指定颜色为灰色<br />![2021-08-07-11-02-48-464294.png](./img/1628305808969-a0e73962-b718-4754-954b-d2e73b75e941.png)<br />显示全部最大值<br />![2021-08-07-11-02-48-549298.png](./img/1628305853768-0ecb705b-84bb-48e9-814f-436417a4eb86.png)<br />那么，Excel如何显示最大值呢？这里以显示全部最大值为例展开介绍，逻辑如下：

1. 通过函数MAX获取数据区域的最大值
2. 然后编辑格式满足单元格值等于这个最大值即可

操作为：选中数据区域，进行条件格式设置->编辑格式规则<br />具体规则如下图：<br />![2021-08-07-11-02-48-668304.png](./img/1628305853803-b50a6002-0328-4865-b8d8-21911b2ae352.png)<br />就可以得到想要的效果：<br />![2021-08-07-11-02-48-754291.png](./img/1628305853808-658b088d-a3dc-4bf6-b0f5-7d89a1e2aaf3.png)<br />同样的道理，可以根据需求高亮列或行的最大值、最小值等
<a name="Cru3y"></a>
### 2.3. 高亮最小值
`df.style.highlight_min()`<br />参数基本同高亮最大值，这里不再赘述，看案例<br />![2021-08-07-11-02-48-849291.png](./img/1628305890698-a35a1563-cc67-44d6-96cf-9a0d11beabed.png)<br />链式调用 最大最小值高亮<br />![2021-08-07-11-02-48-948295.png](./img/1628305890705-d4062693-eddf-492a-9652-c2208b6b27ad.png)
<a name="kxYE8"></a>
### 2.4. 高亮区间值
`df.style.highlight_between`
```python
Signature:
df.style.highlight_between(
    subset: 'Subset | None' = None,
    color: 'str' = 'yellow',
    axis: 'Axis | None' = 0,
    left: 'Scalar | Sequence | None' = None,
    right: 'Scalar | Sequence | None' = None,
    inclusive: 'str' = 'both',
    props: 'str | None' = None,
) -> 'Styler'
Docstring:
Highlight a defined range with a style.
```
`subset`用于指定操作的列或行<br />`color`用于指定颜色，默认是黄色<br />`axis`用于指定行、列或全部，如果left或right作为序列给出，则应用于这些序列的边界<br />`left`用于指定区间最小值<br />`right`用于指定区间最大值<br />`inclusive`用于确定是否左右闭包，可选'both', 'neither', 'left', 'right'<br />`props`用于突出显示CSS属性<br />高亮数量在`[20, 30]`的单元格<br />![2021-08-07-11-02-49-054307.png](./img/1628305921378-22b984a7-ac1a-4d68-b889-344bc2a28bc3.png)<br />`props`用于突出显示CSS属性，案例中将待高亮的部分显示为字体颜色-白色，背景色-紫色<br />![2021-08-07-11-02-49-206306.png](./img/1628305921395-014adec2-dc68-4792-bc5e-582212f8d516.png)<br />金牌数区间[20, 30]、银牌数区间[10, 20]、铜牌数区间[5, 10]<br />![2021-08-07-11-02-49-302294.png](./img/1628305921406-06478eef-4312-4608-820b-24e1f8e2f542.png)
<a name="A9jJp"></a>
### 2.5. 高亮分位数
`df.style.highlight_quantile()`
```python
Signature:
df.style.highlight_quantile(
    subset: 'Subset | None' = None,
    color: 'str' = 'yellow',
    axis: 'Axis | None' = 0,
    q_left: 'float' = 0.0,
    q_right: 'float' = 1.0,
    interpolation: 'str' = 'linear',
    inclusive: 'str' = 'both',
    props: 'str | None' = None,
) -> 'Styler'
Docstring:
Highlight values defined by a quantile with a style.
```
`subset`用于指定操作的列或行<br />`color`用于指定颜色，默认是黄色<br />`axis`用于指定行、列或全部<br />`q_left`用于指定分位数左边界，默认是0<br />`q_right`用于指定分位数右边界，默认是1<br />`inclusive`用于确定是否左右闭包，可选'both', 'neither', 'left', 'right'<br />`props`用于突出显示CSS属性<br />比如，高亮各列奖牌数前15%的值<br />![2021-08-07-11-02-49-398295.png](./img/1628305957451-ded0e966-87db-42f5-971e-cf73c796d055.png)
<a name="JMq2H"></a>
## 3. 色阶（背景及文本渐变色）
色阶部分包含背景渐变色和文本渐变色
<a name="lyyMZ"></a>
### 3.1. 背景渐变色
在Excel中，直接通过条件格式->色阶 操作即可选择想要的背景渐变色效果<br />![2021-08-07-11-02-49-517296.png](./img/1628305957493-0ca0ca52-2c45-4217-8096-b33b9c434e66.png)<br />而在Pandas中，可以通过`df.style.background_gradient()`进行背景渐变色的设置。
```python
Signature:
df.style.background_gradient(
    cmap='PuBu',
    low: 'float' = 0,
    high: 'float' = 0,
    axis: 'Axis | None' = 0,
    subset: 'Subset | None' = None,
    text_color_threshold: 'float' = 0.408,
    vmin: 'float | None' = None,
    vmax: 'float | None' = None,
    gmap: 'Sequence | None' = None,
) -> 'Styler'
Docstring:
Color the background in a gradient style.
```
`cmap`用于指定matplotlib色条<br />`low`和`high`用于指定最小最大值颜色边界，区间[0, 1]<br />`axis`用于指定行、列或全部，默认是列方向<br />`subset`用于指定操作的列或行<br />`text_color_threshold`用于指定文本颜色亮度，区间[0, 1]<br />`vmin`和`vmax`用于指定与cmap最小最大值对应的单元格最小最大值<br />![2021-08-07-11-02-49-636300.png](./img/1628305957496-07856c7a-a074-4a8f-90b3-fb5ecb012a13.png)<br />`low`和`high`用于指定最小最大值颜色边界，区间[0, 1]<br />![2021-08-07-11-02-49-733301.png](./img/1628305957505-83470ecd-fce9-4b39-93e9-ba4ad73f0d26.png)<br />`cmap`用于指定matplotlib色条，采用seaborn美化样式<br />![2021-08-07-11-02-49-849297.png](./img/1628306010420-0b36dffe-08a9-408e-83a5-b07e4494ea72.png)<br />`text_color_threshold`用于指定文本颜色亮度，区间[0, 1]<br />![2021-08-07-11-02-49-968298.png](./img/1628306010447-9f167730-98d7-45b9-a133-ac645ec2717c.png)<br />`vmin`和`vmax`用于指定与cmap最小最大值对应的单元格最小最大值（10以下同色，70以上同色）<br />![2021-08-07-11-02-50-087292.png](./img/1628306010445-e934ccbf-069c-4c96-b358-8b1a69056493.png)<br />可以看到以上对于缺失值来说，其背景色是黑色，可以通过链式方法和高亮缺失值对缺失值背景色进行修改<br />![2021-08-07-11-02-50-185297.png](./img/1628306042115-d6297302-c611-4b12-92b3-c13e64439488.png)
<a name="J9vZ4"></a>
### 3.2. 文本渐变色
文本渐变色顾名思义就是对单元格的文本进行颜色渐变，可以通过`df.style.text_gradient()`来操作，其参数和背景渐变色基本一致。<br />![2021-08-07-11-02-50-278292.png](./img/1628306042120-1a7597ef-b7f0-42ae-8a59-76746b7f7a9c.png)
<a name="C6SQk"></a>
## 4. 数据条
在Excel中，直接通过条件格式->数据条 操作即可选择想要的数据条效果<br />![2021-08-07-11-02-50-360288.png](./img/1628306042133-feea2f0e-8361-44f9-9a0c-2f91afa108be.png)<br />而在Pandas中，可以通过 `df.style.bar()`来进行数据条绘制
```python
Signature:
df.style.bar(
    subset: 'Subset | None' = None,
    axis: 'Axis | None' = 0,
    color='#d65f5f',
    width: 'float' = 100,
    align: 'str' = 'left',
    vmin: 'float | None' = None,
    vmax: 'float | None' = None,
) -> 'Styler'
Docstring:
Draw bar chart in the cell backgrounds.
```
`subset`用于指定操作的列或行<br />`axis`用于指定行、列或全部，默认是列方向<br />`color`用于指定数据条颜色<br />`width`用于指定数据条长度，默认是100，区间[0, 100]<br />`vmin`和`vmax`用于指定与数据条最小最大值对应的单元格最小最大值<br />`align`数据条与单元格对齐方式，默认是left左对齐，还有zero居中和mid位于（max-min）/2<br />比如，奖牌数（不算总的）最低0最高40+颜色为橙色+居中展示，金牌差数据条长度为50（也就是单元格一半的长度）、银牌差mid对齐+数据条为单元格一半长度+正负显示不同颜色<br />![2021-08-07-11-02-50-453287.png](./img/1628306076138-63afb7e3-79a0-451c-b7c5-0036b970a313.png)
<a name="KXhJW"></a>
## 5. 数据格式化
调整数据格式用到`df.style.format()`
```python
Signature:
df.style.format(
    formatter: 'ExtFormatter | None' = None,
    subset: 'Subset | None' = None,
    na_rep: 'str | None' = None,
    precision: 'int | None' = None,
    decimal: 'str' = '.',
    thousands: 'str | None' = None,
    escape: 'str | None' = None,
) -> 'StylerRenderer'
Docstring:
Format the text display value of cells.
```
`formatter` 显示格式<br />`subset`用于指定操作的列或行<br />`na_rep`用于指定缺失值的格式<br />`precision`用于指定浮点位数<br />`decimal`用于用作浮点数、复数和整数的十进制分隔符的字符，默认是`.`<br />`thousands`用作浮点数、复数和整数的千位分隔符的字符<br />`escape`用于特殊格式输出（如html、latex等，这里不做展开，可参考官网）<br />比如，给数据加上单位枚，缺失值显示为无<br />![2021-08-07-11-02-50-527298.png](./img/1628306096305-383f167a-5406-4ffa-a0df-ae758c907460.png)<br />设置小数点位数为0<br />![2021-08-07-11-02-50-623292.png](./img/1628306096333-14ab66ab-132e-4573-965c-d2116d38a8e8.png)<br />指定列进行格式化<br />![2021-08-07-11-02-50-704288.png](./img/1628306096347-5e33b120-c005-40fc-977d-82955274f1ba.png)<br />分别对指定列进行单独格式化<br />![2021-08-07-11-02-50-789298.png](./img/1628306096344-0ed78e4c-4510-40a0-a43a-074c67ec505b.png)
<a name="swes6"></a>
## 6. 自定义格式函数
通过传递样式函数来自定义格式：<br />`applymap()` (elementwise)：接受一个函数，它接受一个值并返回一个带有 CSS 属性值对的字符串。<br />`apply()`(column-/ row- /table-wise): 接受一个函数，它接受一个 Series 或 DataFrame 并返回一个具有相同形状的 Series、DataFrame 或 numpy 数组，其中每个元素都是一个带有 CSS 属性的字符串-值对。此方法根据axis关键字参数一次传递一个或整个表的 DataFrame 的每一列或行。对于按列使用`axis=0`、按行使用`axis=1`，以及一次性使用整个表`axis=None`。<br />比如，定义一个函数，如果金牌数<银牌数，则高亮金牌数这一列对应的值<br />![2021-08-07-11-02-50-919301.png](./img/1628306146748-6abcb172-24ed-4edd-9155-51fbf09c1eda.png)<br />比如，还可以定义函数，如果金牌数<银牌数，则这一行数据都高亮<br />![2021-08-07-11-02-51-067314.png](./img/1628306146759-5a9649ae-90c7-4d04-b39c-72f040e99d5c.png)<br />又或者，可以根据不同的比值对每行进行不同的高亮<br />![2021-08-07-11-02-51-194386.png](./img/1628306146762-3ccbf378-2e00-4fb8-9ed9-0d33b745d930.png)<br />关于以上函数的写法，还可以调用numpy的`where`和`repeat`方法进行优化，如：<br />![2021-08-07-11-02-51-373389.png](./img/1628306146772-7b29eb9e-eaee-4841-9ee0-6f6ed467ebc1.png)
<a name="I8bs4"></a>
## 7. 其他
还有一些小操作，比如添加标题、隐藏索引、隐藏指定列等等
<a name="pw18b"></a>
### 添加标题
![2021-08-07-11-02-51-448372.png](./img/1628306211334-ab2d835e-2996-46b3-b401-ac0d7e81a90c.png)
<a name="yHTwj"></a>
### 隐藏索引
![2021-08-07-11-02-51-543411.png](./img/1628306211373-82749d6d-d212-4417-8c5d-37fe28dc5336.png)
<a name="RD8KC"></a>
### 隐藏指定列
![2021-08-07-11-02-51-657375.png](./img/1628306211382-1cf9d83e-a058-4ac8-a123-38454f1e296d.png)
<a name="lTEds"></a>
### 设置属性
如果一些单元格属性和单元格值无关，可以通过`df.style.set_properties()`来进行定制化操作，比如：背景色-黑色，字体颜色-草绿色，边框颜色-白色。（css样式）<br />![2021-08-07-11-02-51-733379.png](./img/1628306245695-4cdb28f7-91b9-4d73-828f-3310b26b1f5b.png)
<a name="HQg5H"></a>
### 选中放大
![2021-08-07-11-02-51-864387.png](./img/1628306245692-3be9c285-35dc-4ddd-9075-2577424521aa.png)<br />鼠标选择单元格会有放大效果<br />![2021-08-07-11-02-52-064375.gif](./img/1628306245750-ef8f04a8-efa2-4057-8898-e5bed9ce1acc.gif)<br />效果演示
<a name="gKAhh"></a>
### 导出Excel
就直接`to_excel`就行了，`dfs = df.style.xxx`，然后`dfs.to_excel()`<br />![2021-08-07-11-02-52-149373.png](./img/1628306245737-be5bdf89-68b2-4687-9fad-391906246402.png)<br />导出的excel截图
<a name="ldIAn"></a>
### 导出html
![2021-08-07-11-02-52-244379.png](./img/1628306245790-97461628-4e16-4df3-a6e1-fdf30c835420.png)

 
