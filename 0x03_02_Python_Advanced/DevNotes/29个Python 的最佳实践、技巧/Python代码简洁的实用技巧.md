Python
<a name="Uv16S"></a>
## 1、使用Lambda来修改Pandas数据框中的值
假设有以下`df`数据框:
```bash
data = [[1,2,3], [4,5,6], [7,8,9]]
df = pd.DataFrame(data, columns=[0,1,2])
IN[1]: print (df)
OUT[1]:    0  1  2
        0  1  2  3
        1  4  5  6
        2  7  8  9
```
现在由于某种原因，需要在第`0`列的数字上添加`01`的值。一个常见的方法是定义一个函数来完成这个任务，然后用 `apply` 函数来修改一列的值。
```bash
def add_numbers(x):
    return f'{x}01'
df[0] = df[0].apply(add_numbers)
IN[1]: print (df)
OUT[1]:     0   1   2
        0  101  2   3
        1  401  5   6
        2  701  8   9
```
这并不复杂，但是在数据框中对每一个改变创建一个函数是不切实际的。这时`lambda`就派上了用场。<br />`lambda`函数类似于普通的Python函数，但它可以不使用名称来定义，这使得它成为一个漂亮的单行代码。之前使用的代码可以用以下方式来减少。
```bash
df[0] = df[0].apply(lambda x:f'{x}01')
```
不知道是否可以访问一个系列的属性来修改数据时，Lambda变得非常有用。<br />例如，列`0`包含字母，把它们大写。
```bash
# 如果你知道.str的存在，你可以这样做
df[0] = df[0].str.title()
# 如果你不知道.str，你仍然可以用lambda大写
df[0] = df[0].apply(lambda x: x.title())
```
<a name="q10DK"></a>
## 2、使用f-string来连接字符串
字符串连接是Python中非常常见的操作，它可以用不同的方法来完成。最常见的方法是使用`+`运算符；然而，这个运算符的一个问题是不能在字符串之间添加任何分隔符。<br />当然，如果想把 "Hello "和 "World "连接起来，一个典型的变通方法是添加一个空白分隔符（" "）。
```bash
print("Hello" + " " + "World")
```
这就完成了工作，但为了写出更可读的代码，可以用一个f-string来代替它。
```bash
IN[2]: print(f'{Hello} {World}')
OUT[2]: "Hello World"
```
在一个基本的例子中，这似乎是不必要的，但是当涉及到连接多个值时（正如将在提示#3中看到的），f-string将免于书写多次`+ " " +`。过去有多少次不得不写`+`运算符，但现在不会了！<br />其他连接字符串的方法是使用`join()`方法或`format()`函数，然而f-string在字符串连接方面做得更好。
<a name="wGnv1"></a>
## 3、用`Zip()`函数对多个列表进行迭代
是否曾经想在 Python 中循环遍历一个以上的列表？当有两个列表时，可以用 `enumerate` 来实现。
```bash
teams = ['Barcelona', 'Bayern Munich', 'Chelsea']
leagues = ['La Liga', 'Bundesliga', 'Premiere League']
for i, team in enumerate(teams):
    league = leagues[i]
    print(f'{team} plays in {league}')
```
然而，当有两个或更多的列表时，这变得不切实际。一个更好的方法是使用`zip()`函数。`zip()`函数接收迭代数据，将它们聚集在一个元组中，并返回之。<br />再增加一个列表，看看`zip()`的威力!
```bash
teams = ['Barcelona', 'Bayern Munich', 'Chelsea']
leagues = ['La Liga', 'Bundesliga', 'Premiere League']
countries = ['Spain', 'Germany', 'UK']
for team, league, country in zip(teams, leagues, countries):
    print(f'{team} plays in {league}. Country: {country}')
```
上述代码的输出结果为：
```bash
Barcelona plays in La Liga. Country: Spain
Bayern Munich plays in Bundesliga. Country: Germany
Chelsea plays in Premiere League. Country: UK
```
此处注意到在这个例子中使用了f-string吗？代码变得更有可读性，不是吗？
<a name="fbQwQ"></a>
## 4、使用列表理解法
清洗和处理数据的一个常见步骤是修改现有的列表。比如有以下需要大写的列表：
```bash
words = ['california', 'florida', 'texas']
```
将`words`列表的每个元素大写的典型方法是创建一个新的大写列表，执行一次 for 循环，使用`.title()`，然后将每个修改的值附加到新的列表中。
```bash
capitalized = []
for word in words:
    capitalized.append(word.title())
```
然而，Pythonic的方法是使用列表理解来做到这一点。列表理解有一种优雅的方法来制作列表。<br />可以用一行代码重写上面的`for`循环：
```bash
capitalized = [word.title() for word in words]
```
由此可以跳过第一个例子中的一些步骤，结果是一样的。
<a name="XDWXN"></a>
## 5、对文件对象使用`with`语句
当在一个项目上工作时，经常会对文件进行读写操作。最常见的方法是使用`open()`函数打开一个文件，它会创建一个可以操作的文件对象，然后作为一个习惯的做法，应该使用`close()`关闭该文件对象。
```bash
f = open('dataset.txt', 'w')
f.write('new_data')
f.close()
```
这很容易记住，但有时可能会忘记用`f.close()`关闭`f`文件。这时，`with`语句就派上了用场。`with`语句将自动关闭文件对象`f`，形式如下:
```bash
with open('dataset.txt', 'w') as f:
    f.write('new_data')
```
有了这个，可以保持代码的简短。<br />不需要用它来读取CSV文件，因为可以用pandas的 `pd.read_csv()`轻松地读取，但在读取其他类型的文件时，这仍然很有用。例如，从pickle文件中读取数据时经常使用它。
```bash
import pickle 
# 从pickle文件中读取数据集
with open('test', 'rb') as input:
    data = pickle.load(input)
```
<a name="dXyWK"></a>
## 6、停止使用方括号来获取字典项, 利用`.get()`代替
比如，有以下一个字典:
```bash
person = {'name': 'John', 'age': 20}
```
可以通过`person[name]`和`person[age]`分别获得姓名和年龄。但是，由于某种原因，想获得一个不存在的键，如 "工资"，运行`person[salary]`会引发一个`KeyError'。<br />这时，`get()`方法就有用了。如果键在字典中，`get()`方法返回指定键的值，但是如果没有找到键，Python 将返回None。得益于此，代码不会中断。
```bash
person = {'name': 'John', 'age': 20}
print('Name: ', person.get('name'))
print('Age: ', person.get('age'))
print('Salary: ', person.get('salary'))
```
输出结果如下:
```bash
Name:  John
Age:  20
Salary:  None
```
<a name="v2FTi"></a>
## 7、多重赋值
是否曾想减少用于创建多个变量、列表或字典的代码行数？那么，可以用多重赋值轻松做到这一点。
```bash
# 原始操作
a = 1
b = 2
c = 3
# 替代操作
a, b, c = 1, 2, 3
# 代替在不同行中创建多个列表
data_1 = []
data_2 = []
data_3 = []
data_4 = []
# 可以在一行中创建它们的多重赋值
data_1, data_2, data_3, data_4 = [], [], [], []
# 或者使用列表理解法
data_1, data_2, data_3, data_4 = [[] for i in range(4)]
```
