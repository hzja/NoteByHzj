Java
<a name="LHo2W"></a>
### Hash如何存数据
hash表的本质其实就是数组，hash表中通常存放的是键值对Entry。<br />如下图:<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652401272294-de9c1683-a887-4f77-9bd7-4df1282856f2.png#clientId=u679238ef-6592-4&from=paste&id=u869aca02&originHeight=545&originWidth=786&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua0a263ba-dfbe-43f0-b4cc-6c378830697&title=)<br />这里的学号是个key，哈希表就是根据key值来通过哈希函数计算得到一个值，这个值就是下标值，用来确定这个Entry要存放在哈希表中哪个位置。
<a name="Lxle3"></a>
### Hash碰撞
hash碰撞指的是，两个不同的值（比如张三、李四的学号）经过hash计算后，得到的hash值相同，后来的李四要放到原来的张三的位置，但是数组的位置已经被张三占了，导致冲突。
<a name="CBL2x"></a>
### 解决方法
hash碰撞的解决方式是开放寻址法和拉链法。<br />开放寻址法指的是，当前数组位置1被占用了，就放到下一个位置2上去，如果2也被占用了，就继续往下找，直到找到空位置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652401272352-d6051380-92ee-4b90-b0ca-49acfbbbcc48.png#clientId=u679238ef-6592-4&from=paste&id=u989b9aa8&originHeight=515&originWidth=774&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud607b06c-5a9a-4871-a08c-7f22ccbd7e4&title=)<br />拉链法采用的是链表的方式，这个时候位置1就不单单存放的是Entry了，此时的Entry还要额外保存一个next指针，指向数组外的另一个位置，将李四安排在这里，张三那个Entry中的next指针就指向李四的这个位置，也就是保存的这个位置的内存地址。如果还有冲突，就把又冲突的那个Entry放到一个新位置上，然后李四的Entry指向它，这样就形成一个链表。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652401272330-55f1a857-f47b-4e36-a6d8-83f7c8a136d3.png#clientId=u679238ef-6592-4&from=paste&id=udeaa359d&originHeight=524&originWidth=834&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u224cefc4-13fe-4f1d-b5e2-d5452cf72e1&title=)<br />总结起来：**开放寻址法和拉链法都是想办法找到下一个空位置来存发生冲突的值。**
