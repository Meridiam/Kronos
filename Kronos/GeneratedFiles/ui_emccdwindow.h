/********************************************************************************
** Form generated from reading UI file 'emccdwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMCCDWINDOW_H
#define UI_EMCCDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>

QT_BEGIN_NAMESPACE

class Ui_EMCCDWindow
{
public:

    void setupUi(QOpenGLWidget *EMCCDWindow)
    {
        if (EMCCDWindow->objectName().isEmpty())
            EMCCDWindow->setObjectName(QStringLiteral("EMCCDWindow"));
        EMCCDWindow->resize(400, 300);

        retranslateUi(EMCCDWindow);

        QMetaObject::connectSlotsByName(EMCCDWindow);
    } // setupUi

    void retranslateUi(QOpenGLWidget *EMCCDWindow)
    {
        EMCCDWindow->setWindowTitle(QApplication::translate("EMCCDWindow", "EMCCDWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EMCCDWindow: public Ui_EMCCDWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMCCDWINDOW_H
