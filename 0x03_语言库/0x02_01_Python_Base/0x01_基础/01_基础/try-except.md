# try-except语句



[官方文档](https://docs.python.org/zh-cn/3/reference/compound_stmts.html#the-try-statement)

+ 通用程序结构

~~~ python
try:
	正常执行程序
except Exception as e:
    print("捕获到异常:")
    print("异常类型:", type(e))
    print("异常信息:", e)
~~~

