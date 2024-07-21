PyTorch<br />**主要讲单机多卡（单主机多GPUs训练）。**<br />使用多卡训练的方式有很多，当然前提是设备中存在多个GPU：使用命令**nvidia-smi**查看当前Ubuntu平台的GPU数量，其中**每个GPU被编上了序号：[0,1,2,3]。**
<a name="gBRSp"></a>
### 0、显卡编号（什么是主卡）
在默认情况下，**标号为0的显卡为主卡**，如主机中有**4块显卡**，那么每张显卡的**默认标号为[0,1,2,3]**。<br />如何将其他显卡设置为主卡呢？<br />通过`**os.environ["CUDA_VISIBLE_DEVICES"]**`指定所要使用的显卡，如：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = "3,2,0,1"
model = torch.nn.DataParallel(model, device_ids=[0,2,3]).cuda()
```
此时，**3号显卡就变成了主卡**，在使用`**torch.nn.DataParallel**`指定运算显卡时，显卡的对应关系如下：
```python
实际显卡编号----->运算显卡编号
    3     ----->     0（主卡）
    2     ----->     1
    0     ----->     2
    1     ----->     3
```
<a name="Pe1ou"></a>
### 1、使用所有存在的显卡
在存在多卡的条件下，最简单的方法是直接使用`**torch.nn.DataParallel**`将模型**wrap**一下即可：
```python
net = torch.nn.DataParallel(model)
```
这时，**默认所有存在的显卡都会被使用。**
<a name="oPKXY"></a>
### 2、指定编号使用显卡
如果有很多显卡(例如有4张显卡)，但**只想使用0、1、2号显卡**，那么可以：
```python
net = torch.nn.DataParallel(model, device_ids=[0, 1, 2])
```
或者：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = ','.join(map(str, [0,1,2])) # 一般在程序开头设置
# 等价于os.environ["CUDA_VISIBLE_DEVICES"] = '0,1,2'
net = torch.nn.DataParallel(model)
```
**CUDA_VISIBLE_DEVICES** 表示**当前可以被python环境程序检测到的显卡。**<br />如果只需要**指定一张卡**，可以使用`**torch.cuda.set_device(1)**`指定gpu使用编号(不建议用这个方法)。
```python
os.environ["CUDA_VISIBLE_DEVICES"] = ','.join(map(str, [0,1,2])) # 一般在程序开头设置
# 等价于os.environ["CUDA_VISIBLE_DEVICES"] = '0,1,2'
net = torch.nn.DataParallel(model)
```
测试的机器是4卡，所以`print`结果是：4，说明用`torch.cuda.set_device(1)`指定，不会改变可见的显卡。<br />后面还可以用`**torch.nn.DataParallel(model, device_ids=[1, 2])**`进行指定，但是**必须包含**`set_device(1)`指定的device:1的设备，缺点是仍然会存在占用一些device:0的gpu内存。
<a name="xb2dF"></a>
### 3、`os.environ["CUDA_VISIBLE_DEVICES"]`详解
<a name="bPJvE"></a>
#### 3.1. `["CUDA_VISIBLE_DEVICES"]`使用
CUDA_VISIBLE_DEVICES 表示当前可以被python环境程序检测到的显卡。
```python
os.environ["CUDA_VISIBLE_DEVICES"] = ','.join(map(str, [0,1,2])) # 一般在程序开头设置
# 等价于os.environ["CUDA_VISIBLE_DEVICES"] = '0,1,2'
```
`**os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'**`进行指定使用设备这样会修改pytorch感受的设备编号，pytorch感知的编号还是从device:0开始。如上则把1号显卡改为device:0，2号显卡改为device:1，使用时应该这么写：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'
torch.nn.DataParallel(model, device_ids=[0,1])
```
<a name="s7n9S"></a>
#### 3.2. 关于设置`["CUDA_VISIBLE_DEVICES"]`无效的解决
不生效的原因是，这一行代码放置的**位置不对**。<br />一定要把`os.environ['CUDA_VISIBLE_DEVICES']='1'`**放在所有访问GPU的代码之前**。
<a name="rNjMR"></a>
### 4、`torch.cuda`主要函数
<a name="YdQWv"></a>
#### 4.1. `torch.cuda.is_available()`判断GPU是否可用
```python
import torch
print(torch.cuda.is_available())
```
可用即为：True
<a name="Uxkrj"></a>
#### 4.2. `torch.cuda.device_count()`查看可用GPU数量
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'
print(torch.cuda.current_device())
```
结果应该为：3
<a name="m1QdS"></a>
#### 4.3. `torch.cuda.current_device()`查看当前使用的GPU序号
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'
print(torch.cuda.current_device())
```
结果应该是：0<br />这样就证明了上面说的，`os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'`进行指定使用设备，会修改pytorch感受的设备编号，pytorch感知的编号还是从device:0开始。
<a name="P5TTZ"></a>
#### 4.4 其他一些函数
**查看指定GPU的容量、名称：**
```python
torch.cuda.get_device_capability(device)
torch.cuda.get_device_name(device)
```
**清空程序占用的GPU资源：**
```python
torch.cuda.empty_cache()
```
**为GPU设置随机种子：**
```python
torch.cuda.manual_seed(seed)
torch.cuda.manual_seed_all(seed)
```
<a name="MOTT9"></a>
### 5、报错AssertionError: Invalid device id
对模型进行gpu指定编号多gpu训练，**必须要有所指定编号的gpu**，不然会报**AssertionError: Invalid device id**错误。
<a name="S86Su"></a>
#### 报错示例1：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '0,1' # 一般在程序开头设置
net = torch.nn.DataParallel(model,device_ids=[1, 2])
```
CUDA_VISIBLE_DEVICES 表示当前可以被python环境程序检测到的显卡，可见的只有0,1号显卡。<br />而使用`torch.nn.DataParallel(model, device_ids=[1, 2])`指定gpu编号会出现AssertionError: Invalid device id错误<br />原因：2号显卡没有设置被可见。
<a name="qOUwk"></a>
#### 报错示例2：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '1,2' # 一般在程序开头设置
net = torch.nn.DataParallel(model,device_ids=[1, 2])
```
`os.environ["CUDA_VISIBLE_DEVICES"] = '1,2'`进行指定使用设备。这样会修改pytorch感受的设备编号，pytorch感知的编号还是从device:0开始。<br />程序应该这么改：
```python
os.environ["CUDA_VISIBLE_DEVICES"] = ','.join(map(str, [1,2])) # 一般在程序开头设置
net = torch.nn.DataParallel(model,device_ids=[0, 1])
```
<a name="XCwmH"></a>
### 6、报错RuntimeError: module must have its parameters and buffers on device cuda:1 (device_ids[0]) but found
PyTorch框架下，使用`torch.nn.DataParallel`进行多卡并行计算中可能会遇到该问题，是由于在多卡运算时**主卡设置所导致**。在多卡并行计算时，所使用的显卡中**必须包含主卡**，否则就会产生该问题。<br />如下代码中，主卡没有参与运算，就会导致该错误。
```python
os.environ["CUDA_VISIBLE_DEVICES"] = '0,1,2,3' #这里主卡就是机器0号卡
model = torch.nn.DataParallel(model, device_ids=[1,2,3]).cuda()
```
