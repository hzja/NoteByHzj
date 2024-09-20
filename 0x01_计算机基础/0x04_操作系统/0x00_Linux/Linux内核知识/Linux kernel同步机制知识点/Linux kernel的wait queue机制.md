Linux
<a name="kO91C"></a>
## 1、介绍
当编写Linux驱动程序、模块或内核程序时，一些进程会等待或休眠一些事件。Linux中有几种处理睡眠和醒来的方法，每种方法对应不同的需求，而wait queue便是其中一种。<br />每当进程必须等待一个事件（例如数据的到达或进程的终止）时，它都应该进入睡眠状态。睡眠会导致进程暂停执行，从而释放处理器以供其他用途。一段时间后，该过程将被唤醒，并在等待的事件到达时继续其工作。<br />等待队列是内核提供的一种机制，用于实现等待。顾名思义，wait queue是等待事件的进程列表。换句话说，当某个条件成立时，等待队列用于等待有人叫醒你。它们必须小心使用，以确保没有竞争条件的存在。<br />实现wait queue的步骤如下：

1. **初始化等待队列**
2. **排队（将任务置于睡眠状态，直到事件发生）**
3. **唤醒排队的任务**

以下逐步介绍每个步骤的实现方式。
<a name="xopam"></a>
## 2、初始化等待队列
若使用wait queue功能，需要包含/linux/wait.h头文件。可基于动态和静态两种方式实现等待队列的初始化。<br />静态方式：
```
DECLARE_WAIT_QUEUE_HEAD(wq);
```
其中，wq是要将任务置于睡眠状态的队列的名称。<br />动态方式：
```c
wait_queue_head_t wq;
init_waitqueue_head (&wq);
```
除了创建等待队列的方式不同之外，其他操作对于静态和动态方法都是相同的。
<a name="hlZSv"></a>
## 3、排队
一旦声明并初始化了等待队列，进程就可以使用它进入睡眠状态。有几个宏可用于不同的用途。将逐一说明。

1. `**wait_event**`
2. `**wait_event_timeout**`
3. `**wait_event_cmd**`
4. `**wait_event_interruptible**`
5. `**wait_event_interruptible_timeout**`
6. `**wait_event_killable**`

每当使用上面的宏时，它会将该任务添加到创建的等待队列中。然后它会等待事件。
<a name="Y8XgQ"></a>
### wait_event
进程进入休眠状态(TASK_UNINTERUPTIBLE)，直到条件评估为true。每次唤醒等待队列wq时，都会检查该条件。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 */
wait_event(wq, condition);
```
<a name="WPRQ1"></a>
### wait_event_timeout
进程进入休眠状态(TASK_UNINTERUPTIBLE)，直到条件评估为true或超时。每次唤醒等待队列wq时，都会检查该条件。<br />如果超时后条件评估为false，则返回0；如果超时后情况评估为true，则返回1；如果超时前情况评估为true，则返回剩余的jiffies（至少1）。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 * timeout –  超时时间，单位jiffies 
 */
wait_event_timeout(wq, condition, timeout);
```
<a name="DKP5F"></a>
### wait_event_cmd
进程进入休眠状态(TASK_UNINTERUPTIBLE)，直到条件评估为true。每次唤醒等待队列wq时，都会检查该条件。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 * cmd1–该命令将在睡眠前执行
 * cmd2–该命令将在睡眠后执行
 */
wait_event_cmd(wq, condition, cmd1, cmd2);
```
<a name="aPHsw"></a>
### wait_event_interruptible
进程进入休眠状态(TASK_INTERRUPTIBLE)，直到条件评估为真或接收到信号。每次唤醒等待队列wq时，都会检查该条件。<br />如果被信号中断，函数将返回-ERESTARTSYS，如果条件评估为true，则返回0。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 */
wait_event_interruptible(wq, condition);
```
<a name="GbUkV"></a>
### wait_event_interruptible_timeout
进程进入休眠状态(TASK_INTERRUPTIBLE)，直到条件评估为真或接收到信号或超时。每次唤醒等待队列wq时，都会检查该条件。<br />如果超时后条件评估为false，则返回0；如果超时后情况评估为true，则返回1；如果超时前情况评估为true，则返回剩余的jiffies（至少1）；如果被信号中断，则返回-ERESTARTSYS。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 * timeout –  超时时间，单位jiffies 
 */
wait_event_interruptible_timeout(wq, condition, timeout);
```
<a name="wFXSG"></a>
### wait_event_killable
进程进入休眠状态（TASK_KILLABLE），直到条件评估为真或收到信号。每次唤醒等待队列wq时，都会检查该条件。<br />如果被信号中断，函数将返回-ERESTARTSYS，如果条件评估为true，则返回0。
```c
/* wq – 等待队列
 * condition - 要等待的C表达式的事件 
 */
wait_event_killable(wq, condition);
```
<a name="wv1tk"></a>
## 4、唤醒排队的任务
当一些任务由于等待队列而处于睡眠模式时，可以使用下面的函数来唤醒这些任务。

1. `**wake_up**`
2. `**wake_up_all**`
3. `**wake_up_interruptible**`
4. **wake_up_sync and wake_up_interruptible_sync**

通常，调用wake_up会立即触发重新调度，这意味着在wake_up返回之前可能会运行其他进程。“同步”变体使任何唤醒的进程都可以运行，但不会重新调度CPU。这用于避免在已知当前进程进入睡眠状态时重新调度，从而强制重新调度。注意，被唤醒的进程可以立即在不同的处理器上运行，因此不应期望这些函数提供互斥
<a name="ww0bX"></a>
## 5、实践
在两个地方发送了一个wake_up。一个来自读取功能，另一个来自驱动退出。<br />首先创建了一个线程（wait_function）。该线程将始终等待该事件。它会一直睡到接到唤醒事件。当它得到wake_up调用时，它将检查条件。如果条件为1，则唤醒来自读取功能。如果是2，则唤醒来自退出功能。如果wake_up来自读取功能，它将打印读取计数，并再次等待。如果它来自exit函数，那么它将从线程中退出。
<a name="CD59h"></a>
### 静态创建wait queue
```c
/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple linux driver (Waitqueue Static method)
*
*  \author     xxx
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/wait.h>                 // Required for the wait queues
#include <linux/err.h>


uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_etx);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
int wait_queue_flag = 0;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);

/*************** Driver functions **********************/
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = etx_read,
    .write          = etx_write,
    .open           = etx_open,
    .release        = etx_release,
};

/*
** Thread function
*/
static int wait_function(void *unused)
{

    while(1) {
        pr_info("Waiting For Event...\n");
        wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
        if(wait_queue_flag == 2) {
            pr_info("Event Came From Exit Function\n");
            return 0;
        }
        pr_info("Event Came From Read Function - %d\n", ++read_count);
        wait_queue_flag = 0;
    }
    do_exit(0);
    return 0;
}

/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!!!\n");
    return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Read Function\n");
    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_etx);
    return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Write function\n");
    return len;
}

/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
        pr_info("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&etx_cdev,&fops);
    etx_cdev.owner = THIS_MODULE;
    etx_cdev.ops = &fops;

    /*Adding character device to the system*/
    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_info("Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
        pr_info("Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
        pr_info("Cannot create the Device 1\n");
        goto r_device;
    }

    //Create the kernel thread with name 'mythread'
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if (wait_thread) {
        pr_info("Thread Created successfully\n");
        wake_up_process(wait_thread);
    } else
        pr_info("Thread creation failed\n");

    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

    r_device:
    class_destroy(dev_class);
    r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
    wait_queue_flag = 2;
    wake_up_interruptible(&wait_queue_etx);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xxx");
MODULE_DESCRIPTION("Simple linux driver (Waitqueue Static method)");
MODULE_VERSION("1.7");
```
<a name="LdZ7x"></a>
### 动态创建wait queue
```c
/****************************************************************************//**
*  \file       driver.c
*
*  \details    Simple linux driver (Waitqueue Dynamic method)
*
*  \author     xxx
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/wait.h>                 // Required for the wait queues
#include <linux/err.h>


uint32_t read_count = 0;
static struct task_struct *wait_thread;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
wait_queue_head_t wait_queue_etx;
int wait_queue_flag = 0;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);

/*************** Driver functions **********************/
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = etx_read,
    .write          = etx_write,
    .open           = etx_open,
    .release        = etx_release,
};

/*
** Thread function
*/
static int wait_function(void *unused)
{

    while(1) {
        pr_info("Waiting For Event...\n");
        wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
        if(wait_queue_flag == 2) {
            pr_info("Event Came From Exit Function\n");
            return 0;
        }
        pr_info("Event Came From Read Function - %d\n", ++read_count);
        wait_queue_flag = 0;
    }
    return 0;
}

/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!!!\n");
    return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Read Function\n");
    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_etx);
    return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Write function\n");
    return len;
}

/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
        pr_info("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&etx_cdev,&fops);

    /*Adding character device to the system*/
    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_info("Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
        pr_info("Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
        pr_info("Cannot create the Device 1\n");
        goto r_device;
    }

    //Initialize wait queue
    init_waitqueue_head(&wait_queue_etx);

    //Create the kernel thread with name 'mythread'
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if (wait_thread) {
        pr_info("Thread Created successfully\n");
        wake_up_process(wait_thread);
    } else
        pr_info("Thread creation failed\n");

    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

    r_device:
    class_destroy(dev_class);
    r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
    wait_queue_flag = 2;
    wake_up_interruptible(&wait_queue_etx);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xxx");
MODULE_DESCRIPTION("Simple linux driver (Waitqueue Dynamic method)");
MODULE_VERSION("1.8");
```
<a name="fPaS1"></a>
### MakeFile
```
obj-m += driver.o
KDIR = /lib/modules/$(shell uname -r)/build
all:
    make -C $(KDIR)  M=$(shell pwd) modules
clean:
    make -C $(KDIR)  M=$(shell pwd) clean
```
<a name="Ybrzx"></a>
### 编译和测试

- **使用Makefile（sudo make）构建驱动程序**
- **使用sudo insmod driver.ko加载驱动程序**
- **然后检查dmesg**
```
Major = 246 Minor = 0
Thread Created successfully
Device Driver Insert...Done!!!
Waiting For Event...
```

- **因此，该线程正在等待该事件。现在，将通过使用sudo cat/dev/etx_device读取驱动程序来发送事件**
- **现在检查dmesg**
```
Device File Opened...!!!
Read Function
Event Came From Read Function - 1
Waiting For Event...
Device File Closed...!!!
```

- **从读取功能发送唤醒，因此它将打印读取计数，然后再次休眠。现在通过sudo rmmod驱动程序从退出功能发送事件**
```
Event Came From Exit Function
Device Driver Remove...Done!!!
```

- **现在条件是2。因此，它将从线程返回并删除驱动程序。**
