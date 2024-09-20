PyTorchCNN
```python
#  用pytorch简单的构建两层卷积网络
#  对手写字进行识别

import torch
import torchvision

# 参数函数，类似激活函数
import torch.nn.functional as F

# standard datasets
import torchvision.datasets as datasets

# 可以对数据集进行转换
import torchvision.transforms as transforms

from torch import optim  # 参数优化
from torch import nn  # 所有神经网络模块

# Gives easier dataset managment by creating mini batches etc
from torch.utils.data import DataLoader

# for nice progress bar!
from tqdm import tqdm


#--------------------------------------------------------------------
class CNN(nn.Module):
    def __init__(self, in_channels=1, num_classes=10):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv1d(in_channels=in_channels,
                    out_channels=8,
                    kernel_size=(3,3),
                    stride=(1,1),
                    padding=(1,1))
        
        self.pool = nn.MaxPool2d(kernel_size=(2,2),stride=(2,2))

        self.conv2 = nn.Conv2d(in_channels=8,
                    out_channels=16,
                    kernel_size=(3,3),
                    stride=(1,1),
                    padding=(1,1))
        
        self.fc = nn.Linear(16*7*7, num_classes)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = self.pool(x)
        x = F.relu(self.conv2(x))
        x = self.pool(x)
        x = x.reshape(x.shape[0],-1)
        x = self.fc(x)
        return x


#--------------------------------------------------------------------
# Set device cuda for GPU if it's available otherwise run on the CPU
device  = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# 参数设置
in_channels = 1
num_classes = 10
learning_rate = 0.001
batch_size = 64
num_epochs = 3

# Load Training and Test data
train_dataset = datasets.MNIST(root='dataset/',train=True,transform=transforms.ToTensor(),download=True)
test_dataset = datasets.MNIST(root='dataset/',train=False,transform=transforms.ToTensor(),download=True)
train_loader = DataLoader(dataset=train_dataset,shuffle=True,batch_size=batch_size)
test_loader = DataLoader(dataset=test_dataset,shuffle=True,batch_size=batch_size)


#--------------------------------------------------------------------
# 初始化网络
model = CNN(in_channels=in_channels,num_classes=num_classes).to(device)

# loss and optimizer
criterion= nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(),lr=learning_rate)

# 训练网络
for epoch in range(num_epochs):
    for batch_idxm,(data,targets) in enumerate(tqdm(train_loader)):
        data = data.to(device)
        targets = targets.to(device)       

        # forward
        outputs = model(data)

        # 计算损失
        loss = criterion(outputs,targets)

        # 向后传播
        loss.backward()

        # 梯度归0
        optimizer.zero_grad()

        # 梯度优化
        optimizer.step()


#--------------------------------------------------------------------
# Check accuracy on training & test to see how good our model
def check_accuracy(loader, model):
    num_correct = 0
    num_samples = 0
    model.eval()

    with torch.no_grad():
        for x, y in loader:
            x = x.to(device=device)
            y = y.to(device=device)

            outputs = model(x)
            _, indexes = outputs.max(1)
            num_correct += (indexes == y).sum()
            num_samples += indexes.size(0) #  batch_size

    model.train()
    return num_correct/num_samples


#--------------------------------------------------------------------
print(f"Accuracy on training set: {check_accuracy(train_loader, model)*100:.2f}")
print(f"Accuracy on test set: {check_accuracy(test_loader, model)*100:.2f}")

# Accuracy on training set: 9.62
# Accuracy on test set: 9.59
```
