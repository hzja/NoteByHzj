<a name="v0nCu"></a>
## MVC三层架构
在刚成为程序员的时候，就会被前辈们 “教育” 说系统的设计要遵循 **MVC（Model-View-Controller）**架构。它将整体的系统分成了 Model（模型），View（视图）和 Controller（控制器）三个层次，也就是将用户视图和业务处理隔离开，并且通过控制器连接起来，很好地实现了表现和逻辑的解耦，是一种标准的软件分层架构。<br />![MVC三层架构](https://cdn.nlark.com/yuque/0/2023/png/396745/1680675109965-58f9c217-3ee2-4768-a127-e3f71d3ca8d6.png#averageHue=%23c8c4c4&clientId=u445f4f5b-243c-4&from=paste&id=ue637dcd8&originHeight=672&originWidth=1064&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u31cdb265-2e68-4af6-b8e0-b2639361f62&title=MVC%E4%B8%89%E5%B1%82%E6%9E%B6%E6%9E%84 "MVC三层架构")<br />MVC分层架构是架构上最简单的一种分层方式。为了遵循这种分层架构在构建项目时往往会建立这样三个目录：controller、service 和 dao，它们分别对应了表现层、逻辑层还有数据访问层。<br />![三层架构示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1680675109963-f2ed301b-5a41-4ef3-a6ed-ebb8ff81a6c7.png#averageHue=%23b6afaf&clientId=u445f4f5b-243c-4&from=paste&id=u89319490&originHeight=804&originWidth=526&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u1bd11210-4bf1-43a8-9d56-73be0efd9c5&title=%E4%B8%89%E5%B1%82%E6%9E%B6%E6%9E%84%E7%A4%BA%E6%84%8F%E5%9B%BE "三层架构示意图")<br />每层的作用如下：

- Controller层：主要是对访问控制进行转发，各类基本参数校验，或者不复用的业务简单处理。
- Service层：主要是处理业务逻辑和事务
- Dao层：负责与底层数据库MySQL，Oracle等进行数据交互

可是随着业务逻辑越来复杂，代码写的越来越多，这种简单的三层架构的问题也越来越明显。
<a name="CkGbB"></a>
## MVC架构弊端
传统的MVC分层有以下几个很明显的问题：

1. Service层代码臃肿
2. Service层很容易出现大事务，事务嵌套，导致问题很多，而且极难排查
3. dao层参杂业务逻辑
4. dao层sql语句复杂，关联查询比较多

为了解决这个问题，参考《alibaba java开发手册》，在Service层之下再独立出一个通用业务处理层（Manager层）<br />![Manager层](https://cdn.nlark.com/yuque/0/2023/png/396745/1680675109953-1eb65264-c23c-484f-8df2-ca0f3b6d36b9.png#averageHue=%23c4bbbb&clientId=u445f4f5b-243c-4&from=paste&id=u5dc3c01e&originHeight=912&originWidth=676&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5e9612d5-a3d3-47f5-b4a3-4a8bc699a9a&title=Manager%E5%B1%82 "Manager层")<br />在这个分层架构中主要增加了 Manager 层，它与 Service 层的关系是：Manager 层提供原子的服务接口，Service 层负责依据业务逻辑来编排原子接口。
<a name="bKpFN"></a>
## Manager层的特征
在《alibaba java开发手册》中是这样描述Manager层的：<br />Manager 层：通用业务处理层，它有如下特征：

1. 对第三方平台封装的层，预处理返回结果及转化异常信息，适配上层接口；
2. 对 Service 层通用能力的下沉，如缓存方案、中间件通用处理；
3. 与 DAO 层交互，对多个 DAO 的组合复用。

在实际开发中可以这样使用Manager层

1. 复杂业务，service提供数据给Manager层，负责业务编排，然后把事务下沉到Manager层，Manager层不允许相互调用，不会出现事务嵌套。
2. 专注于不带业务sql语言，也可以在manager层进行通用业务的dao层封装。
3. 避免复杂的join查询，数据库压力比java大很多，所以要严格控制好sql，所以可以在manager层进行拆分，比如复杂查询。

**当然对于简单的业务，可以不使用Manager层。**
<a name="HvNye"></a>
## Manager层使用案例
这里举个例子说明一下Manager层的使用场景：<br />假设有一个用户系统，他有一个获取用户信息的接口，它调用逻辑Service层的 getUser 方法，getUser方法又和 User DB 交互获取数据。如下图左边展示部分。<br />这时，产品提出一个需求，在 APP 中展示用户信息的时候，如果用户不存在，那么要自动给用户创建一个用户。同时，要做一个 HTML5 的页面，HTML5 页面要保留之前的逻辑，也就是不需要创建用户。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680675109947-23aa6bb1-2ce5-4fe1-b1b0-a7a04821b1a0.png#averageHue=%23cbc6c3&clientId=u445f4f5b-243c-4&from=paste&id=u2ffa9aff&originHeight=631&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueafc0700-d7f4-48a5-9de4-95369c34afd&title=)<br />此时按照传统的三层架构，逻辑层的边界就变得不清晰，表现层也承担了一部分的业务逻辑，因为往往会在表现层Controller中增加业务逻辑处理，将获取用户和创建用户接口编排起来。<br />而添加Manager层以后，Manager 层提供创建用户和获取用户信息的接口，而 Service 层负责将这两个接口组装起来。这样就把原先散布在表现层的业务逻辑都统一到了 Service 层，每一层的边界就非常清晰了。<br />接下来看一段实际代码说明一下Service层与Manager层如何进行区分？
```java
@Transactional(rollbackFor = Throwable.class)
public Result<String> upOrDown(Long departmentId, Long swapId) {
    // 验证 1
    DepartmentEntity departmentEntity = departmentDao.selectById(departmentId);
    if (departmentEntity == null) {
        return Result.error("部门xxx不存在");
    }
    // 验证 2
    DepartmentEntity swapEntity = departmentDao.selectById(swapId);
    if (swapEntity == null) {
        return Result.error("部门xxx不存在");
    }
    // 验证 3
    Long count = employeeDao.countByDepartmentId(departmentId);
    if (count != null && count > 0) {
        return Result.error("员工不存在");
    }
    // 操作数据库 4
    Long departmentSort = departmentEntity.getSort();
    departmentEntity.setSort(swapEntity.getSort());
    departmentDao.updateById(departmentEntity);
    swapEntity.setSort(departmentSort);
    departmentDao.updateById(swapEntity);
    return Result.OK("success");
}
```
上面代码在采用三层架构时经常会遇到，**那么它有什么问题呢？**<br />上面的代码是典型的长事务问题（_类似的还有调用第三方接口_），前三步都是使用 connection 进行验证操作，但是由于方法上有`@Transactional` 注解，所以这三个验证都是使用的同一个 connection。<br />若对于复杂业务、复杂的验证逻辑，会导致整个验证过程始终占用该 connection 连接，占用时间可能会很长，直至方法结束，connection 才会交还给数据库连接池。<br />对于复杂业务的不可预计的情况，长时间占用同一个 connection 连接不是好的事情，应该尽量缩短占用时间。<br />_说明：对于_`_@Transactional_`_ 注解，当 spring 遇到该注解时，会自动从数据库连接池中获取 connection，并开启事务然后绑定到 ThreadLocal 上，如果业务并没有进入到最终的 操作数据库环节，那么就没有必要获取连接并开启事务，应该直接将 connection 返回给数据库连接池，供其他使用。_<br />所以在加入Manager层以后可以这样写：
```java
public Result<String> upOrDown(Long departmentId, Long swapId) {
    // 验证 1
    DepartmentEntity departmentEntity = departmentDao.selectById(departmentId);
    if (departmentEntity == null) {
        return Result.error("部门xxx不存在");
    }
    // 验证 2
    DepartmentEntity swapEntity = departmentDao.selectById(swapId);
    if (swapEntity == null) {
        return Result.error("部门xxx不存在");
    }
    // 验证 3
    Long count = employeeDao.countByDepartmentId(departmentId);
    if (count != null && count > 0) {
        return Result.error("员工不存在");
    }

    // 操作数据库 4
    departmentManager.upOrDown(departmentSort,swapEntity);

    return Result.OK("success");
}
```
```java
@Transactional(rollbackFor = Throwable.class)
public void upOrDown(DepartmentEntity departmentEntity ,DepartmentEntity swapEntity){
    Long departmentSort = departmentEntity.getSort();
    departmentEntity.setSort(swapEntity.getSort());
    departmentDao.updateById(departmentEntity);
    swapEntity.setSort(departmentSort);
    departmentDao.updateById(swapEntity);
}
```
将数据在 service 层准备好，然后传递给 manager 层，由 manager 层添加`@Transactional`事务注解进行数据库操作。
