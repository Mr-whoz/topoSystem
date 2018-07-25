#ifndef XMLSTREAMWRITER_H
#define XMLSTREAMWRITER_H

#include <QtGui>
#include <QtXml>

class XmlStreamWriter
{
public:
	XmlStreamWriter();
	~XmlStreamWriter();
	bool writeXml(const QString &fileName,QTreeWidget *treeWidget);
private:
	void writeServer(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);
	void writeIP(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);
	void writePort(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);
	void writeName(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);
};

#endif