PythonMatplotlib<br />系统介绍一下优秀的关于Matplotlib颜色表(colormap) 第三方库。主要内容如下：

- **偏学术类型**
- **偏艺术类型**
<a name="t6GQc"></a>
## 偏学术类型
Python-Matplotlib的偏学术类型的colormap，介绍以下几个库，当然，还有MATLAB的颜色表
<a name="YAjea"></a>
### Python-scicomap包
Python-scicomap包可以说是专门为Matplotlib提供科学学术图表的配色，其含有单色系(sequential), 双单色系(bi-sequential), 双色渐变系(diverging)，循环色系(circular)，多色系(qualitative) 和混杂色系(miscellaneous) 颜色表。接下来列举下几个样例查看下：
<a name="YgyJb"></a>
#### 样例一
```python
import scicomap as sc
import matplotlib.pyplot as plt

# the thing that should not be
ugly_jet = plt.get_cmap("jet")
sc_map =  sc.ScicoMiscellaneous(cmap=ugly_jet)
f=sc_map.assess_cmap(figsize=(22,10))
f.set_facecolor("w")
```
![Example01 Of scicomap](./img/1683472809269-c2d7e2f5-f366-4069-aa6b-b9235a568951.png "Example01 Of scicomap")
<a name="HXrxP"></a>
#### 样例二：Comparing color maps
```python
c_l =  ["cividis", "inferno", "magma", "plasma", "viridis"]
f = sc.plot_colorblind_vision(ctype='sequential', 
                              cmap_list=c_l, 
                              figsize=(30, 4), 
                              n_colors=11, 
                              facecolor="black")
```
![Comparing scicomap color maps](./img/1683472809309-dac3b22c-f2a3-48bb-9077-d683f7c849e9.png "Comparing scicomap color maps")
<a name="nJZFO"></a>
#### 样例三
```python
f = sc.compare_cmap(image="grmhd", 
                    ctype='sequential', 
                    ncols=15, 
                    uniformize=True, 
                    symmetrize=True, 
                    unif_kwargs={'lift': 20}, 
                    sym_kwargs={'bitonic': False, 'diffuse': True})
```
![Sequential color maps](./img/1683472809273-7169eb9b-6e4d-4a24-a435-2f7d8fa4744b.png "Sequential color maps")
<a name="FMj1q"></a>
#### 样例四
```python
f = sc.compare_cmap(image="vortex", 
                    ctype='diverging', 
                    ncols=15, 
                    uniformize=True, 
                    symmetrize=True, 
                    unif_kwargs={'lift': None}, 
                    sym_kwargs={'bitonic': False, 'diffuse': True})
```
![Diverging color maps](./img/1683472809298-58c5430f-b4b6-4e20-84e1-ee96864ebac8.png "Diverging color maps")<br />更多关于scicomap颜色包详细内容，可参考：[**Python-scicomap包**](https://github.com/ThomasBury/scicomap)
<a name="rmSW5"></a>
### Python-colorcet包
colorcet包包含太多类型的颜色系了，这里也不过多赘述，直接上案例：
<a name="bZRdc"></a>
#### 样例一
```python
import holoviews as hv
import colorcet as cc
from colorcet.plotting import swatches, sine_combs

hv.notebook_extension("matplotlib")

swatches(group="linear")
```
![Linear (sequential) colormaps, for plotting magnitudes](./img/1683472809393-71243441-825a-4e8c-8308-dd171f65be81.png "Linear (sequential) colormaps, for plotting magnitudes")
<a name="V9lsM"></a>
#### 样例二
```python
swatches(group='diverging')
```
![Diverging colormaps, for plotting magnitudes increasing or decreasing from a central point](./img/1683472809660-b8be4a08-79f0-4330-92ed-ecb3eebc7c54.png "Diverging colormaps, for plotting magnitudes increasing or decreasing from a central point")
<a name="KmNjn"></a>
#### 样例三
```python
swatches(group='nopic')
```
![Colorblind-safe colormaps](./img/1683472809821-6e1676a9-25a5-46dd-92fb-e10752642298.png "Colorblind-safe colormaps")
<a name="cql6i"></a>
#### 样例四
```python
misc = [name for name in cc.all_original_names() if "cyclic" in name or "isoluminant" in name or "rainbow" in name]
swatches(*misc)
```
![Misc colormaps](./img/1683472809791-2dafbc12-23db-4d99-9bd9-974afb039820.png "Misc colormaps")<br />更多关于colorcet颜色包详细内容，大家可参考：[**Python-colorcet包**](https://colorcet.holoviz.org/)
<a name="bpHxc"></a>
### Python-cmasher包
Python-cmasher包也是为学术配色所设计出的一个Matplotlib颜色包，这里直接列举几个色系即可，如下：
<a name="v2w8a"></a>
#### 样例一：Sequential colormaps
![amber](./img/1683472810104-64759ddf-6455-4660-a174-6cb01df01e32.png "amber")<br />![amber_viscm](./img/1683472810168-46208f9a-f6ca-4d5f-9f68-e317726c80c3.png "amber_viscm")<br />![eclipse](./img/1683472810229-d29652de-447c-49e6-a421-f6d7bd17155d.png "eclipse")<br />![eclipse_viscm](./img/1683472810329-da0e3e9c-d8d5-40b1-925f-5d2e6a74935d.png "eclipse_viscm")
<a name="JiYeJ"></a>
#### 样例二：Diverging colormaps
![copper](./img/1683472810442-634ebbb8-c770-481c-b8ea-eb59b4a37074.png "copper")<br />![copper_viscm](./img/1683472810597-3db8767e-f70e-4d0f-839a-fe1862900ad3.png "copper_viscm")<br />![prinsenvlag](./img/1683472810659-ec645727-21ad-4c1b-a76b-3163189b46a4.png "prinsenvlag")<br />![prinsenvlag_viscm](./img/1683472810854-ddeb3346-07fa-4613-9083-4c4176bb7eae.png "prinsenvlag_viscm")<br />更多关于cmasher颜色包详细内容，大家可参考：[**Python-cmasher包**](https://cmasher.readthedocs.io/)
<a name="caGhq"></a>
### Python-viscm包
Python-viscm包是比较喜欢的一个，因为其提供了MATLAB的默认颜色主题，相信许多小伙伴还是蛮期待的，话不多说，直接列举样例，如下：
<a name="Kz67M"></a>
#### 样例一：PARULA
![parula](./img/1683472810812-3e17dfbf-5524-4bee-a8e9-0ba5ae01e241.png "parula")
<a name="xLRTo"></a>
#### 样例二：PLASMA
![PLASMA](./img/1683472810881-892b1824-5367-4345-8f74-0723fa9d719c.png "PLASMA")
<a name="qr40u"></a>
#### 样例三：VIRIDIS
![VIRIDIS](./img/1683472811044-5d528ef3-4adb-4fc3-ab5a-66789c1b5860.png "VIRIDIS")<br />更多关于viscm颜色包详细内容，大家可参考：[**Python-viscm包**](https://bids.github.io/colormap/)
<a name="fcJWI"></a>
## 偏艺术类型
这一部分给出的是一个偏清新的颜色，也就是Python-vapeplot包，详细内容如下：
<a name="EwFas"></a>
### 样例一
```python
import vapeplot
%matplotlib inline
vapeplot.available()
```
![vapeplot example](./img/1683472811127-b8be85a8-96cc-4090-97dc-411d6470c061.png "vapeplot example")
<a name="r0Aid"></a>
### 样例二
```python
vapeplot.view_palette("cool",'sunset')
```
![view_palette](./img/1683472811473-db783886-b3f9-49d9-abd9-b4dbb1bf9f74.png "view_palette")
<a name="i7nO1"></a>
### 样例三：cool
![](./img/1683472811480-37cf3076-70f4-4cad-9750-a0ae413523f2.png)<br />![](./img/1683472811498-02001bb9-0c33-4602-ba2f-9f4c1dfb65cb.png)
<a name="jpBxm"></a>
### 样例四：avanti
![](./img/1683472811489-9ab58617-43c9-4580-a911-7b5137c9c8a4.png)<br />![](./img/1683472811559-3049614c-05c0-4b9e-8080-2dd567acf587.png)<br />更多关于vapeplot颜色包详细内容，大家可参考：[**Python-vapeplot包**](https://github.com/dantaki/vapeplot)
<a name="OdSdg"></a>
## 总结
列举了几个学术和商业Matplotlib颜色包，可以选择自己喜欢的学术配色进行论文的绘制~~
<a name="mMiu5"></a>
## 参考资料
**Python-scicomap包:**[**_https://github.com/ThomasBury/scicomap_**](https://github.com/ThomasBury/scicomap)<br />**Python-colorcet包:**[**_https://colorcet.holoviz.org/_**](https://colorcet.holoviz.org/)<br />**Python-cmasher包:**[**_https://cmasher.readthedocs.io/_**](https://cmasher.readthedocs.io/)<br />**Python-viscm包:**[**_https://bids.github.io/colormap/_**](https://bids.github.io/colormap/)<br />**Python-vapeplot包:**[**_https://github.com/dantaki/vapeplot_**](https://github.com/dantaki/vapeplot)
