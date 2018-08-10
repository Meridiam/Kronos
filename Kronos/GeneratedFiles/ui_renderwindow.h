/********************************************************************************
** Form generated from reading UI file 'renderwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERWINDOW_H
#define UI_RENDERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>

QT_BEGIN_NAMESPACE

class Ui_RenderWindow
{
public:

    void setupUi(QOpenGLWidget *RenderWindow)
    {
        if (RenderWindow->objectName().isEmpty())
            RenderWindow->setObjectName(QStringLiteral("RenderWindow"));
        RenderWindow->resize(400, 300);

        retranslateUi(RenderWindow);

        QMetaObject::connectSlotsByName(RenderWindow);
    } // setupUi

    void retranslateUi(QOpenGLWidget *RenderWindow)
    {
        RenderWindow->setWindowTitle(QApplication::translate("RenderWindow", "RenderWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenderWindow: public Ui_RenderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERWINDOW_H
