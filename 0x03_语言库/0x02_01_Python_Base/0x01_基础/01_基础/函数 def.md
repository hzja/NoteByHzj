# 函数 def

## 作用

复用代码 重用代码 函数将功能封装起来，在用的时候直接调用

~~~ python
def my_function():
    """这是我的第一个函数"""f
    for i in range(1, 7):
        for j in range(1, i + 1):
            print('%d * %d = %d' % (i, j, i * j), end='\t')
# 调用函数
my_function()
print('打印九九乘法表!')

my_function()
# 如果函数有参数的话，那么在调用函数的时候必须传递参数
~~~



## return后其他函数可以直接调用次函数

~~~ python
def my_func():
    print('aaa')
    return
    # 函数中只要执行了return 语句， 函数马上终止
    # 如果说return后面有值，则将值返回给调用者.
    # 如果return后面没没有值, 那么返回None
    print('bbb')


ret = my_func()
print(ret)
~~~



## global用法修改全局变量

~~~ python
# 局部变量，在函数内部定义变量
# 局部变量只能在函数内部有效
#  全局变量可以使用，但是不能直接修改，如果需要修改的话，需要使用global关键字声明

def my_func03():
    # 声明全局变量
    global global_param
    global_param = 500
    print(global_param)

my_func03()
print(global_param)
~~~



## 函数参数的默认值 缺省参数

~~~ python
def my_plus(a=0, b=0, c=0):
    return a + b + c


# 如果函数有参数的话，必须传递参数
ret = my_plus(10, 20, 30)
print(ret)

# 如果我不传递参数的话
ret = my_plus(10)
print(ret)

ret = my_plus(10, 100)
print(ret)
~~~



## 异步函数

async是Python中用于定义异步函数的关键字。在普通的函数定义前面加上async关键字，可以将其声明为一个异步函数。

~~~ python 
async def main():
    async with aiohttp.ClientSession() as session:
        async with session.get('http://httpbin.org/get') as resp:
            print(resp.status)
            print(await resp.text())

asyncio.run(main())
~~~

异步函数使用async/await语法，能够以非阻塞的方式执行任务，允许在等待IO操作完成时暂时释放CPU，从而提高程序的并发性能。

使用async关键字定义的函数可以包含一个或多个await表达式，await用于暂停当前的异步函数的执行，等待一个异步操作（如IO请求）完成后再继续执行。

通过将函数标记为异步函数，可以更方便地编写和管理并发代码，避免了传统的回调函数嵌套和复杂的线程/进程管理。在Python中，aiohttp等异步框架广泛使用async/await来实现高性能的异步IO操作。

需要注意的是，只有在异步上下文中（如异步函数、协程等）才能使用await关键字，否则会导致语法错误。
