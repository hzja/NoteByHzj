Python
<a name="ewebJ"></a>
## 1、合并追加到列表声明
从一个简单的开始。不是声明一个空列表然后附加到它，而是直接用所有元素初始化列表。这缩短了代码并使意图更加明确。它的性能也稍微好一些，因为它避免了对 `append()` 的函数调用。
```python
players = []
players.append("Patrick")
players.append("Max")
players.append("Jessi")

# -> refactor
players = ["Patrick", "Max", "Jessi"]
```
这同样适用于填充其他集合类型，如集合和字典。
<a name="YvSjP"></a>
## 2、使用`items()`直接解包字典值
当遍历字典时，需要键和值，那么不要手动访问值。而是迭代`dictionary.items()`，它同时提供键和值。<br />这节省了过去分配给 players 的行，代码现在读起来更自然，重复更少。
```python

teams_by_color = {"blue": ["Patrick", "Jessi"]}

for team_color in teams_by_color:
    players = teams_by_color[team_color]
    if is_winning(team_color):
        advance_level(players)

# -> refactor
for team_color, players in teams_by_color.items():
    if is_winning(team_color):
        advance_level(players)
```
<a name="x60WH"></a>
## 3、将 `range(len)` 替换为枚举
如果需要遍历列表并且需要同时跟踪索引和当前项，请使用内置`enumerate()`函数而不是`range(len)`。这会将当前索引和当前项目作为元组返回。所以可以直接在这里查看值，也可以访问带有索引的项目。
```python
for i in range(len(players)):
    print(i, players[i])

# -> refactor
for i, player in enumerate(players):
    print(i, player)
```
Enumerate 还带有一个可选的start参数。如果使用它，计数器将从该值开始。但请注意，这些项目仍然从第一个开始。
```python
for i, player in enumerate(players, start=1):
    print(i, player)
```
<a name="typAu"></a>
## 4、用枚举调用替换手动循环计数器
这与之前非常相似。有时会看到直接对项目执行迭代的代码——这本身并不坏——但随后需要一个计数器，它会在循环内手动递增。同样在这里可以简单地使用 enumerate 函数。这更简单，也更快。
```python
i = 0
for player in players:
    print(i, player)
    i += 1

# -> refactor
for i, player in enumerate(players):
    print(i, player)
```
<a name="sM6Yl"></a>
### 4.1 不要手动更新计数器
如果只需要计算项目的数量，也不要遍历循环并手动计算所有项目。相反，只需使用`len()`函数来获取列表中的元素数。
```python
num_players = 0
for player in players:
    num_players += 1

# -> refactor
num_players = len(players)
```
<a name="VZlKw"></a>
## 5、将条件简化为return语句
当到达一个方法的末尾并想要返回 True 或 False 时，一种常见的做法是这样的。如果条件为 True，返回 True。否则最后返回 False。然而，直接返回结果更简洁：
```python
def function():
    if isinstance(a, b) or issubclass(b, a):
        return True
    return False

# -> refactor
def function():
    return isinstance(a, b) or issubclass(b, a)
```
在这里应该注意的一件事是，只有当表达式的计算结果为布尔值时才能这样做。`isinstance()`和`issubclass()`都是返回布尔值的函数，所以这很好。但在下一个示例中，第一个表达式_pythonistas_是一个列表而不是布尔值。<br />如果pythonistas是一个有效的非空列表，这将返回列表而不是预期的布尔值，然后可能是应用程序中的错误。因此，为了确保在这里返回一个布尔值，可以将返回包装在对 `bool()`函数的调用中。
```python
def any_pythonistas():
    pythonistas = [coder for coder in coders if is_good_in_python(coder)]
    if pythonistas or self.is_pythonista():
        return True
    return False

# -> refactor
def any_hats():
    pythonistas = [coder for coder in coders if is_good_in_python(coder)]
    return bool(pythonistas or self.is_pythonista())
```
<a name="WIgdo"></a>
## 6、合并条件中的重复块
应该始终寻找机会删除重复的代码。这样做的好地方是if …elif链中有多个相同的块。<br />在此示例中，if 和 elif 都导致相同的执行功能。所以可以使用or组合前两个块来删除对函数的重复调用。现在，如果需要更改`process_standard_payment()`行，可以在一处而不是两处进行。
```python
def process_payment(payment, currency):
    if currency == "USD":
        process_standard_payment(payment)
    elif currency == "EUR":
        process_standard_payment(payment)
    else:
        process_international_payment(payment)

# -> refactor
def process_payment(payment, currency):
    if currency == "USD" or currency == "EUR":
        process_standard_payment(payment)
    else:
        process_international_payment(payment)
```
<a name="BWM6A"></a>
## 7、用`in`运算符替换同一个变量的多次比较
甚至可以进一步重构以前的代码。由于针对多个值重复检查同一个变量，可以使用 in 运算符来缩短它。如果货币值在定义的列表中，将执行专用操作。
```python
def process_payment(payment, currency):
    if currency == "USD" or currency == "EUR":
        process_standard_payment(payment)
    else:
        process_international_payment(payment)

# -> refactor
def process_payment(payment, currency):
    if currency in ["USD", "EUR"]:
        process_standard_payment(payment)
    else:
        process_international_payment(payment)
```
为了再次改进这一点，应该**在这里使用一个集合**。在集合中查找值更快，而且无论如何都想要这里的唯一元素，所以集合是更好的选择。
```python
# -> refactor
def process_payment(payment, currency):
    if currency in {"USD", "EUR"}:
        process_standard_payment(payment)
    else:
        process_international_payment(payment)
```
<a name="dQk9f"></a>
## 8、将 for 循环中的 yield 替换为 yield from
如果已经熟悉生成器，那么这是一个高级技巧。一个经常被忽略的小技巧是 Python 的 yield 关键字对于可迭代对象有一个对应的yield from。<br />如果有一个像列表这样的可迭代对象，而不是说`for item in iterable: yield item`，可以简单地说`yield from iterable`。这更短，并且消除了对可迭代对象的手动循环，这也可以提高性能。
```python
def get_content(entry):
    for block in entry.get_blocks():
        yield block

# -> refactor
def get_content(entry):
    yield from entry.get_blocks()
```
<a name="tLnth"></a>
## 9、合并嵌套的if语句
从简单的开始。不要像这样嵌套 if 语句，只需将它们合并为一个即可。
```python
if a:
    if b:
        pass

# -> refactor
if a and b:
    pass
```
<a name="ZZ8hl"></a>
## 10、使用 any 而不是循环
这里要检查列表中是否至少有一个正元素。更长的解决方案是遍历所有数字，检查当前数字，然后在条件为真时中断。但是对于这个任务，在 Python 中有一个专门的方法，即 any 函数。如果可迭代对象的任何元素为真，则 any 返回 True。这比手动循环要短得多，也更像 pythonic。
```python
numbers = [-1, -2, -4, 0, 3, -7]
has_positives = False
for n in numbers:
    if n > 0:
        has_positives = True
        break

# -> refactor
has_positives = any(n > 0 for n in numbers)
```
<a name="Cg2iv"></a>
## 11、从 for/while 循环中提取语句
很多时候会看到循环中定义了一个变量，但它永远不会改变。这些都是不必要的操作，所以把它从循环中拉出来，然后只需要创建一次。
```python
for building in buildings:
    city = 'London'
    addresses.append(building.street_address, city)

# -> refactor
city = 'London'
for building in buildings:
    addresses.append(building.street_address, city)
```
<a name="SFrJV"></a>
## 12、去除只使用一次并立即返回的内联变量
很多时候会看到代码在最后一个函数中定义了一个变量，一行之后它立即返回。如果清楚函数是干什么的，直接返回结果即可。这样更简洁并且避免了不必要的变量。但是，有时如果不是很清楚函数在做什么，它仍然会有帮助，然后可以给最后一个变量一个有意义的名称并将其用作自文档代码。
```python
def state_attributes(self):
    """Return the state attributes."""
    state_attr = {
        ATTR_CODE_FORMAT: self.code_format,
        ATTR_CHANGED_BY: self.changed_by,
    }
    return state_attr

# -> refactor
def state_attributes(self):
    """Return the state attributes."""
    return {
        ATTR_CODE_FORMAT: self.code_format,
        ATTR_CHANGED_BY: self.changed_by,
    }
```
<a name="O5X6F"></a>
## 13、用if表达式替换if语句
不用 if else 语句来设置变量的值，可以像这样用 if 表达式在一行中设置它。不过，这种重构技术有点值得商榷。有些人仍然喜欢第一个选项，这很好。
```python
if condition:
    x = 1
else:
    x = 2

# -> refactor
x = 1 if condition else 2
```
<a name="pvizo"></a>
## 14、添加保护条款
查看此代码时，很难快速掌握正在发生的事情。有多个 if-else 语句和多个缩进。一旦仔细观察，可能会发现第一个 if 语句几乎覆盖了整个函数代码，只是在最后有相应的 else 子句，只返回 False。<br />可以把这个 else 子句移到最开始。这也称为_警卫声明_。所以如果条件不成立，就不会执行其余的函数代码。这样就去掉了一个 else 子句，现在整个代码中的缩进少了一层。这看起来更清晰，也更容易理解。
```python
def should_i_wear_this_hat(self, hat):
    if isinstance(hat, Hat):
        current_fashion = get_fashion()
        weather_outside = self.look_out_of_window()
        is_stylish = self.evaluate_style(hat, current_fashion)
        if weather_outside.is_raining:
            print("Damn.")
            return True
        else:
            print("Great.")
            return is_stylish
    else:
        return False

# -> refactor
def should_i_wear_this_hat(self, hat):
    if not isinstance(hat, Hat):
        return False

    current_fashion = get_fashion()
    weather_outside = self.look_out_of_window()
    is_stylish = self.evaluate_style(hat, current_fashion)
    if weather_outside.is_raining:
        print("Damn.")
        return True
    else:
        print("Great.")
        return is_stylish
```
<a name="GZHdo"></a>
## 15、将分配移近它们的用途
这是上一个示例的改进代码，但仍然需要一些时间才能理解这里发生的事情。所以想检查是否应该戴帽子。逻辑是这样的：如果正在下雨，总是说 True，如果没有下雨，如果帽子很时尚，就说 True。可以大大提高此逻辑的可读性的一种简单方法是将分配移至更接近其用法的位置。在使用 if 语句之前先了解天气情况。现在 fashion 和 style 变量只在 else 子句中需要，所以将它们向下移动。现在应该更清楚发生了什么。<br />还记得第 4 条提示吗？可以进一步缩短代码并立即返回评估样式结果。然而，在这个例子中，我也喜欢 is_stylish 这个名字，因为它让你知道如果帽子很时尚，你就说 True，否则就说 False。所以这里把多余的变量留着就好了。
```python
def should_i_wear_this_hat(self, hat):
    if not isinstance(hat, Hat):
        return False

    current_fashion = get_fashion()
    weather_outside = self.look_out_of_window()
    is_stylish = self.evaluate_style(hat, current_fashion)
    if weather_outside.is_raining:
        print("Damn.")
        return True
    else:
        print("Great.")
        return is_stylish

# -> refactor
def should_i_wear_this_hat(self, hat):
    if not isinstance(hat, Hat):
        return False

    weather_outside = self.look_out_of_window()
    if weather_outside.is_raining:
        print("Damn.")
        return True
    else:
        print("Great.")
        current_fashion = get_fashion()
        return self.evaluate_style(hat, current_fashion)
        # is_stylish = self.evaluate_style(hat, current_fashion)
        # return is_stylish
```
<a name="zPGYv"></a>
## 16、简化序列检查
这是经常看到的另一件事。当需要检查集合中是否有元素时，例如在列表中，不需要写`if len(your_list) > 0`。可以简单地说if your_list。这是 pep 8 推荐的方法，也称为**真值测试**。这是可能的，因为在 Python 中，空序列和集合的计算结果为 False。所以这可以应用于字符串、元组、列表、字典和集合。
```python
if len(list_of_hats) > 0:
    hat_to_wear = choose_hat(list_of_hats)

# -> refactor
if list_of_hats:
    hat_to_wear = choose_hat(list_of_hats)
```
