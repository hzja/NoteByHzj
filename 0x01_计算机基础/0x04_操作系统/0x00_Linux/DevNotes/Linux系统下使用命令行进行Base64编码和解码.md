Linux Base64<br />编码是将数据转换为有效传输或存储所需格式的过程。相反，解码是将编码数据转换回其原始格式的过程。<br />Base64 是将二进制数据转换为 ASCII 的编码方式之一。通常，当二进制数据传输到无法正确处理二进制数据的系统（基于文本的系统）时会出现传输问题，比如信息在传输过程中的丢失或损坏。在这种情况下，Base64 编码就可以避免出现上述情况。<br />Base64 编码的一些用途是：<br />● 数据压缩<br />● 数据隐藏<br />● 以另一种格式传输数据<br />对于编码数据，Base64 只允许使用字母、数字和 = 符号。例如，c2FtcGxlCg== 是有效的编码数据，而 HV3.Zh2J==不是有效的编码数据。<br />如何使用 Base64 命令对字符串或文件中的数据进行编码和解码。
<a name="SdDpr"></a>
## Base64 语法
下面是使用 Base64 进行编码的语法：
```bash
$ base64 [OPTION] [FILE]
```
<a name="CV2yc"></a>
## Base64 选项
可以与 Base64 命令一起使用的一些命令行选项包括：
<a name="manFm"></a>
### 解码文件或字符串：
```bash
-d or --decode
```
<a name="FXClh"></a>
### 显示有关 Base64 用法的帮助信息：
```bash
--help
```
<a name="zVPdp"></a>
### 解码时使用下面选项可忽略非字母字符：
```bash
-i, --ignore-garbage
```
<a name="aobzz"></a>
### 显示版本信息：
```bash
--version
```
<a name="jYThH"></a>
## 编码字符串
可以使用 Base64 命令轻松地对字符串进行编码。例如，要将示例文本 “Welcome to Linux” 以 Base64 方式编码，可以使用如下命令：
```bash
$ echo "Welcome to Linux" | base64
```
此命令将使用 Base64 对字符串中的文本进行编码，并将编码的文本打印到标准输出，如下面的屏幕截图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620785070460-a5488db0-97d0-49ac-9dd9-89190dc6fe39.png#clientId=u81c08fa0-e639-4&from=paste&height=73&id=u2a5abf44&originHeight=219&originWidth=1893&originalType=binary&size=111379&status=done&style=shadow&taskId=u5678a276-51c4-41fb-952f-2f1c6f29980&width=631)<br />也可以使用重定向操作符（>）将编码输出保存到文件中，而不是打印到标准输出。以下命令将对文本进行编码并将输出保存到名为 “encodedfile.txt“  的文件中：
```bash
$ echo "Welcome to Linux" | base64 > encodedfile.txt
```
要查看编码文件，可以使用 cat 命令：
```bash
$ cat encodedfile.txt
```
<a name="zIQK3"></a>
## 解码字符串
也可以使用 –decode 或 -d 选项解码 Base64 编码的文本。例如，要解码 Base64 编码文本 “V2VsY29tZSB0byBMaW51eAo=” ，命令是：
```bash
 $ echo "V2VsY29tZSB0byBMaW51eAo=" | base64 --decode
```
这个命令将解码 Base64 中的编码文本，以及在标准输出中打印原始文本，如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620785092850-d3dbf6b6-1d1d-4229-bdf8-254da13f00ac.png#clientId=u81c08fa0-e639-4&from=paste&height=108&id=u74459d54&originHeight=325&originWidth=2427&originalType=binary&size=243849&status=done&style=none&taskId=ua43dd960-3b2e-4611-8207-8ff56e7cf36&width=809)<br />也可以使用重定向操作符（>）将解码后的输出保存到文件中，而不是打印到标准输出。以下命令可以解码编码文本并将原始文本保存到名为 “decodedfile.txt“ 的文件中：
```bash
 $ echo "V2VsY29tZSB0byBMaW51eAo=" | base64 –decode > decodedfile.txt
```
<a name="na98s"></a>
## 编码文本文件
Base64 命令也可用于对文本文件进行编码。如，对名为  “testfile.txt”  进行编码，则命令为：
```bash
 $ base64 testfile.txt
```
此命令将对指定的文本文件进行编码，并在标准输出上打印其编码形式，如下面的屏幕截图所示。<br />![2021-05-12-09-58-23-683266.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620785106194-ef5d4be7-6dd1-44f3-8001-446fbd5adf55.png#clientId=u81c08fa0-e639-4&from=ui&id=u659bb4f8&originHeight=154&originWidth=536&originalType=binary&size=3691&status=done&style=none&taskId=ud061d29d-1777-4b1f-9535-ae29183c654)<br />也可以使用重定向操作符（>）将解码后的输出保存到文件中，而不是打印到标准输出。使用以下命令可以解码编码文本并将原始文本保存到名为 “decodedfile.txt  的文件，这个文件可以使用 cat 命令查看。该命令是：
```bash
$ base64 -d encodedfile.txt > decodedfile.txt
```
<a name="nPwgL"></a>
## 编码用户输入
使用 Base64 编码，可以对任何用户提供的数据进行编码。为此，需要创建一个接受用户输入的脚本，使用 Base64 编码对其进行编码，并在标准输出上打印编码的数据.<br />使用以下代码创建脚本 “test.sh” ：
```bash
 #!/bin/bash
# Print message to ask for input
echo "Provide Some data to encode"
# Save the input to a variable named “data”
read data
# Encode using base64 encoding and save the output to a variable “encod_data”
encod_data=`echo -n $data | base64`
# Print encoded output
echo "Encoded text is : $encod_data"
```
![2021-05-12-09-58-24-181125.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620785119856-53d3dcaf-0412-477a-9d86-4de489c1a047.png#clientId=u81c08fa0-e639-4&from=ui&id=u913b3ca8&originHeight=228&originWidth=631&originalType=binary&size=63153&status=done&style=shadow&taskId=u55a955e8-814f-454b-824f-4190a2cd264)<br />按如下方式运行脚本：
```bash
 $ ./test.sh
```
运行脚本后，系统将要求输入要编码的数据。输入一些数据后并按 Enter 键，就可以在屏幕上接收编码输出了。<br />![2021-05-12-09-58-24-634354.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620785130720-871f7544-6812-4e41-92c9-56534d5fd8f3.jpeg#clientId=u81c08fa0-e639-4&from=ui&id=uf96551ef&originHeight=79&originWidth=383&originalType=binary&size=8237&status=done&style=none&taskId=ueac9d6fe-5033-474f-9499-df61dbf531c)
<a name="Jb2S9"></a>
## 验证用户密钥
现在看一个 Base64 解码的例子。将使用 Base64 解码来检查用户的有效性。为此，需要创建一个脚本，向用户请求密钥。该密钥将首先通过 Base64 解码进行解码，然后它将匹配输入密钥与预定义密钥。如果用户输入的密钥与预定义的密钥匹配，则会打印“您已输入有效密钥”消息，否则，在屏幕上打印的则是“您输入的密钥无效”。<br />使用以下代码创建脚本 “test1.sh” ：
```bash
#!/bin/bash
# Print message to ask for input
echo "Enter your key"
# Save the key provided by the user to a variable named "key"
read key
# Decode the encoded key (QWJjMTIzCg) and save the output to a variable named “orig_key” 
orig_key=`echo 'QWJjMTIzCg==' | base64 --decode`
# Compare the key entered by the user with the decoded key
if [ $key == $orig_key ]; then
#if key matches, print this:
echo "You have entered a valid key"
else
#if key does not match, print this:
echo "The key you have entered is not valid"
fi
```
![2021-05-12-09-58-25-118571.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620785148074-733c2d6e-a948-409d-aa41-7dfffddbad70.png#clientId=u81c08fa0-e639-4&from=ui&id=u16a02090&originHeight=243&originWidth=631&originalType=binary&size=60310&status=done&style=none&taskId=u1f355189-fc18-4021-9b97-fef81429834)<br />按如下方式运行脚本：
```bash
 $ ./test1.sh
```
运行脚本后，系统将要求提供密钥，键入密钥并按 Enter 键。如果输入的密钥与预定义的解码密钥匹配，您将收到“您已输入有效密钥”的消息，否则屏幕上将显示“您输入的密钥无效”的消息。<br />![2021-05-12-09-58-25-686384.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620785157261-c68aab9d-502b-4a36-8cb6-e98db957fcb6.jpeg#clientId=u81c08fa0-e639-4&from=ui&id=u015bcf10&originHeight=137&originWidth=348&originalType=binary&size=13469&status=done&style=none&taskId=u8f7eb648-473d-448a-bb7f-ea60d256c7a)<br />以上就是如何以命令行来使用 Base64 对字符串或文件进行编码和解码。结果可以打印在标准输出上，也可以保存在文件中。但是，值得注意的是，编码与加密不同，编码很容易泄露编码的数据，因此不建议使用编码传输敏感数据。
