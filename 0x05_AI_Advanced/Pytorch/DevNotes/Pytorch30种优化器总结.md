PyTorch
<a name="NERLp"></a>
## 1、SGD
随机梯度下降算法，每次参数更新时，仅仅选取一个样本计算其梯度。<br />算法：<br />![2022-07-01-09-58-41.888532000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640873720-aba83822-f106-497b-aec1-c477bc193ee5.png#clientId=u17442345-ccd6-4&from=ui&id=uf6e72ac5&originHeight=642&originWidth=670&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1293225&status=done&style=shadow&taskId=udd45a258-5339-4f08-96b4-b086ca3c293&title=)<br />函数：
```python
class torch.optim.SGD(params, lr=, momentum=0, dampening=0, weight_decay=0, nesterov=False)

'''
参数：
	params (iterable) – 待优化参数的iterable或者是定义了参数组的dict
	lr (float) – 学习率
	momentum (float, 可选) – 动量因子（默认：0）
	weight_decay (float, 可选) – 权重衰减（L2惩罚）（默认：0）
	dampening (float, 可选) – 动量的抑制因子（默认：0）
	nesterov (bool, 可选) – 使用Nesterov动量（默认：False）
'''

http://www.cs.toronto.edu/~hinton/absps/momentum.pdf
```
<a name="SiWUF"></a>
## 2、ASGD
实现平均随机梯度下降算法。<br />函数：
```python
class torch.optim.ASGD(params, lr=0.01, lambd=0.0001, alpha=0.75, t0=1000000.0, weight_decay=0)[source]

'''
参数：
	params (iterable) – 待优化参数的iterable或者是定义了参数组的dict
	lr (float, 可选) – 学习率（默认：1e-2）
	lambd (float, 可选) – 衰减项（默认：1e-4）
	alpha (float, 可选) – eta更新的指数（默认：0.75）
	t0 (float, 可选) – 指明在哪一次开始平均化（默认：1e6）
	weight_decay (float, 可选) – 权重衰减（L2惩罚）（默认:0）
'''

http://dl.acm.org/citation.cfm?id=131098
https://courses.cs.washington.edu/courses/cse547/18sp/slides/sgd_averaging.pdf
```
<a name="MUDOt"></a>
## 3、Adam
将Momentum算法和RMSProp算法结合起来使用的一种算法，既用动量来累积梯度，又使得收敛速度更快同时使得波动的幅度更小，并进行了偏差修正。<br />算法：<br />![2022-07-01-09-58-42.136684400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640909389-61124546-9244-4ab9-82ae-42f90726338a.png#clientId=u17442345-ccd6-4&from=ui&id=u08fbcee3&originHeight=678&originWidth=649&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1322960&status=done&style=shadow&taskId=uf74f0ce1-e5a2-4164-95af-61d61040738&title=)<br />函数：
```python
class torch.optim.Adam(params, lr=0.001, betas=(0.9, 0.999), eps=1e-08, weight_decay=0)[source]

'''
参数：
	betas (Tuple[float, float], 可选) – 用于计算梯度以及梯度平方的运行平均值的系数（默认：0.9，0.999）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
'''

https://arxiv.org/abs/1412.6980
```
<a name="xow38"></a>
## 4、AdamW
Adam的进化版，是目前训练神经网络最快的方式。<br />优点：比Adam收敛得更快<br />缺点：只有fastai使用，缺乏广泛的框架，而且也具有很大的争议性。<br />算法：<br />![2022-07-01-08-44-31.781028100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640327126-cbe6834a-1033-4792-8632-43bb67ba81bf.png#clientId=u17442345-ccd6-4&from=ui&id=u46e1460e&originHeight=662&originWidth=656&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1305665&status=done&style=shadow&taskId=u6039c1ba-a81f-4436-88e0-4671841c7bd&title=)<br />函数：
```python
torch.optim.AdamW(params,lr=0.001,betas=(0.9,0.999),eps=1e-08,weight_decay=0.01,amsgrad=False)

'''
参数：
	betas (Tuple[float,float], 可选) – 用于计算梯度以及梯度平方的运行平均值的系数（默认：0.9，0.999）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
	amsgrad(boolean, optional) – 是否使用从论文On the Convergence of Adam and Beyond中提到的算法的AMSGrad变体（默认：False）
'''

https://arxiv.org/abs/1711.05101
```
<a name="Px6eM"></a>
## 5、Adagrad
独立地适应所有模型参数的学习率，梯度越大，学习率越小；梯度越小，学习率越大。<br />Adagrad适用于数据稀疏或者分布不平衡的数据集。<br />![2022-07-01-08-44-31.812261800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640326993-385dde86-4484-432c-86e6-e7e9fc761ad6.png#clientId=u17442345-ccd6-4&from=ui&id=DuUkb&originHeight=430&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=827439&status=done&style=shadow&taskId=ub472c9be-1e7b-466d-9dc7-927c6388130&title=)<br />函数：
```python
torch.optim.Adagrad(params, lr=0.01, lr_decay=0, weight_decay=0, initial_accumulator_value=0, eps=1e-10)

'''
参数：
	lr_decay (float, 可选) – 学习率衰减（默认: 0）
'''

http://jmlr.org/papers/v12/duchi11a.html
```
<a name="Bv5fB"></a>
## 6、Adadelta
是Adagard的改进版，对学习率进行自适应约束，但是进行了计算上的简化，加速效果不错，训练速度快。<br />![2022-07-01-08-44-31.858201700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640327000-caaecae3-39b0-41e4-ac65-f8622dc2a92f.png#clientId=u17442345-ccd6-4&from=ui&id=bmx2p&originHeight=425&originWidth=629&originalType=binary&ratio=1&rotation=0&showTitle=false&size=803768&status=done&style=shadow&taskId=udbcf84ba-509a-4417-8897-b85a6da1f5a&title=)<br />函数：
```python
torch.optim.Adadelta(params, lr=1.0, rho=0.9, eps=1e-06, weight_decay=0)

'''
参数：
	rho (float, 可选) – 用于计算平方梯度的运行平均值的系数（默认：0.9）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-6）
	lr (float, 可选) – 在delta被应用到参数更新之前对它缩放的系数（默认：1.0）
'''

http://www.cs.toronto.edu/~hinton/absps/momentum.pdf
```
<a name="Y3dUM"></a>
## 7、SparseAdam
针对稀疏张量的一种“阉割版”Adam优化方法。<br />函数：
```python
torch.optim.SparseAdam(params,lr=0.001,betas=(0.9,0.999),eps=1e-08)

'''
参数：
	betas (Tuple[float,float], 可选) – 用于计算梯度以及梯度平方的运行平均值的系数（默认：0.9，0.999）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
'''
```
<a name="oJfQv"></a>
## 8、Adamax
Adam的改进版，对Adam增加了一个学习率上限的概念，是Adam的一种基于无穷范数的变种。<br />![2022-07-01-08-44-31.957770000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640363606-276b1d87-e548-41a5-87df-d006be8a4cc0.png#clientId=u17442345-ccd6-4&from=ui&id=ueb3cbf0a&originHeight=429&originWidth=669&originalType=binary&ratio=1&rotation=0&showTitle=false&size=862906&status=done&style=shadow&taskId=u470d8249-6ab5-4450-9b34-a0083000416&title=)<br />函数：
```python
torch.optim.Adamax(params, lr=0.002, betas=(0.9, 0.999), eps=1e-08, weight_decay=0)

'''
参数：
	betas (Tuple[float,float], 可选) – 用于计算梯度以及梯度平方的运行平均值的系数
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
'''

https://arxiv.org/abs/1412.6980
```
<a name="SHoPb"></a>
## 9、NAdam
Adam的改进版，类似于带有Nesterov动量项的Adam，Nadam对学习率有了更强的约束，同时对梯度的更新也有更直接的影响。一般而言，在想使用带动量的RMSprop，或者Adam的地方，大多可以使用NAdam取得更好的效果。<br />算法：<br />![2022-07-01-08-44-32.010108000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640363770-4fe3561a-c5d8-4d6c-a9d2-46408e702085.png#clientId=u17442345-ccd6-4&from=ui&id=aLQA1&originHeight=655&originWidth=625&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1230849&status=done&style=shadow&taskId=ubdad2063-b257-4515-804f-a8f27b90071&title=)<br />函数：
```python
torch.optim.NAdam(params, lr=0.002, betas=(0.9, 0.999), eps=1e-08, weight_decay=0, momentum_decay=0.004)

'''
Args:	
	betas (Tuple[float, float], optional) :用于计算梯度以及梯度平方的运行平均值的系数
	eps (float, optional) :为了增加数值计算的稳定性而加到分母里的项
	momentum_decay (float, optional):动量衰减
'''

https://openreview.net/forum?id=OM0jvwB8jIp57ZJjtNEZ
```
<a name="b8Mzp"></a>
## 10、LBFGS
是一种在牛顿法基础上提出的一种求解函数根的算法，简单来说，L-BFGS和梯度下降、SGD干的同样的事情，但大多数情况下收敛速度更快<br />L-BFGS是对BFGS的改进，特点就是节省内存。<br />是解无约束非线性规划问题最常用的方法。<br />优点：收敛速度快、内存开销少，是解无约束非线性规划问题最常用的方法<br />缺点：使用条件严苛<br />函数：
```python
torch.optim.LBFGS(params, lr=1, max_iter=20, max_eval=None, tolerance_grad=1e-05, tolerance_change=1e-09, history_size=100,line_search_fn=None)

'''
参数：
	max_iter (int) – 每一步优化的最大迭代次数（默认：20）)
	max_eval (int) – 每一步优化的最大函数评价次数（默认：max * 1.25）
	tolerance_grad (float) – 一阶最优的终止容忍度（默认：1e-5）
	tolerance_change (float) – 在函数值/参数变化量上的终止容忍度（默认：1e-9）
	history_size (int) – 更新历史的大小（默认：100）
'''
```
<a name="ivPmh"></a>
## 11、RAdam
![2022-07-01-08-44-32.068616200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640363859-3552ab40-521a-446e-b7c4-37c9b6250d9c.png#clientId=u17442345-ccd6-4&from=ui&id=RhCJ1&originHeight=691&originWidth=668&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1387777&status=done&style=shadow&taskId=u0908a596-b56a-4bbe-9428-60882861e57&title=)<br />函数：
```python
torch.optim.RAdam(params, lr=0.001, betas=(0.9, 0.999), eps=1e-08, weight_decay=0)

'''
Args:
	betas (Tuple[float, float], optional) ：计算梯度平均和平方的系数
	eps (float, optional)：分母稳定项
'''

https://arxiv.org/abs/1908.03265
```
<a name="UxhVS"></a>
## 12、Rprop
弹性(resilient)反向传播：

- **首先为各权重变化赋一个初始值，设定权重变化加速因子与减速因子。**
- **在网络前馈迭代中当连续误差梯度符号不变时，采用加速策略，加快训练速度；当连续误差梯度符号变化时，采用减速策略，以期稳定收敛。**
- **网络结合当前误差梯度符号与变化步长实现BP，同时，为了避免网络学习发生振荡或下溢，算法要求设定权重变化的上下限。**

优化方法适用于full-batch，不适用于mini-batch，因此基本上没什么用。<br />算法：<br />![2022-07-01-08-44-32.092157300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640399557-28c56236-90fb-436d-aed3-a84bf995fe7d.png#clientId=u17442345-ccd6-4&from=ui&id=ufa0bb97d&originHeight=540&originWidth=645&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1047208&status=done&style=shadow&taskId=u8cf58099-4bff-406a-b46e-5e577545c01&title=)<br />函数：
```python
torch.optim.Rprop(params, lr=0.01, etas=(0.5, 1.2), step_sizes=(1e-06, 50))

'''
Args:
	etas (Tuple[float, float], optional)：pair of (etaminus, etaplis), 是乘法增加和减少的因子
	step_sizes (Tuple[float, float], optional) ：成对的最小和最大允许步长
'''

http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.21.1417
```
<a name="eALnR"></a>
## 13、RMSprop
均方根传递，RProp的改进版，也是Adagard的改进版。<br />思想：梯度震动较大的项，在下降时，减小其下降速度；对于震动幅度小的项，在下降时，加速其下降速度。<br />RMSprop采用均方根作为分母，可缓解Adagrad学习率下降较快的问题。<br />对于RNN有很好的效果。<br />RMSProp算法在经验上已经被证明是一种有效且实用的深度神经网络优化算法。目前它是深度学习从业者经常采用的优化方法之一。<br />算法：<br />![2022-07-01-08-44-32.174658800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640397073-f464511e-6bff-4b24-949e-e4c1a6fbeb74.png#clientId=u17442345-ccd6-4&from=ui&id=v2VPc&originHeight=620&originWidth=620&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1155759&status=done&style=shadow&taskId=uc72f83d1-9492-4d17-977f-ed42630c607&title=)<br />函数：
```python
torch.optim.RMSprop(params, lr=0.01, alpha=0.99, eps=1e-08, weight_decay=0, momentum=0, centered=False)

'''
参数：
	momentum (float, 可选) – 动量因子（默认：0）
	alpha (float, 可选) – 平滑常数（默认：0.99）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
	centered (bool, 可选) – 如果为True，计算中心化的RMSProp，并且用它的方差预测值对梯度进行归一化
'''

https://www.cs.toronto.edu/~tijmen/csc321/slides/lecture_slides_lec6.pdf
https://arxiv.org/pdf/1308.0850v5.pdf
```
<a name="oBEdS"></a>
## 14、SWA
随机权重平均 SWA， 是一个简单的程序，可以在不增加任何成本的情况下，通过随机梯度下降 (SGD) 提高深度学习的泛化能力，并且可以用作 PyTorch 中任何其他优化器的替代品。<br />算法：<br />![2022-07-01-08-44-32.272717700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640423888-b1716488-19d9-471e-8a6a-309206ffcce2.png#clientId=u17442345-ccd6-4&from=ui&id=udde84711&originHeight=471&originWidth=508&originalType=binary&ratio=1&rotation=0&showTitle=false&size=719513&status=done&style=shadow&taskId=uf6017b9e-3d69-4d42-8c73-91abb7eb33e&title=)<br />函数：
```python
torchcontrib.optim.SWA(optimizer,swa_start=None, swa_freq=None, swa_lr=None)

'''
Args:
    optimizer (torch.optim.Optimizer): optimizer to use with SWA
    swa_start (int): number of steps before starting to apply SWA in
        automatic mode; if None, manual mode is selected (default: None)
    swa_freq (int): number of steps between subsequent updates of
        SWA running averages in automatic mode; if None, manual mode is
        selected (default: None)
    swa_lr (float): learning rate to use starting from step swa_start
        in automatic mode; if None, learning rate is not changed
        (default: None)
'''

https://arxiv.org/abs/1803.05407
https://pytorch.org/blog/stochastic-weight-averaging-in-pytorch/
```
<a name="uWV5C"></a>
## 13、AccSGD
加速随机梯度下降(ASGD)，是一种简单的实现随机算法，它基于Nesterov加速算法的一个相对不那么流行的变体。<br />ASGD比HB、NAG和SGD具有性能优势。<br />算法：<br />![2022-07-01-08-44-32.353644700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640424106-97cd62f2-34f4-448e-a070-3f5f7be11934.png#clientId=u17442345-ccd6-4&from=ui&id=b86aS&originHeight=390&originWidth=997&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1168848&status=done&style=shadow&taskId=uc61b9c98-2b63-4ba8-9f21-091ad22b141&title=)<br />函数：
```python
class torch_optimizer.AccSGD(params, lr=0.001, kappa=1000.0, xi=10.0, small_const=0.7, weight_decay=0)

'''
Args:
	kappa (float) – ratio of long to short step (default: 1000)	
	xi (float) – statistical advantage parameter (default: 10)	
	small_const (float) – any value <=1 (default: 0.7)	
	weight_decay (float) – weight decay (L2 penalty) (default: 0)
'''

https://arxiv.org/abs/1704.08227
https://arxiv.org/abs/1803.05591
https://github.com/rahulkidambi/AccSGD
```
<a name="AwjHY"></a>
## 14、AdaBound
提供了ADAM和AMSGRAD的新变体，分别称为ADABOUND和AMSBOUND，它们利用学习率的动态边界实现从自适应方法到SGD的逐步和平稳过渡。<br />新方法能够消除自适应方法与SGD方法之间的泛化差距，同时在训练早期保持较高的学习速度。此外，它们可以显著改进原型，尤其是在复杂的深度网络上。<br />算法：<br />![2022-07-01-08-44-32.460739200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640423969-ff96df00-cadf-4491-9df5-08ac2f44f553.png#clientId=u17442345-ccd6-4&from=ui&id=z3Sqa&originHeight=298&originWidth=848&originalType=binary&ratio=1&rotation=0&showTitle=false&size=759713&status=done&style=shadow&taskId=u65e1d951-6018-4423-8e68-3b3187f1fdc&title=)<br />函数：
```python
class torch_optimizer.AdaBound(params, lr=0.001, betas=0.9, 0.999, final_lr=0.1, gamma=0.001, eps=1e-08, weight_decay=0, amsbound=False)

'''
Args:
	betas (Tuple[float, float]) – coefficients used for computing running averages of gradient and its square (default: (0.9, 0.999))
	final_lr (float) – final (SGD) learning rate (default: 0.1)
	gamma (float) – convergence speed of the bound functions (default: 1e-3)
	eps (float) – term added to the denominator to improve numerical stability (default: 1e-8)
	amsbound (bool) – whether to use the AMSBound variant of this algorithm
'''

https://arxiv.org/abs/1902.09843
https://github.com/Luolc/AdaBound
```
<a name="WWIJc"></a>
## 15、AdaMod
Adam会产生非常大的学习率，抑制了学习的开始。<br />自适应和矩界(Adaptive and Momental Bound， AdaMod)方法，以自适应和矩界上界限制自适应学习率。动态学习率边界基于自适应学习率本身的指数移动平均，平滑了意外大学习率，稳定了深度神经网络的训练。<br />与Adam相比，AdaMod消除了整个训练过程中极大的学习率，并带来了显著的改进，尤其是在DenseNet和Transformer等复杂网络上。<br />算法：<br />![2022-07-01-08-44-32.552328200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640447987-dd14187b-23a1-43a2-9433-6e62589dcaab.png#clientId=u17442345-ccd6-4&from=ui&id=u196c168f&originHeight=495&originWidth=644&originalType=binary&ratio=1&rotation=0&showTitle=false&size=958461&status=done&style=shadow&taskId=u6ca15903-3650-448a-97b3-49de9fead5a&title=)<br />函数：
```python
class torch_optimizer.AdaMod(params, lr=0.001, betas=0.9, 0.999, beta3=0.999, eps=1e-08, weight_decay=0)

'''
Args:
	betas (Tuple[float, float]) – coefficients used for computing running averages of gradient and its square (default: (0.9, 0.999))
	beta3 (float) – smoothing coefficient for adaptive learning rates (default: 0.9999)
	eps (float) – term added to the denominator to improve numerical stability (default: 1e-8)
'''

https://arxiv.org/abs/1910.12249
https://github.com/lancopku/AdaMod
```
<a name="IlP5P"></a>
## 16、Adafactor
在RMSProp, Adam, Adadelta中，参数更新是通过过去梯度平方的指数移动平均的平方根进行缩放的。维护这些性能参数秒矩估计器需要与参数数量相等的内存。<br />对于神经网络权重矩阵的情况，建议只保持这些移动平均线的行和列和，并根据这些和估计每个参数的秒矩。自适应方法产生大于期望的更新时，第二矩蓄能器的衰减速度太慢。<br />Adafactor，提出更新裁剪和逐渐增加衰减率方案作为补救措施。结合这些方法和下降动量，在优化器中使用非常少的辅助存储，取得了与发布的Adam制度相当的结果。<br />Adafactor（Adaptive Learning Rates with Sublinear Memory Cost）是基于参数本身规模的参数更新方法。<br />算法：<br />![2022-07-01-08-44-32.610810000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640447950-72883bd5-5fbe-441d-9b04-8cea1b1ec7d1.png#clientId=u17442345-ccd6-4&from=ui&id=ak2xq&originHeight=464&originWidth=651&originalType=binary&ratio=1&rotation=0&showTitle=false&size=908200&status=done&style=shadow&taskId=ubc816c34-e1e3-4b93-a49f-1fe0c894eb0&title=)<br />函数：
```python
class torch_optimizer.Adafactor(params, lr=None, eps2=1e-30, 0.001, clip_threshold=1.0, decay_rate=- 0.8, beta1=None, weight_decay=0.0, scale_parameter=True, relative_step=True, warmup_init=False)

'''
Args:
	eps2 (Tuple[float, float]) – regularization constans for square gradient and parameter scale respectively (default: (1e-30, 1e-3))
	clip_threshold (float) – threshold of root mean square of final gradient update (default: 1.0)
	decay_rate (float) – coefficient used to compute running averages of square gradient (default: -0.8)
	beta1 (Optional[float]) – coefficient used for computing running averages of gradient (default: None)
	scale_parameter (bool) – if true, learning rate is scaled by root mean square of parameter (default: True)
	relative_step (bool) – if true, time-dependent learning rate is computed instead of external learning rate (default: True)
	warmup_init (bool) – time-dependent learning rate computation depends on whether warm-up initialization is being used (default: False)
'''

https://arxiv.org/abs/1804.04235
https://github.com/pytorch/fairseq/blob/master/fairseq/optim/adafactor.py
```
<a name="vl4ja"></a>
## 17、AdamP
归一化技术，是现代深度学习的福音。它们使权值收敛得更快，泛化性能往往更好。有人认为，归一化诱导权重之间的尺度不变性为梯度下降(GD)优化器提供了有利的基础：随着时间的推移，有效的步长自动减少，稳定了整个训练过程。然而，在GD优化器中额外引入动量会导致尺度不变权值的有效步长更快地减少，这一现象尚未被研究，在当前实践中可能会造成不必要的副作用。<br />为解决上述问题，AdamP在优化器的每个步骤中，去掉径向分量，或标准递增方向。由于尺度不变性，该方法只改变了有效步长，而不改变有效更新方向，从而保持了GD优化器原有的收敛特性。<br />算法：<br />![2022-07-01-08-44-32.768099900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640470161-bd7e237e-0c4b-413c-8487-018a2ded1e59.png#clientId=u17442345-ccd6-4&from=ui&id=ub32d762f&originHeight=329&originWidth=538&originalType=binary&ratio=1&rotation=0&showTitle=false&size=532267&status=done&style=shadow&taskId=u513d977c-84f3-4f5c-b41d-c385b2b658e&title=)<br />函数：
```python
class torch_optimizer.AdamP(params, lr=0.001, betas=0.9, 0.999, eps=1e-08, weight_decay=0, delta=0.1, wd_ratio=0.1, nesterov=False)

'''
Args:
	betas (Tuple[float, float])：计算梯度均值和平方的系数
	eps (float) ：添加到分母以提高数值稳定性的项
	delta (float)：确定一组参数是否为尺度不变的阈值
	wd_ratio (float)：与应用于尺度可变参数相比，应用于尺度不变参数的相对权重衰减
	nesterov (bool)：enables Nesterov momentum (default: False)
'''

https://arxiv.org/abs/2006.08217
https://github.com/clovaai/AdamP
```
<a name="SOp13"></a>
## 18、AggMo
动量是一个简单而广泛使用的技巧，它允许基于梯度的优化器沿着低曲率方向加快速度。它的性能主要取决于阻尼系数β。较大的β值可能带来更大的加速，但容易出现振荡和不稳定；因此，人们通常采用较小的值，如0.5或0.9。<br />聚合动量(AggMo，AGGREGATED MOMENTUM)，是一种结合了多个速度矢量和不同β参数的动量变体。AggMo实现起来很简单，但能显著抑制振荡，使其即使在激进的β值(如0.999)下也能保持稳定。AggMo是其他动量方法的合适替代品，通常可以在几乎不需要调优的情况下更快地收敛。<br />函数：
```python
class torch_optimizer.AggMo(params, lr=0.001, betas=0.0, 0.9, 0.99, weight_decay=0)

https://arxiv.org/abs/1804.00325
https://github.com/AtheMathmo/AggMo/blob/master/aggmo.py
```
<a name="VMG9Z"></a>
## 19、DiffGrad
随机梯度(SGD)是深度神经网络成功的核心技术之一。梯度提供了函数变化速度最快的方向的信息。基本SGD的主要问题是对所有参数按相同大小的步骤改变，而不考虑梯度行为。因此，对每个参数进行自适应步长是一种有效的深度网络优化方法。<br />近年来，人们对AdaGrad、AdaDelta、RMSProp和Adam等梯度下降方法进行了改进。这些方法依赖于过去梯度的平方的指数移动平均的平方根。因此，这些方法不利用梯度的局部变化。<br />diffGrad（the difference betweenthe present and the immediate past gradient）基于现在和直接过去梯度之间的差异，对每个参数的步长进行调整，使梯度变化快的参数步长更大，梯度变化小的参数步长更小。实验证明，该算法优于其他优化算法。同时，diffGrad对于使用不同的激活函数训练CNN具有一致良好的性能。<br />函数：
```python
class torch_optimizer.DiffGrad(params, lr=0.001, betas=0.9, 0.999, eps=1e-08, weight_decay=0.0)

'''
Args:
	betas (Tuple[float, float])：计算梯度均值和平方的系数
	eps (float)：添加到分母以提高数值稳定性的项
'''

https://arxiv.org/abs/1909.11015
https://github.com/shivram1987/diffGrad
```
<a name="LJlsG"></a>
## 20、Lamb
layerwise adaptive large batch optimization是一种有原则的分层自适应策略，以加速使用大的小批量深度神经网络的训练。<br />LAMB在各种任务(如BERT和RESNET-50训练)上的卓越性能，且只需很少的超参数调优。<br />特别是，对于BERT训练，优化器允许使用非常大的32868批处理，而不会降低性能。<br />算法：<br />![2022-07-01-08-44-32.887934800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640469875-b1bfb81c-6c2c-4b75-a018-701090f894a0.png#clientId=u17442345-ccd6-4&from=ui&id=u4Inq&originHeight=447&originWidth=555&originalType=binary&ratio=1&rotation=0&showTitle=false&size=745981&status=done&style=shadow&taskId=ube62d124-2f66-44fc-b336-0d56a021be8&title=)<br />函数：
```python
class torch_optimizer.Lamb(params, lr=0.001, betas=0.9, 0.999, eps=1e-06, weight_decay=0, clamp_value=10, adam=False, debias=False)

'''
Args:
	betas (Tuple[float, float])：计算梯度均值和平方的系数
	eps (float) –：添加到分母以提高数值稳定性的项
	clamp_value (float)：clamp weight_norm in (0,clamp_value) (default: 10) set to a high value to avoid it (e.g 10e3)
	adam (bool) ：always use trust ratio = 1, which turns this into Adam. Useful for comparison purposes. (default: False)
	debias (bool)：debias adam by (1 - beta**step) (default: False)
'''

https://arxiv.org/abs/1904.00962
https://github.com/cybertronai/pytorch-lamb
```
<a name="GbsQV"></a>
## 21、NovoGrad
NovoGrad是一种自适应随机梯度下降方法，该方法具有逐层梯度归一化和解耦权值衰减。在关于图像分类、语音识别、机器翻译和语言建模的神经网络实验中，它的表现与经过良好调整的SGD(带有动量、Adam和AdamW)相当或更好。<br />此外，NovoGrad：

- **对于学习率和权值初始化的选择是稳健的**
- **在大批量设置中工作良好**
- **内存占用只有Adam的一半**

算法：<br />![2022-07-01-08-44-32.950317300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640469979-35de6a9f-888e-47a4-8c82-ade3681d079b.png#clientId=u17442345-ccd6-4&from=ui&id=ta54V&originHeight=597&originWidth=527&originalType=binary&ratio=1&rotation=0&showTitle=false&size=946063&status=done&style=shadow&taskId=u7439d74d-3733-4643-98be-9777ee20167&title=)<br />函数：
```python
class torch_optimizer.NovoGrad(params, lr=0.001, betas=0.95, 0, eps=1e-08, weight_decay=0, grad_averaging=False, amsgrad=False)

'''
Args:
	betas (Tuple[float, float])：计算梯度均值和平方的系数
	eps (float)：添加到分母以提高数值稳定性的项
	grad_averaging (bool) ：gradient averaging (default: False)	
	amsgrad (bool) ：是否使用论文 On the Convergence of Adam and Beyond 中的该算法的 AMSGrad 变体
'''

https://arxiv.org/abs/1905.11286
https://github.com/NVIDIA/DeepLearningExamples
```
<a name="hIr2w"></a>
## 22、PID
深度神经网络已经在许多计算机视觉应用中展示了其强大的能力。最先进的深度架构，如VGG、ResNet和DenseNet，大多是由SGD-Momentum算法优化的，该算法通过考虑它们过去和现在的梯度来更新权重。但是，SGD-Momentum存在超调问题，阻碍了网络训练的收敛。<br />受比例-积分-微分(PID，proportional-integral-derivative)控制器在自动控制中取得的显著成功的启发，提出了一种加速深度网络优化的PID方法。首先揭示了SGD-Momentum与基于PID的控制器之间的内在联系，然后提出了利用过去、现在和梯度变化来更新网络参数的优化算法。<br />在CIFAR10、CIFAR100和Tiny-ImageNet等基准数据集上的实验验证了所提出的PID方法大大减少了SGD-Momentum的超调现象，并在流行的深度网络架构上实现了高达50%的加速，且精度具有竞争优势。<br />函数：
```python
class torch_optimizer.PID(params, lr=0.001, momentum=0.0, dampening=0, weight_decay=0.0, integral=5.0, derivative=10.0)

'''
Args:
	momentum (float)：动量因子
	dampening (float) ：动量抑制因子
	derivative (float)：D part of the PID (default: 10.0)	
	integral (float)：I part of the PID (default: 5.0)
'''

http://www4.comp.polyu.edu.hk/~cslzhang/paper/CVPR18_PID.pdf
https://github.com/tensorboy/PIDOptimizer
```
<a name="clk2n"></a>
## 23、QHAdam
基于动量的随机梯度下降法(SGD)在深度学习中得到了广泛的应用。<br />提出了准双曲动量算法(QHM,quasi-hyperbolic momentum algorithm)作为动量SGD的一个极其简单的改变，平均一个普通的SGD步与动量步长。<br />并提出了Adam的QH变体，称为QHAdam，经验证明，算法在各种设置下显著改善了训练。<br />算法：<br />![2022-07-01-08-44-32.969580100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640538949-0ebf9905-9e6f-4ed9-a53c-c5957ba91b82.png#clientId=u17442345-ccd6-4&from=ui&id=u71f124de&originHeight=269&originWidth=795&originalType=binary&ratio=1&rotation=0&showTitle=false&size=642954&status=done&style=shadow&taskId=u9160bf20-de5d-4808-8140-c91d5e643df&title=)<br />函数：
```python
class torch_optimizer.QHAdam(params, lr=0.001, betas=0.9, 0.999, nus=1.0, 1.0, weight_decay=0.0, decouple_weight_decay=False, eps=1e-08)

'''
Args:
	betas (Tuple[float, float]) ：计算梯度均值和平方的系数		
	nus (Tuple[float, float])：用于估计梯度及其平方的直接折现因子
	eps (float)：添加到分母以提高数值稳定性的项
	decouple_weight_decay (bool) ：是否将权重衰减与基于梯度的优化步骤解耦
'''

https://arxiv.org/abs/1810.06801
https://github.com/facebookresearch/qhoptim
```
<a name="CWfgi"></a>
## 24、QHM
准双曲动量算法优化算法。<br />算法：<br />![2022-07-01-08-44-33.029388600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640538810-d286223b-af26-4709-8e14-ddf3880324c1.png#clientId=u17442345-ccd6-4&from=ui&id=XcZ5r&originHeight=183&originWidth=796&originalType=binary&ratio=1&rotation=0&showTitle=false&size=437977&status=done&style=shadow&taskId=u5c557173-d3f1-4a76-8f5b-d128ffde056&title=)<br />函数：
```python
class torch_optimizer.QHM(params, lr=0.001, momentum=0.0, nu=0.7, weight_decay=0.0, weight_decay_type='grad')

'''
Args:
	momentum：动量因子 (β from the paper)	
	nu：直接折扣系数 (ν from the paper)	
	weight_decay：L2 regularization coefficient, times two) (default: 0.0)	
	weight_decay_type：应用权重衰减的方法：“grad”用于梯度累积（与 torch.optim.SGD 相同）或“direct”用于直接应用于参数（默认值：“grad”）
'''

https://arxiv.org/abs/1810.06801
https://github.com/facebookresearch/qhoptim
```
<a name="vyVFE"></a>
## 25、RAdam
学习率预热启发式算法对于RMSprop和Adam等自适应随机优化算法在稳定训练、加速收敛和提高泛化能力方面取得了显著的成功。<br />为了研究热身背后的理论，发现了一个自适应学习率的问题--它的方差在早期是有问题的，并假定热身作为一种方差减少技术。提出了修正Adam(RAdam)，它是Adam的一个新变体，通过引入一个术语来修正自适应学习率的方差。<br />算法：<br />![2022-07-01-08-44-33.069904600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640564253-1b75ff29-b153-43c7-99b1-517af5a8d799.png#clientId=u17442345-ccd6-4&from=ui&id=u2ccb12d6&originHeight=524&originWidth=916&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1442880&status=done&style=shadow&taskId=u91874613-b4b1-4746-99a7-95a3c0e1dae&title=)<br />函数：
```python
class torch_optimizer.RAdam(params, lr=0.001, betas=0.9, 0.999, eps=1e-08, weight_decay=0)

'''
Args:
	betas (Tuple[float, float])：计算梯度均值和平方的系数		
	eps (float) ：添加到分母以提高数值稳定性的项
'''

https://arxiv.org/abs/1908.03265
https://github.com/LiyuanLucasLiu/RAdam
```
<a name="lg3pV"></a>
## 26、SGDP
归一化技术，是现代深度学习的福音。它们使权值收敛得更快，泛化性能往往更好。有人认为，归一化诱导权重之间的尺度不变性为梯度下降(GD)优化器提供了有利的基础：随着时间的推移，有效的步长自动减少，稳定了整个训练过程。然而，在GD优化器中额外引入动量会导致尺度不变权值的有效步长更快地减少，这一现象尚未被研究，在当前实践中可能会造成不必要的副作用。<br />为解决上述问题，SGDP在优化器的每个步骤中，去掉径向分量，或标准递增方向。由于尺度不变性，该方法只改变了有效步长，而不改变有效更新方向，从而保持了GD优化器原有的收敛特性。<br />算法：<br />![2022-07-01-08-44-33.105189300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640562617-d40355ba-6d8b-4905-baea-d7164c5090ba.png#clientId=u17442345-ccd6-4&from=ui&id=Wg0g8&originHeight=276&originWidth=529&originalType=binary&ratio=1&rotation=0&showTitle=false&size=439078&status=done&style=shadow&taskId=u6ae3bd62-2f59-425d-b70f-9024ad8c779&title=)<br />函数：
```python
class torch_optimizer.SGDP(params, lr=0.001, momentum=0, dampening=0, eps=1e-08, weight_decay=0, delta=0.1, wd_ratio=0.1, nesterov=False)

'''
Args:
	momentum (float)：动量因子
	dampening (float) ：动量抑制因子
	eps (float)：添加到分母以提高数值稳定性的项
	delta (float)：确定一组参数是否为尺度不变的阈值
	wd_ratio (float)：与应用于尺度变量参数相比，应用于尺度不变参数的相对权重衰减
	nesterov (bool) – enables Nesterov momentum (default: False)
'''

https://arxiv.org/abs/2006.08217
https://github.com/clovaai/AdamP
```
<a name="UoN8f"></a>
## 27、SGDW
L2正则化和权值衰减正则化对于标准随机梯度下降是等价的(当按学习率缩放时)，但正如所证明的，这不是自适应梯度算法的情况，如Adam。<br />虽然这些算法的常见实现采用L2正则化(通常称为“权衰减”，因为暴露的不相等可能会造成误导)，为此提出了一个简单的修改，通过将权衰减从所采取的优化步骤解耦到损失函数，以恢复权衰减正则化的原始公式。<br />经验证据表明，提出的修改：

- **从标准SGD和Adam的学习率设置中解耦了权重衰减因子的最优选择**
- **大大提高了Adam的泛化性能，使其能够在图像分类数据集上与SGD竞争(以前后者在这方面的表现通常优于后者)。**

算法：<br />![2022-07-01-08-44-33.158247100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640563837-29cc660f-364c-427d-8fb2-3c986b24f57b.png#clientId=u17442345-ccd6-4&from=ui&id=JPGBm&originHeight=382&originWidth=877&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1007122&status=done&style=shadow&taskId=u29b4d800-e1da-458c-843e-ee31329c2f8&title=)<br />函数：
```python
class torch_optimizer.SGDW(params, lr=0.001, momentum=0.0, dampening=0.0, weight_decay=0.0, nesterov=False)

'''
Args:
	momentum (float)：动量因子
	dampening (float)：动量抑制因子 (default: 0)	
	nesterov (bool)：enables Nesterov momentum (default: False)
'''

https://arxiv.org/abs/1711.05101
https://github.com/loshchil/AdamW-and-SGDW
```
<a name="NuPqM"></a>
## 28、Shampoo
预条件梯度法是最通用和最强大的优化工具之一。然而，预处理需要存储和操作非常大的矩阵。为此提出了一个新的结构感知预处理算法，称为Shampoo，张量空间上的随机优化。<br />Shampoo维护一组预处理矩阵，每个矩阵在一个维度上操作，在其余维度上收缩。 用最先进的深度学习模型进行的实验表明，Shampoo能够比常用的优化器更快地收敛。虽然它涉及到更复杂的更新规则，但Shampoo每一步的运行时间与SGD、AdaGrad和Adam等简单的梯度方法相当。<br />算法：<br />![2022-07-01-08-44-33.191770700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640587745-3a43e296-1c9d-4d6e-8a7b-08ad0618c945.png#clientId=u17442345-ccd6-4&from=ui&id=u7dbdd66f&originHeight=313&originWidth=542&originalType=binary&ratio=1&rotation=0&showTitle=false&size=510154&status=done&style=shadow&taskId=u6d9359c1-c2af-4ca2-a8a4-72d32ff5716&title=)<br />函数：
```python
class torch_optimizer.Shampoo(params, lr=0.1, momentum=0.0, weight_decay=0.0, epsilon=0.0001, update_freq=1)

'''
Args:
	momentum (float)：动量因子
	epsilon (float)：epsilon 添加到每个 mat_gbar_j 以实现数值稳定性
	update_freq (int)：计算逆的更新频率
'''

https://arxiv.org/abs/1802.09568
https://github.com/moskomule/shampoo.pytorch
```
<a name="ts4CX"></a>
## 29、SWATS
尽管如Adam, Adagrad或RMSprop有更好的训练结果，但与随机梯度下降(SGD)相比，推广效果很差。这些方法往往在训练的最初阶段表现良好，但在训练的后期阶段，SGD的表现优于这些方法。研究了一种混合策略，它以一种自适应方法开始训练，并在适当的时候切换到SGD。<br />具体地，提出了SWATS，这是一种简单的策略，当触发条件满足时，从Adam切换到SGD。提出的条件与Adam阶跃在梯度子空间上的投影有关。根据设计，这种情况的监视过程只会增加很少的开销，而且不会增加优化器中的超参数数量。实验结果表明，该策略能够缩小SGD和Adam在大多数任务上的泛化差距。<br />算法：<br />![2022-07-01-08-44-33.322095100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640588767-a6ef8457-feaa-4039-9bf2-3ccca832037f.png#clientId=u17442345-ccd6-4&from=ui&id=Dy1vQ&originHeight=811&originWidth=553&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1348517&status=done&style=shadow&taskId=u982af77b-f179-41f5-9712-5913f087b1a&title=)<br />函数：
```python
class torch_optimizer.SWATS(params, lr=0.001, betas=0.9, 0.999, eps=0.001, weight_decay=0, amsgrad=False, nesterov=False)

'''
Args:
	betas：计算梯度均值和平方的系数	
	eps (float)：添加到分母以提高数值稳定性的项	
	amsgrad (bool)：是否使用论文 On the Convergence of Adam and Beyond 中该算法的 AMSGrad 变体	
	nesterov (bool)：是否使用 Nesterov momentum (default: False)
'''

https://arxiv.org/pdf/1712.07628.pdf
https://github.com/Mrpatekful/swats
```
<a name="GiyCm"></a>
## 30、Yogi
自适应梯度方法依赖于过去平方梯度的指数移动平均的平方根缩放梯度，如RMSProp, Adam, Adadelta在优化深度学习中出现的非凸问题中发现了广泛的应用。但这种方法即使在简单的凸优化设置中也不能收敛。<br />因此提供了一种新的分析方法，用于非凸随机优化问题，表征了增加小批量大小的影响。分析表明，在这种情况下，这些方法收敛到平稳性的统计极限的方差在随机梯度(由一个常数因子缩放)。<br />特别地，结果表明，增加小批处理的大小使收敛成为可能，从而提供了一种避免不收敛问题的方法。此外，还提出了一种新的自适应优化算法Yogi，该算法控制了有效学习率的提高，在收敛性方面具有相似的理论保证，从而获得了更好的性能。<br />大量的实验表明，在一些具有挑战性的机器学习任务中，使用很少超参数调整的Yogi方法优于Adam等方法。<br />算法：<br />![2022-07-01-08-44-33.354609500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656640587658-8e97c4d1-6c65-421a-8014-8a0335350acf.png#clientId=u17442345-ccd6-4&from=ui&id=x7qvu&originHeight=302&originWidth=492&originalType=binary&ratio=1&rotation=0&showTitle=false&size=446856&status=done&style=shadow&taskId=ua855c707-3e48-41d8-abf4-5c999586e93&title=)<br />函数：
```python
class torch_optimizer.Yogi(params, lr=0.01, betas=0.9, 0.999, eps=0.001, initial_accumulator=1e-06, weight_decay=0)[source]

'''
Args:
	betas：计算梯度均值和平方的系数
	eps (float) ：添加到分母以提高数值稳定性的项
	initial_accumulator (float)：第一和第二时刻的初始值
'''

https://papers.nips.cc/paper/8186-adaptive-methods-for-nonconvex-optimization
https://github.com/4rtemi5/Yogi-Optimizer_Keras
```
