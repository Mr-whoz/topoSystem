#pragma once
#include <QtGui>
#include <QtSql>
#include <String>
class SqlDeal
{
public:
	SqlDeal();
	~SqlDeal(void);
	QList<QString> findIP(std::string);
	void dealTracertFile();
	void setTracerouteFile(QString&);
private:
	double distance(QList<QString>&start_p,QList<QString>&end_p);
	unsigned ipTransform(std::string ip);
	//QFile log;
	//QTextStream out;
	QString filename;
	QSqlDatabase db;
};
