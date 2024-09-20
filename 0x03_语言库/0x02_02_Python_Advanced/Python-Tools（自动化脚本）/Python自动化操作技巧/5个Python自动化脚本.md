Python
<a name="dXnS3"></a>
### 从JSON数据转化CSV文件
下面的这个Python脚本能够将JSON数据转化到CSV文件的表格当中去，输入的是带有.json后缀的文件，输出的是.csv后缀的表格文件，代码如下
```python
import json

def converter(input_file, output_file):
    try:
        with open(input_file, 'r') as f:
            data = json.loads(f.read())

        output = ','.join([*data[0]])
        for obj in data:
            output += f'\n{obj[字段名1]},{obj[字段名2]},{obj[字段名3]}'

        with open(output_file, 'w') as f:
            f.write(output)
    except Exception as ex:
        print(f'Error: {str(ex)}')
```
<a name="waaH1"></a>
### 密码生成器
有时候所设想的密码会太过于简单，也有时候可能压根就不知道该如何设置密码才会足够的安全，那么下面的这个Python脚本可能会派的上用场，代码如下
```python
import random
import string

total = string.ascii_letters + string.digits + string.punctuation

length = 指定的密码的长度
password = "".join(random.sample(total, length))
```
主要是调用了Python当中的`random`和`string`模块来生成指定长度的密码
<a name="oHbVa"></a>
### 给照片添加水印
有时候不希望自己制作的照片随意被别人盗用，所以想要在照片上添加上水印，下面的这几行代码倒是可以派上用场
```python
def watermark_photo(input_image_path,watermark_image_path,output_image_path):
    base_image = Image.open(input_image_path)
    watermark = Image.open(watermark_image_path).convert("RGBA")
    # 添加水印照片
    position = base_image.size
    newsize = (int(position[0]*8/100),int(position[0]*8/100))
    watermark = watermark.resize(newsize)

    new_position = position[0]-newsize[0]-20,position[1]-newsize[1]-20
    # 创建一个全新的空图片
    transparent = Image.new(mode='RGBA',size=position,color=(0,0,0,0))
    # 将原先的图片复制粘贴过去
    transparent.paste(base_image,(0,0))
    # 将水印图片复制过去
    transparent.paste(watermark,new_position,watermark)
    image_mode = base_image.mode
    if image_mode == 'RGB':
        transparent = transparent.convert(image_mode)
    else:
        transparent = transparent.convert('P')
    transparent.save(output_image_path,optimize=True,quality=100)
```
<a name="HxPpf"></a>
### 电脑电量不足提醒
下面这个脚本的作用在于当电脑的电量不足，并且没有在充电的时候，会有提示框弹出提醒充电，代码如下
```python
import psutil
from pynotifier import Notification

battery = psutil.sensors_battery()
plugged = battery.power_plugged
percent = battery.percent

if percent <= 20 and plugged != True:

    Notification(
        title="Battery Low",
        description=str(percent) + "% Battery remain!!",
        duration=5,  # Duration in seconds
    ).send()
```
<a name="MsGtq"></a>
### 网站截图
有时候需要对整个网站进行截图保存，下面的这个代码可以派得上很大的用场，
```python
from selenium import webdriver
from selenium.webdriver.chrome.options import Options


options = Options()
options.add_argument('--headless')
driver = webdriver.Chrome(options=options)

url = "https://www.baidu.com"

try:
    driver.get(url)
    page_width = driver.execute_script('return document.body.scrollWidth')
    page_height = driver.execute_script('return document.body.scrollHeight')
    driver.set_window_size(page_width, page_height)
    driver.save_screenshot('screenshot.png')
    driver.quit()
    print("SUCCESS")

except IndexError:
    print('Usage: %s URL' % url)
```
