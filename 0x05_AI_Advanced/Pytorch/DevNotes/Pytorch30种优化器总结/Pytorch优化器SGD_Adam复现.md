PyTorch
<a name="wbwrI"></a>
## 1、SGD
随机梯度下降算法，每次参数更新时，仅仅选取一个样本计算其梯度。<br />算法：<br />![2022-07-01-09-58-41.888532000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656641973946-b84001ec-9d9d-41b0-b4d4-3abb82b5d91f.png#clientId=u1278244e-6872-4&from=ui&id=u9ad21fb7&originHeight=642&originWidth=670&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1293225&status=done&style=shadow&taskId=ubb52a6a1-d5e8-4dfc-b975-d8257a485e1&title=)<br />函数：
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
自定义优化器基类：
```python
import warnings
import functools
from collections import defaultdict


class CustomOptimizer:

    def __init__(self, params, defaults):

        self.defaults = defaults

        """
        defaultdict的作用在于当字典里的key被查找但不存在时，
            返回的不是keyError而是一个默认值，
            此处defaultdict(dict)返回的默认值会是个空字典。
        """
        self.state = defaultdict(dict)

        self.param_groups = []
        param_groups = list(params)

        if len(param_groups) == 0:
            raise ValueError('optimizer got an empty parameter list')

        ifnot
        isinstance(param_groups[0], dict):
        param_groups = [{'params': param_groups}]

    for param_group in param_groups:
        self.add_param_group(param_group)


    def add_param_group(self, param_group):
        """
        作用是将param_group放进self.param_groups中
        param_group是字典，Key是params，Value是param_groups=list(params)
        """
    
        assert isinstance(param_group, dict), "param group must be a dict"
    
        params = param_group['params']
    
        if isinstance(params, torch.Tensor):
            param_group['params'] = [params]
        else:
            param_group['params'] = list(params)
    
        """将self.defaults中的键值对遍历放到字典param_group中"""
        for name, default in self.defaults.items():
            param_group.setdefault(name, default)
    
        params = param_group['params']
    
        if len(params) != len(set(params)):
            warnings.warn("optimizer contains a parameter group with duplicate parameters; "
                          "in future, this will cause an error; ", stacklevel=3)
    
        """对self.param_groups和param_group中的元素进行判断，确保没有重复的参数"""
        param_set = set()
        for group in self.param_groups:
            param_set.update(set(group['params']))
    
        if not param_set.isdisjoint(set(param_group['params'])):
            raise ValueError("some parameters appear in more than one parameter group")
    

        """将字典param_group放进列表self.param_groups"""
        self.param_groups.append(param_group)


def __setstate__(self, state):
    self.__dict__.update(state)


def __getstate__(self):
    return {
        'defaults': self.defaults,
        'state': self.state,
        'param_groups': self.param_groups,
    }


def step(self, closure):
    raise NotImplementedError


def zero_grad(self):
    r"""将梯度置为零"""
    for group in self.param_groups:
        for p in group['params']:
            if p.grad isnotNone:
                p.grad.detach_()
                p.grad.zero_()


def __repr__(self):
    format_string = self.__class__.__name__ + ' ('
    for i, group in enumerate(self.param_groups):
        format_string += '\n'
        format_string += 'Parameter Group {0}\n'.format(i)
        for key in sorted(group.keys()):
            if key != 'params':
                format_string += '    {0}: {1}\n'.format(key, group[key])
    format_string += ')'
    return format_string
```
自定义SGD优化器：
```python
class CustomSGD(CustomOptimizer):
    def __init__(self, params, lr, momentum=0, dampening=0, weight_decay=0, nesterov=False, maximize=False):

        """参数被打包进字典中命名为defaults"""
        defaults = dict(lr=lr, momentum=momentum, dampening=dampening,
                        weight_decay=weight_decay, nesterov=nesterov, maximize=maximize)
        super(CustomSGD, self).__init__(params, defaults)

    def __setstate__(self, state):
        super(CustomSGD, self).__setstate__(state)
        for group in self.param_groups:
            group.setdefault('nesterov', False)
            group.setdefault('maximize', False)

    @torch.no_grad()
    def step(self, closure=None):
        """更新参数
        Arguments:
          closure (callable, optional): A closure that reevaluates the model
              and returns the loss.
        """
        loss = None
        if closure isnotNone:
            loss = closure()

        """self.param_groups 是在父类的__init__函数中创建的"""
        for group in self.param_groups:
            weight_decay = group['weight_decay']
            momentum = group['momentum']
            dampening = group['dampening']
            nesterov = group['nesterov']
            maximize = group['maximize']

            for p in group['params']:
                if p.grad isNone: continue
                d_p = p.grad.data
                if weight_decay != 0:
                    d_p.add_(weight_decay, p.data)
                if momentum != 0:
                    param_state = self.state[p]
                    if 'momentum_buffer'notin param_state:
                        buf = param_state['momentum_buffer'] = torch.clone(d_p).detach()
                    else:
                        buf = param_state['momentum_buffer']
                        buf.mul_(momentum).add_(1 - dampening, d_p)
                    if nesterov:
                        d_p = d_p.add(momentum, buf)
                    else:
                        d_p = buf

                # 对参数进行更新
                if maximize:
                    p.data.add_(group['lr'], d_p)
                else:
                    p.data.add_(-group['lr'], d_p)
        return loss
```
对比：
```python
from torch.optim import SGD
w0=optim(model, optim_fn=SGD, lr=0.1,weight_decay=0.5)
w0_custom = optim(model, optim_fn=CustomSGD, lr=0.1,weight_decay=0.5)
plot([w0,w0_custom], titles=['SGD','CustomSGD'])
```
![Fcant_2022-07-01_23-33-46.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656689953411-2bb87ede-cb8e-4b4b-b4d0-5ff09de467a0.png#clientId=u6963e43a-160b-4&from=ui&id=u459d963b&originHeight=818&originWidth=1688&originalType=binary&ratio=1&rotation=0&showTitle=false&size=192139&status=done&style=shadow&taskId=u6dac5ad3-4003-4a18-9121-3ff335e9537&title=)
<a name="V8j0t"></a>
## 2、Adam
将Momentum算法和RMSProp算法结合起来使用的一种算法，既用动量来累积梯度，又使得收敛速度更快同时使得波动的幅度更小，并进行了偏差修正。<br />算法：<br />![2022-07-01-09-58-42.136684400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656641973948-b7c7c486-1c0f-4911-8651-19efe8ce86f5.png#clientId=u1278244e-6872-4&from=ui&id=L7dsS&originHeight=678&originWidth=649&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1322960&status=done&style=shadow&taskId=u4babe8d4-3bb8-4765-bfaf-ed923184046&title=)<br />函数：
```python
class torch.optim.Adam(params, lr=0.001, betas=(0.9, 0.999), eps=1e-08, weight_decay=0)[source]

'''
参数：
	betas (Tuple[float, float], 可选) – 用于计算梯度以及梯度平方的运行平均值的系数（默认：0.9，0.999）
	eps (float, 可选) – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
'''

https://arxiv.org/abs/1412.6980
```
自定义Adam优化器：
```python
import math


class CustomAdam(CustomOptimizer):
    def __init__(self, params, lr=0.001, betas=(0.9, 0.999), eps=1e-08, weight_decay=0, amsgrad=False, maximize=False):
        defaults = dict(lr=lr, betas=betas, eps=eps,
                        weight_decay=weight_decay, amsgrad=amsgrad, maximize=maximize)
        super(CustomAdam, self).__init__(params, defaults)

    def __setstate__(self, state):
        super(CustomAdam, self).__setstate__(state)
        for group in self.param_groups:
            group.setdefault('amsgrad', False)
            group.setdefault('maximize', False)

    @torch.no_grad()
    def step(self, closure=None):
        loss = None
        if closure isnotNone:
            loss = closure()

        for group in self.param_groups:
            for p in group['params']:
                if p.grad isNone: continue

                if group['maximize']:
                    grad = -p.grad.data
                else:
                    grad = p.grad.data

                if group['weight_decay'] != 0:
                    grad.add_(group['weight_decay'], p.data)

                """Adam Optimizer只能处理dense gradient，
                要想处理sparse gradient需要使用SparseAdam Optimizer"""
                if grad.is_sparse:
                    raise RuntimeError('Adam does not support sparse gradients, '
                                       'please consider SparseAdam instead')
                amsgrad = group['amsgrad']
                state = self.state[p]

                # 状态初始化
                if len(state) == 0:
                    state['step'] = 0
                    # 梯度值的指数移动平均
                    state['exp_avg'] = torch.zeros_like(p.data)
                    # 梯度平方值的指数移动平均
                    state['exp_avg_sq'] = torch.zeros_like(p.data)
                    if amsgrad:
                        # 保留最大的梯度平均和梯度平方
                        state['max_exp_avg_sq'] = torch.zeros_like(p.data)

                exp_avg, exp_avg_sq = state['exp_avg'], state['exp_avg_sq']

                if amsgrad:
                    max_exp_avg_sq = state['max_exp_avg_sq']

                beta1, beta2 = group['betas']

                state['step'] += 1
                bias_correction1 = 1 - beta1 ** state['step']
                bias_correction2 = 1 - beta2 ** state['step']

                # Decay the first and second moment running average coefficient
                exp_avg.mul_(beta1).add_(1 - beta1, grad)
                exp_avg_sq.mul_(beta2).addcmul_(1 - beta2, grad, grad)

                if amsgrad:
                    # Maintains the maximum of all 2nd moment running avg. till now
                    torch.max(max_exp_avg_sq, exp_avg_sq, out=max_exp_avg_sq)
                    # Use the max. for normalizing running avg. of gradient
                    denom = (max_exp_avg_sq.sqrt() / math.sqrt(bias_correction2)).add_(group['eps'])
                else:
                    denom = (exp_avg_sq.sqrt() / math.sqrt(bias_correction2)).add_(group['eps'])

                step_size = group['lr'] / bias_correction1

                p.data.addcdiv_(-step_size, exp_avg, denom)

        return loss
```
对比：
```python
from torch.optim import Adam
w0=optim(model, optim_fn=Adam, lr=0.1,weight_decay=0.5)
w0_custom = optim(model, optim_fn=CustomAdam, lr=0.1,weight_decay=0.5)
plot([w0,w0_custom], titles=['Adam','CustomAdam'])
```
![Fcant_2022-07-01_23-33-58.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656689953393-2d16f773-d2b6-4fda-b7d1-5a981667e151.png#clientId=u6963e43a-160b-4&from=ui&id=kOkM4&originHeight=813&originWidth=1723&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104891&status=done&style=shadow&taskId=ua7b87455-bb07-4523-99fe-3f7d6209cf2&title=)

 
