## topost
topost display lustre client and sort them by Read MB/s, Write MB/s and IOPS<br>
It use /etc/topost.conf to convert nid to hostname.<br>
/etc/topost.conf must contain each lustre client with it nid.<br>
Example :
<pre>
[root@oss0 ~]# cat /etc/topost.conf
client=pangea01,123.45.31.2@o2ib1
client=pangea02,123.45.31.3@o2ib1
....
</pre>

output example :
<pre>
[root@oss0 ~]# topost
10 clients max to display
6 ost
97 clients in /etc/topost.conf
First read
Wait 10s before to read a second time
Second read
--------------------------------------------------------------------------
Read
OST             CLIENT  		MB_read         IOPS
--------------------------------------------------------------------------
scratch-OST0002 pangealogin2     31      		31


Write
OST             CLIENT  		MB_write        IOPS
--------------------------------------------------------------------------
scratch-OST0008  pangea89        534     		171
scratch-OST0009  pangea89        523     		155
scratch-OST0000  pangea81        473     		141
scratch-OST0009  pangea86        471     		119
scratch-OST0003  pangea81        420     		148
scratch-OST0008  pangea81        404     		121
scratch-OST0002  pangea86        364     		99
scratch-OST0000  pangea86        356     		90
scratch-OST0009  pangea81        296     		85
scratch-OST0003  pangea89        274     		93


IOPS
OST             CLIENT  	MB_read     MB_write        IOPS
--------------------------------------------------------------------------
scratch-OST0008  pangea89        0       534             171
scratch-OST0009  pangea89        0       523             155
scratch-OST0003  pangea81        0       420             148
scratch-OST0000  pangea81        0       473             141
scratch-OST0008  pangea81        0       404             121
scratch-OST0009  pangea86        0       471             119
scratch-OST0002  pangea86        0       364             99
scratch-OST0003  pangea89        0       274             93
scratch-OST0000  pangea86        0       356             90
scratch-OST0009  pangea81        0       296             85

Load : 6.03 4.83 4.44

Pendant l'intervale de 10s :
--------------------------------------------------------------------------
NB CLIENT EN ACTIVITE =         21
TOTAL MB READ  =                31
TOTAL MB WRITE =                4115
TOTAL IOPS READ =               31
TOTAL IOPS WRITE =              1222
</pre>
