#include "XmlStreamReader.h"

XmlStreamReader::XmlStreamReader(QTreeWidget *tree)
{
	treeWidget = tree;
}

bool XmlStreamReader::readFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly|QFile::Text))
	{
		return false;
	}
	reader.setDevice(&file);
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			if (reader.name() == "serverlist")
			{
				readServerList();
			}
			else
			{
				reader.raiseError(QObject::tr("not a serverlist file"));
			}
		}
		else
			reader.readNext();
	}
	file.close();
	return true;
}

void XmlStreamReader::readServerList()
{
	reader.readNext();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name() == "server")
			{
				readserver(treeWidget->invisibleRootItem());
			}
			else
			{
				skipUnknowElement();
			}
		}
		else
		{
			reader.readNext();
		}
	}
}

void XmlStreamReader::readserver(QTreeWidgetItem *parent)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(parent);
	item->setCheckState(0,Qt::Unchecked);

	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name() == "IP")
			{
				readIP(item);
			}
			else if(reader.name() == "port")
			{
				readPort(item);
			}
			else if(reader.name() == "name")
			{
				readName(item);
			}
			else
			{
				skipUnknowElement();
			}
		}
		else
		{
			reader.readNext();
		}
	}
}

void XmlStreamReader::readIP(QTreeWidgetItem *parent)
{
	QString ipInfo=reader.name().toString();
	reader.readNext();
	QTreeWidgetItem *item = new QTreeWidgetItem(parent);
	item->setText(0,ipInfo+" : "+reader.text().toString());
	reader.readNext();
	reader.readNext();
}
void XmlStreamReader::readPort(QTreeWidgetItem *parent)
{
	QString portInfo=reader.name().toString();
	reader.readNext();
	QTreeWidgetItem *item = new QTreeWidgetItem(parent);
	item->setText(0,portInfo+" : "+reader.text().toString());
	reader.readNext();
	reader.readNext();
}
void XmlStreamReader::readName(QTreeWidgetItem *parent)
{
	QString nameInfo=reader.name().toString();
	reader.readNext();
	QTreeWidgetItem *item = new QTreeWidgetItem(parent);
	item->setText(0,nameInfo+" : "+reader.text().toString());
	parent->setText(0,reader.text().toString());
	reader.readNext();
	reader.readNext();
}
void XmlStreamReader::skipUnknowElement()
{
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			skipUnknowElement();
		}
		else
		{
			reader.readNext();
		}
	}
}