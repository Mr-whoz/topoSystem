/********************************************************************************
** Form generated from reading ui file 'cgewnd.ui'
**
** Created: Mon Jul 9 09:43:29 2018
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CGEWND_H
#define UI_CGEWND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CGEwndClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CGEwndClass)
    {
    if (CGEwndClass->objectName().isEmpty())
        CGEwndClass->setObjectName(QString::fromUtf8("CGEwndClass"));
    CGEwndClass->resize(600, 400);
    centralWidget = new QWidget(CGEwndClass);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    centralWidget->setGeometry(QRect(0, 36, 600, 341));
    CGEwndClass->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(CGEwndClass);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 600, 24));
    CGEwndClass->setMenuBar(menuBar);
    mainToolBar = new QToolBar(CGEwndClass);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    mainToolBar->setGeometry(QRect(0, 24, 600, 12));
    CGEwndClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(CGEwndClass);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    statusBar->setGeometry(QRect(0, 377, 600, 23));
    CGEwndClass->setStatusBar(statusBar);

    retranslateUi(CGEwndClass);

    QMetaObject::connectSlotsByName(CGEwndClass);
    } // setupUi

    void retranslateUi(QMainWindow *CGEwndClass)
    {
    CGEwndClass->setWindowTitle(QApplication::translate("CGEwndClass", "CGEwnd", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CGEwndClass);
    } // retranslateUi

};

namespace Ui {
    class CGEwndClass: public Ui_CGEwndClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CGEWND_H
