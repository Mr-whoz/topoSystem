#ifndef THREAD_H
#define THREAD_H

#include <QtGui>

class Thread :public QThread
{
	Q_OBJECT
public:
	Thread();
	~Thread();
	void setTree(QTreeWidget*p,QString);
protected:
	void run();
private:
	QTreeWidget* pa;
	QString IP_file;
};

#endif
