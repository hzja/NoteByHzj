Python<br />filecmp 模块主要用于比较文件及目录，difflib 模块主要用于比较序列的类和函数，下面具体介绍两者的区别。
<a name="Lh7nm"></a>
## filecmp 模块
filecmp 模块作为 Python 提供的标准库之一，无需安装，模块定义了用于比较文件及目录的函数，对服务器上的文件目录的校验非常实用。
<a name="OYQbZ"></a>
### `cmp(f1,f2)`函数
`cmp(f1,f2)`函数用于比较两个文件是否相同，如果 f1 和 f2 相等则返回True，否则返回False。例如：
```python
#新建两个文件f1 和 f2
#f1存储的数据是1,2,3
#f2存储的数据是4,5,6
import filecmp
print(filecmp.cmp("f1","f1"))
print(filecmp.cmp("f1","f2"))

False
True
```
<a name="zZefx"></a>
### `cmpfiles(dir1, dir2, common[, shallow])`函数
`cmpfiles(dir1, dir2, common[, shallow])`函数可以用于比较两个文件夹内指定的文件是否相等，参数dir1 和 dir2 是两个文件夹路径，参数common是指定要比较的文件名列表。<br />例如，`cmpfiles('a', 'b', ['c', 'd/e'])` 将会比较 a/c 与 b/c 以及 a/d/e 与 b/d/e。'c' 和 'd/e' 将会各自出现在返回的三个列表里的某一个列表中。
<a name="MC2oh"></a>
### `clear_cache()`函数
`clear_cache()`函数可以清除文件比较缓冲区，当比较的文件不断地快速修改时，就可以使用这个函数来更新文件的信息。<br />除了以上三个函数外，filecmp 还包含一个用于比较目录的类——dircmp。<br />类`dircmp(a,b[,ignore[,hide]])`用于创建一个比较目录 a 和 b 的新的目录比较对象。`ignore` 是需要忽略的文件名列表，且默认为 `filecmp.DEFAULT_IGNORES`。`hide` 是需要隐藏的文件名列表，且默认为 `[os.curdir, os.pardir]`。<br />dircmp 类提供了三个方法用于比较报告：

- `report()` 将 a 与 b 之间的比较结果打印出来
- `report_partial_closure()` 打印 a 与 b 及共同直接子目录的比较结果
- `report_full_closure()` 打印 a 与 b 及共同子目录比较结果(递归地)
<a name="imsG6"></a>
## difflib模块
difflib 模块是 Python 提供的比较序列差异的功能，包含以下三个类：

- `Differ` 对字符串进行比较
- `HtmlDiff` 将比较结果输出为html格式
- `SequenceMatcher` 任意类型序列的比较 (可以比较字符串)
<a name="SmLVl"></a>
### `Differ`类
`Differ`类的作用是比较由文本行组成的序列，并产生可供人阅读的差异或增量信息。Differ 统一使用 `SequenceMatcher` 来完成行序列的比较以及相似（接近匹配）行内部字符序列的比较。

| 双字母代码 | 含义 |
| --- | --- |
| `'-'` | 行为序列 1 所独有 |
| `'+'` | 行为序列 2 所独有 |
| `' '` | 行在两序列中相同 |
| `'?'` | 行不存在于任一输入序列 |

以 `'?'` 打头的行尝试将视线引至行以外而不存在于任一输入序列的差异。如果序列包含制表符则这些行可能会令人感到迷惑。
<a name="TInKE"></a>
### `HtmlDiff`类
`HtmlDiff`类可以被用来创建HTML表格 (或者说包含表格的html文件) ，两边对应展示或者行对行的展示比对差异结果。
```python
make_file(fromlines, tolines [, fromdesc][, todesc][, context][, numlines])
make_table(fromlines, tolines [, fromdesc][, todesc][, context][, numlines])
```
以上两个方法都可以用来生成包含一个内容为比对结果的表格的html文件，并且部分内容会高亮显示。
<a name="DVhJn"></a>
### `SequenceMatcher`类
`SequenceMatcher`类可用于比较任何类型的序列对，只要序列元素为 hashable 对象。其思路是找到不包含“垃圾”元素的最长连续匹配子序列；所谓“垃圾”元素是指其在某种意义上没有价值，例如空白行或空白符。
```python
import difflib
from pprint import pprint

a = 'pythonclub.org is wonderful'
b = 'Pythonclub.org also wonderful'
s = difflib.SequenceMatcher(None, a, b)

print ("s.get_matching_blocks():")
pprint(s.get_matching_blocks())
print ("s.get_opcodes():")
for tag, i1, i2, j1, j2 in s.get_opcodes():
    print ("%7s a[%d:%d] (%s) b[%d:%d] (%s)" %  (tag, i1, i2, a[i1:i2], j1, j2, b[j1:j2]))
```
输出结果：
```python
s.get_matching_blocks():
[Match(a=1, b=1, size=14),
 Match(a=16, b=17, size=1),
 Match(a=17, b=19, size=10),
 Match(a=27, b=29, size=0)]
s.get_opcodes():
replace a[0:1] (p) b[0:1] (P)
  equal a[1:15] (ythonclub.org ) b[1:15] (ythonclub.org )
replace a[15:16] (i) b[15:17] (al)
  equal a[16:17] (s) b[17:18] (s)
 insert a[17:17] () b[18:19] (o)
  equal a[17:27] ( wonderful) b[19:29] ( wonderful)
```
<a name="nJPsa"></a>
## 总结
参考：<br />[https://docs.python.org/zh-cn/3.7/library/filecmp.html](https://docs.python.org/zh-cn/3.7/library/filecmp.html)<br />[https://docs.python.org/zh-cn/3.7/library/difflib.html](https://docs.python.org/zh-cn/3.7/library/difflib.html)
