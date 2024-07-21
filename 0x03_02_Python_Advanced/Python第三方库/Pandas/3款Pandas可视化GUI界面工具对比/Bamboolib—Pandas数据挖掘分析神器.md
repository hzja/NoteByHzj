PythonPandas<br />介绍一款用于做EDA(探索性数据分析)的利器，并且可以**自动生成代码**，帮助大家极大节省工作时间与提升工作效率的利器，叫做Bamboolib。<br />大家可以将其理解为是Pandas的GUI扩展工具，所具备的功能有

- 查看DataFrame数据集与Series数据集
- 过滤数据
- 数据的统计分析
- 绘制交互式图表
- 文本数据的操作
- 数据清洗与类型转换
- 合并数据集
<a name="MV6ur"></a>
## 安装模块
在使用之前，先需要通过`pip install`进行该模块的安装
```bash
pip install bamboolib 
```
同时因为要在Jupyter Notebook以及JupyterLab上面用到该工具，因此还要安装额外的插件
```bash
# Jupyter Notebook extensions
python -m bamboolib install_nbextensions

# JupyterLab extensions
python -m bamboolib install_labextensions
```
<a name="x0vL4"></a>
## 查看DataFrame数据集
在上面的步骤全都完成之后，开始简单的来尝试使用一下bamboolib，导入需要用到的模块
```python
import bamboolib as bam
import pandas as pd
```
读取数据
```python
df = pd.read_excel(
    io="supermarkt_sales.xlsx",
    engine="openpyxl",
    sheet_name="Sales",
    skiprows=3,
    usecols="B:R",
    nrows=1000,
)

df
```
会弹出如下所示的界面，<br />![](./img/1656340204486-d114b329-4d21-4772-b711-c05c732063a1.gif)<br />先来简单介绍一下界面上的各个按钮，

- Explore DataFrame：对数据集进行探索性分析的按钮
- Create plot: 绘制交互性图表的按钮
- Search Transformations：包含对数据集进行各项操作
- Update: 过滤出指定的列
- Export: 可以将处理完的数据集以及代码导出

点击Explore DataFrame按钮来对数据先来一个大致的印象<br />![](./img/1656340204496-dfe71459-4e2e-497d-bb45-d63e02402653.gif)<br />看到会对数据集有一个大致的介绍，例如数据集是有1000行、18列，然后每一列的数据类型、每一列有多少的唯一值和缺失值都可以直观的看到<br />要是想要查看有着连续型变量的特征，它们之间的相关性，可以点击Correlation Matrix按钮<br />![](./img/1656340204456-c2bcc1e3-5fe3-4b5e-9229-7fe81f9950cb.gif)
<a name="veSZM"></a>
## 过滤数据
要是想要指定某一列数据的话，点击下拉框，选中select or drop columns，<br />![](./img/1656340204447-0e7bbe01-979b-45c9-a2f8-1b580addce35.gif)<br />或者想要删掉某一列的话，也是相类似的操作<br />![](./img/1656340204492-27197e11-d17a-412f-96ac-85a59bec2b95.gif)<br />当然如果想要根据特定的条件来过滤出某些数据的话，则是选中filter rows按钮，然后给出特定的条件，在Bamboolib模块当中有多种方式来过滤数据，有has values、contains、startswith、endswith等等，类似于Pandas模块当中对于文本数据处理的方法，例如想要挑选出“省份”这一列当中的“浙江省”的数据，就这么来做<br />![](./img/1656340204932-83a67b7c-2ba9-40b5-adc8-4064d38f8821.gif)<br />还能够对每行的数据进行排列，点击下拉框选中sort rows，例如以“毛利率”的大小来排序，并且是降序排序，就这么来做<br />![](./img/1656340204848-efb57b7a-c6e3-4b1c-97a2-b88c723d7fc3.gif)<br />要是想要对某一列的列名进行重命名，点击rename columns<br />![](./img/1656340205119-19337453-0e4a-48f9-b4f6-cfa951e27343.gif)
<a name="t3c9P"></a>
## 数据的清洗与类型转换
能够改变数据集当中某一列的数据类型，点击选中change column data dtype<br />![](./img/1656340205000-728423af-9ee7-4672-8971-42f889c566a6.png)<br />对于缺失值的情况，既可以选择去除掉这些缺失值，点击选中drop missing values或者是drop columns with missing values<br />![](./img/1656340205134-cd3d144d-df2e-45ee-b082-6d3d36e42c8b.png)<br />当然可以将这些缺失值**替代**为其他特定的值，无论是平均值或者是众数等等，点击选中find and replace missing values<br />![](./img/1656340205310-aeb8fa94-7c82-4c3d-be34-3094e9080681.png)
<a name="O36hG"></a>
## 数据的统计分析
可以通过bamboolib模块来对数据进行统计分析，例如计算数值的变化(percent change)，在下拉框中找到percent change的选项，然后对指定的列计算当中**数值的变化百分比**<br />![](./img/1656340205381-6b66ed6a-79d8-4240-ae7e-c1472a4ce58b.png)<br />还能够进行累乘/累加的操作，在下拉框中选中cumulative product或者是cumulative sum<br />![](./img/1656340205550-ea558e15-d31c-4314-8718-011716ee2ddb.png)<br />另外还能进行分组统计的计算操作，选中下拉框当中的group by and aggregate按钮，例如以“省份”来分组，计算“总收入”的平均值，可以这么来操作<br />![](./img/1656340205565-cb87608c-3b0e-4446-8e87-8b5ede3647d3.gif)
<a name="K9zcY"></a>
## 合并数据集
要是想要进行合并数据集的操作，在下拉框选中join/merge dataframes选项，当中有四种合并的方式，分别是inner join、left join、right join和outer join，然后选择合并的<br />![](./img/1656340205704-78bad731-e576-4532-b6d8-34fd61c14395.png)
<a name="Cy5Y5"></a>
## 文本数据的操作
还能对数据集当中的文本数据进行各种操作，包括对英文字母大小写的转换，就用convert to lowercase/convert to uppercase<br />![](./img/1656340205779-98d7a5fc-3491-4ad6-8658-e3830d425b52.png)<br />如果需要对字符串当中的空格做一个处理，在下拉框当中选中Remove leading and trailing whitespaces

![](./img/1656340205786-4359e743-ef4d-4b02-b3e5-b362f6a84c0f.png)<br />而要是需要对字符串做一个分割，就在下拉框中选中split text column<br />![](./img/1656340205893-c0d3b892-57b7-471d-83b8-b216ad3511ff.png)
<a name="Ccm1S"></a>
## 绘制交互式的图表
同时还能够通过该模块来绘制交互式的图表，点击“create plot”按钮，能看到在图表类型当中有直方图、柱状图、折线图、饼图等十来种，来绘制当中的一种<br />![](./img/1656340206087-de0f5fbe-4f67-4d78-a233-d146bda14ea2.png)<br />在X轴上指定的是“省份”的数据，然后**根据不同的商品类型配上不同的颜色**，而从绘制出来的结果中能够看到来自“**北京**”的顾客“**食品饮料**”购买的比较多，而来“**浙江**”的顾客“**服装服饰**”购买的比较多<br />![](./img/1656340206198-caf96a5f-1ff0-4ad5-8a25-871584e01aba.gif)<br />当然除了直方图之外，还有其他很多种图表可以通过该模块来绘制，这里由于篇幅有限，就不一一演示。
