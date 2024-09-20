Python Matplotlib 可视化<br />绘图过程如果想要给坐标自定义一些不一样的标记，就可以使用 `plot()` 方法的 `marker` 参数来定义。
<a name="WjKu6"></a>
## 实心圆标记
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([1,3,4,5,8,9,6,1,3,4,5,2,4])
plt.plot(ypoints, marker = 'o')
plt.show()
```
显示结果如下：<br />![](./img/1638319859761-48d27679-6c14-446e-8da0-16660a301874.webp)<br />`marker` 可以定义的符号如下：

| 标记 | 符号 | 描述 |
| --- | --- | --- |
| "." | ![](./img/1638319859523-3a030b57-ed50-411a-8c14-e80154dc8560.webp) | 点 |
| "," | ![](./img/1638319859713-8d763b4d-c7d2-4126-8764-2ffb2267edb0.webp) | 像素点 |
| "o" | ![](./img/1638319859734-69fa9e91-cd40-4150-bfa5-82d1d43e18d0.webp) | 实心圆 |
| "v" | ![](./img/1638319859684-39ba3e17-4b3e-4ed0-b03e-231b1c6c115b.webp) | 下三角 |
| "^" | ![](./img/1638319860066-07a98519-b569-4c6c-baf6-101dbf2609ee.webp) | 上三角 |
| "<" | ![](./img/1638319860138-04f1b251-7a21-4d38-b801-e2b08f026fbd.webp) | 左三角 |
| ">" | ![](./img/1638319860329-af43d60e-8c7b-4783-bb60-a778adc099c8.webp) | 右三角 |
| "1" | ![](./img/1638319860250-d17fcbba-f106-4fe2-9608-07dbff06cdd7.webp) | 下三叉 |
| "2" | ![](./img/1638319860465-a42f808a-f535-4fb5-b23b-4c2d825df9c6.webp) | 上三叉 |
| "3" | ![](./img/1638319860588-f0e28e64-2f5c-4777-9b12-b1f3bf65e2e9.webp) | 左三叉 |
| "4" | ![](./img/1638319860664-38788485-cd19-41be-93a9-2696b195c317.webp) | 右三叉 |
| "8" | ![](./img/1638319860617-8de51d77-f99e-4f2e-a63d-088a290a3b01.webp) | 八角形 |
| "s" | ![](./img/1638319860732-c0e56a08-445d-4744-8101-d44a6126a0c9.webp) | 正方形 |
| "p" | ![](./img/1638319860976-56e9bed2-861e-43d8-a051-c98d351a8c8d.webp) | 五边形 |
| "P" | ![](./img/1638319861143-0bca294a-3355-4d7c-81c2-494de520ddde.webp) | 加号（填充） |
| "*" | ![](./img/1638319861168-fe506efa-bd9c-42eb-a72d-1d57a73d4651.webp) | 星号 |
| "h" | ![](./img/1638319861187-f86a75aa-760b-473c-936e-7fd93ba8925d.webp) | 六边形 1 |
| "H" | ![](./img/1638319861300-fd0882cd-56b3-4a98-9613-79fc836bab0e.webp) | 六边形 2 |
| "+" | ![](./img/1638319861426-47f591a4-ef31-4da5-bd92-a28fa234ed9f.webp) | 加号 |
| "x" | ![](./img/1638319861491-42c20955-1ba8-45bf-baeb-59c7affa5316.webp) | 乘号 x |
| "X" | ![](./img/1638319861790-a857806e-8d98-4eb0-88c4-d44dd5583cfb.webp) | 乘号 x (填充) |
| "D" | ![](./img/1638319861735-d3d391aa-07a1-414b-8882-38bb2e31e339.webp) | 菱形 |
| "d" | ![](./img/1638319861793-e768b226-1a63-46d9-940a-eec63cf6c695.webp) | 瘦菱形 |
| "&#124;" | ![](./img/1638319861956-9cd41d7f-fc46-4869-86e1-fe8ac49bfbf5.webp) | 竖线 |
| "_" | ![](./img/1638319862026-a413379c-dbd0-440b-9b6f-ef40b21b1420.webp) | 横线 |
| 0 (TICKLEFT) | ![](./img/1638319862234-c980269a-15c2-4cb0-8dae-fc2a5e07052a.webp) | 左横线 |
| 1 (TICKRIGHT) | ![](./img/1638319862448-b4a37227-474d-4e11-9f54-8f5f0478bd2a.webp) | 右横线 |
| 2 (TICKUP) | ![](./img/1638319862266-63956681-3d64-4acd-b8af-24a8d112a3a4.webp) | 上竖线 |
| 3 (TICKDOWN) | ![](./img/1638319862563-2f223bf3-9d16-4875-a170-f268cc8dd2d3.webp) | 下竖线 |
| 4 (CARETLEFT) | ![](./img/1638319862543-c9c4d02d-0f96-4e76-8dad-80073cc2b0c5.webp) | 左箭头 |
| 5 (CARETRIGHT) | ![](./img/1638319862756-9d4cd8aa-18b8-4138-bbea-904f8ba3178a.webp) | 右箭头 |
| 6 (CARETUP) | ![](./img/1638319862729-0c5a244b-2d3b-4a3f-8eaf-a2b0e3390df9.webp) | 上箭头 |
| 7 (CARETDOWN) | ![](./img/1638319862991-e944167f-a88f-44c9-b9d8-ff44048708bb.webp) | 下箭头 |
| 8 (CARETLEFTBASE) | ![](./img/1638319863095-ce20af31-fdb0-4e4d-99b8-f6310bbeee2e.webp) | 左箭头 (中间点为基准) |
| 9 (CARETRIGHTBASE) | ![](./img/1638319862920-4a577f07-737c-492c-9bda-81bf8c0765bb.webp) | 右箭头 (中间点为基准) |
| 10 (CARETUPBASE) | ![](./img/1638319863200-8c84f40f-2c3c-4fcf-a3e3-720ce68cb10c.webp) | 上箭头 (中间点为基准) |
| 11 (CARETDOWNBASE) | ![](./img/1638319863263-dd11dfce-cc6d-4be9-854c-5487db739ef7.webp) | 下箭头 (中间点为基准) |
| "None", " " or "" | <br /> | 没有任何标记 |
| '$...$' | ![](./img/1638319863388-104ee3a0-082d-4260-b1e1-228df1078e00.webp) | 渲染指定的字符。例如 "$f$" 以字母 f 为标记。 |

<a name="rOVds"></a>
##  `*` 标记
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([1,3,4,5,8,9,6,1,3,4,5,2,4])
plt.plot(ypoints, marker = '*')
plt.show()
```
显示结果如下：<br />![](./img/1638319863711-98d6e785-068e-475a-acef-781ea7a70f27.webp)
<a name="YYry7"></a>
## 下箭头
```python
import matplotlib.pyplot as plt
import matplotlib.markers
plt.plot([1, 2, 3], marker=matplotlib.markers.CARETDOWNBASE)
plt.show()
```
显示结果如下：<br />![](./img/1638319863817-eb37d01a-5e12-43d0-b0a5-481e4edfecff.webp)
<a name="se2xu"></a>
## fmt 参数
fmt 参数定义了基本格式，如标记、线条样式和颜色。
```python
fmt = '[marker][line][color]'
```
例如 o:r，o 表示实心圆标记，: 表示虚线，r 表示颜色为红色。
<a name="dFn96"></a>
### 实例
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([6, 2, 13, 10])
plt.plot(ypoints, 'o:r')
plt.show()
```
显示结果如下：<br />![](./img/1638319863604-b306bb37-0a93-4f96-a3ae-335650481aa0.webp)
<a name="jphua"></a>
### 线类型
| 线类型标记 | 描述 |
| --- | --- |
| '-' | 实线 |
| ':' | 虚线 |
| '--' | 破折线 |
| '-.' | 点划线 |

<a name="m4R8y"></a>
### 颜色类型
| 颜色标记 | 描述 |
| --- | --- |
| 'r' | 红色 |
| 'g' | 绿色 |
| 'b' | 蓝色 |
| 'c' | 青色 |
| 'm' | 品红 |
| 'y' | 黄色 |
| 'k' | 黑色 |
| 'w' | 白色 |

<a name="ineSx"></a>
## 标记大小与颜色
可以自定义标记的大小与颜色，使用的参数分别是：

- `markersize`，简写为 ms：定义标记的大小。
- `markerfacecolor`，简写为 mfc：定义标记内部的颜色。
- `markeredgecolor`，简写为 mec：定义标记边框的颜色。
<a name="NCC8b"></a>
### 设置标记大小
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([6, 2, 13, 10])
plt.plot(ypoints, marker = 'o', ms = 20)
plt.show()
```
显示结果如下：<br />![](./img/1638319863817-5951eba3-d18d-4787-a68a-b986f5daab57.webp)
<a name="QP7dk"></a>
### 设置标记外边框颜色
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([6, 2, 13, 10])
plt.plot(ypoints, marker = 'o', ms = 20, mec = 'r')
plt.show()
```
显示结果如下：<br />![](./img/1638319864169-2a7e7e29-04fd-48ab-9844-31c391da68c0.webp)
<a name="h40Lh"></a>
### 设置标记内部颜色
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([6, 2, 13, 10])
plt.plot(ypoints, marker = 'o', ms = 20, mfc = 'r')
plt.show()
```
显示结果如下：<br />![](./img/1638319864047-41b1e4b2-d6ae-44a6-a4ef-664282ef7bec.webp)
<a name="zktv0"></a>
### 自定义标记内部与边框的颜色
```python
import matplotlib.pyplot as plt
import numpy as np
ypoints = np.array([6, 2, 13, 10])
plt.plot(ypoints, marker = 'o', ms = 20, mec = '#4CAF50', mfc = '#4CAF50')
plt.show()
```
显示结果如下：<br />![](./img/1638319864118-422e59b6-a603-4fb1-97e1-d86af9651298.webp)
