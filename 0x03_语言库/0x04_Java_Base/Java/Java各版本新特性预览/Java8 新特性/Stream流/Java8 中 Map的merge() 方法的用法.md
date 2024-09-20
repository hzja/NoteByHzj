Java Map
<a name="k6TDI"></a>
### `merge()`用法初试
假设有这么一段业务逻辑，有一个学生成绩对象的列表，对象包含学生姓名、科目、科目分数三个属性，要求求得每个学生的总成绩。加入列表如下：
<a name="D5pRQ"></a>
#### 1、初始化数据
```java
private List<StudentScore> buildATestList() {
    List<StudentScore> studentScoreList = new ArrayList<>();
    StudentScore studentScore1 = new StudentScore() {{
        setStuName("张三");
        setSubject("语文");
        setScore(70);
    }};
    StudentScore studentScore2 = new StudentScore() {{
        setStuName("张三");
        setSubject("数学");
        setScore(80);
    }};
    StudentScore studentScore3 = new StudentScore() {{
        setStuName("张三");
        setSubject("英语");
        setScore(65);
    }};
    StudentScore studentScore4 = new StudentScore() {{
        setStuName("李四");
        setSubject("语文");
        setScore(68);
    }};
    StudentScore studentScore5 = new StudentScore() {{
        setStuName("李四");
        setSubject("数学");
        setScore(70);
    }};
    StudentScore studentScore6 = new StudentScore() {{
        setStuName("李四");
        setSubject("英语");
        setScore(90);
    }};
    StudentScore studentScore7 = new StudentScore() {{
        setStuName("王五");
        setSubject("语文");
        setScore(80);
    }};
    StudentScore studentScore8 = new StudentScore() {{
        setStuName("王五");
        setSubject("数学");
        setScore(85);
    }};
    StudentScore studentScore9 = new StudentScore() {{
        setStuName("王五");
        setSubject("英语");
        setScore(70);
    }};

    studentScoreList.add(studentScore1);
    studentScoreList.add(studentScore2);
    studentScoreList.add(studentScore3);
    studentScoreList.add(studentScore4);
    studentScoreList.add(studentScore5);
    studentScoreList.add(studentScore6);
    studentScoreList.add(studentScore7);
    studentScoreList.add(studentScore8);
    studentScoreList.add(studentScore9);

    return studentScoreList;
}
```
<a name="73eBA"></a>
#### 2、常规的分组计算
```java
ObjectMapper objectMapper = new ObjectMapper();
List<StudentScore> studentScoreList = buildATestList();

Map<String, Integer> studentScoreMap = new HashMap<>();
studentScoreList.forEach(studentScore -> {
    if (studentScoreMap.containsKey(studentScore.getStuName())) {
        studentScoreMap.put(studentScore.getStuName(), 
                            studentScoreMap.get(studentScore.getStuName()) + studentScore.getScore());
    } else {
        studentScoreMap.put(studentScore.getStuName(), studentScore.getScore());
    }
});

System.out.println(objectMapper.writeValueAsString(studentScoreMap));

// 结果如下：
// {"李四":228,"张三":215,"王五":235}
```
<a name="GWtnD"></a>
#### 3、`merge()`的分组计算
```java
Map<String, Integer> studentScoreMap2 = new HashMap<>();
studentScoreList.forEach(studentScore -> studentScoreMap2.merge(
    studentScore.getStuName(),
    studentScore.getScore(),
    Integer::sum));

System.out.println(objectMapper.writeValueAsString(studentScoreMap2));

// 结果如下：
// {"李四":228,"张三":215,"王五":235}
```
<a name="Xg2uP"></a>
### `merge()`简介
`merge()` 可以这么理解：它将新的值赋值到 key （如果不存在）或更新给定的key 值对应的 value，其源码如下：
```java
default V merge(K key, V value, BiFunction<? super V, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    Objects.requireNonNull(value);
    V oldValue = this.get(key);
    V newValue = oldValue == null ? value : remappingFunction.apply(oldValue, value);
    if (newValue == null) {
        this.remove(key);
    } else {
        this.put(key, newValue);
    }
    return newValue;
}
```
可以看到原理也是很简单的，该方法接收三个参数，一个 key 值，一个 value，一个 `remappingFunction` ，如果给定的key不存在，它就变成了 `put(key, value)` 。<br />但是，如果 key 已经存在一些值，`remappingFunction` 可以选择合并的方式，然后将合并得到的 newValue 赋值给原先的 key。
<a name="J6MQ7"></a>
### 使用场景
这个使用场景相对来说还是比较多的，比如分组求和这类的操作，虽然 stream 中有相关 `groupingBy()` 方法，但如果想在循环中做一些其他操作的时候，`merge()` 还是一个挺不错的选择的。
<a name="tjWdC"></a>
### 其他
除了 `merge()` 方法之外，Java 8 中 map 相关的其他方法，比如 `putIfAbsent`、`compute()` 、`computeIfAbsent()` 、`computeIfPresent`，这些方法看名字应该就知道是什么意思了，这里贴一下 `compute()`(Map.class) 的源码，其返回值是计算后得到的新值：
```java
default V compute(K key, BiFunction<? super K, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    V oldValue = this.get(key);
    V newValue = remappingFunction.apply(key, oldValue);
    if (newValue == null) {
        if (oldValue == null && !this.containsKey(key)) {
            return null;
        } else {
            this.remove(key);
            return null;
        }
    } else {
        this.put(key, newValue);
        return newValue;
    }
}
```

