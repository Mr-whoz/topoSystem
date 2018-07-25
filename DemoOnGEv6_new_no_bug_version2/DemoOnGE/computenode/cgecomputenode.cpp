#include <algorithm>
#include <QtGui>
#include "cgecomputenode.h"
#include<fstream>
Point point[1000];//the point over a line
double height[1000];//the height of a point over a line 
int placemark[1000];//check if a router has been drawn
int highLight[6][1000];
int mark[1000];//the mark during dfs process
string treeColor[7];//the 7 different colors of line
int topoRootIndex[6]={0,3,3,15,15,65};//the first root for each continent
int monthday[]={31,28,31,30,31,30,31,31,30,31,30,31};//the day numbers of each month
int edgeMark[600][600];
//int Index;

/******************************************
构造函数
*******************************************/

bool operator<(const Simtime &time1,const Simtime &time2)
{
	if(time1.year<time2.year)
		return true;
	else if(time1.year==time2.year&&time1.month<time2.month)
		return true;
	else if(time1.year==time2.year&&time1.month==time2.month&&time1.date<time2.date)
		return true;
	else if(time1.year==time2.year&&time1.month==time2.month&&time1.date==time2.date&&time1.hour<time2.hour)
		return true;
	return false;
}

Simtime operator+(const Simtime& originDate,const int& monthtoadd)
{
	Simtime newDate;
	newDate.hour=originDate.hour;
	newDate.date=originDate.date;
	newDate.month=originDate.month+monthtoadd;
	newDate.year=(newDate.month-1)/12+originDate.year;
	newDate.month=(newDate.month-1)%12+1;
	return newDate;
}

Simtime operator+(const Simtime& simtime1,const Simtime& simtime2)
{
	Simtime newSimtime;
	newSimtime.hour=simtime1.hour+simtime2.hour;
	newSimtime.date=simtime1.date+simtime2.date+newSimtime.hour/24;
	newSimtime.hour%=24;
	newSimtime.month=simtime1.month;
	while(newSimtime.date>monthday[(newSimtime.month-1)%12])
	{
		newSimtime.date-=monthday[(newSimtime.month-1)%12];
		newSimtime.month++;
	}	
	newSimtime.month+=simtime2.month;
	newSimtime.year=simtime1.year+simtime2.year+(newSimtime.month-1)/12;
	newSimtime.month=(newSimtime.month-1)%12+1;
	return newSimtime;
}

CGEComputeNode::CGEComputeNode()
{
	m_inPoints.clear();
	m_triangles.clear();
	m_allnodesindex.clear();
	m_inseaPoints.clear();

	m_headNodevec.clear();
	m_EdgeNodesVct.clear();
	m_EdgeNodesVct.resize(TREENUM);
	vct_Node.clear();

	m_pathIndex.clear();
	m_NodePathPoints.clear();
	m_TreePathPoints.clear();
	m_AllTreePathPoints.clear();

	m_publicMath = new PublicMath;
	m_headNode = NULL;
	memset(mark,0,sizeof(mark));
	//the time for the root point to start simulation
	treeColor[0]="00FF00";
	treeColor[1]="FF0000";
	treeColor[2]="0000FF";
	treeColor[3]="FF00FF";
	treeColor[4]="FFFF00";
	treeColor[5]="00FFFF";
    treeColor[6]="FFFFFF";

	ifastoas=false;
    //define the unseen AS
}

/******************************************
析构函数
*******************************************/
CGEComputeNode::~CGEComputeNode()
{
		
}

void CGEComputeNode::InitSimtime(Simtime &simtimetoinit,int hour,int date,int month,int year)
{
	simtimetoinit.hour=hour;
	simtimetoinit.date=date;
	simtimetoinit.month=month;
	simtimetoinit.year=year;
}

void CGEComputeNode::initialize()
{
	m_inPoints.clear();
	m_triangles.clear();
	m_allnodesindex.clear();
	
	m_headNodevec.clear();
	m_EdgeNodesVct.clear();
	m_EdgeNodesVct.resize(TREENUM);
	vct_Node.clear();

	m_pathIndex.clear();
	m_NodePathPoints.clear();
	m_TreePathPoints.clear();
	m_AllTreePathPoints.clear();
	cityNum=0;
	circle.clear();
	outCityIndex.clear();
	ifTerminalDisappear=false;
	if(continentId==2||continentId==5)
	{
		circlenum=1;
	}
	else
	{
		circlenum=0;
	}
}
Point CGEComputeNode::getCrossPoint(int nextcityindex,int prevcityindex,double radius)
{
	double cossita,sinsita,dist;int sigcos;
	dist=city[prevcityindex].l-city[nextcityindex].l;
	if(dist>=0&&dist<=180||dist<=-180)
	{
		sigcos=1;
	}
	else
	{
		sigcos=-1;
	}
    dist=fabs(dist);
	dist=dist*dist+(city[prevcityindex].m-city[nextcityindex].m)*(city[prevcityindex].m-city[nextcityindex].m);
	dist=sqrt(dist);
	sinsita=(city[prevcityindex].m-city[nextcityindex].m)/dist;
	cossita=sqrt(1-sinsita*sinsita);
	Point crosspoint;
    crosspoint.l=city[nextcityindex].l+radius*sigcos*cossita;
	crosspoint.m=city[nextcityindex].m+radius*sinsita;
	return crosspoint;
}

bool CGEComputeNode::InCircle(Point portPoint,Point cityPoint,const double radius)
{
	Circle tmpcircle;
	tmpcircle.center=cityPoint;
	tmpcircle.radius=radius;
	return incircle(tmpcircle,portPoint);
}

void CGEComputeNode::toporoute()
{
	traceroute=3;
	fout.open(outputfile.c_str());
	fout<<"<Document>"<<endl;
 
	memset(placemark,0,sizeof(placemark));
	endtime.date=1;
    endtime.month=1;
	endtime.year=2009;
	endtime.hour=0;
	if(continentId>0)
	{
		simtime=begintime;
	}
	else
	{
		simtime.date=1;
		simtime.month=1;
		simtime.year=2009;
		simtime.hour=0;
		begintime=simtime;
	}
	changeIcon("juanjo_Router.png",2);
	for(int i=0;i<(int)rootVector.size();i++)
	{
		drawPoint(rootVector[i]->index,simtime);
	}
	changeIcon("juanjo_Router.png",1);
	m_incirclePoints.clear();
	m_outcirclePoints.clear();
	memset(edgeMark,0,sizeof(edgeMark));
    memset(mark,0,sizeof(mark));

	for(int i=0;i<TREENUM;i++)
	{
	//	simtime=begintime;
		changeLineStyle(4,2);
        memset(mark,0,sizeof(mark));
		dfs(rootVector[i],simtime);
		simtime=endtime;
	}
	simtime=endtime;
	for(int i=0;i<(int)outCityIndex.size()-1;i++)
	{
		if(1==continentId&&1==i)
		{
		    drawPoint(outCityIndex[i]);//画出夏威夷这个点
		}
		drawLine(outCityIndex[i].l,outCityIndex[i].m,outCityIndex[i+1].l,outCityIndex[i+1].m,simtime);
	}
	endtime=simtime;
	fout<<"</Document>"<<endl;
	fout.close();
}
//Judge whether a point can be found during the process of traceroute
bool CGEComputeNode::InCircle(Point input)
{
	for(int i=0;i<circle.size();i++)
	{
		if(incircle(circle[i],input))
		  return true;
	}
	return false;
}
void CGEComputeNode::drawBag(int fatherindex,int childindex,Simtime &st)
{
    int sizenum=0;
	getPoint(city[fatherindex].l,city[fatherindex].m,city[childindex].l,city[childindex].m,sizenum);
	for(int i=0;i<sizenum;i++)
	{
		height[i]=abs(sizenum/2-abs(sizenum/2-i));
		height[i]=pow(height[i],1.0/3.0)*Height;
		height[sizenum-1]=0;
		if(fabs(point[i].l+180)<0.0001)
			point[i].l=180;
	}
	Point startPoint;
	startPoint.l=city[fatherindex].l;
	startPoint.m=city[fatherindex].m;
	Point endPoint;
	endPoint.l=city[childindex].l;
	endPoint.m=city[childindex].m;

	for(int i=0;i<sizenum-1;i++)
	{
		st.hour+=1;
		if(st.hour>=24)
		{
			st.hour=st.hour%24;
			st.date++;
			if(st.date>monthday[st.month-1])
			{
				st.date=1;
				st.month++;
				if(st.month>12)
				{
					st.year++;
					if(isLeapYear(st.year))
						monthday[1]=29;
					else
						monthday[1]=28;
					st.month=1;
				}
			}
		}
		if(1==i%2||InCircle(point[i],startPoint,portCircleRadius))
		{
			continue;
		}
		fout<<"<Placemark>"<<endl;
		fout<<"<name></name>"<<endl;
		fout<<"<description>packet</description>"<<endl;
		fout<<"<styleUrl>#dateicon_style</styleUrl>"<<endl;
		fout<<"<TimeStamp>"<<endl;
		fout<<" <when>"<<st.year<<"-";
		if(st.month<10)
		{
			fout<<"0";
		}
		fout<<st.month<<"-";
		if(st.date<10)
			fout<<"0";
		fout<<st.date<<"T";
		if(st.hour<10)
		fout<<"0";
		fout<<st.hour<<":00:00Z"<<"</when>"<<endl;
		fout<<"</TimeStamp>"<<endl;
		fout<<"<Point>"<<endl;
		fout<<"<coordinates>"<<point[i].l<<","<<point[i].m<<"</coordinates>"<<endl;
		fout<<"</Point>"<<endl;
		fout<<"</Placemark>"<<endl;
	}
}
/******************************************
从文件读出数据(并且初始化)
*******************************************/
bool CGEComputeNode::ReadPointFromFile(string &filename,int option)
{
	QString fileName;
//	string strname="H:/MFC/QT Project/DemoOnGEv6/DemoOnGEv6/DemoOnGE/allcity/";
	if(!option)
	{
		fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"),"/home",tr("*.txt"));
		if(fileName == "")
			return false; 
	}
	else
	{
		fileName=QString::fromLocal8Bit(filename.c_str());
	}
	m_inPoints.clear();
	m_triangles.clear();
    QString path;
	//以上保证filename是正确路径
	if(0==option)
	{
		path = fileName;
	}
	else
	{
		path=QString::fromLocal8Bit(filename.c_str());
	}
	//如果没有数据,就直接返回了
	if (!QFile::exists(path))
	{
		return false;
	}
    filename=path.toStdString();
	filename[2]='/';//c++中的路径要用“/”而不是“\”。
	outputfile=filename.substr(0,filename.size()-4)+".kml";
	;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	initialize();
	QTextStream the_load(&file);

	QString data1,data2,data3;
	float  realx = 0,realy = 0;
   
	minLongitude=180.0;
	maxLongitude=-180.0;
    minLatitude=90.0;
	maxLatitude=-90.0;

	while(!the_load.atEnd())
	{
		the_load>>data1>>data2>>data3>>realx>>realy;
		SpacePoint t_point;

		t_point.index = m_inPoints.size();
		t_point.continent = data1;
		t_point.country = data2;
		city[cityNum].countryDomain=data2.toStdString();
		t_point.city = data3;
		city[cityNum].cityName=data3.toStdString();
		t_point.x = realx;
		if(realx>maxLongitude)
			maxLongitude=realx;
		if(realx<minLongitude)
			minLongitude=realx;
		city[cityNum].m=realx;
		t_point.y = realy;
		if(realy>maxLatitude)
			maxLatitude=realy;
		if(realy<minLatitude)
			minLatitude=realy;
		city[cityNum].l=realy;
     
		if(realx == 0.0 && realy == 0.0)
			continue;
        
		m_inPoints.push_back(t_point);
		cityNum++;
	}

	if(m_inPoints.size() != 0)
	{
		//生成节点和三角形
		CreatGrid(m_inPoints,m_triangles);
		//建立三角形连接关系
		UpdataNeighbor(m_inPoints,m_triangles);
		///删除一些三角形(有些关系在海上的)
		Deletetriangles();
	}
	return true;
}

//Judge whether a year is a leap year
bool CGEComputeNode::isLeapYear(int _year)
{
	if(_year%4!=0||(_year%100==0&&_year%400!=0))
	{
		return false;
	}
	return true;
}



void CGEComputeNode::getAllAsDomain()
{
	AsNum=0;
	int _circletimes=0;
	int asNumForContinent[]={5,2,3,3,3,4};
	int step=cityNum/asNumForContinent[continentId];
	int _nowCityId=topoRootIndex[continentId];
	while (_circletimes<asNumForContinent[continentId])
	{
		ASNo[_circletimes].asCenter.l=city[_nowCityId].l;
		ASNo[_circletimes].asCenter.m=city[_nowCityId].m;
		++_circletimes;
		_nowCityId=(_nowCityId+step)%cityNum;
		AsNum++;
		ASNo[_circletimes].cityInAS.clear();
	}

    for(int i=0;i<CityUsed.size();i++)
	{
		double minDis=INF;
		int asBelongedto=-1;
		for(int j=0;j<AsNum;j++)
		{
			Point _tmpPoint;
			_tmpPoint=city[CityUsed[i]];
		    double tmpdis=getDistance(_tmpPoint,ASNo[j].asCenter);
		    if(tmpdis<minDis)
			{
				minDis=tmpdis;
				asBelongedto=j;
			}
		}
		ASNo[asBelongedto].cityInAS.push_back(CityUsed[i]);
	}
	for(int i=0;i<AsNum;i++)
	{
		ASNo[i].asCenter.l=0;
		ASNo[i].asCenter.m=0;
		int j;
		for(j=0;j<ASNo[i].cityInAS.size();j++)
		{
			ASNo[i].asCenter.l+=city[ASNo[i].cityInAS[j]].l;
			ASNo[i].asCenter.m+=city[ASNo[i].cityInAS[j]].m;
		}
		ASNo[i].asCenter.l/=j;
		ASNo[i].asCenter.m/=j;
	}
}

bool CGEComputeNode::ifLineCross(Point startPoint1,Point endPoint1,Point startPoint2,Point endPoint2)
{
	int tmp1=crossProduct(startPoint1,startPoint2,endPoint2);
	int tmp2=crossProduct(endPoint1,startPoint2,endPoint2);
	if(tmp1*tmp2>=0)
		return false;
	tmp1=crossProduct(startPoint2,startPoint1,endPoint1);
	tmp2=crossProduct(endPoint2,startPoint1,endPoint1);
	if(tmp1*tmp2>=0)
		return false;
	return true;
}

void CGEComputeNode::drawAsIcon(Point _ascenter,Simtime _simTime,int _asno)
{
	
	fout<<"<Placemark>\n<name> AS"<<_asno<<" </name>\n";
	fout<<"<description> AS"<<_asno<<"</description>"<<endl;
	fout<<"<styleUrl>#icon_style</styleUrl>"<<endl;
	//changeLookAt(_ascenter.l,_ascenter.m,2*heightlevel2,_simTime);
	fout<<"<TimeSpan>"<<endl;
	fout<<" <begin>"<<_simTime.year<<"-";
	if(_simTime.month<10)
	{
		fout<<"0";
	}
	fout<<_simTime.month<<"-";
	if(_simTime.date<10)
		fout<<"0";
	fout<<_simTime.date<<"T";
	if(_simTime.hour<10)
			fout<<"0";
	fout<<_simTime.hour<<":00:00Z"<<"</begin>"<<endl;
	fout<<"</TimeSpan>"<<endl;
	fout<<"<Point>"<<endl;
	fout<<"<altitudeMode>relativeToGround</altitudeMode>"<<endl;
	fout<<"<coordinates>"<<_ascenter.l<<","<<_ascenter.m<<",";
	fout<<heightlevel1+heightlevel2<<endl;
	fout<<"</coordinates>"<<endl;
	fout<<"</Point>"<<endl;
	fout<<"</Placemark>"<<endl;
}
//get empty circles for topography
void CGEComputeNode::getCircle(int circlenum,int citynum)
{
	Circle cir;
	
	if(continentId==5)
	{
		cir.center.l=16.37208;
		cir.center.m=48.20849;
		cir.radius=6;
		circle.push_back(cir);
	}
	if(continentId==2)
	{
		cir.center.l=-92.6;
		cir.center.m=39.5833;
		cir.radius=8;
		circle.push_back(cir);
	}
}

//the animation of traceroute 


void CGEComputeNode::drawPoint(int cityindex,Simtime beginTime)
{
	fout<<"<Placemark>\n<name>"<<city[cityindex].cityName<<"</name>\n";
	fout<<"<description>"<<city[cityindex].cityName<<"</description>"<<endl;
	fout<<"<styleUrl>#icon_style</styleUrl>"<<endl;
	fout<<"<TimeSpan>"<<endl;
	fout<<" <begin>"<<beginTime.year<<"-";
	if(beginTime.month<10)
	{
		fout<<"0";
	}
	fout<<beginTime.month<<"-";
	if(beginTime.date<10)
		fout<<"0";
	fout<<beginTime.date<<"T";
	if(beginTime.hour<10)
			fout<<"0";
	fout<<beginTime.hour<<":00:00Z"<<"</begin>"<<endl;
	fout<<"</TimeSpan>"<<endl;
	fout<<"<Point>"<<endl;
	if(ifHeight)
	{
		fout<<"<altitudeMode>relativeToGround</altitudeMode>"<<endl;
	}
	fout<<"<coordinates>"<<city[cityindex].l<<","<<city[cityindex].m<<",";
	if (!ifHeight)
	{
		fout<<"0";
	}
	else
	{
		fout<<heightlevel1;
	}
	fout<<"</coordinates>"<<endl;
	fout<<"</Point>"<<endl;
	fout<<"</Placemark>"<<endl;
	placemark[cityindex]=1;
	if(!ifaslevel&&ifHeight&&ifStatic)
	CityUsed.push_back(cityindex);
}

void CGEComputeNode::drawTerminal(Point terminalpoint,char hostIcon[] ,double hostsize,Simtime _hosttime,Simtime _endtime)
{
	changeIcon(hostIcon,hostsize);
	fout<<"<Placemark>\n<name>"<<"</name>"<<endl;
	fout<<"<description>"<<"host"<<"</description>"<<endl;
	fout<<"<styleUrl>#icon_style</styleUrl>"<<endl;
	fout<<"<TimeSpan>"<<endl;
	fout<<"<begin>";
	fout<<_hosttime.year<<"-";
	if(_hosttime.month<10)
	{
		fout<<"0";
	}
	fout<<_hosttime.month<<"-";
	if(_hosttime.date<10)
		fout<<"0";
	fout<<_hosttime.date<<"T";
	if(_hosttime.hour<10)
			fout<<"0";
	fout<<_hosttime.hour<<":00:00Z";
	fout<<"</begin>"<<endl;
	if(ifTerminalDisappear)
	{
		fout<<"<end>";
		fout<<_endtime.year<<"-";
		if(_endtime.month<10)
		{
			fout<<"0";
		}
		fout<<_endtime.month<<"-";
		if(_endtime.date<10)
			fout<<"0";
		fout<<_endtime.date<<"T";
		if(_endtime.hour<10)
			fout<<"0";
		fout<<_endtime.hour<<":00:00Z";
		fout<<"</end>"<<endl;
	}
	fout<<"</TimeSpan>"<<endl;
	fout<<"<Point>"<<endl;
	fout<<"<coordinates>"<<terminalpoint.l<<","<<terminalpoint.m<<",0</coordinates>"<<endl;
	fout<<"</Point>"<<endl;
	fout<<"</Placemark>"<<endl;
	changeIcon("port.png",portSize);
}

void CGEComputeNode::drawPointport(Point _point,Simtime _beginTime)
{
    int k;
	k=rand()%1024+1;
    fout<<"<Placemark>\n<name>"<<"</name>"<<endl;
	fout<<"<description>"<<"port"<<k<<"</description>"<<endl;
	fout<<"<styleUrl>#icon_style</styleUrl>"<<endl;
	fout<<"<TimeSpan>"<<endl;
	fout<<"<begin>";
	fout<<_beginTime.year<<"-";
	if(_beginTime.month<10)
	{
		fout<<"0";
	}
	fout<<_beginTime.month<<"-";
	if(_beginTime.date<10)
		fout<<"0";
	fout<<_beginTime.date<<"T";
	if(_beginTime.hour<10)
			fout<<"0";
	fout<<_beginTime.hour<<":00:00Z";
	fout<<"</begin>"<<endl;
	fout<<"</TimeSpan>"<<endl;
	fout<<"<Point>"<<endl;
	fout<<"<coordinates>"<<_point.l<<","<<_point.m<<",0</coordinates>"<<endl;
	fout<<"</Point>"<<endl;
	fout<<"</Placemark>"<<endl;
}

void CGEComputeNode::drawPoint(Point cityself)
{
	fout<<"<Placemark>\n<name>"<<"Hawaii"<<"</name>\n";
	fout<<"<description>"<<"Hawaii"<<"</description>"<<endl;
	fout<<"<styleUrl>#icon_style</styleUrl>"<<endl;
	fout<<"<TimeSpan>"<<endl;
	fout<<"<begin>"<<simtime.year<<"-";
	if(simtime.month<10)
	{
		fout<<"0";
	}
	fout<<simtime.month<<"-";
	if(simtime.date<10)
		fout<<"0";
	fout<<simtime.date<<"T";
	if(simtime.hour<10)
			fout<<"0";
	fout<<simtime.hour<<":00:00Z"<<"</begin>"<<endl;
	fout<<"</TimeSpan>"<<endl;
	fout<<"<Point>"<<endl;
	if(ifHeight)
	{
		fout<<"<altitudeMode>relativeToGround</altitudeMode>"<<endl;
	}
	fout<<"<coordinates>"<<cityself.l<<","<<cityself.m<<",";
	if (!ifHeight)
	{
		fout<<"0";
	}
	else
	{
		fout<<heightlevel1;
	}
	fout<<"</coordinates>"<<endl;
	fout<<"</Point>"<<endl;
	fout<<"</Placemark>"<<endl;
}

void CGEComputeNode::showTraceRouteDfs(TreeNode* root,Simtime &st)
{
	mark[root->index]=1;
	if(0==root->nextnodesvec.size())
	{
		if(0==leafNum%sameTimeTraceRoute)
		{
			timeRecord=anotherTimeRecord;
		}
		st=timeRecord;
		leafNum++;
		changeBagIcon("dataicon.png");
		int j;
	
		for(j=0;j<(int)routeStack.size()-1;j++)
		{
			drawBag(routeStack[j],routeStack[j+1],st);
		}
		changeBagIcon("databackicon.png");
		for(;j>0;j--)
		{
			drawBag(routeStack[j],routeStack[j-1],st);
		}
		for(;j<(int)routeStack.size()-1;j++)
		{
			Point tmpPoint;
			if (j)
			{
				tmpPoint=portPoint;
			}
			else
			{
				tmpPoint.l=city[routeStack[0]].l;
				tmpPoint.m=city[routeStack[0]].m;
			}
			portPoint=getCrossPoint(routeStack[j+1],routeStack[j]);
			drawLine(tmpPoint.l,tmpPoint.m,portPoint.l,portPoint.m,st);

			if(j==routeStack.size()-2&&ifTerminalDisappear)
			{
				if(anotherTimeRecord<st)
				anotherTimeRecord=st;
				drawTerminal(portPoint,"probepoint.png",2.0,timeRecord,anotherTimeRecord);
			}
			if(!edgeMark[routeStack[j]][routeStack[j+1]])
			{
					drawPointport(portPoint,st);
			}
			edgeMark[routeStack[j]][routeStack[j+1]]=1;
		}
	}
	for(int i=0;i<(int)root->nextnodesvec.size();i++)
	{
		if(!mark[root->nextnodesvec[i]->index])
		{
			routeStack.push_back(root->nextnodesvec[i]->index);
			showTraceRouteDfs(root->nextnodesvec[i],st);
			routeStack.pop_back();
		}
	}
}

void CGEComputeNode::traceRoute()
{
	fout.open(outputfile.c_str());
	fout<<"<Document>"<<endl;
	fout<<"<Folder>"<<endl;
	fout<<"<name>Hole Overlays</name>"<<endl;
	fout<<"<open>0</open>"<<endl;
	fout<<"<description>Examples of ground overlays</description>"<<endl;
	for(int i=0;i<circlenum;i++)
	{
		fout<<"<GroundOverlay>"<<endl;
		fout<<"<name>hole</name>"<<endl;
		fout<<"<description>hole</description>"<<endl;
		fout<<"<Icon>"<<endl;
		fout<<"<href>hole.png</href>"<<endl;
		fout<<"</Icon>"<<endl;
		fout<<"<LatLonBox>"<<endl;
		fout<<"<north>"<<circle[i].center.m+circle[i].radius<<"</north>"<<endl;
		fout<<"<south>"<<circle[i].center.m-circle[i].radius<<"</south>"<<endl;
        fout<<"<east>"<<change(circle[i].center.l+circle[i].radius)<<"</east>"<<endl;
        fout<<"<west>"<<change(circle[i].center.l-circle[i].radius)<<"</west>"<<endl;
        fout<<"<rotation>0</rotation>"<<endl;
		fout<<"</LatLonBox>"<<endl;
		fout<<"</GroundOverlay>"<<endl;	
	}
	fout<<"</Folder>"<<endl;
	changeIcon("sourcepoint.png",2.5);
    memset(placemark,0,sizeof(placemark));
	endtime.date=1;
    endtime.month=1;
	endtime.year=2009;
	endtime.hour=0;
	if(continentId>0)
	{
		simtime=begintime;
	}
	else
	{
		simtime.date=1;
		simtime.month=1;
		simtime.year=2009;
		simtime.hour=0;
		begintime=simtime;

	}
	for(int i=0;i<(int)rootVector.size();i++)
	{
		drawPoint(rootVector[i]->index,simtime);
	}
	changeIcon("port.png",portSize);
  //  changeIcon("probepoint.png",1.0);

	m_incirclePoints.clear();
	m_outcirclePoints.clear();
	getPointParameter=true;
    memset(edgeMark,0,sizeof(edgeMark));
	for(int i=0;i<TREENUM;i++)
	{
		changeLineStyle(i%6,2);
		while(simtime.date>monthday[(simtime.month)-1])
		{
			simtime.date-=monthday[(simtime.month-1)];
			simtime.month++;
		}
		memset(mark,0,sizeof(mark));
		traceroute=1;
		routeStack.clear();
		routeStack.push_back(rootVector[i]->index);
		if(i+continentId)
		{
			dfs(rootVector[i],simtime);
		}
		else
		{
			timeRecord=simtime;
			anotherTimeRecord=simtime;
			sameTimeTraceRoute=10;
			leafNum=0;
			ifTerminalDisappear=true;
			showTraceRouteDfs(rootVector[i],simtime);
			ifTerminalDisappear=false;
		}
	}
	simtime=endtime;
	if(2==continentId||5==continentId)
	{
		createCircleGridandTreeRoot(3);
	}
	for(int i=0;i<(int)outCityIndex.size()-1;i++)
	{
		if(1==continentId&&1==i)
		{
            Point tmpPoint;
			tmpPoint.l=outCityIndex[i].l;
			tmpPoint.m=outCityIndex[i].m;
		   	drawPointport(tmpPoint,simtime);
		}
		drawLine(outCityIndex[i].l,outCityIndex[i].m,outCityIndex[i+1].l,outCityIndex[i+1].m,simtime);
	}
	endtime=simtime;
	fout<<"</Document>"<<endl;
	fout.close();
}
void CGEComputeNode::topographyformtraceroute()
{
	fout.open(outputfile.c_str());
    fout<<"<Document>"<<endl;
	changeLineStyle(6,2);
	memset(mark,0,sizeof(mark));
	memset(placemark,0,sizeof(placemark));
	changeIcon("probepoint.png",2.5);
	for(int i=0;i<m_outcirclePoints.size();i++)
	{
		drawPoint(m_outcirclePoints[i].index,simtime);
	}
	//changeIcon("juanjo_Router.png",1.0);
	changeIcon("port.png",portSize);
	leafNum=0;
	timeRecord=simtime;
	traceroute=2;
	anotherTimeRecord=simtime;
    changeBagIcon("dataicon.png");
	for(int i=0;i<m_circletreevec.size();i++)
	{
		routeStack.clear();
		routeStack.push_back(m_circletreevec[i]->index);
		topographyFormTraceRoute(m_circletreevec[i],simtime);
	}
	fout<<"</Document>"<<endl;
	fout.close();
}

void CGEComputeNode::topographyFormTraceRoute(TreeNode *root,Simtime& st)
{
	if(0==root->nextnodesvec.size())
	{
		leafNum++;	
		//anotherTimeRecord
		if(1==leafNum%2)
		{
			timeRecord=anotherTimeRecord;
		}
		st=timeRecord;
		for(int i=0;i<(int)routeStack.size()-1;i++)
		{
			drawBag(routeStack[i],routeStack[i+1],st);
		}
		for(int i=0;i<(int)routeStack.size()-1;i++)
		{
			Point tmpPoint;
			if (i==0)
			{
				tmpPoint.l=city[routeStack[i]].l;
				tmpPoint.m=city[routeStack[i]].m;
			}
			else
			{
				tmpPoint=portPoint;
			}
			portPoint=getCrossPoint(routeStack[i+1],routeStack[i]);

			drawLine(tmpPoint.l,tmpPoint.m,portPoint.l,portPoint.m,st);
			if(!edgeMark[routeStack[i]][routeStack[i+1]])
			{
				drawPointport(portPoint,st);
			}
			edgeMark[routeStack[i]][routeStack[i+1]]=1;
		}
		if(anotherTimeRecord<st)
		{
			anotherTimeRecord=st;
		}
	}
	for(int i=0;i<(int)root->nextnodesvec.size();i++)
	{
		routeStack.push_back(root->nextnodesvec[i]->index);
		topographyFormTraceRoute(root->nextnodesvec[i],st);
		routeStack.pop_back();
	}
}
//Draw a line with two endpoints startpoint,endpoint
void CGEComputeNode::drawLine(int startpointindex,int endpointindex,Simtime &st)
{
	if (ifneedoutincirclepoint)
	{
		generateOutAndIn(startpointindex,endpointindex);
	}
	drawLine(city[startpointindex].l,city[startpointindex].m,city[endpointindex].l,city[endpointindex].m,st);
}
//the animation of topo form
void CGEComputeNode::solute()
{
	fout.open(outputfile.c_str());
	fout<<"<Document>"<<endl;
    fout<<"<Folder>"<<endl;
	fout<<"<name>Hole Overlays</name>"<<endl;
	fout<<"<open>0</open>"<<endl;
	fout<<"<description>Examples of ground overlays</description>"<<endl;
	for(int i=0;i<circlenum;i++)
	{
		fout<<"<GroundOverlay>"<<endl;
		fout<<"<name>hole</name>"<<endl;
		fout<<"<description>hole</description>"<<endl;
		fout<<"<Icon>"<<endl;
		fout<<"<href>hole.png</href>"<<endl;
		fout<<"</Icon>"<<endl;
		fout<<"<LatLonBox>"<<endl;
		fout<<"<north>"<<circle[i].center.m+circle[i].radius<<"</north>"<<endl;
		fout<<"<south>"<<circle[i].center.m-circle[i].radius<<"</south>"<<endl;
        fout<<"<east>"<<change(circle[i].center.l+circle[i].radius)<<"</east>"<<endl;
        fout<<"<west>"<<change(circle[i].center.l-circle[i].radius)<<"</west>"<<endl;
        fout<<"<rotation>0</rotation>"<<endl;
		fout<<"</LatLonBox>"<<endl;
		fout<<"</GroundOverlay>"<<endl;	
	}
	fout<<"</Folder>"<<endl;
	memset(placemark,0,sizeof(placemark));
//get the simulation start time
	endtime.date=1;
    endtime.month=1;
	endtime.year=2009;
	endtime.hour=0;
	if(continentId>0)
	{
		simtime=begintime;
	}
	else
	{
		simtime.date=1;
		simtime.month=1;
		simtime.year=2009;
		simtime.hour=0;
		begintime=simtime;
	}
	changeIcon("sourcepoint.png",2.5);
	//traceroute=2;
	for(int i=0;i<(int)rootVector.size();i++)
	{
		drawPoint(rootVector[i]->index,simtime);
	}
	changeIcon("port.png",portSize);
	m_incirclePoints.clear();
	m_outcirclePoints.clear();
	getPointParameter=true;
	memset(edgeMark,0,sizeof(edgeMark));
	for(int i=0;i<TREENUM;i++)
	{
		changeLineStyle(i%6,2);
		while(simtime.date>monthday[(simtime.month)-1])
		{
			simtime.date-=monthday[(simtime.month-1)];
			simtime.month++;
		}
		memset(mark,0,sizeof(mark));
		traceroute=0;
		dfs(rootVector[i],simtime);
		simtime=endtime;
	}
	simtime=endtime;
	if(2==continentId||5==continentId)
	{
		createCircleGridandTreeRoot(3);
	}
	for(int i=0;i<(int)outCityIndex.size()-1;i++)
	{
		if(1==continentId&&1==i)
		{
		    drawPoint(outCityIndex[i]);//画出夏威夷这个点
		}
		drawLine(outCityIndex[i].l,outCityIndex[i].m,outCityIndex[i+1].l,outCityIndex[i+1].m,simtime);
	}
	endtime=simtime;
	fout<<"</Document>"<<endl;
	fout.close();
}

//层析成像拓扑生成
void CGEComputeNode::astopoForm()
{
	getAllAsDomain();
	int asseq[6];
	double _mindis=INF;
	fout.open(outputfile.c_str());
	fout<<"<Document>"<<endl;
	changeIcon("cloud.png",20);
	endtime.date=1;
    endtime.month=1;
	endtime.year=2009;
	endtime.hour=0;
	if(continentId>0)
	{
		simtime=begintime;
	}
	else
	{
		simtime.date=1;
		simtime.month=1;
		simtime.year=2009;
		simtime.hour=0;
		begintime=simtime;
	}
	endtime=begintime;
	asseq[0]=0;
	if(continentId>0)
	{
		ifastoas=true;
		changeLineStyle(3,4);
		if (2==continentId)
		{
			Point tmpPoint;
			tmpPoint.l=-157.85833;
			tmpPoint.m=21.30694;
			drawLine(prevPoint.l,prevPoint.m,tmpPoint.l,tmpPoint.m,simtime);
			drawAsIcon(tmpPoint,simtime,0);
			ifastoas=false;
			changeLineStyle(5,1);
			drawLine(tmpPoint.l,tmpPoint.m,tmpPoint.l,tmpPoint.m,simtime);
			changeLineStyle(3,4);
			ifastoas=true;
			drawLine(tmpPoint.l,tmpPoint.m,ASNo[0].asCenter.l,ASNo[0].asCenter.m,simtime);
		}
		else
		{
			drawLine(prevPoint.l,prevPoint.m,ASNo[0].asCenter.l,ASNo[0].asCenter.m,simtime);
		}
		ifastoas=false;
	}
	switch(continentId)
	{
		case 0:
			asseq[1]=4;
			asseq[2]=3;
			asseq[3]=2;
			asseq[4]=1;
			break;
		case 1:
		    asseq[1]=1;
			break;
		case 2:
			asseq[1]=2;
			asseq[2]=1;
			break;
		case 3:
			asseq[1]=2;
			asseq[2]=1;
			break;
		case 4:
			asseq[1]=2;
			asseq[2]=1;
			break;
		default:
			asseq[1]=2;
			asseq[2]=1;
			asseq[3]=3;
			break;
	}
	lastPoint=ASNo[asseq[AsNum-1]].asCenter;
	for(int i=0;i<AsNum;i++)
	{
		Simtime _tmptime=endtime;
		drawAsIcon(ASNo[asseq[i]].asCenter,_tmptime,asseq[i]);
		changeLineStyle(5,1,0.5);
		for(int j=0;j<(int)ASNo[asseq[i]].cityInAS.size();j++)
		{
			simtime=_tmptime;
			drawLine(ASNo[asseq[i]].asCenter.l,ASNo[asseq[i]].asCenter.m,city[ASNo[asseq[i]].cityInAS[j]].l,city[ASNo[asseq[i]].cityInAS[j]].m,simtime);
		}
		if(i+1<AsNum)
		{
            ifastoas=true;
			changeLineStyle(3,3);
			drawLine(ASNo[asseq[i]].asCenter.l,ASNo[asseq[i]].asCenter.m,ASNo[asseq[i+1]].asCenter.l,ASNo[asseq[i+1]].asCenter.m,endtime);
			ifastoas=false;
		}
	}

	fout<<"</Document>"<<endl;
    fout.close();
}
void CGEComputeNode::changeLineStyle(int colorId,int lineWidth,double alpha)
{
		fout<<"<Style id=\"color_line\">"<<endl;
		fout<<" <LineStyle>"<<endl;	
		fout<<"<color>";
		int _alpha=alpha*255;
		fout<<hex<<_alpha;
		fout<<dec;
		fout<<treeColor[colorId]<<"</color>"<<endl;
		fout<<"<width>"<<lineWidth<<"</width>"<<endl;
		fout<<"</LineStyle>"<<endl;
		fout<<"</Style>"<<endl;
}

void CGEComputeNode::topographyFormTopo()
{
	fout.open(outputfile.c_str());
    fout<<"<Document>"<<endl;
	changeLineStyle(6,2);
    
	traceroute=2;
	
	memset(placemark,0,sizeof(placemark));

	changeIcon("probepoint.png",2.0);
	for(int i=0;i<(int)m_outcirclePoints.size();i++)
	{
		drawPoint(m_outcirclePoints[i].index,simtime);
	}
	changeIcon("port.png",portSize);
	for(int i=0;i<(int)m_circletreevec.size();i++)
	{
		memset(mark,0,sizeof(mark));
		dfs(m_circletreevec[i],simtime);
		simtime=endtime;
	}
	fout<<"</Document>"<<endl;
	fout.close();
}
//产生在洞内和洞边缘的点
void CGEComputeNode::generateOutAndIn(int idFather,int idChild)
{
	if(!getPointParameter)
	{
		return;
	}
	if(InCircle(idChild)&&ifPointsCirclrExist(idChild))
	{
		m_incirclePoints.push_back(m_inPoints[idChild]);
	}
	if(InCircle(idChild)||InCircle(idFather))
	{
		if(!InCircle(idFather)&&ifPointsCirclrExist(idFather))
		{
			m_outcirclePoints.push_back(m_inPoints[idFather]);
		}
		if(!InCircle(idChild)&&ifPointsCirclrExist(idChild))
		{
			m_outcirclePoints.push_back(m_inPoints[idChild]);
		}
	}
}

void CGEComputeNode::changeLookAt(double _longtitude,double _latitude,double _altitude,Simtime st)
{
	fout<<"<LookAt>"<<endl;
	fout<<"<longitude>"<<_longtitude<<"</longitude>"<<endl;
	fout<<"<latitude>"<<_latitude<<"</latitude>"<<endl;
	fout<<"<altitude>"<<(int)_altitude<<"</altitude>"<<endl;
    fout<<"<range>540.68</range>"<<endl;
    fout<<"<tilt>0</tilt>"<<endl;
    fout<<"<heading>0</heading>"<<endl;
	fout<<"<altitudeMode>relativeToGround</altitudeMode>"<<endl;
	fout<<"</LookAt>"<<endl;
}
//Draw a line with two endpoints (a1,b1) (c1,d1)
void CGEComputeNode::drawLine(double a1,double b1,double c1,double d1,Simtime &st)
{
	int sizenum=0;
	getPoint(a1,b1,c1,d1,sizenum);
	double _para;
	_para=(double)(sizenum+1000)/(double)2000;
	for(int i=0;i<sizenum;i++)
	{
		height[i]=abs(sizenum/2-abs(sizenum/2-i));
		if(ifHeight)
		{
			if (!ifaslevel)
			{
				height[i]=pow(height[i],1.0/3.0)*Height*_para+heightlevel1;
				height[sizenum-1]=heightlevel1;
			}
			else
			{
				if(!ifastoas)
				{
					height[i]=(1-(double)i/(double)sizenum)*(double)heightlevel2+heightlevel1;
				}
				else
				height[i]=heightlevel1+heightlevel2;
			}
		}
		else{
			height[i]=0;
		}
		if(fabs(point[i].l+180)<0.0001)
			point[i].l=180;
	}
	Point startPoint;
	startPoint.l=a1;
	startPoint.m=b1;
	Point endPoint;
	endPoint.l=c1;
	endPoint.m=d1;
	Simtime endsimtime;
	InitSimtime(endsimtime,sizenum,0,0,0);
	endsimtime=st+endsimtime;
	int i;
	for(i=0;i<sizenum-1;i++)
	{
		if(i%2==1&&ifDotLine)
		{
			continue;
		}
		fout<<"<Placemark>"<<endl;
		fout<<"<name></name>"<<endl;
		fout<<"<description>colorline</description>"<<endl;
		fout<<"<styleUrl>#color_line</styleUrl>"<<endl;
		if(!ifStatic)
		{
			fout<<"<TimeSpan>"<<endl;
			fout<<" <begin>"<<st.year<<"-";
			if(st.month<10)
			{
				fout<<"0";
			}
			fout<<st.month<<"-";
			if(st.date<10)
			fout<<"0";
			fout<<st.date<<"T";
			if(st.hour<10)
			fout<<"0";
			fout<<st.hour<<":00:00Z"<<"</begin>"<<endl;
			fout<<"</TimeSpan>"<<endl;
			Simtime tmpsimtime;
			InitSimtime(tmpsimtime,1,0,0,0);
			st=st+tmpsimtime;
			if(isLeapYear(st.year))
			{
				monthday[1]=29;
			}
			else
			{
				monthday[1]=28;
			}
		}
		fout<<"<LineString>"<<endl;
		fout<<"<extrude>0</extrude>"<<endl;
		fout<<"<tessellate>1</tessellate>"<<endl;

		if(!ifHeight)
			fout<<"<altitudeMode>clampToGround</altitudeMode>"<<endl;
		else
			fout<<"<altitudeMode>relativeToGround</altitudeMode>"<<endl;

		fout<<"<coordinates>"<<endl;
		fout<<"\t"<<point[i].l<<","<<point[i].m<<","<<height[i]<<endl;
		fout<<"\t"<<point[i+1].l<<","<<point[i+1].m<<","<<height[i+1]<<endl;
		fout<<"</coordinates>"<<endl;
		fout<<"</LineString>"<<endl;
		fout<<"</Placemark>"<<endl;
	}
	if(endtime<st)
		endtime=st;
}

//draw two endpoints and judge whether draw the line between them
void CGEComputeNode::process1(int idFather,int idChild,Simtime &st)
{
	drawLine(city[idFather].l,city[idFather].m,city[idChild].l,city[idChild].m,st);
	if(!placemark[idChild])
	drawPoint(idChild,st);
	if(endtime<st)
	{
		endtime=st;
	}
}
//traceroute=0,1代表圈外traceroute topo生成过程,traceroute=2代表圈内,traceroute=3代表路由器级拓扑恢复 
//the search process in the tree
void CGEComputeNode::dfs(TreeNode* root,Simtime &st)
{
	mark[root->index]=1;
	Simtime tmp=st;Point pointFather;
	pointFather=portPoint;
	for(int i=0;i<root->nextnodesvec.size();i++)
	{
		if(ifbfs)
		{   
			st=tmp;
		}
		if(!mark[root->nextnodesvec[i]->index])
		{
			if (ifneedoutincirclepoint)
			{
				generateOutAndIn(root->index,root->nextnodesvec[i]->index);
			}
			if (ifHeight)
			{
				process1(root->index,root->nextnodesvec[i]->index,st);
			}
			else
			{
				 if(!InCircle(root->index)&&!InCircle(root->nextnodesvec[i]->index)||!ifneedoutincirclepoint)
				 {
					 Point tmpPoint;
					 if(root->fathernode==NULL)
					 {
						 tmpPoint.l=city[root->index].l;
						 tmpPoint.m=city[root->index].m;
					 }
					 else
					 {
						 tmpPoint=pointFather;
					 }
					 portPoint=getCrossPoint(root->nextnodesvec[i]->index,root->index);
					 drawLine(tmpPoint.l,tmpPoint.m,portPoint.l,portPoint.m,st);
					 if(!edgeMark[root->index][root->nextnodesvec[i]->index]&&!ifHeight)
					 {
						 drawPointport(portPoint,st);
					 }
					 edgeMark[root->index][root->nextnodesvec[i]->index]=1;
				 }
			}
			dfs(root->nextnodesvec[i],st);
		}
	}
}

//改变style id="icon_style"所对应的icon
void CGEComputeNode::changeIcon(char iconString[],double scaleSize)
{
	fout<<"<Style id=\"icon_style\">\n<IconStyle>\n<Icon>\n<href>"<<iconString<<"</href>\n</Icon>\n";
	fout<<"<scale>"<<scaleSize<<"</scale>"<<"</IconStyle>\n</Style>"<<endl;
}
//改变style id="dataicon_style"所对应的icon
void CGEComputeNode::changeBagIcon(char iconBag[])
{
	 fout<<"<Style id=\"dateicon_style\">\n<IconStyle>\n<Icon>\n<href>"<<iconBag<<"</href>\n</Icon>\n</IconStyle>\n</Style>"<<endl;
}
double CGEComputeNode::deal(double i)
{
	if(i<0)
	{
		i+=360;
	}
	if(i>180)
	{
		i=-(360-i);
	}
	return i;
}

double CGEComputeNode::deal2(double i)
{
	if(i<0)
		i=360-fabs(i);
	else
	{
		i=-(360-fabs(i));
	}
	return i;
}

double CGEComputeNode::change(double l)
{
	if(l>180)
		l-=360;
    if(l<=-180)
		l+=360;
	return l;
}

void CGEComputeNode::addSpacetime(Simtime &st)//动画播放时间随机停止1--6个小时
{
	st.hour+=rand()%6+1;
	if(st.hour>23)
	{
		st.date++;
		if(st.date>monthday[st.month-1])
		{
			st.date=1;
			st.month++;
			if(st.month>12)
			{
				st.month=1;
				st.year++;
				if(isLeapYear(st.year))
					monthday[1]=29;
				else
					monthday[1]=28;
			}
		}
		st.hour%=24;
	}
}
//get points and save them in the point[] array
void CGEComputeNode::getPoint(double slong,double smag,double elong,double emag,int &size)
{
	double ldiv,mdiv;
	ldiv=elong-slong;
	mdiv=emag-smag;
	if(fabs(ldiv)>180)
	{
		ldiv=deal2(ldiv);
	}
	size=fabs(sqrt(ldiv*ldiv+mdiv*mdiv))*2;// change the point number
	if(size<10)
		size=10;
	if(size>1000)
		size=1000;
	ldiv/=size;
	mdiv/=size;
	double tmp1=slong;
	double tmp2=smag;
	for(int i=0;i<size;i++)
	{
		point[i].l=deal(tmp1);
		point[i].m=tmp2;
		tmp1+=ldiv;
		tmp2+=mdiv;
	}
	point[size].l=deal(tmp1);
	point[size].m=tmp2;
	size++;
	//return point;
}

//Judge whether a point is in a circle
bool CGEComputeNode::incircle(Circle blackhole,Point input)
{
    /* double ldiv=fabs(blackhole.center.l-input.l);
     ldiv=min(ldiv,360-ldiv);
     double dis=ldiv*ldiv+(input.m-blackhole.center.m)*(input.m-blackhole.center.m);*/
     double dis=getDistance(blackhole.center,input);
	 if(dis<blackhole.radius)
		 return true;
	 return false;
}
double CGEComputeNode::getDistance(Point _pt1, Point _pt2)
{
	double ldiv=fabs(_pt1.l-_pt2.l);
	ldiv=min(ldiv,360-ldiv);
	double tmpres=sqrt(ldiv*ldiv+(_pt1.m-_pt2.m)*(_pt1.m-_pt2.m));
    return tmpres;
}

//Judge whether a city num is in a circle
bool CGEComputeNode::InCircle(int cityindex)
{
	Point tmp;
	tmp.l=city[cityindex].l;
	tmp.m=city[cityindex].m;
	if(InCircle(tmp))
		return true;
	return false;
}
/******************************************
产生父亲节点
*******************************************/
void CGEComputeNode::createTreeRoot(int num)
{
	///点太少返回
	if(m_inPoints.size() < 3 || m_triangles.size() == 0 || num > m_inPoints.size())
		return;
    
	getCircle(circlenum,cityNum);
	int step = m_inPoints.size()/(num+1);
	for(int i = 1; i <= num; i++)
	{
		int lastHeadIndex;
		m_allnodesindex.clear();
		int	randindex = step*i;
		m_headNode = new TreeNode;
		//m_headNode->index = randindex;     the firm root and make sure that all the tree roots are not in the topography circle
		if(1==i)
		{
			m_headNode->index=topoRootIndex[continentId];
			lastHeadIndex=m_headNode->index;
		}
		else
		{
			m_headNode->index=(lastHeadIndex+step)%cityNum;
			lastHeadIndex=m_headNode->index;
		}
		while(InCircle(m_headNode->index))
		{
			m_headNode->index=(m_headNode->index+1)%cityNum;
			lastHeadIndex=m_headNode->index;
		}
		if(!continentId)
		{
			switch(i)
			{
				case 2:
					m_headNode->index=3;
					break;
				case 3:
					m_headNode->index=168;
					break;
			}
		}
		if (3==continentId)
		{
			switch(i)
			{			
				case 2:
					m_headNode->index=12;
					break;
				case 3:
					m_headNode->index=0;
					break;
			}
		}
		if (4==continentId)
		{
			switch(i)
			{			
				case 2:
					m_headNode->index=26;
					break;
				case 3:
					m_headNode->index=64;
					break;
			}
		}
		if (5==continentId)
		{
			switch(i)
			{			
				case 2:
					m_headNode->index=3;
					break;
				case 3:
					m_headNode->index=1;
					break;
			}
		}
		m_headNode->degree = 0;
		m_headNode->nextnodesvec.clear();
		m_headNode->fathernode= NULL;
		m_headNodevec.push_back(m_headNode);

		vector<TreeNode *>t_vec;
		t_vec.clear();
		t_vec.push_back(m_headNode);
		UpdataTree(m_triangles,/*m_headNode*/t_vec);
	}
	rootVector=getTreeNodevec();
}

/******************************************
删除三角形(某些在海上的点不能进入三角形连接)
*******************************************/
void CGEComputeNode::Deletetriangles()
{	
	///读入数据

	QString fileName = QString::fromLocal8Bit(outputfile.c_str());//需要更改路径
    
	fileName=fileName.left(fileName.lastIndexOf("/DemoOnGE"));
    fileName = fileName + "/line_in_sea_.txt"; 

	if(fileName == "")
		return;

	QString path = fileName;
	//如果没有数据,就直接返回了
	if (!QFile::exists(path))
	{
		return;
	}
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	m_inseaPoints.clear();
	QTextStream the_load(&file);

	QString data1,data2;
	float realx1 = 0,realy1 = 0;
	float realx2 = 0,realy2 = 0;
	pair<SpacePoint,SpacePoint>t_pair;

	while(!the_load.atEnd())
	{
		the_load>>data1>>realx1>>realy1>>data2>>realx2>>realy2;;
		
		SpacePoint t_point1;
		SpacePoint t_point2;

		t_point1.index = 0;
		t_point1.continent = "";
		t_point1.country = "";
		t_point1.city = data1;
		t_point1.x = realx1;
		t_point1.y = realy1;
		
		t_point2.index = 0;
		t_point2.continent = "";
		t_point2.country = "";
		t_point2.city = data2;
		t_point2.x = realx2;
		t_point2.y = realy2;

		t_pair.first = t_point1;
		t_pair.second = t_point2;

		m_inseaPoints.push_back(t_pair);
	}


	if(0 == m_inseaPoints.size())
		return;

	SpacePoint pair_point1;
	SpacePoint pair_point2;
	vector<SpacePoint>t_spvec;
	
	for(int i=0;i<m_triangles.size();i++)
	{
		vector<Triangle>::iterator iter = m_triangles.begin();
		t_spvec.clear();
		t_spvec.resize(3);
		t_spvec[0] = m_inPoints[m_triangles[i].p[0]];
		t_spvec[1] = m_inPoints[m_triangles[i].p[1]];
		t_spvec[2] = m_inPoints[m_triangles[i].p[2]];

		bool flag1 = false;
		bool flag2 = false;

		for(int j=0;j<m_inseaPoints.size();j++)
		{
			pair_point1 = m_inseaPoints[j].first;
			pair_point2 = m_inseaPoints[j].second;

			for(int k1=0;k1<3;k1++)
			{
				if(pair_point1.city == t_spvec[k1].city)
					flag1 = true;
			}

			for(int k2=0;k2<3;k2++)
			{
				if(pair_point2.city == t_spvec[k2].city)
					flag2 = true;
			}
			///表明已经确定这个三角形有一条边是属于海上连接关系的(即这个三角形需要被删除)
			if(flag1 && flag2)
				break;
		}

		///删除三角形（如果不删除，说明当前这个三角形没有问题，进入下一个）
		if(flag1 && flag2)
		{
			m_triangles.erase(iter + i);
			i--;
		}
	}
}
/******************************************
更新得到节点的树
*******************************************/
void CGEComputeNode::UpdataTree(Triangles &_triangles,vector<TreeNode *>t_vec)
{

	if(0 == t_vec.size())
		return;

	vector<TreeNode*>t_nodevecofonelayer;
	t_nodevecofonelayer.clear();

	//循环遍历一层的所有节点
	for(int t=0;t<t_vec.size();t++)
	{
		if(NULL == t_vec[t])
			continue;

		//父节点id
		int treenum = t_vec[t]->index/*fathernode->index*/;
		if(treenum < 0)
			return;

		bool flag = false;
		flag = ifExist(m_allnodesindex,treenum);
		if(flag)
			m_allnodesindex.push_back(treenum);

		Triangles t_triangles;
		t_triangles.clear();
		//得到相关的三角形
		for(int i=0;i<_triangles.size();i++)
		{
			for(int j =0;j<3;j++)
			{
				if(treenum == _triangles[i].p[j])
				{
					t_triangles.push_back(_triangles[i]);
				}
			}
		}

		for(int i=0;i < t_triangles.size();i++)
		{
			//相关三角形
			for(int k=0;k<3;k++)
			{
				bool tflag1 = false;
				bool tflag2 = false;
				tflag1 = ifExist(m_allnodesindex,t_triangles[i].p[k]);
				tflag2 = ifExistEdge(m_inPoints[t_vec[t]->index],m_inPoints[t_triangles[i].p[k]]);
				
				//说明可以生成节点，放入
				if(tflag1 && tflag2)
				{
					TreeNode* sonnode = new TreeNode;
					sonnode->index = t_triangles[i].p[k];
					sonnode->degree = 0;
					sonnode->nextnodesvec.clear();
					sonnode->fathernode = t_vec[t]/*fathernode*/;
					
					//将节点的index放入保存vector中
					m_allnodesindex.push_back(t_triangles[i].p[k]);
					//将新生成的节点放入当前节点的后继结点容器中
					/*fathernode*/
					t_vec[t]->nextnodesvec.push_back(sonnode);

					//将新生成的节点放入该层的容器中(下次递归时，这就会是父节点序列)
					t_nodevecofonelayer.push_back(sonnode);
				}
			}
		}
	}

	if(0 != t_nodevecofonelayer.size())
		UpdataTree(m_triangles,t_nodevecofonelayer);

}


/******************************************
CreatGrid(Points _points, Triangles &_triangles)
*******************************************/
void CGEComputeNode::CreatGrid(Points _points, Triangles &_triangles)
{
	if (_points.size() < 3)
	{
		//点数太少
		return;
	}

	int inside;
	int i, j, k;
	SpacePoint px, p1, p2, p3, pc;
	double r;
	double xmin, xmax, ymin, ymax, xmid, ymid;
	double dx, dy, dmax;
	Triangle triangle;
	int maxIndex = m_inPoints.size();
	int nv = _points.size();
	LineSegment *edges = new LineSegment[EMAX];
	int nedge;
	int ntri;

	//输入点按x从小到大排序
	std::sort(_points.begin(),_points.end());

	//3 * (nv + 10)是三角形的最大个数
	//注意: 若本部分发生内存越界, 需检查分配的三角形个数是否足够
	int maxTriNum = 3 * (nv + 10);
	bool *comp = new bool[maxTriNum];        //是否需要剖分标记
	Triangle *tria = new Triangle[maxTriNum];


	//找出最大最小边界,用于计算最大边界三角形
	xmin = _points.front().x;
	ymin = _points.front().y;
	xmax = xmin;
	ymax = ymin;

	for (i = 1; i < nv; i++)
	{
		if (_points[i].x < xmin)
		{
			xmin = _points[i].x;
		}
		if (_points[i].x > xmax)
		{
			xmax = _points[i].x;
		}
		if (_points[i].y < ymin)
		{
			ymin = _points[i].y;
		}
		if (_points[i].y > ymax)
		{
			ymax = _points[i].y;
		}
	}

	dx = xmax - xmin;
	dy = ymax - ymin;
	dmax = (dx > dy) ? dx : dy;
	xmid = (xmax + xmin) / 2.0;
	ymid = (ymax + ymin) / 2.0;

	//构造一个大三角形把所有的点都包含进去
	double r1 = sqrt(dx*dx + dy*dy);
	double l = r1/(tan(3.1415926/6));

	p1.x = xmin - l;
	p1.y = ymin - r1;
	p1.index = maxIndex;
	
	p2.x = xmax + l;
	p2.y = ymin - r1;
	p2.index = maxIndex + 1;

	p3.x = xmid;
	p3.y = ymid + 2*r1;
	p3.index = maxIndex + 2;

	_points.push_back(p1);
	_points.push_back(p2);
	_points.push_back(p3);
	
	///test
	m_inPoints.push_back(p1);
	m_inPoints.push_back(p2);
	m_inPoints.push_back(p3);

	//创建第一个三角形
	triangle.p[0] = maxIndex;
	triangle.p[1] = maxIndex + 1;
	triangle.p[2] = maxIndex + 2;
	triangle.neighbor[0] = -1;
	triangle.neighbor[1] = -1;
	triangle.neighbor[2] = -1;
	tria[0] = triangle;
	comp[0] = false;
	ntri = 0;

	//把每一个点循环插入到三角网中
	for (i = 0; i < nv; i++)
	{
		px = _points[i];
		nedge = 0;
		j = -1;
		do	
		{
			//如果点px位于外接圆中,该三角形的三条边加入到边列表中,并对三角形作标记。
			j++;
			if (!comp[j])
			{
				Triangle thisTriangle = tria[j];
				p1 = m_inPoints[thisTriangle.p[0]];
				p2 = m_inPoints[thisTriangle.p[1]];
				p3 = m_inPoints[thisTriangle.p[2]];
				inside = m_publicMath->circumCircle(px, p1, p2, p3, pc, r);

				if (pc.x + r < px.x)
				{
					comp[j] = true;
				}

				if (inside)
				{
					//设置边数据
					edges[nedge].p1 = thisTriangle.p[0];
					edges[nedge].p2 = thisTriangle.p[1];
					edges[nedge + 1].p1 = thisTriangle.p[1];
					edges[nedge + 1].p2 = thisTriangle.p[2];
					edges[nedge + 2].p1 = thisTriangle.p[2];
					edges[nedge + 2].p2 = thisTriangle.p[0];
					nedge += 3;

					//删除相应的三角形
					tria[j] = tria[ntri];
					comp[j] = comp[ntri];
					j--;
					ntri--;
				}
			}
		} while (j < ntri);

		//删除重复边
		for (j = 0; j < nedge - 1; j++)
		{
			for (k = j + 1; k < nedge; k++)
			{
				if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1))
				{
					edges[j].p1 = -1;
					edges[j].p2 = -1;
					edges[k].p1 = -1;
					edges[k].p2 = -1;
				}
				if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2))
				{
					edges[j].p1 = -1;
					edges[j].p2 = -1;
					edges[k].p1 = -1;
					edges[k].p2 = -1;
				}
			}
		}

		//创建新三角形
		for (j = 0; j < nedge; j++)
		{
			if (edges[j].p1 >= 0 && edges[j].p2 >= 0)
			{
				ntri++;
				triangle.p[0] = edges[j].p1;
				triangle.p[1] = edges[j].p2;
				triangle.p[2] = _points[i].index;
				triangle.neighbor[0] = -1;
				triangle.neighbor[1] = -1;
				triangle.neighbor[2] = -1;
				tria[ntri] = triangle;
				comp[ntri] = false;
			}
		}
	}

	//最后清除顶点为最大三角形顶点的剖分三角形
	int triangleNum = ntri;
	for (i = 0; i <= triangleNum; i++)
	{
		if ((tria[i].p[0] >= maxIndex) ||(tria[i].p[1] >= maxIndex) ||(tria[i].p[2] >= maxIndex))
		{
			tria[i] = tria[ntri];
			ntri--;
			triangleNum = ntri;
			i--;
		}
	}

	//将三角形放入到vector中
	_triangles.resize(ntri + 1);

	for (i = 0; i <= ntri; i++)
	{
		_triangles[i] = tria[i];
	}	

	//删除原始数据中增加的三个点
	m_inPoints.pop_back();
	m_inPoints.pop_back();
	m_inPoints.pop_back();

	//清除分配的内存
	delete []edges;
	edges = NULL;
	delete []comp;
	comp = NULL;
	delete []tria;
	tria = NULL;
}

/******************************************
建立三角形到点的映射关系
*******************************************/
void CGEComputeNode::UpdataNeighbor(Points _points, Triangles &_triangles)
{
	int pointNum = m_inPoints.size()/*_points.size()*/;
	int triNum = _triangles.size();

	//定义三角形到点的映射的数据格式
	//每一个vector存储以该点为顶点的三角形的标号
	std::vector<int> *triToPoint = new std::vector<int>[pointNum];
	int i, j; 
	int p1, p2, p3;

	for (i = 0; i < triNum; i++)
	{
		p1 = _triangles[i].p[0];
		p2 = _triangles[i].p[1];
		p3 = _triangles[i].p[2];
		triToPoint[p1].push_back(i);
		triToPoint[p2].push_back(i);
		triToPoint[p3].push_back(i);
		_triangles[i].neighbor[0] = -1;
		_triangles[i].neighbor[1] = -1;
		_triangles[i].neighbor[2] = -1;
	}

	int flag, size, triIndex;
	//对每个三角形寻找相邻的三角形
	for (i = 0; i < triNum; i++)
	{
		//寻找第一条边的相邻三角形
		p1 = _triangles[i].p[0];
		p2 = _triangles[i].p[1];

		//判断以"p1"为顶点的三角形
		flag = -1;
		j = 0;
		size = triToPoint[p1].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p1][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p2 || _triangles[triIndex].p[1] == p2 || _triangles[triIndex].p[2] == p2)
				{
					_triangles[i].neighbor[0] = triIndex;
					flag = 1;
				}
			}

			j++;
		}

		//判断以"p2"为顶点的三角形
		j = 0;
		size = triToPoint[p2].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p2][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p1 || _triangles[triIndex].p[1] == p1 || _triangles[triIndex].p[2] == p1)
				{
					_triangles[i].neighbor[0] = triIndex;
					flag = 1;
				}
			}

			j++;
		}

		//寻找第二条边的相邻三角形
		p1 = _triangles[i].p[1];
		p2 = _triangles[i].p[2];

		//判断以"p1"为顶点的三角形
		flag = -1;
		j = 0;
		size = triToPoint[p1].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p1][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p2 || _triangles[triIndex].p[1] == p2 || _triangles[triIndex].p[2] == p2)
				{
					_triangles[i].neighbor[1] = triIndex;
					flag = 1;
				}
			}

			j++;
		}

		//判断以"p2"为顶点的三角形
		j = 0;
		size = triToPoint[p2].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p2][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p1 || _triangles[triIndex].p[1] == p1 || _triangles[triIndex].p[2] == p1)
				{
					_triangles[i].neighbor[1] = triIndex;
					flag = 1;
				}
			}

			j++;
		}

		//寻找第三条边的相邻三角形
		p1 = _triangles[i].p[0];
		p2 = _triangles[i].p[2];

		//判断以"p1"为顶点的三角形
		flag = -1;
		j = 0;
		size = triToPoint[p1].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p1][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p2 || _triangles[triIndex].p[1] == p2 || _triangles[triIndex].p[2] == p2)
				{
					_triangles[i].neighbor[2] = triIndex;
					flag = 1;
				}
			}

			j++;
		}

		//判断以"p2"为顶点的三角形
		j = 0;
		size = triToPoint[p2].size();

		while (flag < 0 && j < size)
		{
			triIndex = triToPoint[p2][j];

			if (triIndex != i)
			{
				if (_triangles[triIndex].p[0] == p1 || _triangles[triIndex].p[1] == p1 || _triangles[triIndex].p[2] == p1)
				{
					_triangles[i].neighbor[2] = triIndex;
					flag = 1;
				}
			}

			j++;
		}
	}

	delete []triToPoint;
}

/******************************************
判断节点是否存在,以及边是否应该出现
*******************************************/
bool CGEComputeNode::ifExist(vector<int> vec, int index)
{
	if(vec.size() == 0)
		return true;

	for(int i=0;i<vec.size();i++)
	{
		if(vec[i] == index)
		{
			return false;
		}
	}

	return true;
}

bool CGEComputeNode::ifExistEdge(SpacePoint tp1, SpacePoint tp2)
{
	for(int i=0;i<m_inseaPoints.size();i++)
	{
		if(m_inseaPoints[i].first.city == tp1.city && m_inseaPoints[i].second.city == tp2.city)
		{
			return false;
		}

		else if(m_inseaPoints[i].first.city == tp2.city && m_inseaPoints[i].second.city == tp1.city)
		{
			return false;
		}
	}

	return true;
}

/******************************************
产生若干边缘节点到根节点的路径
*******************************************/
void CGEComputeNode::createSomePathes()
{
	m_AllTreePathPoints.clear();
	for(int i = 0; i < TREENUM; i ++)
	{
		m_TreePathPoints.clear();
		//按间隔为step均匀取出一棵树的部分边缘节点
		int step = m_EdgeNodesVct[i].size()/(PATHNUM+1);
		for(int j = step, k = 1; j < m_EdgeNodesVct[i].size(); k++,j = k*step)
		{	
			//对均匀取出的边缘节点进行获取路径计算
			createPathToTheROOTnode(m_EdgeNodesVct[i][j]);
			//将单个边缘节点获得的路径数组，作为元素，存放入该树对应的路径数组
			m_TreePathPoints.push_back(m_NodePathPoints);
		}
		//将每棵树的路径数组，作为元素，放入全部树的路径数组信息中
		m_AllTreePathPoints.push_back(m_TreePathPoints);
	}
	
	int a= 2;
}

/******************************************
产生从边缘节点到跟节点的路径
*******************************************/
void  CGEComputeNode::createPathToTheROOTnode(TreeNode* edgeNode)
{
	//压入该边缘节点的索引号
	m_pathIndex.push_back(edgeNode->index);
	//根据索引号查出空间点<城市>信息，放入路径数组内
	m_NodePathPoints.push_back(m_inPoints[edgeNode->index]);
	//如果该节点没有父节点，则结束
	if(edgeNode->fathernode == NULL)
	{
		return;
	}
	else
	{
		//否则，递归调用处理
		createPathToTheROOTnode( edgeNode->fathernode);
	}
}

/******************************************
获取所有树的叶节点
*******************************************/
void CGEComputeNode::createAllEdgeNodes()
{
	for(int i = 0; i < TREENUM; i ++)
	{	
		vct_Node.clear();
		createAllEdgeNodesofSingleTree(m_headNodevec[i]);
		m_EdgeNodesVct[i] = vct_Node;
	}
}
/******************************************
获取单个树的所有叶节点
*******************************************/
void CGEComputeNode::createAllEdgeNodesofSingleTree(TreeNode* rootTreeNode)
{
	
	TreeNode *node = rootTreeNode;

	if(node ==NULL)
		return;

//如果没有子节点，则为叶子节点
	if(node->nextnodesvec.size() == 0)
	{
		vct_Node.push_back(node);
		return;
	}
	else
	{
		for(int i = 0; i < node->nextnodesvec.size();i++)
		{
			TreeNode* sonNode;
			sonNode =  node->nextnodesvec[i];
			//递归调用
			createAllEdgeNodesofSingleTree(sonNode);
		}
	}
}



/******************************************
判断树上的点是否在圈内部
*******************************************/
void CGEComputeNode::GetPointsInCircle(TreeNode* rootNode,Points& in_points, Points& out_points)
{
	///合法性
	if(NULL != rootNode && 0 <= rootNode->index && rootNode->index <= m_inPoints.size()-1)
	{
		///叶子节点
		if(rootNode->nextnodesvec.size() == 0)
		{
			bool flag1 = false;
			bool flag2 = false;
			flag1 = InCircle(rootNode->index);
			flag2 = ifPointsCirclrExist(rootNode->index);
			///证明在圈内，同时没有出现过
			if(flag1 && flag2)
			{
				SpacePoint t_point = m_inPoints[rootNode->index];
				in_points.push_back(t_point);
				///此时还需要考虑直接与该点相连的节点(其父亲节点)是否在圈外
				if(NULL != rootNode->fathernode && 0 <= rootNode->fathernode->index && rootNode->fathernode->index <= m_inPoints.size()-1)
				{
					bool flag3 = false;
					bool flag4 = false;
					flag3 = InCircle(rootNode->fathernode->index);
					flag4 = ifPointsCirclrExist(rootNode->fathernode->index);
					///证明在圈外，同时没有出现过
					if(!flag3 && flag4)
					{
						SpacePoint tf_point = m_inPoints[rootNode->fathernode->index];
						out_points.push_back(tf_point);
					}
				}
			}
		}
		///非叶子
		else
		{
			bool flag5 = false;
			bool flag6 = false;
			flag5 = InCircle(rootNode->index);
			flag6 = ifPointsCirclrExist(rootNode->index);
			///证明在圈内，同时没有出现过
			if(flag5 && flag6)
			{
				SpacePoint t_point = m_inPoints[rootNode->index];
				in_points.push_back(t_point);
				///此时还需要考虑直接与该点相连的节点(其父亲节点)是否在圈外
				if(NULL != rootNode->fathernode && 0 <= rootNode->fathernode->index && rootNode->fathernode->index <= m_inPoints.size()-1)
				{
					bool flag7 = false;
					bool flag8 = false;
					flag7 = InCircle(rootNode->fathernode->index);
					flag8 = ifPointsCirclrExist(rootNode->fathernode->index);
					///证明在圈外，同时没有出现过
					if(!flag7 && flag8)
					{
						SpacePoint tf_point = m_inPoints[rootNode->fathernode->index];
						out_points.push_back(tf_point);
					}
				}
			}
			///还需要考虑该节点的叶子结点（递归处理）
			for(int i =0 ;i< rootNode->nextnodesvec.size();i++)
			{
				if(NULL == rootNode->nextnodesvec[i])
					continue;

				GetPointsInCircle(rootNode->nextnodesvec[i],in_points,out_points);
			}
		}
	}
}

/******************************************
生成圈子点的网格
*******************************************/
void CGEComputeNode::createCircleGridandTreeRoot(int num)
{
	if(num <= 0)
		return;

//	m_incirclePoints.clear();
//	m_outcirclePoints.clear();

	m_circlePoints.clear();
	m_circletriangles.clear();

	///获取网格点,遍历该大洲已经存在的树
	/*for(int i=0;i<m_headNodevec.size();i++)
	{
		if(NULL == m_headNodevec[i])
			continue;

		GetPointsInCircle(m_headNodevec[i],m_incirclePoints,m_outcirclePoints);
	}*/

	///得到相应的点集合
	for(int i=0;i<m_incirclePoints.size();i++)
	{
		m_circlePoints.push_back(m_incirclePoints[i]);
	}

	for(int i=0;i<m_outcirclePoints.size();i++)
	{
		m_circlePoints.push_back(m_outcirclePoints[i]);
	}
	getPointParameter=false;

	///生成相应的网格
	CreatGrid(m_circlePoints,m_circletriangles);
	UpdataNeighbor(m_circlePoints,m_circletriangles);//There is an error

	m_circletreevec.clear();
	int size = m_outcirclePoints.size();
	int step = size/(num + 1);
	///也就是设置第一个节点为0~size-1之间
	int startnum = rand()%size;

	for(int i=0;i<num;i++)
	{
		int pointnum = (startnum + i*step)%size;
		//if(pointnum >= m_outcirclePoints.size())
		//	continue;

		///此时必须为圈外部点
		SpacePoint t_point  = m_outcirclePoints[pointnum];
		bool tt_flag = false;
		for(int j=0;j<m_circletreevec.size();j++)
		{
			if(t_point.index == m_circletreevec[j]->index)
				tt_flag = true;
		}
		if(tt_flag)
		{   
			pointnum=(pointnum+1)%size;
			continue;
		}
		
		TreeNode* CircleTreeNode = new TreeNode;
		CircleTreeNode->degree = 0;
		CircleTreeNode->index = t_point.index;
		CircleTreeNode->IP = "";
		CircleTreeNode->nextnodesvec.clear();
		CircleTreeNode->prenodesvec.clear();
		CircleTreeNode->fathernode = NULL;
		m_circletreevec.push_back(CircleTreeNode);

		///一棵树的id号
		m_allcirclenodeindex.clear();
		m_allcirclenodeindex.push_back(t_point.index);

		vector<TreeNode*>t_nodevecofonelayer;
		t_nodevecofonelayer.clear();
		t_nodevecofonelayer.push_back(CircleTreeNode);
		UpdateCircleTree(t_nodevecofonelayer);
	}
}

void CGEComputeNode::UpdateCircleTree(vector<TreeNode *>t_vec)
{
	if(0 == t_vec.size())
	{
		return;
	}
	vector<TreeNode*>t_nodevecofonelayer;
	t_nodevecofonelayer.clear();

	//循环遍历一层的所有节点
	for(int t=0;t<t_vec.size();t++)
	{
		if(NULL == t_vec[t])
			continue;

		//父节点id
		int treenum = t_vec[t]->index;
		if(treenum < 0)
			continue;


		bool flag = false;
		flag = ifExist(m_allcirclenodeindex,treenum);
		if(flag)
			m_allcirclenodeindex.push_back(treenum);

		Triangles t_triangles;
		int sonnum = 0;
		t_triangles.clear();
		//得到相关的三角形
		for(int i=0;i<m_circletriangles.size();i++)
		{
			for(int j =0;j<3;j++)
			{
				if(treenum == m_circletriangles[i].p[j])
				{
					t_triangles.push_back(m_circletriangles[i]);
				}
			}
		}

		for(int i=0;i < t_triangles.size();i++)
		{
			//相关三角形
			for(int k=0;k<3;k++)
			{
				bool tflag1 = false;
				bool tflag2 = false;
				tflag1 = ifExist(m_allcirclenodeindex,t_triangles[i].p[k]);
				tflag2 = ifPointsInCircle(t_triangles[i].p[k]);

				//说明可以生成节点，并且节点属于圈内，放入
				if(tflag1 && tflag2)
				{
					TreeNode* sonnode = new TreeNode;
					sonnode->index = t_triangles[i].p[k];
					sonnode->degree = 0;
					sonnode->nextnodesvec.clear();
					sonnode->fathernode = t_vec[t];

					//将节点的index放入保存vector中
					t_vec[t]->nextnodesvec.push_back(sonnode);
					m_allcirclenodeindex.push_back(t_triangles[i].p[k]);

					//将新生成的节点放入该层的容器中(下次递归时，这就会是父节点序列)
					t_nodevecofonelayer.push_back(sonnode);
					sonnum++;
				}
			}
		}

		///如果该节点没有了圈内的子节点，那么看看外部的，只需看一跳
		if(0 == sonnum)
		{
			for(int n=0;n < t_triangles.size();n++)
			{
				bool tflag3;
				//相关三角形
				for(int m=0;m<3;m++)
				{
					tflag3 = false;
					bool tflag4 = false;
					tflag3 = ifExist(m_allcirclenodeindex,t_triangles[n].p[m]);
					tflag4 = ifPointsOutCircle(t_triangles[n].p[m]);

					//说明可以生成节点，并且点属于圈外，放入
					if(tflag3 && tflag4)
					{
						TreeNode* sonnode = new TreeNode;
						sonnode->index = t_triangles[n].p[m];
						sonnode->degree = 0;
						sonnode->nextnodesvec.clear();
						sonnode->fathernode = t_vec[t];

						//将节点的index放入保存vector中
						m_allcirclenodeindex.push_back(t_triangles[n].p[m]);
						t_vec[t]->nextnodesvec.push_back(sonnode);
						//只能生成一次
						break;
					}
				}

				///只生成一次
				if(tflag3)
					break;
			}
		}
	}

	if(0 != t_nodevecofonelayer.size())
		UpdateCircleTree(t_nodevecofonelayer);
	for(int i=0;i<(int)t_vec.size();i++)
	{
		if(t_vec[i]->nextnodesvec.size()>0)
		continue;
		if(InCircle(t_vec[i]->index))
		{
			double _minangle=INF,_mindis=INF;int minoutpoint=-1;
			for(int j=0;j<(int)m_outcirclePoints.size();j++)
			{
					Point _pt1,_pt2;
					_pt1=city[t_vec[i]->index];
					_pt2=m_outcirclePoints[j];
					//if((_pt2.m-_pt1.m)*(_pt1.m-city[t_vec[i]->fathernode->index].m)<0)
					//	continue;
					if((_pt2.l-_pt1.l)*(_pt1.l-city[t_vec[i]->fathernode->index].l)<0)
						continue;
					double tmpangle=fabs((_pt2.m-_pt1.m)/(_pt2.l-_pt1.l)-(_pt1.m-city[t_vec[i]->fathernode->index].m)/(_pt1.l-city[t_vec[i]->fathernode->index].l));
					if(tmpangle<_minangle)
					{
						_minangle=tmpangle;
						minoutpoint=j;
					}
			}
			if(minoutpoint<0)
			{
				for(int j=0;j<(int)m_outcirclePoints.size();j++)
				{
					Point _pt1,_pt2;
					_pt1=city[t_vec[i]->index];
					_pt2=m_outcirclePoints[j];
					double tmpdis=getDistance(_pt1,_pt2);
					if(tmpdis<_mindis)
					{
						_mindis=tmpdis;
						minoutpoint=j;
					}
				}
			}
			TreeNode* sonnode = new TreeNode;
			sonnode->index =m_outcirclePoints[minoutpoint].index;
			sonnode->degree = 0;
			sonnode->nextnodesvec.clear();
			sonnode->fathernode = t_vec[i];
			t_vec[i]->nextnodesvec.push_back(sonnode);
		}
	}
}

bool CGEComputeNode::ifPointsCirclrExist(int index)
{
	if(m_incirclePoints.size() == 0 && m_outcirclePoints.size() == 0)
		return true;

	for(int i=0;i<m_incirclePoints.size();i++)
	{
		if(m_incirclePoints[i].index == index)
		{
			return false;
		}
	}

	for(int i=0;i<m_outcirclePoints.size();i++)
	{
		if(m_outcirclePoints[i].index == index)
		{
			return false;
		}
	}

	return true;
}

///输入点是圈内点
bool CGEComputeNode::ifPointsInCircle(int index)
{
	for(int i=0;i<m_incirclePoints.size();i++)
		if(index == m_incirclePoints[i].index)
			return true;

	return false;
}

///输入点是圈外点
bool CGEComputeNode::ifPointsOutCircle(int index)
{
	for(int i=0;i<m_outcirclePoints.size();i++)
		if(index == m_outcirclePoints[i].index)
			return true;

	return false;
}
