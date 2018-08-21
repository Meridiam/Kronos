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
#include "emccdwindow.h"
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
    EMCCDWindow *emccdDisp;
    RenderWindow *cmosDisp;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *toggleScaleBtn;
    QPushButton *cmosMoveBtn;
    QPushButton *acquireBtn;
    QLineEdit *xSteps;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *yLines;
    QLabel *label_10;
    QLabel *xWidth;
    QLabel *label_12;
    QLabel *yHeight;
    QLabel *inputLabel;
    QLineEdit *emccdExposure;
    QLineEdit *emccdTemp;
    QLabel *label_11;
    QLabel *label_13;
    QPushButton *initBtn;
    QPushButton *abrtBtn;
    QLineEdit *dir;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *emccdTempDisp;

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
        emccdDisp = new EMCCDWindow(KronosClass);
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
        toggleScaleBtn = new QPushButton(KronosClass);
        toggleScaleBtn->setObjectName(QStringLiteral("toggleScaleBtn"));
        toggleScaleBtn->setGeometry(QRect(1020, 490, 75, 23));
        cmosMoveBtn = new QPushButton(KronosClass);
        cmosMoveBtn->setObjectName(QStringLiteral("cmosMoveBtn"));
        cmosMoveBtn->setGeometry(QRect(590, 490, 75, 23));
        acquireBtn = new QPushButton(KronosClass);
        acquireBtn->setObjectName(QStringLiteral("acquireBtn"));
        acquireBtn->setGeometry(QRect(30, 490, 75, 23));
        xSteps = new QLineEdit(KronosClass);
        xSteps->setObjectName(QStringLiteral("xSteps"));
        xSteps->setGeometry(QRect(30, 440, 113, 20));
        label_8 = new QLabel(KronosClass);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(30, 420, 47, 13));
        label_9 = new QLabel(KronosClass);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(160, 420, 47, 13));
        yLines = new QLineEdit(KronosClass);
        yLines->setObjectName(QStringLiteral("yLines"));
        yLines->setGeometry(QRect(160, 440, 113, 20));
        label_10 = new QLabel(KronosClass);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(580, 390, 47, 13));
        xWidth = new QLabel(KronosClass);
        xWidth->setObjectName(QStringLiteral("xWidth"));
        xWidth->setGeometry(QRect(640, 390, 47, 13));
        label_12 = new QLabel(KronosClass);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(580, 410, 47, 13));
        yHeight = new QLabel(KronosClass);
        yHeight->setObjectName(QStringLiteral("yHeight"));
        yHeight->setGeometry(QRect(640, 410, 47, 13));
        inputLabel = new QLabel(KronosClass);
        inputLabel->setObjectName(QStringLiteral("inputLabel"));
        inputLabel->setGeometry(QRect(680, 90, 47, 13));
        emccdExposure = new QLineEdit(KronosClass);
        emccdExposure->setObjectName(QStringLiteral("emccdExposure"));
        emccdExposure->setGeometry(QRect(30, 390, 113, 20));
        emccdTemp = new QLineEdit(KronosClass);
        emccdTemp->setObjectName(QStringLiteral("emccdTemp"));
        emccdTemp->setGeometry(QRect(160, 390, 113, 20));
        label_11 = new QLabel(KronosClass);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(30, 370, 61, 16));
        label_13 = new QLabel(KronosClass);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(160, 370, 47, 13));
        initBtn = new QPushButton(KronosClass);
        initBtn->setObjectName(QStringLiteral("initBtn"));
        initBtn->setGeometry(QRect(490, 470, 75, 23));
        abrtBtn = new QPushButton(KronosClass);
        abrtBtn->setObjectName(QStringLiteral("abrtBtn"));
        abrtBtn->setGeometry(QRect(490, 490, 75, 23));
        dir = new QLineEdit(KronosClass);
        dir->setObjectName(QStringLiteral("dir"));
        dir->setGeometry(QRect(590, 10, 511, 20));
        label_14 = new QLabel(KronosClass);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(500, 10, 91, 20));
        label_15 = new QLabel(KronosClass);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(290, 390, 47, 13));
        emccdTempDisp = new QLabel(KronosClass);
        emccdTempDisp->setObjectName(QStringLiteral("emccdTempDisp"));
        emccdTempDisp->setGeometry(QRect(340, 390, 47, 13));

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
        toggleScaleBtn->setText(QApplication::translate("KronosClass", "100x", nullptr));
        cmosMoveBtn->setText(QApplication::translate("KronosClass", "Move", nullptr));
        acquireBtn->setText(QApplication::translate("KronosClass", "Acquire", nullptr));
        xSteps->setText(QString());
        label_8->setText(QApplication::translate("KronosClass", "X Steps", nullptr));
        label_9->setText(QApplication::translate("KronosClass", "Y Lines", nullptr));
        yLines->setText(QString());
        label_10->setText(QApplication::translate("KronosClass", "Width:", nullptr));
        xWidth->setText(QApplication::translate("KronosClass", "TextLabel", nullptr));
        label_12->setText(QApplication::translate("KronosClass", "Height:", nullptr));
        yHeight->setText(QApplication::translate("KronosClass", "TextLabel", nullptr));
        inputLabel->setText(QApplication::translate("KronosClass", "Input", nullptr));
        label_11->setText(QApplication::translate("KronosClass", "EMExposure", nullptr));
        label_13->setText(QApplication::translate("KronosClass", "SetTemp", nullptr));
        initBtn->setText(QApplication::translate("KronosClass", "Initialize", nullptr));
        abrtBtn->setText(QApplication::translate("KronosClass", "Abort", nullptr));
        label_14->setText(QApplication::translate("KronosClass", "Driver Directory:", nullptr));
        label_15->setText(QApplication::translate("KronosClass", "Temp:", nullptr));
        emccdTempDisp->setText(QApplication::translate("KronosClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KronosClass: public Ui_KronosClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KRONOS_H
