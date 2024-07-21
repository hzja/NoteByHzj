LinuxOpenssl
<a name="PvkuL"></a>
## 1、创建 `openssl` 配置文件 openss.cnf
<a name="NO2Cy"></a>
### 创建目录
```bash
mkdir certs
cd certs
```
<a name="syxBc"></a>
### 创建 `openssl` 配置文件 openss.cnf
```
[req_distinguished_name]
countryName = CN
countryName_default = CN
stateOrProvinceName = GuangDong
stateOrProvinceName_default = GuangDong
localityName = ShenZhen
localityName_default = ShenZhen
organizationUnitName = olive
organizationUnitName_default = olive
commonName = mynexus3.com
commonName_default = mynexus3.com
commonName_max = 64

[req]
req_extensions = v3_req
distinguished_name = req_distinguished_name

[v3_req]
basicConstraints = CA:FALSE
keyUsage = critical,digitalSignature,keyAgreement
subjectAltName = @alt_names

[alt_names]
DNS.1 = mynexus3.com
DNS.2 = *.mynexus3.com
```
<a name="irKxC"></a>
### 解析

- Country Name (2 letter code) [XX]：CN
- State or Province Name (full name) []：省份拼音，比如 GuangDong
- Locality Name (eg, city) [Default City]：地市拼音，比如 ShenZhen
- Organization Name (eg, company) [Default Company Ltd]：公司名称拼音，比如 BaiDu
- Organizational Unit Name (eg, section) []：所在公司的部门拼音，比如 YanFa
- Common Name (eg, your name or your server's hostname) []：域名或者ip，比如 192.168.12.10
- Email Address []：邮箱地址，比如 xxxxx@sina.com
<a name="rGwRH"></a>
## 2、生成证书私钥
```bash
openssl genrsa -out ca.key 2048
```
参数说明：

- `**genras**` 使用 rsa 算法生成密钥
- `**-des3**` （可选）加密密钥，此时需要设置密码，后续使用该密钥时需要验证密码才能使用
- `**-out**` 生成私钥文件
<a name="C11Y2"></a>
## 3、生成证书请求文件(CSR)
**CSR** 是 Certificate Signing Request 的缩写，即**证书签名请求**，这不是证书，只是包含申请证书的基本信息。生成证书时要把这个提交给权威的证书颁发机构，颁发机构审核通过之后，再根据这些申请信息生成相应的证书。
```bash
openssl req -new -out ca.csr -key ca.key -config openssl.cnf
```
参数说明：

- `**req**` 产生证书签发申请命令
- `**-new**` 新的申请
- `**-key**` 输入的 key 文件，即私钥
- `**-out**` 输出为 CSR 文件，即证书请求文件
<a name="hfoO7"></a>
## 4、生成证书
**CRT** 是 certificate 的缩写，即证书
```bash
openssl x509 -req -days 3650 -in ca.csr -signkey ca.key -out ca.crt -extensions v3_req -extfile openssl.cnf
```
**X.509** 是一种证书格式。对 X.509 证书来说，认证者总是CA或由CA指定的人，一份 X.509 证书是一些标准字段的集合，这些字段包含有关用户或设备及其相应公钥的信息。X.509 的证书文件，一般以.crt结尾，根据该文件的内容编码格式，可以分为以下二种格式：

- **PEM**（Privacy Enhanced Mail），打开看文本格式，以"-----BEGIN…"开头，"-----END…"结尾，内容是 BASE64 编码。Apache 和 *NIX 服务器偏向于使用这种编码格式。
- **DER**（Distinguished Encoding Rules），打开看是二进制格式，不可读。Java 和 Windows 服务器偏向于使用这种编码格式。
