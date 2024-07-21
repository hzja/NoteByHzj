Python 可视化 Pyecharts
<a name="JL3jg"></a>
### Theme图表背景
Pyecharts模块内部内置了10多种不同风格的图表绘制样式，分别是
```python
LIGHT = "light"
DARK = "dark"
WHITE = "white"
CHALK: str = "chalk"
ESSOS: str = "essos"
INFOGRAPHIC: str = "infographic"
MACARONS: str = "macarons"
PURPLE_PASSION: str = "purple-passion"
ROMA: str = "roma"
ROMANTIC: str = "romantic"
SHINE: str = "shine"
VINTAGE: str = "vintage"
WALDEN: str = "walden"
WESTEROS: str = "westeros"
WONDERLAND: str = "wonderland"
HALLOWEEN: str = "halloween"
```
依次来看一下每一种风格出来的样子，这次用到的数据集依然是Pyecharts模块当中内置的模块，当然首先需要导入相对应的模块
```python
from pyecharts import options as opts
from pyecharts.charts import Bar, Page
from pyecharts.faker import Collector, Faker
from pyecharts.globals import ThemeType
```
<a name="QGKQ3"></a>
### Light
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.LIGHT))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Light"))
    )

c.render("1.html")
```
output<br />![](./img/1637731503588-ee9e67b6-9bd6-45c6-8ddc-31568d7f1f39.webp)
<a name="HiANh"></a>
### Dark
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.DARK))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Dark"))
    )

c.render("2.html")
```
output<br />![](./img/1637731503487-fdb06e16-7df4-4370-bcb7-4c6dae04d6bc.webp)
<a name="Da2Ip"></a>
### Chalk
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.CHALK))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Chalk"))
    )

c.render("3.html")
```
output<br />![](./img/1637731503588-b4ac79d1-cbe0-4fd0-a62c-99d911de2cfe.webp)
<a name="lAxkp"></a>
### Essos
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.ESSOS))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Essos"))
    )

c.render("4.html")
```
output<br />![](./img/1637731503580-a6e6b845-3940-4566-b4ef-8cff819b2725.webp)
<a name="Hol6y"></a>
### Infographic
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.INFOGRAPHIC))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Infographic"))
    )

c.render("5.html")
```
output<br />![](./img/1637731503650-66b3edfc-6ac1-4d8f-85cc-915f59adbf9b.webp)
<a name="DC4YM"></a>
### Macarons
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.MACARONS))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Macarons"))
    )

c.render("6.html")
```
output<br />![](./img/1637731504107-b37a63df-58c9-455c-90a6-a41dbb675a34.webp)

- Purple_Passion
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.PURPLE_PASSION))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-PURPLE_PASSION"))
    )

c.render("7.html")
```
output<br />![](./img/1637731504148-beb3ad13-d405-47c2-a54b-e9b80b97774b.webp)
<a name="HITMK"></a>
### Roma
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-ROMA"))
    )

c.render("8.html")
```
output<br />![](./img/1637731504104-f1892928-10a4-46da-b01e-a5a1ce7a59f9.webp)
<a name="xmxB4"></a>
### Romantic
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.ROMANTIC))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-ROMANTIC"))
    )

c.render("9.html")
```
output<br />![](./img/1637731504208-45290196-52d5-4c02-931b-9db4f09fee96.webp)
<a name="FLIsS"></a>
### Shine
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.SHINE))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Shine"))
    )

c.render("10.html")
```
output<br />![](./img/1637731504388-c30af513-95f2-4571-94b6-484c1e0ff2de.webp)
<a name="czwPs"></a>
### Vintage
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.VINTAGE))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Vintage"))
    )

c.render("11.html")
```
output<br />![](./img/1637731504609-3a3bbf0a-829e-4e30-b1f7-9ffdea72bad7.webp)
<a name="w7Y01"></a>
### Walden
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.WALDEN))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Walden"))
    )

c.render("12.html")
```
output<br />![](./img/1637731504682-5625816a-4b12-471b-ab6f-447341ad1ce4.webp)
<a name="x160Q"></a>
### Westeros
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.WESTEROS))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Westeros"))
    )

c.render("13.html")
```
output

![](./img/1637731504477-bb37d249-54ed-4a5e-bc2d-3f02e21424b3.webp)
<a name="hJXcW"></a>
### Wonderland
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.WONDERLAND))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Wonderland"))
    )

c.render("14.html")
```
output<br />![](./img/1637731504750-11cd5d97-d46c-4998-86b3-8e4716e922a9.webp)
<a name="JrVbe"></a>
### Halloween
```python
c = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.HALLOWEEN))
        .add_xaxis(Faker.choose())
        .add_yaxis("商家A", Faker.values())
        .add_yaxis("商家B", Faker.values())
        .add_yaxis("商家C", Faker.values())
        .add_yaxis("商家D", Faker.values())
        .set_global_opts(title_opts=opts.TitleOpts("Theme-Halloween"))
    )

c.render("15.html")
```
output<br />![](./img/1637731505148-41431336-c5bb-41a9-9921-3fbce6d15694.webp)
<a name="ZRxnN"></a>
### 图片带有背景图
同时还能为自己绘制的图表配上自带的背景图片
```python
c = (
    Bar(
        init_opts=opts.InitOpts(
            bg_color={"type": "pattern", "image": JsCode("img"), "repeat": "no-repeat"}
        )
    )
    .add_xaxis(Faker.choose())
    .add_yaxis("商家A", Faker.values())
    .add_yaxis("商家B", Faker.values())
    .set_global_opts(
        title_opts=opts.TitleOpts(
            title="Bar-背景图基本示例",
            subtitle="我是副标题",
            title_textstyle_opts=opts.TextStyleOpts(color="white"),
        )
    )
)
c.add_js_funcs(
    """
    var img = new Image(); img.src = 'https://t7.baidu.com/it/u=2638406194,523661981&fm=193&f=GIF';
    """
)
c.render("柱状图-自带背景图.html")
```
output<br />![](./img/1637731505134-c5102329-8fe0-4301-a536-b9dd1bc3e6c7.webp)
