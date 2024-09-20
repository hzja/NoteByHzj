Python Squeeze<br />以这个数据为例。<br />![2021-08-08-13-12-18-725054.png](./img/1628399641985-ccb01b44-f805-4dfa-8b25-8aa15f9b1e6e.png)<br />现在要提取`DataFrame`的中`volume`大于100000000的值。
```python
volume = df['Volume']
volume_highest = volume[volume > 100000000]
```
然后，在Jupyter Notebook的代码框里执行`volume_highest`，会看到结果是这样的。<br />![2021-08-08-13-12-18-970094.png](./img/1628399655934-88f1da4a-a442-4fb9-ab17-ce373ec8f003.png)<br />这个值前面还是跟着一个序号19，因为此时此刻它是个`Seires`结构，用`type`测试下就可以知道了。<br />![2021-08-08-13-12-19-209059.png](./img/1628399655929-713705d0-0186-4c31-99c2-e21a24466673.png)<br />但真正的需求是想把这个值赋给一个变量，如果是Seires类型一定会报错的。使用`squeeze`可以非常简单的处理这种情况。像下面这样一下就可以搞定了，可以直接赋给新的变量。
```python
volume_highest_scalar = volume_highest.squeeze()
```
![2021-08-08-13-12-19-586071.png](./img/1628399619222-4d6a08bf-43ed-4b34-b361-0fbd1b180223.png)<br />下面是`pandas`官方文档对`squeeze`的介绍。<br />![2021-08-08-13-12-20-041057.png](./img/1628399629355-ae75edd9-099e-4943-8130-50648685a878.png)<br />意思就是：

- 具有单个元素的`Series`或`DataFrame`被压缩为标量。
- 具有单列或单行的`DataFrame`被压缩为`Series`。
- 否则，对象不变。

因此，最开始举的例子只是第一种情况。当不知道对象是`Series`还是`DataFrame`，但是知道它只有一列时，`squeeze`方法最有用。在这种情况下，可以安全地调用`squeeze`以确保它变成一个`Series`。
