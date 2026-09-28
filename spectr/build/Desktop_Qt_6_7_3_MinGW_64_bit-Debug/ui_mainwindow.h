/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPushButton *btnDisconnect;
    QPushButton *btnUpdateDark;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnConnect;
    QComboBox *cbPorts;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *tablePhotometric;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_2;
    QLabel *label;
    QDoubleSpinBox *spinWavelength;
    QPushButton *btnSetZero;
    QPushButton *btnMeasure;
    QPushButton *btnClearPhoto;
    QPushButton *btnExportPhoto;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QPushButton *btnScanSample;
    QDoubleSpinBox *spinEndWl;
    QLabel *label_6;
    QPushButton *btnExport;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_2;
    QWidget *widgetGraph;
    QDoubleSpinBox *spinStepMeasure;
    QLabel *label_3;
    QDoubleSpinBox *spinStepZero;
    QTableWidget *tableSpectral;
    QPushButton *btnScanBase;
    QSpacerItem *verticalSpacer_4;
    QDoubleSpinBox *spinStartWl;
    QLabel *label_4;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1314, 720);
        QFont font;
        font.setPointSize(11);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        btnDisconnect = new QPushButton(centralwidget);
        btnDisconnect->setObjectName("btnDisconnect");
        btnDisconnect->setEnabled(false);

        gridLayout->addWidget(btnDisconnect, 0, 2, 1, 1);

        btnUpdateDark = new QPushButton(centralwidget);
        btnUpdateDark->setObjectName("btnUpdateDark");

        gridLayout->addWidget(btnUpdateDark, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName("btnConnect");

        horizontalLayout->addWidget(btnConnect);

        cbPorts = new QComboBox(centralwidget);
        cbPorts->setObjectName("cbPorts");
        cbPorts->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));

        horizontalLayout->addWidget(cbPorts);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        tab->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        tablePhotometric = new QTableWidget(tab);
        tablePhotometric->setObjectName("tablePhotometric");
        tablePhotometric->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"            background-color: white;\n"
"            color: black; \n"
"            alternate-background-color: #f9fbfe; \n"
"            gridline-color: #e0e0e0;\n"
"            selection-background-color: #b2dfdb; \n"
"            selection-color: black;\n"
"            border: 1px solid #cfd8dc;\n"
"            border-radius: 4px;\n"
"        }"));

        horizontalLayout_2->addWidget(tablePhotometric);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 0, 0, 1, 1);

        label = new QLabel(tab);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        spinWavelength = new QDoubleSpinBox(tab);
        spinWavelength->setObjectName("spinWavelength");
        spinWavelength->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));
        spinWavelength->setMinimum(190.000000000000000);
        spinWavelength->setMaximum(900.000000000000000);
        spinWavelength->setValue(500.000000000000000);

        gridLayout_2->addWidget(spinWavelength, 2, 0, 1, 1);

        btnSetZero = new QPushButton(tab);
        btnSetZero->setObjectName("btnSetZero");
        btnSetZero->setFont(font);

        gridLayout_2->addWidget(btnSetZero, 3, 0, 1, 1);

        btnMeasure = new QPushButton(tab);
        btnMeasure->setObjectName("btnMeasure");
        btnMeasure->setFont(font);

        gridLayout_2->addWidget(btnMeasure, 4, 0, 1, 1);

        btnClearPhoto = new QPushButton(tab);
        btnClearPhoto->setObjectName("btnClearPhoto");
        QFont font1;
        font1.setPointSize(9);
        btnClearPhoto->setFont(font1);

        gridLayout_2->addWidget(btnClearPhoto, 5, 0, 1, 1);

        btnExportPhoto = new QPushButton(tab);
        btnExportPhoto->setObjectName("btnExportPhoto");

        gridLayout_2->addWidget(btnExportPhoto, 6, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 7, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName("gridLayout_3");
        label_5 = new QLabel(tab_2);
        label_5->setObjectName("label_5");
        label_5->setFont(font);

        gridLayout_3->addWidget(label_5, 4, 1, 1, 2);

        btnScanSample = new QPushButton(tab_2);
        btnScanSample->setObjectName("btnScanSample");
        btnScanSample->setFont(font1);

        gridLayout_3->addWidget(btnScanSample, 7, 1, 2, 3);

        spinEndWl = new QDoubleSpinBox(tab_2);
        spinEndWl->setObjectName("spinEndWl");
        spinEndWl->setMinimumSize(QSize(100, 30));
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(false);
        spinEndWl->setFont(font2);
        spinEndWl->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));
        spinEndWl->setMinimum(190.000000000000000);
        spinEndWl->setMaximum(900.000000000000000);

        gridLayout_3->addWidget(spinEndWl, 3, 3, 1, 1);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName("label_6");

        gridLayout_3->addWidget(label_6, 5, 1, 1, 2);

        btnExport = new QPushButton(tab_2);
        btnExport->setObjectName("btnExport");

        gridLayout_3->addWidget(btnExport, 9, 1, 1, 3);

        verticalSpacer_3 = new QSpacerItem(20, 130, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 10, 2, 1, 1);

        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 1, 1, 1, 3);

        widgetGraph = new QWidget(tab_2);
        widgetGraph->setObjectName("widgetGraph");
        sizePolicy.setHeightForWidth(widgetGraph->sizePolicy().hasHeightForWidth());
        widgetGraph->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(widgetGraph, 0, 0, 8, 1);

        spinStepMeasure = new QDoubleSpinBox(tab_2);
        spinStepMeasure->setObjectName("spinStepMeasure");
        spinStepMeasure->setMinimumSize(QSize(100, 30));
        spinStepMeasure->setFont(font);
        spinStepMeasure->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));
        spinStepMeasure->setValue(1.000000000000000);

        gridLayout_3->addWidget(spinStepMeasure, 5, 3, 1, 1);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName("label_3");
        label_3->setFont(font);

        gridLayout_3->addWidget(label_3, 3, 2, 1, 1);

        spinStepZero = new QDoubleSpinBox(tab_2);
        spinStepZero->setObjectName("spinStepZero");
        spinStepZero->setMinimumSize(QSize(100, 30));
        spinStepZero->setFont(font);
        spinStepZero->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));
        spinStepZero->setMaximum(100.000000000000000);
        spinStepZero->setValue(1.000000000000000);

        gridLayout_3->addWidget(spinStepZero, 4, 3, 1, 1);

        tableSpectral = new QTableWidget(tab_2);
        tableSpectral->setObjectName("tableSpectral");
        tableSpectral->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"            background-color: white;\n"
"            color: black; /* <--- \320\224\320\276\320\261\320\260\320\262\321\214 \321\215\321\202\321\203 \321\201\321\202\321\200\320\276\320\272\321\203, \321\207\321\202\320\276\320\261\321\213 \321\202\320\265\320\272\321\201\321\202 \320\262\321\201\320\265\320\263\320\264\320\260 \320\261\321\213\320\273 \321\207\320\265\321\200\320\275\321\213\320\274 */\n"
"            alternate-background-color: #f9fbfe; \n"
"            gridline-color: #e0e0e0;\n"
"            selection-background-color: #b2dfdb; \n"
"            selection-color: black;\n"
"            border: 1px solid #cfd8dc;\n"
"            border-radius: 4px;\n"
"        }"));

        gridLayout_3->addWidget(tableSpectral, 8, 0, 3, 1);

        btnScanBase = new QPushButton(tab_2);
        btnScanBase->setObjectName("btnScanBase");
        btnScanBase->setFont(font1);

        gridLayout_3->addWidget(btnScanBase, 6, 1, 1, 3);

        verticalSpacer_4 = new QSpacerItem(20, 130, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 0, 2, 1, 1);

        spinStartWl = new QDoubleSpinBox(tab_2);
        spinStartWl->setObjectName("spinStartWl");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spinStartWl->sizePolicy().hasHeightForWidth());
        spinStartWl->setSizePolicy(sizePolicy1);
        spinStartWl->setMinimumSize(QSize(100, 30));
        spinStartWl->setFont(font);
        spinStartWl->setStyleSheet(QString::fromUtf8("QDoubleSpinBox, QSpinBox, QComboBox, QLineEdit {\n"
"	background-color: white;\n"
"	color: black;\n"
"}"));
        spinStartWl->setMinimum(190.000000000000000);
        spinStartWl->setMaximum(900.000000000000000);

        gridLayout_3->addWidget(spinStartWl, 2, 3, 1, 1);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");
        label_4->setFont(font);

        gridLayout_3->addWidget(label_4, 2, 2, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 3);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnDisconnect->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214", nullptr));
        btnUpdateDark->setText(QCoreApplication::translate("MainWindow", "\320\242\320\265\320\274\320\275\320\276\320\262\320\276\320\271 \321\202\320\276\320\272", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\224\320\273\320\270\320\275\320\260 \320\262\320\276\320\273\320\275\321\213", nullptr));
        btnSetZero->setText(QCoreApplication::translate("MainWindow", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 0", nullptr));
        btnMeasure->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\270\320\265", nullptr));
        btnClearPhoto->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \321\202\320\260\320\261\320\273\320\270\321\206\321\203", nullptr));
        btnExportPhoto->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\262 CSV", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\320\244\320\276\321\202\320\276\320\274\320\265\321\202\321\200\320\270\321\207\320\265\321\201\320\272\320\270\320\271", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\250\320\260\320\263 \320\267\320\260\320\275\321\203\320\273\320\265\320\275\320\270\321\217", nullptr));
        btnScanSample->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\321\200\320\270\321\202\321\214 \321\201\320\277\320\265\320\272\321\202\321\200 \320\276\320\261\321\200\320\260\320\267\321\206\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\250\320\260\320\263 \320\270\320\267\320\274\320\265\321\200\320\265\320\275\320\270\321\217", nullptr));
        btnExport->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262 CSV", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\241\320\277\320\265\320\272\321\202\321\200 \320\264\320\273\320\270\320\275 \320\262\320\276\320\273\320\275", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\264\320\276", nullptr));
        btnScanBase->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\321\200\320\270\321\202\321\214 \320\261\320\260\320\267\320\276\320\262\321\203\321\216 \320\273\320\270\320\275\320\270\321\216", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\276\321\202", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\320\241\320\277\320\265\320\272\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
