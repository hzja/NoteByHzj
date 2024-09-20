Java
<a name="bs3eq"></a>
## 流式编程的概念和作用
Java 流(Stream)是一连串的元素序列，可以进行各种操作以实现数据的转换和处理。流式编程的概念基于函数式编程的思想，旨在简化代码，提高可读性和可维护性。<br />Java Stream 的主要作用有以下几个方面：

1. 简化集合操作：使用传统的 for 循环或迭代器来处理集合数据可能会导致冗长而复杂的代码。而使用流式编程，能够用更直观、更简洁的方式对集合进行过滤、映射、排序、聚合等操作，使代码变得更加清晰易懂。
2. 延迟计算：流式操作允许在处理数据之前定义一系列的操作步骤，但只在需要结果时才会实际执行。这种延迟计算的特性意味着可以根据需要动态调整数据处理的操作流程，提升效率。
3. 并行处理：Java Stream 提供了并行流的支持，可以将数据分成多个块进行并行处理，从而充分利用多核处理器的性能优势，提高代码的执行速度。
4. 函数式编程风格：流式编程鼓励使用函数式编程的思想，通过传递函数作为参数或使用 Lambda 表达式来实现代码的简化和灵活性。这种函数式的编程模式有助于减少副作用，并使代码更易测试和调试。
<a name="TFzvQ"></a>
## 为什么使用流式编程可以提高代码可读性和简洁性

1. 声明式编程风格：流式编程采用了一种声明式的编程风格，只需描述想要对数据执行的操作，而不需要显式地编写迭代和控制流语句。这使得代码更加直观和易于理解，因为可以更专注地表达意图，而无需关注如何实现。
2. 链式调用：流式编程使用方法链式调用的方式，将多个操作链接在一起。每个方法都返回一个新的流对象，这样可以像“流水线”一样在代码中顺序地写下各种操作，使代码逻辑清晰明了。这种链式调用的方式使得代码看起来更加流畅，减少了中间变量和临时集合的使用。
3. 操作的组合：流式编程提供了一系列的操作方法，如过滤、映射、排序、聚合等，这些方法可以按照需要进行组合使用。可以根据具体的业务需求将这些操作串联起来，形成一个复杂的处理流程，而不需要编写大量的循环和条件语句。这种组合操作的方式使得代码更加模块化和可维护。
4. 减少中间状态：传统的迭代方式通常需要引入中间变量来保存中间结果，这样会增加代码的复杂度和维护成本。而流式编程将多个操作链接在一起，通过流对象本身来传递数据，避免了中间状态的引入。这种方式使得代码更加简洁，减少了临时变量的使用。
5. 减少循环和条件：流式编程可以替代传统的循环和条件语句的使用。例如，可以使用 `filter()` 方法进行元素的筛选，使用 `map()` 方法进行元素的转换，使用 `reduce()` 方法进行聚合操作等。这些方法可以用一行代码完成相应的操作，避免了繁琐的循环和条件逻辑，使得代码更加简洁明了。
<a name="eCrIN"></a>
## Stream 基础知识
<a name="M9KCb"></a>
### 什么是 Stream
Stream（流）是 Java 8 引入的一个新的抽象概念，它代表着一种处理数据的序列。简单来说，Stream 是一系列元素的集合，这些元素可以是集合、数组、I/O 资源或者其他数据源。<br />Stream API 提供了丰富的操作方法，可以对 Stream 中的元素进行各种转换、过滤、映射、聚合等操作，从而实现对数据的处理和操作。Stream API 的设计目标是提供一种高效、可扩展和易于使用的方式来处理大量的数据。<br />Stream 具有以下几个关键特点：

1. 数据源：Stream 可以基于不同类型的数据源创建，如集合、数组、I/O 资源等。可以通过调用集合或数组的 stream() 方法来创建一个流。
2. 数据处理：Stream 提供了丰富的操作方法，可以对流中的元素进行处理。这些操作可以按需求组合起来，形成一个流水线式的操作流程。常见的操作包括过滤（filter）、映射（map）、排序（sorted）、聚合（reduce）等。
3. 惰性求值：Stream 的操作是惰性求值的，也就是说在定义操作流程时，不会立即执行实际计算。只有当终止操作（如收集结果或遍历元素）被调用时，才会触发实际的计算过程。
4. 不可变性：Stream 是不可变的，它不会修改原始数据源，也不会产生中间状态或副作用。每个操作都会返回一个新的流对象，以保证数据的不可变性。
5. 并行处理：Stream 支持并行处理，可以通过 parallel() 方法将流转换为并行流，利用多核处理器的优势来提高处理速度。在某些情况下，使用并行流可以极大地提高程序的性能。

通过使用 Stream，可以使用简洁、函数式的方式处理数据。相比传统的循环和条件语句，Stream 提供了更高层次的抽象，使代码更具可读性、简洁性和可维护性。它是一种强大的工具，可以更有效地处理和操作集合数据。
<a name="ceIaz"></a>
### Stream 的特性和优势

1. 简化的编程模型：Stream 提供了一种更简洁、更声明式的编程模型，使代码更易于理解和维护。通过使用 Stream API，可以用更少的代码实现复杂的数据操作，将关注点从如何实现转移到了更关注想要做什么。
2. 函数式编程风格：Stream 是基于函数式编程思想设计的，它鼓励使用不可变的数据和纯函数的方式进行操作。这种风格避免了副作用，使代码更加模块化、可测试和可维护。此外，Stream 还支持 Lambda 表达式，使得代码更加简洁和灵活。
3. 惰性求值：Stream 的操作是惰性求值的，也就是说在定义操作流程时并不会立即执行计算。只有当终止操作被调用时，才会触发实际的计算过程。这种特性可以避免对整个数据集进行不必要的计算，提高了效率。
4. 并行处理能力：Stream 支持并行处理，在某些情况下可以通过 `parallel()` 方法将流转换为并行流，利用多核处理器的优势来提高处理速度。并行流能够自动将数据划分为多个子任务，并在多个线程上同时执行，提高了处理大量数据的效率。
5. 优化的性能：Stream API 内部使用了优化技术，如延迟执行、短路操作等，以提高计算性能。Stream 操作是通过内部迭代器实现的，可以更好地利用硬件资源，并适应数据规模的变化。
6. 支持丰富的操作方法：Stream API 提供了许多丰富的操作方法，如过滤、映射、排序、聚合等。这些方法可以按需求组合起来形成一个操作流程。在组合多个操作时，Stream 提供了链式调用的方式，使代码更加简洁和可读性更强。
7. 可以操作各种数据源：Stream 不仅可以操作集合类数据，还可以操作其他数据源，如数组、I/O 资源甚至无限序列。这使得可以使用相同的编程模型来处理各种类型的数据。
<a name="VUKg3"></a>
### 如何创建 Stream 对象

1. 从集合创建：可以通过调用集合的 `stream()` 方法来创建一个 Stream 对象。例如：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Stream<Integer> stream = numbers.stream();
```

2. 从数组创建：Java 8 引入了 `Arrays` 类的 `stream()` 方法，可以使用它来创建一个 Stream 对象。例如：
```java
String[] names = {"Alice", "Bob", "Carol"};
Stream<String> stream = Arrays.stream(names);
```

3. 通过 `Stream.of()` 创建：可以使用 `Stream.of()` 方法直接将一组元素转换为 Stream 对象。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
```

4. 通过 `Stream.builder()` 创建：如果不确定要添加多少个元素到 Stream 中，可以使用 `Stream.builder()` 创建一个 `Stream.Builder` 对象，并使用其 `add()` 方法来逐个添加元素，最后调用 `build()` 方法生成 Stream 对象。例如：
```java
Stream.Builder<String> builder = Stream.builder();
builder.add("Apple");
builder.add("Banana");
builder.add("Cherry");
Stream<String> stream = builder.build();
```

5. 从 I/O 资源创建：Java 8 引入了一些新的 I/O 类（如 `BufferedReader`、`Files` 等），它们提供了很多方法来读取文件、网络流等数据。这些方法通常返回一个 Stream 对象，可以直接使用。例如：
```java
Path path = Paths.get("data.txt");
try (Stream<String> stream = Files.lines(path)) {
    // 使用 stream 处理数据
} catch (IOException e) {
    e.printStackTrace();
}
```

6. 通过生成器创建：除了从现有的数据源创建 Stream，还可以使用生成器来生成元素。Java 8 中提供了 `Stream.generate()` 方法和 `Stream.iterate()` 方法来创建无限 Stream。例如：
```java
Stream<Integer> stream = Stream.generate(() -> 0); // 创建一个无限流，每个元素都是 0
Stream<Integer> stream = Stream.iterate(0, n -> n + 1); // 创建一个无限流，从 0 开始递增
```
需要注意的是，Stream 对象是一种一次性使用的对象，它只能被消费一次。一旦对 Stream 执行了终止操作（如收集结果、遍历元素），Stream 就会被关闭，后续无法再使用。因此，在使用 Stream 时，需要根据需要重新创建新的 Stream 对象。
<a name="yI5Gd"></a>
### 常用的 Stream 操作方法

1. 过滤（Filter）：`filter()` 方法接受一个 Predicate 函数作为参数，用于过滤 Stream 中的元素。只有满足 Predicate 条件的元素会被保留下来。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> filteredStream = stream.filter(n -> n % 2 == 0); // 过滤出偶数
```

2. 映射（Map）：`map()` 方法接受一个 Function 函数作为参数，用于对 Stream 中的元素进行映射转换。对每个元素应用函数后的结果会构成一个新的 Stream。例如：
```java
Stream<String> stream = Stream.of("apple", "banana", "cherry");
Stream<Integer> mappedStream = stream.map(s -> s.length()); // 映射为单词长度
```

3. 扁平映射（FlatMap）：`flatMap()` 方法类似于 `map()` 方法，不同之处在于它可以将每个元素映射为一个流，并将所有流连接成一个流。这主要用于解决嵌套集合的情况。例如：
```java
List<List<Integer>> nestedList = Arrays.asList(
    Arrays.asList(1, 2),
    Arrays.asList(3, 4),
    Arrays.asList(5, 6)
);
Stream<Integer> flattenedStream = nestedList.stream().flatMap(List::stream); // 扁平化为一个流
```

4. 截断（Limit）：`limit()` 方法可以限制 Stream 的大小，只保留前 n 个元素。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> limitedStream = stream.limit(3); // 只保留前 3 个元素
```

5. 跳过（Skip）：`skip()` 方法可以跳过 Stream 中的前 n 个元素，返回剩下的元素组成的新 Stream。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> skippedStream = stream.skip(2); // 跳过前 2 个元素
```

6. 排序（Sorted）：`sorted()` 方法用于对 Stream 中的元素进行排序，默认是自然顺序排序。还可以提供自定义的 Comparator 参数来指定排序规则。例如：
```java
Stream<Integer> stream = Stream.of(5, 2, 4, 1, 3);
Stream<Integer> sortedStream = stream.sorted(); // 自然顺序排序
```

7. 去重（Distinct）：`distinct()` 方法用于去除 Stream 中的重复元素，根据元素的 `equals()` 和 `hashCode()` 方法来判断是否重复。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 2, 3, 3, 3);
Stream<Integer> distinctStream = stream.distinct(); // 去重
```

8. 汇总（Collect）：`collect()` 方法用于将 Stream 中的元素收集到结果容器中，如 List、Set、Map 等。可以使用预定义的 Collectors 类提供的工厂方法来创建收集器，也可以自定义收集器。例如：
```java
Stream<String> stream = Stream.of("apple", "banana", "cherry");
List<String> collectedList = stream.collect(Collectors.toList()); // 收集为 List
```

9. 归约（Reduce）：`reduce()` 方法用于将 Stream 中的元素依次进行二元操作，得到一个最终的结果。它接受一个初始值和一个 BinaryOperator 函数作为参数。例如：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Optional<Integer> sum = stream.reduce((a, b) -> a + b); // 对所有元素求和
```

10. 统计（Summary Statistics）：`summaryStatistics()` 方法可以从 Stream 中获取一些常用的统计信息，如元素个数、最小值、最大值、总和和平均值。例如：
```java
IntStream stream = IntStream.of(1, 2, 3, 4, 5);
IntSummaryStatistics stats = stream.summaryStatistics();
System.out.println("Count: " + stats.getCount());
System.out.println("Min: " + stats.getMin());
System.out.println("Max: " + stats.getMax());
System.out.println("Sum: " + stats.getSum());
System.out.println("Average: " + stats.getAverage());
```
以上只是 Stream API 提供的一部分常用操作方法，还有许多其他操作方法，如匹配（Match）、查找（Find）、遍历（ForEach）等
<a name="YPTBo"></a>
## Stream 的中间操作
<a name="GJXvq"></a>
### 过滤操作（filter）
过滤操作（filter）是 Stream API 中的一种常用操作方法，它接受一个 Predicate 函数作为参数，用于过滤 Stream 中的元素。只有满足 Predicate 条件的元素会被保留下来，而不满足条件的元素将被过滤掉。<br />过滤操作的语法如下：
```java
Stream<T> filter(Predicate<? super T> predicate)
```
其中，`T` 表示 Stream 元素的类型，`predicate` 是一个函数式接口 `Predicate` 的实例，它的泛型参数和 Stream 元素类型一致。<br />使用过滤操作可以根据自定义的条件来筛选出符合要求的元素，从而对 Stream 进行精确的数据过滤。<br />下面是一个示例，演示如何使用过滤操作筛选出一个整数流中的偶数：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> filteredStream = stream.filter(n -> n % 2 == 0);
filteredStream.forEach(System.out::println); // 输出结果: 2 4
```
在这个示例中，首先创建了一个包含整数的 Stream，并调用 `filter()` 方法传入一个 Lambda 表达式 `n -> n % 2 == 0`，表示要筛选出偶数。然后通过 `forEach()` 方法遍历输出结果。<br />需要注意的是，过滤操作返回的是一个新的 Stream 实例，原始的 Stream 不会受到改变。这也是 Stream 操作方法的一个重要特点，它们通常返回一个新的 Stream 实例，以便进行链式调用和组合多个操作步骤。<br />在实际应用中，过滤操作可以与其他操作方法结合使用，如映射（map）、排序（sorted）、归约（reduce）等，以实现更复杂的数据处理和转换。而过滤操作本身的优点在于，可以高效地对大型数据流进行筛选，从而提高程序的性能和效率。
<a name="aPTSe"></a>
### 映射操作（map）
映射操作（map）是 Stream API 中的一种常用操作方法，它接受一个 Function 函数作为参数，用于对 Stream 中的每个元素进行映射转换，生成一个新的 Stream。<br />映射操作的语法如下：
```java
<R> Stream<R> map(Function<? super T, ? extends R> mapper)
```
其中，`T` 表示原始 Stream 的元素类型，`R` 表示映射后的 Stream 的元素类型，mapper 是一个函数式接口 Function 的实例，它的泛型参数分别是原始 Stream 元素的类型和映射后的元素类型。<br />使用映射操作可以对 Stream 中的元素逐个进行处理或转换，从而获得一个新的 Stream。这个过程通常涉及对每个元素应用传入的函数，根据函数的返回值来构建新的元素。<br />下面是一个示例，演示如何使用映射操作将一个字符串流中的每个字符串转换为其长度：
```java
Stream<String> stream = Stream.of("apple", "banana", "cherry");
Stream<Integer> mappedStream = stream.map(s -> s.length());
mappedStream.forEach(System.out::println); // 输出结果: 5 6 6
```
在这个示例中，首先创建了一个包含字符串的 Stream，并调用 `map()` 方法传入一个 Lambda 表达式 `s -> s.length()`，表示要将每个字符串转换为其长度。然后通过 `forEach()` 方法遍历输出结果。<br />需要注意的是，映射操作返回的是一个新的 Stream 实例，原始的 Stream 不会受到改变。这也是 Stream 操作方法的一个重要特点，它们通常返回一个新的 Stream 实例，以便进行链式调用和组合多个操作步骤。<br />在实际应用中，映射操作可以与其他操作方法结合使用，如过滤（filter）、排序（sorted）、归约（reduce）等，以实现更复杂的数据处理和转换。而映射操作本身的优点在于，可以通过简单的函数变换实现对原始数据的转换，减少了繁琐的循环操作，提高了代码的可读性和维护性。<br />需要注意的是，映射操作可能引发空指针异常（NullPointerException），因此在执行映射操作时，应确保原始 Stream 中不包含空值，并根据具体情况进行空值处理。
<a name="X8Qv1"></a>
### <br />排序操作（sorted）
排序操作（sorted）是 Stream API 中的一种常用操作方法，它用于对 Stream 中的元素进行排序。排序操作可以按照自然顺序或者使用自定义的比较器进行排序。<br />排序操作的语法如下：
```java
Stream<T> sorted()
Stream<T> sorted(Comparator<? super T> comparator)
```
第一种语法形式中，`sorted()` 方法会根据元素的自然顺序进行排序。如果元素实现了 `Comparable` 接口并且具备自然顺序，那么可以直接调用该方法进行排序。<br />第二种语法形式中，`sorted(Comparator<? super T> comparator)` 方法接受一个比较器（Comparator）作为参数，用于指定元素的排序规则。通过自定义比较器，可以对非 `Comparable` 类型的对象进行排序。<br />下面是一个示例，演示如何使用排序操作对一个字符串流进行排序：
```java
Stream<String> stream = Stream.of("banana", "apple", "cherry");
Stream<String> sortedStream = stream.sorted();
sortedStream.forEach(System.out::println); // 输出结果: apple banana cherry
```
在这个示例中，首先创建了一个包含字符串的 Stream，并直接调用 `sorted()` 方法进行排序。然后通过 `forEach()` 方法遍历输出结果。<br />需要注意的是，排序操作返回的是一个新的 Stream 实例，原始的 Stream 不会受到改变。这也是 Stream 操作方法的一个重要特点，它们通常返回一个新的 Stream 实例，以便进行链式调用和组合多个操作步骤。<br />在实际应用中，排序操作可以与其他操作方法结合使用，如过滤（filter）、映射（map）、归约（reduce）等，以实现更复杂的数据处理和转换。排序操作本身的优点在于，可以将数据按照特定的顺序排列，便于查找、比较和分析。<br />需要注意的是，排序操作可能会影响程序的性能，特别是对于大型数据流或者复杂的排序规则。因此，在实际应用中，需要根据具体情况进行权衡和优化，选择合适的算法和数据结构来提高排序的效率。
<a name="C547Y"></a>
### 截断操作（limit 和 skip）
截断操作（limit和skip）是 Stream API 中常用的操作方法，用于在处理流的过程中对元素进行截断。

1. `limit(n)`：保留流中的前n个元素，返回一个包含最多n个元素的新流。如果流中元素少于n个，则返回原始流。
2. `skip(n)`：跳过流中的前n个元素，返回一个包含剩余元素的新流。如果流中元素少于n个，则返回一个空流。

下面分别详细介绍这两个方法的使用。<br />`limit(n)` 方法示例：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> limitedStream = stream.limit(3);
limitedStream.forEach(System.out::println); // 输出结果: 1 2 3
```
在这个示例中，创建了一个包含整数的 Stream，并调用 `limit(3)` 方法来保留前三个元素。然后使用 `forEach()` 方法遍历输出结果。<br />skip(n) 方法示例：
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
Stream<Integer> skippedStream = stream.skip(2);
skippedStream.forEach(System.out::println); // 输出结果: 3 4 5
```
在这个示例中，创建了一个包含整数的 Stream，并调用 `skip(2)` 方法来跳过前两个元素。然后使用 `forEach()` 方法遍历输出结果。<br />需要注意的是，截断操作返回的是一个新的 Stream 实例，原始的 Stream 不会受到改变。这也是 Stream 操作方法的一个重要特点，它们通常返回一个新的 Stream 实例，以便进行链式调用和组合多个操作步骤。<br />截断操作在处理大型数据流或需要对数据进行切分和分页显示的场景中非常有用。通过限制或跳过指定数量的元素，可以控制数据的大小和范围，提高程序的性能并减少不必要的计算。<br />需要注意的是，在使用截断操作时需要注意流的有界性。如果流是无界的（例如 `Stream.generate()`），那么使用 `limit()` 方法可能导致程序陷入无限循环，而使用 `skip()` 方法则没有意义。
<a name="gQnEl"></a>
## Stream 的终端操作
<a name="pSQvk"></a>
### forEach 和 peek
forEach和peek都是Stream API中用于遍历流中元素的操作方法，它们在处理流的过程中提供了不同的功能和使用场景。

1. forEach： forEach是一个终端操作方法，它接受一个Consumer函数作为参数，对流中的每个元素执行该函数。它没有返回值，因此无法将操作结果传递给后续操作。forEach会遍历整个流，对每个元素执行相同的操作。

示例代码：
```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");
names.stream()
     .forEach(System.out::println);
```
这个示例中，创建了一个包含字符串的List，并通过stream()方法将其转换为流。然后使用forEach方法遍历输出每个元素的值。

2. peek： peek是一个中间操作方法，它接受一个Consumer函数作为参数，对流中的每个元素执行该函数。与forEach不同的是，peek方法会返回一个新的流，该流中的元素和原始流中的元素相同。

示例代码：
```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");
List<String> upperCaseNames = names.stream()
                                   .map(String::toUpperCase)
                                   .peek(System.out::println)
                                   .collect(Collectors.toList());
```
在这个示例中，首先将List转换为流，并通过map方法将每个元素转换为大写字母。然后使用peek方法在转换之前输出每个元素的值。最后通过collect方法将元素收集到一个新的List中。<br />需要注意的是，无论是forEach还是peek，它们都是用于在流的处理过程中执行操作。区别在于forEach是终端操作，不返回任何结果，而peek是中间操作，可以和其他操作方法进行组合和链式调用。<br />根据使用场景和需求，选择使用forEach或peek来遍历流中的元素。如果只是需要遍历输出元素，不需要操作结果，则使用forEach。如果需要在遍历过程中执行一些其他操作，并将元素传递给后续操作，则使用peek。
<a name="iOHkd"></a>
### 聚合操作（reduce 和 collect）
reduce和collect都是Stream API中用于聚合操作的方法，它们可以将流中的元素进行汇总、计算和收集。

1. reduce： reduce是一个终端操作方法，它接受一个`BinaryOperator`函数作为参数，对流中的元素逐个进行合并操作，最终得到一个结果。该方法会将流中的第一个元素作为初始值，然后将初始值与下一个元素传递给BinaryOperator函数进行计算，得到的结果再与下一个元素进行计算，以此类推，直到遍历完所有元素。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Optional<Integer> sum = numbers.stream()
                               .reduce((a, b) -> a + b);
sum.ifPresent(System.out::println); // 输出结果: 15
```
在这个示例中，创建了一个包含整数的List，并通过`stream()`方法将其转换为流。然后使用`reduce`方法对流中的元素进行求和操作，将每个元素依次相加，得到结果15。

2. collect： collect是一个终端操作方法，它接受一个Collector接口的实现作为参数，对流中的元素进行收集和汇总的操作。Collector接口定义了一系列用于聚合操作的方法，例如收集元素到List、Set、Map等容器中，或进行字符串连接、分组、计数等操作。

示例代码：
```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");
String joinedNames = names.stream()
                          .collect(Collectors.joining(", "));
System.out.println(joinedNames); // 输出结果: Alice, Bob, Charlie
```
在这个示例中，创建了一个包含字符串的List，并通过stream()方法将其转换为流。然后使用collect方法将流中的元素连接成一个字符串，每个元素之间使用逗号和空格分隔。<br />需要注意的是，reduce和collect都是终端操作，它们都会触发流的遍历和处理。不同的是，reduce方法用于对流中的元素进行累积计算，得到一个最终结果；而collect方法用于对流中的元素进行收集和汇总，得到一个容器或其他自定义的结果。<br />在选择使用reduce还是collect时，可以根据具体需求和操作类型来决定。如果需要对流中的元素进行某种计算和合并操作，得到一个结果，则使用reduce。如果需要将流中的元素收集到一个容器中，进行汇总、分组、计数等操作，则使用collect。
<a name="lHWhQ"></a>
### 匹配操作（allMatch、anyMatch 和 noneMatch）
在 Stream API 中，allMatch、anyMatch 和 noneMatch 是用于进行匹配操作的方法，它们可以用来检查流中的元素是否满足特定的条件。

1. allMatch： allMatch 方法用于判断流中的所有元素是否都满足给定的条件。当流中的所有元素都满足条件时，返回 true；如果存在一个元素不满足条件，则返回 false。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
boolean allEven = numbers.stream()
                         .allMatch(n -> n % 2 == 0);
System.out.println(allEven); // 输出结果: false
```
在这个示例中，创建了一个包含整数的 List，并通过 stream() 方法将其转换为流。然后使用 allMatch 方法判断流中的元素是否都是偶数。由于列表中存在奇数，所以返回 false。

2. anyMatch： anyMatch 方法用于判断流中是否存在至少一个元素满足给定的条件。当流中至少有一个元素满足条件时，返回 true；如果没有元素满足条件，则返回 false。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
boolean hasEven = numbers.stream()
                         .anyMatch(n -> n % 2 == 0);
System.out.println(hasEven); // 输出结果: true
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 anyMatch 方法判断流中是否存在偶数。由于列表中存在偶数，所以返回 true。

3. noneMatch： noneMatch 方法用于判断流中的所有元素是否都不满足给定的条件。当流中没有元素满足条件时，返回 true；如果存在一个元素满足条件，则返回 false。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
boolean noneNegative = numbers.stream()
                             .noneMatch(n -> n < 0);
System.out.println(noneNegative); // 输出结果: true
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 noneMatch 方法判断流中的元素是否都是非负数。由于列表中的元素都是非负数，所以返回 true。<br />需要注意的是，allMatch、anyMatch 和 noneMatch 都是终端操作，它们会遍历流中的元素直到满足条件或处理完所有元素。在性能上，allMatch 和 noneMatch 在第一个不匹配的元素处可以立即返回结果，而 anyMatch 在找到第一个匹配的元素时就可以返回结果。
<a name="hdTKr"></a>
### 查找操作（findFirst 和 findAny）
在 Stream API 中，findFirst 和 findAny 是用于查找操作的方法，它们可以用来从流中获取满足特定条件的元素。

1. findFirst： findFirst 方法用于返回流中的第一个元素。它返回一个 Optional 对象，如果流为空，则返回一个空的 Optional；如果流非空，则返回流中的第一个元素的 Optional。

示例代码：
```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");
Optional<String> first = names.stream()
                              .findFirst();
first.ifPresent(System.out::println); // 输出结果: Alice
```
在这个示例中，创建了一个包含字符串的 List，并通过 `stream()` 方法将其转换为流。然后使用 findFirst 方法获取流中的第一个元素，并使用 ifPresent 方法判断 Optional 是否包含值，并进行相应的处理。

2. findAny： findAny 方法用于返回流中的任意一个元素。它返回一个 Optional 对象，如果流为空，则返回一个空的 Optional；如果流非空，则返回流中的任意一个元素的 Optional。在顺序流中，通常会返回第一个元素；而在并行流中，由于多线程的处理，可能返回不同的元素。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Optional<Integer> any = numbers.stream()
                               .filter(n -> n % 2 == 0)
                               .findAny();
any.ifPresent(System.out::println); // 输出结果: 2 或 4（取决于并行处理的结果）
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 filter 方法筛选出偶数，再使用 findAny 方法获取任意一个偶数，最后使用 ifPresent 方法判断 Optional 是否包含值，并进行相应的处理。<br />需要注意的是，findAny 在并行流中会更有优势，因为在多线程处理时，可以返回最先找到的元素，提高效率。而在顺序流中，findAny 的性能与 findFirst 相当。
<a name="SSTMT"></a>
### 统计操作（count、max 和 min）
在 Stream API 中，count、max 和 min 是用于统计操作的方法，它们可以用来获取流中元素的数量、最大值和最小值。

1. count： count 方法用于返回流中元素的数量。它返回一个 long 类型的值，表示流中的元素个数。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
long count = numbers.stream()
                    .count();
System.out.println(count); // 输出结果: 5
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 count 方法获取流中元素的数量，并将结果输出。

2. max： max 方法用于返回流中的最大值。它返回一个 Optional 对象，如果流为空，则返回一个空的 Optional；如果流非空，则返回流中的最大值的 Optional。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Optional<Integer> max = numbers.stream()
                               .max(Integer::compareTo);
max.ifPresent(System.out::println); // 输出结果: 5
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 max 方法获取流中的最大值，并使用 ifPresent 方法判断 Optional 是否包含值，并进行相应的处理。

3. min： min 方法用于返回流中的最小值。它返回一个 Optional 对象，如果流为空，则返回一个空的 Optional；如果流非空，则返回流中的最小值的 Optional。

示例代码：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Optional<Integer> min = numbers.stream()
                               .min(Integer::compareTo);
min.ifPresent(System.out::println); // 输出结果: 1
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。然后使用 min 方法获取流中的最小值，并使用 ifPresent 方法判断 Optional 是否包含值，并进行相应的处理。<br />这些统计操作方法提供了一种便捷的方式来对流中的元素进行数量、最大值和最小值的计算。通过返回 Optional 对象，可以避免空指针异常。
<a name="EShhA"></a>
## 并行流
<a name="txg9b"></a>
### 什么是并行流
并行流是 Java 8 Stream API 中的一个特性。它可以将一个流的操作在多个线程上并行执行，以提高处理大量数据时的性能。<br />在传统的顺序流中，所有的操作都是在单个线程上按照顺序执行的。而并行流则会将流的元素分成多个小块，并在多个线程上并行处理这些小块，最后将结果合并起来。这样可以充分利用多核处理器的优势，加快数据处理的速度。<br />要将一个顺序流转换为并行流，只需调用流的 parallel() 方法即可。示例代码如下所示：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
numbers.stream()
       .parallel()
       .forEach(System.out::println);
```
在这个示例中，创建了一个包含整数的 List，并通过 `stream()` 方法将其转换为流。接着调用 parallel() 方法将流转换为并行流，然后使用 forEach 方法遍历流中的元素并输出。<br />需要注意的是，并行流的使用并不总是适合所有情况。并行流的优势主要体现在数据量较大、处理时间较长的场景下。对于小规模数据和简单的操作，顺序流可能更加高效。在选择使用并行流时，需要根据具体情况进行评估和测试，以确保获得最佳的性能。<br />此外，还需要注意并行流在某些情况下可能引入线程安全的问题。如果多个线程同时访问共享的可变状态，可能会导致数据竞争和不确定的结果。因此，在处理并行流时，应当避免共享可变状态，或采用适当的同步措施来确保线程安全。
<a name="SEZLG"></a>
### 如何使用并行流提高性能
使用并行流可以通过利用多线程并行处理数据，从而提高程序的执行性能。下面是一些使用并行流提高性能的常见方法：

1. 创建并行流：要创建一个并行流，只需在普通流上调用 `parallel()` 方法。
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
Stream<Integer> parallelStream = numbers.parallelStream();
```

2. 利用任务并行性：并行流会将数据分成多个小块，并在多个线程上并行处理这些小块。这样可以充分利用多核处理器的优势。
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
numbers.parallelStream()
       .map(n -> compute(n)) // 在多个线程上并行处理计算
       .forEach(System.out::println);
```
在这个示例中，使用 map 方法对流中的每个元素进行计算。由于并行流的特性，计算操作会在多个线程上并行执行，提高了计算的效率。

1. 避免共享可变状态：在并行流中，多个线程会同时操作数据。如果共享可变状态（如全局变量）可能导致数据竞争和不确定的结果。因此，避免在并行流中使用共享可变状态，或者采取适当的同步措施来确保线程安全。
2. 使用合适的操作：一些操作在并行流中的性能表现更好，而另一些操作则可能导致性能下降。一般来说，在并行流中使用基于聚合的操作（如 reduce、collect）和无状态转换操作（如 map、filter）的性能较好，而有状态转换操作（如 sorted）可能会导致性能下降。
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);

// good performance
int sum = numbers.parallelStream()
                 .reduce(0, Integer::sum);

// good performance
List<Integer> evenNumbers = numbers.parallelStream()
                                   .filter(n -> n % 2 == 0)
                                   .collect(Collectors.toList());

// potential performance degradation
List<Integer> sortedNumbers = numbers.parallelStream()
                                     .sorted()
                                     .collect(Collectors.toList());
```
在这个示例中，reduce 和 filter 的操作在并行流中具有良好的性能，而 sorted 操作可能导致性能下降。<br />除了上述方法，还应根据具体情况进行评估和测试，并行流是否能够提高性能。有时候，并行流的开销（如线程的创建和销毁、数据切割和合并等）可能超过了其带来的性能提升。因此，在选择使用并行流时，应该根据数据量和操作复杂度等因素进行综合考虑，以确保获得最佳的性能提升。
<a name="GpP5e"></a>
### 并行流的适用场景和注意事项

1. 大规模数据集：当需要处理大规模数据集时，使用并行流可以充分利用多核处理器的优势，提高程序的执行效率。并行流将数据切分成多个小块，并在多个线程上并行处理这些小块，从而缩短了处理时间。
2. 复杂的计算操作：对于复杂的计算操作，使用并行流可以加速计算过程。由于并行流能够将计算操作分配到多个线程上并行执行，因此可以有效地利用多核处理器的计算能力，提高计算的速度。
3. 无状态转换操作：并行流在执行无状态转换操作（如 map、filter）时表现较好。这类操作不依赖于其他元素的状态，每个元素的处理是相互独立的，可以很容易地进行并行处理。

并行流的注意事项包括：

1. 线程安全问题：并行流的操作是在多个线程上并行执行的，因此需要注意线程安全问题。如果多个线程同时访问共享的可变状态，可能会导致数据竞争和不确定的结果。在处理并行流时，应避免共享可变状态，或者采用适当的同步措施来确保线程安全。
2. 性能评估和测试：并行流的性能提升并不总是明显的。在选择使用并行流时，应根据具体情况进行评估和测试，以确保获得最佳的性能提升。有时，并行流的开销（如线程的创建和销毁、数据切割和合并等）可能超过了其带来的性能提升。
3. 并发操作限制：某些操作在并行流中的性能表现可能较差，或者可能导致结果出现错误。例如，在并行流中使用有状态转换操作（如 sorted）可能导致性能下降或结果出现错误。在使用并行流时，应注意避免这类操作，或者在需要时采取适当的处理措施。
4. 内存消耗：并行流需要将数据分成多个小块进行并行处理，这可能导致额外的内存消耗。在处理大规模数据集时，应确保系统有足够的内存来支持并行流的执行，以避免内存溢出等问题。
<a name="t8iUo"></a>
## 实践应用示例
<a name="vZbSp"></a>
### 使用 Stream 处理集合数据

1. 筛选出长度大于等于5的字符串，并打印输出：
```java
List<String> list = Arrays.asList("apple", "banana", "orange", "grapefruit", "kiwi");
list.stream()
    .filter(s -> s.length() >= 5)
    .forEach(System.out::println);
```
输出结果：
```
banana
orange
grapefruit
```

2. 将集合中的每个字符串转换为大写，并收集到新的列表中：
```java
List<String> list = Arrays.asList("apple", "banana", "orange", "grapefruit", "kiwi");
List<String> resultList = list.stream()
                              .map(String::toUpperCase)
                              .collect(Collectors.toList());
System.out.println(resultList);
```
输出结果：
```
[APPLE, BANANA, ORANGE, GRAPEFRUIT, KIWI]
```

3. 统计集合中以字母"a"开头的字符串的数量：
```java
List<String> list = Arrays.asList("apple", "banana", "orange", "grapefruit", "kiwi");
long count = list.stream()
                 .filter(s -> s.startsWith("a"))
                 .count();
System.out.println(count);
```
输出结果：
```
1
```

4. 使用并行流来提高处理速度，筛选出长度小于等于5的字符串，并打印输出：
```java
List<String> list = Arrays.asList("apple", "banana", "orange", "grapefruit", "kiwi");
list.parallelStream()
    .filter(s -> s.length() <= 5)
    .forEach(System.out::println);
```
输出结果：
```
apple
kiwi
```

5. 使用 Stream 对集合中的整数求和：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
int sum = numbers.stream()
                 .mapToInt(Integer::intValue)
                 .sum();
System.out.println(sum);
```
输出结果：
```
15
```
以上示例展示了如何使用 Stream 对集合数据进行筛选、转换、统计等操作。通过链式调用 Stream 的中间操作和终端操作。
<a name="zP0u0"></a>
### 使用 Stream 进行文件操作
```java
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class FileStreamExample {
    public static void main(String[] args) {
        String fileName = "file.txt";

        // 读取文件内容并创建 Stream
        try (Stream<String> stream = Files.lines(Paths.get(fileName))) {
            // 打印文件的每一行内容
            stream.forEach(System.out::println);

            // 统计文件的行数
            long count = stream.count();
            System.out.println("总行数：" + count);

            // 筛选包含关键词的行并打印输出
            stream.filter(line -> line.contains("keyword"))
            .forEach(System.out::println);

            // 将文件内容转换为大写并打印输出
            stream.map(String::toUpperCase)
            .forEach(System.out::println);

            // 将文件内容收集到 List 中
            List<String> lines = stream.collect(Collectors.toList());
            System.out.println(lines);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
在上面的代码中，首先指定了要读取的文件名 file.txt。然后使用 `Files.lines()` 方法读取文件的每一行内容，并创建一个 Stream 对象。接下来，对 Stream 进行一些操作：

- 使用 `forEach()` 方法打印文件的每一行内容。
- 使用 `count()` 方法统计文件的行数。
- 使用 `filter()` 方法筛选出包含关键词的行，并打印输出。
- 使用 `map()` 方法将文件内容转换为大写，并打印输出。
- 使用 `collect()` 方法将文件内容收集到 List 中。

请根据实际需求修改代码中的文件名、操作内容和结果处理方式。需要注意的是，在使用完 Stream 后，应及时关闭文件资源，可以使用 try-with-resources 语句块来自动关闭文件。另外，请处理可能出现的 IOException 异常。
<a name="Zd1EL"></a>
### 使用 Stream 实现数据转换和筛选
```java
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class StreamExample {
    public static void main(String[] args) {
        List<String> names = Arrays.asList("Amy", "Bob", "Charlie", "David", "Eva");

        // 转换为大写并筛选出长度大于3的名称
        List<String> result = names.stream()
                                   .map(String::toUpperCase)
                                   .filter(name -> name.length() > 3)
                                   .collect(Collectors.toList());
        // 打印结果
        result.forEach(System.out::println);
    }
}
```
在上述代码中，首先创建了一个包含一些名字的列表。然后使用 Stream 对列表进行操作：

- 使用 `stream()` 方法将列表转换为一个 Stream。
- 使用 `map()` 方法将每个名称转换为大写。
- 使用 `filter()` 方法筛选出长度大于3的名称。
- 使用 `collect()` 方法将筛选后的结果收集到一个新的列表中。

最后，使用 `forEach()` 方法打印结果列表中的每个名称。
