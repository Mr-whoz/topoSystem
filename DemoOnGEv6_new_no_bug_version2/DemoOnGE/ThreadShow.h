#ifndef THREADSHOW_H
#define THREADSHOW_H
#include <QtGui>
#include "SqlDeal.h"

class ThreadShow :public QThread
{
	Q_OBJECT
public:
	ThreadShow();
	~ThreadShow(void);
	void setFile(QString&);
protected:
	void run();
private:
	SqlDeal deal;
};

#endif