<a name="bG7q4"></a>
## 前言
要在插入数据库时，如果有某某一个主要字段的值重复，则不插入，否则则插入！看了一下Mybatis-Plus是有这个`saveOrUpdate` 方法！<br />原本使用`save`时是没有问题了，改成`saveOrUpdate` 用了一下就报错了。<br />com.baomidou.mybatisplus.core.exceptions.MybatisPlusException: error: can not execute. because can not find column for id from entity!<br />就是这个MybatisPlus不能找到哪个是主键字段，因为这个`saveOrUpdate`默认是根据主键执行操作的！<br />所有需要在原本的实体类的主键头上，打个`@TableId`，如下，后面是对应数据库的字段，已经主键自动递增。
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Subject {

    @TableId(value = "subject_Code", type = IdType.AUTO)
    private long subjectCode;

    private String subjectNameCn;

    private String subjectNameEn;

    private String subjectHref;

    private long subjectParentCode;

    private long levelCode;

    private int isDelete;

    private long operateTimestamp;


}
```
不过还有个问题，就是这个是根据主键做操作的，但是主键本来就是自动递增肯定不会有问题的，接下来就是想个办法，让他根据指定字段做操作，好像是有提供了一个口子。
```java
// 根据updateWrapper尝试更新，否继续执行saveOrUpdate(T)方法
boolean saveOrUpdate(T entity, Wrapper<T> updateWrapper);
```
再去看一下怎么操作的！<br />研究尝试了半天，终于搞出来了。<br />当`saveOrUpdate`不使用条件构造器时，会先做根据主键查询，如果查出来的结果为0，那么就执行插入操作，如果查出来的结果不为0，则执行更新操作。<br />但是一般情况下，主键都不会重复！所以就用条件构造器Wrapper！
```java
UpdateWrapper<Subject> subject_name_cn = new UpdateWrapper<Subject>()
   .eq("subject_Name_Cn", subjectNameCn);
subjectService.saveOrUpdate(subject，subject_name_cn );
```
这样改变后的结果就是会先执行修改，如果执行一条，则执行成功，如果执行结果为0，再执行根据主键查询，然后做插入操作！<br />其实有点多此一举的感觉，因为既然都已经更新不到结果了，那么肯定是没有这个字段！<br />不过转念一想，是指定字段没有，又不是主键没有！<br />但是主键自增那肯定没有！<br />所有又想到一个骚操作，不传`UpdateWrapper`而传`QueryWrapper`会怎么样呢！<br />会不会加在查询条件种呢！丢进去没有报错，不知道结果如何！
```java
QueryWrapper<Subject> subject_name_cn1 = new QueryWrapper<Subject>()
                    .eq("subject_Name_Cn", subjectNameCn);
subjectService.saveOrUpdate(subject,subject_name_cn1);
```
上来全给Update了！丝毫不留情面！把数据删了再试试！<br />幻想破灭！跟传`UpdateWrapper`没有区别！<br />看了一下源码，默认参数是`Wrapper`类型，然后根据条件构造器更新，成功则返回，不成功则走无条件构造器的方法。
```java
default boolean saveOrUpdate(T entity, Wrapper<T> updateWrapper) {
    return this.update(entity, updateWrapper) || this.saveOrUpdate(entity);
}
```
感觉应该加个类型判断！
```java
if(updateWrapper instanceof QueryWrapper){
    //去拼接查询语句！
}
if(updateWrapper instanceof UpdateWrapper){
    //去拼接更新语句！
}
```
这样就不会只根据ID来死查了！
<a name="MWN3d"></a>
### 为什么要用`updateWrapper`？
它与`queryWrapper`的区别就是：`updateWrapper`用set来设置修改的数据；`queryWrapper`应用select来设置要查出来的数据。
<a name="HeaRI"></a>
### `saveOrUpdate` 是否有映射id
Mybatis在插入时，会映射id，但是如果是`saveOrUpdate`会怎么样呢？<br />比如`saveOrUpdate()`后，需要用他的id，但是传进去的对象是没有id的。
```java
@Test
void saveOrUpdate(){
    UserText userText = new UserText();
    userText.setUserSex(Sex.MAN);
    boolean b = userTextService.saveOrUpdate(userText);
    System.out.println(userText.getUserId());
}
```
可以看到他先通过id查了没有再进行插入，然后返回新的id。
```
==>  Preparing: SELECT user_id,user_name,user_sex,start_time FROM user_text WHERE user_id=?
==> Parameters: 0(Long)
<==      Total: 0
Releasing transactional SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@6d0fe80c]
Fetched SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@6d0fe80c] from current transaction
==>  Preparing: INSERT INTO user_text ( user_sex ) VALUES ( ? )
==> Parameters: 1(Integer)
<==    Updates: 1
```
不过这个update，不用试都感觉难搞，因为如果没有id，那么传入这个对象的值，可能查出多个对象，那么他要把哪个id映射回来，是吧！
```java
@Test
void saveOrUpdate(){
    UserText userText = new UserText();
    userText.setUserSex(Sex.MAN);
    UpdateWrapper<UserText> objectUpdateWrapper = new UpdateWrapper<UserText>()
    .eq("user_sex",Sex.MAN);
    boolean b = userTextService.saveOrUpdate(userText,objectUpdateWrapper);
    System.out.println(userText.getUserId());
}
```
但还是试一下，当加了一个`UpdateWrapper`后，有执行成功，执行了3条，返回了id为0。<br />但是这次加了wrapper，再试试如果只插入一条，会怎么样。<br />去看看源码先，看能不能看出什么门道。<br />之前好像也有看了点源码。两种不同构造的方法，执行的逻辑也不一样。
```java
boolean saveOrUpdate(T entity); 

default boolean saveOrUpdate(T entity, Wrapper<T> updateWrapper) {
    return this.update(entity, updateWrapper) || this.saveOrUpdate(entity);
}
```
区别不大，就是会多执行一步更新，如果执行成功就直接走，执行不成功再根据这个对象做`saveOrUpdate`。<br />进去翻了翻就是，如果通过id查到值，就根据id更新，不然就做新增。<br />所以也就不用试了，还是自己手写一个吧，如果需要返回id的话。
<a name="gyBSC"></a>
## 慎用！
细思极恐，当是主键自动生成的数据，一定要写`UpdateWrapper`，不然必然是一直插入！完全不会更新，因为默认是用id查询的。<br />而主键生成的数据，一般都不会去写一个id！
<a name="NSWRY"></a>
## UpdateWrapper 小贴士
上面虽然写了`updateWrapper`可以写一个set属性，有两种情况。<br />首先，一个对象，有5条属性，只有4条有值，1条没有值。<br />Mybatis-plus在执行时，会先去看看你的对象哪条属性有值，哪条没有。<br />只会更新有值的属性，所以只会更新4个属性，另外一个属性并不会把他置空。
<a name="em6zm"></a>
## 后续
发现一个很垃圾的，前面吹的那个updateWrapper的set多牛逼，其实是想的太美了，他只是在原本的基础上再加一个字段！
```java
UpdateWrapper<GameScorePo> updateWrapper = new UpdateWrapper<GameScorePo>()
       .eq("game_id",gameScorePo.getGameId())
       .eq("team_id",gameScorePo.getTeamId())
       .eq("quarter",gameScorePo.getQuarter())
       .set("score",gameScorePo.getScore());

gameScoreService.saveOrUpdate(gameScorePo,updateWrapper);
```
这样的执行结果是这样的！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699703441609-c016c8ea-eec1-4352-be11-012c152e8cdd.png#averageHue=%23363636&clientId=udc87bbcc-5481-4&from=paste&id=u231390bf&originHeight=51&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u83709072-b64a-43f2-bbeb-07af47791ce&title=)<br />两个score！<br />难道是打开的姿势不对？<br />查了一下知道这个set怎么样了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699703441606-b0c44954-7433-4c75-8a1e-5e637b04dc2e.png#averageHue=%23fbfbfa&clientId=udc87bbcc-5481-4&from=paste&id=u48ffd554&originHeight=171&originWidth=868&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20504fd7-0c82-4757-9091-f5c15381a82&title=)<br />就是不要丢对象，丢一个空的对象，这样就能set了！<br />单独的set好用，但是用在`saveOrUpdate`就不好用了！看自己的需求走吧！
