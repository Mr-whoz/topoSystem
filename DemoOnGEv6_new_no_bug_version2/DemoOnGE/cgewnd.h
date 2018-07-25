#ifndef CGEWND_H
#define CGEWND_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QPainter>
#include "ui_cgewnd.h"
#include "GEWidget.h"
#include "./computenode/cgecomputenode.h"
#include "serverSet.h"
#include "Thread.h"
#include "ThreadShow.h"
class CGEwnd : public QMainWindow
{
	Q_OBJECT

public:
	CGEwnd(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CGEwnd();

public:
	//exit the hidden GoogleEarth application
	void GoogleEarthAppExit();
private:
	Ui::CGEwndClass ui;
	GeWidget *geWidget;
	vector<CGEComputeNode*> comnode;
	CGEComputeNode* comnodetmp;	
	//save KML data file path
	QString           m_strKmlFile;
	QString           m_strScrnShotFile;
	QLabel*			  m_title;
	Thread myTrace;
	ThreadShow myShow;
	ServerSet *ooo;
private:
	QMenu *settingMenu;	//设置菜单
	QMenu *fileMenu;		//文件菜单
	QMenu *showMenu;		//演示菜单
	QMenu *viewMenu;		//视图菜单
	QMenu *helpMenu;		//帮助菜单

	QAction *loadKMLAction;		//打开kml文件
	QAction *inputFileAction;		//打开数据文件
	QAction *loadAllKmlAction;		//打开所有kml
	QAction *traceRouteAction;      //open traceRoute.kml
	QAction *topoAction;            //open topo.kml
	QAction *exitAction;		//退出程序
	QAction *stsBarAction;	//状态栏隐藏显示
	QAction *gotoDefaultViewAction;		//显示默认视角
	QAction *aboutAction;	//关于菜单
	QAction *aboutQtAction;	//关于Qt菜单
	QAction *routetopoAction;
	QAction *AStopoAction;
	QAction *serverSetting; //服务器设置

private:
	void creatActions();			//创建动作
	void createMenus();				//创建菜单栏
	void createToolBars();			//创建工具栏

private slots:
	void about();
	void threadFinish();
	void myShowfinish();
	void slot_loadKML();			//打开kml文件
	void slot_inputFile();				//导入数据文件
	void slot_traceRoute();			//show traceRoute process
	void slot_exit();				//退出程序
	void slot_gotoDefaultView();		//显示默认视角
	void setStsBarVisibleOrNot();	//设置状态栏是否显示
	void slot_serverSetting();			//弹出服务器设置窗口
};

#endif // CGEWND_H
