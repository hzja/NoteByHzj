Linux bat 
<a name="bcdiq"></a>
## bat的仓库地址
[bat的仓库地址](https://github.com/sharkdp/bat)<br />[https://github.com/sharkdp/bat](https://github.com/sharkdp/bat)
<a name="48ab146a"></a>
## 功能特性
<a name="wKemz"></a>
### 1、语法高亮显示
bat支持大量编程和markup语言的语法高亮显示。
<a name="e621856b"></a>
### 2、Git集成
bat与git沟通，显示关于修改的索引会在文件的左侧栏进行显示。
<a name="aab1957b"></a>
### 3、自动分页
当文件的输出对于屏幕来说太大的时候，bat 命令自动将自己的输出内容传输到 less 命令中，所以可以一页一页的查看输出内容。
<a name="tZs6J"></a>
## bat的使用
Bat可以直接在终端显示单个文件：
```bash
> bat README.md
```
一次显示多个文件：
```bash
> bat src/*.rs
```
从stdin读取，自动确定语法，通常通过诸如shebang来确定#!/bin/sh：
```bash
> curl -s https://sh.rustup.rs | bat
```
从标准输入中读取，明确指定语言：
```bash
> yaml2json .travis.yml | json_pp | bat -l json
```
高亮无法输出的字符：
```bash
> bat -A /etc/hosts
```
作为cat的替代品:
```bash
bat > note.md  # quickly create a new file
bat header.md content.md footer.md > document.md
bat -n main.rs  # show line numbers (only)
bat f - g  # output 'f', then stdin, then 'g'.
```
