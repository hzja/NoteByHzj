Python 类<br />Python 的类中，所有以双下划线`__`包起来的方法，叫魔术方法，魔术方法在类或对象的某些事件发出后可以自动执行，让类具有神奇的魔力，比如常见的构造方法`__new__`、初始化方法`__init__`、析构方法`__del__`，今天来聊一聊`__new__`的妙用，主要分享以下几点：

- `__new__` 和 `__init__` 的区别
- 应用1：改变内置的不可变类型
- 应用2：实现一个单例
- 应用3：客户端缓存
- 应用4：不同文件不同的解密方法
- 应用5：Metaclasses
<a name="yJ8bJ"></a>
### `__new__` 和 `__init__` 的区别
1、调用时机不同：new 是真正创建实例的方法，init 用于实例的初始化，new 先于 init 运行。<br />2、返回值不同，new 返回一个类的实例，而 init 不返回任何信息。<br />3、new 是 class 的方法，而 init 是对象的方法。<br />示例代码：
```python
class A:
    def __new__(cls, *args, **kwargs):
        print("new", cls, args, kwargs)
        return super().__new__(cls)

    def __init__(self, *args, **kwargs):
        print("init", self, args, kwargs)


def how_object_construction_works():
    x = A(1, 2, 3, x=4)
    print(x)    
    print("===================")
    x = A.__new__(A, 1, 2, 3, x=4)
    if isinstance(x, A):
        type(x).__init__(x, 1, 2, 3, x=4)
    print(x)

if __name__ == "__main__":
    how_object_construction_works()
```
上述代码定义了一个类 A，在调用 `A(1, 2, 3, x=4)` 时先执行 new，再执行 init，等价于：
```python
x = A.__new__(A, 1, 2, 3, x=4)
if isinstance(x, A):
    type(x).__init__(x, 1, 2, 3, x=4)
```
代码的运行结果如下：
```python
new <class '__main__.A'> (1, 2, 3) {'x': 4}
init <__main__.A object at 0x7fccaec97610> (1, 2, 3) {'x': 4}
<__main__.A object at 0x7fccaec97610>
===================
new <class '__main__.A'> (1, 2, 3) {'x': 4}
init <__main__.A object at 0x7fccaec97310> (1, 2, 3) {'x': 4}
<__main__.A object at 0x7fccaec97310>
```
new 的主要作用就是让程序员可以自定义类的创建行为，以下是Python 的构造方法 `__new__` 的常用场景：
<a name="hVFSG"></a>
### 应用1：改变内置的不可变类型
元组是不可变类型，但是继承 tuple ，然后可以在 new 中，对其元组的元素进行修改，因为 new 返回之前，元组还不是元组，这在 init 函数中是无法实现的。比如说，实现一个大写的元组，代码如下：
```python
class UppercaseTuple(tuple):
    def __new__(cls, iterable):
        upper_iterable = (s.upper() for s in iterable)
        return super().__new__(cls, upper_iterable)

    # 以下代码会报错，初始化时是无法修改的
    # def __init__(self, iterable):
    #     print(f'init {iterable}')
    #     for i, arg in enumerate(iterable):
    #         self[i] = arg.upper()

if __name__ == '__main__':
    print("UPPERCASE TUPLE EXAMPLE")
    print(UppercaseTuple(["hello", "world"]))

# UPPERCASE TUPLE EXAMPLE
# ('HELLO', 'WORLD')
```
<a name="QUlG5"></a>
### 应用2：实现一个单例
```python
class Singleton:
    _instance = None

    def __new__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__new__(cls, *args, **kwargs)
        return cls._instance


if __name__ == "__main__":

    print("SINGLETON EXAMPLE")
    x = Singleton()
    y = Singleton()
    print(f"{x is y=}")
# SINGLETON EXAMPLE
# x is y=True
```
<a name="FV7z2"></a>
### 应用3：客户端缓存
当客户端的创建成本比较高时，比如读取文件或者数据库，可以采用以下方法，同一个客户端属于同一个实例，节省创建对象的成本，这本质就是多例模式。
```python
class Client:
    _loaded = {}
    _db_file = "file.db"

    def __new__(cls, client_id):
        if (client := cls._loaded.get(client_id)) is not None:
            print(f"returning existing client {client_id} from cache")
            return client
        client = super().__new__(cls)
        cls._loaded[client_id] = client
        client._init_from_file(client_id, cls._db_file)
        return client

    def _init_from_file(self, client_id, file):
        # lookup client in file and read properties
        print(f"reading client {client_id} data from file, db, etc.")
        name = ...
        email = ...
        self.name = name
        self.email = email
        self.id = client_id


if __name__ == '__main__':
    print("CLIENT CACHE EXAMPLE")
    x = Client(0)
    y = Client(0)
    print(f"{x is y=}")
    z = Client(1)
# CLIENT CACHE EXAMPLE
# reading client 0 data from file, db, etc.
# returning existing client 0 from cache
# x is y=True
# reading client 1 data from file, db, etc.
```
<a name="fxXFc"></a>
### 应用4：不同文件不同的解密方法
先在脚本所在目录创建三个文件：plaintext_hello.txt、rot13_hello.txt、otp_hello.txt，程序会根据不同的文件选择不同的解密算法
```python
import codecs
import itertools


class EncryptedFile:
    _registry = {}  # 'rot13' -> ROT13Text

    def __init_subclass__(cls, prefix, **kwargs):
        super().__init_subclass__(**kwargs)
        cls._registry[prefix] = cls

    def __new__(cls, path: str, key=None):
        prefix, sep, suffix = path.partition(":///")
        if sep:
            file = suffix
        else:
            file = prefix
            prefix = "file"
        subclass = cls._registry[prefix]
        obj = object.__new__(subclass)
        obj.file = file
        obj.key = key
        return obj

    def read(self) -> str:
        raise NotImplementedError


class Plaintext(EncryptedFile, prefix="file"):
    def read(self):
        with open(self.file, "r") as f:
            return f.read()


class ROT13Text(EncryptedFile, prefix="rot13"):
    def read(self):
        with open(self.file, "r") as f:
            text = f.read()
        return codecs.decode(text, "rot_13")


class OneTimePadXorText(EncryptedFile, prefix="otp"):
    def __init__(self, path, key):
        if isinstance(self.key, str):
            self.key = self.key.encode()

    def xor_bytes_with_key(self, b: bytes) -> bytes:
        return bytes(b1 ^ b2 for b1, b2 in zip(b, itertools.cycle(self.key)))

    def read(self):
        with open(self.file, "rb") as f:
            btext = f.read()
        text = self.xor_bytes_with_key(btext).decode()
        return text


if __name__ == "__main__":
    print("ENCRYPTED FILE EXAMPLE")
    print(EncryptedFile("plaintext_hello.txt").read())
    print(EncryptedFile("rot13:///rot13_hello.txt").read())
    print(EncryptedFile("otp:///otp_hello.txt", key="1234").read())
# ENCRYPTED FILE EXAMPLE
# plaintext_hello.txt
# ebg13_uryyb.gkg
# ^FCkYW_X^GL
```
