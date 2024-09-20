Java 压缩 解压<br />Java对zip的压缩文件进行解压有两个开源的工具包，一个是Apache的ant工具包，另一个就是Java api自带的工具包；但是Java自带的工具包存在问题：如果压缩或者解压的文件存在非英文字符（比如中文、以色列文），在操作的过程中会存在问题：MALFORMAL Eception……<br />以下是通过Apache的zip工具包进行压缩和解压的代码（需要ant.jar）：
```java
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
 
import org.apache.tools.zip.ZipEntry;
import org.apache.tools.zip.ZipFile;
import org.apache.tools.zip.ZipOutputStream;
 
 
/**
 * 压缩或解压zip：
 * 由于直接使用java.util.zip工具包下的类，会出现中文乱码问题，所以使用ant.jar中的org.apache.tools.zip下的工具类
 */
 
public class ZipUtil {
     
    /**
     * 压缩文件或路径
     * @param zip 压缩的目的地址
     * @param srcFiles 压缩的源文件
     */
    public static void zipFile( String zip , List<File> srcFiles ){
        try {
            if( zip.endsWith(".zip") || zip.endsWith(".ZIP") ){
                ZipOutputStream _zipOut = new ZipOutputStream(new FileOutputStream(new File(zip))) ;
                _zipOut.setEncoding("GBK");
                for( File _f : srcFiles ){
                    handlerFile(zip , _zipOut , _f , "");
                }
                _zipOut.close();
            }else{
                System.out.println("target file[" + zip + "] is not .zip type file");
            }
        } catch (FileNotFoundException e) {
        } catch (IOException e) {
        }
    }
     
    /**
     *
     * @param zip 压缩的目的地址
     * @param zipOut
     * @param srcFile  被压缩的文件信息
     * @param path  在zip中的相对路径
     * @throws IOException
     */
    private static void handlerFile(String zip , ZipOutputStream zipOut , File srcFile , String path ) throws IOException{
        System.out.println(" begin to compression file[" + srcFile.getName() + "]");
        if( !"".equals(path) && ! path.endsWith(File.separator)){
            path += File.separator ;
        }
        if( ! srcFile.getPath().equals(zip) ){
            if( srcFile.isDirectory() ){
                File[] _files = srcFile.listFiles() ;
                if( _files.length == 0 ){
                    zipOut.putNextEntry(new ZipEntry( path + srcFile.getName() + File.separator));
                    zipOut.closeEntry();
                }else{
                    for( File _f : _files ){
                        handlerFile( zip ,zipOut , _f , path + srcFile.getName() );
                    }
                }
            }else{
                byte[] _byte = new byte[1024] ;
                InputStream _in = new FileInputStream(srcFile) ;
                zipOut.putNextEntry(new ZipEntry(path + srcFile.getName()));
                int len = 0 ;
                while( (len = _in.read(_byte)) > 0  ){
                    zipOut.write(_byte, 0, len);
                }
                _in.close();
                zipOut.closeEntry();
            }
        }
    }
 
    /**
     * 解压缩ZIP文件，将ZIP文件里的内容解压到targetDIR目录下
     * @param zipPath 待解压缩的ZIP文件名
     * @param descDir  目标目录
     */
    public static List<File> upzipFile(String zipPath, String descDir) {
        return upzipFile( new File(zipPath) , descDir ) ;
    }
     
    /**
     * 对.zip文件进行解压缩
     * @param zipFile  解压缩文件
     * @param descDir  压缩的目标地址，如：D:\\测试 或 /mnt/d/测试
     * @return
     */
    @SuppressWarnings("rawtypes")
    public static List<File> upzipFile(File zipFile, String descDir) {
        List<File> _list = new ArrayList<File>() ;
        try {
            ZipFile _zipFile = new ZipFile(zipFile , "GBK") ;
            for( Enumeration entries = _zipFile.getEntries() ; entries.hasMoreElements() ; ){
                ZipEntry entry = (ZipEntry)entries.nextElement() ;
                File _file = new File(descDir + File.separator + entry.getName()) ;
                if( entry.isDirectory() ){
                    _file.mkdirs() ;
                }else{
                    byte[] _byte = new byte[1024] ;
                    File _parent = _file.getParentFile() ;
                    if( !_parent.exists() ){
                        _parent.mkdirs() ;
                    }
                    InputStream _in = _zipFile.getInputStream(entry);
                    OutputStream _out = new FileOutputStream(_file) ;
                    int len = 0 ;
                    while( (len = _in.read(_byte)) > 0){
                        _out.write(_byte, 0, len);
                    }
                    _in.close();
                    _out.flush();
                    _out.close();
                    _list.add(_file) ;
                }
            }
        } catch (IOException e) {
        }
        return _list ;
    }
     
    /**
     * 对临时生成的文件夹和文件夹下的文件进行删除
     */
    public static void deletefile(String delpath) {
        try {
            File file = new File(delpath);
            if (!file.isDirectory()) {
                file.delete();
            } else if (file.isDirectory()) {
                String[] filelist = file.list();
                for (int i = 0; i < filelist.length; i++) {
                    File delfile = new File(delpath + File.separator + filelist[i]);
                    if (!delfile.isDirectory()) {
                        delfile.delete();
                    } else if (delfile.isDirectory()) {
                        deletefile(delpath + File.separator + filelist[i]);
                    }
                }
                file.delete();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
     
    public static void main(String[] args) {
        upzipFile("steven.zip","D://test");
    }
     
}
```
以下是Java api 中Java.util.zip包下面自带的压缩和解压缩的工具包实现的代码（如果文件名存在非英文编码会出现问题）：
```java
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
 
import org.apache.commons.io.IOUtils;
 
public class ZipTest {
 
     
    /**
     * Unzip it
     *
     * @param zipFile
     *            input zip file
     * @param output
     *            zip file output folder
     * @throws IOException
     */
    public static void unZipIt(String file, String outputFolder)
            throws IOException {
        ZipFile zipFile = new ZipFile(file);
        try {
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                File entryDestination = new File(outputFolder, entry.getName());
                if (entry.isDirectory()) {
                    entryDestination.mkdirs();
                } else {
                    entryDestination.getParentFile().mkdirs();
                    InputStream in = zipFile.getInputStream(entry);
                    OutputStream out = new FileOutputStream(entryDestination);
                    IOUtils.copy(in, out);
                    IOUtils.closeQuietly(in);
                    out.close();
                }
            }
        } finally {
            zipFile.close();
        }
    }
     
    public static void main(String[] args) throws Exception {
        unZipIt("test.zip","D://test");
    }
}
```

