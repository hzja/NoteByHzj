Python<br />代码的哪些部分运行时间最长、内存最多？怎样才能找到需要改进的地方？”<br />在开发过程中，很确定大多数人都会想知道这一点，在本文中总结了一些方法来监控 Python 代码的时间和内存使用情况。<br />本文将介绍4种方法，前3种方法提供时间信息，第4个方法可以获得内存使用情况。

- time 模块
- `%%time` 魔法命令
- line_profiler
- memory_profiler
<a name="PGzJ4"></a>
### 1、time 模块
这是计算代码运行所需时间的最简单、最直接(但需要手动开发)的方法。他的逻辑也很简单：记录代码运行之前和之后的时间，计算时间之间的差异。这可以实现如下：
```python
import time

start_time = time.time()
result = 5+2
end_time = time.time()

print('Time taken = {} sec'.format(end_time - start_time))
```
下面的例子显示了for循环和列表推导式在时间上的差异：
```python
import time

# for loop vs. list comp
list_comp_start_time = time.time()
result = [i for i in range(0,1000000)]
list_comp_end_time = time.time()
print('Time taken for list comp = {} sec'.format(list_comp_end_time - list_comp_start_time))

result=[]
for_loop_start_time = time.time()
for i in range(0,1000000):
 	result.append(i)
for_loop_end_time = time.time()
print('Time taken for for-loop = {} sec'.format(for_loop_end_time - for_loop_start_time))

list_comp_time = list_comp_end_time - list_comp_start_time
for_loop_time = for_loop_end_time - for_loop_start_time
print('Difference = {} %'.format((for_loop_time - list_comp_time)/list_comp_time * 100))
```
都知道for会慢一些
```
Time taken for list comp = 0.05843973159790039 sec
Time taken for for-loop = 0.06774497032165527 sec
Difference = 15.922795107582594 %
```
<a name="zJPGv"></a>
### 2、`%%time` 魔法命令
魔法命令是IPython内核中内置的方便命令，可以方便地执行特定的任务。一般情况下都实在jupyter notebook种使用。<br />在单元格的开头添加`%%time` ，单元格执行完成后，会输出单元格执行所花费的时间。
```python
%%time
def convert_cms(cm, unit='m'):
    '''
    Function to convert cm to m or feet
    '''
    if unit == 'm':
        return cm/100
    return cm/30.48

convert_cms(1000)
```
结果如下：
```
CPU times: user 24 µs, sys: 1 µs, total: 25 µs
Wall time: 28.1 µs

Out[8]: 10.0
```
这里的CPU times是CPU处理代码所花费的实际时间，Wall time是事件经过的真实时间，在方法入口和方法出口之间的时间。
<a name="jNajl"></a>
### 3、line_profiler
前两个方法只提供执行该方法所需的总时间。通过时间分析器可以获得函数中每一个代码的运行时间。<br />这里需要使用line_profiler包。使用`pip install line_profiler`。
```python
import line_profiler

def convert_cms(cm, unit='m'):
    '''
    Function to convert cm to m or feet
    '''
    if unit == 'm':
        return cm/100
    return cm/30.48

    # Load the profiler
%load_ext line_profiler

# Use the profiler's magic to call the method
%lprun -f convert_cms convert_cms(1000, 'f')
```
输出结果如下：
```
Timer unit: 1e-06 s

 Total time: 4e-06 s
 File: /var/folders/y_/ff7_m0c146ddrr_mctd4vpkh0000gn/T/ipykernel_22452/382784489.py
 Function: convert_cms at line 1

 Line #     Hits         Time Per Hit   % Time Line Contents
 ==============================================================
      1                                           def convert_cms(cm, unit='m'):
      2                                               '''
      3                                               Function to convert cm to m or feet
      4                                               '''
      5         1         2.0     2.0     50.0     if unit == 'm':
      6                                                   return cm/100
      7         1         2.0     2.0     50.0     return cm/30.48
```
可以看到line_profiler提供了每行代码所花费时间的详细信息。

- Line Contents：运行的代码
- Hits：行被执行的次数
- Time：所花费的总时间(即命中次数x每次命中次数)
- Per Hit：一次执行花费的时间，也就是说 Time =  Hits X Per Hit
- % Time：占总时间的比例

可以看到，每一行代码都详细的分析了时间，这对于分析时间相当的有帮助。
<a name="eHWWV"></a>
### 4、memory_profiler
与line_profiler类似，memory_profiler提供代码的逐行内存使用情况。<br />要安装它需要使用`pip install memory_profiler`。这里监视convert_cms_f函数的内存使用情况
```python
from conversions import convert_cms_f
import memory_profiler

%load_ext memory_profiler

%mprun -f convert_cms_f convert_cms_f(1000, 'f')
```
convert_cms_f函数在单独的文件中定义，然后导入。结果如下：
```
Line #   Mem usage   Increment Occurrences   Line Contents
 =============================================================
      1     63.7 MiB     63.7 MiB           1   def convert_cms_f(cm, unit='m'):
      2                                             '''
      3                                             Function to convert cm to m or feet
      4                                             '''
      5     63.7 MiB     0.0 MiB           1       if unit == 'm':
      6                                                 return cm/100
      7     63.7 MiB     0.0 MiB           1       return cm/30.48memory_profiler 提供对每行代码内存使用情况的详细了解。
```
这里的1 MiB (MebiByte) 几乎等于 1MB。1 MiB  = 1.048576 1MB<br />但是memory_profiler 也有一些缺点：它通过查询操作系统内存，所以结果可能与 python 解释器略有不同，如果在会话中多次运行 %mprun，可能会注意到增量列报告所有代码行为 0.0 MiB。这是因为魔法命令的限制导致的。<br />虽然memory_profiler有一些问题，但是它就使我们能够清楚地了解内存使用情况，对于开发来说是一个非常好用的工具
<a name="zLQk6"></a>
### 5、总结一下
虽然Python并不是一个以执行效率见长的语言，但是在某些特殊情况下这些命令还是非常有帮助的。
