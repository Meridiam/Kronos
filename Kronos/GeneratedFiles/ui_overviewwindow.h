/********************************************************************************
** Form generated from reading UI file 'overviewwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERVIEWWINDOW_H
#define UI_OVERVIEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>

QT_BEGIN_NAMESPACE

class Ui_overviewwindow
{
public:

    void setupUi(QOpenGLWidget *overviewwindow)
    {
        if (overviewwindow->objectName().isEmpty())
            overviewwindow->setObjectName(QStringLiteral("overviewwindow"));
        overviewwindow->resize(400, 300);

        retranslateUi(overviewwindow);

        QMetaObject::connectSlotsByName(overviewwindow);
    } // setupUi

    void retranslateUi(QOpenGLWidget *overviewwindow)
    {
        overviewwindow->setWindowTitle(QApplication::translate("overviewwindow", "overviewwindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class overviewwindow: public Ui_overviewwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERVIEWWINDOW_H
