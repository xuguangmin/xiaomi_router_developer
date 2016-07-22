#小米路由器二次开发项目

#项目背景:

>本人在购买了小米路由器和小兴看看(小蚁摄像机太烂，转手卖了)摄像机后,利用小兴看看的NAS功能和小米路由器支持USB的功能，
将小兴看看的视频文件实时传输到小米路由器下面的硬盘中。但是由于小兴看看硬件设备不带嵌入电源，所以每次重新启动后时间
都会穿越到１９７０年，并且，由于小兴看看的移动客户端需要联网支持，使用起来非常蹩脚，因此决定对小米路由器进行二次开发
，使二者达到完美兼容。

#项目目标:

>１ 实现视频文件及文件名称的时间正确显示,支持管理多个摄像机

>２ 研究在小米路由器上实现raid0，以使不限于单块硬盘存储

>３ 实现视频服务器，支持多个客户端同时访问服务器上的视频

>４ 实现安卓端的视频播放器

#第三方库:

>lw_oopc:是一套輕量級的面向對象C語言編程框架，它是一套C語言的宏。
>
>SQLite:是一款轻型的数据库，它的设计目标是嵌入式的，而且目前已经在很多嵌入式产品中使用了它，它占用资源非常的低。
