JavaJDBC<br />ResultSet对象维护一个指向表中某一行的游标。初始时，游标指向第一行之前。<br />默认情况下，ResultSet对象只能向前移动，且不可更新。<br />但是可以通过在`createStatement(int, int)`方法中传递`TYPE_SCROLL_INSENSITIVE`或TYPE_SCROLL_SENSITIVE，使该对象能够向前和向后移动，同时还可以通过以下方式将该对象设置为可更新：
```java
Statement stmt = con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
```
ResultSet接口的常用方法

| 1) `public boolean next()` | 用于将游标移动到当前位置的下一行。 |
| --- | --- |
| 2) `public boolean previous()` | 用于将游标移动到当前位置的上一行。 |
| 3) `public boolean first()` | 用于将游标移动到结果集对象的第一行。 |
| 4) `public boolean last()` | 用于将游标移动到结果集对象的最后一行。 |
| 5) `public boolean absolute(int row)` | 用于将游标移动到ResultSet对象中指定的行号。 |
| 6) `public boolean relative(int row)` | 用于将游标相对于ResultSet对象当前位置移动到指定的行号，可以是正数或负数。 |
| 7) `public int getInt(int columnIndex)` | 用于将当前行指定列索引的数据作为int返回。 |
| 8) `public int getInt(String columnName)` | 用于将当前行指定列名称的数据作为int返回。 |
| 9) `public String getString(int columnIndex)` | 用于将当前行指定列索引的数据作为String返回。 |
| 10) `public String getString(String columnName)` | 用于将当前行指定列名称的数据作为String返回。 |

<a name="GfnPB"></a>
## 可滚动`ResultSet`的示例
下面是使用ResultSet接口检索第3行数据的简单示例。
```java
import java.sql.*; 
class FetchRecord{ 
    public static void main(String args[])throws Exception{ 
        Class.forName("oracle.jdbc.driver.OracleDriver"); 
        Connection con=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe","system","oracle"); 
        Statement stmt=con.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,ResultSet.CONCUR_UPDATABLE); 
        ResultSet rs=stmt.executeQuery("select * from emp765"); 
        //获取第3行的记录
        rs.absolute(3); 
        System.out.println(rs.getString(1)+" "+rs.getString(2)+" "+rs.getString(3)); 
        con.close(); 
    }
}
```
