#### 拓扑演示系统
1、演示系统全貌
![1](https://raw.githubusercontent.com/Mr-whoz/picture/master/%E6%BC%94%E7%A4%BA%E7%B3%BB%E7%BB%9F/%E6%BC%94%E7%A4%BA%E7%B3%BB%E7%BB%9F%E5%85%A8%E8%B2%8C.png)
2、服务器配置界面
![2](https://raw.githubusercontent.com/Mr-whoz/picture/master/%E6%BC%94%E7%A4%BA%E7%B3%BB%E7%BB%9F/%E6%9C%8D%E5%8A%A1%E5%99%A8%E8%AE%BE%E7%BD%AE.png)
********
简介
这个项目主要用QT二次开发了googleEarth。

因为要配置服务器，所以我用XML文件存储了服务器的配置信息，相应的，XmlStreamReader类用来读取XML文件的内容，并且生成一个QTreeWidget；XmlStreamWriter类用来将QTreeWidget上的信息写入到XML文件中存储下来。AddNode类可以向QTreeWidget添加item（即新的服务器），每当添加完成之后，会调用XmlStreamWriter将最新的服务器配置信息写入到XML文件中。

因为要通过服务器进行traceroute，我的思路是本地是客户端，将IP文件传到服务器，服务器完成traceroute之后，将结果传回客户端。Client类是用winsocket写的客户端，用于与服务器端（之后会介绍）交互。因为这一交互过程比较长，为了保持系统的即时性，我们把这一交互过程单独分配一个线程,即用Thread类包装了Client。

我们还要对服务器返回的结果进行处理，在处理时，我们需要查询每一个IP的地理位置，地理位置查询我存在了SQLITE中，有关SQLITE的操作封装在了SqlDeal类中，这个处理过程也要花很长时间，所以也给他单独分配了一个线程即ThreadShow类。

ServerSet类为图2所示的对话框，中间集成了服务器配置、采集功能。

整个软件的使用说明请看文档《演示系统操作文档》

服务器的配置在server_traceroute文件夹中，详细情况参见文件中的readme。

项目中还用到了openGl的库，需要把相应的文件下载下来拉入指定的文件夹中。