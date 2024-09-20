Java 
<a name="CWO1d"></a>
### `org.apache.commons.io.IOUtils`
```java
closeQuietly()  
toString()  
copy()  
toByteArray()  
write()  
toInputStream()  
readLines()  
copyLarge()  
lineIterator()  
readFully()  
```
<a name="WitXG"></a>
### `org.apache.commons.io.FileUtils`
```java
deleteDirectory()  
readFileToString()  
deleteQuietly()  
copyFile()  
writeStringToFile()  
forceMkdir()  
write()  
listFiles()  
copyDirectory()  
forceDelete()  
```
<a name="enDLx"></a>
### `org.apache.commons.lang.StringUtils`
```java
isBlank()  
isNotBlank()  
isEmpty()  
isNotEmpty()  
equals()  
join()  
split()  
EMPTY  
trimToNull()  
replace()  
```
| 方法 | 说明 |
| --- | --- |
| `StringUtils.isEmpty` | 判断字符串是否为空 |
| `StringUtils.isNotEmpty` | 判断字符串是否非空 |
| `StringUtils.isBlank` | 判断字符串是否为空，为null或者`size==0`或者只存在空白字符(如"")，则返回true |
| `StringUtils.isNotBlank` | 判断字符串是否为非空 |

```java
//StringUtils.isEmpty
System.out.println(StringUtils.isEmpty(""));  //true
System.out.println(StringUtils.isEmpty("  "));  //false
//StringUtils.isNotEmpty
System.out.println(StringUtils.isNotEmpty(""));  //false
        
//StringUtils.isBlank
System.out.println(StringUtils.isBlank(""));  //true
System.out.println(StringUtils.isBlank(" "));  //true
//StringUtils.isNotBlank
System.out.println(StringUtils.isNotBlank(" "));  //false
```
<a name="V7npO"></a>
### `org.apache.http.util.EntityUtils`
```java
toString()  
consume()  
toByteArray()  
consumeQuietly()  
getContentCharSet()  
```
<a name="ub1ty"></a>
### `org.apache.commons.lang3.StringUtils`
```java
isBlank()  
isNotBlank()  
isEmpty()  
isNotEmpty()  
join()  
equals()  
split()  
EMPTY  
replace()  
capitalize()  
```
<a name="EMBUG"></a>
### `org.apache.commons.io.FilenameUtils`
```java
getExtension()  
getBaseName()  
getName()  
concat()  
removeExtension()  
normalize()  
wildcardMatch()  
separatorsToUnix()  
getFullPath()  
isExtension()  
```
<a name="Qnhd9"></a>
### `org.springframework.util.StringUtils`
```java
hasText()  
hasLength()  
isEmpty()  
commaDelimitedListToStringArray()  
collectionToDelimitedString()  
replace()  
delimitedListToStringArray()  
uncapitalize()  
collectionToCommaDelimitedString()  
tokenizeToStringArray()  
```
<a name="tP8gp"></a>
### `org.apache.commons.lang.ArrayUtils`
```java
contains()  
addAll()  
clone()  
isEmpty()  
add()  
EMPTY_BYTE_ARRAY  
subarray()  
indexOf()  
isEquals()  
toObject()  
```
<a name="dAOk8"></a>
### `org.apache.commons.lang.StringEscapeUtils`
```java
escapeHtml()  
unescapeHtml()  
escapeXml()  
escapeSql()  
unescapeJava()  
escapeJava()  
escapeJavaScript()  
unescapeXml()  
unescapeJavaScript()  
```
<a name="CKEBD"></a>
### `org.apache.http.client.utils.URLEncodedUtils`
```java
format()  
parse()  
```
<a name="SSd4M"></a>
### `org.apache.commons.codec.digest.DigestUtils`
```java
md5Hex()  
shaHex()  
sha256Hex()  
sha1Hex()  
sha()  
md5()  
sha512Hex()  
sha1()  
```
<a name="YJMPk"></a>
### `org.apache.commons.collections.CollectionUtils`
```java
isEmpty()  
isNotEmpty()  
select()  
transform()  
filter()  
find()  
collect()  
forAllDo()  
addAll()  
isEqualCollection()  
```
| 方法 | 说明 |
| --- | --- |
| `CollectionUtils.isEmpty` | 判断集合是否为空，为null或者`size==0`，返回true |
| `CollectionUtils.isNotEmpty` | 判断集合是否为非空 |

```java
List<Integer> emptyList = new ArrayList<>();
List<Integer> nullList = null;
List<Integer> notEmptyList = new ArrayList<>();
notEmptyList.add(1);

//CollectionUtils.isEmpty
System.out.println(CollectionUtils.isEmpty(emptyList));   //true
System.out.println(CollectionUtils.isEmpty(nullList));   //true
System.out.println(CollectionUtils.isEmpty(notEmptyList));   //false

//CollectionUtils.isNotEmpty
System.out.println(CollectionUtils.isNotEmpty(emptyList));   //false
System.out.println(CollectionUtils.isNotEmpty(nullList));   //false
System.out.println(CollectionUtils.isNotEmpty(notEmptyList));   //true
```
<a name="eqR6t"></a>
### `org.apache.commons.lang3.ArrayUtils`
```java
contains()  
isEmpty()  
isNotEmpty()  
add()  
clone()  
addAll()  
subarray()  
indexOf()  
EMPTY_OBJECT_ARRAY  
EMPTY_STRING_ARRAY  
```
<a name="fsDlX"></a>
### `org.apache.commons.beanutils.PropertyUtils`
```java
getProperty()  
setProperty()  
getPropertyDescriptors()  
isReadable()  
copyProperties()  
getPropertyDescriptor()  
getSimpleProperty()  
isWriteable()  
setSimpleProperty()  
getPropertyType()  
```
<a name="OzuGl"></a>
### `org.apache.commons.lang3.StringEscapeUtils`
```java
unescapeHtml4()  
escapeHtml4()  
escapeXml()  
unescapeXml()  
escapeJava()  
escapeEcmaScript()  
unescapeJava()  
escapeJson()  
escapeXml10()  
```
<a name="jFdjb"></a>
### `org.apache.commons.beanutils.BeanUtils`
```java
copyProperties()  
getProperty()  
setProperty()  
describe()  
populate()  
copyProperty()  
cloneBean()  
```
