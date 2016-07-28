#小米路由器二次开发项目
		//此文档既是项目介绍也作为开发文档使用
------

#项目背景:

> 本人在购买了小米路由器和小兴看看(小蚁摄像机太烂，转手卖了)摄像机后,利用小兴看看的NAS功能和小米路由器支持USB的功能，将小兴看看的视频文件实时传输到小米路由器下面的硬盘中。但是由于小兴看看硬件设备不带嵌入电源，所以每次重新启动后时间都会穿越到１９７０年，并且，由于小兴看看以及已知的智能摄像机的移动客户端都需要联网支持，使用起来非常蹩脚，因此决定
对小米路由器进行二次开发，使之支持在局域网内也能方便使用。

#项目简介:
 - 开发语言: C + java
 - 开发平台: ubuntu 16.04
 - 目标平台: ARM + Linux
 - 产品名称: 小米路由器mini
 - 开始时间: 2016-07-17
 - 预结束时间: 2017-02
 - 开发人数: 1

#项目目标:
 1. 实现视频文件及文件名称的时间正确显示
 2. 支持管理多个摄像机
 3. 研究在小米路由器上实现raid0，以使不限于单块硬盘存储
 4. 实现视频服务器，支持多个客户端同时访问服务器上的视频
 5. 实现安卓端的视频播放器，支持网络实时预览

#源文件介绍:
 - cx16-vec.[ch]
 - lw_oopc.[ch]
 - xiaomi_obj_year.[ch]
 - xiaoxing_media.[ch]
 - xiaoxing_media.[ch]
 - xiaoxing.[ch]
 - timeStamp.c
 - proto.h

#实现原理:
>通过软件抓取小兴看看智能摄像机的网络通讯数据包分析发现，摄像机的时间同步方式使用的是NTP协议，目标服务器是微软的时间服务器，理论上只要在路由器的DNS服务器上将所有对微软时间服务器的DNS解析请求解析为本地ＩＰ地址，即可建立摄像机和本网络内的时间服务器的网络链接，为像机提供时间服务，因此需要在本地网络内部搭建一个时间服务器。为了尽可能降低硬件成本，时间服务器计划搭建在路由器内部，小米路由器支持二次开始，可以满足以上需求。由于路由器内部没有备用电源，路由器重启之后，其内部的时间将会恢复到默认开始时间(1970年),因此，为了使路由器保持正确的时间，同时为了保证手机与网络断开之后系统能保持正确的时间，安卓客户端需要为路由器上的NTP时间服务器提供时间同步服务。

#相关知识点:

**NTP协议**
>NTP，即网络时间协议。NTP用时间戳表示为一64 bits unsigned,定点数，以秒的形式从1900年1月1日00：00：00算起。整数部分在前32位里，后32bits（seconds Fraction）用以表示秒以下的部分。在Seconds Fraction 部分，无意义的低位应该设置为0。

      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                           Seconds                             |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                  Seconds Fraction (0-padded)                  |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

NTP 报文格式
>NTP 和SNTP 是用户数据报协议( UDP) 的客户端。端口是123。ＮＴＰ在网络通讯模型中属于应用层协议，和ＨＴＴＰ协议类似。NTP 消息的报文格式可按如下方式定义:

		typedef struct ntp_msg_t
		{
			int8_t LI_VN_Mode;
			int8_t stratum;
			int8_t poll;
			int8_t precision;
		
			int32_t rootdelay;	/* 根延迟 */
			int32_t rootdispersion;	/* 根差量  */
			int32_t refreference ID;/* 参考标识符 */
				
			uint64_t reftime;	/* 参考时间戳 */
			uint64_t origtime;
			uint64_t recetime;
			uint64_t transtime;
		}ntp_msg_t;


#第三方库:

####[lw_oopc](#):<br /> 
	是一套轻量级的面向对象C语言編程框架，它是一套C語言的宏。此处用来将面向对象的思想用在C语言中

####[SQLite](#):<br />
	是一款轻型的数据库，它的设计目标是嵌入式的，目前已经在很多嵌入式产品中使用，它占用资源非常的低。在本项目中用来管理存储服务器中
	的文件名称

#遇到的问题:<br />

 - [已解决] 从NTP客户端接收到时间同步请求后，程序解析出来的客户端发送程序比实际时间多了整７０年
    - 原因:NTP协议时间戳计时从1970-01-01 00:00:00开始，而Linux对时间戳的计时从1970-01-01 00:00:00开始。
    - 解决办法:时间戳减去1900-01-01 00:00:00至1970-01-01 00:00:00之间的秒数

#可能遇到的问题:
 - [未开始] 项目中需要用到音视频编解码。


------
持续更新中。。。
	
联系方式:[xxgmin@163.com][1]

 [1]: http://mail.163.com/
