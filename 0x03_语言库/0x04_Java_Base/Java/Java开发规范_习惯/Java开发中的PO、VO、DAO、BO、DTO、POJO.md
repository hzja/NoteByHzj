Java<br />《阿里巴巴Java开发规范》关于领域模型的部分介绍如下<br />**分层领域模型规约：**

- DO(Data Object)：此对象与数据库表结构一一对应，通过 DAO 层向上传输数据源对象。
- DTO(Data Transfer Object)：数据传输对象，Service 或 Manager 向外传输的对象。
- BO(Business Object)：业务对象，由 Service 层输出的封装业务逻辑的对象。
- AO(ApplicationObject)：应用对象，在Web层与Service层之间抽象的复用对象模型，极为贴近展示层，复用度不高。
- VO(View Object)：显示层对象，通常是 Web 向模板渲染引擎层传输的对象。
- Query：数据查询对象，各层接收上层的查询请求。注意超过 2 个参数的查询封装，禁止使用 Map 类来传输。

**领域模型命名规约：**

- 数据对象：xxxDO，xxx即为数据表名
- 数据传输对象：xxxDTO，xxx为业务领域相关的名称。
- 展示对象：xxxVO，xxx一般为网页名称。
- POJO是DO/DTO/BO/VO的统称，禁止命名成xxxPOJO。
<a name="8Xno0"></a>
### 一、PO(persistant object)—持久对象
可以看成是与数据库中的表相映射的java对象。使用Hibernate来生成PO是不错的选择。
<a name="mURiG"></a>
### 二、VO(value object)—值对象
通常用于业务层之间的数据传递，和PO一样也是仅仅包含数据而已。但应是抽象出的业务对象，可以和表对应，也可以不，这根据业务的需要。<br />PO只能用在数据层，VO用在商业逻辑层和表示层。各层操作属于该层自己的数据对象，这样就可以降低各层之间的耦合，便于以后系统的维护和扩展。
<a name="E7Gw5"></a>
### 三、DAO(Data Access Objects)—数据访问对象接口
DAO是Data Access Object数据访问接口，数据访问：顾名思义就是与数据库打交道。夹在业务逻辑与数据库资源中间。<br />J2EE开发人员使用数据访问对象（DAO）设计模式把底层的数据访问逻辑和高层的商务逻辑分开。实现DAO模式能够更加专注于编写数据访问代码。<br />DAO模式是标准的J2EE设计模式之一。开发人员使用这个模式把底层的数据访问操作和上层的商务逻辑分开。一个典型的DAO实现有下列几个组件：

1. 一个DAO工厂类；
2. 一个DAO接口；
3. 一个实现DAO接口的具体类；
4. 数据传递对象（有些时候叫做值对象）。

具体的DAO类包含了从特定的数据源访问数据的逻辑。
<a name="zdhDE"></a>
### 四、BO(Business Object)—业务对象层
表示应用程序领域内“事物”的所有实体类。这些实体类驻留在服务器上，并利用服务类来协助完成它们的职责。
<a name="hmoMW"></a>
### 五、DTO(Data Transfer Object)—数据传输对象
主要用于远程调用等需要大量传输对象的地方。比如一张表有100个字段，那么对应的PO就有100个属性。但是界面上只要显示10个字段，客户端用WEB service来获取数据，没有必要把整个PO对象传递到客户端，这时就可以用只有这10个属性的DTO来传递结果到客户端，这样也不会暴露服务端表结构。到达客户端以后，如果用这个对象来对应界面显示，那此时它的身份就转为VO。
<a name="c22709c5"></a>
### 六、POJO(Plain Old Java Objects)—简单的Java对象
实际就是普通JavaBeans，使用POJO名称是为了避免和EJB混淆起来，而且简称比较直接。其中有一些属性及其getter、setter方法的类，有时可以作为value object或dto(Data Transform Object)来使用。当然，如果有一个简单的运算属性也是可以的，但不允许有业务方法，也不能携带有connection之类的方法。
