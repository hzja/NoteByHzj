在RBAC中，你可以为每个角色定义一组权限，然后将用户分配给一个或多个角色。用户的权限就是他们所属角色的权限的集合。

以下代码中设置一种这样子的权限角色<br />/qwe路径只允许A访问<br />/rty路径只允许B和C访问，但是B和C通过userID参数才能访问对应的数据

效果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1699891870868-c70b63e0-2d3d-4a8e-bde5-29da37ee3ee9.png#averageHue=%23eef8fd&clientId=u26900240-bebc-4&from=paste&height=148&id=ubaae3348&originHeight=185&originWidth=732&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13383&status=done&style=none&taskId=uf58abbfa-de33-489f-834f-a6963efb872&title=&width=585.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1699892832222-e560dc32-b595-4fb5-98c8-34603c39344e.png#averageHue=%23caf5f4&clientId=u26900240-bebc-4&from=paste&height=136&id=u05e3e858&originHeight=170&originWidth=676&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=12734&status=done&style=none&taskId=u6952dba5-95cb-4834-88d6-d5e30fe42ce&title=&width=540.8)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1699891935336-6a5fc8fb-ccbc-4cdb-8acb-1fe4f630a88f.png#averageHue=%23bef1f2&clientId=u26900240-bebc-4&from=paste&height=128&id=u9fd61371&originHeight=160&originWidth=587&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=11879&status=done&style=none&taskId=ud7301fb5-b3d1-4a5c-84b9-a9f6220ef8e&title=&width=469.6)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1699891925773-a9d4532a-96bb-43c7-8bb9-4076aa68afd9.png#averageHue=%23c0f1f2&clientId=u26900240-bebc-4&from=paste&height=130&id=u3b64f072&originHeight=162&originWidth=614&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=12501&status=done&style=none&taskId=ue61a1848-411f-496d-bae7-300e48e8275&title=&width=491.2)

代码demo如下：
```shell
package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"strings"
)

// 定义角色和他们的权限
var rolePermissions = map[string][]string{
	"A": {"qwe"},
	"B": {"rty"},
	"C": {"rty"},
}

// 定义用户和他们的数据
var userData = map[string]string{
	"B": "B's data",
	"C": "C's data",
}

func RBAC() gin.HandlerFunc {
	return func(c *gin.Context) {
		// 从GET参数中获取用户的角色和用户ID
		role := c.Query("role")
		userID := c.Query("userID")

		// 检查用户的角色是否有权限访问当前路由
		permissions, ok := rolePermissions[role]
		if !ok {
			c.JSON(http.StatusForbidden, gin.H{"error": "Forbidden"})
			c.Abort()
			return
		}

		// 获取路由路径，并去掉前导斜线
		path := strings.TrimPrefix(c.Request.URL.Path, "/")

		for _, permission := range permissions {
			if permission == path {
				// 如果用户访问的是'rty'，检查他们是否只访问自己的数据
				if path == "rty" && role != userID {
					c.JSON(http.StatusForbidden, gin.H{"error": "Forbidden"})
					c.Abort()
					return
				}
				c.Next()
				return
			}
		}

		c.JSON(http.StatusForbidden, gin.H{"error": "Forbidden"})
		c.Abort()
	}
}

func main() {
	r := gin.Default()

	r.Use(RBAC())

	r.GET("/qwe", func(c *gin.Context) {
		c.JSON(http.StatusOK, gin.H{"data": "QWE"})
	})

	r.GET("/rty", func(c *gin.Context) {
		userID := c.Query("userID")
		c.JSON(http.StatusOK, gin.H{"data": userData[userID]})
	})

	r.Run()
}

```
