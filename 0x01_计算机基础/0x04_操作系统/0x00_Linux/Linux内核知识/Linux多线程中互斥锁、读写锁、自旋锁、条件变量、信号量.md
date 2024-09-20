Linux
<a name="HB9dc"></a>
## 同步和互斥

- 互斥：多线程中互斥是指多个线程访问同一资源时同时只允许一个线程对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的；
- 同步：多线程同步是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源。
<a name="pfW5E"></a>
## 互斥锁
在多任务操作系统中，同时运行的多个任务可能都需要使用同一种资源。为了同一时刻只允许一个任务访问资源，需要用互斥锁对资源进行保护。互斥锁是一种简单的加锁的方法来控制对共享资源的访问，互斥锁只有两种状态，即上锁( lock )和解锁( unlock )。
<a name="sNyQh"></a>
### 互斥锁操作基本流程

1. 访问共享资源前，对互斥锁进行加锁
2. 完成加锁后访问共享资源
3. 对共享资源完成访问后，对互斥锁进行解锁

对互斥锁进行加锁后，任何其他试图再次对互斥锁加锁的线程将会被阻塞，直到锁被释放
<a name="iUBMT"></a>
### 互斥锁特性

- 原子性：互斥锁是一个原子操作，操作系统保证如果一个线程锁定了一个互斥锁，那么其他线程在同一时间不会成功锁定这个互斥锁
- 唯一性：如果一个线程锁定了一个互斥锁，在它解除锁之前，其他线程不可以锁定这个互斥锁
- 非忙等待：如果一个线程已经锁定了一个互斥锁，第二个线程又试图去锁定这个互斥锁，则第二个线程将被挂起且不占用任何CPU资源，直到第一个线程解除对这个互斥锁的锁定为止，第二个线程则被唤醒并继续执行，同时锁定这个互斥锁
<a name="ZfwMC"></a>
### 示例
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

char *pTestBuf = nullptr; // 全局变量

/* 定义互斥锁 */
pthread_mutex_t mutex;

void *ThrTestMutex(void *p)
{
    pthread_mutex_lock(&mutex);     // 加锁
    {
        pTestBuf = (char*)p;
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);   // 解锁
}

int main()
{   
    /* 初始化互斥量, 默认属性 */
    pthread_mutex_init(&mutex, NULL);

    /* 创建两个线程对共享资源访问 */
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, ThrTestMutex, (void *)"Thread1");
    pthread_create(&tid2, NULL, ThrTestMutex, (void *)"Thread2"); 

    /* 等待线程结束 */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL); 

    /* 销毁互斥锁 */
    pthread_mutex_destroy(&mutex);  

    return 0;
}
```
<a name="Kc6Y4"></a>
## 读写锁

- 读写锁允许更高的并行性，也叫共享互斥锁。互斥量要么是加锁状态，要么就是解锁状态，而且一次只有一个线程可以对其加锁。读写锁可以有3种状态：读模式下加锁状态、写模式加锁状态、不加锁状态。一次只有一个线程可以占有写模式的读写锁，但是多个线程可以同时占有读模式的读写锁，即允许多个线程读但只允许一个线程写。
- 当读操作较多，写操作较少时，可用读写锁提高线程读并发性
<a name="UFrq5"></a>
### 读写锁特性

1. 如果有线程读数据，则允许其它线程执行读操作，但不允许写操作
2. 如果有线程写数据，则其它线程都不允许读、写操作
3. 如果某线程申请了读锁，其它线程可以再申请读锁，但不能申请写锁
4. 如果某线程申请了写锁，其它线程不能申请读锁，也不能申请写锁
5. 读写锁适合于对数据的读次数比写次数多得多的情况
<a name="mmwHX"></a>
### 读写锁创建和销毁
```c
#include <pthread.h>
int phtread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

- 参数：rwlock：读写锁，attr：读写锁属性
- 返回值：成功返回0，出错返回错误码
<a name="k5iKh"></a>
### 读写锁加锁解锁
```c
#include <pthread.h>
/** 加读锁 */
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
/** 加写锁 */
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
/** 释放锁 */
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

- 参数：rwlock：读写锁
- 返回值：成功返回 0；出错，返回错误码
<a name="IlS6H"></a>
### 示例
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/* 定义读写锁 */
pthread_rwlock_t rwlock;

/* 定义共享资源变量 */
int g_nNum = 0;

/* 读操作 其他线程允许读操作 不允许写操作 */
void *fun1(void *arg)  
{  
    while(1)  
    {  
        pthread_rwlock_rdlock(&rwlock);  
        {
            printf("read thread 1 == %d\n", g_nNum);
        }      
        pthread_rwlock_unlock(&rwlock);

        sleep(1);
    }
}  

/* 读操作，其他线程允许读操作，不允许写操作 */
void *fun2(void *arg)
{    
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);  
        {
            printf("read thread 2 == %d\n", g_nNum);
        }      
        pthread_rwlock_unlock(&rwlock);

        sleep(1);
    }
} 

/* 写操作，其它线程都不允许读或写操作 */
void *fun3(void *arg)
{    
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        {
            g_nNum++;        
            printf("write thread 1\n");
        }
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
} 
/* 写操作，其它线程都不允许读或写操作 */ 
void *fun4(void *arg)
{    
    while(1)
    {  
        pthread_rwlock_wrlock(&rwlock);  
        {
            g_nNum++;  
            printf("write thread 2\n");  
        }
        pthread_rwlock_unlock(&rwlock); 

        sleep(1);  
    }  
}  

int main(int arc, char *argv[])  
{  
    pthread_t ThrId1, ThrId2, ThrId3, ThrId4;  

    pthread_rwlock_init(&rwlock, NULL);  // 初始化一个读写锁  

    /* 创建测试线程 */
    pthread_create(&ThrId1, NULL, fun1, NULL);  
    pthread_create(&ThrId2, NULL, fun2, NULL);  
    pthread_create(&ThrId3, NULL, fun3, NULL);  
    pthread_create(&ThrId4, NULL, fun4, NULL);  

    /* 等待线程结束，回收其资源 */
    pthread_join(ThrId1, NULL);  
    pthread_join(ThrId2, NULL);  
    pthread_join(ThrId3, NULL);  
    pthread_join(ThrId4, NULL);  

    pthread_rwlock_destroy(&rwlock);      // 销毁读写锁  

    return 0;  
}
```

- 结果

![DM_20230319163130_001.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1679215876235-36453f3f-cc72-41f7-b3e8-6460556800c0.png#averageHue=%23300a24&clientId=u50e09d0c-eddd-4&from=ui&id=ufcdc8a98&originHeight=417&originWidth=821&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1372143&status=done&style=none&taskId=ucccea1ea-9a88-44a2-9702-1917fb9e329&title=)
<a name="tQ6S1"></a>
## 自旋锁

- 自旋锁与互斥锁功能相同，唯一不同的就是互斥锁阻塞后休眠不占用CPU，而自旋锁阻塞后不会让出CPU，会一直忙等待，直到得到锁
- 自旋锁在用户态较少用，而在内核态使用的比较多
- 自旋锁的使用场景：锁的持有时间比较短，或者说小于2次上下文切换的时间
- 自旋锁在用户态的函数接口和互斥量一样，把`pthread_mutex_lock()`/`pthread_mutex_unlock()`中mutex换成spin，如：`pthread_spin_init()`
<a name="s3xlm"></a>
### 自旋锁函数

- linux中的自旋锁用结构体spinlock_t 表示，定义在include/linux/spinlock_type.h。自旋锁的接口函数全部定义在include/linux/spinlock.h头文件中，实际使用时只需include<linux/spinlock.h>即可
<a name="hX0mc"></a>
### 示例
```c
include<linux/spinlock.h>
spinlock_t lock;      //定义自旋锁
spin_lock_init(&lock);   //初始化自旋锁
spin_lock(&lock);      //获得锁，如果没获得成功则一直等待
{
	.......         //处理临界资源
}
spin_unlock(&lock);     //释放自旋锁
```
<a name="YoMdg"></a>
## 条件变量

- 条件变量用来阻塞一个线程，直到条件发生。通常条件变量和互斥锁同时使用。条件变量使线程可以睡眠等待某种条件满足。条件变量是利用线程间共享的全局变量进行同步的一种机制。
- 条件变量的逻辑：一个线程挂起去等待条件变量的条件成立，而另一个线程使条件成立。
<a name="lcQRu"></a>
### 基本原理
线程在改变条件状态之前先锁住互斥量。如果条件为假，线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步
<a name="lSKMC"></a>
### 示例
```c
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <pthread.h> 

pthread_cond_t taxicond = PTHREAD_COND_INITIALIZER;  
pthread_mutex_t taximutex = PTHREAD_MUTEX_INITIALIZER;  

void *ThrFun1(void *name)  
{  
    char *p = (char *)name;  

    // 加锁，把信号量加入队列，释放信号量
    pthread_mutex_lock(&taximutex); 
    {
        pthread_cond_wait(&taxicond, &taximutex);  
    } 
    pthread_mutex_unlock(&taximutex);  

    printf ("ThrFun1: %s now got a signal!\n", p);  
    pthread_exit(NULL);  
}  

void *ThrFun2(void *name)  
{  
    char *p = (char *)name;  
    printf ("ThrFun2: %s cond signal.\n", p);    // 发信号
    pthread_cond_signal(&taxicond);  
    pthread_exit(NULL);  
}  

int main (int argc, char **argv)  
{  
    pthread_t Thread1, Thread2;  
    pthread_attr_t threadattr;
    pthread_attr_init(&threadattr);  // 线程属性初始化

    // 创建三个线程 
    pthread_create(&Thread1, &threadattr, ThrFun1, (void *)"Thread1");  
    sleep(1);  

    pthread_create(&Thread2, &threadattr, ThrFun2, (void *)"Thread2");  
    sleep(1);   

    pthread_join(Thread1, NULL);
    pthread_join(Thread2, NULL);

    return 0;  
}
```

- 结果

![DM_20230319163130_002.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1679215624713-64412981-079c-415c-887c-94d26b63c036.png#averageHue=%23300a25&clientId=u50e09d0c-eddd-4&from=ui&id=AYdoD&originHeight=133&originWidth=817&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=435562&status=done&style=none&taskId=u4bfdf09d-5688-4fcb-bfd6-d92e61aff9e&title=)
<a name="O7Cor"></a>
### 虚假唤醒

- 当线程从等待已发出信号的条件变量中醒来，却发现它等待的条件不满足时，就会发生虚假唤醒。之所以称为虚假，是因为该线程似乎无缘无故地被唤醒了。但是虚假唤醒不会无缘无故发生：它们通常是因为在发出条件变量信号和等待线程最终运行之间，另一个线程运行并更改了条件
<a name="Yfe8z"></a>
### 避免虚假唤醒

- 在wait端，必须把判断条件和`wait()`放到while循环中
```c
pthread_mutex_lock(&taximutex); 
{
    while(value != wantValue)
    {
        pthread_cond_wait(&taxicond, &taximutex);  
    }
} 
pthread_mutex_unlock(&taximutex);
```
<a name="LqIQp"></a>
## 信号量

- 信号量用于进程或线程间的同步和互斥，信号量本质上是一个非负的整数计数器，它被用来控制对公共资源的访问。编程时可根据操作信号量值的结果判断是否对公共资源具有访问的权限，当信号量值大于0时，则可以访问，否则将阻塞
```c
#include <semaphore.h>

// 初始化信号量
int sem_init(sem_t *sem, int pshared, unsigned int value);

// 信号量P操作（减 1）
int sem_wait(sem_t *sem);

// 以非阻塞的方式来对信号量进行减1操作
int sem_trywait(sem_t *sem);

// 信号量V操作（加 1）
int sem_post(sem_t *sem);

// 获取信号量的值
int sem_getvalue(sem_t *sem, int *sval);

// 销毁信号量
int sem_destroy(sem_t *sem);
```
<a name="psbDV"></a>
### 示例
```c
// 信号量用于同步实例
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_g,sem_p;   //定义两个信号量
char s8Test = 'a'; 

void *pthread_g(void *arg)  //此线程改变字符的值
{    
    while(1)
    {
        sem_wait(&sem_g);
        s8Test++;
        sleep(2);
        sem_post(&sem_p);
    }
} 
void *pthread_p(void *arg)  //此线程打印字符的值
{    
    while(1)
    {
        sem_wait(&sem_p);        
        printf("%c",s8Test);
        fflush(stdout);
        sem_post(&sem_g);
    }
} 
int main(int argc, char *argv[])
{    
    pthread_t tid1,tid2;
    sem_init(&sem_g, 0, 0); // 初始化信号量为0
    sem_init(&sem_p, 0, 1); // 初始化信号量为1

    pthread_create(&tid1, NULL, pthread_g, NULL);
    pthread_create(&tid2, NULL, pthread_p, NULL); 

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);    
    return 0;
}
```

- 结果

![DM_20230319163130_003.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1679215591718-feaf2cdd-7fb9-43fd-aab1-fc023c9dec16.png#averageHue=%23300a25&clientId=u50e09d0c-eddd-4&from=ui&id=ua0d5c313&originHeight=109&originWidth=813&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=355227&status=done&style=none&taskId=u24f4f3e5-df74-46ea-b0db-bc2bfbfbd38&title=)
