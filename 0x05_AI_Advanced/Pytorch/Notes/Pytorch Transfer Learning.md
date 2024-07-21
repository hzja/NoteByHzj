PyTorch<br />所谓**迁移学习**，就是能让现有的模型算法**稍加调整**即可应用于一个**新的领域**和功能的一项技术。
<a name="D5XPX"></a>
## 1、加载训练好的模型
```python
import torchvision

# Gives easier dataset managment and creates mini batches
from torch.utils.data import DataLoader

# Has standard datasets we can import in a nice way
import torchvision.datasets as datasets 

# Transformations we can perform on our dataset
import torchvision.transforms as transforms  

# Load pretrain model & modify it
model = torchvision.models.vgg16(pretrained=True)

print(model)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657176166930-d903ef46-b6e9-4b38-a703-72d989e198ca.png#clientId=u5dbf6ba8-e802-4&from=paste&id=u6689cc69&originHeight=773&originWidth=620&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u410639be-cf1c-43c8-8adb-5303bb0aa3a&title=)
<a name="s1IZL"></a>
## 2、特征层处理
```python
train_dataset = datasets.CIFAR10(
    root="dataset/", train=True, transform=transforms.ToTensor(), download=True
)
train_loader = DataLoader(dataset=train_dataset, batch_size=batch_size, shuffle=True)
model2 = model.features
model2.eval()
train = next(iter(train_loader))
print(len(train))
x = train[0].to(device)
y = train[1].to(device)
y_pred = model2(x) 

print(y_pred.shape)
# torch.Size([64, 512, 1, 1])
```
结果是在所有的conv层之后，avgpool层的输入是1x1(即512x1x1) (avgpool工作的维度，即1x1)，所以这里不需要执行avgpool(理论上可以应用大小为1x1的avgpool，而不是使用自定义Identity层，结果是一样的)。<br />顺便说一下，avgpool的参数是“output_size”，而不是“kernel_size”。这意味着它保证最终输出是请求的大小。在这种情况下，1x1将被复制为7x7，这没有任何好处。
```python
class Identity(nn.Module):
    def __init__(self):
        super(Identity, self).__init__()

    def forward(self, x):
        return x
```
因此可以训练以下模型：
```python
# Load pretrain model & modify it
model = torchvision.models.vgg16(pretrained=True)
model.avgpool = Identity()
model.classifier = nn.Linear(512,10)

# 如果只想改变classifier中的某一层，
# 可用model.classifier[i]=nn.Linear() 或者nn.Relu()，
# i是某层所在的索引，本模型中classifier有7层
```
结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657176167320-d7d60eca-4ee3-4eb2-a94f-83006c837cb4.png#clientId=u5dbf6ba8-e802-4&from=paste&id=u8db4c23b&originHeight=60&originWidth=514&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad88c830-2e47-43ac-b456-a49f1d5dbf7&title=)
<a name="LLoDQ"></a>
## 3、训练部分模型
```python
# 如果不想训练整个模型，可将模型的参数的梯度设置为False。
for param in model.parameters():
    param.requires_grad = False

# 同时对需要训练的部分重新设置网络。
model.avgpool = Identity()
model.classifier = nn.Sequential(
    nn.Linear(512, 100), nn.ReLU(), nn.Linear(100, num_classes)
)
```
