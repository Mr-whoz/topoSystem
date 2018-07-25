#ifndef ADDNODE_H
#define ADDNODE_H

#include<QtGui>
class AddNode : public QDialog
{
	Q_OBJECT
public:
	AddNode(QWidget *parent = 0,QTreeWidget* tree=NULL,QString config="./Resources/server_config.xml");
	~AddNode();
	private slots:
		void addNodeConfirm();
private:
	QLabel *label_ip;
	QLabel *label_port;
	QLabel *label_name;
	QLineEdit *text_ip;
	QLineEdit *text_port;
	QLineEdit *text_name;
	QPushButton *confirm;
	QTreeWidget *treeWidget;
	QString file;
};
#endif