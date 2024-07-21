Kubernetes<br />Limits 和 Requests 并不是 CPU 管理的灵丹妙药，在某些情况下，其他替代方案可能是更好的选择。<br />阅读目标：

- CPU requests 如何工作
- CPU limits 如何工作
- 按编程语言分类的现状
- 限制不是最佳选择的情况
- 可以使用哪些替代方法来限制
<a name="O5aSq"></a>
## Kubernetes CPU Requests
Requests 在 Kubernetes 中有两个目的：
<a name="PzCR9"></a>
### 调度可用性
首先，它们告诉 Kubernetes 需要多少 CPU 可用。然后它会过滤掉任何没有足够的 Requests 资源来完成调度的节点。<br />如果没有足够的 Requests CPU/内存，则 Pod 将处于未调度状态，直到有满足 Requests 的资源。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680740510513-41128a1b-3239-46de-9373-6c030b411588.png#averageHue=%23cecf97&clientId=ue8323e13-6799-4&from=paste&id=u49520bd9&originHeight=485&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u675e3095-3efa-47fd-a89c-ac72f101489&title=)
<a name="QEKOs"></a>
### Guaranteed CPU 分配
一旦确定了在哪个节点上运行 Pod（在预选阶段幸存下来，然后得分最高的节点），它就会设置 Linux CPU shares 以大致与 mCPU 指标保持一致。[**](https://www.batey.info/cgroup-cpu-shares-for-kubernetes.html)。<br />通常，cpu_shares 可以是任意数字，Linux 使用每个 cgroup 的 shares 数与所有 shares 总数之间的比率来确定在 CPU 上应该调度哪个进程的优先级。例如，如果一个进程有 1,000 个 shares，所有 shares 的总和为 3,000，则它将获得总时间的三分之一。就 Kubernetes 而言：

- 它应该是在每个节点上配置 Linux CPU shares 的唯一工具，并且它总是将这些 shares 与 mCPU 指标对齐。
- 它只允许在机器上可用的实际逻辑 CPU 内核中请求最大的 mCPU(如果没有足够的未请求 CPU 可用，它将被调度器过滤掉)。
- 因此，它将 Linux cgroups CPU shares 几乎变成了系统上容器最少 CPU 内核的“保证”，而在 Linux 上不一定这样使用。

由于设置 CPU request 是可选的，如果不设置会怎样？

- 如果通过 LimitRange 在命名空间上设置了一个 Limit，那么这个 Limit 将会应用。
- 如果没有设置一个 request，但设置了 limit(直接或通过命名空间 LimitRange)，那么它也会设置一个 request 等于 limit。
- 如果命名空间上没有 LimitRange，也没有 limit，那么如果不指定，就不会有 request。

如果不指定 CPU request：

- 没有节点会在调度中被过滤掉，所以它可以在任何节点上调度。这意味着从 CPU 的角度来看，可以过度配置集群。
- 结果得到的 Pods/容器将是节点上优先级最低的，因为它们没有 CPU shares (而那些设置了 request 的 Pod 是有的)。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680740510508-010e25c4-fc35-4734-8913-76cd040f1983.png#averageHue=%233d545f&clientId=ue8323e13-6799-4&from=paste&id=u11544d96&originHeight=594&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8b94415f-314d-43ce-acdd-b9ea6cc7000&title=)<br />还要注意，如果没有在 Pod 上设置 Request，则会将其作为 BestEffort 服务质量(QoS)，并使其最有可能从节点中被逐出。<br />因此，设置一个足够高的 Request 来代表容器满足需求(可用性、性能/延迟等)所需的最小值是非常重要的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680740510531-1eca3de5-1b47-4f98-95f9-26660186fb4d.png#averageHue=%23ffffff&clientId=ue8323e13-6799-4&from=paste&id=u942c455e&originHeight=364&originWidth=961&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc68c118e-800e-46ef-b986-29c2f7688a3&title=)<br />此外，假设应用程序是无状态的，从可用性的角度来看，向外扩展(将 Pod 分布在更多的节点上)而不是向上扩展通常是有意义的。这也意味着在调度时，更小的 pod 更容易安装到节点上，从而更有效地利用资源。
<a name="wGX6v"></a>
## Kubernetes CPU 限制
除了请求，Kubernetes 也有 CPU 限制。限制是应用程序可以使用的最大资源量，但不能保证，因为它们取决于可用资源。<br />虽然可以将请求视为确保容器至少具有该数量/比例的 CPU 时间，但限制却确保容器的 CPU 时间不能超过该数量。<br />虽然这可以保护系统上的其他工作负载免于与受限容器竞争，但它也会严重影响受限容器的性能。限制工作的方式也不直观，用户经常错误配置它们。<br />requests 使用 Linux cgroup CPU shares，而 limits 则使用 cgroup CPU quotas。<br />这些有两个主要组成部分：

- **Accounting Period**：重置配额之前的时间量（以微秒为单位）。Kubernetes 默认将此设置为 100,000us（100 毫秒）。
- **配额周期**：cgroup（本例中为容器）在该周期内可以拥有的 CPU 时间量（以微秒为单位）。Kubernetes 设置了这个 CPU == 1000m CPU == 100,000us == 100ms。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680740510584-7e9e78b3-42c7-4012-b814-f2d888215877.png#averageHue=%23d4ddef&clientId=ue8323e13-6799-4&from=paste&id=ud77e8c45&originHeight=452&originWidth=752&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20fd6006-7715-4dd1-a6ef-b51c0879fe4&title=)<br />如果这是一个单线程应用程序(一次只能在一个 CPU 核心上运行)，那么这就很直观了。如果将限制设置为 1 个 CPU，那么每 100 毫秒就会获得 100 毫秒的 CPU 时间，或者全部时间。问题是当有一个多线程应用程序可以同时在多个 cpu 上运行时。<br />如果有两个线程，那么可以在最短的 50ms 内消耗一个 CPU 周期(100ms)。10 个线程可以在 10 毫秒内消耗 1 个 CPU 周期，这样每 100 毫秒就会节流 90 毫秒，也就是 90%的时间!这通常会导致比没有节流的单个线程更差的性能。<br />另一个重要的考虑是，一些应用程序或语言/运行时将看到 Node 中的核心数量，并假设它可以使用所有的核心，而不管它的请求或限制。假设 Kubernetes 节点/集群/环境在拥有的内核数量上不一致。在这种情况下，相同的 Limit 可能导致节点之间的不同行为，因为它们将运行不同数量的线程来对应不同数量的 core。<br />所以，需要:

- 设置 Limit 以容纳所有线程。
- 降低线程数以便与 Limit 对齐。

除非语言/运行时知道查看 cgroup 并自动适应 Limit。
<a name="VtuTI"></a>
## 编程语言的现状
<a name="Ind6e"></a>
### node.js
Node 是单线程的(除非使用 worker_threads)，所以如果没有它们，代码就不能使用一个以上的 CPU 内核。这使得它成为 Kubernetes 上扩展到更多 pod 的良好候选，而不是扩展单个 pod。

- 例外的是，Node.js 在默认情况下运行四个线程来执行各种操作(文件系统 IO、DNS、crypto 和 zlib)，而 UV_THREADPOOL_SIZE 环境变量可以控制这一点。
- 如果确实使用 worker_threads，那么想防止节流，那么需要确保运行的并发线程不会超过 CPU 限制。如果通过 piscina 这样的线程池包使用它们，这将更容易做到，在这种情况下，需要确保 maxThreads 被设置为与 CPU 限制相匹配，而不是与节点中的物理 CPU 相匹配(piscina 目前默认为物理 CPU 的 1.5 倍-并且不会从 cgroup/container limit 自动计算出限制)。
<a name="bmlQ2"></a>
### Python
像 Node.js 一样，解释性 Python 通常是单线程的，除了一些例外(使用多处理库，C/c++扩展等)，不应该使用多个 CPU。这使得它成为一个很好的候选，就像 Node.js 一样，可以扩展到 Kubernetes 上的更多 pod，而不是扩展单个 pod。<br />注意，如果使用它，多处理库假定 Node 的物理内核数是默认情况下应该运行的线程数。目前似乎没有办法影响这种行为，除了在代码中设置它，而不是采用默认池大小。[**](https://github.com/python/cpython/issues/77167)。
<a name="Tn8Wp"></a>
### Java
Java 虚拟机 (JVM) 现在提供自动容器/cgroup 检测支持，这允许它确定在 Docker 容器中运行的 Java 进程可用的内存量和处理器数量。它使用此信息来分配系统资源。此支持仅适用于 Linux x64 平台。如果支持，则默认启用容器支持。如果不支持，可以使用 `-XX:ActiveProcessorCount=X` [**手动指定 CPU 限制的核心数**](https://docs.oracle.com/en/java/javase/19/docs/specs/man/java.html#advanced-runtime-options-for-java)。
<a name="tQK8e"></a>
### NET/C
与 Java 一样，[**](https://github.com/dotnet/designs/blob/main/accepted/2019/support-for-memory-limits.md)。
<a name="dgKIE"></a>
### Golang
[**为容器设置 GOMAXPROCS 环境变量**](https://pkg.go.dev/runtime)以匹配任何 CPU 限制。或者使用 [**](https://github.com/uber-go/automaxprocs) 根据 Uber 的容器限制自动设置。
<a name="hahr9"></a>
## 真的需要限制吗?
也许可以避免所有这些，只使用请求，因为它们讨论的是总 CPU 的比例，而不是像限制那样的 CPU 时间，因此在概念上更有意义。<br />限制的主要目的是确保特定的容器只能获得节点的特定份额。如果节点数量是相当固定的，可能是因为这是一个内部部署的数据中心，在接下来的几个月/几年里使用的裸金属节点数量是固定的，那么它们可能是维护整个多租户系统/环境的稳定性和公平性不得不接受的事。<br />然而，在云计算中，拥有的节点数量实际上是无限的，除了它们的成本。他们可以快速地提供和取消供应。这种弹性和灵活性是许多客户首先在云中运行这些工作负载的重要原因!<br />同样，需要多少节点直接取决于需要多少 pod 和它们的大小。需要的 pod 数量可以自动/动态地缩放，以响应在任何给定时间内要完成的工作量(请求的数量、队列中的项目数量等)。<br />可以做很多事情来节省成本，但是以一种损害其性能甚至可用性的方式限制工作负载应该是最后的手段。
<a name="iIT68"></a>
## Limit 的替代品
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680740510528-6ef444d2-574b-4dc3-ab90-35dcf1a70e9f.png#averageHue=%23fcfefe&clientId=ue8323e13-6799-4&from=paste&id=u294676cb&originHeight=176&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua7850aab-9e12-4050-8f9f-28fd94f4e31&title=)<br />假设工作量增加（这会因工作负载而异，可能是请求/秒、队列中的工作量、响应的延迟等）。首先，Horizontal Pod Autoscaler (HPA) 会自动添加另一个 Pod，但没有足够的节点容量来调度它。Kubernetes Cluster Autoscaler 看到这个 Pending Pod，然后添加一个节点。<br />随后，工作日结束，由于要做的工作较少，HPA 缩减了几个 Pods。集群自动缩放器有足够的空闲容量来缩小一些节点，并且仍然适合所有运行的节点。因此，它排空并终止一些节点，以便为现在正在运行的较少的 Pods 调整环境的大小。<br />在这种情况下，不需要限制，只需要设置准确的请求(这样可以保证每个系统通过 CPU 共享运行所需的最小值)和集群自动定标器。这将确保不会对工作负载进行不必要的节流，并确保在任何给定时间都具有适当的容量。
<a name="WloMU"></a>
## 结论
现在更好地了解了 Kubernetes CPU 限制，是时候检查工作负载并适当地配置它们了。<br />这通常意味着确保运行的并发线程数符合限制（尽管某些语言/运行时，如 Java 和 C# 现在可以做到这一点）。<br />而且，有时，这意味着根本不使用它们，而是依靠 Requests 和 Horizontal Pod Autoscaler (HPA) 的组合来确保始终有足够的容量以更动态（和更少的节流）方式运行。
<a name="ven5V"></a>
## 参考资料
Kubernetes 限制和请求的基础知识: [https://sysdig.com/blog/kubernetes-limits-requests/](https://sysdig.com/blog/kubernetes-limits-requests/)<br />内存不足终止和 CPU 节流: [https://sysdig.com/blog/troubleshoot-kubernetes-oom/](https://sysdig.com/blog/troubleshoot-kubernetes-oom/)<br />CPU Shares 是 Linux 控制组 cgroups 功能: [https://www.batey.info/cgroup-cpu-shares-for-kubernetes.html](https://www.batey.info/cgroup-cpu-shares-for-kubernetes.html)<br />目前 GitHub 上有一个未解决的问题: [https://github.com/python/cpython/issues/77167](https://github.com/python/cpython/issues/77167)<br />手动指定 CPU 限制的核心数: [https://docs.oracle.com/en/java/javase/19/docs/specs/man/java.html#advanced-runtime-options-for-java](https://docs.oracle.com/en/java/javase/19/docs/specs/man/java.html#advanced-runtime-options-for-java)<br />提供了自动容器/cgroup 检测支持: [https://github.com/dotnet/designs/blob/main/accepted/2019/support-for-memory-limits.md](https://github.com/dotnet/designs/blob/main/accepted/2019/support-for-memory-limits.md)<br />为容器设置 GOMAXPROCS 环境变量: [https://pkg.go.dev/runtime](https://pkg.go.dev/runtime)<br />automaxprocs 包: [https://github.com/uber-go/automaxprocs](https://github.com/uber-go/automaxprocs)
