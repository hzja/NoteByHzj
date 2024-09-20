OAuth2<br />在使用Keycloak的时候可能有同学都注意到用户的管理都是通过Keycloak提供的UI来进行的，虽然很方便但是很多时候并不适合在开发中使用。比如注册总不能让终端用户直接去Keycloak的管理控制台（Admin Console）操作吧。所以需要将这些功能API化才行，来分享一个通过编程对Keycloak进行操作的方法。
<a name="hXQqP"></a>
## Keycloak Admin Client简介
在Keycloak Admin Console中的所有操作都有特定的Restful API，被统称为Keycloak Admin REST API。而 Keycloak Admin Client正是对Keycloak Admin REST API的Java HTTP客户端封装。只需要引入下面的依赖就可以集成了：
```xml
<dependency>
  <groupId>org.keycloak</groupId>
  <artifactId>keycloak-admin-client</artifactId>
  <version>${version}</version>
</dependency>
```
这里简单提一下，其底层用的是JBoss的Rest Web Service客户端Resteasy。JBoss RESTEasy 是一个用来使用Java语言开发 RESTFul Web服务的框架。它是 JAX-RS（Java API for RESTful Web Services） 的一个实现，它的一些亮点：

- 不需要配置文件，基于注解和Java POJO就可以实现RESTful客户端。
- 基于JBoss  Seam（Java EE的上层强化）编程模型。

这些只作为扩展知识，除非深入定制，否则不需要学习它，因为 Keycloak Admin Client已经屏蔽了其陡峭的学习成本，接下来开始使用它。
<a name="XHWyW"></a>
## Keycloak Admin Client的使用
Keycloak Admin REST API都需要在请求中放置一个Bearer Token的Authorization请求头。根据Token中携带的权限信息来获取对应API的访问权限。所以在使用Keycloak Admin Client时要特别注意当前使用的客户端是否有权限访问。接下来的例子就拿注册新用户为例来使用它。
<a name="Caxy7"></a>
### 使用Admin账户创建新用户
Master Realm中的Admin管理员拥有管理Keycloak的最高权限，使用它几乎可以在Keycloak中“为所欲为”。<br />根据配置声明一个Keyclock实例：
```java
Keycloak adminCli = KeycloakBuilder.builder()
    // 服务器地址
    .serverUrl("http://localhost:8011/auth/")
    // master领域
    .realm("master")
    // 客户端为 admin-cli  
    .clientId("admin-cli")
    // 需要在该客户端下生成密钥
    .clientSecret("f7da6497-98ee-455a-87ba-158793134e56")
    // 管理员账户
    .username("admin")
    // 管理员密码 
    .password("admin")
    // 授权方式是密码授权 
    .grantType(OAuth2Constants.PASSWORD)
    .build();
```
这里授权方式为密码授权，要求必须在客户端的Setting选项下打开Direct Access Grants Enabled，这意味着admin-cli客户端能够访问用户的用户名和密码，并以此从Keycloak服务器获取访问令牌，继而能够进行进一步的访问授权操作。<br />那么如何注册新用户呢？<br />可以这样实现：
```java
UserRepresentation user = new UserRepresentation();
user.setUsername("apicreated");
user.setEnabled(true);

CredentialRepresentation credentialRepresentation = new CredentialRepresentation();
credentialRepresentation.setTemporary(false);
credentialRepresentation.setType(CredentialRepresentation.PASSWORD);
credentialRepresentation.setValue("123456");

user.setCredentials(Collections.singletonList(credentialRepresentation));

RealmResource realm = adminCli.realm("master");

UsersResource users = realm.users();

Response response = users.create(user);
System.out.println("response = " + response.readEntity(String.class));
```
这里的UserRepresentation就是用户对象，定义了一个用户名为apicreated、非临时密码为123456的用户并注册到Master Realm中。<br />官方给了一个Admin API的说明文档，地址为：[https://www.keycloak.org/docs-api/15.0/rest-api/index.html](https://www.keycloak.org/docs-api/15.0/rest-api/index.html)<br />这是使用Keycloak Admin REST API必须看的文档。
<a name="israW"></a>
### 使用Realm管理用户创建新用户
Master Realm的管理员账户不屑于干这些“低级”的操作，这种事一般会交给“小弟”去做。为了在fcant.cn这个Realm创建用户，可以给一个Master Realm的用户赋予一个创建fcant.cn用户的角色manager-users:<br />![2021-08-26-15-48-41-459646.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629964836889-5290ec79-807c-49da-922b-e967d037cdda.png#clientId=ud4017daa-56e9-4&from=ui&id=u095c0cea&originHeight=392&originWidth=1080&originalType=binary&ratio=1&size=62595&status=done&style=shadow&taskId=u68e4d117-366d-42c7-9870-42618727c43)<br />创建一个管理给特定的Realm用户<br />红框中还有很多角色需要去了解。<br />利用Master账户给fcant.cn创建用户应该这么写：
```java
Keycloak adminCli = KeycloakBuilder.builder()
    .serverUrl("http://localhost:8011/auth/")
    .realm("master")
    .clientId("admin-cli")
    .clientSecret("86ef2225-14d4-49b1-908e-2b5e058030cc")
    .username("fcantadmin")
    .password("123456")
    .grantType(OAuth2Constants.PASSWORD)
    .build();
// 创建用户        
UserRepresentation user = new UserRepresentation();
user.setUsername("apicreated2");
user.setEnabled(true);

CredentialRepresentation credentialRepresentation = new CredentialRepresentation();
credentialRepresentation.setTemporary(false);
credentialRepresentation.setType(CredentialRepresentation.PASSWORD);
credentialRepresentation.setValue("123456");

user.setCredentials(Collections.singletonList(credentialRepresentation));

RealmResource realm = adminCli.realm("fcant.cn");

UsersResource users = realm.users();

Response response = users.create(user);
```
<a name="ANYoh"></a>
### 利用服务账户创建用户
每个Realm都有一个叫realm-management的管理客户端，这个客户端的作用就是用来管理当前的Realm。按照下面的配置可以开启realm-management的服务账户功能。<br />![2021-08-26-15-48-41-636656.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629964836906-813474ae-eae3-4218-9c5a-e4de3d6ad1b5.png#clientId=ud4017daa-56e9-4&from=ui&id=tU7Op&originHeight=742&originWidth=916&originalType=binary&ratio=1&size=54217&status=done&style=shadow&taskId=u3aa7def4-a996-4ab8-a341-c505a304937)<br />开启服务账户功能<br />这样可以直接向Keycloak服务器获取realm-management的访问凭据，因为realm-management有全部的管理功能，所以可以以客户端的名义而非管理用户的名义创建新用户了，而且不局限于创建用户。<br />Keycloak的实例化代码就跟之前不大一样了：
```java
Keycloak adminCli = KeycloakBuilder.builder()
        .serverUrl("http://localhost:8011/auth/")
        .realm("fcant.cn")
        .clientId("realm-management")
        .clientSecret("38836e47-2c82-4412-a858-9be2a35aa366")
        .grantType(OAuth2Constants.CLIENT_CREDENTIALS)
        .build();
```
这里的授权模式区别于用户行为，它属于客户端行为，因此`grant_type`改成了客户端模式。
<a name="hp3Zp"></a>
## 总结
创建用户和前面两种方法相同，可以创建一个用户试试，还有其它的API都可以用这种方式实现。介绍了如何调用Keycloak Admin REST API，它可以实现在代码中对Keycloak进行一些管理操作。需要注意的是，这些操作和当前操作主体的角色息息相关。
