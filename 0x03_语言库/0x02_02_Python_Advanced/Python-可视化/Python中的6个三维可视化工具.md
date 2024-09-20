Python

- Python拥有很多优秀的三维图像可视化工具，主要基于图形处理库WebGL、OpenGL或者VTK。
- 这些工具主要用于大规模空间标量数据、向量场数据、张量场数据等等的可视化，实际运用场景主要在海洋大气建模、飞机模型设计、桥梁设计、电磁场分析等等。
- 工具背后的算法逻辑非常复杂，由于小编是非专业的，不敢造次 。
- 这里简单介绍几个Python三维图像可视化工具，工具都有大量demo、完善的使用文档、功能非常强大，系统学习请戳文中链接。
<a name="F8VHX"></a>
## pyvista
专注于3D可视化和mesh分析，底层是VTK<br />![Orbiting1](./img/1650690469953-f9bc2ba3-94e5-4371-bbf6-229a8cb9554e.gif "Orbiting1")<br />![Orbiting1](./img/1650690469947-6bbde367-f0b1-4f9b-9b98-885004e7967c.gif "Orbiting1")<br />![支持GIF/MP4小电影](./img/1650690469901-fa710e72-8da6-4601-83fb-7ba23c3cf3d8.gif "支持GIF/MP4小电影")<br />![支持多种主题配色](./img/1650690469971-f5ed7d8d-4931-40a9-bf2a-b5f3e2855c5a.png "支持多种主题配色")<br />![支持多种主题配色](./img/1650690469973-e021af73-066e-45d2-89a9-3367a7ccaee0.png "支持多种主题配色")<br />![Compute Gradients of a Field](./img/1650690470688-9a60f64e-b2bc-41e4-8757-332dbe407b06.png "Compute Gradients of a Field")<br />![Add Background](./img/1650690470745-1ab3244b-61ec-440d-85ab-367e22083427.png "Add Background")<br />![Plot with Opacity](./img/1650690470877-188f5283-289e-4f20-947c-d884892ae4ba.png "Plot with Opacity")<br />![more......](./img/1650690471475-bc495492-f571-437e-8441-420b3309e7d5.png "more......")

- 进一步学习：[https://github.com/pyvista/pyvista](https://github.com/pyvista/pyvista)

---

<a name="THU5j"></a>
## Glumpy
作者为Nicolas P. Rougier，Glumpy是Python、Numpy和OpenGL的完美结合，支持GPU加速，让可视化变得快速、美观、动态、可交互等。<br />![银河系动态模拟](./img/1650690471467-8cdc1d3c-762a-4d23-9960-7dbf94e69c83.gif "银河系动态模拟")<br />![3D曲面图](./img/1650690471618-3c9ba46b-d537-4f9c-b68e-42547462ba56.png "3D曲面图")<br />![more......](./img/1650690472109-83eece8c-69df-46ca-ad6b-ba43c0c34b77.png "more......")

- 进一步学习：[https://github.com/glumpy/glumpy](https://github.com/glumpy/glumpy)

---

<a name="gZVM3"></a>
## itkwidgets
itkwidgets是Jupyter的一个交互式小部件，主要运用于可视化图像、点集和mesh。<br />![小demo](./img/1650690472209-16001063-ba01-47b2-afae-e8e2d6bdb7d8.gif "小demo")

- 进一步学习：[https://github.com/InsightSoftwareConsortium/itkwidgets](https://github.com/InsightSoftwareConsortium/itkwidgets)

---

<a name="E05v4"></a>
## vedo
几行代码实现3D point clouds, meshes and volumes<br />![1D, 2D and 3D histogramming](./img/1650690472514-f5e39854-192e-46ef-8452-d120e87b16ef.png "1D, 2D and 3D histogramming")<br />![more......](./img/1650690472670-6c1d4347-0250-45b6-a9cc-f77bf7715edb.gif "more......")

- 进一步学习：[https://github.com/marcomusy/vedo](https://github.com/marcomusy/vedo)

---

<a name="tBlNg"></a>
## ipyvolume
可在Jupyter Notebook中渲染3D图，基于WebGL，功能较少。<br />![小demo](./img/1650690472728-32466362-2abd-4c07-98be-8f01d0bf398e.gif "小demo")<br />进一步学习：[https://github.com/widgetti/ipyvolume](https://github.com/widgetti/ipyvolume)

---

<a name="Ocffm"></a>
## mayavi
![](./img/1650690473203-a486089a-4355-4644-8f6f-153aa95744d2.png)mayavi主要有两大部分功能：

- `mayavi.mlab`用于处理图形可视化和图形操作，类似于matlab和matplotlib；
- `mayavi.tools.pipeline`用于操作管线对象窗口对象。

![z-warped surface](./img/1650690473307-694431e8-22be-4d45-8aeb-c3d5e6e69e44.png "z-warped surface")<br />![protein graph structure](./img/1650690473428-18654b4a-f37a-4b0f-be9e-87c3b7e47c91.png "protein graph structure")<br />![modify the data visualized via an interactive dialog.](./img/1650690473983-b296b893-93f5-45ea-b782-34d4ed809e48.png "modify the data visualized via an interactive dialog.")

- 进一步学习：[http://docs.enthought.com/mayavi/mayavi/index.html](http://docs.enthought.com/mayavi/mayavi/index.html)
