<a name="R6lkr"></a>
## 1、使用 Pandas 进行数据分析
[**Pandas**](https://pandas.pydata.org/)是一个强大的数据分析和处理库。只需几行代码，就可以从 CSV 文件或数据库等各种来源读取、清理和分析数据。下面是一个示例脚本：
```python
import pandas as pd

# Read data from a CSV file
data = pd.read_csv('data.csv')

# Perform basic analysis
mean = data['column_name'].mean()
print(f"Mean: {mean}")
```
<a name="zUSNS"></a>
## 2、使用 BeautifulSoup 进行网络抓取
[**BeautifulSoup**](https://www.crummy.com/software/BeautifulSoup/)是一个用于网络抓取的 Python 库。它能让你轻松地从网站中提取数据。下面是一个简单的网络抓取脚本：
```python
import requests
from bs4 import BeautifulSoup

url = 'https://example.com'
response = requests.get(url)
soup = BeautifulSoup(response.text, 'html.parser')

# Extract data from the webpage
data = soup.find('div', class_='content')
print(data.text)
```
<a name="rhtOq"></a>
## 3、文件重命名器
当需要根据特定条件重命名文件夹中的多个文件时，这个脚本非常方便。例如，可以在文件名中添加前缀、后缀或替换文本。
```python
import os

folder_path = '/path/to/folder'
for filename in os.listdir(folder_path):
    if filename.startswith('prefix_'):
        new_filename = filename.replace('prefix_', 'new_prefix_')
        os.rename(os.path.join(folder_path, filename), os.path.join(folder_path, new_filename))
```
<a name="jSmN8"></a>
## 4、使用 Pillow 调整图像大小
[**Pillow**](https://python-pillow.org/)是一个简化图像处理的 Python 图像库。该脚本可将一批图像的大小调整为指定的分辨率或长宽比：
```python
from PIL import Image
import os

input_folder = '/path/to/images'
output_folder = '/path/to/resized_images'
desired_size = (100, 100)

for filename in os.listdir(input_folder):
    with Image.open(os.path.join(input_folder, filename)) as img:
        img.thumbnail(desired_size)
        img.save(os.path.join(output_folder, filename))
```
<a name="AkbgQ"></a>
## 5、使用 ReportLab 创建 PDF
[**ReportLab**](https://www.reportlab.com/)是一个用 Python 创建 PDF 文档的库。可以从文本或 HTML 内容生成 PDF 文件。下面是一个基本示例：
```python
from reportlab.pdfgen import canvas

pdf_file = 'output.pdf'
text = 'Hello, this is a sample PDF.'

c = canvas.Canvas(pdf_file)
c.drawString(100, 750, text)
c.save()
```
<a name="MMNGq"></a>
## 6、使用 smtplib 自动发送电子邮件
需要自动发送电子邮件？Python 的 smtplib 库可以提供帮助。该脚本可通过编程发送电子邮件：
```python
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

smtp_server = 'smtp.example.com'
sender_email = 'your_email@example.com'
receiver_email = 'recipient@example.com'
password = 'your_password'

message = MIMEMultipart()
message['From'] = sender_email
message['To'] = receiver_email
message['Subject'] = 'Sample Email Subject'

body = 'This is a sample email message.'
message.attach(MIMEText(body, 'plain'))

with smtplib.SMTP(smtp_server, 587) as server:
    server.starttls()
    server.login(sender_email, password)
    server.sendmail(sender_email, receiver_email, message.as_string())
```
<a name="kXPXY"></a>
## 7、数据备份脚本
自动备份文件和目录，确保数据安全：
```python
import shutil

source_folder = '/path/to/source_folder'
backup_folder = '/path/to/backup_folder'

shutil.copytree(source_folder, backup_folder)
```
<a name="uWzGw"></a>
## 8、密码生成器
生成强大的随机密码，提高安全性：
```python
import random
import string

def generate_password(length=12):
    characters = string.ascii_letters + string.digits + string.punctuation
    return ''.join(random.choice(characters) for _ in range(length))

password = generate_password()
print(password)
```
<a name="YFpgT"></a>
## 9、简单Web服务器
创建一个用于测试和开发的基本 HTTP 服务器：
```python
import http.server
import socketserver

port = 8000

with socketserver.TCPServer(('', port), http.server.SimpleHTTPRequestHandler) as httpd:
    print(f"Serving at port {port}")
    httpd.serve_forever()
```
<a name="oTVjP"></a>
## 10、使用 SQLite 备份和还原数据库
SQLite 是一个 C 库，它提供了一个基于磁盘的轻量级数据库，不需要单独的服务器进程，并允许使用 SQL 查询语言的非标准变体访问数据库。一些应用程序可以使用 SQLite 进行内部数据存储。也可以使用 SQLite 制作应用程序原型，然后将代码移植到 PostgreSQL 或 Oracle 等大型数据库。<br />下面，将提供用 Python 备份和还原 SQLite 数据库的示例代码，这是一种轻量级的常用数据库系统：
```python
import sqlite3
import shutil

# Database file paths
source_db_file = 'source.db'
backup_db_file = 'backup.db'

# Function to create a backup of the SQLite database
def backup_database():
    try:
        shutil.copy2(source_db_file, backup_db_file)
        print("Backup successful.")
    except Exception as e:
        print(f"Backup failed: {str(e)}")

# Function to restore the SQLite database from a backup
def restore_database():
    try:
        shutil.copy2(backup_db_file, source_db_file)
        print("Restore successful.")
    except Exception as e:
        print(f"Restore failed: {str(e)}")

# Usage
while True:
    print("Options:")
    print("1. Backup Database")
    print("2. Restore Database")
    print("3. Quit")
    choice = input("Enter your choice (1/2/3): ")

    if choice == '1':
        backup_database()
    elif choice == '2':
        restore_database()
    elif choice == '3':
        break
    else:
        print("Invalid choice. Please enter 1, 2, or 3.")
```
在此代码中

1. `backup_database()`函数复制 SQLite 数据库源文件并将其命名为备份文件。可以运行此函数来创建数据库备份。
2. `restore_database()` 函数将备份文件复制回源文件，有效地将数据库恢复到创建备份时的状态。
3. 用户可选择备份数据库、恢复数据库或退出程序。
4. 可以调整 source_db_file 和 backup_db_file 变量，指定 SQLite 源文件和备份数据库文件的路径。
<a name="HJsYo"></a>
## 参考资料
Pandas: [_https://pandas.pydata.org/_](https://pandas.pydata.org/)<br />BeautifulSoup: [_https://www.crummy.com/software/BeautifulSoup/_](https://www.crummy.com/software/BeautifulSoup/)<br />Pillow: [_https://python-pillow.org/_](https://python-pillow.org/)<br />ReportLab: [_https://www.reportlab.com/_](https://www.reportlab.com/)
