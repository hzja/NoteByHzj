Java<br />进度条在日常使用的软件中已经很频繁了，不过除了安装软件一般都是在网页中看到，那如何在标准输出中实现一个进度条呢？<br />光有想法是不行，接下来就开始实践一下，通过 Java 代码在标准输出中实现一个类似的进度条。<br />首先看到一个进度条是从 0 加载到 100 的一个过程，实现进度条需要一个最终的状态，然后逐渐从无到有变化形成，所以要有一个百分比显示，其次要有两种不一样的样式变化。可以通过先展示一种符合为百分百，然后不断通过替换新的符合，产生一种变化的效果，这里采用下面这两种符号来表示
```java
char incomplete = '░'; // U+2591 Unicode Character 表示还没有完成的部分
char complete = '█'; // U+2588 Unicode Character 表示已经完成的部分
```
通过用完成的符号不断地替换未完成的符号，形成一种不断在进行的效果。<br />接下来实现一个静态方法，首先通过`Stream.generate()`，创建未完成的部分，这里用 StringBuilder 来创建字符串对象，通过 append 方式来填充，形式如下 `Stream.generate(() -> incomplete).limit(total).forEach(builder::append);`这里 total 设置成 100，限制一下长度。<br />再通过一个 for 循环来不断的替换未完成中的部分，将字符挨个替换为完成的字符，`builder.replace(i, i + 1, String.valueOf(complete));`。<br />这里有三个需要需要的地方

1. 进度条始终展示在一行，并没有换号，所以要用`System.out.print()` 方法输出，不能用 `System.out.println();` 方法来输出；
2. 如果直接通过`System.out.print();` 输出的话，会发现虽然在同一行出现，但是会越来越长，如下所示，每次循环的字符串会接在后面，这样是不行的。所以这里需要使用到回车符号，大家都知道`\r\n`，用来回车和换行，其实这种两个符号`\r`  是回车，`\n` 是换行。回车是将光标移动到行首，换行则是将光标移动的下一行

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652660038835-029902d3-c92e-434b-9cde-e659e0e2c9da.jpeg#clientId=u97e24f6b-414d-4&from=paste&id=ud7a3b168&originHeight=132&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u59be1c3f-6734-4441-8431-6ff429ebf30&title=)

3. 进度条会根据网络和资源包大小下载的时候慢慢变化的，所以要有一定的速度，不能一下子就完成了，这里可以简单通过 `Thread.sleep()` 来实现；

所以在代码修改成如下所示，这里为了表示越到后面越慢的场景，这边的 sleep 不是一个固定的数值。
```java
for (int i = 0; i < total; i++) {
    builder.replace(i, i + 1, String.valueOf(complete));
    String progressBar = "\r" + builder;
    String percent = " " + (i + 1) + "%";
    System.out.print(progressBar + percent);
    try {
        Thread.sleep(i * 5L);
    } catch (InterruptedException ignored) {

    }
}
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652660038878-5330dc3b-fbb9-487d-bdc8-577f418415b8.jpeg#clientId=u97e24f6b-414d-4&from=paste&id=u120c2215&originHeight=600&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua634307a-afab-41eb-ab75-13e0b18ad64&title=)至此一个基于 Java 标准输出的进度条就实现了，只要思考情况，实现起来还是很简单的，只是其中有几个知识点知道就行了。重点就是要知道如何实现将光标移动的行首，可能符号都认识，只是平时没想到还可以这样使用。
