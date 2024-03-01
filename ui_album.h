/********************************************************************************
** Form generated from reading UI file 'album.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALBUM_H
#define UI_ALBUM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Album
{
public:
    QPushButton *previous;
    QPushButton *next;
    QPushButton *back;
    QPushButton *delete_2;
    QLabel *label;

    void setupUi(QWidget *Album)
    {
        if (Album->objectName().isEmpty())
            Album->setObjectName(QString::fromUtf8("Album"));
        Album->resize(800, 480);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Album->sizePolicy().hasHeightForWidth());
        Album->setSizePolicy(sizePolicy);
        previous = new QPushButton(Album);
        previous->setObjectName(QString::fromUtf8("previous"));
        previous->setGeometry(QRect(10, 200, 70, 81));
        sizePolicy.setHeightForWidth(previous->sizePolicy().hasHeightForWidth());
        previous->setSizePolicy(sizePolicy);
        previous->setLayoutDirection(Qt::LeftToRight);
        previous->setIconSize(QSize(40, 40));
        next = new QPushButton(Album);
        next->setObjectName(QString::fromUtf8("next"));
        next->setGeometry(QRect(720, 200, 70, 81));
        sizePolicy.setHeightForWidth(next->sizePolicy().hasHeightForWidth());
        next->setSizePolicy(sizePolicy);
        next->setIconSize(QSize(40, 40));
        back = new QPushButton(Album);
        back->setObjectName(QString::fromUtf8("back"));
        back->setGeometry(QRect(10, 10, 70, 70));
        sizePolicy.setHeightForWidth(back->sizePolicy().hasHeightForWidth());
        back->setSizePolicy(sizePolicy);
        delete_2 = new QPushButton(Album);
        delete_2->setObjectName(QString::fromUtf8("delete_2"));
        delete_2->setGeometry(QRect(10, 400, 70, 70));
        sizePolicy.setHeightForWidth(delete_2->sizePolicy().hasHeightForWidth());
        delete_2->setSizePolicy(sizePolicy);
        label = new QLabel(Album);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 0, 600, 480));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        retranslateUi(Album);

        QMetaObject::connectSlotsByName(Album);
    } // setupUi

    void retranslateUi(QWidget *Album)
    {
        Album->setWindowTitle(QApplication::translate("Album", "Form", nullptr));
        previous->setText(QApplication::translate("Album", "<", nullptr));
        next->setText(QApplication::translate("Album", ">", nullptr));
        back->setText(QApplication::translate("Album", "\350\277\224\345\233\236", nullptr));
        delete_2->setText(QApplication::translate("Album", "\345\210\240\351\231\244", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Album: public Ui_Album {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALBUM_H
