import os
import requests
from bs4 import BeautifulSoup
import time

UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.76"
Referer = "https://c.biancheng.net/view/3886.html"
Cookie = "__51vcke__KDf6QzBhogyQjall=3e5072bc-45e8-5e96-9751-6b12a80f8e53;" \
         " __51vuft__KDf6QzBhogyQjall=1709553282705; __51uvsct__KDf6QzBhogyQjall=4;" \
         " __vtins__KDf6QzBhogyQjall=%7B%22sid%22%3A%20%228618cc21-4f3e-5e0f-a820-b30" \
         "4b34db5e1%22%2C%20%22vd%22%3A%209%2C%20%22stt%22%3A%20755228%2C%20%22dr%22%3A%20708" \
         "127%2C%20%22expires%22%3A%201714922554121%2C%20%22ct%22%3A%201714920754121%7D"
headers = {"User-Agent": UserAgent, "Referer": Referer, "Cookie": Cookie}
BaseUrl = "https://c.biancheng.net/"

if os.path.exists("Qt5_C语言中文网第二版"):
    pass
else:
    os.mkdir("Qt5_C语言中文网第二版")
os.chdir("./Qt5_C语言中文网第二版")

def GetThePageOfUrls():
    ThePageOfUrls = "https://c.biancheng.net/qt/"
    ThePageOfUrlsResponse = requests.get(url=ThePageOfUrls, headers=headers)
    ThePageOfUrlsContent = ThePageOfUrlsResponse.content.decode("utf-8")
    ThePageOfUrlsSoup = BeautifulSoup(ThePageOfUrlsContent, "html.parser")
    dds = ThePageOfUrlsSoup.find("dl", id="arc-list").find_all("dd")
    Urls = []
    for dd in dds:
        Urls.append(BaseUrl + dd.a.get("href"))
    return Urls

def GetThePageOfUrl(Urls):
    i = 1
    FileList = ""
    for Url in Urls:
        ThePageOfUrlResponse = requests.get(Url, headers=headers)
        ThePageOfUrlContent = ThePageOfUrlResponse.content.decode("utf-8")
        ThePageOfUrlsSoup = BeautifulSoup(ThePageOfUrlContent, "html.parser")
        ArticleName =  ThePageOfUrlsSoup.find("div", id="article").h1.text
        ArticleContent = ThePageOfUrlsSoup.find("div", id="arc-body")
        links = ArticleContent.find_all("a")
        for link in links:
            text = link.get_text()
            href = BaseUrl + link.get("href")
            link.string = f'[{text}]({href})'
        imgs = ArticleContent.find_all("img")
        for img in imgs:
            ImgAlt = img.get("alt")
            ImgSrc = BaseUrl + img.get("src")
            img.string = f'![{ImgAlt}]({ImgSrc})'
        ArticleContent = ArticleContent.get_text()

        with open(f"{i}_{ArticleName}.md", "w", encoding="utf-8") as f:
            f.write(ArticleContent)
        print(f"{i}_{ArticleName}.md 写入完成")
        FileList = FileList + f"{i}_{ArticleName}.md\n"
        i = i + 1
    with open(f"{i}_文件总目录.md", "w", encoding="utf-8") as f:
        f.write(FileList)
    return True

Urls = GetThePageOfUrls()
Content = GetThePageOfUrl(Urls)

