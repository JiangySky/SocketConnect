SocketConnect
=============
开发语言：C++

传输协议：字节流 / JSON

结构：https://github.com/JiangySky/SocketConnect/blob/master/README.rtfd/Pasted%20Graphic.tiff?raw=true

======================================

启动连接：
	#include "AppConnect.h"

	AppConnect::client()->startServer(dstIP, dstPort);
	// AppConnect::client()->startServer();

======================================

添加解析包：
	直接添加安装好的Packet模板文件，定义相关变量，改写相应 write 和 read 即可。
	
	同时需要修改如下几处：
	  @required
		PacketAll.h 中包含添加的头文件
		ConnectDefine.h 中定义相关Id
		PacketManager::getPacket(uint) 中添加相应case
	  @optional
		AppClient 中添加对请求的处理以及响应函数
		AppClient ::onResponse(Packet *)中添加相应case
		AppServer 中添加对请求的数据封装函数
		AppServer ::sendRequest(Packet *)中添加相应case
		(AppServer封装可能需要创建临时数据对象，也可直接调用相应sendPacket)

	PS: Packet模板文件的安装
		将 Packet class.xctemplate 文件夹复制到Xcode模板路径下即可
		e.g.   "/Users/Jiangy/Library/Developer/Xcode/Templates"
		跳转方法：Xcode --> command + , --> Locations选项卡 --> Locations表单 --> 点 DerivedData 后面的小箭头

