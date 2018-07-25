#include "XmlStreamWriter.h"

XmlStreamWriter::XmlStreamWriter()
{
}

XmlStreamWriter::~XmlStreamWriter()
{
}
bool XmlStreamWriter::writeXml(const QString &fileName,QTreeWidget *treeWidget)
{
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly))
	{
		return false;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("serverlist");
	for (int i=0;i<treeWidget->topLevelItemCount();++i)
		writeServer(&xmlWriter,treeWidget->topLevelItem(i));
	xmlWriter.writeEndElement();
	file.close();
	return true;
}
void XmlStreamWriter::writeServer(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item)
{
	xmlWriter->writeStartElement("server");
	writeIP(xmlWriter,item->child(0));
	writePort(xmlWriter,item->child(1));
	writeName(xmlWriter,item->child(2));
	xmlWriter->writeEndElement();
}

void XmlStreamWriter::writeIP(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item)
{
	xmlWriter->writeStartElement("IP");
	QString text=item->text(0);
	xmlWriter->writeCharacters(text.split(" : ")[1]);
	xmlWriter->writeEndElement();
}
void XmlStreamWriter::writePort(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item)
{
	xmlWriter->writeStartElement("port");
	QString text=item->text(0);
	xmlWriter->writeCharacters(text.split(" : ")[1]);
	xmlWriter->writeEndElement();
}

void XmlStreamWriter::writeName(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item)
{
	xmlWriter->writeStartElement("name");
	QString text=item->text(0);
	xmlWriter->writeCharacters(text.split(" : ")[1]);
	xmlWriter->writeEndElement();
}