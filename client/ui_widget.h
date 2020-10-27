/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textMsg;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textInput;
    QVBoxLayout *verticalLayout;
    QPushButton *image;
    QPushButton *textSend;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(424, 293);
        textMsg = new QTextEdit(Widget);
        textMsg->setObjectName(QString::fromUtf8("textMsg"));
        textMsg->setGeometry(QRect(6, 6, 412, 181));
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(6, 196, 412, 93));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        textInput = new QTextEdit(layoutWidget);
        textInput->setObjectName(QString::fromUtf8("textInput"));

        horizontalLayout->addWidget(textInput);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        image = new QPushButton(layoutWidget);
        image->setObjectName(QString::fromUtf8("image"));

        verticalLayout->addWidget(image);

        textSend = new QPushButton(layoutWidget);
        textSend->setObjectName(QString::fromUtf8("textSend"));

        verticalLayout->addWidget(textSend);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "client", nullptr));
        image->setText(QCoreApplication::translate("Widget", "\345\233\276\347\211\207", nullptr));
        textSend->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
