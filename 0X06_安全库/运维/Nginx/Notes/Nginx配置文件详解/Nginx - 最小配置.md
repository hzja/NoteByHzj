运维 Nginx<br />安全服务器是只允许所需数量的服务器。理想情况下，将通过单独启用其他功能来基于最小系统构建服务器。进行最少的配置也有助于调试。如果该错误在最小系统中不可用，则分别添加功能，然后继续搜索错误。<br />这是运行nginx所需的最低配置:
```nginx
# /etc/nginx/nginx.confevents {}         # event context have to be defined to consider config validhttp {
server {
  listen 80;
  server_name  javatpoint.co  www.javatpoint.co  *.javatpoint.co;

  return 200 "Hello";
}
```
<a name="poZhC"></a>
## `Root`,`Location`和`try_files`指令
<a name="KhVX9"></a>
### Root 指令
`root`指令用于设置请求的根目录，从而允许nginx将传入的请求映射到文件系统上。
```nginx
server {
  listen 80;
  server_name javatpoint.co;
  root /var/www/javatpoint.co;
}
```
它允许nginx根据请求返回服务器内容:
```nginx
javatpoint.co:80/index.html     # returns /var/www/learnfk.com/index.html
javatpoint.co:80/foo/index.html # returns /var/www/learnfk.com/foo/index.html
```
<a name="zg34R"></a>
### `Location`指令
`location`指令用于根据请求的URI(统一资源标识符)来设置配置。<br />语法为:
```nginx
location [modifier] path
```
示例:
```nginx
location /foo {
  # ...
}
```
如果未指定修饰符，则将路径视为前缀，之后可以跟随任何内容。上面的示例将匹配:
```nginx
/foo
/fooo
/foo123
/foo/bar/index.html
...
```
还可以在给定的上下文中使用多个`location`指令:
```nginx
server {
  listen 80;
  server_name javatpoint.co;
  root /var/www/javatpoint.co;

  location/{
    return 200 "root";
  }

  location /foo {
    return 200 "foo";
  }
}
javatpoint.co:80  /      # => "root"
javatpoint.co:80   /foo    # => "foo"
javatpoint.co:80   /foo123 # => "foo"
javatpoint.co:80   /bar    # => "root"
```
Nginx还提供了一些可以与 `location` 指令结合使用的修饰符。<br />修饰符已分配优先级:
```nginx
=           - Exact match
^~          - Preferential match
~ && ~*     - Regex match
no modifier - Prefix match
```
首先，nginx将检查所有精确匹配项。如果不存在，它将寻找优先选项。如果此匹配也失败，则将按其出现顺序测试正则表达式匹配。如果其他所有操作均失败，则将使用最后一个前缀匹配。
```nginx
location /match {
  return 200 'Prefix match: will match everything that starting with /match';
}

location ~* /match[0-9] {
  return 200 'Case insensitive regex match';
}

location ~ /MATCH[0-9] {
  return 200 'Case sensitive regex match';
}

location ^~ /match0 {
  return 200 'Preferential match';
}

location = /match {
  return 200 'Exact match';
}
/match     # => 'Exact match'
/match0    # => 'Preferential match'
/match1    # => 'Case insensitive regex match'
/MATCH1    # => 'Case sensitive regex match'
/match-abc # => 'Prefix match: matches everything that starting with /match'
```
<a name="jGmV2"></a>
### `try_files`指令
该指令尝试不同的路径，并返回找到的任何路径。
```nginx
try_files $uri index.html =404;
```
因此，/foo.html将尝试按以下顺序返回文件:
```nginx
$uri(/foo.html);

index.html
```
如果未找到:404<br />如果在服务器上下文中定义`try_files`，然后定义查找所有请求的位置，则不会执行`try_files`。发生这种情况是因为服务器上下文中的`try_files`定义了其伪位置，该伪位置是可能的最低特定位置。因此，定义`location/` 会比伪位置更具体。
```nginx
server {
  try_files $uri /index.html =404;

  location/{
  }
}
```
因此，应该避免在服务器上下文中使用`try_files`:
```nginx
server {
  location/{
    try_files $uri /index.html =404;
  }
}
```
