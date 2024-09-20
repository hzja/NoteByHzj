PythonDeque<br />**如果经常在 Python 中使用列表，会发现，当需要在列表左端弹出和追加元素时，其性能可能不够快。Python 的 collections 模块提供了一个叫做 **[**deque**](https://docs.python.org/3/library/collections.html#collections.deque)** 的类，专门设计用来提供快速和节省内存的方法，以从底层数据结构的两端追加和弹出元素。**<br />Python 中的 deque **是一个低级别的、高度优化的双端队列**，对于实现优雅、高效的Pythonic 队列和堆栈很有用，它们是计算中最常见的列表式数据类型。<br />**一起学习如下内容：**

- 开始使用deque
- 有效地弹出和追加元素
- 访问deque中的任意元素
- 用deque构建高效队列
<a name="x3n7X"></a>
## 开始使用Deque
向 Python 列表的右端追加元素和弹出元素的操作，一般非常高效。如果用大 O 表示时间复杂性，那么可以说它们是 _O_(1)。而当 Python 需要重新分配内存来增加底层列表以接受新的元素时，这些操作就会变慢，时间复杂度可能变成 _O_(n)。<br />此外，在 Python 列表的左端追加和弹出元素的操作，也是非常低效的，时间复杂度为_O_(n)。<br />由于列表提供了 `.append()` 和 `.pop()` 这两种操作，它们可以作为堆栈和队列使用。而列表的左右端追加和弹出操作的性能问题会大大影响应用程序的整体性能。<br />Python 的 deque 是早在 Python 2.4 中添加到 collections 模块的第一个数据类型。这个数据类型是专门为克服 Python list 中的 `.append()`和 `.pop()` 的效率问题而设计的。<br />Deques是类似于序列的数据类型，被设计为**堆栈**和**队列**的一般化，它们在数据结构的两端支持高效的内存和快速的追加和弹出操作。<br />在 deque 对象两端的追加和弹出操作是稳定的，而且同样有效，因为 deque 是作为双链接列表实现。此外，deque 上的追加和弹出操作也是线程安全的和内存高效的。这些特性使得 deque 在Python中创建自定义栈和队列时特别有用。<br />如果需要保存最后看到的元素列表，也可以使用 deque，因为可以限制 deque 的最大长度。如果这样做了，那么一旦 deque 满了，当在另一端添加新的元素时，它会自动丢弃一端的元素。<br />**下面是 deque 的主要特点的总结：**

- **存储任何数据类型的元素**
- **是可变数据类型**
- **支持带in操作符的成员操作**
- **支持索引，比如**`**a_deque[i]**`
- **不支持切片，比如 **`**a_deque[0:2]**`
- **支持对序列和可迭代对象进行操作的内置函数，如 **`**len()**`** ，**`**sorted()**`** ，**`**reversed()**`** 等**
- **不支持 **`**inplace**`** 排序**
- **支持正常迭代和反向迭代**
- **支持使用 **`**pickle**`
- **确保在两端快速、内存高效和线程安全的弹出和追加操作**

创建 `deque` 实例比较简单。只需要从 collection 中导入 `deque`，然后用一个可选的迭代器作为参数来调用它。
```python
>>> from collections import deque

>>> # 创建一个空的 deque
>>> deque()
deque([])

>>> # 使用不同的迭代器来创建 deque
>>> deque((1, 2, 3, 4))
deque([1, 2, 3, 4])

>>> deque([1, 2, 3, 4])
deque([1, 2, 3, 4])

>>> deque(range(1, 5))
deque([1, 2, 3, 4])

>>> deque("abcd")
deque(['a', 'b', 'c', 'd'])

>>> numbers = {"one": 1, "two": 2, "three": 3, "four": 4}
>>> deque(numbers.keys())
deque(['one', 'two', 'three', 'four'])

>>> deque(numbers.values())
deque([1, 2, 3, 4])

>>> deque(numbers.items())
deque([('one', 1), ('two', 2), ('three', 3), ('four', 4)])
```
如果实例化 deque 时没有提供 iterable 作为参数，那么会得到一个空的 deque。如果提供并输入 iterable ，那么 deque 会用它的数据初始化新实例。初始化使用 `deque.append()` 从左到右进行。<br />`Deque` 初始化器需要以下两个可选参数。

1. **iterable 一个提供初始化数据的迭代器。**
2. **maxlen 一个整数，指定deque的最大长度。**

如前所述，如果不提供一个 iterable ，那么会得到一个空的 deque。如果给 maxlen 提供一个值，那么 deque 只会存储最多的 maxlen 项。<br />最后，还可以使用无序的可迭代对象，如 collections 来初始化 deque。在这些情况下，不会有最终 deque 中元素的预定义顺序。
<a name="zlLha"></a>
## 有效地弹出和追加元素
Deque 和 List 之间最重要的区别是，前者可以在序列的两端进行有效的追加和弹出操作。**Deque 类实现了专门的 **`**.popleft()**`** 和 **`**.appendleft()**`** 方法，直接对序列的左端进行操作。**
```python
>>> from collections import deque

>>> numbers = deque([1, 2, 3, 4])
>>> numbers.popleft()
1
>>> numbers.popleft()
2
>>> numbers
deque([3, 4])

>>> numbers.appendleft(2)
>>> numbers.appendleft(1)
>>> numbers
deque([1, 2, 3, 4])
```
在这里，使用 `.popleft()` 和 `.appendleft()` 来分别弹出和增加 numbers的左端值。这些方法是针对deque的设计的，而 list 没有这样的方法。<br />Deque也提供了像list一样的 `.append()` 和 `.pop()` 方法来对序列的右端进行操作。然而，`.pop()` 的行为是不同的。
```python
>>> from collections import deque

>>> numbers = deque([1, 2, 3, 4])
>>> numbers.pop()
4

>>> numbers.pop(0)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: pop() takes no arguments (1 given)
```
在这里，`.pop()` 删除并返回 deque 容器中的最后一个元素。该方法不接受索引作为参数，因此不能使用它从 deque 中删除任意项。只能使用它来删除并返回最右边的项。<br />可以认为 deque 是一个**双链表**。因此，给定 deque 容器中的每一项都保存着序列中上下一项的引用(指针)。<br />双链表使得从两端添加和弹出元素的操作变得简单而高效，因为只有指针需要更新，因此，这两个操作具有相似的性能，均为_O_(1)。它们在性能方面也是可预测的，因为不需要重新分配内存和移动现有项来接受新项。<br />从常规 Python 列表的左端追加和弹出元素需要移动所有元素，这最终是一个 _O_(n) 操作。此外，将元素添加到列表的右端通常需要Python重新分配内存，并将当前项复制到新的内存位置，之后，它可以添加新项。这个过程需要更长的时间来完成，并且追加操作从 _O_(1)传递到 _O_(n)。<br />考虑以下关于在序列左端添加项的性能测试，deque vs list。
```python
# time_append.py

from collections import deque
from time import perf_counter

TIMES = 10_000
a_list = []
a_deque = deque()

def average_time(func, times):
    total = 0.0
    for i in range(times):
        start = perf_counter()
        func(i)
        total += (perf_counter() - start) * 1e9
    return total / times

list_time = average_time(lambda i: a_list.insert(0, i), TIMES)
deque_time = average_time(lambda i: a_deque.appendleft(i), TIMES)
gain = list_time / deque_time

print(f"list.insert()      {list_time:.6} ns")
print(f"deque.appendleft() {deque_time:.6} ns  ({gain:.6}x faster)")
```
在这个脚本中，`average_time()` 计算了执行一个给定次数的函数（func）的平均时间。如果在命令行中运行该脚本，那么会得到以下输出。
```bash
$ python time_append.py
list.insert()      3735.08 ns
deque.appendleft() 238.889 ns  (15.6352x faster)
```
在这个例子中，deque 上的 `.appendleft()` 要比 list  上的 `.insert()` 快几倍。注意 `deque.appendleft()` 执行时间是常量_O_(1)。但列表左端的 `list.insert()` 执行时间取决于要处理的项的数量_O_(n)。<br />在这个例子中，如果增加 TIMES 的值，那么 `list.insert()` 会有更高的时间测量值，而 `deque.appendleft()` 会得到稳定(常数)的结果。如果对 deque 和 list 的 pop 操作进行类似的性能测试，那么可以展开下面的练习块。<br />**Exercise**：测试 `deque.popleft()` 与 `list.pop(0)` 的性能<br />可以将上面的脚本修改为时间`deque.popleft()`与`list.pop(0)`操作并估计它们的性能。<br />**Solution**：测试 `deque.popleft()` 与 `list.pop(0)` 的性能
```python
# time_pop.py

from collections import deque
from time import perf_counter

TIMES = 10_000
a_list = [1] * TIMES
a_deque = deque(a_list)

def average_time(func, times):
    total = 0.0
    for _ in range(times):
        start = perf_counter()
        func()
        total += (perf_counter() - start) * 1e9
    return total / times

list_time = average_time(lambda: a_list.pop(0), TIMES)
deque_time = average_time(lambda: a_deque.popleft(), TIMES)
gain = list_time / deque_time

print(f"list.pop(0)     {list_time:.6} ns")
print(f"deque.popleft() {deque_time:.6} ns  ({gain:.6}x faster)")

```
```bash
list.pop(0)     2002.08 ns
deque.popleft() 326.454 ns  (6.13282x faster)
```
同样，它deque比list从底层序列的左端删除元素要快。<br />尝试更改TIMES的值，看看会发生什么<br />Deque 数据类型的设计是为了保证在序列的两端进行有效的追加和弹出操作。它是处理需要在 Python 中实现队列和堆栈数据结构的问题的理想选择。
<a name="Rc2F7"></a>
## 访问Deque中的任意元素
Python 的 deque 返回可变的序列，其工作方式与列表相当类似。除了可以有效地从其末端追加和弹出元素外，deque 还提供了一组类似列表的方法和其他类似序列的操作，以处理任意位置的元素。下面是其中的一些。

| **选项** | **描述** |
| --- | --- |
| .insert(i, value) | 在索引为i的deque容器中插入一个名为value的元素。 |
| .remove (value) | 删除第一个出现的 value ，如果 value 不存在则引发ValueError |
| a_deque[i] | 从一个deque容器中检索索引为 i 的项。 |
| del a_deque[i] | 从deque容器中移除索引为 i 的项。 |

可以使用这些方法和技术来处理 deque 对象内部任何位置的元素。下面是如何做到这一点的。
```python
>>> from collections import deque
>>> letters = deque("abde")
>>> letters.insert(2, "c")
>>> letters
deque(['a', 'b', 'c', 'd', 'e'])

>>> letters.remove("d")
>>> letters
deque(['a', 'b', 'c', 'e'])

>>> letters[1]
'b'

>>> del letters[2]
>>> letters
deque(['a', 'b', 'e'])
```
在这里，首先将"c"插入到位置 2的letters中。然后使用 `.remove()` 从deque容器中移除"d"。Deque 还允许**索引**来访问元素，在这里使用它来访问索引1处的b。最后，可以使用 `del` 关键字从 deque 中删除任何存在的项。请注意， `.remove()` 允许按值删除项，而`del`则按索引删除项。<br />尽管 deque 对象支持索引，但它们不支持**切片**，即不能像常规列表一样使用切片语法， `[start:stop:step]` 从现有的 deque 中提取：
```python
>>> from collections import deque
>>> numbers = deque([1, 2, 3, 4, 5])
>>> numbers[1:3]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: sequence index must be integer, not 'slice'
```
Deque支持索引，却不支持分片。通常来说在一个链表上执行切片非常低效。<br />虽然 deque 与 list 非常相似，但 list 是基于数组的，而 deque 是基于双链表的。<br />Deque 基于双链表，在访问、插入和删除任意元素都是无效操作。如果需要执行这些操作，则解释器必须在deque中进行迭代，直到找到想要的元素。因而他们的时间复杂度是_O_(n)而不是_O_(1)。<br />下面演示了在处理任意元素时 deques 和 list 的行为。
```python
# time_random_access.py

from collections import deque
from time import perf_counter

TIMES = 10_000
a_list = [1] * TIMES
a_deque = deque(a_list)

def average_time(func, times):
    total = 0.0
    for _ in range(times):
        start = perf_counter()
        func()
        total += (perf_counter() - start) * 1e6
    return total / times

def time_it(sequence):
    middle = len(sequence) // 2
    sequence.insert(middle, "middle")
    sequence[middle]
    sequence.remove("middle")
    del sequence[middle]

list_time = average_time(lambda: time_it(a_list), TIMES)
deque_time = average_time(lambda: time_it(a_deque), TIMES)
gain = deque_time / list_time

print(f"list  {list_time:.6} μs ({gain:.6}x faster)")
print(f"deque {deque_time:.6} μs")
```
这个脚本对插入、删除和访问一个 deque 和一个 list 中间的元素进行计时。如果运行这个脚本，得到如下所示的输出:
```bash
$ python time_random_access.py
list  63.8658 μs (1.44517x faster)
deque 92.2968 μs
```
Deque并不像列表那样是随机访问的数据结构。因此，从 deque 的中间访问元素的效率要比在列表上做同样的事情低。这说明 deque 并不总是比列表更有效率。<br />Python 的 deque 对序列两端的操作进行了优化，所以它们在这方面一直比 list 好。另一方面，列表更适合于随机访问和固定长度的操作。下面是 deque 和 list 在性能上的一些区别。

| **运作** | **deque** | **list** |
| --- | --- | --- |
| 通过索引访问任意的元素 | _O_(n) | _O_(1) |
| 在左端弹出和追加元素 | _O_(1) | _O_(n) |
| 在右端弹出和追加元素 | _O_(1) | _O_(1) + 重新分配 |
| 在中间插入和删除元素 | _O_(n) | _O_(n) |

对于列表，当解释器需要扩大列表以接受新项时，.append()的性能优势受到内存重新分配的影响而被降低。此操作需要将所有当前项复制到新的内存位置，这将极大地影响性能。<br />此总结可以帮助我们为手头的问题选择适当的数据类型。但是，在从列表切换到 deque 之前，一定要对代码进行剖析，它们都有各自的性能优势。
<a name="aQS6d"></a>
## 用Deque构建高效队列
Deque 是一个双端队列，提供了**堆栈**和**队列**的泛化。在本节中，将一起学习如何使用deque以优雅、高效和Pythonic的方式在底层实现自己的队列抽象数据类型（ADT）。<br />**注意：** 在 Python 标准库中，queue 模块实现了多生产者、多消费者的队列，可以在多个线程之间安全地交换信息。<br />如果正在处理队列，那么最好使用那些高级抽象而不是 deque ，除非正在实现自己的数据结构。<br />队列是元素的collections，可以通过在一端添加元素和从另一端删除元素来修改队列。<br />队列 **以先入先出**（FIFO）的方式管理元素，像一个管道一样工作，在管道的一端推入新元素，并从另一端弹出旧元素。向队列的一端添加一个元素称为 **enqueue** 操作；从另一端删除一个元素称为 **dequeue**。<br />为了更好地理解队列，以餐厅为例，餐馆里有很多人在排队等着点餐。通常情况下，后来的人将排在队列的末端。一旦有了空桌子，排在队伍开头的人就会离开队伍进去用餐。<br />下面演示了使用一个原始的deque对象来模拟这个过程。
```python
>>> from collections import deque

>>> customers = deque()

>>> # People arriving
>>> customers.append("Jane")
>>> customers.append("John")
>>> customers.append("Linda")

>>> customers
deque(['Jane', 'John', 'Linda'])

>>> # People getting tables
>>> customers.popleft()
'Jane'
>>> customers.popleft()
'John'
>>> customers.popleft()
'Linda'

>>> # No people in the queue
>>> customers.popleft()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: pop from an empty deque

```
首先创建一个空的 deque 对象来表示到达餐厅的人的队列。person排队放入队列，可以使用`.append()`，将单个条目添加到右端。要从队列中取出一个person，可以使用`.popleft()` ，删除并返回deque容器左侧的各个条目。<br />用队列模拟工作，然而，由于deque是一个泛化，它的API]不匹配常规的队列API。例如，不是`.enqueue()`，而是`.append()`。还有`.popleft()` 而不是`.dequeue()`。此外，deque 还提供了其他一些可能不符合特定需求的操作。<br />可以创建具有特定功能的自定义队列类。可以在内部使用 deque 来存储数据，并在自定义队列中提供所需的功能。可以把它看作是适配器设计模式的一个实现，在这个模式中，把 deque 的接口转换成看起来更像队列接口的东西。<br />例如，需要一个自定义的队列抽象数据类型，提供以下功能。

- **排列元素**
- **去排队的元素**
- **返回队列的长度**
- **支持成员资格测试**
- **支持正常和反向迭代**
- **提供一个方便用户的字符串表示法**

此时可以写一个Queue类。
```python
# custom_queue.py

from collections import deque

class Queue:
    def __init__(self):
        self._items = deque()

    def enqueue(self, item):
        self._items.append(item)

    def dequeue(self):
        try:
            return self._items.popleft()
        except IndexError:
            raise IndexError("dequeue from an empty queue") from None

    def __len__(self):
        return len(self._items)

    def __contains__(self, item):
        return item in self._items

    def __iter__(self):
        yield from self._items

    def __reversed__(self):
        yield from reversed(self._items)

    def __repr__(self):
        return f"Queue({list(self._items)})"

```
`._items` 是一个 deque 对象，可以存储和操作队列中的元素。Queue使用 `deque.append()`  实现了 `.enqueue()`，将元素添加到队列的末端。还用 `deque.popleft()` 实现了 `.dequeue()`，以有效地从队列的开头删除元素。<br />支持以下特殊方法

| **Method** | **Support** |
| --- | --- |
| .__len__ () | 长度的 len() |
| .__contains__() | 带有in的成员测试 |
| .__iter__() | 常规迭代 |
| .__reversed__() | 反向迭代 |
| .__repr__() | 字符串表示形式 |

理想情况下，`.__repr__()`返回一个字符串，代表一个有效的 Python 表达式。可以用这个表达式以相同的值重新创建这个对象。<br />然而，在上面的例子中，目的是使用方法的返回值在 interactive shell 上优雅地显示对象。可以通过接受初始化可迭代对象作为`.__init__()` 的参数并从中构建实例，从而从这个特定的字符串表示形式构建 Queue 实例。<br />有了这些补充，Queue 类就完成了。要在代码中使用这个类，可以做如下事情。
```python
>>> from custom_queue import Queue
>>> numbers = Queue()
>>> numbers
Queue([])

>>> # Enqueue items
>>> for number in range(1, 5):
...     numbers.enqueue(number)
...
>>> numbers
Queue([1, 2, 3, 4])

>>> # Support len()
>>> len(numbers)
4

>>> # Support membership tests
>>> 2 in numbers
True
>>> 10 in numbers
False

>>> # Normal iteration
>>> for number in numbers:
...     print(f"Number: {number}")
...
1
2
3
4
```
<a name="kHIwO"></a>
## 探索Deque的其他功能
deque 还提供了其他的方法和属性，这些都是其内部设计所特有的。它们为这个多功能的数据类型增加了新的和有用的功能。<br />一起学习 deques 提供的其他方法，属性及工作原理，还有在代码中使用 deque 的应用案例。
<a name="lsLwo"></a>
### **限制项目的最大数量**
<a name="xH023"></a>
#### **maxlen**
Deque 最有用的功能之一是在实例化该类时，可以使用 maxlen 参数指定一个给定 deque 的 **最大长度**。<br />如果给 maxlen 提供了一个值，那么 deque 将只存储到 maxlen 的项目。在这种情况下，有一个 **有界** deque。一旦一个有界 deque 被指定数量的项目填满，在任何一端添加一个新的项目都会自动删除并丢弃另一端的项目。<br />如果给 maxlen 提供了一个值，那么 deque 只会存储最多的 maxlen 项。此时得到一个有界的 deque。一旦该 deque 被指定数量的项填满，在两端添加一个新项会自动删除并丢弃另一端的项:
```python
>>> from collections import deque

>>> four_numbers = deque([0, 1, 2, 3, 4], maxlen=4) # Discard 0
>>> four_numbers
deque([1, 2, 3, 4], maxlen=4)

>>> four_numbers.append(5)  # Automatically remove 1
>>> four_numbers
deque([2, 3, 4, 5], maxlen=4)

>>> four_numbers.append(6)  # Automatically remove 2
>>> four_numbers
deque([3, 4, 5, 6], maxlen=4)

>>> four_numbers.appendleft(2) # Automatically remove 6
>>> four_numbers
deque([2, 3, 4, 5], maxlen=4)

>>> four_numbers.appendleft(1)  # Automatically remove 5
>>> four_numbers
deque([1, 2, 3, 4], maxlen=4)

>>> four_numbers.maxlen
4
```
如果输入的迭代器中的项目数大于 maxlen，那么 deque 就会丢弃最左边的项目（例子中是0）。一旦 deque 满了，在任何一端追加一个项目都会自动删除另一端的项目。<br />注意，如果没有给 maxlen 指定一个值，那么它默认为 None，deque可以增长到任意数量的项目。<br />有了限制最大项目数量的选项，就可以使用 deques 来追踪特定对象或事件序列中的最新元素。例如，可以跟踪一个银行账户中的最后五笔交易，一个编辑器中的最后十个打开的文本文件，一个浏览器中的最后五个页面，等等。<br />注意，maxlen在 deque 中可以作为一个只读属性，可以用 `deque.maxlen == len(deque)`来检查 deque 是否已满。<br />最后，可以将 maxlen 设置为任何正的整数，代表想存储在一个特定 deque 中的最大项目数。如果给 maxlen 提供一个负值，那么程序会抛出一个 ValueError。
<a name="iOTDf"></a>
### **旋转元素**
<a name="UC0Rg"></a>
#### `**.rotate()**`
Deques 的另一个有趣的特点是可以通过对一个非空 deque 调用 `.rotate()` 来旋转其元素。这个方法需要一个整数n作为参数，将项目向右旋转n步。换句话说，它以循环方式将n个项目从右端移到左端。<br />n的默认值是1。如果给n提供一个负值，那么就向左旋转。
```python
>>> from collections import deque

>>> ordinals = deque(["第一", "第二", "第三"])

>>> # 将项目向右旋转
>>> ordinals.rotate()
>>> ordinals
deque(['第三', '第一', '第二'])

>>> ordinals.rotate(2)
>>> ordinals
deque(['第一', '第二', '第三'])

>>> # 将项目向左旋转
>>> ordinals.rotate(-2)
>>> ordinals
deque(['第三', '第一', '第二'])

>>> ordinals.rotate(-1)
>>> ordinals
deque(['第一', '第二', '第三'])
```
在这些例子中，使用 `.rotate()` 对 n 进行多次旋转。如果调用 `.rotate()` 时不带参数，那么它将依赖于 n 的默认值，并将 deque  1的位置向右旋转。使用负的 n 调用该方法允许您向左旋转项目。
<a name="Lcf5N"></a>
### **一次性添加几个元素**
<a name="bTmvE"></a>
#### `**.extendleft()**`
像常规列表一样，deques 提供了一个 `.extend()` 方法，可以使用一个可迭代的参数向 deque 的右端添加几个项目。此外，deques 还有一个叫做 `extendleft()` 的方法，该方法以一个可迭代项为参数，一次性将其项目添加到目标 deque 的左端。
```python
>>> from collections import deque
>>> numbers = deque([1, 2])

>>> # Extend to the right
>>> numbers.extend([3, 4, 5])
>>> numbers
deque([1, 2, 3, 4, 5])

>>> # Extend to the left
>>> numbers.extendleft([-1, -2, -3, -4, -5])
>>> numbers
deque([-5, -4, -3, -2, -1, 1, 2, 3, 4, 5])
```
使用 iterable 调用 `.extendleft()` 将目标 deque 向左扩展。在内部， `.extendleft()` 执行一系列单独的 `.appendleft()` 操作，从左到右处理输入的可迭代对象。这最终将项目以相反的顺序添加到目标deque的左端。
<a name="SUkUA"></a>
## 使用类似于序列的Deque的特征
由于 deques 是可变序列，它们几乎实现了序列和可变序列所共有的所有方法和操作。到目前为止，已经了解了其中的一些方法和操作，如`.insert()`、索引、成员测试等等。<br />下面是一些可以对 deque 对象进行其他操作的例子。
```python
>>> from collections import deque
>>> numbers = deque([1, 2, 2, 3, 4, 4, 5])

>>> # 连接法
>>> numbers + deque([6, 7, 8])
deque([1, 2, 2, 3, 4, 4, 5, 6, 7, 8])

>>> # 重复
>>> 数字 * 2
deque([1, 2, 2, 3, 4, 4, 5, 1, 2, 2, 3, 4, 4, 5])

>>> # 常见的序列方法
>>> numbers = deque([1, 2, 2, 3, 4, 4, 5])
>>> numbers.index(2)
1
>>> numbers.count(4)
2

>>> # 常见的可变序列方法
>>> numbers.reverse()
>>> numbers
deque([5, 4, 4, 3, 2, 2, 1])

>>> numbers.clear()
>>> numbers
deque([])
```
可以使用加法运算符（+）来串联两个现有的 deques。另一方面，乘法运算符（*）会返回一个新的 deque，相当于将原来的 deque 重复多少次都可以。<br />关于其他序列方法，下表提供了一个摘要。

| **方法** | **方法描述** |
| --- | --- |
| `.clear()` | 删除deque容器中的所有元素。 |
| `.copy()` | 创建一个deque容器的浅副本。 |
| `.count(value)` | 计数 value 在deque容器中出现的次数。 |
| `.index(value)` | 返回 value 在deque容器中的位置。 |
| `.reverse()` | 对deque容器中的元素进行反转，然后返回 None 。 |

这里，`.index()`还可以接受两个可选参数：start 和 stop。它们允许将搜索限制在 start 和 stop 之前或之后的那些项目上。如果值没有出现在手头的 deque 中，该方法会引发一个 ValueError。<br />与列表不同，deques 不包括一个`.sort()`方法来对序列进行就地排序。这是因为对一个链接列表进行排序是一个低效的操作。如果需要对 deque 进行排序，那么仍然可以使用 `sorted()` 方法。
<a name="oBfV8"></a>
## Deque应用案例
可以在相当多的用例中使用 deque，例如实现队列、堆栈和循环缓冲区。也可以用它们来维护一个撤销重做的历史记录，排队进入一个web服务的请求，保存一个最近打开的文件和网站的列表，在多个线程之间安全地交换数据，等等。<br />在下面几节中，将编写一些小示例，这些示例将帮助我们更好地理解如何在代码中使用 deque。
<a name="GCAuV"></a>
### **保存页面历史**
使用 maxlen 来限制项的最大数量使 deque 适合解决几个问题。例如，构建一个应用程序，从搜索引擎和社交媒体网站上抓取数据。有时需要跟踪应用程序请求数据的最后三个站点。<br />要解决这个问题，可以使用一个deque, maxlen值为3:
```python
>>> from collections import deque

>>> sites = (
...     "google.com",
...     "yahoo.com",
...     "bing.com"
... )

>>> pages = deque(maxlen=3)
>>> pages.maxlen
3

>>> for site in sites:
...     pages.appendleft(site)
...

>>> pages
deque(['bing.com', 'yahoo.com', 'google.com'], maxlen=3)

>>> pages.appendleft("facebook.com")
>>> pages
deque(['facebook.com', 'bing.com', 'yahoo.com'], maxlen=3)

>>> pages.appendleft("twitter.com")
>>> pages
deque(['twitter.com', 'facebook.com', 'bing.com'], maxlen=3)
```
在这个例子中，pages 保留了一个应用程序最后访问的三个网站的列表。一旦 pages 满了，在 deque 的一端添加一个新的网站，就会自动丢弃另一端的网站。这种行为使我们的列表保持在最后使用的三个网站的最新状态。<br />注意，可以将 maxlen 设置为任何正整数，代表要存储在 deque 中的项目数量。例如，如果想保存10个网站的列表，那么可以把 maxlen 设置为10。
<a name="XEi5X"></a>
### **在线程之间共享数据**
当编写多线程应用程序时，Python的 deque 也很有用，正如Raymond Hettinger所描述的，他是 deque 和 collections 模块的核心开发人员和创建者。<br />Deque 的 `.append()`、`.appendleft()`、`.pop()`、`.popleft()`和`len(d)`操作在CPython中是线程安全的。<br />因此可以安全地从独立的线程中同时添加和删除 deque 两端的数据，而没有数据损坏或其他相关问题的风险。<br />下面试试 deque 在多线程应用程序中是如何工作的。
```python
# threads.py

import logging
import random
import threading
import time
from collections import deque

logging.basicConfig(level=logging.INFO, format="%(message)s")

def wait_seconds(mins, maxs):
    time.sleep(mins + random.random() * (maxs - mins))

def produce(queue, size):
    while True:
        if len(queue) < size:
            value = random.randint(0, 9)
queue.append(value)
logging.info("Produced: %d -> %s", value, str(queue))
else:
logging.info("Queue is saturated")
wait_seconds(0.1, 0.5)

def consume(queue):
    while True:
        try:
            value = queue.popleft()
except IndexError:
logging.info("Queue is empty")
else:
logging.info("Consumed: %d -> %s", value, str(queue))
wait_seconds(0.2, 0.7)

logging.info("Starting Threads...\n")
logging.info("Press Ctrl+C to interrupt the execution\n")

shared_queue = deque()

threading.Thread(target=produce, args=(shared_queue, 10)).start()
threading.Thread(target=consume, args=(shared_queue,)).start()
```
这里，`produce()` 以 queue 和 size 作为参数。然后它在 while 循环中使用 `random.randint()` 连续产生随机数，并将其存储在名为 shared_queue 的全局 deque 容器中。由于向 deque 添加项是线程安全的操作，因此不需要使用锁来保护共享数据不受其他线程的影响。<br />辅助函数 `wait_seconds()` 模拟了`production()` 和 `consumer()` 都表示长时间运行的操作。它返回一个在给定的秒、分钟和最大范围之间的随机等待时间值。<br />在 `consumer()` 中，在一个循环中调用 `.popleft()`，系统地从 shared_queue 中检索和删除数据。将对 `.popleft()` 的调用包裹在一个try ... except语句中，以处理共享队列为空的情况。<br />注意，虽然在全局命名空间中定义了 shared_queue，但通过 `produce()` 和 `consumer()` 中的局部变量访问它。直接访问全局变量会有更多问题。<br />脚本的最后两行创建并启动独立的线程，以并发地执行 `produce()` 和 `consumer()`。如果在命令行中运行该脚本，那么会得到类似于以下的输出。
```python
$ python threads.py
Starting Threads...

Press Ctrl+C to interrupt the execution

Produced: 1 -> deque([1])
Consumed: 1 -> deque([])
Queue is empty
Produced: 3 -> deque([3])
Produced: 0 -> deque([3, 0])
Consumed: 3 -> deque([0])
Consumed: 0 -> deque([])
Produced: 1 -> deque([1])
Produced: 0 -> deque([1, 0])
...
```
生产者线程向共享 deque 的右端添加数字，而消费者线程从左端消耗数字。由于是一直循环运行，要中断脚本的执行，可以按键盘上的Ctrl+C。<br />最后，可以在 `produce()` 和 `consumer()` 里面玩一下时间间隔。改变传递给`wait_seconds()` 的值，观察当生产者比消费者慢时和反过来时程序的表现。
<a name="yUgN5"></a>
### **模仿tail命令**
在这里模仿 tail 命令该命令可在 Unix 和 Unix-like 操作系统上使用。该命令在命令行接受一个文件路径，并将该文件的最后10行打印到系统的标准输出。可以使用 `-n`、`-lines` 选项来调整 `tail` 需要打印的行数。<br />这里有一个小的Python函数，它模拟了tail的核心功能。
```python
>>> from collections import deque

>>> def tail(filename, lines=10):
...     try:
...         with open(filename) as file:
...             return deque(file, lines)
...     except OSError as error:
...         print(f'Opening file "{filename}" failed with error: {error}')
...
```
这里，定义了`tail()`函数。第一个参数 filename 字符串类型，是目标文件的路径。第二个参数 lines，表示想从目标文件的末端检索的行数，默认为10，模拟tail的默认行为。<br />Deque最多只能存储传递给行的数量。这可以保证从输入文件的末尾得到所需的行数。<br />当创建一个有界的 deque 并且用一个包含超过允许的项目（maxlen）的 iterable 初始化它时，deque 构造函数会丢弃输入中所有最左边的项目。因此最终会得到目标文件的最后几行 maxlen。
<a name="IPB0n"></a>
## 总结
队列和堆栈是编程中常用的 **抽象数据类型**。它们通常需要在底层数据结构的两端进行有效的 **pop** 和 **append** 操作。Python 的 collections 模块提供了一种叫做 deque 的数据类型，它是专门为两端的快速和节省内存的追加和弹出操作而设计的。<br />有了deque，可以用优雅、高效和Pythonic的方式在低层次上编写自己的队列和堆栈。<br />总结下本文所学内容：

- **如何在代码中创建和使用Python的deque**
- **如何有效地从deque的两端追加和弹出项目**
- **如何使用deque来构建高效的队列和堆栈**
- **什么时候值得使用deque而不是list**
<a name="XHt90"></a>
## 参考资料
deque: [https://docs.python.org/3/library/collections.html#collections.deque](https://docs.python.org/3/library/collections.html#collections.deque)
