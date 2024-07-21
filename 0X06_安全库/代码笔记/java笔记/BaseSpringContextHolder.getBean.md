MyService的类，希望从Spring上下文中检索它。首先，需要在Spring配置文件中将MyService定义为bean,<br />这段代码定义了一个名为“myService”的bean，类型为com.example.MyService。
```php
<bean id="myService" class="com.example.MyService"/>
```

basespringcontexholder.getbean来检索MyService bean
```php
MyService myService = (MyService) BaseSpringContextHolder.getBean("myService");
```

```php
MyService myService = (MyService) BaseSpringContextHolder.getBean("myService");

// Call a method on MyService
myService.doSomething();
```


代码审计的时候发现个反射问题，最后是落到reflectInvokeIndicatorMethod这个方法上，BaseSpringContextHolder.getBean获取传进来spring容器中上下文已经注册的类，该类还得是IndicatorServiceImpl的类型或者它的子类才可以进行反射调用，如果是Object类型的话就可以用jdbcTemplate 他有个方法叫做execute
```php
IndicatorServiceImpl indicatorService = BaseSpringContextHolder.getBean(BeanUtil.toLowerCase(className));
```

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683127922676-21d5cef5-d93b-40a9-9fd0-eb3095162495.png#averageHue=%23f9f7f4&clientId=u97d7931c-1bba-4&from=paste&height=466&id=u6a8b3ddf&originHeight=582&originWidth=1230&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=77630&status=done&style=none&taskId=ud2741bea-da18-4df8-9fb8-3d5d7f7037b&title=&width=984)
```php
    public static Map<String, Object> reflectInvokeIndicatorMethod(String acquireMethodString, List<String> skcList) {
        try {
            // 分解类名和方法
            String[] acquireMethodArray = acquireMethodString.split("#");
            // 指标值获取类
            Class<?> indicatorServiceClass = CLASS_MAP.get(acquireMethodArray[0]);
            if (indicatorServiceClass == null) {
                indicatorServiceClass = Class.forName(acquireMethodArray[0]);
                CLASS_MAP.put(acquireMethodArray[0],indicatorServiceClass);
            }
            String[] classNameArray = acquireMethodArray[0].split(CLASS_DOT);
            // 指标值获取类名，用于获取bean
            String className = classNameArray[classNameArray.length - 1];
            IndicatorServiceImpl indicatorService = BaseSpringContextHolder.getBean(BeanUtil.toLowerCase(className));
            // 指标值的获取方法,入参固定为List<String> skcList
            Method method = METHOD_MAP.get(acquireMethodArray[1]);
            if (method == null) {
                method = indicatorServiceClass.getDeclaredMethod(acquireMethodArray[1], List.class);
                method.setAccessible(true);
                METHOD_MAP.put(acquireMethodArray[1],method);
            }
            // 反射执行指标值获取方法,出参固定为Map:key为skc、sku、sku+其他，value为指标包装对象
            Object invoke = method.invoke(indicatorService, skcList);
            return (Map<String, Object>) invoke;
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | IllegalAccessException e) {
            log.error("反射执行指标取值方法异常, invokeMethod:{}, skcList:{}, error:{}", acquireMethodString,skcList,ExceptionUtils.getStackTrace(e));
        }
```
