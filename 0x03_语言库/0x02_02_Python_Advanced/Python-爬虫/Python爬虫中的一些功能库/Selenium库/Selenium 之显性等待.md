Python 爬虫 Selenium
<a name="rbR9p"></a>
## 显性等待使用
在使用 `WebDriverWait` 调用可执行方法，除了可定位的元素，还可以使用 `selenium` 提供的 `expected_conditions` 模块中的各种条件，也可以使用 `WebElement` 的 `is_enabled()`，`is_selected()`，`is_displayed()` 等等方法，还可以使用自己封装的方法。<br />这次换百度官网测试，首先使用显性等待的标准使用方式，这里使用的 `LINK_TEXT` 查找的元素：
```python
# 导入库
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as ECS
from selenium.webdriver.common.by import By
import time

# executable_path 用于指定driver存放路径
browser = webdriver.Chrome(executable_path='/Users/xx/python/chromedriver')

# 打开京东官网
browser.get('https://www.baidu.com/')

# 定位要查找的元素
loc = (By.LINK_TEXT, "关于百度")

try:
  # 等待5秒，直到发现元素
    WebDriverWait(driver, 5).until(ECS.presence_of_element_located(loc))
except:
  # 没有发现元素则会打印提示
    print("没有找到对应元素！")
finally:
  # 发现元素则执行下面的方法
    driver.find_element_by_link_text('关于百度').click()

#退出浏览器
browser.quit()
```
<a name="IL3MM"></a>
## 显性等待方式介绍
好了，知道显性等待怎么用了，接下来来看下，还有哪些其他常见的使用方式：

- 先来个简单的，这里通过页面标题来判断
```python
# 元素等待直到标题出现 `百度一下，你就知道` 
WebDriverWait(driver,3).until(ECS.title_is('百度一下，你就知道'))

# 元素等待直到标题出现包含 `百度` 即可
WebDriverWait(driver,3).until(ECS.title_contains('百度'))
```

- 通常在网页中并不是所有元素都是可见的，也有的是初始化时不可见，加载完成时则状态又变为可见，所以在元素等待中以下这些方式是经常被用到的。
```python
# 元素等待直到能定位到元素，这里是定位到搜索输入框
# 这里定位时不关心元素是否可见，只要是加载到DOM中能定位到即可
WebDriverWait(driver,3).until(ECS.presence_of_element_located((By.ID,'kw')))

# 元素等待直到能定位到可见的元素，比如这里定位到搜索按钮
# 和上面不同，这里等待的元素除加载DOM中，也必须可见
WebDriverWait(driver,3).until(ECS.visibility_of_element_located((By.ID,'su')))

# 元素等待直到能定位到可见的元素，比如这里定位到搜索框
# 这个方法和上面 `visibility_of_element_located` 使用类似
WebDriverWait(driver,3).until(ECS.visibility_of((By.ID,value='su')))

# 元素等待直到通过 `CSS` 定位到至少有一个元素存在，则结束等待
WebDriverWait(driver,3).until(ECS.presence_of_all_elements_located((By.CSS_SELECTOR,'.uinv')))

# 这和上一个等待方法类似，主要就是需要元素可见才会结束等待
WebDriverWait(driver,3).until(ECS.visibility_of_any_elements_located((By.CSS_SELECTOR,'.uinv')))

# 元素等待直到元素被加载，可以定位到并且元素必须为不可见状态，才会结束等待
WebDriverWait(driver,3).until(ECS.invisibility_of_element_located((By.CSS_SELECTOR,'#nwWrap')))
```

- 在平时使用中，也经常会用到等待的元素包含某些内容来判断，而且这种方式能适用很多场景
```python
# 元素等待直到定位到指定的元素，并且元素的属性值中包含指定的字符串
WebDriverWait(driver,3).until(ECS.text_to_be_present_in_element((By.XPATH,"//*[@id='bottom_layer']/div/p[1]/a"),'关于百度'))

# 这种实现跟上面效果差不多
WebDriverWait(driver,3).until(ECS.text_to_be_present_in_element_value((By.CSS_SELECTOR,'#su'),'百度一下'))
```

- 下面几种方式主要是针对下拉框类型的等待，还有可点击的组件，如超链接
```python
# 元素等待直到元素被加载，为可见状态，并且是可点击的状态，才会结束等待
WebDriverWait(driver,3).until(ECS.element_to_be_clickable((By.XPATH,"//*[@id='bottom_layer']/div/p[1]/a"))).click()

# 元素等待直到元素被加载，可见状态并且这个元素是被选中的状态
# 这种方式我运用于下拉选择框
WebDriverWait(driver,3).until(ECS.element_to_be_selected((By.XPATH,"//*[@id='sel']/option[2]")))

# 元素等待直到元素被加载后，并且元素选中状态为预期的状态，才会结束等待
WebDriverWait(driver,3).until(ECS.element_selection_state_to_be((By.XPATH,"//*[@id='sel']/option[1]"),False))
```

- 以下是比较特殊的几种方式，在某些情况下用处很大
```python
# 元素等待直到页面加载出现 `alert`，出现自己切换过去并将提示内容返回
WebDriverWait(driver,3).until(ECS.alert_is_present())

# 其他都是等待加载元素并操作，这种是等待元素被从Dom中移除时，才会触发结束等待
WebDriverWait(driver,3).until(ECS.staleness_of(driver.find_element(By.ID,'su')))

# 这种是页面有多个Frame时，等待页面加载直到 `frame1` 可以切换进去，这时就会自动切换到 `frame1` ，然后结束等待
WebDriverWait(driver,3).until(ECS.frame_to_be_available_and_switch_to_it(frame1))
```
除了上面这些，还有很多其他的方式有兴趣可以自己去研究一下，然后写些小例子实际使用下。<br />如果隐性等待和显性等待都使用了，那它会按哪种方式等待呢？在这里说下，它会按等待时间较长的设置为最终等待时间。
