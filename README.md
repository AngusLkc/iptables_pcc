模块说明
===
此项目是iptables连接哈希取余匹配模块，类似于Mikrotik RouterOS的PCC功能。本项目能基于源地址、目标地址、源端口、目标端口进行匹配。
OpenWRT下编译可直接下载本项目，然后解压pcc文件夹到OpenWRT编译工作的Package目录内，然后重新执行make menuconfig即可。

模块应用：
===
*对每条连接的第一个包打上标签，并把标签值应用到同一连接会话的所有包中。
然后对含有特定标签值的包设置不同的出口网关*
```Shell
iptables -t mangle -N PCCLOAD
iptables -t mangle -A PCCLOAD -m pcc --src-addr --dst-addr --src-port --dst-port --pcc-mod 4 --pcc-value 0 -j CONNMARK --set-mark 1
iptables -t mangle -A PCCLOAD -m pcc --src-addr --dst-addr --src-port --dst-port --pcc-mod 4 --pcc-value 1 -j CONNMARK --set-mark 2
iptables -t mangle -A PCCLOAD -m pcc --src-addr --dst-addr --src-port --dst-port --pcc-mod 4 --pcc-value 2 -j CONNMARK --set-mark 3
iptables -t mangle -A PCCLOAD -m pcc --src-addr --dst-addr --src-port --dst-port --pcc-mod 4 --pcc-value 3 -j CONNMARK --set-mark 4
iptables -t mangle -A PCCLOAD -j CONNMARK --save-mark

iptables -t mangle -N RESTORE
iptables -t mangle -A RESTORE -m conntrack --ctstate ESTABLISHED,RELATED -j RESTORE
iptables -t mangle -A RESTORE -m conntrack --ctstate NEW -j PCCLOAD

iptables -t mangle -A PREROUTING -s 192.168.0.0/24 -j RESTORE

cat /etc/iproute2/rt_tables 
255    local
254    main
253    default
0      unspec
252    wan1
251    wan2
250    wan3
249    wan4

ip route flush table wan1
ip route flush table wan2
ip route flush table wan3
ip route flush table wan4
ip route add table wan1 default via 192.168.1.1 dev ppp0
ip route add table wan2 default via 192.168.2.1 dev wlan1
ip route add table wan3 default via 192.168.1.1 dev wlan2
ip route add table wan4 default via 192.168.4.1 dev ppp1

ip rule del from all fwmark 1
ip rule del from all fwmark 2
ip rule del from all fwmark 3
ip rule del from all fwmark 4
ip rule add fwmark 1 table wan1
ip rule add fwmark 2 table wan2
ip rule add fwmark 3 table wan3
ip rule add fwmark 4 table wan4
ip route flush cache
```
