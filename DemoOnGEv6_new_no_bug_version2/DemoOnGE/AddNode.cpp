#include "AddNode.h"
#include "XmlStreamWriter.h"
AddNode::AddNode(QWidget *parent,QTreeWidget *tree,QString config):QDialog(parent)
{
	file = config;
	treeWidget = tree;
	label_ip = new QLabel(tr("IP :"));
	text_ip = new QLineEdit(tr("127.0.0.1"));
	//QRegExp regExp("((?:(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d))))");
	//text_ip->setValidator(new QRegExpValidator(regExp, this));
	label_ip->setBuddy(text_ip);

	label_port = new QLabel(tr("port :"));
	text_port = new QLineEdit(tr("27015"));
	label_port->setBuddy(text_port);

	label_name = new QLabel(tr("name :"));
	text_name = new QLineEdit(tr("node on Tokyo"));
	label_name->setBuddy(text_name);

	confirm = new QPushButton(tr("OK"));
	connect(confirm,SIGNAL(clicked()),this,SLOT(addNodeConfirm()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(label_ip,0,0,1,1);
	mainLayout->addWidget(text_ip,0,1,1,2);
	mainLayout->addWidget(label_port,1,0,1,1);
	mainLayout->addWidget(text_port,1,1,1,2);
	mainLayout->addWidget(label_name,2,0,1,1);
	mainLayout->addWidget(text_name,2,1,1,2);
	mainLayout->addWidget(confirm,3,2,1,1);
	setLayout(mainLayout);
	setWindowTitle(tr("add node"));
	setAttribute(Qt::WA_DeleteOnClose);
}

AddNode::~AddNode(void)
{
}
void AddNode::addNodeConfirm()
{
	QTreeWidgetItem *parent = new QTreeWidgetItem(treeWidget,QStringList(text_name->text()));
	parent->setCheckState(0,Qt::Unchecked);
	QTreeWidgetItem *child_ip = new QTreeWidgetItem(parent,QStringList("IP : "+text_ip->text()));
	QTreeWidgetItem *child_port = new QTreeWidgetItem(parent,QStringList("port : "+text_port->text()));
	QTreeWidgetItem *child_name = new QTreeWidgetItem(parent,QStringList("name : "+text_name->text()));

	XmlStreamWriter writer;
	writer.writeXml(file,this->treeWidget);
}
