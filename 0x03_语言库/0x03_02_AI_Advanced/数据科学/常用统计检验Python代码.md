Python<br />比较一个变量在不同组中的分布是数据科学中的一个常见问题。当想要评估一项策略(用户体验功能、广告活动、药物等)的因果效应时，因果推断的黄金标准便是随机对照试验，也就是所谓的A /B测试。在实践中，为研究选择一个样本，并将其随机分为对照组（control group）和实验组（treatment group）比较两组之间的结果。随机化确保了两组之间的唯一差异，这样就可以将结果差异归因于实验效果。<br />因为是随机的所以两组个体不会完全的相同（identical）。但是有时候，它们在总体表现时甚至不是“相似”的（similar）。例如，可能在一个群体中有更多的男性，或者年长的人，等等。(通常称这些特征为协变量或控制变量)。当这种情况发生时，就不能再确定结果的差异只是由于实验得来的。因此，随机化后，检查所有观察变量是否在组间平衡，是否没有系统差异是非常重要的。<br />在这里将看到比较两个(或更多)分布的不同方法，并评估它们差异的量级和重要性。将考虑两种不同的方法，可视化和统计。这两种方法可以提供直觉和严谨的数值来快速评估和探索差异，但是需要注意的是，这些方法很难区分这些差异是系统性的还是由于噪声造成的。<br />**样本**<br />假设需要对一组人进行实验并且已经将他们随机分为实验组和对照组。希望它们尽可能具有可比性，以便将两组之间的任何差异仅归因于实验效果。还将实验组分为不同的组，以检验不同实验方法的效果(例如，同一种药物的轻微变化)。<br />对于这个例子，模拟了1000个人的数据集，观察他们的一组特征。从src导入了数据生成进程`dgp_rnd_assignment()`。DGP和src.utils中的一些绘图函数和库。
```python
from src.utils import * 
from src.dgp import dgp_rnd_assignment 

df = dgp_rnd_assignment().generate_data() 
df.head()
```
![Data snapshot](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596020865-e0b0a475-e3bc-4bca-a795-a5f194bf86b0.png#averageHue=%23f3f3f3&clientId=u2853592a-a575-4&from=paste&id=u3cfe1dcd&originHeight=286&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u22dbef8c-2a59-4325-bf24-20c9781fc0b&title=Data%20snapshot "Data snapshot")<br />有1000个人的信息，观察他们的性别、年龄和周收入。每个人要么被分配到4个不同的实验组要么被分配到对照组。
<a name="sB8QF"></a>
## **两组数据对比--可视化**
从最简单的开始：想要比较整个实验组和对照组的收入分配。首先探索可视化方法，然后是统计方法。第一种方法的优点是可以使用直觉进行判断，第二种方法的优点是使用数字判断更加的严谨。<br />对于大多数可视化，这里将使用Python的seaborn库。
<a name="JgIUm"></a>
### **箱线图**
第一种视觉方法是箱线图。箱线图是汇总统计和数据可视化之间的良好折衷。框的中心代表中位数，而边框分别代表第1（Q1）和第3四分位数（Q3）。扩展线延伸到框外超过四分位距 (Q3 - Q1) 1.5 倍的第一个数据点。落在扩展线之外的点是单独绘制的，通常会被认为是异常值。<br />因此，箱线图提供了汇总统计数据（方框和扩展线）和直接数据可视化（异常值）。
```python
sns.boxplot(data=df, x='Group', y='Income'); 
plt.title("Boxplot");
```
![实验组和对照组的收入分配情况](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596020792-0d26ad98-04f0-4833-863d-4681d3b94b34.png#averageHue=%23f4f1ee&clientId=u2853592a-a575-4&from=paste&id=u063ff2b9&originHeight=718&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u62eaf767-d5a7-4b25-9063-6e0f4f9e407&title=%E5%AE%9E%E9%AA%8C%E7%BB%84%E5%92%8C%E5%AF%B9%E7%85%A7%E7%BB%84%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D%E6%83%85%E5%86%B5 "实验组和对照组的收入分配情况")<br />实验组的收入分配更加分散：橙色盒子更大，它的扩展线覆盖范围更广。但是箱线图的问题是它隐藏了数据的形状，它告诉我们一些汇总的统计数据，但没有显示实际的数据分布。
<a name="A9k52"></a>
### **直方图**
绘制分布图最直观的方法是直方图。直方图将数据分组到同等宽的容器（bin）中，并绘制出每个容器中的观察数据的数量。
```python
sns.histplot(data=df, x='Income', hue='Group', bins=50); 
plt.title("Histogram");
```
![实验组和对照组的收入分配](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596020834-6abd6dbc-0413-4b6d-8a21-1966da2fd227.png#averageHue=%23fbfbfb&clientId=u2853592a-a575-4&from=paste&id=u0afb1bcb&originHeight=732&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u0273bb74-2258-46cd-bf0b-fa9cd4e8ae9&title=%E5%AE%9E%E9%AA%8C%E7%BB%84%E5%92%8C%E5%AF%B9%E7%85%A7%E7%BB%84%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D "实验组和对照组的收入分配")<br />直方图也存在一些问题

- 由于两组的观察次数不同，因此两个直方图不具有可比性
- bin的数量是任意的

可以使用 stat 选项来绘制密度而不是计数来解决第一个问题，并将 common_norm 设置为 False 分别对每个直方图进行归一化。
```python
sns.histplot(data=df, x='Income', hue='Group', bins=50, stat='density', common_norm=False); 
plt.title("Density Histogram");
```
![实验组和对照组的收入分配](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596020765-5b6b7f1f-e7e5-48aa-9340-ce10a6ba19be.png#averageHue=%23f9f8f7&clientId=u2853592a-a575-4&from=paste&id=ue2e06596&originHeight=707&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u069f6c30-849a-46b3-b244-4c27cc0a8eb&title=%E5%AE%9E%E9%AA%8C%E7%BB%84%E5%92%8C%E5%AF%B9%E7%85%A7%E7%BB%84%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D "实验组和对照组的收入分配")<br />**这样这两个直方图就具有可比性！**<br />但是一个重要的问题仍然存在：bin的大小是任意的。在极端特殊的情况下，如果将数据更少分组，最终会得到最多只有一个观察值的 bin，如果将数据分组更多，最终会只得到一个 bin。在这两种情况下，都无法判断。这是一个经典的偏差-方差权衡的问题。
<a name="euHro"></a>
### **核密度**
一种可能的解决方案是使用核密度函数，该函数尝试使用核密度估计 (KDE) 用连续函数逼近直方图。
```python
sns.kdeplot(x='Income', data=df, hue='Group', common_norm=False); 
plt.title("Kernel Density Function");
```
![实验组和对照组的收入分配](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596020825-b11c7665-56ca-4efb-9ee3-0e7c8417c42a.png#averageHue=%23faf9f9&clientId=u2853592a-a575-4&from=paste&id=u6cf7d5e2&originHeight=707&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u44458278-2027-4d01-8125-d25ebf53ce8&title=%E5%AE%9E%E9%AA%8C%E7%BB%84%E5%92%8C%E5%AF%B9%E7%85%A7%E7%BB%84%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D "实验组和对照组的收入分配")<br />从图中可以看到，收入核密度似乎在实验组中具有更高的方差，但是各组的平均值却是相似的。核密度估计的问题在于它有点像一个黑匣子，可能会掩盖数据的相关特征。
<a name="l9C77"></a>
### **累积分布**
两种分布更透明的表示是它们的累积分布函数(Cumulative Distribution Function)。在 x 轴（收入）的每个点，绘制具有相等或更低值的数据点的百分比。累积分布函数的主要优点是

- 不需要做出任何的选择（例如bin的数量）
- 不需要执行任何近似（例如使用 KDE），图形代表所有数据点
```python
sns.histplot(x='Income', data=df, hue='Group', bins=len(df), stat="density", 
             element="step", fill=False, cumulative=True, common_norm=False); 
plt.title("Cumulative distribution function");
```
![实验组和对照组的累计收入分布](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596021286-fef4fe11-ef38-478c-87d0-4b8ee139fee0.png#averageHue=%23faf9f9&clientId=u2853592a-a575-4&from=paste&id=ud397a8eb&originHeight=728&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u1b08e858-7f27-4e98-8cd4-52afa8903b1&title=%E5%AE%9E%E9%AA%8C%E7%BB%84%E5%92%8C%E5%AF%B9%E7%85%A7%E7%BB%84%E7%9A%84%E7%B4%AF%E8%AE%A1%E6%94%B6%E5%85%A5%E5%88%86%E5%B8%83 "实验组和对照组的累计收入分布")<br />应该如何解释这张图?

- 由于这两条线在0.5 (y轴)处或多或少交叉，这意味着它们的中值是相似的
- 因为橙色线在左边的蓝线之上，在右边的蓝线之下，这意味着实验组的分布是fatter tails（肥尾）
<a name="wSk44"></a>
### **QQ图**
一种相关的方法是 QQ 图，其中 q 代表分位数。QQ 图绘制了两个分布的分位数。如果分布相同应该得到一条 45 度线。<br />Python 中没有原生的 QQ 图功能，而 statsmodels 包提供了 qqplot 功能，但相当麻烦。因此，将手动完成。<br />首先，需要使用 percentile 函数计算两组的四分位数。
```python
income = df['Income'].values 
income_t = df.loc[df.Group=='treatment', 'Income'].values 
income_c = df.loc[df.Group=='control', 'Income'].values 

df_pct = pd.DataFrame() 
df_pct['q_treatment'] = np.percentile(income_t, range(100)) 
df_pct['q_control'] = np.percentile(income_c, range(100))
```
现在可以绘制两个分位数分布，加上 45 度线，代表基准完美拟合。
```python
plt.figure(figsize=(8, 8)) 
plt.scatter(x='q_control', y='q_treatment', data=df_pct, label='Actual fit'); 
sns.lineplot(x='q_control', y='q_control', data=df_pct, color='r', label='Line of perfect fit'); 
plt.xlabel('Quantile of income, control group') 
plt.ylabel('Quantile of income, treatment group') 
plt.legend() 
plt.title("QQ plot");
```
![Q-Q plot](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596021439-f50e201f-2923-4402-a917-ccac121de7fd.png#averageHue=%23fafafa&clientId=u2853592a-a575-4&from=paste&id=uf7e6503e&originHeight=1053&originWidth=1033&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uadd2df01-07c0-452f-928c-76647c43ac7&title=Q-Q%20plot "Q-Q plot")<br />QQ 图在累积分布图方面提供了非常相似的信息：实验组的收入具有相同的中位数（线在中心交叉）但尾部更宽（点在左边的线以下，右边的线以上）。
<a name="Pg6QJ"></a>
## **两组数据对比--统计学方法**
到目前为止，已经看到了不同的方法来可视化分布之间的差异。可视化的主要优点是直观：可以观察差异并直观地评估它们。<br />然而，可能想要更加严格，并尝试评估分布之间差异的统计显着性，即 回答“观察到的差异是系统性的还是由于采样噪声？”的问题。<br />现在将分析不同的检验方法以区分两个分布。
<a name="PNGOz"></a>
### **T检验**
第一个也是最常见的是学生 t 检验。T 检验通常用于比较均值。要检验两组的收入分配均值是否相同。两均值比较检验的检验统计量由下式给出：<br />![](https://cdn.nlark.com/yuque/__latex/6ed6048d1d78648602426f6b71a43f7f.svg#card=math&code=s%20t%20a%20t%3D%5Cfrac%7B%5Cleft%7C%5Cbar%7Bx%7D_1-%5Cbar%7Bx%7D_2%5Cright%7C%7D%7B%5Csqrt%7Bs%5E2%20%2F%20n%7D%7D&id=hZhSS)<br />其中 x̅ 是样本均值，s 是样本标准差。在较温和的条件下，检验统计量作为学生 t 分布渐近分布。<br />使用 scipy 中的 ttest_ind 函数来执行 t 检验。该函数返回检验统计量和隐含的 p 值。
```python
from scipy.stats import ttest_ind 
 
stat, p_value = ttest_ind(income_c, income_t) 
print(f"t-test: statistic={stat:.4f}, p-value={p_value:.4f}") 
 
# t-test: statistic=-1.5549, p-value=0.1203
```
检验的p值为0.12，因此不拒绝实验组和对照组平均值无差异的零假设。
<a name="md10E"></a>
### **标准化平均差 (SMD)**
一般来说，当进行随机对照试验或 A/B 测试时，最好对实验组和对照组中所有变量的均值差异进行检验。<br />然而，由于 t 检验统计量的分母取决于样本量，因此 t 检验的 p 值难以跨研究进行比较。所以可能在一个差异非常小但样本量很大的实验中获得显着的结果，而在差异很大但样本量小的实验中可能会获得不显着的结果。<br />解决这个问题的一种解决方案是标准化平均差 (SMD)。顾名思义，这不是一个适当的统计量，而只是一个标准化的差异，可以计算为：<br />通常，低于0.1的值被认为是一个“小”的差异。<br />最将实验组和对照组的所有变量的平均值以及两者之间的距离度量（t 检验或 SMD）收集到一个称为平衡表的表中。可以使用causalml库中的create_table_one函数来生成它。正如该函数的名称所显示的那样，在执行A/B测试时，平衡表应该是你希望看到的的第一个表。
```python
from causalml.match import create_table_one 
 
df['treatment'] = df['Group']=='treatment' 
create_table_one(df, 'treatment', ['Gender', 'Age', 'Income'])
```
![平衡表](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596021529-4c2831a6-b7af-4274-a759-05f7eff1ce8a.png#averageHue=%23f3f3f3&clientId=u2853592a-a575-4&from=paste&id=uacc80c42&originHeight=274&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6c405fb7-a2b7-4220-a896-26cdd887927&title=%E5%B9%B3%E8%A1%A1%E8%A1%A8 "平衡表")<br />在前两列中，可以看到实验组和对照组的不同变量的平均值，括号中是标准误差。在最后一列中，SMD 的值表示所有变量的标准化差异均大于 0.1，这表明两组可能不同。
<a name="oxHm1"></a>
### **Mann–Whitney U检验**
另一种检验是 Mann-Whitney U 检验，它比较两个分布的中位数。该检验的原假设是两组具有相同的分布，而备择假设是一组比另一组具有更大（或更小）的值。<br />与上面看到的其他检验不同，Mann-Whitney U 检验对异常值不可知的。<br />检验过程如下。

- 合并所有数据点并对它们进行排名（按升序或降序排列）
- 计算 U₁ = R₁ - n₁(n₁ + 1)/2，其中 R₁ 是第一组数据点的秩和，n₁ 是第一组数据点的数量。
- 类似地计算第二组的 U₂。
- 检验统计量由 stat = min(U₁, U₂) 给出。

在两个分布之间没有系统等级差异的原假设下（即相同的中位数），检验统计量是渐近正态分布的，具有已知的均值和方差。<br />计算 R 和 U 背后的理论如下：如果第一个样本中的值都大于第二个样本中的值，则 R₁ = n₁(n₁ + 1)/2 并且作为结果，U 1 将为零（可达到的最小值）。否则如果两个样本相似，U1 和 U2 将非常接近 n1 n2 / 2（可达到的最大值）。<br />使用 scipy 的 mannwhitneyu 函数。
```python
from scipy.stats import mannwhitneyu 
 
stat, p_value = mannwhitneyu(income_t, income_c) 
print(f" Mann–Whitney U Test: statistic={stat:.4f}, p-value={p_value:.4f}") 
 
Mann–Whitney U Test: statistic=106371.5000, p-value=0.6012
```
得到的p值为0.6，这意味着不拒绝实验组和对照组的中位数没有差异的零假设。
<a name="mz4R7"></a>
### **置换检验**
一种非参数替代方法是置换检验。在原假设下，两个分布应该是相同的，因此打乱组标签不应该显着改变任何统计数据。<br />可以选择任何统计数据并检查其在原始样本中的值如何与其在组标签排列中的分布进行比较。例如使用实验组和对照组之间样本均值的差异作为检验统计。
```python
sample_stat = np.mean(income_t) - np.mean(income_c)stats = np.zeros(1000) 
for k in range(1000): 
    labels = np.random.permutation((df['Group'] == 'treatment').values) 
    stats[k] = np.mean(income[labels]) - np.mean(income[labels==False]) 
p_value = np.mean(stats > sample_stat) 

print(f"Permutation test: p-value={p_value:.4f}") 

Permutation test: p-value=0.0530
```
置换检验得到的 p 值为 0.053，这意味着在 5% 的水平上对原假设的弱不拒绝。那么应该如何解释 p 值？这意味着数据中均值的差异大于置换样本中均值差异的 1–0.0560 = 94.4%。<br />可以通过绘制检验统计在排列中的分布与其样本值的分布来可视化。
```python
plt.hist(stats, label='Permutation Statistics', bins=30); 
plt.axvline(x=sample_stat, c='r', ls='--', label='Sample Statistic'); 
plt.legend(); 
plt.xlabel('Income difference between treatment and control group') 
plt.title('Permutation Test');
```
![排列间的平均差分布](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596092361-84406b0b-30c9-40d6-a458-2fb90a819bb0.png#averageHue=%23d5dde9&clientId=u2853592a-a575-4&from=paste&id=u5b1b8485&originHeight=747&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub71ac990-2889-46a9-bcf5-6e8e9b2e9ec&title=%E6%8E%92%E5%88%97%E9%97%B4%E7%9A%84%E5%B9%B3%E5%9D%87%E5%B7%AE%E5%88%86%E5%B8%83 "排列间的平均差分布")<br />正如看到的，样本统计量相对于置换样本中的值是相当极端的，但并不过分。
<a name="FFLtU"></a>
### **卡方检验**
卡方检验是一种非常强大的检验，主要用于检验频率差异。<br />卡方检验最不为人知的应用之一是检验两个分布之间的相似性。这个想法是对两组的观察结果进行分类。如果两个分布相同，会期望每个 bin 中的观察频率相同。这里重要的一点是需要在每个 bin 中进行足够的观察，以使检验有效。<br />生成与对照组中收入分布的十分位数相对应的bin，然后如果两个分布相同，计算实验组中每个bin中的预期观察数。
```python
# Init dataframe 
df_bins = pd.DataFrame() 

# Generate bins from control group 
_, bins = pd.qcut(income_c, q=10, retbins=True) 
df_bins['bin'] = pd.cut(income_c, bins=bins).value_counts().index 

# Apply bins to both groups 
df_bins['income_c_observed'] = pd.cut(income_c, bins=bins).value_counts().values 
df_bins['income_t_observed'] = pd.cut(income_t, bins=bins).value_counts().values 

# Compute expected frequency in the treatment group 
df_bins['income_t_expected'] = df_bins['income_c_observed'] / np.sum(df_bins['income_c_observed']) * np.sum(df_bins['income_t_observed']) 

df_bins
```
![bin和频率](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596124641-f0143d38-45eb-4325-86f9-e06047d9e835.png#averageHue=%23efefef&clientId=u2853592a-a575-4&from=paste&id=u5f8fa6e5&originHeight=523&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u338f49fc-00c3-4201-8b8c-40c733a687f&title=bin%E5%92%8C%E9%A2%91%E7%8E%87 "bin和频率")<br />现在可以通过比较处理组中跨bin的预期 (E) 和观察 (O) 观察数来执行检验。检验统计量由下式给出<br />其中 bin 由 i 索引，O 是 bin i 中观察到的数据点数，E 是 bin i 中的预期的数据点数。由于使用对照组中收入分布的十分位数生成了 bin，因此预计处理组中每个 bin 的观察数在各个 bin 之间是相同的。检验统计量渐近分布为卡方分布。<br />为了计算检验统计量和检验的 p 值，使用 scipy 的卡方函数。
```python
from scipy.stats import chisquare 

stat, p_value = chisquare(df_bins['income_t_observed'], df_bins['income_t_expected']) 
print(f"Chi-squared Test: statistic={stat:.4f}, p-value={p_value:.4f}") 

Chi-squared Test: statistic=32.1432, p-value=0.0002
```
与上面介绍的所有其他检验不同，卡方检验强烈拒绝两个分布相同的原假设。这是为什么？<br />原因在于这两个分布具有相似的中心但尾部不同，并且卡方检验测试了整个分布的相似性，而不仅仅是中心，就像在之前的检验中所做的那样。<br />这个结果讲述了一个警示：在从 p 值得出盲目结论之前，了解实际检验的内容非常重要！
<a name="TFUSw"></a>
### **Kolmogorov-Smirnov 检验**
Kolmogorov-Smirnov检验的思想是比较两组的累积分布。特别是，Kolmogorov-Smirnov 检验统计量是两个累积分布之间的最大绝对差。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596202175-ab537cdb-a73b-4530-bd49-1b9e5c0d4914.png#averageHue=%23f9f9f9&clientId=u2853592a-a575-4&from=paste&id=u42361862&originHeight=142&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac35e908-6be4-40cb-867e-76c98ccbec1&title=)<br />其中 F₁ 和 F₂ 是两个累积分布函数，x 是基础变量的值。Kolmogorov-Smirnov 检验统计量的渐近分布是 Kolmogorov 分布。<br />为了更好地理解，绘制累积分布函数和检验统计量。首先计算累积分布函数。
```python
df_ks = pd.DataFrame() 
df_ks['Income'] = np.sort(df['Income'].unique()) 
df_ks['F_control'] = df_ks['Income'].apply(lambda x: np.mean(income_c<=x)) 
df_ks['F_treatment'] = df_ks['Income'].apply(lambda x: np.mean(income_t<=x)) 
df_ks.head()
```
![累积分布数据集](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596202125-d3425d46-6289-4aab-bfec-1bfaee1c9c64.png#averageHue=%23f4f4f4&clientId=u2853592a-a575-4&from=paste&id=ua833fb93&originHeight=286&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u4fc7eb13-197c-414a-9096-7144a7c6df7&title=%E7%B4%AF%E7%A7%AF%E5%88%86%E5%B8%83%E6%95%B0%E6%8D%AE%E9%9B%86 "累积分布数据集")<br />现在需要找到累积分布函数之间的绝对距离最大的点。
```python
k = np.argmax( np.abs(df_ks['F_control'] - df_ks['F_treatment'])) 
ks_stat = np.abs(df_ks['F_treatment'][k] - df_ks['F_control'][k])
```
可以通过绘制两个累积分布函数和检验统计量的值来可视化检验统计量的值。
```python
y = (df_ks['F_treatment'][k] + df_ks['F_control'][k])/2 
plt.plot('Income', 'F_control', data=df_ks, label='Control') 
plt.plot('Income', 'F_treatment', data=df_ks, label='Treatment') 
plt.errorbar(x=df_ks['Income'][k], y=y, 
             yerr=ks_stat/2, color='k', 
             capsize=5, mew=3, 
             label=f"Test statistic: {ks_stat:.4f}") 
plt.legend(loc='center right'); 
plt.title("Kolmogorov-Smirnov Test");
```
![Kolmogorov-Smirnov检验统计量](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596233174-bbc903b2-8a05-4680-8dcd-822f3adad3f5.png#averageHue=%23faf9f9&clientId=u2853592a-a575-4&from=paste&id=uaef9eccd&originHeight=718&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u57e19273-c57e-4321-9093-a57c98c9dd7&title=Kolmogorov-Smirnov%E6%A3%80%E9%AA%8C%E7%BB%9F%E8%AE%A1%E9%87%8F "Kolmogorov-Smirnov检验统计量")<br />从图中可以看出，检验统计量的值对应于收入~650 时的两个累积分布之间的距离。对于该收入值在两组之间存在最大的不平衡。<br />可以使用 scipy 中的 kstest 函数执行实检验。
```python
from scipy.stats import kstest 

stat, p_value = kstest(income_t, income_c) 
print(f" Kolmogorov-Smirnov Test: statistic={stat:.4f},
      p-value={p_value:.4f}") 

Kolmogorov-Smirnov Test: statistic=0.0974, p-value=0.0355
```
p 值低于 5%：以 95% 的置信度拒绝两个分布相同的原假设。
<a name="HhvgN"></a>
## **多组数据对比--可视化**

到目前为止，只考虑了两组的情况，但是如果有多个组呢？在上面看到的一些方法可以很好地扩展，而另一些则不能。<br />作为一个示例，现在将查看不同实验组的收入分配是否相同。
<a name="dzQn7"></a>
### **箱线图**
当有多组时，箱线图可以很好地扩展，因为可以并排放置不同的框。
```python
sns.boxplot(x='Arm', y='Income', 
            data=df.sort_values('Arm')); 
plt.title("Boxplot, multiple groups");
```
![各部门的收入分配箱线图](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596282257-46e02f01-65df-408c-9698-d265758d3986.png#averageHue=%23f4f2f1&clientId=u2853592a-a575-4&from=paste&id=ud0853957&originHeight=718&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubd6e77a7-b81b-4dc5-82aa-4cfe9c3cd8e&title=%E5%90%84%E9%83%A8%E9%97%A8%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D%E7%AE%B1%E7%BA%BF%E5%9B%BE "各部门的收入分配箱线图")<br />从图中可以看出，不同实验组的收入分配不同，编号越高的组平均收入越高。
<a name="FTrOi"></a>
### **提琴图**
结合汇总统计和核密度估计的箱线图的一个非常好的扩展是小提琴图。小提琴图沿 y 轴显示不同的密度，因此它们不会重叠。默认情况下，它还在里面添加了一个微型箱线图。
```python
sns.violinplot(x='Arm', y='Income', 
               data=df.sort_values('Arm')); 
plt.title("Violin Plot, multiple groups");
```
![各部门的收入分配提琴图](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596325773-8cb1a331-6b90-4a90-a0cb-29f03d7e5b34.png#averageHue=%23f2f0ee&clientId=u2853592a-a575-4&from=paste&id=u9ce9e852&originHeight=718&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u62102214-ff14-45b4-9729-5fe55dc4c2e&title=%E5%90%84%E9%83%A8%E9%97%A8%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D%E6%8F%90%E7%90%B4%E5%9B%BE "各部门的收入分配提琴图")<br />小提琴图表明不同实验组的收入不同。

<a name="P6GuH"></a>
### **山脊图**
山脊图沿 x 轴绘制了多个核密度分布，它比小提琴图更直观。在 matplotlib 和 seaborn 中都没有默认的山脊线图。素以需要joypy包。
```python
from joypy import joyplot 

joyplot(df, by='Arm', column='Income', 
        colormap=sns.color_palette("crest", as_cmap=True)); 
plt.xlabel('Income'); 
plt.title("Ridgeline Plot, multiple groups");
```
![各部门的收入分配山脊图](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596349831-9372ea73-8f35-4892-ade0-7451e3e81689.png#averageHue=%23f6f6f6&clientId=u2853592a-a575-4&from=paste&id=u1f241bcd&originHeight=725&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u1fcf58c3-59ca-46fb-b7bb-56ecf7f2719&title=%E5%90%84%E9%83%A8%E9%97%A8%E7%9A%84%E6%94%B6%E5%85%A5%E5%88%86%E9%85%8D%E5%B1%B1%E8%84%8A%E5%9B%BE "各部门的收入分配山脊图")<br />山脊图表明，编号越高的实验组收入越高。从这个图中也更容易理解分布的不同形状。
<a name="bFZz0"></a>
## **多组数据对比--统计学方法**
最后考虑比较多个组的假设检验。为了简单起见，将集中讨论最常用的一个:f检验。
<a name="hX8mm"></a>
### **F检验**
对于多个组最流行的检验方法是 F 检验。F 检验比较不同组间变量的方差。这种分析也称为方差分析。<br />F 检验统计量由下式给出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683596380663-cf3f3250-150a-443d-928b-998c95d3635d.png#averageHue=%23e9e9e9&clientId=u2853592a-a575-4&from=paste&id=ue53d2b06&originHeight=134&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubbb3629f-88ea-43f8-b150-7090c921ae3&title=)<br />其中 G 是组数，N 是观察数，x̅ 是总体平均值，x̅g 是组 g 内的平均值。在组独立性的原假设下，f 统计量是 F 分布的。
```python
from scipy.stats import f_oneway 

income_groups = [df.loc[df['Arm']==arm, 'Income'].values for arm in df['Arm'].dropna().unique()] 
stat, p_value = f_oneway(*income_groups) 
print(f"F Test: statistic={stat:.4f}, p-value={p_value:.4f}") 

F Test: statistic=9.0911, p-value=0.0000
```
检验 p 值基本上为零，这意味着强烈拒绝实验组之间收入分配没有差异的原假设。
<a name="NbsJC"></a>
## **总结**
在这里看到了很多不同的方法来比较两个或多个分布，无论是在可视化上还是在统计上。这是许多应用程序中的主要问题，尤其是在因果推断中，需要使随机化使实验组和对照组尽可能具有可比性。<br />还看到了不同的方法如何适用于不同的情况。视觉方法非常直观，但统计方法对于决策至关重要，因为需要能够评估差异的幅度和统计意义。
