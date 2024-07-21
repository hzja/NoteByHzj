<a name="Hv31w"></a>
## 1、批量修改文件扩展名
将指定目录下所有特定扩展名的文件更改为新的扩展名。
```python
import os

def rename_file_extensions(folder_path, old_ext, new_ext):
    for filename in os.listdir(folder_path):
        if filename.endswith(old_ext):
            base = os.path.splitext(filename)[0]
            new_filename = base + new_ext
            os.rename(os.path.join(folder_path, filename), os.path.join(folder_path, new_filename))

# 使用示例：将所有.txt文件改为.md
rename_file_extensions('/path/to/folder', '.txt', '.md')
```
<a name="mKiI2"></a>
## 2、自动创建多个文件夹
快速创建一系列以数字或日期命名的文件夹。
```python
import os

def create_folders(base_path, prefix, count):
    for i in range(1, count + 1):
        os.makedirs(os.path.join(base_path, f"{prefix}{i}"))

# 使用示例：在指定路径下创建10个名为"Folder1"到"Folder10"的文件夹
create_folders('/path/to/base', 'Folder', 10)
```
<a name="d4HZ1"></a>
## 3、下载网络图片
根据提供的URL列表下载图片。
```python
import requests

def download_images(url_list, save_folder):
    for url in url_list:
        img_data = requests.get(url).content
        filename = url.split('/')[-1]
        with open(os.path.join(save_folder, filename), 'wb') as file:
            file.write(img_data)

# 使用示例
url_list = ['http://example.com/image1.jpg', 'http://example.com/image2.jpg']
download_images(url_list, '/path/to/save/folder')
```
<a name="J1GaH"></a>
## 4、简单的文件搜索工具
在指定目录下搜索包含特定文本的文件。
```python
import os

def search_files(directory, text):
    for filename in os.listdir(directory):
        if text in filename:
            print(os.path.join(directory, filename))

# 使用示例：在目录中搜索包含"report"的文件
search_files('/path/to/directory', 'report')
```
<a name="bjLci"></a>
## 5、批量删除特定扩展名的文件
在指定目录中删除所有特定扩展名的文件。
```python
import os

def delete_files_by_extension(folder_path, extension):
    for filename in os.listdir(folder_path):
        if filename.endswith(extension):
            os.remove(os.path.join(folder_path, filename))

# 使用示例：删除所有扩展名为.tmp的文件
delete_files_by_extension('/path/to/folder', '.tmp')
```
<a name="pnUoe"></a>
## 6、CSV文件转换为JSON
将CSV文件的内容转换为JSON格式。
```python
import csv
import json

def csv_to_json(csv_file_path, json_file_path):
    with open(csv_file_path, 'r') as csv_file, open(json_file_path, 'w') as json_file:
        reader = csv.DictReader(csv_file)
        json_data = [row for row in reader]
        json.dump(json_data, json_file, indent=4)

# 使用示例
csv_to_json('/path/to/file.csv', '/path/to/file.json')
```
<a name="UDhA9"></a>
## 7、监控文件夹变化
实时监控指定文件夹的变化（如文件添加、删除、修改）。
```python
import time
import os

def monitor_folder_changes(folder_path):
    initial_files = set(os.listdir(folder_path))
    while True:
        current_files = set(os.listdir(folder_path))
        new_files = current_files - initial_files
        deleted_files = initial_files - current_files
        if new_files:
            print(f"Added: {new_files}")
        if deleted_files:
            print(f"Deleted: {deleted_files}")
        initial_files = current_files
        time.sleep(1)

# 使用示例：监控指定文件夹
monitor_folder_changes('/path/to/monitor')
```
<a name="p7EEm"></a>
## 8、简单的网页内容抓取
抓取网页的标题。
```python
import requests
from bs4 import BeautifulSoup

def fetch_webpage_title(url):
    response = requests.get(url)
    soup = BeautifulSoup(response.content, 'html.parser')
    return soup.title.text

# 使用示例
title = fetch_webpage_title('http://example.com')
print(title)
```
<a name="iXFtB"></a>
## 9、批量修改图片尺寸
更改指定文件夹中所有图片的尺寸。
```python
from PIL import Image
import os

def resize_images(folder_path, output_folder, size):
    for filename in os.listdir(folder_path):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg')):
            img = Image.open(os.path.join(folder_path, filename))
            img = img.resize(size, Image.ANTIALIAS)
            img.save(os.path.join(output_folder, filename))

# 使用示例：将所有图片尺寸修改为500x500
resize_images('/path/to/images', '/path/to/output', (500, 500))
```
<a name="NTY4c"></a>
## 10、批量压缩PDF文件
减小指定目录中所有PDF文件的大小。
```python
from PyPDF2 import PdfFileReader, PdfFileWriter
import os

def compress_pdf(input_folder, output_folder):
    for filename in os.listdir(input_folder):
        if filename.endswith('.pdf'):
            pdf_reader = PdfFileReader(os.path.join(input_folder, filename))
            pdf_writer = PdfFileWriter()

            for page_num in range(pdf_reader.getNumPages()):
                pdf_writer.addPage(pdf_reader.getPage(page_num))

            with open(os.path.join(output_folder, filename), 'wb') as out:
                pdf_writer.write(out)

# 使用示例
compress_pdf('/path/to/pdf/folder', '/path/to/output/folder')
```
这些脚本覆盖了从文件管理、网络操作到图像处理等多种场景，可根据具体需求调整使用，希望你能用在工作场景。
