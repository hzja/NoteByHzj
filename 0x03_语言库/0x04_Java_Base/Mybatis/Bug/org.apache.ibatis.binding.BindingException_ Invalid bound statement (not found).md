<a name="Cjnyl"></a>
# 1、问题分析
Mybatis中Mapper接口与Mapper的XML映射文件绑定出现问题，即根据Mapper的xml配置文件的 `namespace` 路径找不到对应的Mapper接口，要么是找到了却匹配不到。
<a name="TNZxs"></a>
# 2、综合解决办法
<a name="sEOYs"></a>
## A.Mapper接口文件名和Mapper映射文件名要一直
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1577773595883-dc736f8a-d162-4b7d-9dc0-29e8b0139ef9.png#align=left&display=inline&height=320&originHeight=320&originWidth=266&size=14868&status=done&style=none&width=266)
<a name="1ZdlG"></a>
## B.Mapper的xml映射文件由namespace指定的接口是否正确
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1577773703914-a0364186-6ed8-44b2-8bf7-c1167d4d0324.png#align=left&display=inline&height=393&originHeight=393&originWidth=1266&size=109961&status=done&style=none&width=1266)
<a name="P7UVM"></a>
## C.Mapper接口中的方法在xml映射文件未实现
<a name="fALjG"></a>
## D.接口的返回值方法为List集合，没有写对应的ResultMap接收
