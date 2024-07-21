property 是Python中的一个内置装饰器，它用于创建属性并允许开发者定义特定的操作，例如获取（getter）、设置（setter）和删除（deleter）属性值的方法。这种特性使得我们能更好地管理类的属性。
<a name="fqoe3"></a>
## 1、创建只读属性
使用 property 可以创建只读属性，即只允许获取值，不允许修改。
```python
class MyClass:
    def __init__(self, _value):
        self._value = _value

    @property
    def value(self):
        return self._value

obj = MyClass(10)
print(obj.value)  # 输出：10

# 试图修改只读属性会引发 AttributeError
# obj.value = 20
```
<a name="rTQjK"></a>
## 2、创建可读写属性
通过 `@property` 装饰器的 `@value.setter` 方法，可以定义属性的设置方式。
```python
class MyClass:
    def __init__(self, _value):
        self._value = _value

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, new_value):
        self._value = new_value

obj = MyClass(10)
print(obj.value)  # 输出：10

obj.value = 20
print(obj.value)  # 输出：20
```
<a name="acgup"></a>
## 3、创建可删除属性
通过 `@property` 装饰器的 `@value.deleter` 方法，可以定义属性的删除方式。
```python
class MyClass:
    def __init__(self, _value):
        self._value = _value

    @property
    def value(self):
        return self._value

    @value.deleter
    def value(self):
        del self._value

obj = MyClass(10)
print(obj.value)  # 输出：10

del obj.value
# 试图访问已删除的属性会引发 AttributeError
# print(obj.value)
```
<a name="i0p4C"></a>
## 4、实际应用 - 数据验证
property 还可以用于数据验证。例如，确保属性值符合特定条件。
```python
class Temperature:
    def __init__(self, celsius):
        self._celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature below absolute zero is not possible.")
        self._celsius = value

temp = Temperature(25)
print(temp.celsius)  # 输出：25

temp.celsius = -300  # 引发 ValueError
```
<a name="SfDDs"></a>
## 总结
property 特性为Python中的属性管理提供了灵活性和可控性。通过其设置、获取和删除方法，能够精细地控制属性的行为。这使得属性操作更加安全和可靠，并能够确保数据的完整性和合法性。深入理解和灵活运用 property 特性可以更好地管理类的属性，确保程序的健壮性和稳定性。
