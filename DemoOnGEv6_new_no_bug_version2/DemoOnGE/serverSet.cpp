#include<QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <string>
#include <QtGui>
#include "serverSet.h"
#include "AddNode.h"
#include "Client.h"
#include <iostream>
#include "XmlStreamWriter.h"
ServerSet::ServerSet(QWidget *parent,Thread*pt):QDialog(parent)
{
	trace=pt;

	button_add = new QPushButton(tr("添加节点"),this);
	connect(button_add,SIGNAL(clicked()),this,SLOT(addClicked()));
	button_add->setMaximumSize(100,25);
	button_delete = new QPushButton(tr("删除节点"),this);
	connect(button_delete,SIGNAL(clicked()),this,SLOT(deleteClicked()));
	button_add->setMaximumSize(100,25);

	button_apply = new QPushButton(tr("采集"),this);
	connect(button_apply,SIGNAL(clicked()),this,SLOT(traceClicked()));
	button_add->setMaximumSize(100,25);
	button_exit = new QPushButton(tr("退出"),this);

	treeWidget = new QTreeWidget(this);
	treeWidget->setHeaderLabel ( "server" );
	treeWidget->setMaximumSize(300,380);
	QString file= "./Resources/server_config.xml";
	XmlStreamReader reader(treeWidget);
	reader.readFile(file);

	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(button_add);
	rightLayout->addWidget(button_delete);
	rightLayout->addWidget(button_apply);
	rightLayout->addWidget(button_exit);
	rightLayout->addStretch();
	QHBoxLayout * leftLayout = new QHBoxLayout;
	leftLayout->addWidget(treeWidget);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	setMinimumSize(600,400);
	setMaximumSize(600,400);
	setLayout(mainLayout);
	setWindowTitle(tr("服务器设置"));
	//setAttribute(Qt::WA_DeleteOnClose);
}
ServerSet::~ServerSet()
{
}
void ServerSet::closeEvent(QCloseEvent *event)
{
	this->hide();
	/*if (trace->isRunning())
	{
		QMessageBox::information(this, tr("提示"),
			QString(tr("正在进行traceroute，请勿关闭此对话框")));
		event->ignore();
	}
	else
		event->accept();*/
}
void ServerSet::addClicked()
{
	AddNode *diag = new AddNode(this,treeWidget);
	diag->show();
}
void ServerSet::deleteClicked()
{
	for(int i=0;i<treeWidget->topLevelItemCount();++i)
	{
		QTreeWidgetItem* item = treeWidget->topLevelItem(i);
		if (item->checkState(0)==Qt::Checked)
		{
			treeWidget->takeTopLevelItem(i);
			--i;
		}
	}
	//更新book.xml
	XmlStreamWriter writer;
	writer.writeXml("book.xml",this->treeWidget);
}

void ServerSet::traceClicked()
{
	QString IP_file = QFileDialog::getOpenFileName(this, tr("打开IP文件"),
		"",tr("KML (*.txt)")); 
	trace->setTree(treeWidget,IP_file);
	trace->start();
}