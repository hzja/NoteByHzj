Java<br />元数据（metadata）表示关于数据的数据，即可以从数据中获取更多信息。<br />如果需要获取表的元数据，比如总列数、列名、列类型等，可以使用`ResultSetMetaData`接口，因为它提供了从`ResultSet`对象中获取元数据的方法。
<a name="Z9FxV"></a>
### ResultSetMetaData接口常用方法
| 方法 | 描述 |
| --- | --- |
| `public int getColumnCount() throws SQLException` | 返回ResultSet对象中的列总数。 |
| `public String getColumnName(int index) throws SQLException` | 返回指定列索引的列名。 |
| `public String getColumnTypeName(int index) throws SQLException` | 返回指定索引的列类型名。 |
| `public String getTableName(int index) throws SQLException` | 返回指定列索引的表名。 |

<a name="CJF4j"></a>
### **获取**`**ResultSetMetaData**`**对象**
`ResultSet`接口的`getMetaData()`方法返回`ResultSetMetaData`的对象。语法如下：
```java
public ResultSetMetaData getMetaData() throws SQLException
```
<a name="opx0A"></a>
### **ResultSetMetaData接口示例**
```java
import java.sql.*;

class Rsmd {
    public static void main(String args[]) {
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:xe", "system", "oracle");

            PreparedStatement ps = con.prepareStatement("select * from emp");
            ResultSet rs = ps.executeQuery();
            ResultSetMetaData rsmd = rs.getMetaData();

            System.out.println("Total columns: " + rsmd.getColumnCount());
            System.out.println("Column Name of 1st column: " + rsmd.getColumnName(1));
            System.out.println("Column Type Name of 1st column: " + rsmd.getColumnTypeName(1));

            con.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
```
输出：
```
Total columns: 2
Column Name of 1st column: ID
Column Type Name of 1st column: NUMBER
```
