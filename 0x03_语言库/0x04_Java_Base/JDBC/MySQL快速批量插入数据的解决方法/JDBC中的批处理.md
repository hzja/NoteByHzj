与执行单个查询不同，可以执行一批（多个）查询。这样可以提高性能。因为当一次性将多个SQL语句发送给数据库时，通信开销显著降低，因为不需要频繁与数据库进行通信，从而提高了性能。<br />`java.sql.Statement`和`java.sql.PreparedStatement`接口提供了批处理的方法。
<a name="RlyBc"></a>
## **批处理的优点**
快速性能
<a name="Gzabc"></a>
## Statement接口的方法
批处理所需的方法如下所示：

| 方法 | 描述 |
| --- | --- |
| `void addBatch(String query)` | `CallableStatement`、`PreparedStatement`和`Statement`的`addBatch(String query)`方法用于将单个语句添加到批处理中。 |
| `int[] executeBatch()` | `executeBatch()`方法开始执行所有组合在一起的语句。该方法返回一个整数数组，数组的每个元素表示相应更新语句的更新计数。 |
| `boolean DatabaseMetaData.supportsBatchUpdates() throws SQLException` | 如果目标数据库支持批量更新处理，则该方法返回true。 |
| `void clearBatch()` | 该方法删除使用`addBatch()`方法添加的所有语句。 |

<a name="c0JRG"></a>
## JDBC中批处理的示例
看一个JDBC中批处理的简单示例。它遵循以下步骤：

- 加载驱动程序类
- 创建连接
- 创建语句
- 将查询添加到批处理中
- 执行批处理
- 关闭连接

文件名：FetchRecords.java
```java
import java.sql.*;

class FetchRecords{
    public static void main(String args[])throws Exception{
        Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection con=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe","system","oracle");
        con.setAutoCommit(false);

        Statement stmt=con.createStatement();
        stmt.addBatch("insert into user420 values(190,'abhi',40000)");
        stmt.addBatch("insert into user420 values(191,'umesh',50000)");

        stmt.executeBatch();//executing the batch

        con.commit();
        con.close();
    }
}
```
如果查看user420表，将会看到已添加两条记录。
<a name="qT52g"></a>
## 使用`PreparedStatement`的批处理示例
文件名：BP.java
```java
import java.sql.*;
import java.io.*;

class BP{
    public static void main(String args[]){
        try{
            Class.forName("oracle.jdbc.driver.OracleDriver");
            Connection con=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe","system","oracle");
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

                ps.addBatch();
                System.out.println("Want to add more records y/n");
                String ans=br.readLine();
                if(ans.equals("n")){
                    break;
                }

            }
            ps.executeBatch();// for executing the batch
            System.out.println("record successfully saved");
            con.close();
        }catch(Exception e){System.out.println(e);}
    }
}
```
输出：
```
enter id
101
enter name
Manoj Kumar
enter salary
10000
Want to add more records y/n
y
enter id
101
enter name
Harish Singh
enter salary
15000
Want to add more records y/n
y
enter id
103
enter name
Rohit Anuragi
enter salary
30000
Want to add more records y/n
y
enter id
104
enter name
Amrit Gautam
enter salary
40000
Want to add more records y/n
n
record successfully saved
```
它将把查询添加到批处理中，直到用户按下n键。最后，它执行批处理。因此，所有添加的查询都将被执行。
