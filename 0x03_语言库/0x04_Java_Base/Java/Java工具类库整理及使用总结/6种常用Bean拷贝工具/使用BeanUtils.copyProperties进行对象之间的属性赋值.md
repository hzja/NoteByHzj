Java
<a name="oqVkA"></a>
## 1、使用`org.springframework.beans.BeanUtils.copyProperties`方法进行对象之间属性的赋值，避免通过`get`、`set`方法一个一个属性的赋值
```java
/**
 * 对象属性拷贝 <br>
 * 将源对象的属性拷贝到目标对象
 *
 * @param source 源对象
 * @param target 目标对象
 */
public static void copyProperties(Object source, Object target) {
    try {
        BeanUtils.copyProperties(source, target);
    } catch (BeansException e) {
        LOGGER.error("BeanUtil property copy  failed :BeansException", e);
    } catch (Exception e) {
        LOGGER.error("BeanUtil property copy failed:Exception", e);
    }
}
```
<a name="fzEGN"></a>
## 2、List集合之间的对象属性赋值
```java
/**
 * @param input 输入集合
 * @param clzz  输出集合类型
 * @param <E>   输入集合类型
 * @param <T>   输出集合类型
 * @return 返回集合
 */
public static <E, T> List<T> convertList2List(List<E> input, Class<T> clzz) {
    List<T> output = Lists.newArrayList();
    if (CollectionUtils.isNotEmpty(input)) {
        for (E source : input) {
            T target = BeanUtils.instantiate(clzz);
            BeanUtil.copyProperties(source, target);
            output.add(target);
        }
    }
    return output;
}
```
比如有两个类，User和Employee，将存储Employee对象的List赋给存储User对象的List。<br />User类：
```java
public class User {
    private String name;
    private Integer age;
 
    public String getName() {
        return name;
    }
 
    public void setName(String name) {
        this.name = name;
    }
 
    public Integer getAge() {
        return age;
    }
 
    public void setAge(Integer age) {
        this.age = age;
    }
 
    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
```
Employee类：
```java
public class Employee {
    private String name;
 
    private Integer age;
    private String dept;
 
    public Employee(String name, Integer age, String dept) {
        this.name = name;
        this.age = age;
        this.dept = dept;
    }
 
    public String getName() {
        return name;
    }
 
    public void setName(String name) {
        this.name = name;
    }
 
    public Integer getAge() {
        return age;
    }
 
    public void setAge(Integer age) {
        this.age = age;
    }
 
    public String getDept() {
        return dept;
    }
 
    public void setDept(String dept) {
        this.dept = dept;
    }
 
    @Override
    public String toString() {
        return "Employee{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", dept='" + dept + '\'' +
                '}';
    }
}
```
测试类：
```java
@RunWith(PowerMockRunner.class)
public class TestUtil
{
    @Test
    public void test(){
        Employee ee1=new Employee("A",21,"it");
        Employee ee2=new Employee("B",23,"account");
        User user=new User();
        BeanUtil.copyProperties(ee1, user);
        System.out.println(user);
        System.out.println("-------------分割线--------------");
        List<User> output=new ArrayList<>();
        List<Employee> source= Arrays.asList(ee1,ee2);
        output=BeanUtil.convertList2List(source,User.class);
        for (User str:output) {
            System.out.println(str);
        }
    }
}
```
![2021-07-21-21-47-24-092229.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626875285847-26c1fde0-9cbd-4e6c-8743-b2cb2378db98.png#clientId=uf7d1ff35-c198-4&from=ui&id=u8d2ba759&originHeight=287&originWidth=914&originalType=binary&ratio=1&size=788597&status=done&style=shadow&taskId=ud1446a46-f306-40fc-aa2d-6a156620b8d)
