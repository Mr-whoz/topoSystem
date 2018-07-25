#ifndef WWWWW_H
#define WWWWW_H

#include <QDialog>
#include "ui_wwwww.h"

class wwwww : public QDialog
{
	Q_OBJECT

public:
	wwwww(QWidget *parent = 0);
	~wwwww();

private:
	Ui::wwwwwClass ui;
};

#endif // WWWWW_H
