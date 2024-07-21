import os
import re
import requests

# 处理markdown文件
def dealfile(filename):
    print(f"正在打开{filename}")
    file = open(filename, "r", encoding="utf-8")
    lines = file.readlines()
    file.close()

    urls = []
    filecontent = ""
    for line in lines:
        filecontent = filecontent + line

    # 匹配所有的图片连接
    urls = re.findall(r"https://cdn.nlark.com/yuque/[%&=\#\w./-]+", filecontent)
    
    # 创建img文件夹保存图片
    print("正在创建img文件夹--------------------")
    if os.path.exists("img"):
        pass
    else:
        os.mkdir("img")
    print("img文件夹创建完成--------------------")
    print("正在切换到img文件夹--------------------")
    os.chdir("./img")

    # 下载图片并保存
    for url in urls:
        try:
            print("图像的网址:", url)
            print("图像正在下载中...")
            img = requests.get(url)
            print("图像下载完成-------------------------")
            prename = url.split("/")[-1]
            imgname = prename.split("#")[0]
            print(f"正在写入{imgname}.......", imgname)
            with open(f"{imgname}", "wb") as file:
                file.write(img.content)
            print(f"{imgname}写入完成-------------------------")
            file_path = f"./img/{imgname}"
            filecontent = filecontent.replace(url, file_path, 1)
            print("-----------------------------------------------------")
        except:
            print("图像写入失败-------------------------------------")
            print("-----------------------------------------------------")

    # 替换所有的图片地址
    print("正在切换到上一级目录--------------------")
    os.chdir("..")
    file = open(filename, "w", encoding="utf-8")
    file.write(filecontent)
    file.close()
    print("所有图像的网址已替换完成----------------")

# 处理文件夹
def dealdir(dirname):
    os.chdir(f'./{dirname}')
    list = os.scandir()
    filelist = []
    dirlist = []
    for i in list:
        if i.is_file():
            filelist.append(i)
        if i.is_dir():
            dirlist.append(i)

    # 处理匹配到的每个文件
    for file in filelist:
        if file.name.split(".")[-1] == "md":
            if file.name == "README.md":
                break
            print("正在处理文件:", file.name)
            dealfile(file.name)
        else:
            print(file.name,"文件不是.md文件,不处理")

    # 处理匹配到的每个文件夹
    for dir in dirlist:
        if dir.name != "img":
            print("正在处理文件夹:", dir.name)
            dealdir(dir.name)
        else:
            print("文件夹是img文件夹,不处理")
    os.chdir("..")

list = os.scandir(path='.')
filelist = []
dirlist = []
for i in list:
    if i.is_file():
        filelist.append(i)
    if i.is_dir():
        dirlist.append(i)

for file in filelist:
    if file.name.split(".")[-1] == "md":
        print("正在处理文件:", file.name)
        dealfile(file.name)
    else:
        print("文件不是.md文件,不处理")

for dir in dirlist:
    if dir.name != "img":
        print("正在处理文件夹:", dir.name)
        dealdir(dir.name)
    else:
        print("文件夹是img文件夹,不处理")