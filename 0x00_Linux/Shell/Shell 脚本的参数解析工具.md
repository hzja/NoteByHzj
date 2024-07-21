Shell
<a name="xJKjM"></a>
### 1. 使用空格分隔
<a name="L8N29"></a>
#### 实际用法
```bash
./myscript.sh -e conf -s /etc -l /usr/lib /etc/hosts
```
<a name="lBCuv"></a>
#### 实现脚本
```shell
#!/bin/bash
POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
    -e|--extension)
      EXTENSION="$2"
      shift # past argument
      shift # past value
      ;;
    -s|--searchpath)
      SEARCHPATH="$2"
      shift # past argument
      shift # past value
      ;;
    -l|--lib)
      LIBPATH="$2"
      shift # past argument
      shift # past value
      ;;
    --default)
      DEFAULT=YES
      shift # past argument
      ;;
      *)
    POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
      ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters
echo FILE EXTENSION  = "${EXTENSION}"
echo SEARCH PATH     = "${SEARCHPATH}"
echo LIBRARY PATH    = "${LIBPATH}"
echo DEFAULT         = "${DEFAULT}"
echo "Number files in SEARCH PATH with EXTENSION:" $(ls -1 "${SEARCHPATH}"/*."${EXTENSION}" | wc -l)
if [[ -n $1 ]]; then
  echo "Last line of file specified as non-opt/last argument:"
  tail -1 "$1"
fi
```
<a name="oUdkx"></a>
### 2. 使用等号分隔
<a name="Hao6k"></a>
#### 实际用法
```bash
./myscript.sh -e=conf -s=/etc -l=/usr/lib /etc/hosts
```
<a name="w9ZHB"></a>
#### 实现脚本
```shell
#!/bin/bash
for key in "$@"; do
  case $key in
    -e=*|--extension=*)
      EXTENSION="${key#*=}"
      shift # past argument=value
      ;;
    -s=*|--searchpath=*)
      SEARCHPATH="${key#*=}"
      shift # past argument=value
      ;;
    -l=*|--lib=*)
      LIBPATH="${key#*=}"
      shift # past argument=value
      ;;
    --default)
      DEFAULT=YES
      shift # past argument with no value
      ;;
    *)
      ;;
  esac
done
echo "FILE EXTENSION  = ${EXTENSION}"
echo "SEARCH PATH     = ${SEARCHPATH}"
echo "LIBRARY PATH    = ${LIBPATH}"
echo "Number files in SEARCH PATH with EXTENSION:" $(ls -1 "${SEARCHPATH}"/*."${EXTENSION}" | wc -l)
if [[ -n $1 ]]; then
  echo "Last line of file specified as non-opt/last argument:"
  tail -1 $1
fi |
```
<a name="Td8uA"></a>
### 3. 使用 getopts 工具
<a name="Be6kp"></a>
#### 实际用法
```bash
./myscript.sh -h
./myscript.sh -v -f
```
<a name="S76Vy"></a>
#### 实现脚本
```shell
#!/bin/sh
# 重置以防止在前面的shell中使用getopts工具(这是一个POSIX变量)
OPTIND=1
# 初始化变量名称
OUTPUT_FILE=""
VERSION=0
# getopts的缺点就是它只能处理短选项，如-h，而不能是--help格式
while getopts "h?vf:" key; do
    case "$key" in
    h|\?)
        show_help
        exit 0
        ;;
    v)
        VERSION=1
        ;;
    f)
        output_file=$OPTARG
        ;;
    esac
done
shift $((OPTIND-1))
[ "${1:-}" = "--" ] && shift
echo "verbose=$VERSION, output_file='$output_file', Leftovers: $@" |
```
<a name="KdPOM"></a>
### 4. 使用 argbash 工具
这个工具主要提供脚本参数的解析功能，而且不再引用任何第三方库的情况下。一般会比普通脚本多`30`多行而且，但是效果非常好。<br />详细信息可以通过官方网站地址了解。<br />[https://argbash.io/generate#results](https://argbash.io/generate#results)
```shell
#!/bin/bash
# This is a rather minimal example Argbash potential
# Example taken from http://argbash.readthedocs.io/en/stable/example.html
# [可选参数]
# ARG_OPTIONAL_SINGLE([option], [o], [optional argument help msg])
# [可选布尔参数]
# ARG_OPTIONAL_BOOLEAN([print], , [boolean optional argument help msg])
# [固定参数]
# ARG_POSITIONAL_SINGLE([positional-arg], [positional argument help  msg], )
# [帮助信息]
# ARG_HELP([The general script's help msg])
# ARGBASH_GO
# [ <-- needed because of Argbash
echo "Value of --option: $_arg_option"
echo "print is $_arg_print"
echo "Value of positional-arg: $_arg_positional_arg"
# ] <-- needed because of Argbash |
```
