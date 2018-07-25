#include <math.h>
#include "SqlDeal.h"
SqlDeal::SqlDeal()//:log("data.log"),out(&log)
{
	//log.open(QFile::ReadWrite);
	//out.setCodec("UTF-8");
	//out<<QDateTime::currentDateTime().toString(Qt::ISODate)<<":start"<<endl;
	db = QSqlDatabase::addDatabase("QSQLITE","test");
	db.setDatabaseName("test.db");
	if (!db.open())
	{
		//out << QDateTime::currentDateTime().toString(Qt::ISODate)<<":can't connect database"<<endl;
	}
	//out << QDateTime::currentDateTime().toString(Qt::ISODate)<<":connect database"<<endl;

}

SqlDeal::~SqlDeal(void)
{
	//log.close();
}
void SqlDeal::setTracerouteFile(QString&file)
{
	filename = file;
}
void SqlDeal::dealTracertFile()
{
	QFile tracerouteFile(filename);
	tracerouteFile.open(QFile::ReadOnly);
	QTextStream traceroute(&tracerouteFile);
	QString line = traceroute.readLine();
	QList<QString> path;
	QFile kml(filename+".kml");
	kml.open(QFile::WriteOnly);
	QTextStream gene_kml(&kml);
	gene_kml<<"<Document>"<<endl;
	gene_kml<<"<Style id=\"color_line\"><LineStyle><color>ff0000ff</color><width>7</width></LineStyle></Style>"<<endl;
	gene_kml<<"<Style id=\"icon_style\"><IconStyle><scale>1</scale><Icon><href>juanjo_Router.png</href></Icon></IconStyle></Style>"<<endl;
	gene_kml<<"<Placemark><styleUrl>#icon_style</styleUrl><TimeSpan><begin>2009-04-30T23:00:00Z</begin></TimeSpan></Placemark>"<<endl;
	while (!traceroute.atEnd())
	{
		if (line.split(" ")[0] == "traceroute")
		{
			line = traceroute.readLine();
			path.clear();
			while(!traceroute.atEnd() && line.split(" ")[0]!="traceroute")
			{
				line = line.replace(" ","");
				QString ipString ;
				int start=0,end=0;
				for (int i=0;i<line.size();++i)
				{
					if (line[i] == '(')
						start = i+1;
					else if (line[i] == ')')
					{
						end = i-1;
						break;
					}
				}
				if (start!=end)
				{
					ipString = line.mid(start,end-start+1);
					path.append(ipString);
				}
				line = traceroute.readLine();
			}
			if (path.size()>2)
			{
				int date=1,hour=0;//确定时间和日期
				std::string ss = std::string(path[0].toLocal8Bit().data());
				QList<QString> last = findIP(ss);
				for (int i=1;i<path.size();++i)
				{
					date = 1+i/24;
					hour = i%24;
					ss = std::string(path[i].toLocal8Bit().data());
					QList<QString> location = findIP(ss);
					if (last[0]==""||last[1]==""||location[0]==""||location[1]=="")
					{
						last=location;
						continue;
					}
					gene_kml<<"<Placemark><styleUrl>#color_line</styleUrl>"<<endl;
					gene_kml<<"	<TimeSpan><begin>2009-05-0";
					gene_kml<<date<<"T";
					if (hour<10)
						gene_kml<<"0"<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
					else
						gene_kml<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
					gene_kml<<"<LineString><extrude>0</extrude><tessellate>1</tessellate><altitudeMode>relativeToGround</altitudeMode><coordinates>";
					double dis = distance(last,location);
					int section = dis/900;
					double lineStartlo =last[0].toDouble(),lineStartla = last[1].toDouble();
					double lineEndlo = location[0].toDouble(),lineEndla = location[1].toDouble();
					if (section==0)
					{
						gene_kml<<last[0]<<","<<last[1]<<","<<"40000 ";
						gene_kml<<location[0]<<","<<location[1]<<","<<"40000 ";
					}
					else
					{
						double longi = (lineStartlo-lineEndlo)/section;
						double latit = (lineStartla-lineEndla)/section;
						for(int ii = 0;ii<=section;++ii)
						{
							gene_kml<<(last[0].toDouble()-longi*ii)<<","<<(last[1].toDouble()-latit*ii)<<","<<"40000 ";
						}
						gene_kml<<location[0]<<","<<location[1]<<","<<"40000 ";
					}
					gene_kml<<"</coordinates></LineString></Placemark>"<<endl;

					gene_kml<<"<Placemark><styleUrl>#icon_style</styleUrl>"<<endl;
					gene_kml<<"	<TimeSpan><begin>2009-05-0";
					gene_kml<<date<<"T";
					if (hour<10)
						gene_kml<<"0"<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
					else
						gene_kml<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
					gene_kml<<"	<Point><altitudeMode>relativeToGround</altitudeMode><coordinates>";
					gene_kml<<last[0]<<","<<last[1]<<","<<"40000 "<<"</coordinates></Point></Placemark>"<<endl;
					last=location;
				}
				gene_kml<<"<Placemark><styleUrl>#icon_style</styleUrl>"<<endl;
				gene_kml<<"	<TimeSpan><begin>2009-05-0";
				gene_kml<<date<<"T";
				if (hour<10)
					gene_kml<<"0"<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
				else
					gene_kml<<hour<<":00:00Z</begin></TimeSpan>"<<endl;
				gene_kml<<"	<Point><altitudeMode>relativeToGround</altitudeMode><coordinates>";
					gene_kml<<last[0]<<","<<last[1]<<","<<"40000 "<<"</coordinates></Point></Placemark>"<<endl;
			}
		}
	}
	gene_kml<<"</Document>"<<endl;
	tracerouteFile.close();
}
QList<QString> SqlDeal::findIP(std::string ip)
{
	//QSqlDatabase db = QSqlDatabase::database("test"); //建立数据库连接
	QSqlQuery query(db);
	query.exec(QString("select longitude ,latitude from location where %1 between minip and maxip").arg(ipTransform(ip)));
	query.next();
	QList<QString> ans;
	ans.append(query.value(0).toString());
	ans.append(query.value(1).toString());
	return ans;
}
unsigned SqlDeal::ipTransform(std::string ip)
{
	char *input=(char*)ip.c_str();
	char* temp = NULL;
	int ipArray[4];
	int i = 0;
	temp = strtok(input,".");
	while(i<4)
	{
		ipArray[i]=atoi(temp);
		temp=strtok(NULL,".");
		i++;
	}
	unsigned returnNum = ipArray[0]*256*256*256+ipArray[1]*256*256+ipArray[2]*256+ipArray[3];
	return returnNum;
}
double SqlDeal::distance(QList<QString>&start_p,QList<QString>&end_p)
{
	double pi = 3.141592653;
	double wB = start_p[1].toDouble()/180*pi;
	double jB = start_p[0].toDouble()/180*pi;
	double wA = end_p[1].toDouble()/180*pi;
	double jA = end_p[0].toDouble()/180*pi;
	double distan = 6371*acos(sin(wA)*sin(wB)+cos(wA)*cos(wB)*cos(jA-jB));
	return distan;
}