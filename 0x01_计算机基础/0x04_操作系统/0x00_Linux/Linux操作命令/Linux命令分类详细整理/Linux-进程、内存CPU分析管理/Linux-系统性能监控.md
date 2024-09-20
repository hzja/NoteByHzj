Linux sar
<a name="VwLOQ"></a>
# sar
<a name="MibC6"></a>
## `sar -h`：sar的帮助命令
```bash
[root@iZuligp6e1dyzfZ ~]# sar -h
Usage: sar [ options ] [ <interval> [ <count> ] ]
Main options and reports:
        -b      I/O and transfer rate statistics
        -B      Paging statistics
        -d      Block device statistics
        -H      Hugepages utilization statistics
        -I { <int> | SUM | ALL | XALL }
                Interrupts statistics
        -m { <keyword> [,...] | ALL }
                Power management statistics
                Keywords are:
                CPU     CPU instantaneous clock frequency
                FAN     Fans speed
                FREQ    CPU average clock frequency
                IN      Voltage inputs
                TEMP    Devices temperature
                USB     USB devices plugged into the system
        -n { <keyword> [,...] | ALL }
                Network statistics
                Keywords are:
                DEV     Network interfaces
                EDEV    Network interfaces (errors)
                NFS     NFS client
                NFSD    NFS server
                SOCK    Sockets (v4)
                IP      IP traffic      (v4)
                EIP     IP traffic      (v4) (errors)
                ICMP    ICMP traffic    (v4)
                EICMP   ICMP traffic    (v4) (errors)
                TCP     TCP traffic     (v4)
                ETCP    TCP traffic     (v4) (errors)
                UDP     UDP traffic     (v4)
                SOCK6   Sockets (v6)
                IP6     IP traffic      (v6)
                EIP6    IP traffic      (v6) (errors)
                ICMP6   ICMP traffic    (v6)
                EICMP6  ICMP traffic    (v6) (errors)
                UDP6    UDP traffic     (v6)
        -q      Queue length and load average statistics
        -r      Memory utilization statistics
        -R      Memory statistics
        -S      Swap space utilization statistics
        -u [ ALL ]
                CPU utilization statistics
        -v      Kernel table statistics
        -w      Task creation and system switching statistics
        -W      Swapping statistics
        -y      TTY device statistics
```
<a name="w42Gs"></a>
## `sar -u`：CPU监控
```bash
[root@iZuligp6e1dyzfZ ~]# sar -u 10 3
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      07/31/2020      _x86_64_        (1 CPU)

02:41:12 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
02:41:23 PM     all     60.87      0.00     39.13      0.00      0.00      0.00
02:41:33 PM     all     60.87      0.00     39.13      0.00      0.00      0.00
02:41:43 PM     all     51.85      0.00     48.15      0.00      0.00      0.00
Average:        all     57.53      0.00     42.47      0.00      0.00      0.00
```
<a name="t5TxM"></a>
## `sar -r`：内存监控
```bash
[root@iZuligp6e1dyzfZ ~]# sar -r 10 3
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      07/31/2020      _x86_64_        (1 CPU)

02:51:26 PM kbmemfree kbmemused  %memused kbbuffers  kbcached  kbcommit   %commit  kbactive   kbinact   kbdirty
02:51:36 PM     69672   1814052     96.30    148648    879688   1478656     78.50   1242540    385528       152
02:51:46 PM     69556   1814168     96.31    148648    879704   1478656     78.50   1242592    385528       128
02:51:56 PM     69284   1814440     96.32    148648    879728   1478656     78.50   1242892    385520       168
Average:        69504   1814220     96.31    148648    879707   1478656     78.50   1242675    385525       149
```
<a name="crVX4"></a>
## `sar -b`：I/O监控
```bash
[root@iZuligp6e1dyzfZ ~]# sar -b 10 3
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      07/31/2020      _x86_64_        (1 CPU)

02:54:17 PM       tps      rtps      wtps   bread/s   bwrtn/s
02:54:27 PM    111.76      0.00    111.76      0.00   4800.00
02:54:37 PM    175.00      0.00    175.00      0.00   6114.29
02:54:47 PM    190.48      0.00    190.48      0.00   7314.29
Average:       153.01      0.00    153.01      0.00   5879.52
```
<a name="4HsAp"></a>
## `sar -w`：系统SWAP监控
```bash
[root@iZuligp6e1dyzfZ ~]# sar -w 10 3
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      07/31/2020      _x86_64_        (1 CPU)

02:55:06 PM    proc/s   cswch/s
02:55:16 PM     46.15  61788.46
02:55:26 PM     58.33  66933.33
02:55:36 PM     58.33  67225.00
Average:        54.05  65220.27
```
