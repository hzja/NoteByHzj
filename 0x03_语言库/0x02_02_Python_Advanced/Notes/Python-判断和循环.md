Python 
<a name="Y5ton"></a>
## 判断语句
<a name="40CzI"></a>
### if判断语句
该语句的语法格式如下：
```python
#单条件判断：
if condition_1:
  result_1
else:
  result_2
#多条件判断：
if condition_1:
  result_1
elif:
  result_2
elif:
  result_3
else:
  result_4
   
'''
现在做一个成绩评级的小程序
要求：按照90-100/80-89/60-79/40-59/小于39
划分五个等级
'''
score=eval(input("请输入成绩:\n"))
#eval是Python的一个内置函数，这个函数的作用是，返回传入字符串的表达式的结果。
if (score>90)&(score<=100):
  print('该同学成绩为优')
elif (score>80)&(score<=89):   #多条件判断
  print('该同学成绩为良')
elif (score>60)&(score<=79):
  print('该同学成绩为良')
elif (score>40)&(score<=59):
  print('该同学成绩为不及格')
elif (score<39)&(score>0):
  print('该同学请自行退学')
'''
请输入成绩:
34
该同学请自行退学
'''
```
<a name="fnTyE"></a>
## 循环语句
<a name="01IcQ"></a>
### while 循环
**while语句格式：**
```python
while condition(判断条件):
  statements(执行语句)
```
**实例：计算1-100的和**
```python
#简单一点的写法
n=0
sum=0
while n<=100:
  sum +=n
  n+=1
print(sum)
#规范一点的写法
# n = 100
# sum = 0
# counter = 1
# while counter <= n:
#     sum = sum + counter
#     counter += 1
# # print("1 到 %d 之和为: %d" % (n,sum))
#5050
```
<a name="AqDSi"></a>
### continue、break的用法

- continue是终止本次循环
- break是终止循环
```python
#continue举例
#输出十以内小于等于4和大于等于7的数
count=0
while count<10:
count +=1
  if count>4 and count<7:
    continue
   print(count)
'''
1
2
3
4
7
8
9
10
'''
```
<a name="503115ee"></a>
#### continue + else的用法
```python
count=0
while count<10:
  count +=1
if count>4 and count<7:    
      #当数字满足上述条件时，执行continue，终止本次循环，不执行打印语句。
    continue
   print(count)
else:
print('循环正常结束')
'''
1
2
3
4
7
8
9
10
循环正常结束
'''
```
<a name="Zz3lR"></a>
#### break的用法
```python
count=0
while count<10:
   
   count +=1
   if count>4:   #当数字大于4的时候执行break，终止循环
       break
   print(count)
else:
   print('循环正常结束')
'''
1
2
3
4
'''
```
<a name="jFFsz"></a>
### for语句
for循环可以遍历任何序列的所有元素<br />**for语句的一般格式如下：**
```python
for variable(变量) in [sequence(序列)]:
   statements(执行语句)
else:
   statements(执行语句)
```
**循环实例：**
```python
a=['welcome','to','data','science']
for i in a:     #遍历数组内的每一个元素并且打印
   print (i)
   
for i in 'DataScience':  #遍历该字符串的每一个元素且打印
   print (i)
print('遍历结束')
'''
welcome
to
data
science
D
a
t
a
S
c
i
e
n
c
e
遍历结束
'''
```
<a name="r7ZFw"></a>
#### for...else...语句
```python
#判断一个数是不是质数
n=int(input('请随机输入一个实数：'))  
for i in range(2,n):
   if n%i == 0:    #判断是否能整除，如能，打印输出，终止循环；若不能，执行else语句        
       print(n,'能被{}整除，不是质数'.format(i))  #format的用法，之前的课程我们有讲到
       break
#         continue
else:                  
      print (n,'是一个质数')
'''
请随机输入一个实数：4
4 能被2整除，不是质数
'''
```
<a name="XtXIE"></a>
#### 循环嵌套
for 循环嵌套和while 循环嵌套的案例演示<br />**for 循环嵌套格式**
```python
for variable in seuqence:
   for veriable_ in sequence_:
       statements_
   statements
#while 循环嵌套格式
while condition:
   while condition_:
       statements_
   statements
#for 和 while 也可相互嵌套
```
**判断1-20之间哪些是质数**
```python
i = 2
while(i < 20):
   
   j = 2
   while(j <= (i/j)):
       
       if not(i%j):
           break
       j = j + 1
   if (j > i/j):
       print (i, " 是质数")
   i = i + 1
print ("结束!")
'''
2 是质数
3 是质数
5 是质数
7 是质数
11 是质数
13 是质数
17 是质数
19 是质数
结束!
'''
```

