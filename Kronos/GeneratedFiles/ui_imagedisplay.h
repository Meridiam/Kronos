/********************************************************************************
** Form generated from reading UI file 'imagedisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEDISPLAY_H
#define UI_IMAGEDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageDisplay
{
public:
    QLabel *hilabel;

    void setupUi(QWidget *ImageDisplay)
    {
        if (ImageDisplay->objectName().isEmpty())
            ImageDisplay->setObjectName(QStringLiteral("ImageDisplay"));
        ImageDisplay->resize(400, 301);
        hilabel = new QLabel(ImageDisplay);
        hilabel->setObjectName(QStringLiteral("hilabel"));
        hilabel->setGeometry(QRect(160, 130, 47, 13));

        retranslateUi(ImageDisplay);

        QMetaObject::connectSlotsByName(ImageDisplay);
    } // setupUi

    void retranslateUi(QWidget *ImageDisplay)
    {
        ImageDisplay->setWindowTitle(QApplication::translate("ImageDisplay", "ImageDisplay", nullptr));
        hilabel->setText(QApplication::translate("ImageDisplay", "Hi", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageDisplay: public Ui_ImageDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEDISPLAY_H
