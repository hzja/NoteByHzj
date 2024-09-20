Gradle<br />Gradle依赖的粒度控制相较于Maven也更加精细，maven只有compile、provided、test、runtime四种scope，而Gradle有以下几种scope：

1. implementation，默认的scope。implementation的作用域会让依赖在编译和运行时均包含在内，但是不会暴露在类库使用者的编译时。举例，如果类库包含了gson，那么其他人使用类库时，编译时不会出现gson的依赖。
2. api，和implementation类似，都是编译和运行时都可见的依赖。但是api允许将自己类库的依赖暴露给类库的使用者。
3. compileOnly和runtimeOnly，这两种顾名思义，一种只在编译时可见，一种只在运行时可见。而runtimeOnly和Maven的provided比较接近。
4. testImplementation，这种依赖在测试编译时和运行时可见，类似于Maven的test作用域。
5. testCompileOnly和testRuntimeOnly，这两种类似于compileOnly和runtimeOnly，但是作用于测试编译时和运行时。

通过简短精悍的依赖配置和多种多样的作用与选择，Gradle可以提供比Maven更加优秀的依赖管理功能。
