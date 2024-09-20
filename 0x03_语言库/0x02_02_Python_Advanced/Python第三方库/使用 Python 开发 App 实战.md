Python<br />Python语言虽然很万能，但用它来开发app还是显得有点不对路，因此用Python开发的app应当是作为编码练习、或者自娱自乐所用，加上目前这方面的模块还不是特别成熟，bug比较多，总而言之，劝君莫轻入。

## 准备工作
利用Python开发app需要用到Python的一个模块–kivy，kivy是一个开源的，跨平台的Python开发框架，用于开发使用创新的应用程序。简而言之，这是一个Python桌面程序开发框架（类似wxpython等模块），强大的是kivy支持Linux、Mac、Windows、android、ios平台，这也是为什么开发app需要用到这个模块。<br />虽然kivy是跨平台的，但是想要在不同的平台使用Python代码，还需要将Python代码打包成对应平台的可执行程序，好在kivy项目下有个打包工具项目–buildozer，这是官方推荐的打包工具，因为相对比较简单，自动化程度高，其他项目比如：Python-for-android也能起到类似的作用，这里不展开介绍。
<a name="Rvzf8"></a>
## 搭建kivy开发环境
需要在pc上安装kivy开发环境，这里演示下Mac与Linux下的安装过程。
```bash
install kivy for mac
```
安装一些依赖包：
```bash
brew install pkg-config sdl2 sdl2_image sdl2_ttf sdl2_mixer gstreamer
```
安装cython以及kivy：
```bash
pip install cython==0.25
pip install kivy
```
如果安装kivy报错，则使用下面的方式安装kivy：
```bash
git clone https://github.com/kivy/kivy
python setup.py install
```
安装后测试：
```
$ python
Python 2.7.10 (default, Jul 15 2017, 17:16:57)
[GCC 4.2.1 Compatible Apple LLVM 9.0.0 (clang-900.0.31)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>>
>>> import kivy
[INFO   ] [Logger      ] Record log in /Users/didi/.kivy/logs/kivy_18-05-08_4.txt
[INFO   ] [Kivy        ] v1.10.1.dev0, git-5f6c66e, 20180507
[INFO   ] [Python      ] v2.7.10 (default, Jul 15 2017, 17:16:57)
[GCC 4.2.1 Compatible Apple LLVM 9.0.0 (clang-900.0.31)]
```
说明：导入kivy模块没有报错则说明安装成功。
```bash
install kivy for centos7
```
先安装依赖：
```bash
yum install \
make \
mercurial \
automake \
gcc \
gcc-c++ \
SDL_ttf-devel \
SDL_mixer-devel \
khrplatform-devel \
mesa-libGLES \
mesa-libGLES-devel \
gstreamer-plugins-good \
gstreamer \
gstreamer-python \
mtdev-devel \
python-devel \
python-pip \
java-devel
```
安装cython以及kivy:
```
pip install Cython==0.20
pip install kivy
```
CentOS安装kivy参考：[https://kivy.org/docs/installation/installation-linux.html#using-software-packages](https://kivy.org/docs/installation/installation-linux.html#using-software-packages)<br />说明：其他安装kivy方式可移步：[https://kivy.org/#download](https://kivy.org/#download)
<a name="CbcTr"></a>
## 用kivy开发第一个Python app
安装完kivy就可以开发app程序了，这里演示下hello-world程序，关于kivy更复杂的用法不是本文重点，后面再成文介绍。<br />**创建一个main.py文件，写入：**
```python
#! -*- coding:utf-8 -*-
from kivy.app import App
class HelloApp(App):
    pass
if __name__ == '__main__':
    HelloApp().run()
```
**创建一个hello.kv文件，写入：**
```
Label:
text: 'Hello, World! I am nMask'
```
简单说明：main.py是入口函数，定义了一个HelloApp类，该类继承kivy.app；hello.kv文件是kivy程序，相当于定义界面风格等，该文件命名规则为类名小写且去除app。
<a name="LhV3A"></a>
## 运行第一个Python app
```bash
python main.py
```
运行结果：<br />![](./img/1691026581340-042889d7-9546-4c4f-929b-04120fbab406.jpeg)
<a name="l1F9u"></a>
## 安装buildozer工具
通过以上的编码，创建了自己的第一个python app程序，该程序可以直接在Mac、Linux、Windows平台下运行，那么如何让它在安卓或者苹果手机上运行呢？在安卓上运行，需要将其打包成apk安装程序，因此就需要用到前面提到过的buildozer工具，（buildozer工具可以打包kivy程序，支持android、ios等），buildozer的安装过程比较简单：
```bash
pip install buildozer
```
<a name="Wo9GF"></a>
## 使用buildozer工具将kivy程序打包成apk
在Python项目目录下运行：
```bash
buildozer init
```
运行成功将会创建一个配置文件buildozer.spec，可以通过修改配置文件更改app的名称等，然后运行：
```bash
buildozer android debug deploy run
```
运行以上命令将会生成跨平台的安装包，可适用安卓、ios等，如果用于安卓，则是利用python-for-android项目。<br />在第一次运行以上命令的时候，会自动在系统中下载安卓sdk等必要文件，如下图。<br />![](./img/1691026581374-e56c6ccd-35f6-4e94-be5a-8b714bf9b7ec.png)<br />说明：这里只演示打包成apk文件，iso平台的可自行研究，参考文档：[https://github.com/kivy/buildozer](https://github.com/kivy/buildozer)。
<a name="vbVDV"></a>
## Python apk程序测试
如果以上步骤都运行成功的话，应该会在项目目录下的bin目录下生成一个apk文件，类似如下：<br />![](./img/1691026581359-0bc952ee-8a97-4703-911e-5737c6515cf4.jpeg)<br />然后将apk下载到安卓系统的手机上，安装即可，测试效果如下：<br />![](./img/1691026581334-309ade38-79a9-44d0-b446-0810b3bc6c6d.jpeg)<br />打开app<br />![](./img/1691026581326-b1a270f5-a8fd-4bfc-9130-00f630b769ee.jpeg)
<a name="s1SSg"></a>
## buildozer使用说明
```
Usage:
buildozer [--profile <name>] [--verbose] [target] <command>...
buildozer --version
Available targets:
android        Android target, based on python-for-android project
ios            iOS target, based on kivy-ios project
android_old    Android target, based on python-for-android project (old toolchain)
Global commands (without target):
distclean          Clean the whole Buildozer environment.
help               Show the Buildozer help.
init               Create a initial buildozer.spec in the current directory
serve              Serve the bin directory via SimpleHTTPServer
setdefault         Set the default command to run when no arguments are given
version            Show the Buildozer version
Target commands:
clean      Clean the target environment
update     Update the target dependencies
debug      Build the application in debug mode
release    Build the application in release mode
deploy     Deploy the application on the device
run        Run the application on the device
serve      Serve the bin directory via SimpleHTTPServer
Target "android_old" commands:
adb                Run adb from the Android SDK. Args must come after --, or
use --alias to make an alias
logcat             Show the log from the device
Target "ios" commands:
list_identities    List the available identities to use for signing.
xcode              Open the xcode project.
Target "android" commands:
adb                Run adb from the Android SDK. Args must come after --, or
use --alias to make an alias
logcat             Show the log from the device
p4a                Run p4a commands. Args must come after --, or use --alias
to make an alias
```
<a name="i8OML"></a>
## buildozer打包过程中的坑点
如果在打包过程中遇到报错，可以修改buildozer.spec配置文件中的log_level为2，然后重新运行，可以看具体的错误信息。
<a name="hu2zP"></a>
### 报错：You might have missed to install 32bits libs
这个错是在CentOS7上运行时报的错，大意是系统缺少了某些32位的依赖文件。<br />解决方案：
```bash
yum -y install --skip-broken glibc.i686 arts.i686 audiofile.i686 bzip2-libs.i686 cairo.i686 cyrus-sasl-lib.i686 dbus-libs.i686 directfb.i686 esound-libs.i686 fltk.i686 freeglut.i686 gtk2.i686 hal-libs.i686 imlib.i686 lcms-libs.i686 lesstif.i686 libacl.i686 libao.i686 libattr.i686 libcap.i686 libdrm.i686 libexif.i686 libgnomecanvas.i686 libICE.i686 libieee1284.i686 libsigc++20.i686 libSM.i686 libtool-ltdl.i686 libusb.i686 libwmf.i686 libwmf-lite.i686 libX11.i686 libXau.i686 libXaw.i686 libXcomposite.i686 libXdamage.i686 libXdmcp.i686 libXext.i686 libXfixes.i686 libxkbfile.i686 libxml2.i686 libXmu.i686 libXp.i686 libXpm.i686 libXScrnSaver.i686 libxslt.i686 libXt.i686 libXtst.i686 libXv.i686 libXxf86vm.i686 lzo.i686 mesa-libGL.i686 mesa-libGLU.i686 nas-libs.i686 nss_ldap.i686 cdk.i686 openldap.i686 pam.i686 popt.i686 pulseaudio-libs.i686 sane-backends-libs-gphoto2.i686 sane-backends-libs.i686 SDL.i686 svgalib.i686 unixODBC.i686 zlib.i686 compat-expat1.i686 compat-libstdc++-33.i686 openal-soft.i686 alsa-oss-libs.i686 redhat-lsb.i686 alsa-plugins-pulseaudio.i686 alsa-plugins-oss.i686 alsa-lib.i686 nspluginwrapper.i686 libXv.i686 libXScrnSaver.i686 qt.i686 qt-x11.i686 pulseaudio-libs.i686 pulseaudio-libs-glib2.i686 alsa-plugins-pulseaudio.i686 python-matplotli
```
参考：[https://ask.fedoraproject.org/en/question/9556/how-do-i-install-32bit-libraries-on-a-64-bit-fedora/](https://ask.fedoraproject.org/en/question/9556/how-do-i-install-32bit-libraries-on-a-64-bit-fedora/)
<a name="jsBNd"></a>
### 报错：Error compiling Cython file
错误大意为cython文件出错，可能是cython模块没有安装，或者版本有问题。解决方案：
```bash
pip install cython==0.25
```
报错：IOError: [Errno 2] No such file or directory….. 这是在打包的最后一步，将apk文件copy到项目bin目录下时报的错，是buildozer的一个bug。<br />解决方案：<br />修改/usr/local/lib/python2.7/dist-packages/buildozer/tagets/android.py文件：<br />**在文件开头导入:**<br />from distutils.version import LooseVersion<br />**将786行:XXX found how the apk name is really built from the title这一行以下的代码替换为：**
```
__sdk_dir = self.android_sdk_dir
build_tools_versions = os.listdir(join(__sdk_dir, 'build-tools'))
build_tools_versions = sorted(build_tools_versions, key=LooseVersion)
build_tools_version = build_tools_versions[-1]
gradle_files = ["build.gradle", "gradle", "gradlew"]
is_gradle_build = any((exists(join(dist_dir, x)) for x in gradle_files)) and build_tools_version >= ’25.0'
```
<a name="rUuaI"></a>
## buildozer虚拟机
kivy官方推出了一个buildozer虚拟机镜像，已经安装好了buildozer以及一些依赖文件，为buildozer打包测试提供平台。由于之前在Mac上利用buildozer打包一直报错，后来换成CentOS也依然没有成功，因此便下载了此虚拟机，测试效果如下：<br />![](./img/1691026581962-98291749-3eb9-4edd-a887-a91f5b0d9370.jpeg)<br />虚拟机下载地址：[http://txzone.net/files/torrents/kivy-buildozer-vm-2.0.zip](http://txzone.net/files/torrents/kivy-buildozer-vm-2.0.zip)<br />说明：对于无法解决依赖问题的朋友，可以使用此虚拟机进行程序打包，开发环境还是推荐用自己的本机。
<a name="nnNoJ"></a>
## kivy开发实例
因为本文重点在于介绍如何利用kivy+buildozer开发一款Python app，因此对于kivy的开发过程，以及app功能进行了最简化。
