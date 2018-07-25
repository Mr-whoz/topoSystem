#include <QtGui/QApplication>
#include "cgewnd.h"
#include <QTextCodec>
char **nowFile;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    nowFile=argv;
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	CGEwnd w;
	//main window show maximized
	w.showMaximized();
	a.exec();
	//exit the hidden GoogleEarth application
	w.GoogleEarthAppExit();
	return 0;
}
