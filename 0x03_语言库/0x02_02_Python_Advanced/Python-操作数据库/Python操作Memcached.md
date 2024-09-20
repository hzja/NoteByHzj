Python 数据库 Memcached
<a name="WIIMv"></a>
## 1、前言
Memcached：一款高性能分布式内存对象缓存系统，通过 内存缓存，以减少数据库的读取，从而分担数据库的压力，进而提高网站的加载速度<br />Memcached，实际上是一套简洁的键值对存储系统，可以存储各种类型的数据，包含：字符串、对象、图像、文件、视频等<br />由于 Memcached 的数据存储在内存中，因此重启服务或系统之后，数据会全部丢失；另外，当 Memcached 使用容量达到指定值时，会基于 LRU 算法，自动删除掉不使用的缓存<br />Memcached 协议简洁而强大，开发方便快捷，兼容大部分的开发语言
<a name="M1jkB"></a>
## 2、准备
以在云服务器 Centos 7.8 安装 Memcached-Server 为例<br />首先，在云服务器上安装 Memcached
```bash
# 使用yum安装memcached
yum install memcached
```
通过 `whereis` 命令，查询出 Memcached 安装的目录
```bash
# 使用whereis命令查询memcached安装目录
# /usr/bin/memcached
[root@VM-16-11-centos ~]# whereis memcached
memcached: /usr/bin/memcached /usr/share/man/man1/memcached.1.gz
```
然后，通过命令行及参数启动 Memcached 服务
```bash
# 运行memcached服务
# -p：端口号
# -m：分类的内存
# -d：守护进程，后台运行
# -u：运行memcached的用户
# -l：设置哪些ip可以连接到服务，0.0.0.0：容许外网ip访问
/usr/bin/memcached -p 11211 -m 64m -d -u root -l 0.0.0.0
```
常见的启动参数包含：

- `-d`：通过守护进程，在后台运行
- `-m`：分配的最大内存，默认大小为 64 M
- `-u`：运行 Memcached 的用户
- `-l`：设置可以访问 Memecache 服务的 ip 地址，默认只能通过本机访问；如果想外网进行访问，需要设置为：0.0.0.0
- `-p`：设置 Memcached 监听的端口号，默认为 111211
- `-c`：最大运行的并发连接数，默认为 1024

接着，开启防火墙端口<br />需要注意的是，如果是云服务器，需要在安全组中开放相应端口号
```bash
# 开启端口号
firewall-cmd --add-port=11211/tcp --permanent

# 重启立即生效
firewall-cmd --reload
```
完成以上操作，有两种方式操作数据<br />分别是：<br />1、通过 `telnet` 连接 Memecached 服务，使用命令行去操作
```bash
# 连接
telnet IP地址 11211
```
2、通过编程语言，包含：Python、Java、Php，去操作 Memcached 数据<br />以 Python 为例，需要安装 python-memcached 依赖
```bash
# 安装依赖
pip3 install python-memcached
```
<a name="N9u13"></a>
## 3、实战
在操作 Memcache 中的数据之前，需要导入 memcache，使用 `Client()` 方法指定要操作的 Memecache 服务，构建一个数据连接对象
```python
def __init__(self):
    # 连接memcached服务，可以包含多个服务ip
    self.mc = memcache.Client(['ip:11211'], debug=True)
```
接下来以新增、查询、更新、追加、删除、为例，讲讲 Python 操作这些数据的方法
<a name="nr04Q"></a>
### 1、新增操作
新增操作对应的方法是：`add(key,value,timeout)`<br />其中，参数 timeout 表示数据保留的时间，超时会自动被清除掉<br />需要注意的是，如果准备插入键值对中的 key 在原数据集中不存在，则会新增一条记录到数据集中；否则，会添加失败，并警告提示
```python
def __add(self):
    """
    添加记录
    :return:
    """
    # 添加一条数据，如果已经存在，会添加失败，并提出异常
    # time：保存时间，超时会被清除，单位为秒，默认是保存永久
    # 注意：如果添加的key已经存在，会警告：MemCached: while expecting 'STORED', got unexpected response 'NOT_STORED'
    self.mc.add("name", "xag", time=60 * 5)
    self.mc.add("age", 23)
```
<a name="lWMfD"></a>
### 2、查询操作
Memcached 查询操作分为：

- 通过 Key 去查询单条记录
- 通过多个 Key 组成的列表去查询多条记录

其中，单条记录查询对应的方法是：`get(key)`
```python
def __query_one(self, key):
    """
    查询单条记录
    :return:
    """
    result = self.mc.get(key)
    print('单条记录查询,key:', key, ",value:", result)
```
多条记录的查询，对应的方法是：`get_multi(keys)`
```python
def __query_many(self, keys):
    """
    查询多条记录
    :param keys: 键值列表
    :return:
    """
    values = self.mc.get_multi(keys)

    # dict，键值对
    print(type(values))
    print('多条记录查询:', values)
```
<a name="lqkwV"></a>
### 3、更新操作
更新操作包含三种方法，分别是：

- 更新一条记录，如果键不存在，则更新失败对应方法是：`replace(key,value)`
- 更新一条记录，如果键不存在，则新增一条记录对应方法是：`set(key,value)`
- 更新多条记录，如果其中有不存在的键，则将对应的键值对新增到数据集中对应方法：`set_multi({key1:value1...})`

具体实例代码如下：
```python
def __update_default(self):
    """
    更新数据，包含：replace、set、set_multi
    :return:
    """
    # 1、使用replace更新一条记录
    # self.mc.replace("name","xag1")
    # self.__query_one("name")

    # 2、使用set更新记录，如果key不存在，则添加一条记录
    # set = replace + add
    # self.mc.set("name", "xag2")
    # self.__query_one("name")

    # 3、使用set_multi更新多条记录，如果其中有不存在的key，则新建一个键值对
    self.mc.set_multi({"name": "xag3", "age": 28})
    self.__query_many(["name", "age"])
```
<a name="bAAnX"></a>
### 4、追加操作
追加操作相当于修改某一个键的值，在头部或者尾部追加数据<br />其中：

- `append(str)`：在尾部追加一段数据
- `prepend(str)`：在头部新增一段数据

实践代码如下：
```python
def __update_append(self):
    """
    追加更新，包含：append、prepend
    :return:
    """

    # 1、append
    # value尾部追加内容
    self.mc.append("name","我是新增的内容")
    self.__query_one("name")

    # 2、prepend
    # 头部新增内容
    self.mc.prepend("name","头部新增的内容")
    self.__query_one("name")
```
<a name="pVZRy"></a>
### 5、删除操作
和查询操作类似，删除操作同样支持单个键值对的删除和多个键值对的删除

- `delete(key)`：删除某一个键值对
- `delete_multi(keys)`：删除列表中所有的 Key 对应的所有键值对

对应的操作代码如下：
```python
def __delete(self):
    """
    删除数据，包含：delete、delete_multi
    :return:delete_multi
    """
    # 1、delete
    # 删除单条键值对
    # self.mc.delete("age")
    # self.__query_one("age")

    # 2、delete_multi
    # 删除多条记录
    keys = ["name","age"]
    self.mc.delete_multi(keys)
    self.__query_many(keys)
```
