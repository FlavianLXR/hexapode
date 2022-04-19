/********************************************************************************
** Form generated from reading UI file 'udp.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UDP_H
#define UI_UDP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UDP
{
public:
    QGridLayout *gridLayout;
    QFrame *line;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_11;
    QLabel *statconnection;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLabel *LL1;
    QLabel *LL2;
    QLabel *LL3;
    QLabel *LL4;
    QLabel *LL5;
    QLabel *LL6;
    QSpacerItem *verticalSpacer;
    QFrame *line_4;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labroll;
    QLabel *labpitch;
    QLabel *labheading;
    QFrame *line_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *UDP)
    {
        if (UDP->objectName().isEmpty())
            UDP->setObjectName(QString::fromUtf8("UDP"));
        UDP->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UDP->sizePolicy().hasHeightForWidth());
        UDP->setSizePolicy(sizePolicy);
        UDP->setMinimumSize(QSize(0, 0));
        UDP->setContextMenuPolicy(Qt::PreventContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/hexa.png"), QSize(), QIcon::Normal, QIcon::Off);
        UDP->setWindowIcon(icon);
        gridLayout = new QGridLayout(UDP);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        line = new QFrame(UDP);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 9, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_11 = new QLabel(UDP);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_11);

        statconnection = new QLabel(UDP);
        statconnection->setObjectName(QString::fromUtf8("statconnection"));
        statconnection->setStyleSheet(QString::fromUtf8(""));
        statconnection->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(statconnection);


        horizontalLayout_7->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(UDP);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_3);

        label_6 = new QLabel(UDP);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_6);

        label_5 = new QLabel(UDP);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_5);

        label_4 = new QLabel(UDP);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_4);

        label_2 = new QLabel(UDP);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_2);

        label = new QLabel(UDP);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        LL1 = new QLabel(UDP);
        LL1->setObjectName(QString::fromUtf8("LL1"));
        LL1->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL1);

        LL2 = new QLabel(UDP);
        LL2->setObjectName(QString::fromUtf8("LL2"));
        LL2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL2);

        LL3 = new QLabel(UDP);
        LL3->setObjectName(QString::fromUtf8("LL3"));
        LL3->setScaledContents(true);
        LL3->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL3);

        LL4 = new QLabel(UDP);
        LL4->setObjectName(QString::fromUtf8("LL4"));
        LL4->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL4);

        LL5 = new QLabel(UDP);
        LL5->setObjectName(QString::fromUtf8("LL5"));
        LL5->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL5);

        LL6 = new QLabel(UDP);
        LL6->setObjectName(QString::fromUtf8("LL6"));
        LL6->setLayoutDirection(Qt::LeftToRight);
        LL6->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(LL6);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 7, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

        line_4 = new QFrame(UDP);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 10, 0, 1, 1);

        line_3 = new QFrame(UDP);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 3, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton = new QPushButton(UDP);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_4->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_4, 11, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_8 = new QLabel(UDP);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_8);

        label_9 = new QLabel(UDP);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_9);

        label_7 = new QLabel(UDP);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_7);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labroll = new QLabel(UDP);
        labroll->setObjectName(QString::fromUtf8("labroll"));
        labroll->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(labroll);

        labpitch = new QLabel(UDP);
        labpitch->setObjectName(QString::fromUtf8("labpitch"));
        labpitch->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(labpitch);

        labheading = new QLabel(UDP);
        labheading->setObjectName(QString::fromUtf8("labheading"));
        labheading->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(labheading);


        verticalLayout_2->addLayout(horizontalLayout_6);


        gridLayout->addLayout(verticalLayout_2, 4, 0, 1, 1);

        line_2 = new QFrame(UDP);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 8, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 6, 0, 1, 1);


        retranslateUi(UDP);
        QObject::connect(pushButton, &QPushButton::pressed, UDP, qOverload<>(&QWidget::close));

        QMetaObject::connectSlotsByName(UDP);
    } // setupUi

    void retranslateUi(QWidget *UDP)
    {
        UDP->setWindowTitle(QCoreApplication::translate("UDP", "FlightSimMotionControl", nullptr));
        label_11->setText(QCoreApplication::translate("UDP", "\303\251tat de connection", nullptr));
        statconnection->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("UDP", "verin n\302\2601", nullptr));
        label_6->setText(QCoreApplication::translate("UDP", "verin n\302\2602", nullptr));
        label_5->setText(QCoreApplication::translate("UDP", "verin n\302\2603", nullptr));
        label_4->setText(QCoreApplication::translate("UDP", "verin n\302\2604", nullptr));
        label_2->setText(QCoreApplication::translate("UDP", "verin n\302\2605", nullptr));
        label->setText(QCoreApplication::translate("UDP", "verin n\302\2606", nullptr));
        LL1->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        LL2->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        LL3->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        LL4->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        LL5->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        LL6->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("UDP", "quitter", nullptr));
        label_8->setText(QCoreApplication::translate("UDP", "roulis / Rx", nullptr));
        label_9->setText(QCoreApplication::translate("UDP", "tangage / Ry", nullptr));
        label_7->setText(QCoreApplication::translate("UDP", "lacet / Rz", nullptr));
        labroll->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        labpitch->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
        labheading->setText(QCoreApplication::translate("UDP", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UDP: public Ui_UDP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UDP_H
