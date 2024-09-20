PyTorch
```python
#  用pytorch简单的构建全连接层神经网络
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
class NN(nn.Module):
    def __init__(self, input_size, num_classes):
        super(NN, self).__init__()
        self.fc1 = nn.Linear(input_size, 50)
        self.fc2 = nn.Linear(50, num_classes)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = self.fc2(x)
        return x

#--------------------------------------------------------------------
# Set device cuda for GPU if it's available otherwise run on the CPU
device  = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# 参数设置
input_size = 784
num_classes = 10
learning_rate = 0.001
batch_size = 64
num_epochs = 3

#--------------------------------------------------------------------
# Load Training and Test data
train_dataset = datasets.MNIST(root='dataset/',train=True,transform=transforms.ToTensor(),download=True)
test_dataset = datasets.MNIST(root='dataset/',train=False,transform=transforms.ToTensor(),download=True)
train_loader = DataLoader(dataset=train_dataset,shuffle=True,batch_size=batch_size)
test_loader = DataLoader(dataset=test_dataset,shuffle=True,batch_size=batch_size)

#--------------------------------------------------------------------
# 初始化网络
model = NN(input_size=input_size,num_classes=num_classes).to(device)

# loss and optimizer
criterion= nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(),lr=learning_rate)

# 训练网络
for epoch in range(num_epochs):
    for batch_idxm,(data,targets) in enumerate(tqdm(train_loader)):
        data = data.to(device)
        targets = targets.to(device)

        # [64,1,28,28] -> [64,1*28*28]
        print(data.shape)
        data = data.reshape(data.shape[0],-1)

        # 计算损失
        outputs = model(data)
        loss = criterion(outputs,targets)

        # 向后传播
        loss.backward()

        # 梯度归0
        optimizer.zero_grad()

        # 梯度优化
        optimizer.step()


#--------------------------------------------------------------------
# model save and load
def save_checkpoint(state, filename="my_checkpoint.pth.tar"):
    print("=> Saving checkpoint")
    torch.save(state, filename)


def load_checkpoint(checkpoint, model, optimizer):
    print("=> Loading checkpoint")
    model.load_state_dict(checkpoint["state_dict"])
    optimizer.load_state_dict(checkpoint["optimizer"])

checkpoint = {"state_dict": model.state_dict(), "optimizer": optimizer.state_dict()}
# Try save checkpoint
save_checkpoint(checkpoint)

# Try load checkpoint
load_checkpoint(torch.load("my_checkpoint.pth.tar"), model, optimizer)


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
            x = x.reshape(x.shape[0], -1)

            outputs = model(x)
            _, indexes = outputs.max(1)
            num_correct += (indexes == y).sum()
            num_samples += indexes.size(0) #  batch_size

    model.train()
    return num_correct/num_samples


#--------------------------------------------------------------------
print(f"Accuracy on training set: {check_accuracy(train_loader, model)*100:.2f}")
print(f"Accuracy on test set: {check_accuracy(test_loader, model)*100:.2f}")

# Accuracy on training set: 15.65
# Accuracy on test set: 15.53
```
