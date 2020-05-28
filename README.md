## topost
topost display client for all ost on a lustre oss and sort them by read, write and iops usage<br>
It parse /proc/fs/lustre .... and read stats 2 times , calculate the bandwith used by each client, sort the client by usage<br>
and print them.<br>
It has been tested on redhat 7.4 and lustre 2.7<br>
It use /etc/topost.conf to convert nid to hostname.<br>
/etc/topost.conf must contain each lustre client and begin with word client.<br>
Example :
<pre>
[root@oss0 ~]# cat /etc/topost.conf
client=client1,123.45.31.2@o2ib1
client=client2,123.45.31.3@o2ib1
</pre>

output example :
<pre>
[root@oss0 ~]# topost
10 clients max to display
4 ost
97 clients in /etc/topost.conf
First read
Wait 10s before to read a second time
Second read
--------------------------------------------------------------------------
Read
OST             CLIENT  MB_read         IOPS
--------------------------------------------------------------------------


Write
OST             CLIENT  MB_write        IOPS
--------------------------------------------------------------------------
lustrefs-OST0000 client111        134     145
lustrefs-OST0002 client111        105     128
lustrefs-OST0001 client111        72      77
lustrefs-OST0003 client111        42      47
lustrefs-OST0000  client73        4       5
lustrefs-OST0002  client73        3       7
lustrefs-OST0003  client21        1       1


IOPS
OST             CLIENT  MB_read         MB_write        IOPS
--------------------------------------------------------------------------
lustrefs-OST0000 client111        0       134             145
lustrefs-OST0002 client111        0       105             128
lustrefs-OST0001 client111        0       72              77
lustrefs-OST0003 client111        0       42              47
lustrefs-OST0002  client73        0       3               7

Load : 2.63 2.21 1.71

Pendant l'intervale de 10s :
--------------------------------------------------------------------------
NB CLIENT EN ACTIVITE =         12
TOTAL MB READ  =                0
TOTAL MB WRITE =                361
TOTAL IOPS READ =               0
TOTAL IOPS WRITE =              410
</pre>
