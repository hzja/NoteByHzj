要去unidoc官网免费注册个key出来使用。
```
package main

import (
	"fmt"
	"github.com/unidoc/unioffice/common/license"
	"github.com/unidoc/unioffice/document"
)

func main() {
	err := license.SetMeteredKey("")
	if err != nil {
		fmt.Printf("error setting license key: %s\n", err)
		return
	}

	headings := []string{"a", "b", "c"}
	content := []string{"d", "e", "f"}

	doc := document.New()

	for i := 0; i < len(headings); i++ {
		para := doc.AddParagraph()
		run := para.AddRun()
		run.Properties().SetBold(true)
		run.Properties().SetSize(20)
		run.AddText(headings[i])

		para = doc.AddParagraph()
		run = para.AddRun()
		run.AddText(content[i])
		para.AddRun().AddBreak() //内容换行
	}

	err = doc.SaveToFile("example.docx")
	if err != nil {
		fmt.Printf("error saving document: %s\n", err)
	}
}
 
```
