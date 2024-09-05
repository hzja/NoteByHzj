Linux 进程
<a name="WBFlA"></a>
### 通过修改掉进程的pid达到隐藏的效果(小隐于用户)
```bash
$ target->pid = 0x7fffffff;
```
原理：

- task被创建的时候，根据其pid注册procfs目录结构。
- 展示procfs目录结构的时候，遍历task list以其pid作为key来查找procfs目录结构。
- 0x7fffffff(或者任何其它合理的值)根本没有注册过，当然无法显示。
