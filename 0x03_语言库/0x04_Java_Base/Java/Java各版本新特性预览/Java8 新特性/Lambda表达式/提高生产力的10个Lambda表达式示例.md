Java
<a name="fjTJs"></a>
### 前言
Lambda表达式是一种在现代编程语言中越来越常见的特性，可以简化代码、提高生产力。这里介绍10个必须掌握的Lambda表达式，这些表达式涵盖了在实际编程中经常用到的常见场景，例如列表操作、函数组合、条件筛选等。通过学习这些Lambda表达式，将更加深入地理解函数式编程的思想，能够更加高效地编写代码。
<a name="LML6b"></a>
### 1、使用Lambda表达式进行集合遍历
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
for (String fruit : list) {
    System.out.println(fruit);
}
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
list.forEach(fruit -> System.out.println(fruit));
```
<a name="MlAcb"></a>
### 2、使用Lambda表达式进行排序
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
Collections.sort(list, new Comparator<String>() {
    public int compare(String s1, String s2) {
        return s1.compareTo(s2);
    }
});
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
Collections.sort(list, (s1, s2) -> s1.compareTo(s2));
```
<a name="ueaQM"></a>
### 3、使用Lambda表达式进行过滤
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<String> filteredList = new ArrayList<String>();
for (String fruit : list) {
    if (fruit.startsWith("a")) {
        filteredList.add(fruit);
    }
}
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<String> filteredList = list.stream().filter(fruit -> fruit.startsWith("a")).collect(Collectors.toList());
```
<a name="UfRdn"></a>
### 4、使用Lambda表达式进行映射
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<Integer> lengths = new ArrayList<Integer>();
for (String fruit : list) {
    lengths.add(fruit.length());
}
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<Integer> lengths = list.stream().map(fruit -> fruit.length()).collect(Collectors.toList());
```
<a name="xqNGh"></a>
### 5、使用Lambda表达式进行归约
未使用Lambda表达式：
```java
List<Integer> list = Arrays.asList(1, 2, 3, 4, 5);
int sum = 0;
for (int i : list) {
    sum += i;
}
```
使用Lambda表达式：
```java
List<Integer> list = Arrays.asList(1, 2, 3, 4, 5);
int sum = list.stream().reduce(0, (a, b) -> a + b);
```
<a name="BNfvA"></a>
### 6、使用Lambda表达式进行分组
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
Map<Integer, List<String>> grouped = new HashMap<Integer, List<String>>();
for (String fruit : list) {
    int length = fruit.length();
    if (!grouped.containsKey(length)) {
        grouped.put(length, new ArrayList<String>());
    }
    grouped.get(length).add(fruit);
}
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
Map<Integer, List<String>> grouped = list.stream().collect(Collectors.groupingBy(fruit -> fruit.length()));
```
<a name="jyR4t"></a>
### 7、使用Lambda表达式进行函数式接口的实现
未使用Lambda表达式：
```java
public interface MyInterface {
    public void doSomething(String input);
}

MyInterface myObject = new MyInterface() {
    public void doSomething(String input) {
        System.out.println(input);
    }
};
myObject.doSomething("Hello World");
```
使用Lambda表达式：
```java
MyInterface myObject = input -> System.out.println(input);
myObject.doSomething("Hello World");
```
<a name="Yl1tL"></a>
### 8、使用Lambda表达式进行线程的创建
未使用Lambda表达式：
```java
Thread thread = new Thread(new Runnable() {
    public void run() {
        System.out.println("Thread is running.");
    }
});
thread.start();
```
使用Lambda表达式：
```java
Thread thread = new Thread(() -> System.out.println("Thread is running."));
thread.start();
```
<a name="HWgIN"></a>
### 9、使用Lambda表达式进行`Optional`的操作
未使用Lambda表达式：
```java
String str = "Hello World";
if (str != null) {
    System.out.println(str.toUpperCase());
}
```
使用Lambda表达式：
```java
Optional<String> str = Optional.ofNullable("Hello World");
str.map(String::toUpperCase).ifPresent(System.out::println);
```
<a name="SqXc3"></a>
### 10、使用Lambda表达式进行Stream的流水线操作
未使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<String> filteredList = new ArrayList<String>();
for (String fruit : list) {
    if (fruit.startsWith("a")) {
        filteredList.add(fruit.toUpperCase());
    }
}
Collections.sort(filteredList);
```
使用Lambda表达式：
```java
List<String> list = Arrays.asList("apple", "banana", "orange");
List<String> filteredList = list.stream().filter(fruit -> fruit.startsWith("a")).map(String::toUpperCase).sorted().collect(Collectors.toList());
```
