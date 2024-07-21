Python 可视化<br />空间（地理）数据可视化神器：keplergl是Uber完全开源的，也是Uber内部进行空间数据可视化的默认工具。<br />通过其面向Python开放的接口包keplergl，可以在jupyter notebook中通过书写Python代码的方式传入多种格式的数据，在其嵌入notebook的交互窗口中使用其内建的多种丰富的空间数据可视化功能。下面有3个主要的学习地址：<br />1、官网地址：[https://kepler.gl/](https://kepler.gl/)<br />2、jupyter notebook使用手册地址：[https://github.com/keplergl/kepler.gl/tree/master/docs/keplergl-jupyter#geojson](https://github.com/keplergl/kepler.gl/tree/master/docs/keplergl-jupyter#geojson)<br />3、案例地址：[https://github.com/keplergl/kepler.gl/tree/master/bindings/kepler.gl-jupyter/notebooks](https://github.com/keplergl/kepler.gl/tree/master/bindings/kepler.gl-jupyter/notebooks)<br />![](./img/1641138590141-94ed2e44-ca10-4061-9663-bef5e2373137.webp)
<a name="ZPYfh"></a>
## 安装
keplergl的安装是非常简单的。
```bash
pip install keplergl
```
<a name="xz0TY"></a>
## 惊艳图
一大波惊艳图形来了：<br />![](./img/1641138590025-ace9c205-268c-4765-9d1b-0b9737fb0f4a.webp)<br />![](./img/1641138590166-e86e3598-613d-4d01-8553-3f1aadf871e3.webp)<br />![](./img/1641138590004-e9a667cf-19d1-420f-9c7d-b870f08a24ae.webp)<br />![](./img/1641138590201-891f3d99-008c-4862-b40b-f8472904a248.webp)<br />![](./img/1641138590388-49b9ab78-5ce6-46e0-906b-5d1715bf554b.webp)<br />![](./img/1641138590546-8704fcbb-b7ff-4a94-ad17-b0d06030ef3d.webp)<br />![](./img/1641138590503-1e40c481-1cf4-4570-a34d-264b8181d1c9.webp)<br />![](./img/1641138590693-a12f1067-8a61-4fc7-9193-69532cd9552a.webp)<br />![](./img/1641138590931-0ea2edeb-ddf7-4fc0-8d20-6c4b463240df.webp)
<a name="BT3j1"></a>
## 案例入门
```python
import pandas as pd
import geopandas as gpd

from keplergl import KeplerGl

# 创建对象
kep1 = KeplerGl(height=600)
# 激活对象，加载到jupyter notebook中
kep1
```
可以看到，运行了基本代码后在Jupyter中直接就产生了内置的图形，图形本身也是动态的：<br />![](./img/1641138591118-a09f467e-e821-474f-ab74-b832c1600dd0.gif)
<a name="HiJGA"></a>
## 添加数据
keplergl默认是可以添加3种形式的数据：

- csv
- GeoJSON
- DataFrame
<a name="AbojV"></a>
### csv格式
本地目录下有一份csv数据：china.csv，记录的是我国各个省份的经纬度：
```python
with open("china.csv","r") as f:
    csv_data = f.read()
    
# add_data添加数据
kep1.add_data(data=csv_data, name="csv_kep")
kep1
```
![](./img/1641138591038-bc4ccf6b-7d34-436c-8e9e-b515fd179cfd.webp)
<a name="GnWZQ"></a>
### DataFrame格式
```python
china = pd.read_csv("china.csv")
kep1.add_data(data=china, name="dataframe_kep")
kep1
```
![](./img/1641138591060-e480f153-d9be-4af8-a85d-c61a65598bba.webp)
<a name="Pp6ny"></a>
### GeoJson格式
```python
url = 'http://eric.clst.org/assets/wiki/uploads/Stuff/gz_2010_us_040_00_500k.json'
country_gdf = gpd.read_file(url)  # geopandas读取json文件

kep1.add_data(data=country_gdf, name="state")
kep1
```
![](./img/1641138591104-c4b34452-f88b-423a-b066-022dbf1bc8b5.webp)
<a name="FAM8H"></a>
## 自定义图形
Keplergl的自定义方法：关键性按钮。进去了之后就可以自定义操作<br />![](./img/1641138591274-81034109-e519-43ba-afff-5a47c00babf1.webp)<br />![](./img/1641138591439-06bceb15-3d84-4fa9-8bc8-c589d365c39e.webp)<br />![](./img/1641138591488-4a61cfd2-aea9-4282-a0f7-8afb45d33ed8.webp)
<a name="L5DYN"></a>
## 配置的保存及复用
实例化kep的配置可以保存在后面的实例对象中复用：
<a name="FwRIF"></a>
### 1、保存
```python
# 保存成文件
with open('config1.py','w') as f:
    f.write('config={}'.format(kep1.config))
    
# 运行：魔法命令%run
%run config1.py
```
<a name="nNWau"></a>
### 2、复用
```python
kep2 = KeplerGl(height=400,
                data={"layer1":df},
                config=kep1.config  # kep1的配置
               )
kep2
```
<a name="vx8VX"></a>
## 保存图形
<a name="nF8a9"></a>
### 1、简约版本，主要是文件名
```python
kep1.save_to_html(file_name="first_kep.html")
```
<a name="DWXm1"></a>
### 2、完整版本：文件名、配置、数据、可读性
```python
# 4个参数
kep1.save_to_html(file_name="first_kep.html",
                  data={'data_1':china},
                  config=config,
                  read_only=True
                 )
```
<a name="xx4Di"></a>
## 在线操作
上面显示的操作都是在notebook中完成的，也可以直接在线操作：[https://kepler.gl/demo](https://kepler.gl/demo)
