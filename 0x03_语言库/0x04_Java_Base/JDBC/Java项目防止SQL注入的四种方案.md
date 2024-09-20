Java
<a name="TU4Fo"></a>
## 一、什么是SQL注入？
SQL注入即是指web应用程序对用户输入数据的合法性没有判断或过滤不严，攻击者可以在web应用程序中事先定义好的查询语句的结尾上添加额外的SQL语句，在管理员不知情的情况下实现非法操作，以此来实现欺骗数据库服务器执行非授权的任意查询，从而进一步得到相应的数据信息。<br />SQL案列
```sql
String sql = "delete from table1 where id = " + "id";
```
这个id从请求参数中获取，若参数被拼接为：
```
1001 or  1 = 1
```
最执行语句变为：
```sql
String sql = "delete from table1 where id = 1001 or 1 = 1";
```
此时，数据库的数据都会被清空掉，后果非常严重
<a name="yA8Gg"></a>
## 二、Java项目防止SQL注入方式
这里总结4种：

- `PreparedStatement`防止SQL注入
- Mybatis中`#{}`防止SQL注入
- 对请求参数的敏感词汇进行过滤
- Nginx反向代理防止SQL注入
<a name="QpU3d"></a>
### 1、`PreparedStatement`防止SQL注入
`PreparedStatement`具有预编译功能，以上述SQL为例<br />使用`PreparedStatement`预编译后的SQL为：
```sql
delete from table1 where id= ?
```
此时SQL语句结构已固定，无论"?"被替换为任何参数，SQL语句只认为where后面只有一个条件，当再传入 1001 or  1 = 1时，语句会报错，从而达到防止SQL注入效果
<a name="ihODz"></a>
### 2、Mybatis中`#{}`防止SQL注入
Mybatis中`#{}`表达式防止SQL注入与`PreparedStatement`类似，都是对SQL语句进行预编译处理<br />**注意：**<br />`#{}` ：参数占位符<br />`${}` ：拼接替换符，不能防止SQL注入，一般用于

- 传入数据库对象（如：数据库名称、表名）
- order by  后的条件
<a name="gXze6"></a>
### 3、对请求参数的敏感词汇进行过滤
这里是SpringBoot的写法，如下：
```java
import org.springframework.context.annotation.Configuration;
import javax.servlet.*;
import javax.servlet.annotation.WebFilter;
import java.io.IOException;
import java.util.Enumeration;

@WebFilter(urlPatterns = "/*",filterName = "sqlFilter")
@Configuration
public class SqlFilter implements Filter {

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {}

    /**
     * @description sql注入过滤
     */
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        ServletRequest request = servletRequest;
        ServletResponse response = servletResponse;
        // 获得所有请求参数名
        Enumeration<String> names = request.getParameterNames();
        String sql = "";
        while (names.hasMoreElements()){
            // 得到参数名
            String name = names.nextElement().toString();
            // 得到参数对应值
            String[] values = request.getParameterValues(name);
            for (int i = 0; i < values.length; i++) {
                sql += values[i];
            }
        }
        if (sqlValidate(sql)) {
            //TODO 这里直接抛异常处理，前后端交互项目中，请把错误信息按前后端"数据返回的VO"对象进行封装
            throw new IOException("您发送请求中的参数中含有非法字符");
        } else {
            filterChain.doFilter(request,response);
        }
    }

    /**
     * @description 匹配效验
     */
    protected static boolean sqlValidate(String str){
        // 统一转为小写
        String s = str.toLowerCase();
        // 过滤掉的sql关键字，特殊字符前面需要加\\进行转义
        String badStr =
        "select|update|and|or|delete|insert|truncate|char|into|substr|ascii|declare|exec|count|master|into|drop|execute|table|"+
        "char|declare|sitename|xp_cmdshell|like|from|grant|use|group_concat|column_name|" +
        "information_schema.columns|table_schema|union|where|order|by|" +
        "'\\*|\\;|\\-|\\--|\\+|\\,|\\//|\\/|\\%|\\#";
        //使用正则表达式进行匹配
        boolean matches = s.matches(badStr);
        return matches;
    }

    @Override
    public void destroy() {}
}
```
<a name="eCDW7"></a>
### 4、Nginx反向代理防止SQL注入
越来越多网站使用Nginx进行反向代理，该层也可以进行防止SQL注入配置。<br />将下面的Nginx配置文件代码放入到server块中，然后重启Nginx即可
```nginx
if ($request_method !~* GET|POST) { return 444; }
#使用444错误代码可以更加减轻服务器负载压力。
#防止SQL注入
if ($query_string ~* (\$|'|--|[+|(%20)]union[+|(%20)]|[+|(%20)]insert[+|(%20)]|[+|(%20)]drop[+|(%20)]|[+|(%20)]truncate[+|(%20)]|[+|(%20)]update[+|(%20)]|[+|(%20)]from[+|(%20)]|[+|(%20)]grant[+|(%20)]|[+|(%20)]exec[+|(%20)]|[+|(%20)]where[+|(%20)]|[+|(%20)]select[+|(%20)]|[+|(%20)]and[+|(%20)]|[+|(%20)]or[+|(%20)]|[+|(%20)]count[+|(%20)]|[+|(%20)]exec[+|(%20)]|[+|(%20)]chr[+|(%20)]|[+|(%20)]mid[+|(%20)]|[+|(%20)]like[+|(%20)]|[+|(%20)]iframe[+|(%20)]|[\<|%3c]script[\>|%3e]|javascript|alert|webscan|dbappsecurity|style|confirm\(|innerhtml|innertext)(.*)$) { return 555; }
if ($uri ~* (/~).*) { return 501; }
if ($uri ~* (\\x.)) { return 501; }
#防止SQL注入 
if ($query_string ~* "[;'<>].*") { return 509; }
if ($request_uri ~ " ") { return 509; }
if ($request_uri ~ (\/\.+)) { return 509; }
if ($request_uri ~ (\.+\/)) { return 509; }
#if ($uri ~* (insert|select|delete|update|count|master|truncate|declare|exec|\*|\')(.*)$ ) { return 503; }
#防止SQL注入
if ($request_uri ~* "(cost\()|(concat\()") { return 504; }
if ($request_uri ~* "[+|(%20)]union[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]and[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]select[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]or[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]delete[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]update[+|(%20)]") { return 504; }
if ($request_uri ~* "[+|(%20)]insert[+|(%20)]") { return 504; }
if ($query_string ~ "(<|%3C).*script.*(>|%3E)") { return 505; }
if ($query_string ~ "GLOBALS(=|\[|\%[0-9A-Z]{0,2})") { return 505; }
if ($query_string ~ "_REQUEST(=|\[|\%[0-9A-Z]{0,2})") { return 505; }
if ($query_string ~ "proc/self/environ") { return 505; }
if ($query_string ~ "mosConfig_[a-zA-Z_]{1,21}(=|\%3D)") { return 505; }
if ($query_string ~ "base64_(en|de)code\(.*\)") { return 505; }
if ($query_string ~ "[a-zA-Z0-9_]=http://") { return 506; }
if ($query_string ~ "[a-zA-Z0-9_]=(\.\.//?)+") { return 506; }
if ($query_string ~ "[a-zA-Z0-9_]=/([a-z0-9_.]//?)+") { return 506; }
if ($query_string ~ "b(ultram|unicauca|valium|viagra|vicodin|xanax|ypxaieo)b") { return 507; }
if ($query_string ~ "b(erections|hoodia|huronriveracres|impotence|levitra|libido)b") {return 507; }
if ($query_string ~ "b(ambien|bluespill|cialis|cocaine|ejaculation|erectile)b") { return 507; }
if ($query_string ~ "b(lipitor|phentermin|pro[sz]ac|sandyauer|tramadol|troyhamby)b") { return 507; }
#这里大家根据自己情况添加删减上述判断参数，cURL、wget这类的屏蔽有点儿极端了，但要“宁可错杀一千，不可放过一个”。
if ($http_user_agent ~* YisouSpider|ApacheBench|WebBench|Jmeter|JoeDog|Havij|GetRight|TurnitinBot|GrabNet|masscan|mail2000|github|wget|curl|Java|python) { return 508; }
#同上，大家根据自己站点实际情况来添加删减下面的屏蔽拦截参数。
if ($http_user_agent ~* "Go-Ahead-Got-It") { return 508; }
if ($http_user_agent ~* "GetWeb!") { return 508; }
if ($http_user_agent ~* "Go!Zilla") { return 508; }
if ($http_user_agent ~* "Download Demon") { return 508; }
if ($http_user_agent ~* "Indy Library") { return 508; }
if ($http_user_agent ~* "libwww-perl") { return 508; }
if ($http_user_agent ~* "Nmap Scripting Engine") { return 508; }
if ($http_user_agent ~* "~17ce.com") { return 508; }
if ($http_user_agent ~* "WebBench*") { return 508; }
if ($http_user_agent ~* "spider") { return 508; } #这个会影响国内某些搜索引擎爬虫，比如：搜狗
#拦截各恶意请求的UA，可以通过分析站点日志文件或者waf日志作为参考配置。
if ($http_referer ~* 17ce.com) { return 509; }
#拦截17ce.com站点测速节点的请求，所以明月一直都说这些测速网站的数据仅供参考不能当真的。
if ($http_referer ~* WebBench*") { return 509; }
#拦截WebBench或者类似压力测试工具，其他工具只需要更换名称即可。
```
