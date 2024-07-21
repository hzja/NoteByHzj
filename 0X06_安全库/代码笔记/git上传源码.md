```java
git init //把这个目录变成Git可以管理的仓库

git add README.md //文件添加到仓库

git add . //不但可以跟单一文件，还可以跟通配符，更可以跟目录。一个点就把当前目录下所有未追踪的文件全部add了 

git commit -m "first commit" //把文件提交到仓库 每次重交代码，使用这条命令就行　　
git remote add origin https://github.com/AuFeng111/linux_c2.git //关联远程仓库

git push -u origin master //把本地库的所有内容推送到远程库上
```
重新提交只需要<br />git commit -m "first commit" <br />git push -u origin master

如果修改了read.md文件的话，需要然后再重新提交<br />$ git add README.md

