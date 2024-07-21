Python<br />![](./img/1697593510266-ca0e5bf9-c2f8-4b34-b952-7a7133e20d85.png)<br />Numpy是Python的一个非常基础且通用的库，基本上常见的库Pandas，Opencv，Pytorch，TensorFlow等都会用到。<br />Numpy的核心就是n维array，这里介绍一维，二维和多维array。
<a name="jTKzg"></a>
## Numpy与List的异同点
他俩非常相似，**同样都是容器，都能快速的取值的修改值，但是插入和删除会慢一点**。<br />![](./img/1697593510349-83d0652a-c374-49e3-81e6-7ff57888d665.png)<br />Numpy的优点

- 更**紧凑**，特别是**多维数据**
- 当数据可以**向量化**的时候比list更快
- 通常是**同质化**的，数据相同时处理更快，比如都是浮点型或者整数型

![](./img/1697593510307-25d0a05b-1e50-4a62-8848-6afc351603c1.png)
<a name="H3ZeZ"></a>
## 向量 Vector 或者一维向量 1D array
<a name="GLW9O"></a>
### 向量初始化
通过list转化，自动变成np类型，**_shape_**为（3，）<br />![](./img/1697593510269-d7547377-f33e-4def-9486-dc29e8e4de5e.png)
:::danger
注意，如果list里面的值类型不相同，那么**_dtype_**就会返回”object“
:::
如果暂时没有想要转化的list，可以全用0代替<br />![](./img/1697593510346-76a76f78-a54c-48be-83c0-e89bb2d38ca4.png)<br />也可以复制一个已经存在的全0 向量<br />![](./img/1697593510603-e5ebe1c5-d510-4621-9f2a-9761ef5e185c.png)
:::danger
注意，所有创建包含固定值_vector_的方法都有_**_like**_函数
:::
![](./img/1697593510648-f4ec3c8f-e5b7-486a-bd21-4a9bfbb6aaec.png)<br />还有经典的_arange_和_linspace_方法<br />![](./img/1697593510679-31d68419-5e8d-423a-a784-62c713909f92.png)<br />![](./img/1697593510826-3d66c414-4660-470e-9813-efe022d2c1da.png)<br />**arange**方法对于数据类型敏感，比如**_arange_**(3)，**_dtype_** 为int，如果需要float类型，可以`_arange(3).astype(float)_`
<a name="KjNZe"></a>
#### 生成随机array
![](./img/1697593510811-80e6297d-0f54-4062-a15e-67fa3c78bedc.png)
<a name="vmiMv"></a>
### 向量索引
基础的向量索引操作，**只是展示部分数据，而不改变数据本身**<br />![](./img/1697593511112-1933d522-a4e4-45b0-bf40-33e7eceb0fb2.png)<br />布尔操作<br />![](./img/1697593511120-6267f76c-f965-4b89-96d0-0e27cb43f254.jpeg)<br />也可以用`.**_where_**` 和**_clip_**代替上面的方法<br />![](./img/1697593511124-f122354a-fde2-41c9-b82a-205e6b80116b.png)
<a name="W4Lm1"></a>
### 向量操作
numpy的优势就是把vector当做数做整体运算，避免循环运算<br />![](./img/1697593511206-1594c5f1-da07-4ed3-808e-88c34c55bfa3.png)
<a name="TMdkA"></a>
#### - * /无所不能
![](./img/1697593511231-6eb9d188-86db-4fb1-9104-5c6a49278c2d.png)
<a name="TDRUl"></a>
#### 复杂的数学运算不在话下
![](./img/1697593511497-fcd8bfef-e2f4-43f8-bd00-d01f67f8eb0a.png)
<a name="J9iHe"></a>
#### 标量运算


![](./img/1697593511495-ae1a438d-a631-433e-bff4-012cf5fda121.png)
<a name="hEEUF"></a>
####  三角函数<br />![](./img/1697593511589-444e4380-df65-459d-9643-c2f12c5ea17a.jpeg)
<a name="IRDA3"></a>
#### 整体取整
![](./img/1697593511631-a703564b-7812-4116-81fd-639239c1eff6.png)<br />numpy还可以做基础的统计操作，比如**_max，min， mean， sum_**等<br />![](./img/1697593511660-3cc57be9-5a5c-4d71-855b-e22e123b01a9.png)<br />排序操作<br />![](./img/1697593511927-d0d6541b-cce9-42b5-aa9b-b000b29dc7da.png)
<a name="Md5Yi"></a>
### 查找操作
numpy不像list有**_index_**函数，通常会用**_where_**等操作<br />![](./img/1697593512010-eb1b3566-5378-4ca0-985c-7f76f71b83e2.png)<br />其中有三种方法：

1. where，难懂且对于x处于array末端很不友好
2. next，相对较快，但需要numba
3. searchsorted，针对于已排过序的array
<a name="jf4MF"></a>
## 二维array，也称matrix矩阵
初始化，注意「双括号」<br />![](./img/1697593512068-a6881c14-dbaf-4d5c-9618-922e1b6e31a0.png)<br />随机matrix，同一维类似<br />![](./img/1697593512102-4ff1b1ac-01b1-4791-b5b2-00fae1cfb168.png)<br />索引操作，不改变matrix本身<br />![](./img/1697593512437-62fbea75-579c-42d5-95f5-f1e026044958.png)<br />Axis 轴操作，在matrix中，axis = 0 代表列， axis = 1 代表行，默认axis = 0<br />![](./img/1697593512596-511322ce-8a44-4437-8277-9ec6c90edac6.png)<br />matrix算术 + - * / 和 ** 都可<br />![](./img/1697593512534-cd4c3f8c-f45a-4b8e-9edb-ce208793d297.png)<br />也可以matrix与单个数，matrix与vector，vector与vector进行运算<br />![](./img/1697593512592-d188e7dd-34bf-4a94-bcf5-20eeb8f52b6e.png)<br />![](./img/1697593512722-bd1f7ebf-02e3-4c29-8c98-c44df111a3e5.png)<br />行向量 列向量<br />二维的转置如下，一维的也就是vector转置为自己本身<br />![](./img/1697593512910-6582a303-38a0-4994-8c01-6838b8e1bf94.png)<br />reshape改变形态<br />![](./img/1697593512942-01bb60d3-1204-4e0b-b6fe-5916ff3a2230.png)<br />自此，三种向量，一维array，二维列vector，二维行向量<br />![](./img/1697593512984-bb7e178b-a430-482d-a129-d42155786eb6.png)
<a name="Gyek4"></a>
### 矩阵操作
<a name="vapkc"></a>
#### 合并matrix，hstack横向，vstack纵向，也可以理解为堆叠
![](./img/1697593513058-d25787c8-6384-4426-a13a-812c5926bdc2.png)
<a name="VYnMM"></a>
#### 反向操作hsplit和vsplit
![](./img/1697593513046-f47f0f2d-182f-4a0e-87a6-59b7d9f3f871.png)
<a name="WgAmi"></a>
#### matrix的复制操作，tile整个复制，repeat可以理解为挨个复制
![](./img/1697593513400-aed2ea65-bfaf-4624-ab62-6e99d7cb9b81.png)
<a name="EBC1S"></a>
#### delete删除操作
![](./img/1697593513371-5d5be72b-b412-4f06-8d25-2731984dd8e3.png)
<a name="HA09b"></a>
#### 删除的同时也可以插入
![](./img/1697593513366-d47d6571-a279-46ff-926c-f5dc987adc9e.png)
<a name="cQzuI"></a>
#### append操作，只能在末尾操作
![](./img/1697593513502-1f1e2880-7b1c-404d-8381-f41b7062eca8.jpeg)
<a name="E5mAG"></a>
#### 如果只增加固定值，也可以用pad
![](./img/1697593513546-1b48be4c-9a7f-4800-b8bf-5e14d9799e11.png)
<a name="fwkFC"></a>
#### 网格化
c和python都很麻烦，跟别说再大点的数了<br />![](./img/1697593513758-c2c9fb58-0613-48cd-b81a-98e6c699b326.png)<br />采用类似MATLAB会更快点<br />![](./img/1697593513849-1df4afce-5caa-4186-afc0-ef8ed78ea0c8.png)<br />**当然numpy有更好的办法**<br />![](./img/1697593513840-8511d972-16d6-4542-92e6-0022c0e6f595.png)
<a name="x8b3w"></a>
### matrix统计
<a name="au9Zk"></a>
#### sum,min,max,mean,median等等
![](./img/1697593513964-978ed84e-6d2b-494b-9231-0f73b2b8ee07.png)
<a name="dWxxp"></a>
#### argmin和argmax返回最小值和最大值的下标
![](./img/1697593514042-debf990c-fb3a-4790-9668-bae73efa8094.png)
<a name="AUXJ7"></a>
#### all和any也可以用
![](./img/1697593514141-2329f156-d406-403e-8eee-c494b0afa1e6.png)
<a name="MnIYo"></a>
### matrix排序，注意axis
![](./img/1697593514255-a2693b0b-3950-47bb-871c-75f14e543ed2.png)
<a name="Gngyp"></a>
## 3D array或者以上
初始化，**_reshape_**或者硬来<br />![](./img/1697593514319-c0279b2f-7683-4af8-aea3-43a7c93d5cf1.png)<br />可以考虑把数据抽象成一层层的数据<br />就像RGB值的图像一样<br />![](./img/1697593514364-4e3d5d89-51f9-402d-869d-89fd40e47c5b.jpeg)<br />跟1D和2D类似的操作，**_zeros_**， **_ones_**，**_rand_**等<br />![](./img/1697593514454-ace1ffa2-7b3a-4fba-ac2c-7e086de159c2.jpeg)<br />**_vstack_**和**_hstack_**照样可以用，现在多了一个dstack，代表维度的堆叠<br />![](./img/1697593514485-2a69ce80-c119-462e-ba5f-8615fcd92682.png)<br />**_concatenate_**也有同样的效果<br />![](./img/1697593514735-7ffd20f6-12d4-42d8-879f-492f42c7e377.png)
<a name="HgG5p"></a>
## 总结
本文总结了numpy对于1D，2D和多维的基本操作。
