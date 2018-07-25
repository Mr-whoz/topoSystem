#include "Thread.h"
#include "Client.h"
#include "serverSet.h"

Thread::Thread()
{
}

Thread::~Thread()
{

}
void Thread::setTree(QTreeWidget*p,QString ip)
{
	pa=p;
	IP_file=ip;
}
void Thread::run()
{
	Client tracer;
	const char* hostName;
	const char*sendName;
	const char*recvName;
	for(int i=0;i<pa->topLevelItemCount();++i)
	{
		QTreeWidgetItem* item = pa->topLevelItem(i);
		if (item->checkState(0)==Qt::Checked)
		{
			QString temp = item->child(0)->text(0).split(" : ")[1];
			QByteArray ba = temp.toLocal8Bit();
			hostName =ba.data();
			QByteArray ipBarry = IP_file.toLocal8Bit();
			sendName = ipBarry.data();
			temp = ("./Resources/Recv_from_"+item->child(0)->text(0).split(" : ")[1]);
			QByteArray reBarry = temp.toLocal8Bit();
			recvName = reBarry.data();
			tracer.run(hostName,sendName,recvName);
		}
	}

}