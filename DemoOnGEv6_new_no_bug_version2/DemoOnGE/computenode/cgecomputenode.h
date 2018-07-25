#pragma once

#include <QtGui>
#include <QObject>
#include <vector>
#include "cgedataformatdefine.h"
#include "cgepublicmath.h"
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

#define EMAX 2000 // 最大边数
#define TREENUM 3 //产生的树个数
#define PATHNUM 20//路径个数
#define portCircleRadius 0.3
#define portSize 0.3
#define INF 0x7ffffff
using namespace std;

typedef struct City
{
	string cityName;
	string countryDomain;
	double m,l;	
}City;

typedef struct Point
{
	double l,m;
	Point& operator=(const City& cityToPoint)
	{
		l=cityToPoint.l;
		m=cityToPoint.m;
		return *this;
	}
	Point& operator=(const SpacePoint& spacepointtopoint)
	{
		l=spacepointtopoint.y;
		m=spacepointtopoint.x;
		return *this;
	}
}Point;

typedef struct Circle
{
	Point center;double radius;
}Circle;


typedef struct Simtime
{
	int date;
	int month;
	int year;
	int hour;
    friend bool operator<(const Simtime &,const Simtime &);
	friend Simtime operator+(const Simtime&,const int&);
	friend Simtime operator+(const Simtime&,const Simtime&);
}Simtime;


class CGEComputeNode :public QObject
{
	Q_OBJECT

public:
	CGEComputeNode();
	~CGEComputeNode();
   
	//初始化变量
	void initialize();
	//读入经度点，予以保存
	bool ReadPointFromFile(string&,int);
	void InitSimtime(Simtime&,int,int,int,int);
	
	//调用算法，生成网格点
	void CreatGrid(Points _points, Triangles &_triangles);
	void UpdataNeighbor(Points _points, Triangles &_triangles);
	void Deletetriangles();
	
	//得到某个节点(随机)的树状拓扑
	void UpdataTree(Triangles &_triangles,/*TreeNode* fathernode*/vector<TreeNode *>t_vec);
	void createTreeRoot(int num);
	void createCircleGridandTreeRoot(int num);
    void changeBagIcon(char[]);
	void drawAsIcon(Point,Simtime,int);


	//保存生成的网络拓扑
	void SaveGridPoint();
	
	//向外面返回根节点vector
	vector<TreeNode*> getTreeNodevec(){return m_headNodevec;}

	//获取到节点的路径
	vector<int> TreePathToDstNode(int index);
	//产生路径
	void createPathToTheROOTnode(TreeNode* edgeNode);
	//获取所有树的叶节点
	void createAllEdgeNodes();
	//获取单个树的所有叶节点
	void createAllEdgeNodesofSingleTree(TreeNode* rootTreeNode);
	//生成若干路径
	void createSomePathes();


	void solute();//圈外拓扑生成过程
	void toporoute();
	
	double getDistance(Point _pt1, Point _pt2);
	void topographyformtraceroute();//圈内traceroute生成过程，遍历整个树
	void topographyFormTraceRoute(TreeNode*,Simtime &);// 圈内traceroute生成过程及调用topographyformtraceroute
	void topographyFormTopo();//圈内拓扑生成的过程，遍历整个树
	void process1(int idRoot,int idChild,Simtime &st);//画出线段的两个端点（为城市id号）及他们的连线，还有仿真时间
	void showTraceRouteDfs(TreeNode* ,Simtime &);//显示收发包的traceroute过程
	void drawBag(int,int,Simtime &);//画两点之间收发包时间过程
	void drawPoint(int,Simtime);//画出一个点（路由器或主机）
	void drawPoint(Point);//为了夏威夷专门写的画点函数
	void getCircle(int circlenum,int citynum);//为北美和欧洲获得层析成像所画的圆
	double deal(double i);//没有必要知道具体含义的函数
	double deal2(double i);//同上
	void dfs(TreeNode* root,Simtime &st);//遍历整个树
	void getPoint(double slong,double smag,double elong,double emag,int &size);//获得线段上的多个节点，以方便画树
	void drawLine(double a1,double b1,double c1,double d1,Simtime &st);//画出一条（实/虚）线，实虚由traceroute变量的值决定
	bool incircle(Circle blackhole,Point input);//判断一个点是否在圆内
	void traceRoute(void);//traceroute过程
	void addSpacetime(Simtime &st1);
	double change(double l);//没有必要知道这个函数的意义 
	bool isLeapYear(int);//判断一年是否为闰年
	void generateOutAndIn(int,int);//产生圈内圈外的节点，生成三角形
	void routedfs(void);
	int continentId;//获得该对象所代表的大洲号
	bool InCircle(Point);//判断一个点是否在圆内
	bool InCircle(int);//判断一个城市的索引号是否在圆内
	bool InCircle(Point,Point,double);
	void changeIcon(char[],double);//更换图标和图标大小，输入为图标的文件名，需要放置在allcity文件夹里
	void drawPointport(Point,Simtime);
	void getAllAsDomain(void);
	void astopoForm(void); 
	void drawTerminal(Point,char[],double,Simtime,Simtime _endtime);
	void changeLookAt(double,double,double,Simtime);
	//double getDistance(Point _pt1,Point _pt2);
	bool ifLineCross(Point startPoint1,Point endPoint1,Point startPoint2,Point endPoint2);
	inline int crossProduct(Point _pt1,Point _pt2,Point _pt3)
	{
		return (_pt2.l-_pt1.l)*(_pt3.m-_pt1.m)-(_pt3.l-_pt1.l)*(_pt2.m-_pt1.m);
	}
    void changeLineStyle(int colorid,int lineWidth,double alpha=1);
	bool ifHeight;
	bool ifStatic;
	bool ifDotLine;
	bool ifaslevel;
	bool ifastoas;
	bool ifTerminalDisappear;
	bool ifbfs;
	bool ifneedoutincirclepoint;
	double minLongitude;
	double maxLongitude;
	double minLatitude;
	double maxLatitude;
	Point lastPoint;
	Point prevPoint;

	void GetPointsInCircle(TreeNode* rootNode,Points& in_points, Points& out_points);
	void UpdateCircleTree(vector<TreeNode *>);
	Point getCrossPoint(int nextcityindex,int precityindex,double radius=portCircleRadius); 
	
	void drawLine(int startPointIndex,int endPointIndex,Simtime &st);//画线，实现或虚线，输入参数为起始和终了节点的城市索引号

    int cityNum;//记录了该大洲所有城市个数
	string outputfile;//输出的文件名
	Simtime simtime;
    Simtime begintime;
	Simtime endtime;//仿真的结束时间
	vector<Point> outCityIndex;//the city go out of the topo
	vector<vector<vector<SpacePoint>>> getAllTreePathPoints(){return m_AllTreePathPoints;}

	struct VariousAs
	{
		Point asCenter;
		vector<int> cityInAS;
		int Height;
	}ASNo[200];

private:
	//内联函数
	inline bool ifExist(vector<int>vec, int index); 
	static const int Height=150000;
	static const int heightlevel1=150000;
	static const int heightlevel2=1500000;

	inline bool ifExistEdge(SpacePoint tp1, SpacePoint tp2);
	inline bool ifPointsCirclrExist(int index);
	inline bool ifPointsInCircle(int index);
	inline bool ifPointsOutCircle(int index);
	bool getPointParameter;

private:

	Points m_inPoints;                                         //in Points
	PublicMath *m_publicMath;                                  //public math
	Triangles m_triangles;                                     //public triangles
	vector<pair<SpacePoint, SpacePoint> > m_inseaPoints;       //the Points in the sea   

	City city[1000];        //city in a continent

	Point portPoint;
	vector<Circle> circle;
	vector<int>m_allnodesindex;
	vector<int>m_pathIndex;			//路径上的点的索引号
	vector<SpacePoint>		m_NodePathPoints;	//边缘节点到根节点路径上所有的点
	vector<vector<SpacePoint>> m_TreePathPoints;//树的所有边缘节点到该树跟节点路径上的点
	vector<vector<vector<SpacePoint>>> m_AllTreePathPoints;//所有树<TREENUM个>，每个树<N >点
	vector<int> CityUsed;
	TreeNode* m_headNode;

	vector<TreeNode*>m_headNodevec;                 //保存根节点
	vector<TreeNode*>m_nodesvec;                    //一层的根节点
	vector<vector<TreeNode*>> m_EdgeNodesVct;//所有树里的叶节点,TREENUM行,每行的元素为该树的边缘节点
	vector<TreeNode*> vct_Node;								//单颗树里的叶子节点
	int circlenum;
	int traceroute;
	int leafNum;
	int AsNum;
	string nowPenColor;
	string nowColor;	
	vector<int> routeStack;
	vector<int> routeStack2;
	vector<Point> pointStack;
	ofstream fout;
	vector<TreeNode *> rootVector;
	
	Simtime timeRecord;
	Simtime anotherTimeRecord;
	int sameTimeTraceRoute;
    vector<Point> pointVector;

	Points m_incirclePoints;                                   // a continent may have some trees and a circle, some points may in the circle and ,of course some are not;
	Points m_outcirclePoints;                                  // a continent may have some trees and a circle, some points may in the circle and ,of course some are not;
	Points m_circlePoints;                                     // the sum of the above two
	Triangles m_circletriangles;                               // the triangles with the points
	vector<TreeNode *> m_circletreevec;                        // the points with the circle may generate some trees
	vector<int> m_allcirclenodeindex;
};