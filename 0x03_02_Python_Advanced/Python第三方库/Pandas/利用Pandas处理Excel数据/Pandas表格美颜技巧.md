Python Pandas<br />美化Pandas的DataFrame的数据。主要是通过Pandas中的两个方法来实现：

1. `Styler.applymap`：逐个元素，返回带有CSS属性-值对的单个字符串
2. `Styler.apply`：列、表、行的方式，返回具有相同形状的Series或者DataFrame，其中每个值都是带有CSS属性值对的字符串。该方法在作用的时候，通过参数`axis`来传递，`axis=0`表示按列作用，`axis=1`表示按行作用。

官网学习地址：

- [https://pandas.pydata.org/pandas-docs/version/1.1.5/user_guide/style.html#Export-to-Excel](https://pandas.pydata.org/pandas-docs/version/1.1.5/user_guide/style.html#Export-to-Excel)
- [https://pandas.pydata.org/pandas-docs/stable/user_guide/style.html](https://pandas.pydata.org/pandas-docs/stable/user_guide/style.html)

![](./img/1638508976787-88bc9b2d-1b4a-4692-8836-69b611b84601.webp)
<a name="FIiLT"></a>
## 模拟数据
```python
import pandas as pd
import numpy as np

# 设置随机种子，保证每次运行结果相同
np.random.seed(20)  

# 模拟数据
df = pd.DataFrame({'A': np.linspace(1, 10, 10)})
# 将两个DataFrame进行合并
df1 = pd.concat([df, pd.DataFrame(np.random.randn(10,4), columns=list("BCDE"))],axis=1)
df1
```
numpy中的`linspace(start,stop,setp)`：表示按照步长取数，包含头尾的数据：<br />![](./img/1638508976766-9fd82c02-c482-4cf8-9a76-84af683256e4.webp)<br />为了查看空值的效果，特意设置了3个空值nan：<br />![](./img/1638508976716-8872020f-3b3c-4902-859b-4b8ad1f7f933.webp)
<a name="UI4lk"></a>
## 查看和隐藏样式
首先是查看DataFrame的样式设置：<br />![](./img/1638508976795-00d785d0-d715-4a29-ac61-a920bc61187c.webp)<br />可以通过`render`方法来查看数据框样式的具体值：发现都是默认的CSS样式的代码<br />![](./img/1638508983277-c8888457-92d6-4c2b-800c-60d9f17bd2bd.webp)
<a name="YOI7g"></a>
### 隐藏索引
隐藏index的代码：<br />![](./img/1638508983280-a0f1c35c-32cc-47c7-b1a6-8525eba98a55.webp)
<a name="QoNkA"></a>
### 隐藏指定的列
可以发现AC两个列被隐藏了，使用的是参数`subset`参数：<br />![](./img/1638508983692-a32b5928-4679-435a-bea6-c0f496740565.webp)<br />下面是各种显示样式的案例讲解：
<a name="Xos9b"></a>
## 案例1：正负颜色
小于0显示红色，大于0显示蓝色
```python
def color_change(val):
    color = 'red' if val < 0 else 'blue'
    return 'color: %s' % color  

# 使用applymap并调用写好的函数
df1.style.applymap(color_change)
```
`applymap`方法是针对整个DataFrame的<br />![](./img/1638508983775-e0d2ec44-5834-4236-b515-3abf48e1a2b9.webp)
<a name="etbI6"></a>
## 案例2：高亮显示数据
可以高亮显示最大、最小和缺失值。手写一个简单的高亮函数：
```python
def highlight_min(x):
    is_min = x == x.min()
    return ["background-color: yellow" if v else '' for v in is_min]
```
![](./img/1638508984338-6ad9b58a-3cf9-4edb-9929-1ef61949c6b4.webp)
<a name="aWFDF"></a>
## 实例3：使用默认高亮函数
Pandas中已经默认写好了内置的3个高亮函数：

- `highlight_max()`：最大
- `highlight_min()`：最小
- `highlight_null()`：空值

同时还可以搭配参数axis来显示行或者列
<a name="OOOTA"></a>
### 1、高亮最大值
![](./img/1638508986193-3fdfbe45-6a50-4637-ab8b-31855f58bb73.webp)
<a name="Mx0qz"></a>
### 2、高亮每列的最小值
![](./img/1638508986909-29ee5f5e-7b89-4275-bcca-407d0dc8d040.webp)
<a name="tR873"></a>
### 3、高亮空值：默认是红色
![](./img/1638508987305-972a83ce-9bb2-4f23-a5a0-da62b5aaa595.webp)<br />当然是可以改变颜色的，通过使用参数`null_color`：<br />![](./img/1638508987500-39a0599a-db5c-4be0-9ad4-e2c7dc8aacdd.webp)<br />使用参数`axis=1`，表示在行的方向上进行操作：<br />![](./img/1638508987661-f39d7d41-9f32-4b29-aacf-697c6586031a.webp)
<a name="FI2py"></a>
## 案例4：链式调用
链式调用指的是在同一个样式操作中同时使用多个函数（方法）：
```python
# color_change + highlight_max
df1.style.applymap(color_change).apply(highlight_min)

如果方法特别多，可以分行写：
# color_change + highlight_max
df1.style.\
applymap(color_change).\
apply(highlight_min)

# 或者：外面再加层括号，表示整个代码是一个整体
(df1.style.
applymap(color_change).
apply(highlight_min))
```
![](./img/1638508989123-fbd1aece-fdd2-446d-9bd5-7ce356598304.webp)<br />空值高亮和改变颜色参数同时使用，实现链式调用：<br />![](./img/1638508989430-9b57e148-0ba4-4eaa-93cb-490200e9fa92.webp)<br />隐藏列属性和自定义参数同时使用：<br />![](./img/1638508989657-c038f766-705b-4860-8d68-55566caf15c9.webp)
<a name="CEfK7"></a>
## 实例5：部分数据美化
使用参数`subset`参数控制作用的行列，传入的参数可以是：

- 列标签
- 列表（`numpy`数组）
- 元组（`row_indexer`， `column_indexer`）
<a name="Xr7QG"></a>
### 部分列属性
通过参数`subset`来指定我们想要美化的列属性：<br />![](./img/1638508990506-0c45f8d4-a76d-44dc-a8ea-e149223cbfb6.webp)
<a name="FglCM"></a>
### `pd.IndexSlice`构造列表
通过`pd.IndexSlice`来构造<br />![](./img/1638508990927-10e89480-873d-4dc8-8331-529c03f7fb12.webp)
<a name="fdtPz"></a>
## 实例6：值的格式化显示（`Styler.format`）
`Styler.format`来控制数值的格式化输出，类似于Python中的字符串格式化
<a name="NMOK7"></a>
### 整体显示
比如想要全部数据只显示两位小数：<br />![](./img/1638508991721-aae72bc1-a1cf-433f-89d5-26b1ff52d38c.webp)
<a name="FUPvK"></a>
### 使用字典，格式化特定列
通过字典的形式，列属性名当做键，格式说明当做值，进行格式化设置：<br />![](./img/1638508993085-608c7d31-6e13-4bf8-a9a9-780eb39c4eb5.webp)
<a name="hbAiK"></a>
### 使用`lambda`匿名函数
![](./img/1638508993101-53c07872-6ab2-4296-9351-5b05c75003af.webp)<br />![](./img/1638508993449-4cc9bd9c-158c-4154-836e-c3f020592d96.webp)
<a name="TgJRh"></a>
### 空值显示
通过参数`na_rep`，将空值进行替代：<br />![](./img/1638508993427-7ae56418-ade9-4604-b932-60c2315db3b0.webp)<br />用“空值”两个字来替代：<br />![](./img/1638508994197-ea8ada4c-4194-4ae4-9795-2634edba4bce.webp)<br />链式调用同时使用“-”来替代：<br />![](./img/1638508995242-a7988a27-f744-4681-b60a-6d973b1e31ed.webp)
<a name="N5sV0"></a>
### 表情符
使用emoji表情符：<br />![](./img/1638508996028-5c54cb8f-8360-41a5-9231-0fa1eac1e4dc.webp)
<a name="sVGOH"></a>
## 实例7：内置样式
<a name="HHOf9"></a>
### 内置函数
在这里使用的是内置的空值高亮函数：<br />![](./img/1638508996033-f8b5fc69-f0df-4945-90af-baefb664e964.webp)
<a name="oXh28"></a>
### 热力图制作
借助`seaborn`库来实现：
```python
import seaborn as sns

cm = sns.light_palette("blue", as_cmap=True)
s = df1.style.background_gradient(cmap=cm)
s 
```
![](./img/1638508996561-1c146c71-52d1-4924-abe1-b137817bb72e.webp)
<a name="sCvzT"></a>
### 连续色谱
![](./img/1638508996993-958d803c-3e86-4a7b-ab52-26765e5f4870.webp)<br />通过参数`low`和`high`来指定色谱的范围：<br />![](./img/1638508998544-dc3396c9-9a3e-470f-b5c6-d0abc268e767.webp)
<a name="GE5yC"></a>
### `set_properties`使用
```python
### set_properties使用

df1.style.set_properties(**{"background":"yellow",  # 背景色
                           "color":"red",  # 字体
                           "border-color":"black"})  # 边界
```
![](./img/1638508998580-63870af0-da80-43d3-bc4a-8a95aba70e24.webp)
<a name="pT8cJ"></a>
## 实例8：个性化条形图
<a name="pYiL4"></a>
### 默认方式
通过`bar`方法来操作：<br />![](./img/1638508998944-c87fb000-ce44-4f22-953a-2c8a5f8a0d39.webp)
<a name="oOs0P"></a>
### 参数`align`
有3种对齐方式：

- `left`：最小值从单元格的左侧开始
- `zero`：零值位于单元格的中心
- `mid`：单元格中的中心在（max-min)/2处

![](./img/1638508999565-8033e7ed-69ec-4c54-8a30-2032247e30a0.webp)<br />![](./img/1638508999882-1e3408d7-2b98-4635-a6b1-ebb50983b991.webp)<br />![](./img/1638509001209-58361bb5-1a6e-474d-b69c-fade79ad6ab9.webp)
<a name="lyhOo"></a>
## 实例9：样式共享
假设某个一个DataFrame创建了一个样式，然后想在另一个DataFrame中直接使用这个样式，该怎么做呢？<br />![](./img/1638509001467-b179f245-161e-4f3a-829a-1720361bf3df.webp)<br />先创建一个样式style1：<br />![](./img/1638509001466-d00deb03-a10f-4b8d-83bd-55e43e8c45c0.webp)<br />将style1运用到style2中：<br />![](./img/1638509002063-d85016f1-e3d1-4569-bab5-d6dc3ac6dac1.webp)
<a name="epuGe"></a>
## 实例10：设置精度`set_precision`
通常是给数据框中的数据指定精度（小数位）：<br />![](./img/1638509002210-1a2dbbc1-3de3-491b-b9a7-d9564ebf9d90.webp)<br />![](./img/1638509003766-4b03d8c1-1a77-40fb-a90b-674a4d3f49a2.webp)
<a name="xiIRy"></a>
## 实例11：设置标题
使用`set_caption`方法<br />![](./img/1638509004745-20d4a1df-e107-4fcb-80cd-d1cb15bf538b.webp)
<a name="Fo8Ds"></a>
## 实例12：缺失值设置
使用的是`set_na_rep`函数：
```python
(df1.style.set_na_rep("FAIL")
    .format(None, na_rep="PASS", subset=["D"])
    .highlight_null("yellow"))
```
![](./img/1638509004816-d21a5c6e-345e-4c41-a3db-6244d6c8aae5.webp)
<a name="doMfN"></a>
## 实例13：综合案例
```python
(df1.style
  .set_na_rep('-')  # 设置空值
  .format({'C':'{:.4f}',  # 精度设置
           'D':'{:.4f}',
           'E':'{:.2%}'},na_rep="-")  # 精度和空值同时设置
  .highlight_null("green")  # 空值高亮
  .applymap(color_change,subset=['D'])  # D列使用color_change函数
  .apply(highlight_min,subset=['C'])  # C列使用highlight_min
  .background_gradient(cmap='cubehelix',subset=['B','D'])  # 背景色设置
  .bar(subset=['E'], align='mid', color=['#60BCD4','#6A1B9A'])  # 柱状图设置
  .set_caption("Title of Pandas Style")  # 标题设置
  .hide_index()  # 隐藏索引
  .hide_columns(subset=['A']))  # 隐藏A列
```
![](./img/1638509005243-97e2e9aa-6943-40aa-ac8b-6e04594ce1fb.webp)
<a name="TWb2A"></a>
## 实例14：终极武器
```python
# 版本至少需要pandas1.2.0
style1 = [
    dict(selector="th", props=[("font-size", "125%"), 
                               ("text-align", "center"),
                               ("background-color", "#F0F3CF"),
                               ('width',"100px"),
                               ('height','80px')]),
    
    dict(selector="td", props=[("font-size", "105%"), 
                               ("text-align", "right"),
                               ('width',"150px"),
                               ('height','50px')]),
    
    dict(selector="caption", props=[("caption-side", "top"),
                                    ("font-size","150%"),
                                    ("font-weight","bold"),
                                    ("text-align", "left"),
                                    ('height','50px'),
                                    ('color','#E74C3C')])]

style2 = {
    'A': [dict(selector='td', props=[('text-align','center'),
                                     ("font-weight","bold"),
                                     ("text-transform","capitalize")])],
    'B': [dict(selector='td', props=[('text-align','left'),
                                     ("font-style","italic")])],
    'C': [dict(selector='td', props=[('text-decoration','underline'),
                                     ('text-decoration-color','red'),
                                     ('text-decoration-style','wavy')])]}
# 链式调用各种设置方法
(df1.style
  .set_na_rep('-')  # 整体空值
  .format({'C':'{:.4f}','D':'{:.4f}','E':'{:.2%}'},na_rep="-")  # 精度
  .highlight_null("gray")  # 高亮空值
  .applymap(color_change,subset=['B']).highlight_max(subset=['A'])  # color_change函数和最大值高亮
  .background_gradient(cmap='Pastel1',subset=['C','D'])  #  背景
  .bar(subset=['E'], align='mid', color=['#90BCD4','#6A1B9A'])  # 柱状
  .set_caption("Advanced use of Pandas Style")  # 标题 
  .hide_index()  # 隐藏索引
  .hide_columns(subset=['E'])  # 隐藏E列  
  .set_table_styles(style1).set_table_styles(style2,overwrite=False)  # 样式传递
  .set_properties(**{'font-family': 'Microsoft Yahei','border-collapse': 'collapse',  # 属性设置
                     'border-top': '1px solid black','border-bottom': '1px solid black'}))
```
![](./img/1638509005254-d6ba70b6-6459-44cb-bc18-619995c5a367.webp)
<a name="CzpBC"></a>
## 输出到Excel
这是一个还在开发阶段的功能，将DataFrame使用openyxl或者xlswriter作为引擎导出到Excel表格中，官网是这样说的：<br />![](./img/1638509005409-2176ba51-6153-4dd3-a462-74e2c093450f.webp)<br />在这里看一个简单的案例：
```python
(df1.style
   .applymap(color_change)  # 正负改变颜色
   .apply(highlight_min)  # 高亮最小值
   .to_excel('styled.xlsx', engine='openpyxl'))
```
![](./img/1638509007180-f270bbff-db47-424f-91ed-f188e69ad693.webp)<br />如果不想要索引号，添加`index=False`：
```python
(df1.style
   .applymap(color_change)  # 正负改变颜色
   .apply(highlight_min)  # 高亮最小值
   .to_excel('styled.xlsx', engine='openpyxl', index=False))
```
![](./img/1638509007763-7a8f92ec-ad57-4deb-9dbe-6cbc60e979a1.webp)
