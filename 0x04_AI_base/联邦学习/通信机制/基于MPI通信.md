<a name="EM68M"></a>
# MPI介绍
MPI是一种用于编写并行程序的标准，包括跨语言的通信协议和语义说明，支持进程间点对点的通信和广播通信。MPI有MPICH和OpenMPI等一些具体的实现，提供Fortran、C/C++的相应编程接口。MPI的目标是高性能、大规模和可移植性。MPI在今天仍是高性能计算的主要模型。
<a name="Y9ouw"></a>
# Python调用MPI
常见的MPI的具体实现并没有提供Python的编程接口，这就使得我们无法直接使用Python调用MPI实现高性能计算。幸运的是，在Python环境下，我们可以使用开源库mpi4py(MPI for Python)。mpi4py提供让相关进程之间进行通信、同步等操作的API，可以说是并行计算必备的基础库，适用于同一个数据中心的并行模拟计算。
