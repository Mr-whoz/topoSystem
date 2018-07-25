/********************************************************************************
** 服务器配置界面。
**
** 
**  
**
** 
********************************************************************************/
#ifndef SERVERSET_H
#define SERVERSET_H

#include<QtGui/QDialog>
#include "XmlStreamReader.h"
#include "AddNode.h"
#include "Thread.h"
class ServerSet : public QDialog
{
	Q_OBJECT
public:
	ServerSet(QWidget *parent ,Thread*);
	~ServerSet();
private slots:
	void addClicked();
	void deleteClicked();
	void traceClicked();
protected:
	void closeEvent(QCloseEvent *event);
private:
	Thread *trace;
	QPushButton *button_add;
	QPushButton *button_delete;
	QPushButton *button_apply;
	QPushButton *button_exit;
	QTreeWidget *treeWidget;
};
#endif