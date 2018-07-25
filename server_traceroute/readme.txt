服务端 server.py :
    监听端口 27015 等待连接, 接收文件保存为 host.txt , 执行 tracert.py 按行读 
  host.txt 调用系统命令 traceroute 返回保存为 result.txt, 发送文件 result.txt.
  
客户端 Client.exe 和 client.py:
  命令格式: Client.exe <服务器地址> <发送文件> <接收文件[可选]>
    Client.exe 127.0.0.1 host.txt result.txt
	
	Client.c 运行 Client.bat 进行编译, 需要安装 Microsoft Visual Studio 开发
  工具, 修改 Client.bat 文件为你的安装位置.
  
  
host.txt 示例:
  61.139.2.69
  www.baidu.com
  
服务器端的运行,要转为后台运行:
nohup python -u ./server.py > server.log 2>&1 &

记得windows下he linux下换行符不一致问题 否则traceroute会出错