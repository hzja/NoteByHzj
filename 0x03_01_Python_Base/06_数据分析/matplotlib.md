# matplotlib

matplotlib是一个python 2D绘图库，利用它可以画出许多高质量的图像。只需几行代码即可生成直方图，条形图，饼图，散点图等。

如果下面的函数不知道怎么用可以用[chatgpt](https://texttools.cn/)查询函数的功能和用法。



## 折线图

### 单图单线

~~~ python
from matplotlib import pyplot as plt
import random
import matplotlib
from matplotlib import font_manager


#windws和linux设置字体的放(不常用)
# font = {'family' : 'MicroSoft YaHei',
#         'weight': 'bold',
#         'size': 'larger'}
# matplotlib.rc("font",**font)
# matplotlib.rc("font",family='MicroSoft YaHei',weight="bold")

#另外一种设置字体的方式(常用)
my_font = font_manager.FontProperties(fname="C:\\Windows\Fonts\msyhbd.ttc")

# x,y轴数据
x = range(0,120)
y = [random.randint(20,35) for i in range(120)]

#设置图片大小
plt.figure(figsize=(20,8),dpi=80)

#绘图，可以根据多组数据绘制多组图
plt.plot(x,y)

#调整x轴的刻度
_xtick_labels = ["10点{}分".format(i) for i in range(60)]
_xtick_labels += ["11点{}分".format(i) for i in range(60)]

#取步长，数字和字符串一一对应，数据的长度一样
plt.xticks(list(x)[::3],_xtick_labels[::3],rotation=45,fontproperties=my_font) #rotaion旋转的度数

#添加描述信息
plt.xlabel("时间",fontproperties=my_font)
plt.ylabel("温度 单位(℃)",fontproperties=my_font)
plt.title("10点到12点每分钟的气温变化情况",fontproperties=my_font)

#保存
plt.savefig("./t1.png")

#展示图形
plt.show()
~~~



### 单图多线

~~~ python
from matplotlib import pyplot as plt
from matplotlib import font_manager


my_font = font_manager.FontProperties(fname="C:\\Windows\Fonts\msyhbd.ttc")
y_1 = [1,0,1,1,2,4,3,2,3,4,4,5,6,5,4,3,3,1,1,1]
y_2 = [1,0,3,1,2,2,3,3,2,1 ,2,1,1,1,1,1,1,1,1,1]
x = range(11,31)

#设置图形大小
plt.figure(figsize=(20,8),dpi=80)

#绘图，绘制多条线
plt.plot(x,y_1,label="自己",color="#F08080")
plt.plot(x,y_2,label="同桌",color="#DB7093",linestyle="--")

#设置x轴刻度
_xtick_labels = ["{}岁".format(i) for i in x]
plt.xticks(x,_xtick_labels,fontproperties=my_font)
# plt.yticks(range(0,9))


#绘制网格，可以设置清晰的的百分比
plt.grid(alpha=0.4,linestyle=':')

#添加图例
plt.legend(prop=my_font,loc="upper left")

#展示
plt.show()
~~~



## 散点图

~~~ python 
from matplotlib import pyplot as plt
from matplotlib import font_manager


my_font = font_manager.FontProperties(fname="C:\\Windows\Fonts\msyhbd.ttc")
y_3 = [11,17,16,11,12,11,12,6,6,7,8,9,12,15,14,17,18,21,16,17,20,14,15,15,15,19,21,22,22,22,23]
y_10 = [26,26,28,19,21,17,16,19,18,20,20,19,22,23,17,20,21,20,22,15,11,15,5,13,17,10,11,13,12,13,6]
x_3 = range(1,32)
x_10 = range(51,82)

#设置图形大小
plt.figure(figsize=(20,8),dpi=80)

#绘图（之前绘制折线图的唯一区别）
plt.scatter(x_3,y_3,label="3月份")
plt.scatter(x_10,y_10,label="10月份")

#调整x轴的刻度
_x = list(x_3)+list(x_10)
_xtick_labels = ["3月{}日".format(i) for i in x_3]
_xtick_labels += ["10月{}日".format(i-50) for i in x_10]
plt.xticks(_x[::3],_xtick_labels[::3],fontproperties=my_font,rotation=45)

#添加图例
plt.legend(loc="upper left",prop=my_font)

#添加描述信息
plt.xlabel("时间",fontproperties=my_font)
plt.ylabel("温度",fontproperties=my_font)
plt.title("标题",fontproperties=my_font)

#展示
plt.show()
~~~



## 柱状图

### 单柱图形

~~~ python 
from matplotlib import pyplot as plt
from matplotlib import font_manager


my_font = font_manager.FontProperties(fname="C:\\Windows\Fonts\msyhbd.ttc")

a = ["战狼2","速度与激情8","功夫瑜伽","西游伏妖篇","变形金刚5：最后的骑士","摔跤吧！爸爸","加勒比海盗5：死无对证","金刚：骷髅岛","极限特工：终极回归","生化危机6：终章","乘风破浪","神偷奶爸3","智取威虎山","大闹天竺","金刚狼3：殊死一战","蜘蛛侠：英雄归来","悟空传","银河护卫队2","情圣","新木乃伊",]

b=[56.01,26.94,17.53,16.49,15.45,12.96,11.8,11.61,11.28,11.12,10.49,10.3,8.75,7.55,7.32,6.99,6.88,6.86,6.58,6.23]

#设置图形大小
plt.figure(figsize=(20,15),dpi=80)

#绘制条形图
plt.bar(range(len(a)),b,width=0.3)

#下面这种写法，可以让图形横向延申（二选一）
plt.barh(range(len(a)),b,height=0.3,color="orange")

#设置字符串到x轴
plt.xticks(range(len(a)),a,fontproperties=my_font,rotation=90)
plt.savefig("./movie.png")

plt.show()
~~~



### 多柱图形

~~~ python
from matplotlib import pyplot as plt
from matplotlib import font_manager


my_font = font_manager.FontProperties(fname="C:\\Windows\Fonts\msyhbd.ttc")

a = ["猩球崛起3：终极之战","敦刻尔克","蜘蛛侠：英雄归来","战狼2"]

b_16 = [15746,312,4497,319]
b_15 = [12357,156,2045,168]
b_14 = [2358,399,2358,362]

#柱子粗细
bar_width = 0.2
x_14 = list(range(len(a)))
x_15 = [i+bar_width for i in x_14]
x_16 = [i+bar_width*2 for i in x_14]

#设置图形大小
plt.figure(figsize=(20,8),dpi=80)

#竖向图
plt.bar(range(len(a)),b_14,width=bar_width,label="9月14日")
plt.bar(x_15,b_15,width=bar_width,label="9月15日")
plt.bar(x_16,b_16,width=bar_width,label="9月16日")

#设置图例
plt.legend(prop=my_font)

#设置x轴的刻度
plt.xticks(x_15,a,fontproperties=my_font)

plt.show()
~~~

