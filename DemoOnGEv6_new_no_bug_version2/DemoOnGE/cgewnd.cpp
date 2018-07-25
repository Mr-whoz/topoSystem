#include "cgewnd.h"
#include <comdef.h>
#include <QMessageBox>
#include <QResizeEvent>
#include <QFileDialog>
#include <fstream>
#include <string>
#include<QtGui>
#include"serverSet.h"

#import "C:/Program Files (x86)/Google/Google Earth/client/googleearth.exe"  named_guids
using namespace EARTHLib;
extern IApplicationGEPtr gGoogleEarthApp;
extern char **nowFile;
ICameraInfoGEPtr defaultCamera;

CGEwnd::CGEwnd(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	ui.setupUi(this);
	this->setWindowTitle(tr("全球互联网拓扑测量演示系统  版本：V1.0"));
	this->setWindowIcon(QIcon(":/CGEwnd/Resources/logo.png"));
	creatActions();
	createMenus();
	createToolBars();
	//将Google Earth放在CentralWidget中
	comnodetmp=new CGEComputeNode;
	geWidget = new GeWidget(this);
	setCentralWidget(geWidget);
	connect(&myTrace,SIGNAL(finished()),this,SLOT(threadFinish()));
	connect(&myShow,SIGNAL(finished()),this,SLOT(myShowfinish()));
	ooo = new ServerSet(this,&myTrace);
	
}

CGEwnd::~CGEwnd()
{
	
}
/*创建动作*/
void CGEwnd::creatActions()
{
	loadKMLAction = new QAction(tr("打开KML文件(&O)"),this);
	loadKMLAction->setIcon(QIcon(":/CGEwnd/Resources/open.png"));
	loadKMLAction->setToolTip(tr("打开 .kml或 .kmz文件..."));
	loadKMLAction->setShortcut(tr("Ctrl+O"));
	connect(loadKMLAction, SIGNAL(triggered()), this, SLOT(slot_loadKML()));

	inputFileAction = new QAction(tr("打开坐标数据文件"), this);
	connect(inputFileAction, SIGNAL(triggered()), this, SLOT(slot_inputFile()));


	traceRouteAction=new QAction(tr("拓扑信息收集演示"),this);
	traceRouteAction->setIcon(QIcon(":/CGEwnd/Resources/tance.png"));
	connect(traceRouteAction,SIGNAL(triggered()), this, SLOT(slot_traceRoute()));
	exitAction = new QAction(tr("退出(&X)"),this);
	exitAction->setIcon(QIcon(":/CGEwnd/Resources/close.png"));
	exitAction->setToolTip(tr("退出程序"));
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(slot_exit()));
	gotoDefaultViewAction = new QAction(tr("默认视角"), this);
	gotoDefaultViewAction->setIcon(QIcon(":/CGEwnd/Resources/earth.png"));
	gotoDefaultViewAction->setToolTip(tr("默认视角"));
	gotoDefaultViewAction->setShortcut(tr("Ctrl+0"));
	connect(gotoDefaultViewAction, SIGNAL(triggered()), this, SLOT(slot_gotoDefaultView()));

	stsBarAction = new QAction(tr("状态栏(&S)"),this);
	stsBarAction->setCheckable(true);			//设置可选
	stsBarAction->setChecked(true);				//初始化时窗口显示状态栏，故设置初试值为选中
	connect(stsBarAction, SIGNAL(triggered()), this, SLOT(setStsBarVisibleOrNot()));

	aboutAction = new QAction(tr("关于软件(&A)"),this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("关于QT(&Q)"),this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	serverSetting = new QAction(tr("数据采集服务器设置"),this);
	connect(serverSetting,SIGNAL(triggered()),this,SLOT(slot_serverSetting()));
}

void CGEwnd::slot_serverSetting()
{
	if (myTrace.isRunning())
	{
		QMessageBox::information(this, tr("提示"),
			QString(tr("正在进行traceroute，请这次traceroute完成之后再进行traceroute")));
		return;
	}
	else
	{
		//ServerSet*ooo = new ServerSet(this,&myTrace);
		ooo->show();
	}
	return;
}
void CGEwnd::threadFinish()
{
	QMessageBox::information(this, tr("提示"),
		QString(tr("traceroute完毕！")));
}
void CGEwnd::myShowfinish()
{
	QMessageBox::information(this, tr("提示"),
		QString(tr("traceroute解析完毕完毕！")));
}
void CGEwnd::slot_inputFile()
{
	comnodetmp->initialize();
	string useLessString;

    if(comnodetmp->ReadPointFromFile(useLessString,0))
	{
		comnodetmp->createTreeRoot(TREENUM);		//生成TREENUM个树
		comnodetmp->solute();
		comnodetmp->traceRoute();
	}
}

/*退出软件*/
void CGEwnd::slot_traceRoute()
{
	QString IP_file = QFileDialog::getOpenFileName(this, QObject::tr("打开IP文件"),"",QObject::tr("")); 
	QMessageBox::information(this, tr("提示"),
		QString(tr("进行traceroute解析,解析完成后请在同文件下打开结果")));
	myShow.setFile(IP_file);
	myShow.start();
}


void CGEwnd::createMenus()
{
	//fileMenu = menuBar()->addMenu(tr("文件(&F)"));//menuBar()是QMainWindow中的函数,返回QMenuBar*,addMenu()是QMenuBar中的函数，返回QMenu*;
	settingMenu = menuBar()->addMenu(tr("采集(&S)"));
	settingMenu ->addAction(serverSetting);

	showMenu = menuBar()->addMenu(tr("解析(&S)"));
	showMenu->addAction(gotoDefaultViewAction);
	showMenu->addSeparator();
	showMenu->addAction(traceRouteAction);

	fileMenu = ui.menuBar->addMenu(tr("文件(&F)"));//ui.menuBar和下面的menuBar所指的对象为同一个，即*this的菜单栏
	fileMenu->addAction(loadKMLAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	viewMenu = menuBar()->addMenu(tr("视图(&V)"));
	viewMenu->addAction(ui.mainToolBar->toggleViewAction());
	viewMenu->addAction(stsBarAction);

	helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
	helpMenu->addAction(aboutAction);
	//helpMenu->addAction(aboutQtAction);

}
/*创建工具栏*/
void CGEwnd::createToolBars()
{
	ui.mainToolBar->setWindowTitle(tr("工具栏"));
	ui.mainToolBar->addAction(loadKMLAction);
    ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(gotoDefaultViewAction);
	ui.mainToolBar->addAction(traceRouteAction);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(exitAction);
}
/*退出Google earth*/
void CGEwnd::GoogleEarthAppExit()
{
	//退出Google earth
	geWidget->Exit();
}

void CGEwnd::about()
{
	//获取Google地球的版本信息
	int verMajor = gGoogleEarthApp->GetVersionMajor();
	int verMinor = gGoogleEarthApp->GetVersionMinor();

	QMessageBox::about(this, tr("关于本软件"),
		QString(tr("本软件系统使用QT开发，\n"
		"QT版本4.4.0\n"
		"集成了Google Earth地球显示功能，\n"
		"Google Earth版本号为%1.%2")).arg(verMajor).arg(verMinor));
}

void CGEwnd::setStsBarVisibleOrNot()
{
	//检查stsBarAction的选中状态，选中则显示状态栏；否，则不显示
	if (stsBarAction->isChecked())
	{
		ui.statusBar->setVisible(true);
	}
	else
	{
		ui.statusBar->setVisible(false);
	}
}
/*导入KML文件*/
void CGEwnd::slot_loadKML()
{
	QString strPath;
	if (m_strKmlFile.isEmpty())
	{
		strPath = "";
	}
	else
	{
		strPath = m_strKmlFile.left(m_strKmlFile.lastIndexOf('\\'));
	}
	m_strKmlFile = QFileDialog::getOpenFileName(this, tr("打开.kml 或 kmz文件..."),
		strPath,
		tr("KML (*.kml *.kmz)")); 

	//_bstr_t strFileName = L"E:/path.kml";


	if (!m_strKmlFile.isEmpty())
	{
		//this string format conversion exits
		// for OpenKmlFile takes wchar_t array as input
		//MAX_PATH is a constant defined by windows
		wchar_t wchPath[MAX_PATH];
		memset(wchPath, 0, sizeof (wchar_t) * MAX_PATH);
		m_strKmlFile.toWCharArray(wchPath);
		gGoogleEarthApp->OpenKmlFile(wchPath, 1);

	}
}
/*导入数据文件*/

/*退出软件*/
void CGEwnd::slot_exit()
{
	if (QMessageBox::Yes != QMessageBox::information(this, tr("提示"),
		QString(tr("是否退出本程序")),QMessageBox::Yes |QMessageBox::Cancel))
	{
		return;
	}

	close();
}
/*显示默认视图*/
void CGEwnd::slot_gotoDefaultView()
{

	defaultCamera = gGoogleEarthApp->GetCamera(1);

	// 设置camera参数
	defaultCamera->PutRange(1000);
	defaultCamera->PutFocusPointLongitude(108.535353);
	defaultCamera->PutFocusPointLatitude(33.717171);
	defaultCamera->PutFocusPointAltitude(12000000);
	defaultCamera->PutFocusPointAltitudeMode(RelativeToGroundAltitudeGE);
	defaultCamera->PutTilt(0);
	defaultCamera->PutAzimuth(0);
	gGoogleEarthApp->SetCamera(defaultCamera, 0.2);

}