JavaSpring<br />Spring作为项目中不可缺少的底层框架，提供的最基础的功能就是bean的管理了。bean的注入相信大家都比较熟悉了，但是有几种不太常用到的集合注入方式，可能有的同学会不太了解，通过实例看看它的使用。<br />首先，声明一个接口：
```java
public interface UserDao {  
    String getName();  
}
```
然后定义两个类来分别实现这个接口，并通过`@Component`注解把bean放入spring容器中：
```java
@Component  
public class UserDaoA implements UserDao {  
    @Override  
    public String getName() {  
        return "Hydra";  
    }  
}
```
```java
@Component  
public class UserDaoB implements UserDao {  
    @Override  
    public String getName() {  
        return "Hello";  
    }  
}
```
准备工作完成后，看看几种不同类型的集合注入方式。
<a name="s0bjT"></a>
## Map注入
首先来看Map类型的注入，直接在Service中注入一个Map，key为字符串类型，value为上面定义的接口类型。
```java
@Service  
@AllArgsConstructor  
public class UserMapService {  
    final Map<String, UserDao> userDaoMap;  
  
    public Map<String,UserDao> getDaos(){  
        return userDaoMap;  
    }  
}
```
通过接口测试，查看这个Map中的内容：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401286-c154289f-a601-4b60-8770-eaeb9257294e.png#clientId=u01453f26-cda3-4&from=paste&id=ub7984663&originHeight=353&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc8df4fb7-540e-4cbc-a84a-0c288466927&title=)<br />可以看到，Map中的value是实现了接口的实例对象，key则是beanName，可以通过`@Component`的`value`属性进行自定义。<br />修改UserDaoA，指定名称：
```java
@Component(value = "Hydra")  
public class UserDaoA implements UserDao {...}
```
可以看到，key的值发生了改变：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401294-6d9a2649-efd4-4123-8494-bc99afbe6639.png#clientId=u01453f26-cda3-4&from=paste&id=uf55965df&originHeight=316&originWidth=696&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u37133e34-3b5f-4b65-8723-a0a35e903bc&title=)
<a name="ucA5l"></a>
## List注入
在Service中，这次注入泛型为接口UserDao类型的List。
```java
@Service  
@AllArgsConstructor  
public class UserListService {  
    private final List<UserDao> userDaoLists;  
  
    public List<UserDao> getDaos(){  
        return userDaoLists;  
    }  
}
```
测试这个方法，查看List中的内容，是放入容器中的两个bean：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401373-ca800d02-2f9c-4545-b246-930375896851.png#clientId=u01453f26-cda3-4&from=paste&id=u67432f57&originHeight=232&originWidth=610&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua5a6b2d9-c79e-4b11-b5d1-c11409be413&title=)<br />List是一个有序的数据结构，那么如果想要修改List中bean的排序，该如何做呢？<br />很简单，修改注入到spring容器中的两个bean，为它们添加`@Order`注解并指定加载顺序，数字越小越优先加载。
```java
@Component  
@Order(1)  
public class UserDaoA implements UserDao {……}
```
```java
@Component  
@Order(-1)  
public class UserDaoB implements UserDao {……}
```
修改完成后，再进行测试，可以看到bean的顺序发生了改变：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401294-8311d413-17f4-48a5-a16c-1ee6c5e4aff3.png#clientId=u01453f26-cda3-4&from=paste&id=u910b619b&originHeight=259&originWidth=618&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uafb688c1-e24a-4dc1-8775-e90a0ad28e8&title=)
<a name="AL912"></a>
## Set注入
同样，也可以使用无序的Set注入bean，泛型指定为接口类型。
```java
@Service  
@AllArgsConstructor  
public class UserSetService {  
    private final Set<UserDao> userDaoSet;  
  
    public Set<UserDao> getDaos(){  
        return userDaoSet;  
    }  
}
```
查看Set中的元素，和List相同，只不过顺序变为无序，不会因为@Order注解的值而改变：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401300-27e9f110-b5d5-4146-be30-63e81ad67da1.png#clientId=u01453f26-cda3-4&from=paste&id=ue24085a1&originHeight=243&originWidth=580&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4a0d2ca6-4815-484e-937e-df2f579c83c&title=)
<a name="niDC8"></a>
## 数组注入
最后，再来看一下数组注入的方式：
```java
@Service  
@AllArgsConstructor  
public class UserArrayService {  
    private final  UserDao[] userDaoArray;  
  
    public UserDao[] getDaos(){  
        return userDaoArray;  
    }  
}
```
查看数组中的元素：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665895401724-6417f5e4-965e-4cce-9681-7eb5558c799a.png#clientId=u01453f26-cda3-4&from=paste&id=ud364e7f7&originHeight=234&originWidth=535&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua39b7625-d6db-4d3c-8537-238369c2a79&title=)<br />并且，和List比较类似的，数组中bean的排序会受到`@Order`注解数值的影响，有兴趣的同学可以自己尝试一下。
<a name="aP2xy"></a>
## 应用
了解了这几种注入方式后，再简单提一下它的使用场景。例如，可以用Map注入实现策略模式，来替换代码中繁杂的if/else判断。例如，原始的代码中判断逻辑可能是这样的：
```java
public String choice(String name){  
    if (name.equals("auth")){  
        return "Hydra";  
    }else if (name.equals("official")){  
        return "Hello";  
    }      
    return null;  
}
```
使用策略模式进行改造，首先修改beanName：
```java
@Component(value = "auth")  
public class UserDaoA implements UserDao {  
    @Override  
    public String getName() {  
        return "Hydra";  
    }  
}
```
```java
@Component(value = "official")  
public class UserDaoB implements UserDao {  
    @Override  
    public String getName() {  
        return "Hello";  
    }  
}
```
再修改Servie中的方法，一行代码即可实现原有的if/else判断：
```java
@Service  
@AllArgsConstructor  
public class TestService {  
    final Map<String, UserDao> userDaoMap;  
  
    public String choice2(String name){  
        return userDaoMap.get(name).getName();  
    };  
}
```
可能在这个例子中，这种写法的优点体现的不十分明显，但是当有一个非常长的if/else判断时，这种模式能使代码看上去简洁很多，并且符合代码按照功能拆分的原则。<br />同理，如果已经通过`@Order`注解定义好了bean的加载顺序，也可以将它理解为bean的优先级，例如想要调用优先级最高的符合类型的bean的方法，那么完全可以这样写：
```java
@Service  
@AllArgsConstructor  
public class TestService {  
    final List<UserDao> userDaoLists;  
      
    public String choiceFirst(){  
        return userDaoLists.get(0).getName();  
    };  
}
```
通过上面两个简单的例子可以看到，集合注入的方式使用起来非常灵活，可以在实际使用中，结合各种设计模式，写出实用而优雅的代码。
