Python GUI

![](./img/1635399334875-28695940-0c5c-4e7c-a238-1b1b4b3392c2.gif)

PyQtGraph主要使用领域：数学/科学/工程应用等；

PyQtGraph为PyQt5/PyQt6/PySide2等图形用户界面 (GUI) 开发框架的一款强大可视化工具，底层为NumPy (快速科学计算)、Qt的GraphicsView框架 (2D图形可视化)、OpenGL (3D图形可视化)；<br />相比于之前的工具，PyQtGraph在以下方面尤其出色：

- 界面修图 (点一点即可修改)；
- 快速成图 (底层为NumPy)；
- 实时绘制数据；
- 医学影像图展示 (如MRI)；
- 交互图快速制作 (数据选择、标记、小部件)；
- 对python/qt程序员更友好；
- 完美支持 Linux, Windows和OSX；
- 纯python编写，比pyqwt更易于移植等

---

<a name="uliA1"></a>
## 安装
```bash
pip install pyqtgraph -i https://pypi.tuna.tsinghua.edu.cn/simple
```
<a name="ka5no"></a>
## 使用方式

- 交互式shell里使用 (如python -i, ipython, etc)；
- 应用程序中直接弹出窗口使用；
- 以小部件的形式嵌入到在PyQt应用程序中使用；
- PyQt和PySide中使用等等。
<a name="F1jQ0"></a>
## 快速上手
以在command line里使用为例：
```python
import numpy as np
import pyqtgraph as pg

data = np.random.normal(size=(500,500))
pg.image(data, title="Simplest possible image example")
```
以上几行代码即可得到一个界面可修改的交互图：![](./img/1635399334548-1695fcf0-ec25-41d8-8d05-1f23d66b6fbe.gif)

---

<a name="y8JhE"></a>
## 齐全的demo
PyQtGraph提供了人性化的案例界面，非常有助于使用：
```python
import pyqtgraph.examples  
pyqtgraph.examples.run()
```
![](./img/1635399334492-9ac8719b-5ae2-4ffc-870c-84ce8fe6d193.webp)
<a name="PoH2i"></a>
## 例子
<a name="ewiyw"></a>
### 实时绘图
![](./img/1635399334902-6108ac4e-af07-476d-9029-d20a502f2d35.gif)
<a name="fODsZ"></a>
### 音频信号
 ![](./img/1635399334518-a3e79e71-4061-4927-8bed-4236c7caebf3.gif)![](./img/1635399335071-71f66eeb-8169-4d2b-a747-e140cf8cadbc.gif)![](./img/1635399335458-b825c5ed-7e7d-4839-a037-d96eb6d5062f.gif)
<a name="SwxMK"></a>
### 医学影像图![](./img/1635399335948-fcba56b2-ee19-4526-8cd6-11098c8112a5.webp)
<a name="Tc7Gf"></a>
### 3D图
![](./img/1635399336007-85b66d63-4dfe-4e4d-910d-b2eeeeca9b6d.gif)
<a name="ZTXjH"></a>
### 2D图
![](./img/1635399336295-b50c3b84-bc22-4cc0-abd2-770f2ff7e4cd.gif)![](./img/1635399336663-e2f8e063-2600-4d67-b028-828dc0f6d8e2.webp)
<a name="kc2Nb"></a>
### 交互图形
![](./img/1635399336937-d2137ab5-e5d0-4a07-946f-9b35bf94d0ee.gif)<br />参考：[https://github.com/pyqtgraph/pyqtgraph](https://github.com/pyqtgraph/pyqtgraph)
