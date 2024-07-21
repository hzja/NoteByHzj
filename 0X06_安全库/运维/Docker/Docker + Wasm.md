Docker<br />Docker 宣布推出与 WebAssembly 集成 **(Docker+Wasm)** 的首个技术预览版，并表示公司已加入字节码联盟 (Bytecode Alliance)，成为投票成员。<br />Bytecode Alliance（字节码联盟）由 Mozilla、Fastly、Intel 与 Red Hat 联合成立，是一个推动 WebAssembly 标准化的组织，该联盟旨在通过协作实施标准和提出新标准，以完善 WebAssembly 在浏览器之外的生态。目前正在积极推动 WASI (WebAssembly System Interface)，使 WebAssembly 能够安全地访问文件、网络和内存等系统资源。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679216263639-91deca12-f911-49e4-bd7b-9b0c86939ced.png#averageHue=%23ffffff&clientId=u93f701d5-b4ac-4&from=paste&id=u2fa75ccd&originHeight=404&originWidth=966&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue3c04c70-ba88-46ab-be67-40ee9909427&title=)<br />公告写道，Docker+Wasm 让开发者能够更容易地**快速构建面向 Wasm 运行时的应用程序**。<br />下图介绍了 Docker 是如何与 WebAssembly 进行集成。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679216263655-06cdc10a-d1b9-4606-a839-683ead3491fe.png#averageHue=%23c7d6c2&clientId=u93f701d5-b4ac-4&from=paste&id=u9b1584c1&originHeight=364&originWidth=616&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9a42c17c-cdf3-4a1d-af4d-6ba5e438311&title=)<br />可以看到，Docker Engine 继续使用与整体生态相统一的 containerd 容器运行时，但创建了一个新的 containerd shim—— 把负责容器进程运行的 runC 替换成 WasmEdge runtime。据称这是和 WasmEdge 合作的项目，这个 containerd shim 从 OCI artifact 中提取 Wasm 模块，并使用 WasmEdge runtime 来运行。<br />Docker 没有透露更多细节，但 WasmEdge 承诺会提供优于 Linux 容器的启动时间，而且 WasmEdge 应用程序的体积也明显更小，且运行速度更快。<br />Docker 联合创始人 Michael Irwin 写道：“我们将 Wasm 视为 Linux 容器的补充性技术，开发者可以结合自身情况选择最适合的技术方案（或者二者并用）。随着社区对 Wasm 的探索，我们希望开发者可以使用熟悉和喜爱的经验和工具，帮助让 Wasm 应用程序更易于开发、构建和运行。”
<a name="QMNTG"></a>
## 使用
使用下面命令即可启动示例 Wasm 应用程序：
```bash
docker run -dp 8080:8080 --name=wasm-example --runtime=io.containerd.wasmedge.v1 --platform=wasi/wasm32 michaelirwin244/wasm-example
```
参数解释：

- `--runtime=io.containerd.wasmedge.v1`– 告知 Docker Engine 要使用 Wasm containerd shim 而不是标准的 Linux 容器运行时
- `--platform=wasi/wasm32`– 指定要使用的镜像的架构。通过利用 Wasm 架构，开发者无需为不同的架构构建单独的镜像。Wasm runtime 将完成将 Wasm 二进制文件转换为机器指令的最后一步。
<a name="THv0x"></a>
## 获取方式
下载 Docker Desktop 的技术预览版本即可体验 Docker+Wasm：

- macOS Apple Silicon：[https://dockr.ly/3sf56vH](https://dockr.ly/3sf56vH)
- macOS Intel：[https://dockr.ly/3VF6uFB](https://dockr.ly/3VF6uFB)
- Windows AMD64：[https://dockr.ly/3ShlsP0](https://dockr.ly/3ShlsP0)
- Linux Arm64 (deb)：[https://dockr.ly/3TDcjRV](https://dockr.ly/3TDcjRV)
- Linux AMD64 (deb)：[https://dockr.ly/3TgpWH8](https://dockr.ly/3TgpWH8)（rpm）：[https://dockr.ly/3eG6Mvp](https://dockr.ly/3eG6Mvp)（tar）：[https://dockr.ly/3yUhdCk](https://dockr.ly/3yUhdCk)

要注意的是，由于 Docker+Wasm 尚处于早期的技术预览阶段，不建议在生产环境使用，因为有可能会出现变更，甚至从未来版本中删除。<br />详情查看：[https://docs.docker.com/desktop/wasm/](https://docs.docker.com/desktop/wasm/)
