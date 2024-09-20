PyTorchRNN
```python
# Example code of a simple RNN, GRU, LSTM on the MNIST dataset.

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

# Set device cuda for GPU if it's available otherwise run on the CPU
device  = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

#--------------------------------------------------------------------
# Recurrent neural network (many-to-one)
class RNN(nn.Module):
  def __init__(self,input_size,hidden_size,num_layers,num_classes):
    super(RNN,self).__init__()
    self.hidden_size = hidden_size
    self.num_layers = num_layers
    self.rnn = nn.RNN(input_size,hidden_size,num_layers,batch_first=True)
    self.fc = nn.Linear(hidden_size*seq_len,num_classes)
  def forward(self,x):
    # set initial hidden and cell stats
    h0 = torch.zeros(self.num_layers,x.size(0),self.hidden_size).to(device)

    out,_ = self.rnn(x,h0)
    out = out.reshape(x.shape[0],-1)
    out = self.fc(out)
    return out

#--------------------------------------------------------------------
class RNN_GRU(nn.Module):
  def __int__(self,input_size,hidden_size,num_layers,num_classes):
    super(RNN_GRU,self).__init__()
    self.hidden_size = hidden_size
    self.num_layers = num_layers
    selg.gru = nn.GRU(input_size,hidden_size,num_layers,batch_size=True)
    self.fc = nn.Linear(hidden_size*seq_len,num_classes)
  
  def forward(self,x):
    h0 = torch.zeros(num_layers,x.size(0),self.hidden_size).to(device)

    out,_ = self.gru(x,h0)
    out = out.reshape(x.size(0),-1)
    out =self.fc(out)
    return out

#--------------------------------------------------------------------
class RNN_LSTM(nn.Module):
  def __init__(self,input_size,hidden_size,num_layers,num_classes):
    super(RNN_LSTM,self).__init__()
    self.hidden_size = hidden_size
    self.num_layers = num_layers
    self.lstm = nn.LSTM(input_size,hidden_size,num_layers,batch_first=True)
    self.fc = nn.Linear(hidden_size*seq_len,num_classes)
  
  def forward(self,x):
    c0 = torch.zeros(num_layers,x.size(0),self.hidden_size)
    h0 = torch.zeros(num_layers,x.size(0),self.hidden_size)

    out,_ = self.lstm(x,(h0,c0)) 
    # out: tensor of shape (batch_size, seq_length, hidden_size)
    out = out.reshape(out.shape[0], -1)
    out = self.fc(out)
    return out

#--------------------------------------------------------------------
# 参数设置
input_size = 28
num_classes = 10
learning_rate = 0.001
batch_size = 64
num_epochs = 3
num_layers = 2
seq_len = 28
hidden_size = 256

# Load Training and Test data
train_dataset = datasets.MNIST(root='dataset/',train=True,transform=transforms.ToTensor(),download=True)
test_dataset = datasets.MNIST(root='dataset/',train=False,transform=transforms.ToTensor(),download=True)
train_loader = DataLoader(dataset=train_dataset,shuffle=True,batch_size=batch_size)
test_loader = DataLoader(dataset=test_dataset,shuffle=True,batch_size=batch_size)

#--------------------------------------------------------------------
# 初始化网络
model = RNN_LSTM(input_size, hidden_size, num_layers, num_classes).to(device)

# loss and optimizer
criterion= nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(),lr=learning_rate)

# 训练网络
for epoch in range(num_epochs):
    for batch_idxm,(data,targets) in enumerate(tqdm(train_loader)):
        data = data.to(device).squeeze(1)
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
            x = x.to(device=device).squeeze(1)
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

# Accuracy on training set: 10.22
# Accuracy on test set: 10.10
```
