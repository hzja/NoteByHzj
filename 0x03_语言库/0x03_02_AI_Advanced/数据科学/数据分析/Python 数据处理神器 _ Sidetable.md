Python 数据处理<br />Pandas 是数据科学社区中最流行的 Python 包，它包含许多函数和方法来分析和处理数据。尽管它的功能对于数据分析来说足够有效，但定制的库可以为 Pandas 带来更多的价值。<br />Sidetable 就是一个开源 Python 库，它是一种可用于数据分析和探索的工具，作为 `value_counts` 和 crosstab 的功能组合使用的。
<a name="Y709z"></a>
### 安装
可以使用从 PyPI 安装 Sidetable
```bash
pip install sidetable
```
<a name="CBAq0"></a>
### 用法
使用从 Kaggle 下载的 Titanic 数据集来实现该库。<br />sidetable 的思想是减少数据分析所需的代码行数并加快工作流程。对于任何数据集，都需要执行一些数据分析任务，包括可视化特征分布、频率计数、缺失记录计数。<br />使用 Titanic 数据集详细讨论 Sidetable 库的特性。
<a name="X8ybb"></a>
#### 1、`freq()`
Pandas 提供了 `value_counts()` 函数，用于计算特征的频率计数。Pandas 可以计算分布计数和概率分布，但可能希望更容易组合这些值。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064382977-0ca20aca-4e6b-4655-a4ef-83aa9cdbeb26.webp#clientId=u2032e77b-8c7f-4&from=paste&id=ub944f355&originHeight=231&originWidth=355&originalType=url&ratio=1&status=done&style=shadow&taskId=u84a979ae-e3fb-481c-89aa-91980a014d3)<br />分布计数和概率分布可以结合使用，但需要大量的输入和代码记忆。<br />对于 sidetable，使用 `freq()` 函数在一行Python代码中实现它更简单。可以获得累计总数、百分比和更大的灵活性。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064383014-25cd36a6-9cc4-475a-b59a-14f2318099fb.webp#clientId=u2032e77b-8c7f-4&from=paste&id=u009b1aee&originHeight=156&originWidth=552&originalType=url&ratio=1&status=done&style=shadow&taskId=uabde81e3-17ac-4488-b094-8baedbcca96)<br />除此之外，还可以对多个列进行分组，以可视化已分组要素的分布。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064383000-09371aca-b7cd-4077-b0c9-80a9bf7ffe39.webp#clientId=u2032e77b-8c7f-4&from=paste&id=udfd41f1b&originHeight=221&originWidth=600&originalType=url&ratio=1&status=done&style=shadow&taskId=ud08b1da7-1802-4267-a176-eabc1d5dcbc)<br />还可以使用参数 value 指定要素列，以指示分组的数据“sum”应基于特定列。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064383076-08bd7091-500c-4a26-85e5-2981fac0f1cf.webp#clientId=u2032e77b-8c7f-4&from=paste&id=udd679c90&originHeight=189&originWidth=554&originalType=url&ratio=1&status=done&style=shadow&taskId=u224ebc88-193e-4ae1-9202-7cdae96620a)
<a name="se33y"></a>
#### 2、Counts
sidetable 中的 `counts()` 函数可以生成一个汇总表，该汇总表可用于确定需要考虑为分类或数值的特征，以便进一步分析和建模。`counts()` 函数显示特征的唯一值的数量以及最频繁和最不频繁的值。<br />![2021-09-08-09-26-34-011601.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631064761326-7c04a3ee-f650-4939-ad42-27963e8fb1f5.png#clientId=u2032e77b-8c7f-4&from=ui&id=u9d95b5f0&originHeight=464&originWidth=912&originalType=binary&ratio=1&size=12250&status=done&style=shadow&taskId=ue4a06485-f344-4a47-854f-6936dc0c9fe)<br />可以使用 `exclude` 和 `include` 参数从数据集中排除或包含特定数据类型。
<a name="NFIid"></a>
#### 3、`missing()`
sidetable 中 `missing()`函数生成一个汇总表，该汇总表按每列的总缺失值的计数和百分比显示缺失记录。![2021-09-08-09-26-34-140600.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631064761342-3c4c0552-1b6f-4bdb-9515-35088620639a.png#clientId=u2032e77b-8c7f-4&from=ui&id=hMrZG&originHeight=458&originWidth=321&originalType=binary&ratio=1&size=6205&status=done&style=shadow&taskId=uf3d240e1-db3a-4564-a955-9273379343d)
<a name="Zhz0G"></a>
#### 4、`subtotal()`
Sidetable 中 `subtotal()` 函数最适合与 Pandas 中的 `group by` 函数一起使用。它可用于计算数据帧分组的一个或多个级别的小计。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064383858-d04fed66-bcaa-4745-a847-0e601c62ccf7.webp#clientId=u2032e77b-8c7f-4&from=paste&id=uc1b1a92c&originHeight=375&originWidth=1050&originalType=url&ratio=1&status=done&style=shadow&taskId=uf716bf1e-0737-4abf-8563-dc13800644f)<br />`subtotal()`函数可以将其添加到分组数据的一个或多个级别。需要首先使用`groupby()`函数对数据框进行分组，然后在每个级别添加一个小计。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631064384235-340c0e0e-710c-4f9f-b61d-edb2de9a8ab4.webp#clientId=u2032e77b-8c7f-4&from=paste&id=u7a74881b&originHeight=436&originWidth=546&originalType=url&ratio=1&status=done&style=shadow&taskId=u171ea105-dff3-455c-a8ab-db8dc449a35)
<a name="s0JLM"></a>
### 结论
Sidetable 是一种高效且方便的工具，它结合了 Pandas 的 `value_counts` 和 crosstab，生成一个可解释且易于理解的汇总表，还可用于提供分析结果。语法的简单性使其成为用于数据分析和探索的更好的库。
