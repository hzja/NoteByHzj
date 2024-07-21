Tensorflow<br />TensorFlow 是一个开源的、基于 Python 的机器学习框架，它由 Google 开发，提供了 Python，C/C++、Java、Go、R 等多种编程语言的接口，并在图形分类、音频处理、推荐系统和自然语言处理等场景下有着丰富的应用，是目前最热门的机器学习框架。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654344088431-607a7fab-cf45-4195-8b53-86454e4c81fb.jpeg#clientId=ucf853bb6-6cfd-4&from=paste&id=u58860d6b&originHeight=630&originWidth=736&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udf2e5566-7910-410b-a0b8-94562b57ae0&title=)
<a name="gDC6t"></a>
## **一、Tensorflow的排序与张量**
Tensorflow允许用户把张量操作和功能定义为计算图。张量是通用的数学符号，代表保存数据值的多维列阵，张量的维数称为阶。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654344088434-1dd920d3-4d5a-4bac-a1f7-059376021261.jpeg#clientId=ucf853bb6-6cfd-4&from=paste&id=ubda675d8&originHeight=443&originWidth=929&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c0f1a07-1b6f-4c81-ab46-6f736f50a3a&title=)<br />引用相关的库
```python
import tensorflow as tf
import numpy as np
```
获取张量的阶（从下面例子看到tf的计算过程）
```python
# 获取张量的阶（从下面例子看到tf的计算过程）
g = tf.Graph()
# 定义一个计算图
with g.as_default():
    ## 定义张量t1,t2,t3
    t1 = tf.constant(np.pi)
    t2 = tf.constant([1,2,3,4])
    t3 = tf.constant([[1,2],[3,4]])
    
    ## 获取张量的阶
    r1 = tf.rank(t1)
    r2 = tf.rank(t2)
    r3 = tf.rank(t3)
    
    ## 获取他们的shapes
    s1 = t1.get_shape()
    s2 = t2.get_shape()
    s3 = t3.get_shape()
    print("shapes:",s1,s2,s3)
# 启动前面定义的图来进行下一步操作
with tf.Session(graph=g) as sess:
    print("Ranks:",r1.eval(),r2.eval(),r3.eval())
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344088469-7387ec77-c4e2-4d79-bd1a-69fae86d917c.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u6a7329fa&originHeight=63&originWidth=766&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u86a81139-636b-4bc8-b436-4c7c5a8ff9c&title=)
<a name="JJBlR"></a>
## **二、Tensorflow 计算图**
Tensorflow 的核心在于构建计算图，并用计算图推导从输入到输出的所有张量之间的关系。<br />假设有0阶张量a,b,c，要评估  ，可以表示为下图所示的计算图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344088580-9e11655a-56f3-4cf3-af1f-a14524a8d754.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u2ae6dcb3&originHeight=478&originWidth=922&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud3e4f302-fb6c-4e08-9c5a-63c1a18dd20&title=)<br />可以看到，计算图就是一个节点网络，每个节点就像是一个操作，将函数应用到输入张量，然后返回0个或者更多个张量作为张量作为输出。<br />在Tensorflow编制计算图步骤如下： 

1. 初始化一个空的计算图
2. 为该计算图加入节点（张量和操作）
3. 执行计算图：
   1. 开始一个新的会话
   2. 初始化图中的变量
   3. 运行会话中的计算图
```python
# 初始化一个空的计算图
g = tf.Graph()

# 为该计算图加入节点（张量和操作）
with g.as_default():
    a = tf.constant(1,name="a")
    b = tf.constant(2,name="b")
    c = tf.constant(3,name="c")
    
    z = 2*(a-b)+c
    
# 执行计算图
## 通过调用tf.Session产生会话对象，该调用可以接受一个图为参数（这里是g），否则将启动默认的空图
## 执行张量操作的用sess.run(),他将返回大小均匀的列表
with tf.Session(graph=g) as sess:
    print('2*(a-b)+c =>',sess.run(z))
```
2*(a-b)+c => 1
<a name="Btw0U"></a>
## **三、Tensorflow中的占位符**
Tensorflow有提供数据的特别机制。其中一种机制就是使用占位符，他们是一些预先定义好类型和形状的张量。<br />通过调用`tf.placeholder`函数把这些张量加入计算图中，而且他们不包括任何数据。然而一旦执行图中的特定节点就需要提供数据阵列。
<a name="NZuA3"></a>
### 3.1 定义占位符
```python
g = tf.Graph()

with g.as_default():
    tf_a = tf.placeholder(tf.int32,shape=(),name="tf_a")  # shape=[]就是定义0阶张量,更高阶张量可以用【n1,n2,n3】表示，如shape=（3,4,5）
    tf_b = tf.placeholder(tf.int32,shape=(),name="tf_b")
    tf_c = tf.placeholder(tf.int32,shape=(),name="tf_c")
    
    r1 = tf_a - tf_b
    r2 = 2*r1
    z = r2 + tf_c
```
<a name="dbWM3"></a>
### 3.2 为占位符提供数据
当在图中处理节点的时候，需要产生Python字典来为占位符来提供数据阵列。
```python
with tf.Session(graph=g) as sess:
    feed = {
        tf_a:1,
        tf_b:2,
        tf_c:3
    }
    
    print('z:',sess.run(z,feed_dict=feed))
```
z: 1
<a name="VV4gu"></a>
### 3.3 用batch_sizes为数据阵列定义占位符
在研发神经网络模型的时候，有时会碰到大小规模不一致的小批量数据。占位符的一个功能是把大小无法确定的维度定义为None。
```python
g = tf.Graph()

with g.as_default():
    tf_x = tf.placeholder(tf.float32,shape=(None,2),name="tf_x")
    x_mean = tf.reduce_mean(tf_x,axis=0,name="mean")
    
np.random.seed(123)
with tf.Session(graph=g) as sess:
    x1 = np.random.uniform(low=0,high=1,size=(5,2))
    print("Feeding data with shape",x1.shape)
    print("Result:",sess.run(x_mean,feed_dict={tf_x:x1}))
    
    x2 = np.random.uniform(low=0,high=1,size=(10,2))
    print("Feeding data with shape",x2.shape)
    print("Result:",sess.run(x_mean,feed_dict={tf_x:x2}))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344088646-b2452aaf-afc0-4758-9949-64090106f865.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ud77bab2d&originHeight=115&originWidth=684&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89c77bdb-f19e-444b-b73b-d0e676d565f&title=)
<a name="LSceT"></a>
## **四、Tensorflow 的变量**
就Tensorflow而言，变量是一种特殊类型的张量对象，他允许在训练模型阶段，在tensorflow会话中储存和更新模型的参数。
<a name="y7pzW"></a>
### 4.1 定义变量

- 方式1：`tf.Variable()` 是为新变量创建对象并将其添加到计算图的类。
- 方式2：`tf.get_variable()`是假设某个变量名在计算图中，可以复用给定变量名的现有值或者不存在则创建新的变量，因此变量名的name非常重要！

无论采用哪种变量定义方式，直到调用`tf.Session`启动计算图并且在会话中具体运行了初始化操作后才设置初始值。事实上，只有初始化Tensorflow的变量之后才会为计算图分配内存。
```python
g1 = tf.Graph()

with g1.as_default():
    w = tf.Variable(np.array([[1,2,3,4],[5,6,7,8]]),name="w")
    print(w)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089183-40810caa-bbe1-48f6-81cc-661214aa82c8.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u704561c0&originHeight=38&originWidth=725&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufbccebd5-0d30-4f6c-aa15-c2b48dcfbc9&title=)
<a name="XS4z8"></a>
### 4.2 初始化变量
由于变量是直到调用`tf.Session`启动计算图并且在会话中具体运行了初始化操作后才设置初始值，只有初始化Tensorflow的变量之后才会为计算图分配内存。因此这个初始化的过程十分重要，这个初始化过程包括：为 相关张量分配内存空间并为其赋予初始值。<br />初始化方式：

- 方式1：`tf.global_variables_initializer`函数，返回初始化所有计算图中现存的变量，要注意的是：定义变量一定要造初始化之前，不然会报错！！！
- 方式2：将`tf.global_variables_initializer`函数储存在init_op（名字不唯一，自己定）对象内，然后用sess.run出来
```python
with tf.Session(graph=g1) as sess:
    sess.run(tf.global_variables_initializer())
    print(sess.run(w))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089250-a9169183-6276-496b-946a-28ac081670a9.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u904cbb42&originHeight=64&originWidth=556&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34534b7c-80c4-4058-8d61-72949526bfd&title=)
```python
# 来比较定义变量与初始化顺序的关系
g2 = tf.Graph()

with g2.as_default():
    w1 = tf.Variable(1,name="w1")
    init_op = tf.global_variables_initializer()
    w2 = tf.Variable(2,name="w2")
    
with tf.Session(graph=g2) as sess:
    sess.run(init_op)
    print("w1:",sess.run(w1))
```
w1: 1
```python
with tf.Session(graph=g2) as sess:
	sess.run(init_op)
    print("w2:",sess.run(w2))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089622-5bc175b9-829d-40fa-be85-861854a0c78a.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u95adf3a4&originHeight=603&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2580e4f7-6084-48af-ac70-9075fe21438&title=)
<a name="mAQD6"></a>
### 4.3 变量范围
变量范围是一个重要的概念，对建设大型神经网络计算图特别有用。<br />可以把变量的域划分为独立的子部分。在创建变量时，该域内创建的操作与张量的名字都以域名为前缀，而且这些域可以嵌套。
```python
g = tf.Graph()

with g.as_default():
    with tf.variable_scope("net_A"):   #定义一个域net_A
        with tf.variable_scope("layer-1"): # 在域net_A下再定义一个域layer-1
            w1 = tf.Variable(tf.random_normal(shape=(10,4)),name="weights")   # 该变量定义在net_A/layer-1域下
        with tf.variable_scope("layer-2"):
            w2 = tf.Variable(tf.random_normal(shape=(20,10)),name="weights")
    with tf.variable_scope("net_B"):   # 定义一个域net_B
        with tf.variable_scope("layer-2"):
            w3 = tf.Variable(tf.random_normal(shape=(10,4)),name="weights")
    print(w1)
    print(w2)
    print(w3)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089695-0efac2ac-6677-4cb8-8a3c-34a2669f359b.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ude532af2&originHeight=85&originWidth=1044&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79aa4084-5770-4009-b2c8-c596ef1fb6f&title=)
<a name="dpETo"></a>
## **五、建立回归模型**
需要定义的变量：

1. 输入x：占位符tf_x
2. 输入y：占位符tf_y
3. 模型参数w：定义为变量weight
4. 模型参数b：定义为变量bias
5. 模型输出 ̂ y^：有操作计算得到
```python
import tensorflow  as tf
import numpy as np
import matplotlib.pyplot as plt
%matplotlib inline

g = tf.Graph()

# 定义计算图
with g.as_default():
    tf.set_random_seed(123)
    
    ## placeholder
    tf_x = tf.placeholder(shape=(None),dtype=tf.float32,name="tf_x")
    tf_y = tf.placeholder(shape=(None),dtype=tf.float32,name="tf_y")
    
    ## define the variable (model parameters)
    weight = tf.Variable(tf.random_normal(shape=(1,1),stddev=0.25),name="weight")
    bias = tf.Variable(0.0,name="bias")
    
    ## build the model
    y_hat = tf.add(weight*tf_x,bias,name="y_hat")
    
    ## compute the cost
    cost = tf.reduce_mean(tf.square(tf_y-y_hat),name="cost")
    
    ## train the model
    optim = tf.train.GradientDescentOptimizer(learning_rate=0.001)
    train_op = optim.minimize(cost,name="train_op")
```
```python
# 创建会话启动计算图并训练模型
## create a random toy dataset for regression
np.random.seed(0)
def make_random_data():
    x = np.random.uniform(low=-2,high=4,size=100)
    y = []
    for t in x:
        r = np.random.normal(loc=0.0,scale=(0.5 + t*t/3),size=None)
        y.append(r)
    return x,1.726*x-0.84+np.array(y)

x,y = make_random_data()

plt.plot(x,y,'o')
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089608-d8b37cfe-9d23-437c-878b-6cb74dfc7f4a.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u62d53533&originHeight=373&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc98e1edd-bbe4-4f8e-aa2b-7b04774a278&title=)
```python
## train/test splits
x_train,y_train = x[:100],y[:100]
x_test,y_test = x[100:],y[100:]

n_epochs = 500
train_costs  = []
with tf.Session(graph=g) as sess:
    sess.run(tf.global_variables_initializer())
    
    ## train the model for n_epochs
    for e in range(n_epochs):
        c,_ = sess.run([cost,train_op],feed_dict={tf_x:x_train,tf_y:y_train})
        train_costs.append(c)
        if not e % 50:
            print("Epoch %4d: %.4f"%(e,c))
plt.plot(train_costs)
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089955-acc9ac33-ec28-4f88-9e6e-e6f3124a8b17.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u6a5ae1bb&originHeight=578&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13dddf30-c62e-412a-8f3c-71da2a196f8&title=)
<a name="LQdp4"></a>
## 六、在Tensorflow计算图中用张量名执行对象
只需要把
```python
sess.run([cost,train_op],feed_dict={tf_x:x_train,tf_y:y_train})
```
改为
```python
sess.run(['cost:0','train_op:0'],feed_dict={'tf_x:0':x_train,'tf_y:0':y_train})
```
注意：只有张量名才有：0后缀，操作是没有：0后缀的，例如train_op并没有train_op：0
```python
## train/test splits
x_train,y_train = x[:100],y[:100]
x_test,y_test = x[100:],y[100:]

n_epochs = 500
train_costs  = []
with tf.Session(graph=g) as sess:
    sess.run(tf.global_variables_initializer())
    
    ## train the model for n_epochs
    for e in range(n_epochs):
        c,_ = sess.run(['cost:0','train_op'],feed_dict={'tf_x:0':x_train,'tf_y:0':y_train})
        train_costs.append(c)
        if not e % 50:
            print("Epoch %4d: %.4f"%(e,c))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344089873-eb41d0ef-eb8a-4765-9bbc-ccc2b8c812d6.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ub6d7a6b8&originHeight=229&originWidth=730&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc78e9729-f1ad-4189-a804-15868b6724d&title=)
<a name="QwdtI"></a>
## **七、在Tensorflow中储存和恢复模型**
神经网络训练可能需要几天几周的时间，因此需要把训练出来的模型储存下来供下次使用。<br />储存的方法是在定义计算图的时候加入：`saver = tf.train.Saver()`，并且在训练后输入`saver.save(sess,'./trained-model')`
```python
g = tf.Graph()

# 定义计算图
with g.as_default():
    tf.set_random_seed(123)
    
    ## placeholder
    tf_x = tf.placeholder(shape=(None),dtype=tf.float32,name="tf_x")
    tf_y = tf.placeholder(shape=(None),dtype=tf.float32,name="tf_y")
    
    ## define the variable (model parameters)
    weight = tf.Variable(tf.random_normal(shape=(1,1),stddev=0.25),name="weight")
    bias = tf.Variable(0.0,name="bias")
    
    ## build the model
    y_hat = tf.add(weight*tf_x,bias,name="y_hat")
    
    ## compute the cost
    cost = tf.reduce_mean(tf.square(tf_y-y_hat),name="cost")
    
    ## train the model
    optim = tf.train.GradientDescentOptimizer(learning_rate=0.001)
    train_op = optim.minimize(cost,name="train_op")
    saver = tf.train.Saver()
# 创建会话启动计算图并训练模型
## create a random toy dataset for regression
np.random.seed(0)
def make_random_data():
    x = np.random.uniform(low=-2,high=4,size=100)
    y = []
    for t in x:
        r = np.random.normal(loc=0.0,scale=(0.5 + t*t/3),size=None)
        y.append(r)
    return x,1.726*x-0.84+np.array(y)

x,y = make_random_data()

plt.plot(x,y,'o')
plt.show()

## train/test splits
x_train,y_train = x[:100],y[:100]
x_test,y_test = x[100:],y[100:]

n_epochs = 500
train_costs  = []
with tf.Session(graph=g) as sess:
    sess.run(tf.global_variables_initializer())
    
    ## train the model for n_epochs
    for e in range(n_epochs):
        c,_ = sess.run(['cost:0','train_op'],feed_dict={'tf_x:0':x_train,'tf_y:0':y_train})
        train_costs.append(c)
        if not e % 50:
            print("Epoch %4d: %.4f"%(e,c))
    saver.save(sess,'C:/Users/Leo/Desktop/trained-model/')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344090355-44d13daa-0399-4bb1-921c-f24ed1c100f3.png#clientId=ucf853bb6-6cfd-4&from=paste&id=uea299efa&originHeight=580&originWidth=820&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaadd4ed4-c93b-4b0f-8f62-77d73aae45a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654344090374-351d9e35-5595-4835-9516-542f18cef4cb.jpeg#clientId=ucf853bb6-6cfd-4&from=paste&id=ub4671851&originHeight=166&originWidth=749&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue51da9dd-059d-42bb-9e4a-a891beab49a&title=)
```python
# 加载保存的模型g2 = tf.Graph()
with tf.Session(graph=g2) as sess:
    new_saver = tf.train.import_meta_graph("C:/Users/Leo/Desktop/trained-model/.meta")
    new_saver.restore(sess,'C:/Users/Leo/Desktop/trained-model/')
    y_pred = sess.run('y_hat:0',feed_dict={'tf_x:0':x_test})
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344090384-51ade2d2-8f6a-4d09-936b-72f235b7d44c.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u2473f8a3&originHeight=36&originWidth=746&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3f8ca3f7-4dc0-418d-b71d-fec6eac95d6&title=)
```python
## 可视化模型
x_arr = np.arange(-2,4,0.1)
g2 = tf.Graph()
with tf.Session(graph=g2) as sess:
    new_saver = tf.train.import_meta_graph("C:/Users/Leo/Desktop/trained-model/.meta")
    new_saver.restore(sess,'C:/Users/Leo/Desktop/trained-model/')
    y_arr = sess.run('y_hat:0',feed_dict={'tf_x:0':x_arr})
    plt.figure()
    plt.plot(x_train,y_train,'bo')
    plt.plot(x_test,y_test,'bo',alpha=0.3)
    plt.plot(x_arr,y_arr.T[:,0],'-r',lw=3)
    plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344090416-df782291-7aa1-43d9-b648-c70839c7492a.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u07028b37&originHeight=379&originWidth=906&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uffe0975d-1e88-4e7e-b20d-a73829b5de2&title=)
<a name="kCpzI"></a>
## **八、把张量转换成多维数据阵列**
<a name="or3y4"></a>
### 8.1 获得张量的形状
在numpy中可以用`arr.shape`来获得Numpy阵列的形状，而在Tensorflow中则用`tf.get_shape`函数完成：<br />注意：在`tf.get_shape`函数的结果是不可以索引的，需要用`as.list()`换成列表才能索引。
```python
g = tf.Graph()

with g.as_default():
    arr = np.array([[1.,2.,3.,3.5],[4.,5.,6.,6.5],[7.,8.,9.,9.5]])
    T1 = tf.constant(arr,name="T1")
    print(T1)
    s = T1.get_shape()
    print("Shape of T1 is ",s)
    T2 = tf.Variable(tf.random_normal(shape=s))
    print(T2)
    T3 = tf.Variable(tf.random_normal(shape=(s.as_list()[0],)))
    print(T3)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344090635-bf2f9618-1ea3-4ed3-a110-90eaf3d6320f.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ua3980474&originHeight=102&originWidth=837&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uba90f4e4-6ae4-4cd7-ac7d-a19ff7d336b&title=)
<a name="PdyX0"></a>
### 8.2 改变张量的形状
现在来看看Tensorflow如何改变张量的形状，在Numpy可以用`np.reshape`或`arr.reshape`，在一维的时候可以用-1来自动计算最后的维度。在Tensorflow内调用`tf.reshape`
```python
with g.as_default():
    T4 = tf.reshape(T1,shape=[1,1,-1],name="T4")
    print(T4)
    T5 = tf.reshape(T1,shape=[1,3,-1],name="T5")
    print(T5)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344090849-f11f3b58-5d38-4626-bd8d-18d15200f0af.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u5f475c5b&originHeight=60&originWidth=707&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud53e9df3-8e65-4123-8978-963223352a9&title=)
```python
with tf.Session(graph=g) as sess:
    print(sess.run(T4))
    print()
    print(sess.run(T5))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091053-9274cc77-b5c0-4479-82dc-c375195298c5.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u8d0c5f9f&originHeight=131&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub63c5c5e-7a6d-4940-bb38-98d8f4a349c&title=)
<a name="NyDP3"></a>
### 8.3 将张量分裂为张量列表
```python
with g.as_default():
    tf_splt = tf.split(T5,num_or_size_splits=2,axis=2,name="T8")
    print(tf_splt)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091119-37bfd277-90bc-4edf-b1e6-202d88739371.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u269ab3fb&originHeight=31&originWidth=927&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81f416b0-fdf5-42fc-9548-4a9b26a791e&title=)
<a name="c9Yek"></a>
### 8.4 张量的拼接
```python
g = tf.Graph()

with g.as_default():
    t1 = tf.ones(shape=(5,1),dtype=tf.float32,name="t1")
    t2 = tf.zeros(shape=(5,1),dtype=tf.float32,name="t2")
    print(t1)
    print(t2)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091064-7adde4d7-6a74-42e5-b6cd-eeae74f43f99.png#clientId=ucf853bb6-6cfd-4&from=paste&id=udfde7816&originHeight=63&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue7ee33cf-6d02-49f0-ab39-14770cdc66f&title=)
```python
with g.as_default():
    t3 = tf.concat([t1,t2],axis=0,name="t3")
    print(t3)
    t4 = tf.concat([t1,t2],axis=1,name="t4")
    print(t4)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091121-113c7440-27bd-4a11-8759-d759abcd033d.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u00f29159&originHeight=58&originWidth=684&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd508694-7e52-4568-af6f-33c374a9a9d&title=)
```python
with tf.Session(graph=g) as sess:
    print(t3.eval())
    print()
    print(t4.eval())
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091217-c1b627b0-2cc0-4959-9f19-893dc1e2cee8.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ub0119f5b&originHeight=363&originWidth=689&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f3f60b6-7537-4ae3-9d14-f8cd8a4487b&title=)
```python
with tf.Session(graph=g) as sess:    print(sess.run(t3))
    print()
    print(sess.run(t4))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091345-37b197d5-8a61-45e5-85d2-ab7b1b86a65f.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u1442425e&originHeight=361&originWidth=695&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubd9903dc-c91e-4ee7-93fd-949f1946aac&title=)
<a name="usZ4r"></a>
## 九、利用控制流构图
这里主要讨论在Tensorflow执行像Python一样的if语句，循环while语句，if...else..语句等。
<a name="QobXY"></a>
### 9.1 条件语句
`tf.cond()`语句来试试：<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1654344091446-8b6251df-7915-4d82-8031-6ea538497cad.svg#clientId=ucf853bb6-6cfd-4&from=paste&id=uf03abff6&originHeight=43&originWidth=191&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa5b2e0d-c3cb-4d5c-ab96-5077dc5d592&title=)
```python
x,y = 1.0,2.0

g = tf.Graph()

with g.as_default():
    tf_x = tf.placeholder(dtype=tf.float32,shape=None,name="tf_x")
    tf_y = tf.placeholder(dtype=tf.float32,shape=None,name="tf_y")
    res = tf.cond(tf_x<tf_y,lambda: tf.add(tf_x,tf_y,name="result_add"),lambda: tf.subtract(tf_x,tf_y,name="result_sub"))
    print("Object:",res)  #对象被命名为"cond/Merge:0"
    
with tf.Session(graph=g) as sess:
    print("x<y: %s -> Result:"%(x<y),res.eval(feed_dict={"tf_x:0":x,"tf_y:0":y}))
    x,y = 2.0,1.0
    print("x<y: %s -> Result:"%(x<y),res.eval(feed_dict={"tf_x:0":x,"tf_y:0":y}))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091584-105b9e9c-cebd-4fee-96bc-2c485535ea27.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u58ef3b1d&originHeight=81&originWidth=778&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucda69e2c-db4e-47fb-9ff2-6db73daaa5f&title=)
<a name="OyOkN"></a>
### 9.2 执行Python的if...else语句
`tf.case()`
```python
f1 = lambda: tf.constant(1)
f2 = lambda: tf.constant(0)
result = tf.case([(tf.less(x,y),f1)],default=f2)
print(result)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091545-58f621de-64e2-4086-bdf5-a31e2f68f4b7.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u962da0ea&originHeight=36&originWidth=682&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1326de25-a4c6-439b-8346-73de70afb59&title=)
<a name="MJmkB"></a>
### 9.3 执行Python的while语句
```python
i = tf.constant(0)
threshold = 100
c = lambda i: tf.less(i,100)
b = lambda i: tf.add(i,1)
r = tf.while_loop(cond=c,body=b,loop_vars=[i])
print(r)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091559-72d1fcdc-25c9-4f38-a3da-0c820cfbd255.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u7faf93a4&originHeight=36&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0549499e-0ce3-46a8-8d52-2a2551cbad5&title=)
<a name="QdEuq"></a>
## **十、用TensorBoard可视化图**
TensorBoard是Tensorflow一个非常好的工具，它负责可视化和模型学习。可视化允许看到节点之间的连接，探索它们之间的依赖关系，并且在需要的时候进行模型调试。
```python

def build_classifier(data, labels, n_classes=2):
    data_shape = data.get_shape().as_list()
    weights = tf.get_variable(name='weights',
                              shape=(data_shape[1], n_classes),
                              dtype=tf.float32)
    bias = tf.get_variable(name='bias',
                           initializer=tf.zeros(shape=n_classes))
    print(weights)
    print(bias)
    logits = tf.add(tf.matmul(data, weights),
                    bias,
                    name='logits')
    print(logits)
    return logits, tf.nn.softmax(logits)

def build_generator(data, n_hidden):
    data_shape = data.get_shape().as_list()
    w1 = tf.Variable(
        tf.random_normal(shape=(data_shape[1],
                                n_hidden)),
        name='w1')
    b1 = tf.Variable(tf.zeros(shape=n_hidden),
                     name='b1')
    hidden = tf.add(tf.matmul(data, w1), b1,
                    name='hidden_pre-activation')
    hidden = tf.nn.relu(hidden, 'hidden_activation')
        
    w2 = tf.Variable(
        tf.random_normal(shape=(n_hidden,
                                data_shape[1])),
        name='w2')
    b2 = tf.Variable(tf.zeros(shape=data_shape[1]),
                     name='b2')
    output = tf.add(tf.matmul(hidden, w2), b2,
                    name = 'output')
    return output, tf.nn.sigmoid(output)

batch_size=64
g = tf.Graph()

with g.as_default():
    tf_X = tf.placeholder(shape=(batch_size, 100),
                          dtype=tf.float32,
                          name='tf_X')
    ## build the generator
    with tf.variable_scope('generator'):
        gen_out1 = build_generator(data=tf_X,
                                   n_hidden=50)
    
    ## build the classifier
    with tf.variable_scope('classifier') as scope:
        ## classifier for the original data:
        cls_out1 = build_classifier(data=tf_X,
                                    labels=tf.ones(
                                        shape=batch_size))
        
        ## reuse the classifier for generated data
        scope.reuse_variables()
        cls_out2 = build_classifier(data=gen_out1[1],
                                    labels=tf.zeros(
                                        shape=batch_size))
        
        init_op = tf.global_variables_initializer()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344091763-6428b35b-fbaa-4581-9bc5-66488bfc155d.png#clientId=ucf853bb6-6cfd-4&from=paste&id=u87f61e02&originHeight=143&originWidth=802&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u08f4e302-1a04-4842-9991-f689e0bbe51&title=)
```python
with tf.Session(graph=g) as sess:
	sess.run(tf.global_variables_initializer())
    file_writer = tf.summary.FileWriter(logdir="C:/Users/Leo/Desktop/trained-model/logs/",graph=g)
```
在win+R输入cmd后输入命令：
```bash
tensorboard --logdir="C:/Users/Leo/Desktop/trained-model/logs"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344092050-5ccad7ee-9083-4fc3-8c61-bd1393cc506f.png#clientId=ucf853bb6-6cfd-4&from=paste&id=ud345c51d&originHeight=564&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1e9a8761-55b2-4160-a045-bf2f88f77ff&title=)<br />接着复制这个链接到浏览器打开：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654344092074-eecc67c1-4caf-4624-9681-a05fde585270.png#clientId=ucf853bb6-6cfd-4&from=paste&id=uea5fd93f&originHeight=507&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61490c18-f621-4635-a3ff-7c2ce5e4688&title=)
