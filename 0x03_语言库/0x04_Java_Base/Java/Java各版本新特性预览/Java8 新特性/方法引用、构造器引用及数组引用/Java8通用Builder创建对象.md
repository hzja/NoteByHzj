Java8 Builder
<a name="ChVhm"></a>
### new创建对象的写法
<a name="01hnx"></a>
#### 定义类GirlFirend
```java
public class GirlFriend {
    private String name;
    private int age;
    // 省略 getter & setter ...
    public static void main(String[] args) {
        GirlFriend myGirlFriend = new GirlFriend();
        myGirlFriend.setName("小美");
        myGirlFriend.setAge(18);
    }
}
```
<a name="4FHqO"></a>
#### 类属性多的情况
```java
public class GirlFriend {
    private String name;
    private int age;
    private int bust;
    private int waist;
    private int hips;
    private List<String> hobby;
    private String birthday;
    private String address;
    private String mobile;
    private String email;
    private String hairColor;
    private Map<String, String> gift;
    // 等等等等 ...
    // 省略 getter & setter ...
    public static void main(String[] args) {
        GirlFriend myGirlFriend = new GirlFriend();
        myGirlFriend.setName("小美");
        myGirlFriend.setAge(18);
        myGirlFriend.setBust(33);
        myGirlFriend.setWaist(23);
        myGirlFriend.setHips(33);
        myGirlFriend.setBirthday("2001-10-26");
        myGirlFriend.setAddress("上海浦东");
        myGirlFriend.setMobile("18688888888");
        myGirlFriend.setEmail("pretty-xiaomei@qq.com");
        myGirlFriend.setHairColor("浅棕色带点微卷");
        List<String> hobby = new ArrayList<>();
        hobby.add("逛街");
        hobby.add("购物");
        hobby.add("买东西");
        myGirlFriend.setHobby(hobby);
        Map<String, String> gift = new HashMap<>();
        gift.put("情人节礼物", "LBR 1912女王时代");
        gift.put("生日礼物", "迪奥烈焰蓝金");
        gift.put("纪念日礼物", "阿玛尼红管唇釉");
        myGirlFriend.setGift(gift);
        // 等等等等 ...
    }
}
```
<a name="8dj31"></a>
#### 使用 `new` 关键字实例化类GirlFriend 
```java
GirlFriend girlFriend = new GirlFriend{name='小美'
    , age=18
    , bust=33
    , waist=23
    , hips=33
    , hobby=[逛街, 购物, 买东西]
    , birthday='2001-10-26'
    , address='上海浦东'
    , mobile='18688888888'
    , email='pretty-xiaomei@qq.com'
    , hairColor='浅棕色带点微卷'
    , gift={情人节礼物=LBR 1912女王时代, 生日礼物=迪奥烈焰蓝金, 纪念日礼物=阿玛尼红管唇釉}
          }
```
缺点：使用new 方式创建对象时，需要根据构造器指定的参数才能正确创建对象，而且在不同场景下需要创建不同的构造器方法才能应对不同情况的实例化。可以看出其中的麻烦程度。
<a name="u5D8g"></a>
### 通过通用Builder来创建对象
通用Builder方式创建对象：适用于所有类，不需要改造原来类，不需要 lombok 插件支持。
<a name="Dsp8V"></a>
#### 示例代码
```java
public class GirlFriend {
    // 省略属性 ...
    // 省略 getter & setter ...
    // 为了演示方便，加几个聚合方法
    public void addHobby(String hobby) {
        this.hobby = Optional.ofNullable(this.hobby).orElse(new ArrayList<>());
        this.hobby.add(hobby);
    }
    public void addGift(String day, String gift) {
        this.gift = Optional.ofNullable(this.gift).orElse(new HashMap<>());
        this.gift.put(day, gift);
    }
    public void setVitalStatistics(int bust, int waist, int hips) {
        this.bust = bust;
        this.waist = waist;
        this.hips = hips;
    }
    public static void main(String[] args) {
        GirlFriend myGirlFriend = Builder.of(GirlFriend::new)
                .with(GirlFriend::setName, "小美")
                .with(GirlFriend::setAge, 18)
                .with(GirlFriend::setVitalStatistics, 33, 23, 33)
                .with(GirlFriend::setBirthday, "2001-10-26")
                .with(GirlFriend::setAddress, "上海浦东")
                .with(GirlFriend::setMobile, "18688888888")
                .with(GirlFriend::setEmail, "pretty-xiaomei@qq.com")
                .with(GirlFriend::setHairColor, "浅棕色带点微卷")
                .with(GirlFriend::addHobby, "逛街")
                .with(GirlFriend::addHobby, "购物")
                .with(GirlFriend::addHobby, "买东西")
                .with(GirlFriend::addGift, "情人节礼物", "LBR 1912女王时代")
                .with(GirlFriend::addGift, "生日礼物", "迪奥烈焰蓝金")
                .with(GirlFriend::addGift, "纪念日礼物", "阿玛尼红管唇釉")
                // 等等等等 ...
                .build();
    }
}
```
实例化和属性设置在同一条语句执行，链式操作，非常简洁！
<a name="2ggXv"></a>
#### 通用Builder的定义
```java
/**
 * 通用的 Builder 模式构建器
 *
 * @author: Fcant
 * @since 2019/8/29
 */
public class Builder<T> {
    private final Supplier<T> instantiator;
    private List<Consumer<T>> modifiers = new ArrayList<>();
    public Builder(Supplier<T> instantiator) {
        this.instantiator = instantiator;
    }
    public static <T> Builder<T> of(Supplier<T> instantiator) {
        return new Builder<>(instantiator);
    }
    public <P1> Builder<T> with(Consumer1<T, P1> consumer, P1 p1) {
        Consumer<T> c = instance -> consumer.accept(instance, p1);
        modifiers.add(c);
        return this;
    }
    public <P1, P2> Builder<T> with(Consumer2<T, P1, P2> consumer, P1 p1, P2 p2) {
        Consumer<T> c = instance -> consumer.accept(instance, p1, p2);
        modifiers.add(c);
        return this;
    }
    public <P1, P2, P3> Builder<T> with(Consumer3<T, P1, P2, P3> consumer, P1 p1, P2 p2, P3 p3) {
        Consumer<T> c = instance -> consumer.accept(instance, p1, p2, p3);
        modifiers.add(c);
        return this;
    }
    public T build() {
        T value = instantiator.get();
        modifiers.forEach(modifier -> modifier.accept(value));
        modifiers.clear();
        return value;
    }
    /**
     * 1 参数 Consumer
     */
    @FunctionalInterface
    public interface Consumer1<T, P1> {
        void accept(T t, P1 p1);
    }
    /**
     * 2 参数 Consumer
     */
    @FunctionalInterface
    public interface Consumer2<T, P1, P2> {
        void accept(T t, P1 p1, P2 p2);
    }
    /**
     * 3 参数 Consumer
     */
    @FunctionalInterface
    public interface Consumer3<T, P1, P2, P3> {
        void accept(T t, P1 p1, P2 p2, P3 p3);
    }
}
```
这个示例最多支持三个参数的设置属性方法，应对普通场景完全够用。如果要扩展也很容易，添加多个参数的Consumer即可。
