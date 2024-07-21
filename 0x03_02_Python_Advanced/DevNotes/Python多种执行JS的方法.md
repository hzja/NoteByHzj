JavaScript（JS）是一种常用的脚本语言，通常用于网页开发，但有时也需要在Python中执行或调用JavaScript代码。这种需求可能是因为希望与网页进行交互，或者希望在Python应用程序中运行JavaScript以实现某些功能。<br />本文将详细介绍Python中执行和调用JavaScript的多种方法，包括内置的execjs库、外部库如PyExecJS、使用浏览器引擎和与Node.js的交互。每种方法都将附有示例代码，方便理解如何在Python中与JavaScript进行互动。
<a name="dqo0L"></a>
## 一、使用内置的execjs库
Python中的execjs库可以执行JavaScript代码。这是一个纯Python库，无需任何依赖，适用于基本的JavaScript操作。<br />以下是一个简单示例，演示如何在Python中使用execjs执行JavaScript代码：
```python
import execjs

# 创建一个JavaScript环境
context = execjs.compile("""
function add(a, b) {
    return a + b;
}
""")

# 调用JavaScript函数
result = context.call("add", 2, 3)
print(result)  # 输出：5
```
在这个示例中，创建了一个execjs环境，并编写了一个JavaScript函数`add`，然后使用`context.call()`来调用该函数并获得结果。
<a name="xVVRL"></a>
## 二、使用PyExecJS库
PyExecJS是另一个Python库，它是对execjs库的封装，提供更多功能和更好的性能。<br />以下是一个使用PyExecJS的示例：
```python
import PyExecJS

# 创建一个JavaScript环境
context = PyExecJS.compile("""
function subtract(a, b) {
    return a - b;
}
""")

# 调用JavaScript函数
result = context.call("subtract", 5, 3)
print(result)  # 输出：2
```
与execjs类似，创建了一个JavaScript环境，并使用`context.call()`来执行JavaScript函数。
<a name="fhjDK"></a>
## 三、使用浏览器引擎
还可以在Python中模拟浏览器行为或与网页进行交互。为此，可以使用Python中的浏览器引擎，如Selenium和Playwright。这些工具自动化浏览器，执行JavaScript代码，并获取网页上的数据。<br />以下是使用Selenium的示例：
```python
from selenium import webdriver

# 创建浏览器驱动
driver = webdriver.Chrome()

# 打开网页
driver.get("https://example.com")

# 执行JavaScript
result = driver.execute_script("return 2 + 3")
print(result)  # 输出：5

# 关闭浏览器
driver.quit()
```
在这个示例中，使用Selenium创建了一个Chrome浏览器实例，然后打开网页、执行JavaScript代码并获取结果。
<a name="xgDo6"></a>
## 四、与Node.js的交互
如果需要在Python中运行Node.js脚本或与Node.js进程交互，可以使用subprocess模块。<br />以下是一个示例，演示如何在Python中调用Node.js脚本：
```python
import subprocess

# 定义Node.js脚本
node_script = """
console.log(2 + 3);
"""

# 在Python中执行Node.js脚本
result = subprocess.check_output(["node", "-e", node_script], text=True)
print(result)  # 输出：5
```
在这个示例中，定义了一个Node.js脚本，然后使用subprocess模块执行它。
<a name="dCVf9"></a>
## 总结
Python中执行和调用JavaScript代码有多种方法，每种方法适用于不同的场景。execjs和PyExecJS适用于在Python中轻松执行JavaScript代码，而Selenium和Playwright等浏览器引擎用于与Web页面进行交互和执行JavaScript。另外，如果需要与Node.js进行交互，可以使用subprocess模块。<br />选择合适的方法取决于具体需求和项目背景。无论您是在Web开发中需要与前端代码交互，还是需要自动化执行JavaScript脚本，Python提供了多种工具来满足需求。
