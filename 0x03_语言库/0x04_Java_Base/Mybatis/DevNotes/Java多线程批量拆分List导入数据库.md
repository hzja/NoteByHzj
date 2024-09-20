Java 
<a name="AHA6g"></a>
## | 直接把list怼进MySQL
使用mybatis的批量导入操作：
```java
@Transactional(rollbackFor = Exception.class)
public int addFreshStudentsNew2(List<FreshStudentAndStudentModel> list, String schoolNo) {
    if (list == null || list.isEmpty()) {
        return 0;
    }
    List<StudentEntity> studentEntityList = new LinkedList<>();
    List<EnrollStudentEntity> enrollStudentEntityList = new LinkedList<>();
    List<AllusersEntity> allusersEntityList = new LinkedList<>();

    for (FreshStudentAndStudentModel freshStudentAndStudentModel : list) {

        EnrollStudentEntity enrollStudentEntity = new EnrollStudentEntity();
        StudentEntity studentEntity = new StudentEntity();
        BeanUtils.copyProperties(freshStudentAndStudentModel, studentEntity);
        BeanUtils.copyProperties(freshStudentAndStudentModel, enrollStudentEntity);
        String operator = TenancyContext.UserID.get();
        String studentId = BaseUuidUtils.base58Uuid();
        enrollStudentEntity.setId(BaseUuidUtils.base58Uuid());
        enrollStudentEntity.setStudentId(studentId);
        enrollStudentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        enrollStudentEntity.setOperator(operator);
        studentEntity.setId(studentId);
        studentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        studentEntity.setOperator(operator);
        studentEntityList.add(studentEntity);
        enrollStudentEntityList.add(enrollStudentEntity);

        AllusersEntity allusersEntity = new AllusersEntity();
        allusersEntity.setId(enrollStudentEntity.getId());
        allusersEntity.setUserCode(enrollStudentEntity.getNemtCode());
        allusersEntity.setUserName(enrollStudentEntity.getName());
        allusersEntity.setSchoolNo(schoolNo);
        allusersEntity.setTelNum(enrollStudentEntity.getTelNum());
        allusersEntity.setPassword(enrollStudentEntity.getNemtCode());  //密码设置为考生号
        allusersEntityList.add(allusersEntity);
    }
    enResult = enrollStudentDao.insertAll(enrollStudentEntityList);
    stuResult = studentDao.insertAll(studentEntityList);
    allResult = allusersFacade.insertUserList(allusersEntityList);

    if (enResult > 0 && stuResult > 0 && allResult) {
        return 10;
    }
    return -10;
}
```
Mapper.xml
```xml
<insert id="insertAll" parameterType="com.dmsdbj.itoo.basicInfo.entity.EnrollStudentEntity">
  insert into tb_enroll_student
  <trim prefix="(" suffix=")" suffixOverrides=",">
    id,  
    remark,  
    nEMT_aspiration,  
    nEMT_code,  
    nEMT_score,  
    student_id,  
    identity_card_id,  
    level,  
    major,  
    name,  
    nation,  
    secondary_college,  
    operator,  
    sex,  
    is_delete,  
    account_address,  
    native_place,  
    original_place,  
    used_name,  
    pictrue,  
    join_party_date,  
    political_status,  
    tel_num,  
    is_registry,  
    graduate_school,  
    create_time,  
    update_time        </trim>        
  values
  <foreach collection="list" item="item" index="index" separator=",">
    (
    #{item.id,jdbcType=VARCHAR},
    #{item.remark,jdbcType=VARCHAR},
    #{item.nemtAspiration,jdbcType=VARCHAR},
    #{item.nemtCode,jdbcType=VARCHAR},
    #{item.nemtScore,jdbcType=VARCHAR},
    #{item.studentId,jdbcType=VARCHAR},
    #{item.identityCardId,jdbcType=VARCHAR},
    #{item.level,jdbcType=VARCHAR},
    #{item.major,jdbcType=VARCHAR},
    #{item.name,jdbcType=VARCHAR},
    #{item.nation,jdbcType=VARCHAR},
    #{item.secondaryCollege,jdbcType=VARCHAR},
    #{item.operator,jdbcType=VARCHAR},
    #{item.sex,jdbcType=VARCHAR},
    0,
    #{item.accountAddress,jdbcType=VARCHAR},
    #{item.nativePlace,jdbcType=VARCHAR},
    #{item.originalPlace,jdbcType=VARCHAR},
    #{item.usedName,jdbcType=VARCHAR},
    #{item.pictrue,jdbcType=VARCHAR},
    #{item.joinPartyDate,jdbcType=VARCHAR},
    #{item.politicalStatus,jdbcType=VARCHAR},
    #{item.telNum,jdbcType=VARCHAR},
    #{item.isRegistry,jdbcType=TINYINT},
    #{item.graduateSchool,jdbcType=VARCHAR},
    now(),
    now()        
    )   
  </foreach>                
</insert> 
```
代码说明：<br />底层的mapper是通过逆向工程来生成的，批量插入如下，是拼接成类似：`insert into tb_enroll_student()values ()，()…….();`<br />这样的缺点是，数据库一般有一个默认的设置，就是每次sql操作的数据不能超过4M。这样插入，数据多的时候，数据库会报错`Packet for query is too large (6071393 > 4194304). You can change this value on the server by setting the max_allowed_packet' variable.`，虽然可以通过<br />类似 修改 my.ini 加上 `max_allowed_packet =67108864`，`67108864=64M`，默认大小4194304 也就是4M<br />修改完成之后要重启mysql服务，如果通过命令行修改就不用重启mysql服务。<br />完成本次操作，但是不能保证项目单次最大的大小是多少，这样是有弊端的。所以可以考虑进行分组导入。
<a name="vUybb"></a>
## | 分组把list导入MySQL中
同样适用mybatis批量插入，区别是对每次的导入进行分组计算，然后分多次进行导入：
```java
@Transactional(rollbackFor = Exception.class)
public int addFreshStudentsNew2(List<FreshStudentAndStudentModel> list, String schoolNo) {
    if (list == null || list.isEmpty()) {
        return 0;
    }
    List<StudentEntity> studentEntityList = new LinkedList<>();
    List<EnrollStudentEntity> enrollStudentEntityList = new LinkedList<>();
    List<AllusersEntity> allusersEntityList = new LinkedList<>();

    for (FreshStudentAndStudentModel freshStudentAndStudentModel : list) {

        EnrollStudentEntity enrollStudentEntity = new EnrollStudentEntity();
        StudentEntity studentEntity = new StudentEntity();
        BeanUtils.copyProperties(freshStudentAndStudentModel, studentEntity);
        BeanUtils.copyProperties(freshStudentAndStudentModel, enrollStudentEntity);
        String operator = TenancyContext.UserID.get();
        String studentId = BaseUuidUtils.base58Uuid();
        enrollStudentEntity.setId(BaseUuidUtils.base58Uuid());
        enrollStudentEntity.setStudentId(studentId);
        enrollStudentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        enrollStudentEntity.setOperator(operator);
        studentEntity.setId(studentId);
        studentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        studentEntity.setOperator(operator);
        studentEntityList.add(studentEntity);
        enrollStudentEntityList.add(enrollStudentEntity);

        AllusersEntity allusersEntity = new AllusersEntity();
        allusersEntity.setId(enrollStudentEntity.getId());
        allusersEntity.setUserCode(enrollStudentEntity.getNemtCode());
        allusersEntity.setUserName(enrollStudentEntity.getName());
        allusersEntity.setSchoolNo(schoolNo);
        allusersEntity.setTelNum(enrollStudentEntity.getTelNum());
        allusersEntity.setPassword(enrollStudentEntity.getNemtCode());  //密码设置为考生号
        allusersEntityList.add(allusersEntity);
    }

    int c = 100;
    int b = enrollStudentEntityList.size() / c;
    int d = enrollStudentEntityList.size() % c;

    int enResult = 0;
    int stuResult = 0;
    boolean allResult = false;

    for (int e = c; e <= c * b; e = e + c) {
        enResult = enrollStudentDao.insertAll(enrollStudentEntityList.subList(e - c, e));
        stuResult = studentDao.insertAll(studentEntityList.subList(e - c, e));
        allResult = allusersFacade.insertUserList(allusersEntityList.subList(e - c, e));
    }
    if (d != 0) {
        enResult = enrollStudentDao.insertAll(enrollStudentEntityList.subList(c * b, enrollStudentEntityList.size()));
        stuResult = studentDao.insertAll(studentEntityList.subList(c * b, studentEntityList.size()));
        allResult = allusersFacade.insertUserList(allusersEntityList.subList(c * b, allusersEntityList.size()));
    }

    if (enResult > 0 && stuResult > 0 && allResult) {
        return 10;
    }
    return -10;
}
```
代码说明：<br />这样操作，可以避免上面的错误，但是分多次插入，无形中就增加了操作实践，很容易超时。所以这种方法还是不值得提倡的。<br />再次改进，使用多线程分批导入。
<a name="tLQgo"></a>
## | 多线程分批导入MySQL
依然使用mybatis的批量导入，不同的是，根据线程数目进行分组，然后再建立多线程池，进行导入。
```java
@Transactional(rollbackFor = Exception.class)
public int addFreshStudentsNew(List<FreshStudentAndStudentModel> list, String schoolNo) {
    if (list == null || list.isEmpty()) {
        return 0;
    }
    List<StudentEntity> studentEntityList = new LinkedList<>();
    List<EnrollStudentEntity> enrollStudentEntityList = new LinkedList<>();
    List<AllusersEntity> allusersEntityList = new LinkedList<>();

    list.forEach(freshStudentAndStudentModel -> {
        EnrollStudentEntity enrollStudentEntity = new EnrollStudentEntity();
        StudentEntity studentEntity = new StudentEntity();
        BeanUtils.copyProperties(freshStudentAndStudentModel, studentEntity);
        BeanUtils.copyProperties(freshStudentAndStudentModel, enrollStudentEntity);
        String operator = TenancyContext.UserID.get();
        String studentId = BaseUuidUtils.base58Uuid();
        enrollStudentEntity.setId(BaseUuidUtils.base58Uuid());
        enrollStudentEntity.setStudentId(studentId);
        enrollStudentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        enrollStudentEntity.setOperator(operator);
        studentEntity.setId(studentId);
        studentEntity.setIdentityCardId(freshStudentAndStudentModel.getIdCard());
        studentEntity.setOperator(operator);
        studentEntityList.add(studentEntity);
        enrollStudentEntityList.add(enrollStudentEntity);

        AllusersEntity allusersEntity = new AllusersEntity();
        allusersEntity.setId(enrollStudentEntity.getId());
        allusersEntity.setUserCode(enrollStudentEntity.getNemtCode());
        allusersEntity.setUserName(enrollStudentEntity.getName());
        allusersEntity.setSchoolNo(schoolNo);
        allusersEntity.setTelNum(enrollStudentEntity.getTelNum());
        allusersEntity.setPassword(enrollStudentEntity.getNemtCode());  //密码设置为考生号
        allusersEntityList.add(allusersEntity);
    });


    int nThreads = 50;

    int size = enrollStudentEntityList.size();
    ExecutorService executorService = Executors.newFixedThreadPool(nThreads);
    List<Future<Integer>> futures = new ArrayList<Future<Integer>>(nThreads);

    for (int i = 0; i < nThreads; i++) {
        final List<EnrollStudentEntity> EnrollStudentEntityImputList = enrollStudentEntityList.subList(size / nThreads * i, size / nThreads * (i + 1));
        final List<StudentEntity> studentEntityImportList = studentEntityList.subList(size / nThreads * i, size / nThreads * (i + 1));
        final List<AllusersEntity> allusersEntityImportList = allusersEntityList.subList(size / nThreads * i, size / nThreads * (i + 1));

        Callable<Integer> task1 = () -> {
            studentSave.saveStudent(EnrollStudentEntityImputList,studentEntityImportList,allusersEntityImportList);
            return 1;
        };
        futures.add(executorService.submit(task1));
    }
    executorService.shutdown();
    if (!futures.isEmpty() && futures != null) {
        return 10;
    }
    return -10;
}
```
代码说明：<br />上面是通过应用`ExecutorService` 建立了固定的线程数，然后根据线程数目进行分组，批量依次导入。一方面可以缓解数据库的压力，另一个面线程数目多了，一定程度会提高程序运行的时间。缺点就是要看服务器的配置，如果配置好的话就可以开多点线程，配置差的话就开小点。
