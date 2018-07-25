#include "GEWidget.h"
#include <comdef.h>

#import "C:/Program Files (x86)/Google/Google Earth/client/googleearth.exe" named_guids

using namespace EARTHLib;
IApplicationGEPtr gGoogleEarthApp;
//ICameraInfoGEPtr  defaultCamera;

#define MIN_MAIN_WND_HEIGHT 480
#define MIN_MAIN_WND_WIDTH  480

GeWidget::GeWidget(QWidget *parent, Qt::WFlags flags)
	:QWidget(parent, flags)
{
	// Component Object initialization 
	CoInitialize(NULL);


	//Create Google Earth App
	HRESULT result = gGoogleEarthApp.CreateInstance(__uuidof(ApplicationGE));
	if (S_OK != result)
	{
		QMessageBox::critical(NULL, tr("错误"), tr("加载谷歌地球插件时出现错误，请检查谷歌地球版本，需2015年之前的版本"));
		return;
	}

	//Obtain GoogleEarth Plugin handle(windows api)
	//first we obtain the plugin window handle
	//then place it in QMainWnd, and hide GoogleEarth main window
	OLE_HANDLE renderOLE = gGoogleEarthApp->GetRenderHwnd(); 
	OLE_HANDLE mainOLE   = gGoogleEarthApp->GetMainHwnd(); 

	m_hRenderHandle      = (HWND)renderOLE;
	m_hParentHandle      = GetParent(m_hRenderHandle); 
	m_hMainHandle        = (HWND)(mainOLE); 
	WId id = winId(); 
	SetParent(m_hParentHandle, (HWND)id); 
	//hide GoogleEarth main window
	ShowWindow(m_hMainHandle, SW_HIDE); 

	//wait for GoogleEarth Loading data
	if (SUCCEEDED(result)) 
	{ 
		while (gGoogleEarthApp->IsInitialized() == 0) 
		{ 
			Sleep(500);
		} 
	} 

	//resize main window 
	//and place Google earth
	//using Win32 API
	RECT rcGE;
	::GetWindowRect(m_hParentHandle, &rcGE);
	//rcGE.bottom += ui.menuBar->size().height();
	setMinimumSize(MIN_MAIN_WND_WIDTH, MIN_MAIN_WND_HEIGHT);
	resize(rcGE.right - rcGE.left, rcGE.bottom - rcGE.top);
	::SetWindowPos(m_hParentHandle, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	::SetWindowPos(m_hRenderHandle, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
}

GeWidget::~GeWidget()
{

}

void GeWidget::resizeEvent( QResizeEvent* e)
{
	QSize sz = e->size();
	::SetWindowPos(m_hParentHandle, NULL, 0, 0, sz.width(), sz.height() ,SWP_NOZORDER);
	::SetWindowPos(m_hRenderHandle, NULL, 0, 0, sz.width(), sz.height() ,SWP_NOZORDER);

}

void GeWidget::Exit()
{
	gGoogleEarthApp.Release();
	//close the cloaked GoogleEarth main window
	PostMessage(m_hMainHandle, WM_QUIT, 0, 0); 
}