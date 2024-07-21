PyTorch<br />PyTorch作为一款流行深度学习框架其热度大有超越TensorFlow的感觉。根据此前的统计，目前TensorFlow虽然仍然占据着工业界，但PyTorch在视觉和NLP领域的顶级会议上已呈一统之势。<br />这里聚焦于PyTorch的自定义数据读取`pipeline`模板和相关`trciks`以及如何优化数据读取的`pipeline`等。从PyTorch的数据对象类`Dataset`开始。`Dataset`在PyTorch中的模块位于`utils.data`下。
```python
from torch.utils.data import Dataset
```
将围绕`Dataset`对象分别从原始模板、`torchvision`的`transforms`模块、使用pandas来辅助读取、torch内置数据划分功能和`DataLoader`来展开阐述。
<a name="G0ZRM"></a>
### `Dataset`原始模板
PyTorch官方提供了自定义数据读取的标准化代码代码模块，作为一个读取框架，这里称之为原始模板。其代码结构如下：
```python
from torch.utils.data import Dataset
class CustomDataset(Dataset):
    def __init__(self, ...):
        # stuff
        
    def __getitem__(self, index):
        # stuff
        return (img, label)
        
    def __len__(self):
        # return examples size
        return count
```
根据这个标准化的代码模板，只需要根据自己的数据读取任务，分别往`__init__()`、`__getitem__()`和`__len__()`三个方法里添加读取逻辑即可。作为PyTorch范式下的数据读取以及为了后续的data loader，三个方法缺一不可。其中：

- `__init__()`函数用于初始化数据读取逻辑，比如读取包含标签和图片地址的csv文件、定义transform组合等。
- `__getitem__()`函数用来返回数据和标签。目的上是为了能够被后续的dataloader所调用。
- `__len__()`函数则用于返回样本数量。

现在往这个框架里填几行代码来形成一个简单的数字案例。创建一个从1到100的数字例子：
```python
from torch.utils.data import Dataset
class CustomDataset(Dataset):
    def __init__(self):
        self.samples = list(range(1, 101))
    def __len__(self):
        return len(self.samples)
    def __getitem__(self, idx):
        return self.samples[idx]
        
if __name__ == '__main__':
    dataset = CustomDataset()
    print(len(dataset))
    print(dataset[50])
    print(dataset[1:100])
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658030391631-1ecad14b-4b29-4c9e-b151-355162efcd1f.png#clientId=uf38a9b6a-6501-4&from=paste&id=u85baec67&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uabb2222b-beef-49f6-a927-c0dd5b0a31d&title=)
<a name="Y1M6d"></a>
### 添加`torchvision.transforms`
然后来看如何从内存中读取数据以及如何在读取过程中嵌入`torchvision`中的`transforms`功能。`torchvision`是一个独立于torch的关于数据、模型和一些图像增强操作的辅助库。主要包括`datasets`默认数据集模块、models经典模型模块、`transforms`图像增强模块以及utils模块等。在使用torch读取数据的时候，一般会搭配上`transforms`模块对数据进行一些处理和增强工作。<br />添加了`tranforms`之后的读取模块可以改写为：
```python
from torch.utils.data import Dataset
from torchvision import transforms as T

class CustomDataset(Dataset):
    def __init__(self, ...):
        # stuff
        ...
        # compose the transforms methods
        self.transform = T.Compose([T.CenterCrop(100),
                                T.ToTensor()])
        
    def __getitem__(self, index):
        # stuff
        ...
        data = # Some data read from a file or image
        # execute the transform
        data = self.transform(data)  
        return (img, label)
        
    def __len__(self):
        # return examples size
        return count
        
if __name__ == '__main__':
    # Call the dataset
    custom_dataset = CustomDataset(...)
```
可以看到，使用了`Compose`方法来把各种数据处理方法聚合到一起进行定义数据转换方法。通常作为初始化方法放在`__init__()`函数下。以猫狗图像数据为例进行说明。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658030391636-fa1e8285-f000-4fea-a673-f30dd8f4e789.jpeg#clientId=uf38a9b6a-6501-4&from=paste&id=u7c87b263&originHeight=278&originWidth=450&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda85e68b-5435-4824-9330-71bd6af1bf7&title=)

定义数据读取方法如下：
```python
class DogCat(Dataset):    
    def __init__(self, root, transforms=None, train=True, val=False):
        """
        get images and execute transforms.
        """
        self.val = val
        imgs = [os.path.join(root, img) for img in os.listdir(root)]
        # train: Cats_Dogs/trainset/cat.1.jpg
        # val: Cats_Dogs/valset/cat.10004.jpg
        imgs = sorted(imgs, key=lambda x: x.split('.')[-2])
        self.imgs = imgs         
        if transforms is None:
            # normalize      
            normalize = T.Normalize(mean = [0.485, 0.456, 0.406],
                                     std = [0.229, 0.224, 0.225])
            # trainset and valset have different data transform 
            # trainset need data augmentation but valset don't.
            # valset

            if self.val:
                self.transforms = T.Compose([
                    T.Resize(224),
                    T.CenterCrop(224),
                    T.ToTensor(),
                    normalize
                ])
            # trainset
            else:
                self.transforms = T.Compose([
                    T.Resize(256),
                    T.RandomResizedCrop(224),
                    T.RandomHorizontalFlip(),
                    T.ToTensor(),
                    normalize
                ])
                       
    def __getitem__(self, index):
        """
        return data and label
        """
        img_path = self.imgs[index]
        label = 1 if 'dog' in img_path.split('/')[-1] else 0
        data = Image.open(img_path)
        data = self.transforms(data)
        return data, label
  
    def __len__(self):
        """
        return images size.
        """
        return len(self.imgs)

if __name__ == "__main__":
    train_dataset = DogCat('./Cats_Dogs/trainset/', train=True)
    print(len(train_dataset))
    print(train_dataset[0])
```
因为这个数据集已经分好了训练集和验证集，所以在读取和`transforms`的时候需要进行区分。运行示例如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658030391642-2754ac82-9a59-4c8f-aba6-612b19d305e9.png#clientId=uf38a9b6a-6501-4&from=paste&id=u41b9df06&originHeight=477&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1ef42d1d-5ce7-40f4-b289-e7f300e8718&title=)
<a name="qEnyj"></a>
### 与pandas一起使用
很多时候数据的目录地址和标签都是通过csv文件给出的。如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658030391637-62eb08a5-d606-4458-8044-1c550f11f625.png#clientId=uf38a9b6a-6501-4&from=paste&id=ua0cb947f&originHeight=546&originWidth=911&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u17ade572-ac98-459a-a23e-e7bffd45ca1&title=)<br />此时在数据读取的`pipeline`中需要在`__init__()`方法中利用pandas把csv文件中包含的图片地址和标签融合进去。相应的数据读取`pipeline`模板可以改写为：
```python
class CustomDatasetFromCSV(Dataset):
    def __init__(self, csv_path):
        """
        Args:
            csv_path (string): path to csv file
            transform: pytorch transforms for transforms and tensor conversion
        """
        # Transforms
        self.to_tensor = transforms.ToTensor()
        # Read the csv file
        self.data_info = pd.read_csv(csv_path, header=None)
        # First column contains the image paths
        self.image_arr = np.asarray(self.data_info.iloc[:, 0])
        # Second column is the labels
        self.label_arr = np.asarray(self.data_info.iloc[:, 1])
        # Calculate len
        self.data_len = len(self.data_info.index)

    def __getitem__(self, index):
        # Get image name from the pandas df
        single_image_name = self.image_arr[index]
        # Open image
        img_as_img = Image.open(single_image_name)
        # Transform image to tensor
        img_as_tensor = self.to_tensor(img_as_img)
        # Get label of the image based on the cropped pandas column
        single_image_label = self.label_arr[index]
        return (img_as_tensor, single_image_label)

    def __len__(self):
        return self.data_len

if __name__ == "__main__":
    # Call dataset
    dataset =  CustomDatasetFromCSV('./labels.csv')
```
以mnist_label.csv文件为示例：
```python
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from torchvision import transforms as T
from PIL import Image
import os
import numpy as np
import pandas as pd

class CustomDatasetFromCSV(Dataset):
    def __init__(self, csv_path):
        """
        Args:
            csv_path (string): path to csv file            
            transform: pytorch transforms for transforms and tensor conversion
        """
        # Transforms
        self.to_tensor = T.ToTensor()
        # Read the csv file
        self.data_info = pd.read_csv(csv_path, header=None)
        # First column contains the image paths
        self.image_arr = np.asarray(self.data_info.iloc[:, 0])
        # Second column is the labels
        self.label_arr = np.asarray(self.data_info.iloc[:, 1])
        # Third column is for an operation indicator
        self.operation_arr = np.asarray(self.data_info.iloc[:, 2])
        # Calculate len
        self.data_len = len(self.data_info.index)

    def __getitem__(self, index):
        # Get image name from the pandas df
        single_image_name = self.image_arr[index]
        # Open image
        img_as_img = Image.open(single_image_name)
        # Check if there is an operation
        some_operation = self.operation_arr[index]
        # If there is an operation
        if some_operation:
            # Do some operation on image
            # ...
            # ...
            pass

        # Transform image to tensor
        img_as_tensor = self.to_tensor(img_as_img)
        # Get label of the image based on the cropped pandas column
        single_image_label = self.label_arr[index]
        return (img_as_tensor, single_image_label)

    def __len__(self):
        return self.data_len

if __name__ == "__main__":
    transform = T.Compose([T.ToTensor()])
    dataset = CustomDatasetFromCSV('./mnist_labels.csv')
    print(len(dataset))
    print(dataset[5])
```
运行示例如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658030391659-2cd9cf80-4b4e-4c08-8f03-b7c91c44113c.png#clientId=uf38a9b6a-6501-4&from=paste&id=ub8f190aa&originHeight=490&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u43f06069-652e-4465-ab40-4a32b754937&title=)
<a name="hrL78"></a>
### 训练集验证集划分
一般来说，为了模型训练的稳定，需要对数据划分训练集和验证集。torch的`Dataset`对象也提供了`random_split`函数作为数据划分工具，且划分结果可直接供后续的`DataLoader`使用。<br />以kaggle的花朵数据为例：
```python
from torch.utils.data import DataLoader
from torchvision.datasets import ImageFolder
from torchvision import transforms as T
from torch.utils.data import random_split

transform = T.Compose([
    T.Resize((224, 224)),
    T.RandomHorizontalFlip(),
    T.ToTensor()
 ])

dataset = ImageFolder('./flowers_photos', transform=transform)
print(dataset.class_to_idx)

trainset, valset = random_split(dataset, 
                [int(len(dataset)*0.7), len(dataset)-int(len(dataset)*0.7)])

trainloader = DataLoader(dataset=trainset, batch_size=32, shuffle=True, num_workers=1)
for i, (img, label) in enumerate(trainloader):
    img, label = img.numpy(), label.numpy()
    print(img, label)

valloader = DataLoader(dataset=valset, batch_size=32, shuffle=True, num_workers=1)
for i, (img, label) in enumerate(trainloader):
    img, label = img.numpy(), label.numpy()
    print(img.shape, label)
```
这里使用了ImageFolder模块，可以直接读取各标签对应的文件夹，部分运行示例如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658030392050-8728526b-0b85-491c-80ac-d5c050ea35ec.png#clientId=uf38a9b6a-6501-4&from=paste&id=u9ed39c92&originHeight=411&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8394936f-6ab9-4de2-b59a-208ef587ad7&title=)

<a name="XFRxg"></a>
### 使用DataLoader
dataset方法写好之后，还需要使用`DataLoader`将其逐个喂给模型。上一节的数据划分已经用到了`DataLoader`函数。从本质上来讲，`DataLoader`只是调用了`__getitem__()`方法并按批次返回数据和标签。使用方法如下：
```python
from torch.utils.data import DataLoader
from torchvision import transforms as T

if __name__ == "__main__":
	# Define transforms
	transformations = T.Compose([T.ToTensor()])
	# Define custom dataset
	dataset = CustomDatasetFromCSV('./labels.csv')
	# Define data loader
	data_loader = DataLoader(dataset=dataset, batch_size=10, shuffle=True)
	for images, labels in data_loader:
        # Feed the data to the model
```
以上就是PyTorch读取数据的Pipeline主要方法和流程。基于Dataset对象的基本框架不变，具体细节可自定义化调整。
