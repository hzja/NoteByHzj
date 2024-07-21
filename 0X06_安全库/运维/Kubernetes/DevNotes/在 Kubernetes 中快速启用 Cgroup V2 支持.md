KubernetesCgroup<br />Linux 中有两个 cgroup 版本：cgroup v1 和 cgroup v2。cgroup v2 是新一代的 cgroup API。<br />Kubernetes 自 v1.25 起 cgroup2 特性正式 stable。
<a name="AmCmn"></a>
## cgroup v2 有哪些优势
cgroup v2 提供了一个具有增强资源管理能力的统一控制系统。<br />cgroup v2 对 cgroup v1 进行了多项改进，例如：

- API 中单个统一的层次结构设计
- 更安全的子树委派给容器
- 更新的功能特性，例如[压力阻塞信息（Pressure Stall Information，PSI）](https://www.kernel.org/doc/html/latest/accounting/psi.html)
- 跨多个资源的增强资源分配管理和隔离
- 统一核算不同类型的内存分配（网络内存、内核内存等）
- 考虑非即时资源变化，例如页面缓存回写

一些 Kubernetes 特性专门使用 cgroup v2 来增强资源管理和隔离。例如，[MemoryQoS](https://kubernetes.io/blog/2021/11/26/qos-memory-resources/) 特性改进了内存 QoS 并依赖于 cgroup v2 原语。
<a name="D94Rh"></a>
## 使用 cgroup v2 前提
cgroup v2 具有以下要求：

- 操作系统发行版启用 cgroup v2
- Ubuntu（从 21.10 开始，推荐 22.04+）
- Debian GNU/Linux（从 Debian 11 Bullseye 开始）
- Fedora（从 31 开始）
- RHEL 和类似 RHEL 的发行版（从 9 开始）
- …
- Linux 内核为 5.8 或更高版本
- 容器运行时支持 cgroup v2。例如：
- [containerd](https://containerd.io/) v1.4 和更高版本
- [cri-o](https://cri-o.io/) v1.20 和更高版本
- kubelet 和容器运行时被配置为使用 [systemd cgroup 驱动](https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes#systemd-cgroup-driver)
<a name="UXtS2"></a>
## 使用 cgroup v2
这里以 Debian 11 Bullseye + containerd v1.4 为例。
<a name="E5f8w"></a>
### 启用并检查 Linux 节点的 cgroup v2
Debian 11 Bullseye 默认已启用 cgroup v2。<br />可以通过如下命令验证：
```bash
stat -fc %T /sys/fs/cgroup/
```

- 对于 cgroup v2，输出为 cgroup2fs。
- 对于 cgroup v1，输出为 tmpfs。

如果没有启用，可以通过在 /etc/default/grub 下的 GRUB_CMDLINE_LINUX 中添加 `systemd.unified_cgroup_hierarchy=1`，然后执行 `sudo update-grub`<br />📝Notes：如果是树莓派，标准 Raspberry Pi OS 安装时不会启用 cgroups。需要 cgroups 来启动 systemd 服务。可以通过将 cgroup_memory=1 cgroup_enable=memory `systemd.unified_cgroup_hierarchy=1` 附加到 /boot/cmdline.txt 来启用 cgroups。并重启生效
<a name="Gjkxa"></a>
### kubelet 使用 `systemd cgroup` 驱动
kubeadm 支持在执行 kubeadm init 时，传递一个 KubeletConfiguration 结构体。KubeletConfiguration 包含 cgroupDriver 字段，可用于控制 kubelet 的 cgroup 驱动。<br />说明：在版本 1.22 中，如果用户没有在 KubeletConfiguration 中设置 cgroupDriver 字段，kubeadm init 会将它设置为默认值 systemd。<br />这是一个最小化的示例，其中显式的配置了此字段：
```yaml
# kubeadm-config.yaml
kind: ClusterConfiguration
apiVersion: kubeadm.k8s.io/v1beta3
kubernetesVersion: v1.21.0
---
kind: KubeletConfiguration
apiVersion: kubelet.config.k8s.io/v1beta1
cgroupDriver: systemd
```
这样一个配置文件就可以传递给 kubeadm 命令了：
```bash
kubeadm init --config kubeadm-config.yaml
```
说明：<br />Kubeadm 对集群所有的节点，使用相同的 KubeletConfiguration。KubeletConfiguration 存放于 kube-system 命名空间下的某个 [ConfigMap](https://kubernetes.io/zh-cn/docs/concepts/configuration/configmap) 对象中。<br />执行 init、join 和 upgrade 等子命令会促使 kubeadm 将 KubeletConfiguration 写入到文件 /var/lib/kubelet/config.yaml 中，继而把它传递给本地节点的 kubelet。
<a name="WXUjV"></a>
### containerd 使用 systemd cgroup 驱动
编辑 /etc/containerd/config.toml：
```toml
[plugins.cri.containerd.runtimes.runc.options]
    SystemdCgroup = true
```
<a name="EAFXV"></a>
### 升级监控组件以支持 cgroup v2 监控
📚️Reference：<br />cgroup v2 使用一个与 cgroup v1 不同的 API，因此如果有任何应用直接访问cgroup 文件系统，则需要将这些应用更新为支持 cgroup v2 的版本。例如：

- 一些第三方监控和安全代理可能依赖于 cgroup 文件系统。要将这些代理更新到支持 > cgroup v2 的版本。
- 如果以独立的 DaemonSet 的形式运行 [cAdvisor](https://github.com/google/cadvisor) 以监控 Pod 和容器，需将其更> 新到 v0.43.0 或更高版本。
- 如果使用 JDK，推荐使用 JDK 11.0.16 及更高版本或 JDK 15 及更高版本，以便> [完全支持 cgroup v2](https://bugs.openjdk.org/browse/JDK-8230305)。

完成🎉🎉🎉
<a name="aJMSa"></a>
### 总结
Kubernetes 自 v1.25 起 cgroup2 特性正式 stable. cgroup2 相比 cgroup v1 有以下优势：

- API 中单个统一的层次结构设计
- 更安全的子树委派给容器
- 更新的功能特性，例如[压力阻塞信息（Pressure Stall Information，PSI）](https://www.kernel.org/doc/html/latest/accounting/psi.html)
- 跨多个资源的增强资源分配管理和隔离
- 统一核算不同类型的内存分配（网络内存、内核内存等）
- 考虑非即时资源变化，例如页面缓存回写

推荐在使用 Kubernetes v1.25及以上版本时，使用支持 cgroup v2 的linux 和 CRI。并启用 Kubernetes 的cgroup v2 功能。
<a name="eOMo2"></a>
## References
压力阻塞信息（Pressure Stall Information，PSI）：[https://www.kernel.org/doc/html/latest/accounting/psi.html](https://www.kernel.org/doc/html/latest/accounting/psi.html)<br />MemoryQoS：[https://kubernetes.io/blog/2021/11/26/qos-memory-resources/](https://kubernetes.io/blog/2021/11/26/qos-memory-resources/)<br />containerd：[https://containerd.io/](https://containerd.io/)<br />cri-o：[https://cri-o.io/](https://cri-o.io/)<br />systemd cgroup 驱动：[https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes#systemd-cgroup-driver](https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes#systemd-cgroup-driver)<br />ConfigMap：[https://kubernetes.io/zh-cn/docs/concepts/configuration/configmap](https://kubernetes.io/zh-cn/docs/concepts/configuration/configmap)<br />cAdvisor：[https://github.com/google/cadvisor](https://github.com/google/cadvisor)<br />完全支持 cgroup v2：[https://bugs.openjdk.org/browse/JDK-8230305](https://bugs.openjdk.org/browse/JDK-8230305)<br />压力阻塞信息（Pressure Stall Information，PSI）：[https://www.kernel.org/doc/html/latest/accounting/psi.html](https://www.kernel.org/doc/html/latest/accounting/psi.html)<br />关于 cgroup v2 | Kubernetes：[https://kubernetes.io/zh-cn/docs/concepts/architecture/cgroups/](https://kubernetes.io/zh-cn/docs/concepts/architecture/cgroups/)<br />配置 cgroup 驱动 | Kubernetes：[https://kubernetes.io/zh-cn/docs/tasks/administer-cluster/kubeadm/configure-cgroup-driver/#configuring-the-kubelet-cgroup-driver](https://kubernetes.io/zh-cn/docs/tasks/administer-cluster/kubeadm/configure-cgroup-driver/#configuring-the-kubelet-cgroup-driver)<br />容器运行时 | Kubernetes：[https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes/#containerd-systemd](https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes/#containerd-systemd)
