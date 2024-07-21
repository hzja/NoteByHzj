Python<br />用 Python 连接到 MySQL 数据库的方法不是很系统，实际中有几种不同的连接方法，而且不是所有的方法都能与不同的操作系统很好地配合。除此之外，启用SSL也是一个挑战。<br />本文涵盖了四种方法，可以用它们来连接Python应用程序和MySQL。这些例子将使用PlanetScale，一个用于MySQL的无服务器数据库平台。<br />本文将涵盖以下连接方法：

- mysqlclient
- mysql-connector-python
- PyMySQL
- aiomysql
<a name="J5uAg"></a>
## 设置PlanetScale
如果需要一个免费的数据库来测试连接，PlanetScale是一个很好的选择。PlanetScale建立在Vitess之上，为你提供一个熟悉的工作流程分支来测试和部署不同模式。<br />Vitess建于2010年，用于解决YouTube的扩展问题。从那时起，这个开源项目持续增长，现在帮助一些公司，如Slack和Square处理他们的大规模数据扩展需求。<br />可以从下面注册一个账户，获得一个免费的5GB数据库。<br />[https://auth.planetscale.com/sign-up](https://auth.planetscale.com/sign-up)
<a name="bsRQ6"></a>
## 创建一个数据库
当进入PlanetScale的仪表板。

- 在你的组织概览仪表板上点击 "创建新数据库"。
- 给你的数据库起个名字。
- 从下拉菜单中选择一个区域。
- 点击创建数据库。

你的数据库将被创建为一个主要的开发分支。可以在这里进行模式修改，一旦准备好了，就可以把它推广到生产中。
<a name="k9irS"></a>
## 获取连接凭证
要生成连接凭证，必须做到以下步骤：

- 在你的数据库概览页面上点击 "连接"。
- 从 "连接方式 "下拉菜单中选择 "Python"。
- 将凭证复制到你的Python应用程序文件夹中的.env文件。

为了确保连接的安全性，PlanetScale使用了来自证书颁发机构(CA)的证书，它是几乎所有平台上的系统根的一部分。当连接到数据库时，你必须指定你信任的CA证书的路径。注意，这个路径取决于你的平台。这里有一个跨越各种操作系统和发行版的常见路径列表。<br />[https://planetscale.com/docs/concepts/secure-connections#ca-root-configuration](https://planetscale.com/docs/concepts/secure-connections#ca-root-configuration)<br />如果你从PlanetScale仪表板上复制了证书，你已经完成这一步，因为PlanetScale会自动检测你的操作系统。<br />如果你使用的是Windows系统，你需要下载根证书，然后用你所使用的驱动程序的正确配置选项指向它们的位置。
<a name="LqasV"></a>
## 用mysqlclient连接到MySQL
mysqlclient包是用于MySQL的最流行的Python包之一。它包含MySQLdb模块，一个提供Python数据库API的MySQL接口。<br />首先安装mysqlclient。你可以通过两种方式来完成这件事。

- 使用来自MySQL官方网站的安装程序，它与你正在运行的操作系统和SQL版本相匹配。
- 使用 pip 来安装它。
```bash
pip install mysqlclient
```
在Linux上，可能需要在安装mysqlclient之前安装Python3和MySQL开发头文件和库。
```bash
sudo apt-get install python3-dev default-libmysqlclient-dev build-essential
```
一旦安装了mysqlclient，可以使用下面的代码连接到数据库。
```python
import os
import MySQLdb # import the MySQLdb module
from dotenv import load_dotenv
load_dotenv()

# Create the connection object
connection = MySQLdb.connect(
    host=os.getenv("HOST"),
    user=os.getenv("USERNAME"),
    passwd=os.getenv("PASSWORD"),
    db=os.getenv("DATABASE"),
    ssl_mode="VERIFY_IDENTITY",
    ssl={
        'ca': os.getenv("SSL_CERT")
    }
)

# Create cursor and use it to execute SQL command
cursor = connection.cursor()
cursor.execute("select @@version")
version = cursor.fetchone()

if version:
    print('Running version: ', version)
else:
    print('Not connected.')
```
注意，必须安装 python-dotenv 并导入 os 模块以访问.env 文件中的数据库凭证。<br />Windows用户要注意，从wheel轮子文件中安装的mysqlclient不支持ssl模式。由于你想确保数据库连接是安全的，建议使用另一个驱动，比如mysql-connector-python。<br />这个例子使用连接来获取数据库的SQL版本，然后关闭连接。<br />其余的连接方法的语法与这个例子几乎相同。这种类似的语法源于 Python 数据库 API 规范 (PEP 249)，它鼓励用于访问数据库的 Python 模块之间的一致性。
<a name="chRgc"></a>
## 用MySQL connector连接到MySQL
MySQL connector/Python模块是Oracle支持的官方驱动，用于通过Python连接MySQL。该连接器完全是Python语言，而mysqlclient是用C语言编写的。它也是独立的，意味着它不需要MySQL客户端库或标准库以外的任何Python模块。<br />注意，MySQL Connector/Python不支持旧的MySQL服务器认证方法，这意味着4.1之前的MySQL版本不能工作。<br />首先安装mysql-connector-python模块。建议使用pip来安装它。
```bash
pip install mysql-connector-python
```
安装后，使用以下代码连接到MySQL：
```python
import os
from dotenv import load_dotenv
from mysql.connector import Error
import mysql.connector

load_dotenv()

connection = mysql.connector.connect(
host=os.getenv("HOST"),
database=os.getenv("DATABASE"),
user=os.getenv("USERNAME"),
password=os.getenv("PASSWORD"),
ssl_ca=os.getenv("SSL_CERT")
)

try:
    if connection.is_connected():
        cursor = connection.cursor()
    cursor.execute("select @@version ")
    version = cursor.fetchone()
    if version:
        print('Running version: ', version)
    else:
        print('Not connected.')
except Error as e:
    print("Error while connecting to MySQL", e)
finally:
    connection.close()
```
<a name="KUkoS"></a>
## 用PyMySQL连接到MySQL
PyMySQL包是另一个连接器，可以用它来连接Python和MySQL。如果追求速度，这是一个很好的选择，因为它比mysql-connector-python快。<br />可以使用 pip 来安装它。
```bash
pip install PyMySQL
```
然后，使用以下连接代码。
```python
from dotenv import load_dotenv
import pymysql
import os
load_dotenv()
connection = pymysql.connect(
    host=os.getenv("HOST"),
    database=os.getenv("DATABASE"),
    user=os.getenv("USERNAME"),
    password=os.getenv("PASSWORD"),
    ssl_ca=os.getenv("SSL_CERT")
)
cursor = connection.cursor()
cursor.execute("select @@version ")
version = cursor.fetchone()
if version:
    print('Running version: ', version)
else:
    print('Not connected.')
connection.close()
```
<a name="quxBG"></a>
## 用aiomysql连接到MySQL
aiomysql库用于从asyncio框架访问MySQL数据库。除了是异步的特性之外，连接代码与PyMySQL相似。注意，使用aiomysql需要Python 3.7以上版本和PyMySQL。<br />要使用aiomysql，首先要用安装异步模块。
```bash
pip install asyncio
```
然后，用以下方法安装aiomysql。
```bash
pip install aiomysql
```
然后可以使用以下代码连接到MySQL。
```python
import os
import asyncio
import aiomysql
import ssl

from dotenv import load_dotenv
load_dotenv()

ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
ctx.load_verify_locations(cafile=os.getenv("SSL_CERT"))

loop = asyncio.get_event_loop()

async def connect_db():
   connection = await aiomysql.connect(
       host=os.getenv("HOST"),
       port=3306,
       user=os.getenv("USERNAME"),
       password=os.getenv("PASSWORD"),
       db=os.getenv("DATABASE"),
       loop=loop,
       ssl=ctx
   )
   cursor = await connection.cursor()
   await cursor.execute("select @@version")
   version = await cursor.fetchall()
   print('Running version: ', version)
   await cursor.close()
   connection.close()
loop.run_until_complete(connect_db())
```
<a name="zsjDc"></a>
## 总结
在数据库中保存数据是应用程序的共同特征之一。像Python一样，大多数编程语言支持与不同数据库的连接和交互。
