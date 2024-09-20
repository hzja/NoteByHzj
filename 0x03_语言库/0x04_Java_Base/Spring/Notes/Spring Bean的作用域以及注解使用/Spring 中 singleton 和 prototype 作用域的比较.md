JavaSpring
<a name="UwGJs"></a>
## Spring Bean 作用域
主要了解 singleton 和 prototype 就好了，他们分别使用 单例模式 和 原型模式

| 来源 | 说明 |
| --- | --- |
| singleton | 默认作用域，一个BeanFactory有且仅有一个实例（并不是在JVM进程里是唯一的） |
| prototype | 原型作用域，每次以来查找和依赖注入生成新的Bean对象 |
| request | 将Spring Bean存储在ServletRequest上下文中 |
| session | 将Spring Bean存储在HttpSession中 |
| application | 将Spring Bean 存储在ServletContext中 |

<a name="haJ0Y"></a>
## 依赖查找：比较 singleton 和 prototype 两种作用域
:::success
结论：使用依赖查找时，singleton会拿到相同的对象，prototype 会拿到不同的对象
:::
首先将User对象通过 singleton 和prototype 两种方式分别注入到容器中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911108-1de23db4-8f2c-4cc8-9e31-3b2001d3d3c3.png#averageHue=%232a2b32&clientId=u0ada6233-f6ce-4&from=paste&id=uab31e6be&originHeight=659&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua58d8341-bb02-439c-89ee-48252149b94&title=)<br />三次拿到User对象，比较结果（比较name属性）<br />观察可以看到，使用 singleton 作用域拿到的是相同的对象，prototype 作用域拿到的是不同的对象<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911122-eb897694-ea7f-4892-85f1-23d4a487f475.png#averageHue=%232a2c32&clientId=u0ada6233-f6ce-4&from=paste&id=u3350ca21&originHeight=484&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc22b50e7-2af4-4420-ac9d-033ce500675&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911136-c440d3e4-a007-4e0a-9ec6-fb48058448aa.png#averageHue=%23323130&clientId=u0ada6233-f6ce-4&from=paste&id=u3650df09&originHeight=310&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2c85a653-a20a-4f86-bdc6-062e697c0cd&title=)
<a name="toyFI"></a>
## 依赖注入 比较 singleton 和 prototype 两种作用域
:::success
结论：singleton 作用域拿到的是相同的对象，prototype 作用域拿到的仍然是不同的对象
:::
通过`@Autowired`方式依赖查找对象<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911106-4e14cf13-8225-4802-becb-2cdb31608d03.png#averageHue=%23292c32&clientId=u0ada6233-f6ce-4&from=paste&id=ufb75eb70&originHeight=781&originWidth=724&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u79f546f6-907f-493c-946a-9bf6cda72e0&title=)<br />把他们打印出来看看，这样子会报错，因为容器中有重复的对象，没有指定哪个是primary<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911109-513f750f-9cca-4ee0-b500-f90d4b4f8f72.png#averageHue=%232a2c33&clientId=u0ada6233-f6ce-4&from=paste&id=u8df12dc6&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud3b33395-0964-4a15-b8ac-dfe38d56b84&title=)<br />如果使用 `@Qualifier` 注解指定要注入的对象名称，就不会报错了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911697-2659a467-ace5-4a7c-b79d-73b1f6a6c1f8.png#averageHue=%23292b32&clientId=u0ada6233-f6ce-4&from=paste&id=u2da0b9d9&originHeight=943&originWidth=709&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5feada13-c27c-4bd5-98c8-363e1f7651d&title=)<br />输出结果如下：可以看到，singleton 作用域拿到的是相同的对象，prototype 作用域拿到的仍然是不同的对象<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911506-53bb0310-d3f9-409c-b68f-eba0b54f16bf.png#averageHue=%2331302f&clientId=u0ada6233-f6ce-4&from=paste&id=u6982b69e&originHeight=377&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u31a92396-0058-42a2-8abb-4ec90535d2b&title=)
<a name="Hmles"></a>
## 依赖注入集合时 singleton 和 prototype 作用域的比较
:::success
结论：集合中会同时存在一份 singleton bean 和 prototype bean
:::
使用`@Autowired`直接注入map集合<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911545-70af8436-c380-4d67-b98b-07ef76246a08.png#averageHue=%232a2d34&clientId=u0ada6233-f6ce-4&from=paste&id=u8adb4e28&originHeight=178&originWidth=821&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc0b5e8b-6e17-4e17-8e8f-b5ab6d63919&title=)<br />输出，打印，可以看到 singleton 和 prototype 作用域的对象同时存在<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911637-b2c8f30e-e516-4f85-8f12-884c46c19562.png#averageHue=%232a2c32&clientId=u0ada6233-f6ce-4&from=paste&id=u3684aff2&originHeight=431&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud6d20474-0789-4b44-b5f9-71a46d4c18f&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911670-4d3dbeb9-1005-463d-8d2b-0526a55c7fc4.png#averageHue=%232f2f2e&clientId=u0ada6233-f6ce-4&from=paste&id=u76bc1c75&originHeight=177&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udb2b2507-5c32-4b83-9c5b-44b620889b7&title=)
<a name="Hsb06"></a>
## singleton 和 prototype 作用域的生命周期方法的回调情况
:::success
结论：singleton 和 prototype 都会执行初始化方法回调，但仅 singleton 会执行销毁方法回调
:::
为了方便看到 beanName，User类实现了 BeanNameAware 接口，并定义了 初始化 和 销毁方法，具体如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911890-fb15573d-21bf-41fe-80fd-dcf11d9d1736.png#averageHue=%23292b31&clientId=u0ada6233-f6ce-4&from=paste&id=u50ff100d&originHeight=1255&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3e849996-824d-433e-bab7-3ece3b293c9&title=)<br />仍然使用依赖注入集合的方式同时拿到 singleton 和 prototype 作用域中的 User bean，并将他们输出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415911994-9816f87b-7374-4878-9655-f023c5150d58.png#averageHue=%232a2c32&clientId=u0ada6233-f6ce-4&from=paste&id=ueda2cc82&originHeight=537&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud59b11c9-cfac-44f3-bb94-c3aad444d24&title=)<br />输出看看，可以看到 prototype bean 的销毁方法并没有被回调<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677415912049-39e444ec-268d-4cea-b582-aaa62e280e4f.png#averageHue=%232e2d2d&clientId=u0ada6233-f6ce-4&from=paste&id=u2e335e08&originHeight=243&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u07be885b-1548-4196-86c3-2166e7090c1&title=)
