Python 模块 包
<a name="SEru4"></a>
## 模块
<a name="JrH7z"></a>
### 模块的定义
定义：逻辑上就是一组功能的组合；实质上一个模块就是一个包含了Python定义和声明的文件，文件名就是模块名字加上.py后缀。<br />`import`加载的模块分为四个通用类别：

- 使用python编写的代码（.py文件）
- 已被编译为共享库或DLL的C或C++扩展；
- 包好一组模块的包；
- 使用C编写并链接到Python解释器的内置模块；
<a name="aLHvz"></a>
### 使用模块
想要使用模块，必须先要将模块加载进来，可以通过关键字 `import`  或 `from`进行加载；需要注意的是模块和当前文件在不同的命名空间中。
<a name="ymjmo"></a>
#### 模块的构成
模块可以包含可执行的语句和函数的定义，这些语句的目的是初始化模块，它们只在模块名第一次遇到导入`import`语句时才执行（`import`语句是可以在程序中的任意位置使用的,为了避免同一个模块重复导入，Python的优化手段是：第一次导入后就将模块名加载到内存了，后续的`import`语句仅是对已经加载大内存中的模块对象增加了一次引用，不会重新执行模块内的语句）
<a name="PQBDw"></a>
#### 模块导入的过程
`import` 语句<br />想使用 Python 源文件，只需在另一个源文件里执行 `import` 语句，语法如下：

1. `import moduels` (模块名字）  <br />导入整个模块，这种导入方式比较占用内存
2. `import moduels (模块名字) as XX`<br />这里是导入整个模块的同时给它取一个别名，因为有些模块名字比较长，用一个缩写的别名代替在下次用到它时就比较方便
3. `from modules(模块名字) import func(方法)`<br />从一个模块里导入方法，要用到模块里的什么方法就从那个模块里导入那个方法，这样占用的内存就比较少<br />也可以用别名表示 ：`from modules(模块名字) import func(方法）as XX`
4. `from package.modules import func(方法)`<br />从一个包的模块里导入方法 这个方法跟上面那种基本一样，占用的内存也比较少<br />也可以用别名表示，`from modules(模块名字) import func(方法）as XX`
5. `from.....import *`<br />表示导入模块中所有的不是以下划线(`_`)开头的名字都导入到当前位置，大部分情况下Python程序不应该使用这种导入方式，因为`*`不知道导入什么名字，很有可能会覆盖掉之前已经定义的名字。而且可读性极其的差，在交互式环境中导入时没有问题。

还有一点要说的是，如果使用`*` 的方式进行了导入，这时只想使用里面的某个或某些功能时，可以使用`__all__`来进行约束；<br />注意：`__all__`只是用来约束`*` 方式的，其他方式导入的话，不会生效；
<a name="k6D4K"></a>
##### 直接导入
```python
import pandas
```
<a name="3cEGY"></a>
##### 加别名
```python
import pandas as pd
```
<a name="f10Vq"></a>
##### `from.....import` 的使用
```python
from sys import modules
print(modules)
```
<a name="Szlam"></a>
##### 使用`as`引用这个模块的某个功能
```python
from matplotlib import pyplot as plt
```
:::info
这样在之后要调用这个模块只需要用plt即可，相当于起别名。
:::
<a name="VUEqe"></a>
## 包
包是一种组织管理代码的方式，包里放着模块<br />用于将模块包含在一起的一个文件夹<br />假如一个模块的名称是 A.B，那么他表示一个包 A中的子模块B 。
<a name="ojD0H"></a>
### 包的结构

- /--- 包   #顶层包
- /---/--- init.py 包的  #初始化顶层包
- /---/--- 模块1
- /---/--- 模块2
- /---/--- 子包
- /---/---/--- init.py 包的标志文件
- /---/---/--- 子包的模块1
<a name="CO7TW"></a>
### 包的导入操作
包的导入分为`import`和`from...import...`两种，但是无论哪种方式，在导入时必须都遵循一个原则：凡是在导入时带点的，点的左边必须是一个包，否则非法。在导入后使用时，就没有这些规则了，点的左边可以是包、模块、函数等（他们可以用点的方式调用自己的属性）。<br />包的本质就是文件夹，导入包就相当于导入包下的`__init__.py`文件<br />包的导入方式

1. `import package_name`<br />直接导入一个包，可以直接使用`__init__.py`的内容<br />eg.`import time`， 用`import`直接导入Python的`time`模块
2. `import package name as p`<br />具体用法跟作用方法，跟上面一致，相当于起别名<br />注意的是此方法默认是对 `__init__.py`的内容进行导入
3. `from ... import ...导入`<br />`from package import module,module2.....`<br />eg. `from random import randint`   即导入`random`包中的`randint`函数<br />      `from time import time,localtima`  即从`time`包导入`time`函数和`localtime`函数
4. `from package import *`<br />导入指定包的所有内容
<a name="TflHP"></a>
### 包和模块区别
包是比模块更高层的概念，模块一般是一个文件，包是一个目录，一个包中可以包含很多个模块。<br />包的all 定义在init.py文件中，模块的all定义在模块文件的开头<br />以下包用P表示，模块用M表示，方法用F表示
<a name="N4IPc"></a>
#### 在Python中引入模块可以用
```python
import M
from M import *
from M import F
```
<a name="VzKJ8"></a>
#### 引入包的方式可以是
```python
import P
from P import M
from P import *
from P.M import F
from P1.P2.M import F
```
:::danger
另外，包的init.py文件不能少。
:::
<a name="Momod"></a>
### 概念说明
这里理清Python中模块、库、包之间的概念差异<br />模块(module)其实就是py文件，里面定义了一些函数、类、变量等。<br />包(package)是多个模块的聚合体形成的文件夹，里面可以是多个py文件，也可以嵌套文件夹。<br />库是参考其他编程语言的说法，是指完成一定功能的代码集合，在Python中的形式就是模块和包。
