<a name="NWGqE"></a>
## 1、什么是动态编译
在 Java 中，动态编译是指在运行时动态地编译 Java 源代码，生成字节码，并加载到 JVM 中执行。动态编译可以用于实现动态代码生成、动态加载、插件化等功能。
<a name="qAhZK"></a>
### 1.1、动态编译的相关概念

- `JavaFileManager` 对象：用于管理编译过程中的文件。
   - `JavaFileManager` 是一个接口，提供了对 Java 文件的管理功能，包括创建、查找、读写等操作。`JavaFileManager` 有多种实现方式，例如 `StandardJavaFileManager`、`ForwardingJavaFileManager` 等。
- `DiagnosticListener` 对象：用于收集编译时的诊断信息。
   - `DiagnosticListener` 是一个接口，用于接收编译时的诊断信息，例如错误、警告等。
- `JavaFileObject` 对象：表示要编译的 Java 源代码。
   - `JavaFileObject` 是一个抽象类，用于表示 Java 源代码或字节码。`JavaFileObject` 有多种实现方式，例如 `SimpleJavaFileObject`、`JavaFileObjectWrapper` 等。
<a name="bWPPj"></a>
### 1.2、如何简单的实现动态编译

- 创建一个 `JavaCompiler` 对象，该对象用于编译 Java 源代码。
- 创建一个 `DiagnosticCollector` 对象，该对象用于收集编译时的诊断信息。
- 创建一个 `JavaFileManager` 对象，该对象用于管理编译过程中的文件。
- 创建一个 `JavaFileObject` 对象，该对象用于表示要编译的 Java 源代码。
- 调用 `JavaCompiler` 对象的 getTask 方法，传入 `JavaFileManager` 对象和 `DiagnosticCollector` 对象，获取一个 `CompilationTask` 对象。
- 调用 `CompilationTask` 对象的 call 方法，编译 Java 源代码。
- 获取 `DiagnosticCollector` 对象的诊断信息，并处理编译结果。

下面是一个简单的示例，演示如何使用动态编译：
```java
public class DynamicCompiler {
    public static void main(String[] args) throws Exception {
        // 创建 JavaCompiler 对象
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        // 创建 DiagnosticCollector 对象，用于收集编译时的诊断信息
        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<>();
        // 创建 JavaFileManager 对象，用于管理编译过程中的文件
        StandardJavaFileManager fileManager = compiler.getStandardFileManager(diagnostics, null, null);
        // 创建 JavaFileObject 对象，用于表示要编译的 Java 源代码
        String code = "public class HelloWorld { public static void main(String[] args) { System.out.println(\"Hello World!\"); } }";
        JavaFileObject source = new JavaSourceFromString("HelloWorld", code);
        // 获取 CompilationTask 对象
        Iterable<? extends JavaFileObject> compilationUnits = Arrays.asList(source);
        CompilationTask task = compiler.getTask(null, fileManager, diagnostics, null, null, compilationUnits);
        // 编译 Java 源代码
        boolean success = task.call();
        // 获取诊断信息
        List<Diagnostic<? extends JavaFileObject>> messages = diagnostics.getDiagnostics();
        for (Diagnostic<? extends JavaFileObject> message : messages) {
            System.out.println(message.getMessage(null));
        }
        // 处理编译结果
        if (success) {
            System.out.println("Compilation was successful.");
        } else {
            System.out.println("Compilation failed.");
        }
        fileManager.close();
    }
}

class JavaSourceFromString extends SimpleJavaFileObject {
    final String code;

    JavaSourceFromString(String name, String code) {
        super(URI.create("string:///" + name.replace('.', '/') + Kind.SOURCE.extension), Kind.SOURCE);
        this.code = code;
    }

    @Override
    public CharSequence getCharContent(boolean ignoreEncodingErrors) {
        return code;
    }
}
```
运行结果：
```
Hello World!
Compilation was successful.
```
<a name="jL6vG"></a>
## 2、如何结合 SpringBoot 项目使用
上面展示了如何简单使用 Java 的动态编译功能，但是在日常项目开发中，会面对更多的场景。结合前言中遇到的问题，简单的给大家介绍下在项目中是如何使用 Java 的动态编译功能来解决遇到的问题的。<br />当时的想法是这样的：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689212400757-b83e4e86-c5f6-40ee-ad8f-87ea36864fc0.png#averageHue=%23828282&clientId=u7f30545f-806f-4&from=paste&id=u35a7b87a&originHeight=282&originWidth=701&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua7b1ee92-0fe9-42d4-9404-add449b1a7f&title=)<br />这样，各个业务方就可以自己管理自己的代码块，与外部对接或者修改代码无需在发布应用！
<a name="wzzl7"></a>
### 2.1、动态编译在项目中遇到的问题
<a name="Z32iL"></a>
#### 2.1.1、必须重写类加载器新编译的代码才能生效
在 Java 中使用动态编译功能时，重写类加载器是必要的。这是因为动态编译生成的类需要加载到 JVM 中执行，而默认的类加载器无法加载动态生成的类。<br />在 Java 中，类加载器分为三种：启动类加载器、扩展类加载器和应用程序类加载器。默认情况下，Java 使用应用程序类加载器来加载类。应用程序类加载器只能加载预先编译好的类，无法加载动态生成的类。因此，需要重写类加载器，使其能够加载动态生成的类。<br />重写类加载器有两种方式：继承 `ClassLoader` 类或实现 `ClassLoader` 接口。一般情况下，建议使用继承 `ClassLoader` 类的方式，因为这样可以更方便地控制类加载的过程。<br />当重写类加载器时，需要实现 `findClass` 方法。`findClass` 方法用于查找指定名称的类。如果类已经被加载过，可以直接返回已加载的类；否则，需要使用动态编译生成类的字节码，并通过 `defineClass` 方法将其加载到 JVM 中执行。
<a name="bQvrw"></a>
#### 2.1.2、没有依赖的简单代码可以编译成功，但是一旦有依赖关系，编译就会失败
Java 编译器是通过 `JavaFileManager` 来加载相关依赖类的，如果不重写使用的是默认的 `JavaFileManager` 来获取 SpringBoot 的 jarFile 来读取嵌套 jar，自然是获取不到的，需要重写 `JavaFileManager`，去获取编译代码所需的依赖，具体写法详见 2.2 代码示例。
<a name="OBT4E"></a>
### 2.2、代码示例
```java
// 通过调用这个方法即可实现 java 的动态编译功能啦
public static Class compile(String className, String code) {
    try (MemoryClassLoader loader = MemoryClassLoader.genInstance()) {
        loader.registerJava(className, code);
        return MemoryClassLoader.getInstance().loadClass(className);
    } catch (Exception e) {
        // ignore
    }
}
```
```java
public class MemoryClassLoader extends URLClassLoader {

    private static final Map<String, byte[]> classBytes = new ConcurrentHashMap<>();

    private MemoryClassLoader() {
        super(new URL[0], MemoryClassLoader.class.getClassLoader());
    }

    private static final Map<String, MemoryClassLoader> CLASSLOADER_MAP = new ConcurrentHashMap<String, MemoryClassLoader>() {{
        put(KEY_CLASSLOADER, new MemoryClassLoader());
    }};

    private static final String KEY_CLASSLOADER = "key_classloader";

    /**
     * 注册 Java 字符串到内存类加载器中
     */
    public void registerJava(String className, String javaCode) {
        try {
            Map<String, byte[]> compile = compile(className, javaCode);
            if (null != compile) {
                classBytes.putAll(compile);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 编译 Java 代码
     */
    private static Map<String, byte[]> compile(String className, String javaCode) {
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        StandardJavaFileManager stdManager = getStandardFileManager(null, null, null);
        try (MemoryJavaFileManager manager = new MemoryJavaFileManager(stdManager)) {
            JavaFileObject javaFileObject = manager.makeStringSource(className, javaCode);
            JavaCompiler.CompilationTask task = compiler.getTask(null, manager, null, null, null, Collections.singletonList(javaFileObject));
            Boolean result = task.call();
            if (result != null && result) {
                return manager.getClassBytes();
            }
        }
        return null;
    }

    @Override
    public Class<?> findClass(String name) throws ClassNotFoundException {
        byte[] buf = classBytes.get(name);
        if (buf == null) {
            return super.findClass(name);
        }
        return defineClass(name, buf, 0, buf.length);
    }

    @Override
    public void close() {
        classBytes.clear();
        CLASSLOADER_MAP.clear();
    }

    /**
     * 自定义 Java 文件管理器
     */
    public static SpringJavaFileManager getStandardFileManager(DiagnosticListener<? super JavaFileObject> var1, Locale var2, Charset var3) {
        Context var4 = new Context();
        var4.put(Locale.class, var2);
        if (var1 != null) {
            var4.put(DiagnosticListener.class, var1);
        }
        PrintWriter var5 = var3 == null ? new PrintWriter(System.err, true) : new PrintWriter(new OutputStreamWriter(System.err, var3), true);
        var4.put(Log.outKey, var5);
        return new SpringJavaFileManager(var4, true, var3);
    }

    /**
     * 获取实例
     */
    public static MemoryClassLoader getInstance() {
        return CLASSLOADER_MAP.get(KEY_CLASSLOADER);
    }

    /**
     * 生成新的实例
     */
    public static MemoryClassLoader genInstance() {
        MemoryClassLoader classLoader = new MemoryClassLoader();
        CLASSLOADER_MAP.put(KEY_CLASSLOADER, new MemoryClassLoader());
        return classLoader;
    }

    public static String getPath() {
        ApplicationHome home = new ApplicationHome(MemoryJavaFileManager.class);
        String path = home.getSource().getPath();
        return path;
    }

    public static boolean isJar() {
        return getPath().endsWith(".jar");
    }

}
```
```java
class MemoryJavaFileManager extends ForwardingJavaFileManager<JavaFileManager> {

    // compiled classes in bytes:
    final Map<String, byte[]> classBytes = new HashMap<>();

    final Map<String, List<JavaFileObject>> classObjectPackageMap = new HashMap<>();

    private JavacFileManager javaFileManager;

    /**
     * key 包名 value javaobj 主要给 jdk 编译 class 的时候找依赖 class 用
     */
    public final static Map<String, List<JavaFileObject>> CLASS_OBJECT_PACKAGE_MAP = new HashMap<>();

    private static final Object lock = new Object();

    private boolean isInit = false;

    public void init() {
        try {
            String jarBaseFile = MemoryClassLoader.getPath();
            JarFile jarFile = new JarFile(new File(jarBaseFile));
            List<JarEntry> entries = jarFile.stream().filter(jarEntry -> jarEntry.getName().endsWith(".jar")).collect(Collectors.toList());
            JarFile libTempJarFile;
            List<JavaFileObject> onePackageJavaFiles;
            String packageName;
            for (JarEntry entry : entries) {
                libTempJarFile = jarFile.getNestedJarFile(jarFile.getEntry(entry.getName()));
                if (libTempJarFile.getName().contains("tools.jar")) {
                    continue;
                }
                Enumeration<JarEntry> tempEntriesEnum = libTempJarFile.entries();
                while (tempEntriesEnum.hasMoreElements()) {
                    JarEntry jarEntry = tempEntriesEnum.nextElement();
                    String classPath = jarEntry.getName().replace("/", ".");
                    if (!classPath.endsWith(".class") || jarEntry.getName().lastIndexOf("/") == -1) {
                        continue;
                    } else {
                        packageName = classPath.substring(0, jarEntry.getName().lastIndexOf("/"));
                        onePackageJavaFiles = CLASS_OBJECT_PACKAGE_MAP.containsKey(packageName) ? CLASS_OBJECT_PACKAGE_MAP.get(packageName) : new ArrayList<>();
                        onePackageJavaFiles.add(new MemorySpringBootInfoJavaClassObject(jarEntry.getName().replace("/", ".").replace(".class", ""),
                                new URL(libTempJarFile.getUrl(), jarEntry.getName()), javaFileManager));
                        CLASS_OBJECT_PACKAGE_MAP.put(packageName, onePackageJavaFiles);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        isInit = true;

    }

    MemoryJavaFileManager(JavaFileManager fileManager) {
        super(fileManager);
        this.javaFileManager = (JavacFileManager) fileManager;
    }

    public Map<String, byte[]> getClassBytes() {
        return new HashMap<>(this.classBytes);
    }

    @Override
    public void flush() {
    }

    @Override
    public void close() {
        classBytes.clear();
        classObjectPackageMap.clear();
        CLASS_OBJECT_PACKAGE_MAP.clear();
    }


    public List<JavaFileObject> getLibJarsOptions(String packgeName) {
        synchronized (lock) {
            if (!isInit) {
                init();
            }
        }
        return CLASS_OBJECT_PACKAGE_MAP.get(packgeName);
    }

    @Override
    public Iterable<JavaFileObject> list(Location location,String packageName, Set<JavaFileObject.Kind> kinds,
                                         boolean recurse) throws IOException {
        if ("CLASS_PATH".equals(location.getName()) && MemoryClassLoader.isJar()) {
            List<JavaFileObject> result = getLibJarsOptions(packageName);
            if (result != null) {
                return result;
            }
        }
        Iterable<JavaFileObject> it = super.list(location, packageName, kinds, recurse);
        if (kinds.contains(JavaFileObject.Kind.CLASS)) {
            final List<JavaFileObject> javaFileObjectList = classObjectPackageMap.get(packageName);
            if (javaFileObjectList != null) {
                if (it != null) {
                    for (JavaFileObject javaFileObject : it) {
                        javaFileObjectList.add(javaFileObject);
                    }
                }
                return javaFileObjectList;
            } else {
                return it;
            }
        } else {
            return it;
        }
    }

    @Override
    public String inferBinaryName(Location location, JavaFileObject file) {
        if (file instanceof MemoryInputJavaClassObject) {
            return ((MemoryInputJavaClassObject) file).inferBinaryName();
        }
        return super.inferBinaryName(location, file);
    }

    @Override
    public JavaFileObject getJavaFileForOutput(Location location, String className, JavaFileObject.Kind kind,
                                               FileObject sibling) throws IOException {
        if (kind == JavaFileObject.Kind.CLASS) {
            return new MemoryOutputJavaClassObject(className);
        } else {
            return super.getJavaFileForOutput(location, className, kind, sibling);
        }
    }

    JavaFileObject makeStringSource(String className, final String code) {
        String classPath = className.replace('.', '/') + JavaFileObject.Kind.SOURCE.extension;
        return new SimpleJavaFileObject(URI.create("string:///" + classPath), JavaFileObject.Kind.SOURCE) {
            @Override
            public CharBuffer getCharContent(boolean ignoreEncodingErrors) {
                return CharBuffer.wrap(code);
            }
        };
    }

    void makeBinaryClass(String className, final byte[] bs) {
        JavaFileObject javaFileObject = new MemoryInputJavaClassObject(className, bs);
        String packageName = "";
        int pos = className.lastIndexOf('.');
        if (pos > 0) {
            packageName = className.substring(0, pos);
        }
        List<JavaFileObject> javaFileObjectList = classObjectPackageMap.get(packageName);
        if (javaFileObjectList == null) {
            javaFileObjectList = new LinkedList<>();
            javaFileObjectList.add(javaFileObject);

            classObjectPackageMap.put(packageName, javaFileObjectList);
        } else {
            javaFileObjectList.add(javaFileObject);
        }
    }

    class MemoryInputJavaClassObject extends SimpleJavaFileObject {
        final String className;
        final byte[] bs;

        MemoryInputJavaClassObject(String className, byte[] bs) {
            super(URI.create("string:///" + className.replace('.', '/') + Kind.CLASS.extension), Kind.CLASS);
            this.className = className;
            this.bs = bs;
        }

        @Override
        public InputStream openInputStream() {
            return new ByteArrayInputStream(bs);
        }

        public String inferBinaryName() {
            return className;
        }
    }

    class MemoryOutputJavaClassObject extends SimpleJavaFileObject {
        final String className;

        MemoryOutputJavaClassObject(String className) {
            super(URI.create("string:///" + className.replace('.', '/') + Kind.CLASS.extension), Kind.CLASS);
            this.className = className;
        }
        @Override
        public OutputStream openOutputStream() {
            return new FilterOutputStream(new ByteArrayOutputStream()) {
                @Override
                public void close() throws IOException {
                    out.close();
                    ByteArrayOutputStream bos = (ByteArrayOutputStream) out;
                    byte[] bs = bos.toByteArray();
                    classBytes.put(className, bs);
                    makeBinaryClass(className, bs);
                }
            };
        }
    }
}
```
```java
class MemorySpringBootInfoJavaClassObject extends BaseFileObject {
    private final String className;
    private URL url;

    MemorySpringBootInfoJavaClassObject(String className, URL url, JavacFileManager javacFileManager) {
        super(javacFileManager);
        this.className = className;
        this.url = url;
    }

    @Override
    public Kind getKind() {
        return Kind.valueOf("CLASS");
    }

    @Override
    public URI toUri() {
        try {
            return url.toURI();
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public String getName() {
        return className;
    }

    @Override
    public InputStream openInputStream() {
        try {
            return url.openStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public OutputStream openOutputStream() throws IOException {
        return null;
    }

    @Override
    public CharSequence getCharContent(boolean ignoreEncodingErrors) throws IOException {
        return null;
    }

    @Override
    public Writer openWriter() throws IOException {
        return null;
    }
    @Override
    public long getLastModified() {
        return 0;
    }

    @Override
    public boolean delete() {
        return false;
    }

    @Override
    public String getShortName() {
        return className.substring(className.lastIndexOf("."));
    }
    @Override
    protected String inferBinaryName(Iterable<? extends File> iterable) {
        return className;
    }

    @Override
    public boolean equals(Object o) {
        return false;
    }

    @Override
    public int hashCode() {
        return 0;
    }

    @Override
    public boolean isNameCompatible(String simpleName, Kind kind) {
        return false;
    }
}
```
```java
// 自定义 springboot 的类加载器
class SpringJavaFileManager extends JavacFileManager {

    public SpringJavaFileManager(Context context, boolean b, Charset charset) {
        super(context, b, charset);
    }

    @Override
    public ClassLoader getClassLoader(Location location) {
        nullCheck(location);
        Iterable var2 = this.getLocation(location);
        if (var2 == null) {
            return null;
        } else {
            ListBuffer var3 = new ListBuffer();
            Iterator var4 = var2.iterator();

            while (var4.hasNext()) {
                File var5 = (File) var4.next();

                try {
                    var3.append(var5.toURI().toURL());
                } catch (MalformedURLException var7) {
                    throw new AssertionError(var7);
                }
            }
            return this.getClassLoader((URL[]) var3.toArray(new URL[var3.size()]));
        }
    }

    protected ClassLoader getClassLoader(URL[] var1) {
        ClassLoader var2 = this.getClass().getClassLoader();
        try {
            Class loaderClass = Class.forName("org.springframework.boot.loader.LaunchedURLClassLoader");
            Class[] var4 = new Class[]{URL[].class, ClassLoader.class};
            Constructor var5 = loaderClass.getConstructor(var4);
            return (ClassLoader) var5.newInstance(var1, var2);
        } catch (Throwable var6) {
        }
        return new URLClassLoader(var1, var2);
    }
}
```
<a name="AxcOg"></a>
## 总结
动态编译可能在日常工作中所使用的场景不多，但在特定的场景下能够很好的解决遇到的问题，当遇到类似场景时或许动态编译能够很好的解决它！
