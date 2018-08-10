/********************************************************************************
** Form generated from reading UI file 'kronos.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KRONOS_H
#define UI_KRONOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "renderwindow.h"

QT_BEGIN_NAMESPACE

class Ui_KronosClass
{
public:
    QPushButton *forwardbtn;
    QPushButton *backbtn;
    QPushButton *leftbtn;
    QPushButton *rightbtn;
    QPushButton *connectbtn;
    QLabel *label;
    QPushButton *disconnectbtn;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QLineEdit *yvel;
    QLineEdit *xvel;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *runbtn;
    QPushButton *stopbtn;
    RenderWindow *emccdDisp;
    RenderWindow *cmosDisp;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *drawbtn;

    void setupUi(QWidget *KronosClass)
    {
        if (KronosClass->objectName().isEmpty())
            KronosClass->setObjectName(QStringLiteral("KronosClass"));
        KronosClass->resize(1124, 947);
        forwardbtn = new QPushButton(KronosClass);
        forwardbtn->setObjectName(QStringLiteral("forwardbtn"));
        forwardbtn->setGeometry(QRect(120, 180, 75, 23));
        backbtn = new QPushButton(KronosClass);
        backbtn->setObjectName(QStringLiteral("backbtn"));
        backbtn->setGeometry(QRect(120, 240, 75, 23));
        leftbtn = new QPushButton(KronosClass);
        leftbtn->setObjectName(QStringLiteral("leftbtn"));
        leftbtn->setGeometry(QRect(40, 210, 75, 23));
        rightbtn = new QPushButton(KronosClass);
        rightbtn->setObjectName(QStringLiteral("rightbtn"));
        rightbtn->setGeometry(QRect(200, 210, 75, 23));
        connectbtn = new QPushButton(KronosClass);
        connectbtn->setObjectName(QStringLiteral("connectbtn"));
        connectbtn->setGeometry(QRect(20, 20, 75, 23));
        label = new QLabel(KronosClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 80, 251, 20));
        label->setAlignment(Qt::AlignCenter);
        disconnectbtn = new QPushButton(KronosClass);
        disconnectbtn->setObjectName(QStringLiteral("disconnectbtn"));
        disconnectbtn->setGeometry(QRect(20, 50, 75, 23));
        label_2 = new QLabel(KronosClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(100, 40, 91, 16));
        lineEdit = new QLineEdit(KronosClass);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 140, 113, 20));
        label_3 = new QLabel(KronosClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(100, 120, 47, 13));
        yvel = new QLineEdit(KronosClass);
        yvel->setObjectName(QStringLiteral("yvel"));
        yvel->setGeometry(QRect(360, 160, 113, 20));
        xvel = new QLineEdit(KronosClass);
        xvel->setObjectName(QStringLiteral("xvel"));
        xvel->setGeometry(QRect(360, 200, 113, 20));
        label_4 = new QLabel(KronosClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(360, 180, 61, 16));
        label_5 = new QLabel(KronosClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(360, 140, 51, 16));
        runbtn = new QPushButton(KronosClass);
        runbtn->setObjectName(QStringLiteral("runbtn"));
        runbtn->setGeometry(QRect(360, 230, 75, 23));
        stopbtn = new QPushButton(KronosClass);
        stopbtn->setObjectName(QStringLiteral("stopbtn"));
        stopbtn->setGeometry(QRect(440, 230, 75, 23));
        emccdDisp = new RenderWindow(KronosClass);
        emccdDisp->setObjectName(QStringLiteral("emccdDisp"));
        emccdDisp->setGeometry(QRect(20, 520, 551, 411));
        cmosDisp = new RenderWindow(KronosClass);
        cmosDisp->setObjectName(QStringLiteral("cmosDisp"));
        cmosDisp->setGeometry(QRect(580, 520, 531, 411));
        cmosDisp->setToolTipDuration(-13);
        label_6 = new QLabel(KronosClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(280, 500, 47, 13));
        label_7 = new QLabel(KronosClass);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(830, 500, 47, 13));
        drawbtn = new QPushButton(KronosClass);
        drawbtn->setObjectName(QStringLiteral("drawbtn"));
        drawbtn->setGeometry(QRect(410, 350, 75, 23));

        retranslateUi(KronosClass);

        QMetaObject::connectSlotsByName(KronosClass);
    } // setupUi

    void retranslateUi(QWidget *KronosClass)
    {
        KronosClass->setWindowTitle(QApplication::translate("KronosClass", "Kronos", nullptr));
        forwardbtn->setText(QApplication::translate("KronosClass", "Forward", nullptr));
        backbtn->setText(QApplication::translate("KronosClass", "Backward", nullptr));
        leftbtn->setText(QApplication::translate("KronosClass", "Left", nullptr));
        rightbtn->setText(QApplication::translate("KronosClass", "Right", nullptr));
        connectbtn->setText(QApplication::translate("KronosClass", "Connect", nullptr));
        label->setText(QApplication::translate("KronosClass", "Output", nullptr));
        disconnectbtn->setText(QApplication::translate("KronosClass", "Disconnect", nullptr));
        label_2->setText(QApplication::translate("KronosClass", "DISCONNECTED", nullptr));
        label_3->setText(QApplication::translate("KronosClass", "Units", nullptr));
        label_4->setText(QApplication::translate("KronosClass", "X Velocity", nullptr));
        label_5->setText(QApplication::translate("KronosClass", "Y Velocity", nullptr));
        runbtn->setText(QApplication::translate("KronosClass", "Run", nullptr));
        stopbtn->setText(QApplication::translate("KronosClass", "Stop", nullptr));
        label_6->setText(QApplication::translate("KronosClass", "EMCCD", nullptr));
        label_7->setText(QApplication::translate("KronosClass", "CMOS", nullptr));
        drawbtn->setText(QApplication::translate("KronosClass", "Draw", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KronosClass: public Ui_KronosClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KRONOS_H
