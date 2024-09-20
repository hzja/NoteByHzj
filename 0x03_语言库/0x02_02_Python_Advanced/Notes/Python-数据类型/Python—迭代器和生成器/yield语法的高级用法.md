<a name="Sa02r"></a>
## `yield`用于生成器产生单个值
大多数小伙伴肯定是从**生成器函数**中第一次看到的`yield`关键字的吧？<br />没错，这是`yield`最常用的场景。
```python
def simple_gen():
    yield 1
    yield 2
    yield 3

gen = simple_gen()
print(next(gen))  # 输出 1
print(next(gen))  # 输出 2
```
上面的代码，`yield`用于在生成器函数中产生单个值。<br />控制权返回给调用者，但保留生成器的状态，使得下次调用`next()`时可以从上一次`yield`的位置继续执行。<br />如果将生成器放入循环语句中`yield`可多次产生值
```python
def simple_gen():
    yield 1
    yield 2
    yield 3

gen = simple_gen()
for i in gen:
    print(i)
# 输出 1 2 3
```
<a name="DL360"></a>
## yield from + 委托生成器
如果有多个生成需要组合成一个生成器，并希望有顺序地输出值该怎么办？
```python
def generator1():
    yield 1
    yield 2

def generator2():
    yield 3
    yield 4
```
可以这样写去实现：
```python
def generator1():
    yield 1
    yield 2

def generator2():
    yield 3
    yield 4

def combined_generator():
    for i in generator1():
        yield i
    for i in generator2():
        yield i

main_gen = combined_generator()
for i in main_gen:
    print(i)

# 输出 1 2 3 4
```
需要for循环才能实现，有点难看...<br />引入**yield from + 委托生成器**之后进行改写：<br />`yield from`是python3.3版本引入的。
```python
def generator1():
    yield 1
    yield 2

def generator2():
    yield 3
    yield 4

####################
def combined_generator():
    yield from generator1()
    yield from generator2()
####################

main_gen = combined_generator()

for i in main_gen:
    print(i)

# 输出 1 2 3 4
```
改写后的代码，`yield from`用于在生成器函数(`combined_generator`)中**委托部分生成任务**给其他生成器(generator1,generator2)， 而将子生成器的输出直接传递给调用者，而不需要在主生成器中显式循环子生成器的输出。<br />更重要的是！！减少了循环的使用，减少了嵌套层次使得代码更加简洁。**前提是已经学会了这个语法。**
<a name="ZyMIf"></a>
## 有yield参与的赋值语句
先上代码：
```python
def generator_function():
    value = yield "hello"
    print(f"从yield接收的值是{value}")
    yield value
```
有没有感觉到一脸懵？？？为什么yield能用于赋值语句？？？<br />说好的yield和return类似能在函数中返回一个值，怎么还能用于赋值语句？？？<br />来强调下一件事：`return`是返回后退出函数，函数是被**销毁**的。而`yield`返回后是将函数挂起，函数还是**存活**的。<br />所以`value = yield "hello"`这句执行后，函数是被挂起的。<br />那从`yield`那里接收的值是"hello"吗？<br />不是的，从`yield`中接收的值要从函数外部给它**发送**进去。<br />完整代码：
```python
def generator_function():
    value = yield "hello"
    print(f"从yield接收的值是{value}")
    yield value

g = generator_function()

first = next(g)
print(first)
second = g.send("world") # 向yield发送要接收的值 
print(second)
```
当send函数执行后，会默认对生成器执行next方法到下一个返回/挂起点。<br />代码执行结果：
```
hello
从yield接收的值是world
world
```
<a name="O3vA0"></a>
### yield实现一个状态机
如果对上面的知识点能略懂略懂，那么就可以看下这段**状态机**的代码巩固下知识点。
```python
def vending_machine():
    state = "idle"
    while True:
        if state == "idle":
            print("售货机空闲。插入硬币以开始售货。")
            coin = yield
            if coin:
                print("插入硬币。切换到售货状态。")
                state = "售货中"
            else:
                print("未插入硬币。保持在空闲状态。")

        elif state == "售货中":
            print("售货机售货中。按下按钮以选择商品。")
            selection = yield
            if selection:
                print(f"售货商品 {selection}。返回到空闲状态。")
                state = "空闲"
            else:
                print("未选择商品。继续售货。")

# 创建状态机生成器对象
vm_gen = vending_machine()

# 启动状态机
next(vm_gen)

# 模拟用户操作
vm_gen.send(True)  # 插入硬币，切换到售货状态
vm_gen.send("苹果汁")  # 选择商品，返回到空闲状态
```
代码解释：

1. vending_machine 函数定义了一个状态机，有两个状态："idle"（空闲）和 "vending"（售货中）。
2. 通过 while True 实现一个无限循环，保持状态机的运行。
3. 在 "idle" 状态下，状态机等待硬币的插入。如果收到硬币，状态切换到 "vending"。
4. 在 "vending" 状态下，状态机等待用户按下按钮选择商品。如果选择了商品，状态切换回 "idle"。
5. 通过 yield 实现暂停和恢复状态机的执行。yield 语句在产生值的同时也接收到来自外部的输入。
6. 在主程序中，创建了状态机生成器对象 vm_gen，并通过调用 `next(vm_gen)` 启动了状态机。然后，通过 vm_gen.send(True) 模拟了插入硬7. 币的操作，再通过 `vm_gen.send("苹果汁")` 模拟了选择商品的操作。

如果还是对这段代码存疑，最好再使用编辑器的debug跑一下一段代码，观察yield的状态（作用）。
