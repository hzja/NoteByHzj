Python<br />PS：这里以 Chrome 为例，其他浏览器类似
<a name="gL3Cj"></a>
### 1、查看本地 Chrome 浏览器的的安装地址
比如：C:\Program Files\Google\Chrome\Application\chrome.exe
<a name="IbC7U"></a>
### 2、命令行启动浏览器
打开 CMD 终端，输入下面命令行打开 Chrome 浏览器
```bash
# 使用无痕模式，打开目标网站，最大化展示
cd C:\Program Files\Google\Chrome\Application\ && chrome.exe --remote-debugging-port=6666 --user-data-dir="C:\work\chrome"  --start-maximized  --incognito --new-window https://www.taobao.com
```
其中

- `--remote-debugging-port 端口号`

指定浏览器调试端口号<br />PS：这里可以随机指定一个端口号，不要指定为已经被占用的端口号

- `--user-data-dir 用户数据保存目录`

指定浏览器的用户数据保存目录<br />注意：需要设置到一个全新的目录，不要影响 Chrome 浏览器系统用户的数据

- `--incognito 用户数据保存目录`无痕模式打开，默认非无痕模式
- `--start-maximized`窗口最大化显示
- `--new-window 目标地址`直接打开目标网站
<a name="noaOA"></a>
### 3、编写代码操作浏览器
使用命令行打开 Chrome 浏览器后，就可以使用 Playwright 编写代码，继续对浏览器进行操作<br />注意：必须保证上面的操作只打开一个浏览器窗口，方便进行操作
<a name="qW9r7"></a>
### 4、实战一下
目标：使用 Playwright 操作上面命令行打开的浏览器页面，根据关键字进行搜索，获取商品标题及地址<br />需要注意的是，通过 connect_over_cdp 指定的端口号要和浏览器调试端口号保持一致
```python
from playwright.sync_api import sync_playwright

with sync_playwright() as p:
    browser = p.chromium.connect_over_cdp('http://localhost:6666/')

    # 获取页面对象
    # 上下文索引：第一个
    # page索引：第一个
    page = browser.contexts[0].pages[0]

    # 输入
    page.locator("#q").fill("Python")

    # 点击搜索
    page.locator(".btn-search").click()

    # 等待元素出现
    # 注意：由于page.locator能获取多条数据，这里使用first获取1条数据，避免wait_for()方法报错
    page.locator("div[class^=Card--doubleCard]").first.wait_for(state='visible')

    # 获取商品盒子下所有div元素列表
    elements = page.locator("div[class^=Content--contentInner]>div").all()

    # 遍历，获取标题及地址
    for element in elements:
        # 标题
        title_element = element.locator("div[class^=Title--title--] > span").first

        # 地址
        href_element = element.locator("a[class^=Card--doubleCardWrapper--]")
        href = "https:" + href_element.get_attribute("href")
        print("标题：", title_element.text_content(), "，URL：", href)
```
