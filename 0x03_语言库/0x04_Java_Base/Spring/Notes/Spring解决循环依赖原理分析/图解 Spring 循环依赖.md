Java Spring 循环依赖
<a name="GhvqI"></a>
## 前言
Spring如何解决的循环依赖，是近两年流行起来的一道Java面试题。
<a name="YfxTn"></a>
## 正文
通常来说，如果问Spring内部如何解决循环依赖，一定是单默认的单例Bean中，属性互相引用的场景。<br />比如几个Bean之间的互相引用：<br />![2021-06-02-13-36-53-494602.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622612407479-25ca2661-95f1-4e0c-9b1a-5d5adfb54237.png#clientId=u3f99ab37-527e-4&from=ui&id=ud07d90e9&originHeight=766&originWidth=640&originalType=binary&size=1473938&status=done&style=shadow&taskId=u3a13897c-c460-426c-ab7d-82a6f61c161)<br />甚至自己“循环”依赖自己：<br />![2021-06-02-13-36-53-591346.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622612419518-8c5b3531-5d73-45ce-9389-e7a850eb1090.png#clientId=u3f99ab37-527e-4&from=ui&id=ue1a0a7ab&originHeight=507&originWidth=640&originalType=binary&size=975592&status=done&style=shadow&taskId=u7b3dd7ce-23d7-4c0d-99ff-229fba4299c)<br />先说明前提：原型(Prototype)的场景是不支持循环依赖的，通常会走到`AbstractBeanFactory`类中下面的判断，抛出异常。
```java
if (isPrototypeCurrentlyInCreation(beanName)) {
  throw new BeanCurrentlyInCreationException(beanName);
}
```
原因很好理解，创建新的A时，发现要注入原型字段B，又创建新的B发现要注入原型字段A...<br />这就套娃了, 猜一下是先StackOverflow还是OutOfMemory？<br />Spring怕你不好猜，就先抛出了BeanCurrentlyInCreationException<br />基于构造器的循环依赖，就更不用说了，官方文档都摊牌了，想让构造器注入支持循环依赖，是不存在的，不如把代码改了。<br />那么默认单例的属性注入场景，Spring是如何支持循环依赖的？
<a name="bHTfj"></a>
### Spring解决循环依赖
首先，Spring内部维护了三个Map，也就是通常说的三级缓存。<br />翻阅Spring文档倒是没有找到三级缓存的概念，可能也是本土为了方便理解的词汇。<br />在Spring的`DefaultSingletonBeanRegistry`类中，可以发现类上方挂着这三个Map：

- `singletonObjects` 俗称“单例池”“容器”，缓存创建完成单例Bean的地方。
- `singletonFactories` 映射创建Bean的原始工厂
- `earlySingletonObjects` 映射Bean的早期引用，也就是说在这个Map里的Bean不是完整的，甚至还不能称之为“Bean”，只是一个`Instance`。

后两个Map其实是“垫脚石”级别的，只是创建Bean的时候，用来借助了一下，创建完成就清掉了。<br />所以前文对“三级缓存”这个词有些迷惑，可能是因为注释都是以Cache of开头吧。<br />为什么成为后两个Map为垫脚石，假设最终放在`singletonObjects`的Bean想要的一杯“凉白开”。<br />那么Spring准备了两个杯子，即`singletonFactories`和`earlySingletonObjects`来回“倒腾”几番，把热水晾成“凉白开”放到`singletonObjects`中。<br />![2021-06-02-13-36-53-921459.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622612545583-a8f52338-4ba5-4e59-8724-4f9c9f6e2749.gif#clientId=u3f99ab37-527e-4&from=ui&id=ucfe1479b&originHeight=371&originWidth=646&originalType=binary&size=503828&status=done&style=shadow&taskId=ua6b1dff9-dd80-47f7-90b3-947e7132520)<br />上面的GIF三秒一帧，比较慢。<br />笔者画了17张图简化表述了Spring的主要步骤，GIF上方即是刚才提到的三级缓存，下方展示是主要的几个方法。<br />当然了，这个地步肯定要结合Spring源码来看，要不肯定看不懂。<br />如果只是想大概了解，或者面试，可以先记住上文提到的“三级缓存”，以及下文即将要说的本质。
<a name="Z0deJ"></a>
### 循环依赖的本质
上文了解完Spring如何处理循环依赖之后，跳出“阅读源码”的思维，假设实现一个有以下特点的功能，怎么做？

- 将指定的一些类实例为单例
- 类中的字段也都实例为单例
- 支持循环依赖

举个例子，假设有类A：
```java
public class A {
    private B b;
}
```
类B：
```java
public class B {
    private A a;
}
```
说白了让你模仿Spring：假装A和B是被`@Component`修饰， 并且类中的字段假装是`@Autowired`修饰的，处理完放到Map中。<br />其实非常简单，写了一份简单的代码，可供参考：
```java
/**
 * 放置创建好的bean Map
*/
private static Map<String, Object> cacheMap = new HashMap<>(2);

public static void main(String[] args) {
    // 假装扫描出来的对象
    Class[] classes = {A.class, B.class};
    // 假装项目初始化实例化所有bean
    for (Class aClass : classes) {
        getBean(aClass);
    }
    // check
    System.out.println(getBean(B.class).getA() == getBean(A.class));
    System.out.println(getBean(A.class).getB() == getBean(B.class));
}

@SneakyThrows
private static <T> T getBean(Class<T> beanClass) {
    // 本文用类名小写 简单代替bean的命名规则
    String beanName = beanClass.getSimpleName().toLowerCase();
    // 如果已经是一个bean，则直接返回
    if (cacheMap.containsKey(beanName)) {
        return (T) cacheMap.get(beanName);
    }
    // 将对象本身实例化
    Object object = beanClass.getDeclaredConstructor().newInstance();
    // 放入缓存
    cacheMap.put(beanName, object);
    // 把所有字段当成需要注入的bean，创建并注入到当前bean中
    Field[] fields = object.getClass().getDeclaredFields();
    for (Field field : fields) {
        field.setAccessible(true);
        // 获取需要注入字段的class
        Class<?> fieldClass = field.getType();
        String fieldBeanName = fieldClass.getSimpleName().toLowerCase();
        // 如果需要注入的bean，已经在缓存Map中，那么把缓存Map中的值注入到该field即可
        // 如果缓存没有 继续创建
        field.set(object, cacheMap.containsKey(fieldBeanName)
                  ? cacheMap.get(fieldBeanName) : getBean(fieldClass));
    }
    // 属性填充完成，返回
    return (T) object;
}
```
这段代码的效果，其实就是处理了循环依赖，并且处理完成后，cacheMap中放的就是完整的“Bean”了<br />![2021-06-02-13-36-54-029171.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622612629349-d4214bd6-c547-4328-8e06-7aa2ba587be6.jpeg#clientId=u3f99ab37-527e-4&from=ui&id=uc91fab30&originHeight=470&originWidth=640&originalType=binary&size=35462&status=done&style=shadow&taskId=u5a89c1d1-e32c-4bed-92ac-b9038981b75)<br />这就是“循环依赖”的本质，而不是“Spring如何解决循环依赖”。<br />如果真看不懂，不如先写出基础版本，逆推Spring为什么要这么实现，可能效果会更好。
<a name="NOX7I"></a>
#### 问题的本质居然是two sum！
看完刚才的代码有没有似曾相识？没错，和two sum的解题是类似的。<br />two sum是刷题网站leetcode序号为1的题，也就是大多人的算法入门的第一题。<br />常常被人调侃，有算法面的公司，被面试官钦定了，合的来。那就来一道two sum走走过场。<br />问题内容是：给定一个数组，给定一个数字。返回数组中可以相加得到指定数字的两个索引。<br />比如：给定`nums = [2, 7, 11, 15], target = 9`那么要返回 `[0, 1]`，因为`2 + 7 = 9`<br />这道题的优解是，一次遍历+HashMap：
```java
class Solution {
    public int[] twoSum(int[] nums, int target) {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            int complement = target - nums[i];
            if (map.containsKey(complement)) {
                return new int[] { map.get(complement), i };
            }
            map.put(nums[i], i);
        }
        throw new IllegalArgumentException("No two sum solution");
    }
}
```
先去Map中找需要的数字，没有就将当前的数字保存在Map中，如果找到需要的数字，则一起返回。<br />和上面的代码是不是一样？<br />先去缓存里找Bean，没有则实例化当前的Bean放到Map，如果有需要依赖当前Bean的，就能从Map取到。
