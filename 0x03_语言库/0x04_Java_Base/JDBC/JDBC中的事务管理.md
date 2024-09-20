JavaJDBC<br />事务代表一个单独的工作单位。<br />ACID属性很好地描述了事务管理。ACID代表原子性（Atomicity）、一致性（Consistency）、隔离性（Isolation）和持久性（Durability）。<br />原子性意味着要么全部成功，要么全部失败。<br />一致性确保将数据库从一致状态带到另一个一致状态。<br />隔离性确保事务与其他事务隔离。<br />持久性意味着一旦事务提交，即使发生错误、断电等情况，它也会保持提交状态。
<a name="RQOwp"></a>
### 事务管理的优点
快速性能：由于在提交时访问数据库，因此性能较快。<br />在JDBC中，`Connection`接口提供了管理事务的方法。

| 方法 | 描述 |
| --- | --- |
| `void setAutoCommit(boolean status)` | 默认为true，表示每个事务默认都是提交的。 |
| `void commit()` | 提交事务。 |
| `void rollback()` | 取消事务。 |

<a name="BO3y8"></a>
### **使用**`**Statement**`**进行JDBC事务管理的简单示例**
看一下使用`Statement`进行事务管理的简单示例。
```java
import java.sql.*;

class FetchRecords{
    public static void main(String args[])throws Exception{
        Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection con=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe","system","oracle");
        con.setAutoCommit(false);

        Statement stmt=con.createStatement();
        stmt.executeUpdate("insert into user420 values(190,'abhi',40000)");
        stmt.executeUpdate("insert into user420 values(191,'umesh',50000)");

        con.commit();
        con.close();
    }
}
```
如果查看user420表，将看到已添加2条记录。
<a name="pwyVq"></a>
### **使用**`**PreparedStatement**`**进行JDBC事务管理的示例**
看一下使用`PreparedStatement`进行事务管理的简单示例
```java
import java.sql.*;
import java.io.*;

class TM{
    public static void main(String args[]){
        try{

            Class.forName("oracle.jdbc.driver.OracleDriver");
            Connection con=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe","system","oracle");
            con.setAutoCommit(false);

            PreparedStatement ps=con.prepareStatement("insert into user420 values(?,?,?)");

            BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
            while(true){

                System.out.println("enter id");
                String s1=br.readLine();
                int id=Integer.parseInt(s1);

                System.out.println("enter name");
                String name=br.readLine();

                System.out.println("enter salary");
                String s3=br.readLine();
                int salary=Integer.parseInt(s3);

                ps.setInt(1,id);
                ps.setString(2,name);
                ps.setInt(3,salary);
                ps.executeUpdate();

                System.out.println("commit/rollback");
                String answer=br.readLine();
                if(answer.equals("commit")){
                    con.commit();
                }
                if(answer.equals("rollback")){
                    con.rollback();
                }

                System.out.println("Want to add more records y/n");
                String ans=br.readLine();
                if(ans.equals("n")){
                    break;
                }

            }
            con.commit();
            System.out.println("record successfully saved");

            con.close();//before closing connection commit() is called
        }catch(Exception e){System.out.println(e);}

    }
}
```
它会要求用户添加更多记录，直到按下n键。如果按下n键，事务将被提交。
