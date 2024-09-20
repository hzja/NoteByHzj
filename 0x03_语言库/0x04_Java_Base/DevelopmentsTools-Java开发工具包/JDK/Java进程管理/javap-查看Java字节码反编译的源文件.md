Java javap
<a name="kRQLQ"></a>
### `javap -help` ：javap命令的帮助命令
```bash
❯ javap -help
用法: javap <options> <classes>
其中, 可能的选项包括:
  -help  --help  -?        输出此用法消息
  -version                 版本信息
  -v  -verbose             输出附加信息
  -l                       输出行号和本地变量表
  -public                  仅显示公共类和成员
  -protected               显示受保护的/公共类和成员
  -package                 显示程序包/受保护的/公共类
                           和成员 (默认)
  -p  -private             显示所有类和成员
  -c                       对代码进行反汇编
  -s                       输出内部类型签名
  -sysinfo                 显示正在处理的类的
                           系统信息 (路径, 大小, 日期, MD5 散列)
  -constants               显示最终常量
  -classpath <path>        指定查找用户类文件的位置
  -cp <path>               指定查找用户类文件的位置
  -bootclasspath <path>    覆盖引导类文件的位置
```
<a name="ytzMA"></a>
### javap-命令参数
javap的命令格式如下

| javap -help -help -? | 输出此用法消息 |
| --- | --- |
| javap -version | 版本信息 |
| javap _v -verbose | 17.09 |
| javap -public | 仅显示公共类和成员 |
| javap -protected | 显示受保护的/公共类和成员 |
| javap -package | 显示程序包/受保护的/公共类和成员(默认) |
| javap -p -private | 显示所有类和成员 |
| javap -c | 对代码进行反汇编 |
| javap -s | 输出内部类型签名 |
| javap -sysinfo | 显示正在处理的类的系统信息(路径，大小，日期，MD5散列) |
| javap -constants | 显示静态最终常量 |
| javap -classpath <path> | 指定查找用户类文件的位置 |
| javap -bootclasspath <path> | 覆盖引导类文件的位置 |

<a name="VEqLT"></a>
### `javap <ClassFileName.class>` ：对该class文件反编译输出
```bash
❯ javap UserServiceImpl.class
Compiled from "UserServiceImpl.java"
public class com.fcant.competition.service.impl.UserServiceImpl implements com.fcant.competition.service.IUserService {
  com.fcant.competition.mapper.UserMapper userMapper;
  com.fcant.competition.mapper.SaveFileMapper saveFileMapper;
  com.fcant.competition.mapper.RoleMapper roleMapper;
  com.fcant.competition.mapper.GroupMapper groupMapper;
  com.fcant.competition.mapper.SocietyMapper societyMapper;
  com.fcant.competition.mapper.CampusInfoMapper campusInfoMapper;
  com.fcant.competition.mapper.CompanyInfoMapper companyInfoMapper;
  com.fcant.competition.mapper.SchoolInfoMapper schoolInfoMapper;
  public com.fcant.competition.service.impl.UserServiceImpl();
  public com.fcant.competition.utils.MsgUtil updateUser(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil getUserByName(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil getUserInfo(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil register(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil login(com.fcant.competition.bean.User);
  public com.fcant.competition.bean.User toPerfectUserInfo(com.fcant.competition.bean.User);
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594979440844-18634b4f-de2a-4ecd-b0b3-1e1209631121.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2122088&status=done&style=none&width=1107.6666666666667)
<a name="tLxSz"></a>
###  javap -s <ClassFileName.class> ：反编译class文件并输出内部类型签名
```bash
❯ javap -s UserServiceImpl.class
Compiled from "UserServiceImpl.java"
public class com.fcant.competition.service.impl.UserServiceImpl implements com.fcant.competition.service.IUserService {
  com.fcant.competition.mapper.UserMapper userMapper;
    descriptor: Lcom/fcant/competition/mapper/UserMapper;
  com.fcant.competition.mapper.SaveFileMapper saveFileMapper;
    descriptor: Lcom/fcant/competition/mapper/SaveFileMapper;
  com.fcant.competition.mapper.RoleMapper roleMapper;
    descriptor: Lcom/fcant/competition/mapper/RoleMapper;
  com.fcant.competition.mapper.GroupMapper groupMapper;
    descriptor: Lcom/fcant/competition/mapper/GroupMapper;
  com.fcant.competition.mapper.SocietyMapper societyMapper;
    descriptor: Lcom/fcant/competition/mapper/SocietyMapper;
  com.fcant.competition.mapper.CampusInfoMapper campusInfoMapper;
    descriptor: Lcom/fcant/competition/mapper/CampusInfoMapper;
  com.fcant.competition.mapper.CompanyInfoMapper companyInfoMapper;
    descriptor: Lcom/fcant/competition/mapper/CompanyInfoMapper;
  com.fcant.competition.mapper.SchoolInfoMapper schoolInfoMapper;
    descriptor: Lcom/fcant/competition/mapper/SchoolInfoMapper;
  public com.fcant.competition.service.impl.UserServiceImpl();
    descriptor: ()V

  public com.fcant.competition.utils.MsgUtil updateUser(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/utils/MsgUtil;

  public com.fcant.competition.utils.MsgUtil getUserByName(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/utils/MsgUtil;

  public com.fcant.competition.utils.MsgUtil getUserInfo(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/utils/MsgUtil;

  public com.fcant.competition.utils.MsgUtil register(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/utils/MsgUtil;

  public com.fcant.competition.utils.MsgUtil login(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/utils/MsgUtil;

  public com.fcant.competition.bean.User toPerfectUserInfo(com.fcant.competition.bean.User);
    descriptor: (Lcom/fcant/competition/bean/User;)Lcom/fcant/competition/bean/User;
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594979879968-212a06c4-df99-408c-a8fe-f8be4d134441.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2170259&status=done&style=none&width=1107.6666666666667)
<a name="h8WCQ"></a>
###  javap -sysinfo <ClassFileName.class>：反编译class文件并输出处理类的系统信息
```bash
❯ javap -sysinfo UserServiceImpl.class
Classfile /D:/LinkSpace/Work/WorkSpace/Code/IDEA/Competition/target/classes/com/fcant/competition/service/impl/UserServiceImpl.class
  Last modified 2020-5-10; size 7333 bytes
  MD5 checksum f1b80f2c1e6f4e3416b4036f321609ed
  Compiled from "UserServiceImpl.java"
public class com.fcant.competition.service.impl.UserServiceImpl implements com.fcant.competition.service.IUserService {
  com.fcant.competition.mapper.UserMapper userMapper;
  com.fcant.competition.mapper.SaveFileMapper saveFileMapper;
  com.fcant.competition.mapper.RoleMapper roleMapper;
  com.fcant.competition.mapper.GroupMapper groupMapper;
  com.fcant.competition.mapper.SocietyMapper societyMapper;
  com.fcant.competition.mapper.CampusInfoMapper campusInfoMapper;
  com.fcant.competition.mapper.CompanyInfoMapper companyInfoMapper;
  com.fcant.competition.mapper.SchoolInfoMapper schoolInfoMapper;
  public com.fcant.competition.service.impl.UserServiceImpl();
  public com.fcant.competition.utils.MsgUtil updateUser(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil getUserByName(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil getUserInfo(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil register(com.fcant.competition.bean.User);
  public com.fcant.competition.utils.MsgUtil login(com.fcant.competition.bean.User);
  public com.fcant.competition.bean.User toPerfectUserInfo(com.fcant.competition.bean.User);
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594980131117-78c7285d-25c2-4800-830f-09c70daf1f0e.png#align=left&display=inline&height=511&originHeight=1532&originWidth=3323&size=1932530&status=done&style=none&width=1107.6666666666667)
