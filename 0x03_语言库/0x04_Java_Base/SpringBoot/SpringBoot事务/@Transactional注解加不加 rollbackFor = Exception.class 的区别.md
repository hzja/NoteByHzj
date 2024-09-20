Java
<a name="ml2Ca"></a>
## 1、首先在MySQL中准备了一条数据
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660092807400-87a4a047-8e99-4911-894d-edb3bebcb44b.png#clientId=u32ee2d8d-2de5-4&from=paste&id=u2a83226c&originHeight=152&originWidth=631&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4867f590-2277-4251-866a-ea815eab199&title=)
<a name="bMlbF"></a>
## 2、简单粗暴的开始测试了
1、目的是需要把delflag修改为0 简单的准备一下sql
```xml
<update id="test">
	UPDATE tbl_users set delflag='0' where account='admin'
 </update>
```
2、先来测试一下`@Transactional`，代码如下，都知道2/0必会抛出异常
```java
@Override
@Transactional
public Ret test(){
	int i = articleMapper.test();
	int a = 2/0;
	if(i > 0){
		ResultUtil.success();
	}
	return ResultUtil.error();
}
```
3、执行测试 i=1说明更新成功，继续断点往下面走<br />4、果然不出所料，执行到第54行的时候报错了，出现了java.lang.ArithmeticException: /by zero<br />5、细心的同学会发现`ArithmeticException`这个异常类是继承了`RuntimeException`的<br />而`@Transactional`默认回滚的的异常就是`RuntimeException`<br />6、在点进去`RuntimeException`这个类里面一探究竟，可以发现`RuntimeException`又是继承`Exception`的<br />而所有的异常类基本都是继承`RuntimeException`包括刚才上面的java.lang.ArithmeticException异常<br />所以只要是`RuntimeException`和`RuntimeException`下面的子类抛出的异常 `@Transactional`都可以回滚的<br />7、这个时候去看一下数据库的值到底有没有修改成功，很显然数据是被回滚了，并没有修改成0<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660092807806-a7758a0b-0241-4c27-94b7-594490dfc0e1.png#clientId=u32ee2d8d-2de5-4&from=paste&id=ub76f39a0&originHeight=143&originWidth=658&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8364b85-39cd-4b7d-8e21-2325aa553be&title=)<br />1、下面在试试`@Transactional`不能过滚的异常，代码如下<br />直接先用try catch来捕获异常，然后在catch里面自定义抛出Exception异常
```java
@Override
@Transactional
public Ret test() throws Exception {
	int i = articleMapper.test();
	
	try {
		int a = 2 / 0;
	} catch (Exception e) {
		throw new Exception();
	}
	if (i > 0) {
		ResultUtil.success();
	}
	return ResultUtil.error();
}
```
2、ok，直接抛出的异常是指定的java.lang.Exception异常，去看看数据库<br />3、数据库被更新成0了，说明`@Transactional`并不能回滚Exception异常<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660092807888-291efe22-060e-48b7-bc6b-9a8052f1273b.png#clientId=u32ee2d8d-2de5-4&from=paste&id=u58ff7ace&originHeight=143&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8b69975-fa69-4033-989f-8eae70f5176&title=)
<a name="RL7i6"></a>
## 总结一下
`@Transactional`只能回滚`RuntimeException`和`RuntimeException`下面的子类抛出的异常，不能回滚`Exception`异常<br />如果需要支持回滚`Exception`异常请用`@Transactional(rollbackFor = Exception.class)`<br />这里如果是增删改的时候建议大家都使用`@Transactional(rollbackFor = Exception.class)`
