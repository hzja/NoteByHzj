Nvidia<br />设备跟踪和管理正成为机器学习工程的中心焦点。这个任务的核心是在模型训练过程中跟踪和报告gpu的使用效率。<br />有效的GPU监控可以配置一些非常重要的超参数，例如批大小，还可以有效的识别训练中的瓶颈，比如CPU活动(通常是预处理图像)占用的时间很长，导致GPU需要等待下一批数据的交付，从而处于空闲状态。
<a name="waJzb"></a>
## 什么是利用率？
过去的一个采样周期内GPU 内核执行时间的百分比，就称作GPU的利用率。<br />如果这个值很低，则意味着您的 GPU 并没有全速的工作，可能是受到 CPU或者IO 操作的瓶颈，如果使用的按小时付费的云服务器，那么就是在浪费时间和金钱！
<a name="tMuAN"></a>
## 使用终端命令监控
```bash
nvidia-smi
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636805531800-c1ec4111-59f4-4b20-a062-a27c32d0c2b8.png#clientId=u80da2bf1-72d8-4&from=paste&height=587&id=u19771eae&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2043136&status=done&style=none&taskId=uc19e2b6e-178b-4de8-bb9d-3b03c3d6a31&title=&width=1107.6666666666667)<br />以下是收集的一些信息：

- GPU：Tesla T4
- 设备温度：设备当前运行温度为 25 摄氏度
- 功耗：GPU 目前运行功率9W，官方设定的额定最大功率消耗 70W  。
- 显存：0MiB / 15109MiB 上限
- GPU利用率：0%。同样，NVIDIA 将利用率定义如下：过去采样周期中一个或多个内核在 GPU 上执行的时间百分比。

如果负责硬件相关的工作，温度和功率是跟踪的可能是关注的主要问题，这样就可以平衡尝试最大化计算和维护设备安全。如果是硬件使用者（就像一般使用云服务器一样），最关心的应该是内存使用和GPU利用率。<br />使用 `nvidia-smi` 进行监控的其他一些技巧：<br />调用 `watch -n 1 nvidia-smi`  可以每一秒进行自动的刷新。<br />nvidia-smi 也可以通过添加 `--format=csv` 以 CSV 格式输。在 CSV 格式中，可以通过添加 `--gpu-query=...` 参数来选择显示的指标。<br />为了实时显示 CSV 格式并同时写入文件，可以将 nvidia-smi 的输出传输到 tee 命令中，如下所示。这将写入选择的文件路径。
```bash
nvidia-smi --query-gpu=timestamp,pstate,temperature.gpu,utilization.gpu,utilization.memory,memory.total,memory.free,memory.used --format=csv | tee gpu-log.csv
```
<a name="JnWlW"></a>
## 用 Python 代码监控
基于终端的工具很棒，但有时希望将 GPU 监控和日志记录直接整合到 Python 程序中。这里提供2中方法：
<a name="Bc19L"></a>
### 1、使用NVIDIA 管理库 (NVML)
NVML（nvidia-management-library）是CUDA中提供的可以查看显卡信息的工具包,nvidia-smi也是基于这个工具包<br />在python中NVML有很多个包，只比较其中的两个。nvvidia-ml-py3 ，它是 NVML 的简单接口，没有任何重要的附加功能。使用此库可能如下所示：
```python
# Install with "pip install nvidia-ml-p3"
import pynvml# Must call this first
pynvml.nvmlInit()

# Use device index to get handle
handle = pynvml.nvmlDeviceGetHandleByIndex(0)

# Use handle to get device stats
memory_info = pynvml.nvmlDeviceGetMemoryInfo(handle)
utilization = pynvml.nvmlDeviceGetUtilizationRates(handle)

# Report device stats
print("Total memory:", memory_info.total)
print("Free memory:", memory_info.free)
print("Used memory:", memory_info.used)
print("GPU Utilization:", utilization.gpu)
print("Memory Utilization:", utilization.memory)
```
另一个比较好用的库是py3nvml，因为它添加了一些用于管理 GPU 的实用功能，而 nvidia-ml-py3 仅用于监控。除了上面显示的功能类型之外，该库还允许执行以下操作（摘自官方文档）：
```python
import py3nvml
import tensorflow as tf
py3nvml.grab_gpus(3)
sess = tf.Session() # now we only grab 3 gpus!
```
在这里，可以在一台可以访问多个 GPU 的机器上运行，但只想将其中三个用于 本次TensorFlow session。使用 py3nvml，可以简单地调用 `py3nvml.grab_gpus(3)` 来分配三个设备。

<a name="Kjj5a"></a>
## 用Golang代码监控
利用Golang监控NVIDIA GPU的运行状态和使用情况实质是解析nvidia-smi指令的信息，主要使用的是Golang中xml解析包 "[encoding/xml](https://pkg.go.dev/encoding/xml)"
<a name="M4Odj"></a>
### 解析的结构体
```go
type NvidiaSmiLog struct {
	DriverVersion string `xml:"driver_version"`
	AttachedGPUs  string `xml:"attached_gpus"`
	GPUs          []struct {
		ProductName  string `xml:"product_name"`
		ProductBrand string `xml:"product_brand"`
		UUID         string `xml:"uuid"`
		FanSpeed     string `xml:"fan_speed"`
		PCI          struct {
			PCIBus string `xml:"pci_bus"`
		} `xml:"pci"`
		FbMemoryUsage struct {
			Total string `xml:"total"`
			Used  string `xml:"used"`
			Free  string `xml:"free"`
		} `xml:"fb_memory_usage"`
		Utilization struct {
			GPUUtil    string `xml:"gpu_util"`
			MemoryUtil string `xml:"memory_util"`
		} `xml:"utilization"`
		Temperature struct {
			GPUTemp              string `xml:"gpu_temp"`
			GPUTempMaxThreshold  string `xml:"gpu_temp_max_threshold"`
			GPUTempSlowThreshold string `xml:"gpu_temp_slow_threshold"`
		} `xml:"temperature"`
		PowerReadings struct {
			PowerDraw  string `xml:"power_draw"`
			PowerLimit string `xml:"power_limit"`
		} `xml:"power_readings"`
		Clocks struct {
			GraphicsClock string `xml:"graphics_clock"`
			SmClock       string `xml:"sm_clock"`
			MemClock      string `xml:"mem_clock"`
			VideoClock    string `xml:"video_clock"`
		} `xml:"clocks"`
		MaxClocks struct {
			GraphicsClock string `xml:"graphics_clock"`
			SmClock       string `xml:"sm_clock"`
			MemClock      string `xml:"mem_clock"`
			VideoClock    string `xml:"video_clock"`
		} `xml:"max_clocks"`
	} `xml:"gpu"`
}
```
<a name="w5lav"></a>
### 解析的过程
```go
package domain

import (
	"encoding/xml"
	"log"
	"os"
	"os/exec"
	"regexp"
	"strconv"
)

//https://www.jianshu.com/p/dd8a113b02a3 go语言执行shell脚本

//https://www.cnblogs.com/caishunzhe/articles/12667889.html

const NvidiaSmiPath = "/usr/bin/nvidia-smi"

var testMode string

/**
GPU信息
*/
type Gpu struct {
	Temp        []GpuItem `json:"temp"`        //温度(C)
	Usage       []GpuItem `json:"usage"`       //显存使用(使用情况 MiB)
	UsedPercent []GpuItem `json:"usedPercent"` //使用占比(%)
}

//temp: [{ data:["72","72C"],name: 0 },{ data:["72","72C"],name: 1 }]
//usage: [{ data:["8493","8493Mib/15079Mib"],name: 0 },{ data:["9403","9403Mib/15079Mib"],name: 1 }]
//usedPercent: [{ data:["56","56%"],name: 0 },{ data:["43","43%"],name: 1 }]
type GpuItem struct {
	Data []string `json:"data"`
	Name int      `json:"name"`
}

/**
  获取gpu信息
*/
func (gpu *Gpu) GpuInfo() {

	//获得GPU metrics 信息
	metrics(gpu)

}

type NvidiaSmiLog struct {
	DriverVersion string `xml:"driver_version"`
	AttachedGPUs  string `xml:"attached_gpus"`
	GPUs          []struct {
		ProductName  string `xml:"product_name"`
		ProductBrand string `xml:"product_brand"`
		UUID         string `xml:"uuid"`
		FanSpeed     string `xml:"fan_speed"`
		PCI          struct {
			PCIBus string `xml:"pci_bus"`
		} `xml:"pci"`
		FbMemoryUsage struct {
			Total string `xml:"total"`
			Used  string `xml:"used"`
			Free  string `xml:"free"`
		} `xml:"fb_memory_usage"`
		Utilization struct {
			GPUUtil    string `xml:"gpu_util"`
			MemoryUtil string `xml:"memory_util"`
		} `xml:"utilization"`
		Temperature struct {
			GPUTemp              string `xml:"gpu_temp"`
			GPUTempMaxThreshold  string `xml:"gpu_temp_max_threshold"`
			GPUTempSlowThreshold string `xml:"gpu_temp_slow_threshold"`
		} `xml:"temperature"`
		PowerReadings struct {
			PowerDraw  string `xml:"power_draw"`
			PowerLimit string `xml:"power_limit"`
		} `xml:"power_readings"`
		Clocks struct {
			GraphicsClock string `xml:"graphics_clock"`
			SmClock       string `xml:"sm_clock"`
			MemClock      string `xml:"mem_clock"`
			VideoClock    string `xml:"video_clock"`
		} `xml:"clocks"`
		MaxClocks struct {
			GraphicsClock string `xml:"graphics_clock"`
			SmClock       string `xml:"sm_clock"`
			MemClock      string `xml:"mem_clock"`
			VideoClock    string `xml:"video_clock"`
		} `xml:"max_clocks"`
	} `xml:"gpu"`
}

func formatValue(key string, meta string, value string) string {
	result := key
	if meta != "" {
		result += "{" + meta + "}"
	}
	return result + " " + value + "\n"
}

func filterNumber(value string) string {
	r := regexp.MustCompile("[^0-9.]")
	return r.ReplaceAllString(value, "")
}

func metrics(gpu *Gpu) {

	var cmd *exec.Cmd
	if testMode == "1" {
		dir, err := os.Getwd()
		if err != nil {
			log.Fatal(err)
		}
		cmd = exec.Command("/bin/cat", dir+"/test.xml")
	} else {
		cmd = exec.Command(NvidiaSmiPath, "-q", "-x")
	}

	// Execute system command
	stdout, err := cmd.Output()
	if err != nil {

		//初始信息
		log.Println("解析GPU信息错误，错误信息为：", err.Error())
		//一块显卡的温度信息
		dataTemp := []string{"72", "72C"}
		itemTemp := GpuItem{Data: dataTemp, Name: 0}
		itemsTemp := []GpuItem{itemTemp}
		gpu.Temp = itemsTemp
		//一块显卡的显存使用信息
		dataUsage := []string{"8493", "8493Mib/15079Mib"}
		itemUsage := GpuItem{Data: dataUsage, Name: 0}
		itemsUsage := []GpuItem{itemUsage}
		gpu.Usage = itemsUsage
		//一块显卡的使用占比信息
		dataUsedPercent := []string{"56", "56%"}
		itemUsedPercent := GpuItem{Data: dataUsedPercent, Name: 0}
		itemsUsedPercent := []GpuItem{itemUsedPercent}
		gpu.UsedPercent = itemsUsedPercent

	} else {

		//一块显卡的温度信息

		// Parse XML
		var xmlData NvidiaSmiLog
		_ = xml.Unmarshal(stdout, &xmlData)

		//创建GPU温度切片
		itemsTemp := make([]GpuItem, len(xmlData.GPUs))

		//创建显卡的显存使用信息切片
		itemsUsage := make([]GpuItem, len(xmlData.GPUs))

		//创建显卡的使用占比信息切片
		itemsUsedPercent := make([]GpuItem, len(xmlData.GPUs))

		log.Println("nvidiasmi_driver_version:%s", xmlData.DriverVersion)

		log.Println("nvidiasmi_attached_gpus:%s", filterNumber(xmlData.AttachedGPUs))

		for index, info := range xmlData.GPUs {

			//温度信息开始
			gpuTemp := filterNumber(info.Temperature.GPUTemp)

			dataTemp := []string{gpuTemp, gpuTemp + "C"}

			itemsTemp[index] = GpuItem{Data: dataTemp, Name: index}
			//温度信息结束

			//显存使用情况开始
			used := filterNumber(info.FbMemoryUsage.Used)
			total := filterNumber(info.FbMemoryUsage.Total)

			dataUsage := []string{used, used + "Mib/" + total + "Mib"}

			itemsUsage[index] = GpuItem{Data: dataUsage, Name: index}

			//显存使用情况结束
			usedInt, _ := strconv.ParseInt(used, 10, 64)
			totalInt, _ := strconv.ParseInt(used, 10, 64)

			UsedPercent := float32(0.0)
			if usedInt == 0 || totalInt == 0 {
				UsedPercent = 0.00
			} else {
				UsedPercent = float32(usedInt / totalInt)
			}

			//显卡的使用占比信息开始
			dataUsedPercent := []string{FloatToString(UsedPercent), (FloatToString(UsedPercent)) + "%"}
			itemsUsedPercent[index] = GpuItem{Data: dataUsedPercent, Name: index}
			//显卡的使用占比信息结束

		}

		gpu.Temp = itemsTemp
		gpu.Usage = itemsUsage
		gpu.UsedPercent = itemsUsedPercent

	}

}

//float32 转 String工具类，保留6位小数
func FloatToString(input_num float32) string {
	// to convert a float number to a string
	return strconv.FormatFloat(float64(input_num), 'f', 2, 64)
}
```
这里的解析过程部分是业务输出，为了监控信息完整添加了默认信息
```go
//初始信息
log.Println("解析GPU信息错误，错误信息为：", err.Error())
//一块显卡的温度信息
dataTemp := []string{"72", "72C"}
itemTemp := GpuItem{Data: dataTemp, Name: 0}
itemsTemp := []GpuItem{itemTemp}
gpu.Temp = itemsTemp
//一块显卡的显存使用信息
dataUsage := []string{"8493", "8493Mib/15079Mib"}
itemUsage := GpuItem{Data: dataUsage, Name: 0}
itemsUsage := []GpuItem{itemUsage}
gpu.Usage = itemsUsage
//一块显卡的使用占比信息
dataUsedPercent := []string{"56", "56%"}
itemUsedPercent := GpuItem{Data: dataUsedPercent, Name: 0}
itemsUsedPercent := []GpuItem{itemUsedPercent}
gpu.UsedPercent = itemsUsedPercent
```
这里使用者可以根据需要是否保留
