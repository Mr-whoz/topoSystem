#ifndef QTTEST_H
#define QTTEST_H

#include <QtXml>
#include <QtGui>

class XmlStreamReader
{
	//Q_OBJECT

public:
	XmlStreamReader(QTreeWidget *tree);
	bool readFile(const QString &fileName);

private:
	void readServerList();
	void readserver(QTreeWidgetItem *parent);
	void readIP(QTreeWidgetItem *parent);
	void readPort(QTreeWidgetItem *parent);
	void readName(QTreeWidgetItem *parent);
	void skipUnknowElement();
	QTreeWidget *treeWidget;
	QXmlStreamReader reader;
};

#endif // QTTEST_H
