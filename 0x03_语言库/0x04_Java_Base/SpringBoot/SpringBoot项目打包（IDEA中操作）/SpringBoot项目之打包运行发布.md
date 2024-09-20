Java SpringBoot 
<a name="PQoZA"></a>
## 1、使用使用IDEA提供的run运行时缺少依赖，并不能显示错误日志
<a name="z3oBf"></a>
## 2、打包后的项目显示无法找到加载主类
<a name="ogJCC"></a>
### 原因
IDEA提供的运行机制不够完善
<a name="KJcSB"></a>
### 解决办法
使用SpringBoot通过的Maven插件进行运行以及打包，这样会将父级依赖以及多重依赖完整的编译运行<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1576117581202-9240fe22-2793-411b-b806-4f5ef92f7f18.png#averageHue=%23f3f0ee&height=728&id=PdkFq&originHeight=728&originWidth=1366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=209183&status=done&style=none&title=&width=1366)
<a name="O0hKb"></a>
## 3、打包后显示父级测试类缺少
<a name="phaUH"></a>
### 原因
打包时未忽略Test依赖
<a name="SxSuP"></a>
### 解决办法
选择Maven控制窗口，点击异常图标，即可将测试类及依赖忽略，再使用Maven仓库中SpringBoot的打包插件即可<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1576118124377-d6b4a9bf-6476-4281-acb9-6eaae1ce2633.png#averageHue=%23f4f2ef&height=728&id=S0W1t&originHeight=728&originWidth=1366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=204150&status=done&style=none&title=&width=1366)

