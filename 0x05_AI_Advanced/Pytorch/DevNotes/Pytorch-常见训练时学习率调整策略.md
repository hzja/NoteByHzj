Pytorch
<a name="MTyZn"></a>
## Pytorch中常见学习率调整函数
学习率作为超参数，训练过程中其实是可以动态调整的，这个很多深度学习框架都予以支持，Pytorch框架在这个方面接口化做的非常符合开发者愿意。Pytorch的可调节学习率的方式主要是借助于Pytorch优化器包中几个学习率动态调节函数，相关的学习率调整。`torch.optim.lr_scheduler`提供了几种基于`epoch`的动态学习率调整函数。它们的解释与使用方法分别如下：
<a name="NxeYL"></a>
### 1、函数一
该函数表示在发现`loss`不再降低或者`acc`不再提高之后，降低学习率。
```python
torch.optim.lr_scheduler.ReduceLROnPlateau(
    optimizer,
    mode='min',
    factor=0.1,
    patience=10,
    threshold=0.0001,
    threshold_mode='rel',
    cooldown=0,
    min_lr=0,
    eps=1e-08,
    verbose=False
)
```
<a name="SvN66"></a>
### 2、函数二
设置学习率为上一次的学习率乘以给定lr_lambda函数的值
```python
torch.optim.lr_scheduler.MultiplicativeLR(
    optimizer, // 优化器
    lr_lambda, // 函数
    last_epoch=-1, // 学习率最后一个epoch，计数
    verbose=False
)
```
<a name="CdX0f"></a>
### 3、函数三
周期性的学习率调整函数，指定多个`epoch`调整一次学习率，学习率每次调整由`gamma`参数决定，`step_size`表示`epoch`的周期大小
```python
torch.optim.lr_scheduler.StepLR(optimizer,step_size,gamma=0.1,last_epoch=-1,verbose=False)
```
代码演示如下：
```python
# Assuming optimizer uses lr = 0.05 for all groups
# lr = 0.05     if epoch < 30
# lr = 0.005    if 30 <= epoch < 60
# lr = 0.0005   if 60 <= epoch < 90
# ...
scheduler = StepLR(optimizer, step_size=30, gamma=0.1)
for epoch in range(100):
         train(...)
         validate(...)
         scheduler.step()
```
主要：必须最后完成学习率更新！
<a name="K3Fcb"></a>
### 4、函数四
该函数跟`StepLR`的区别是为`StepLR`是周期性改变学习率，而该函数可以用户自定义`epoch`的里程碑，根据自定义的里程碑来调整学习率。
```python
torch.optim.lr_scheduler.MultiStepLR(
    optimizer, // 优化器
    milestones, // 表示定义epoch里程碑
    gamma=0.1,
    last_epoch=-1,
    verbose=False
)
```
代码演示如下：
```python
# Assuming optimizer uses lr = 0.05 for all groups
# lr = 0.05     if epoch < 30
# lr = 0.005    if 30 <= epoch < 80
# lr = 0.0005   if epoch >= 80
scheduler = MultiStepLR(optimizer, milestones=[30,80], gamma=0.1)
for epoch in range(100):
         train(...)
         validate(...)
         scheduler.step()
```
<a name="vM0XV"></a>
### 5、函数五
使用指数衰减模型来调整学习率
```python
torch.optim.lr_scheduler.ExponentialLR(optimizer,gamma,last_epoch=-1,verbose=False)
```
<a name="hEHXk"></a>
### 6、函数六
基于余弦退火函数实现学习率调整的
```python
torch.optim.lr_scheduler.CosineAnnealingLR(
    optimizer,
    T_max,
    eta_min=0,
    last_epoch=-1,
    verbose=False
)
```
每个epoch的学习率表达式如下：
<a name="vbsGf"></a>
### 7、函数七
上述的函数都是每个`epoch`之后调整学习率，该函数支持每个`step`/`batch`调整学习率。函数与参数表示如下：
```python
torch.optim.lr_scheduler.CyclicLR(
    optimizer,
    base_lr,
    max_lr,
    step_size_up=2000,
    step_size_down=None,
    mode='triangular',
    gamma=1.0,
    scale_fn=None,
    scale_mode='cycle',
    cycle_momentum=True,
    base_momentum=0.8,
    max_momentum=0.9,
    last_epoch=-1,
    verbose=False
)
```
代码演示如下：
```python
optimizer = torch.optim.SGD(model.parameters(), lr=0.1, momentum=0.9)
scheduler = torch.optim.lr_scheduler.CyclicLR(optimizer, base_lr=0.01, max_lr=0.1)
data_loader = torch.utils.data.DataLoader(...)
for epoch in range(10):
    for batch in data_loader:
        train_batch(...)
        scheduler.step()
```

