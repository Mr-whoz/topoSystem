#ifndef GEWIDGET_H
#define GEWIDGET_H

#include <comdef.h>
#include <QWidget>
#include <QResizeEvent>
#include <QMessageBox>
#include <QLabel>
class GeWidget : public QWidget
{
	Q_OBJECT
public:
	GeWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GeWidget();
public:
	//exit the hidden GoogleEarth application
	void Exit();
private:
	//the following is the GoogleEarth Plugin handle
	HWND              m_hMainHandle; 
	HWND              m_hParentHandle;
	HWND              m_hRenderHandle;

protected:
	virtual void resizeEvent(QResizeEvent*);
};
#endif	//GEWIDGET_H