PyTorch<br />复现一篇论文的训练代码时，发现原论文中的总loss由多个loss组成。如果只有一个loss，那么直接`loss.backward()`即可，但是这里不止一个。一开始看到不止一个loss时，不知道将`backward()`放在哪里。
```python
for j in range(len(output)):
    loss += criterion(output[j], target_var)
```
一般传统的**梯度回传步骤**是这样的：
```python
outputs = model(images)
loss = criterion(outputs,target)

optimizer.zero_grad()  
loss.backward()
optimizer.step()
```

- 首先模型会通过输入的图像与标签计算相应的损失函数；
- 然后清除之前过往的梯度`optimizer.zero_grad()`；
- 进行梯度的回传，并计算当前的梯度`loss.backward()`反向传播，计算当前梯度；
- 根据当前的梯度来更新网络参数。一般来说是进来一个`batch`的数据就会计算一次梯度，然后更新网络`optimizer.step()`

而现在需要在一个for循环中计算loss，于是就在想是否需要在for循环中进行`backward()`的计算呢？
```python
for j in range(len(output)):
    loss += criterion(output[j], target_var)
    loss.backward()
```
但是当计算完一个loss之后就使用`backward`方法，发现报错：**Pytorch - RuntimeError: Trying to backward through the graph a second time, but the buffers have already been freed. Specify retain_graph=True when calling backward the first time.**<br />原因是在Pytorch中，**一张计算图只允许存在一次损失的回传计算**，当每次进行梯度回传之后，中间的变量都会被释放掉。所以如果想在本次`batch`中再次计算图的梯度时，程序会发现中间的计算图已经没了，那么自然而然也就没有办法计算梯度。<br />网上看到有个解决办法是在`backward`中加入`retain_grad=True`，也就是`backward(retain_graph=True)`。<br />这句话的意思是**暂时不释放计算图**，所以在后续的训练过程中计算图不会被释放掉，而是会一直累积，但是随着训练的进行，会出现OOM。因此，需要**在最后一个**loss计算时，把(`retain_graph=True`)去掉，也就是只使用`backward()`，也就是除了最终的loss要释放资源、计算梯度，前面若干个的loss都不进行此步骤。
```python
for j in range(len(output)):
    loss += criterion(output[j], target_var)
loss.backward()
```
也许有同学会问，为什么不这么写呢？之前也是这样的，可是发现loss并没有降低，于是就开始从loss里找原因了，但是为什么不降低，也没有理解明白，希望有明白的同学可以交流下~<br />其实当遇到这种情况，最好的办法就是分开写，然后再汇总到一个总loss中计算`backward`计算。如：
```python
loss1= Loss(output[0], target)
loss2= Loss(output[1], target)
loss3= Loss(output[2], target)
loss4= Loss(output[3], target)

loss = loss1 + loss2 + loss3 + loss4
loss.backward()
```
当这么写的时候，loss就正常下降了。看到loss下降得还算是正常时，就稍微放心了。
<a name="hfGGz"></a>
## 发生错误的其他可能原因
在查询资料的时候，发现即使只计算一个loss，也可能会出现错误。

- 有可能计算的设备一个在cpu上，一个在gpu，所以将设备设置为同一个即可。
- 也有可能在多次循环中，有一些输入是不需要计算梯度的，这个时候就可以将输入的`require_grad`设置为False。
- 关于张量`tensor`中的`require_grad`属性：如果一个张量它的`requires_grad=True`，那么在反向传播计算梯度时调用`backward()`方法就会计算这个张量的梯度。但是需要注意的是：计算完梯度之后，这个梯度并不一定会一直保存在属性grad中，只有对于`requires_grad=True`的叶子结点才会一直保存梯度，即将梯度一直保存在该叶子张量的grad属性中。而对于非叶子节点，即中间节点的张量,我们在计算完梯度之后为了更高效地利用内存，一般会将中间计算的梯度释放掉。
- 在使用LSTM、GRU这一些网络时，我想是因为它们不仅会从前往后计算梯度，也会从后往前计算梯度，所以可以看做是梯度在两个方向上进行传播，那么这个过程中就会有重叠的部分。因此可能就需要使用`detach`来进行截断。在源码中，`detach`的注释是：**Returns a new Variable, detached from the current graph**。是将某个结点变成不需要梯度的变量，将其从当前的计算图剥离出来。因此当反向传播经过这个结点时，梯度就不会从这个结点往前面传播。
<a name="EdV8T"></a>
### `detach()`与`detach_()`
Pytorch里面多任务Loss是加起来还是分别backward？中有两个函数`detach()`、`detach_()`，它们两个名字、功能都很像，都是用于切断梯度的反向传播。那么什么时候会用到呢？<br />当在训练网络的时候可能希望保持一部分的网络参数不变，而只对网络的一部分参数进行调整；或者只训练网络的部分分支网络，并且不想让其梯度对主网络的梯度造成影响。这时候我们就可以使用这两个函数来进行截断梯度的反向传播。<br />二者的区别就是`detach_()`是**对本身进行更改**，而`detach()`则是**生成了一个新的**`tensor`。<br />使用`detach()`会返回一个新的Variable。虽然它是从当前计算图中分离下来的，但是仍指向原变量的存放位置，也就是**共享同一个内存区域**。使用了`detach`后，它的`requires_grad`属性为False，也就是不需要再计算它的梯度。即使之后重新将它的`requires_grad`变为True，它也不会具有梯度`grad`。这样我们就会继续使用这个新的变量进行计算，后面当我们进行反向传播时，梯度会一直计算直到到达这个调用了`detach()`的结点，到达这个结点后就会停止，不会再继续向前进行传播。<br />但是返回的变量和原始的结点是共用同一个内存区域，所以如果使用了`detach`后，又对其进行修改，那么进行调用`backward()`时，就**可能会导致错误**。<br />使用`tensor.detach_()`则会将一个`tensor`从创建它的图中分离，并把它设置成叶子结点。举个例子：<br />假设一开始的变量关系为：x ->m -> y，那么这里的叶子结点就是x，当这个时候对m进行了`m.detach_()`操作，首先会取消m与前一个结点x的关联，并且`grad_fn`为None。此时，这里的关系就会变成x，m ->y，这个时候m就变成了叶子结点。然后再将m的`requires_grad`属性设置为False，当我们对y进行`backward()`时就不会求m的梯度。
<a name="Ogy8a"></a>
## 如何编写更能节省内存的backward
说到梯度回传，在网上也看到有人的写法是这样的，目的是为了节省内存：
```python
for i, (images, target) in enumerate(train_loader):
    images = images.cuda(non_blocking=True)
    target = torch.from_numpy(np.array(target)).float().cuda(non_blocking=True)
    outputs = model(images)
    loss = criterion(outputs, target)
    loss = loss / accumulation_steps   

    loss.backward()

    if (i + 1) % accumulation_steps == 0:
        optimizer.step()       
        optimizer.zero_grad()
```

- 首先进行正向传播，将数据传入网络进行推理，得到结果
- 将预测结果与label输入进损失函数中计算损失
- 进行反向传播，计算梯度
- 重复前面的步骤，先不清空梯度，而是先将梯度进行累加，当梯度累加达到固定次数之后就更新网络参数，然后将梯度置零

梯度累加就是每次获取1个batch的数据，计算1次梯度，但是先不进行清零，而是做梯度的累加，不断地进行累加，当累加到一定的次数之后，再更新网络参数，然后将梯度清零，进行下一个循环。<br />通过这种**参数延迟更新**的手段，**可以实现与采用大**batch size**相近的效果**。在平时的实验过程中，我一般会采用梯度累加技术，大多数情况下，采用梯度累加训练的模型效果，要比采用小batch size训练的模型效果要好很多。<br />一定条件下，batch size越大训练效果越好，**梯度累加**则实现了batch size的变相扩大，如果accumulation_steps为8，则batch size就**变相扩大了8倍**，使用时需要注意，**学习率也要适当放大**：因为使用的**样本增多**，**梯度更加稳定**了。<br />有人会问，在上面的代码中为什么不直接对多个batch的loss先求和然后再取平均、再进行梯度回传和更新呢？<br />按我的理解这是为了减小内存的消耗。当采用多个batch的loss求和平再均后再回传的方式时，我们会进行`accumulation_steps `次batch的前向计算，而前向计算后都会生成一个计算图。也就是说，在这种方式下，会生成`accumulation_steps`个计算图再进行backward计算。<br />而采用上述代码的方式时，当每次的batch前向计算结束后，就会进行`backward`的计算，计算结束后也就释放了计算图。又因为这两者计算过程的梯度都是累加的，所以计算结果都是相同的，但是上述的方法在每一时刻中，最多只会生成一张计算图，所以也就减小了计算中的内存消耗。
<a name="lYGZy"></a>
## 结语
其实通过这次探讨，只能说是了解地稍微深一些了，但是其中的原理还是不太明白。比如autograd的跟踪、in-place operations的属性，什么时候`requires_grad`为True，什么时候又为False，什么时候梯度会进行覆盖等等，这一些还是一头雾水。特别是上面那种写法，搞不明白loss为什么就突然下降了，所以还是得多学多用才能记住，才能深刻理解。<br />很多文章提到：其实大部分的写法都十分高效了，所以除非处于非常沉重的内存压力下，否则一般不会用到太多骚操作。
