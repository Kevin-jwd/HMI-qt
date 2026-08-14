/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.19
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *layRoot;
    QFrame *statusStrip;
    QHBoxLayout *layStatusStrip;
    QLabel *lblMode;
    QFrame *lineStatus;
    QLabel *lblAuth;
    QSpacerItem *spStatusStrip;
    QLabel *lblStripTemp;
    QHBoxLayout *layBody;
    QListWidget *navList;
    QStackedWidget *stackMain;
    QWidget *pageDashboard;
    QVBoxLayout *layDashboard;
    QGroupBox *grpCabin;
    QVBoxLayout *layCabin;
    QLabel *lblCabinTemp;
    QLabel *lblHumidity;
    QGroupBox *grpSpeed;
    QVBoxLayout *laySpeed;
    QLabel *lblSpeed;
    QProgressBar *pbSpeed;
    QGroupBox *grpUltrasonic;
    QGridLayout *layUltrasonic;
    QLabel *lblUltraLeftName;
    QProgressBar *pbUltraLeft;
    QLabel *lblUltraLeft;
    QLabel *lblUltraFrontName;
    QProgressBar *pbUltraFront;
    QLabel *lblUltraFront;
    QLabel *lblUltraRightName;
    QProgressBar *pbUltraRight;
    QLabel *lblUltraRight;
    QSpacerItem *spDashboard;
    QWidget *pageDriver;
    QVBoxLayout *layPageDriver;
    QGroupBox *grpCamInternal;
    QVBoxLayout *layCamInternal;
    QLabel *lblCamInternal;
    QLabel *lblRecogResult;
    QGroupBox *grpDriverManage;
    QVBoxLayout *layDriverManage;
    QPushButton *btnRecogToggle;
    QHBoxLayout *layDriverInput;
    QLineEdit *editDriverName;
    QPushButton *btnRegisterFace;
    QPushButton *btnDriverList;
    QSpacerItem *spDriver;
    QWidget *pageCamRear;
    QVBoxLayout *layPageCamRear;
    QGroupBox *grpCamRear;
    QVBoxLayout *layCamRear;
    QLabel *lblCamRear;
    QHBoxLayout *layCamRearBtns;
    QLabel *lblRearWarning;
    QSpacerItem *spCamRear;
    QPushButton *btnCamRearToggle;
    QWidget *pageControl;
    QVBoxLayout *layPageControl;
    QHBoxLayout *layControlTop;
    QPushButton *btnModeToggle;
    QPushButton *btnHazard;
    QGroupBox *grpDirection;
    QGridLayout *layDirection;
    QPushButton *btnUp;
    QPushButton *btnLeft;
    QPushButton *btnStop;
    QPushButton *btnRight;
    QPushButton *btnDown;
    QSpacerItem *spControl;
    QWidget *pageEnv;
    QVBoxLayout *layPageEnv;
    QGroupBox *grpFan;
    QHBoxLayout *layFan;
    QPushButton *btnFanOn;
    QPushButton *btnFanOff;
    QPushButton *btnFanAuto;
    QGroupBox *grpThreshold;
    QHBoxLayout *layThreshold;
    QDoubleSpinBox *spinThreshold;
    QPushButton *btnApplyThreshold;
    QSpacerItem *spThreshold;
    QSpacerItem *spEnv;
    QStatusBar *statusbar;
    QButtonGroup *groupFan;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 640);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layRoot = new QVBoxLayout(centralwidget);
        layRoot->setObjectName(QString::fromUtf8("layRoot"));
        statusStrip = new QFrame(centralwidget);
        statusStrip->setObjectName(QString::fromUtf8("statusStrip"));
        statusStrip->setFrameShape(QFrame::Shape::StyledPanel);
        layStatusStrip = new QHBoxLayout(statusStrip);
        layStatusStrip->setObjectName(QString::fromUtf8("layStatusStrip"));
        lblMode = new QLabel(statusStrip);
        lblMode->setObjectName(QString::fromUtf8("lblMode"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        lblMode->setFont(font);

        layStatusStrip->addWidget(lblMode);

        lineStatus = new QFrame(statusStrip);
        lineStatus->setObjectName(QString::fromUtf8("lineStatus"));
        lineStatus->setFrameShape(QFrame::VLine);
        lineStatus->setFrameShadow(QFrame::Sunken);

        layStatusStrip->addWidget(lineStatus);

        lblAuth = new QLabel(statusStrip);
        lblAuth->setObjectName(QString::fromUtf8("lblAuth"));
        lblAuth->setFont(font);

        layStatusStrip->addWidget(lblAuth);

        spStatusStrip = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layStatusStrip->addItem(spStatusStrip);

        lblStripTemp = new QLabel(statusStrip);
        lblStripTemp->setObjectName(QString::fromUtf8("lblStripTemp"));

        layStatusStrip->addWidget(lblStripTemp);


        layRoot->addWidget(statusStrip);

        layBody = new QHBoxLayout();
        layBody->setObjectName(QString::fromUtf8("layBody"));
        navList = new QListWidget(centralwidget);
        new QListWidgetItem(navList);
        new QListWidgetItem(navList);
        new QListWidgetItem(navList);
        new QListWidgetItem(navList);
        new QListWidgetItem(navList);
        new QListWidgetItem(navList);
        navList->setObjectName(QString::fromUtf8("navList"));
        navList->setMaximumSize(QSize(170, 16777215));
        QFont font1;
        font1.setPointSize(11);
        navList->setFont(font1);

        layBody->addWidget(navList);

        stackMain = new QStackedWidget(centralwidget);
        stackMain->setObjectName(QString::fromUtf8("stackMain"));
        pageDashboard = new QWidget();
        pageDashboard->setObjectName(QString::fromUtf8("pageDashboard"));
        layDashboard = new QVBoxLayout(pageDashboard);
        layDashboard->setObjectName(QString::fromUtf8("layDashboard"));
        grpCabin = new QGroupBox(pageDashboard);
        grpCabin->setObjectName(QString::fromUtf8("grpCabin"));
        layCabin = new QVBoxLayout(grpCabin);
        layCabin->setObjectName(QString::fromUtf8("layCabin"));
        lblCabinTemp = new QLabel(grpCabin);
        lblCabinTemp->setObjectName(QString::fromUtf8("lblCabinTemp"));
        QFont font2;
        font2.setPointSize(40);
        font2.setBold(true);
        lblCabinTemp->setFont(font2);
        lblCabinTemp->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layCabin->addWidget(lblCabinTemp);

        lblHumidity = new QLabel(grpCabin);
        lblHumidity->setObjectName(QString::fromUtf8("lblHumidity"));
        lblHumidity->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layCabin->addWidget(lblHumidity);


        layDashboard->addWidget(grpCabin);

        grpSpeed = new QGroupBox(pageDashboard);
        grpSpeed->setObjectName(QString::fromUtf8("grpSpeed"));
        laySpeed = new QVBoxLayout(grpSpeed);
        laySpeed->setObjectName(QString::fromUtf8("laySpeed"));
        lblSpeed = new QLabel(grpSpeed);
        lblSpeed->setObjectName(QString::fromUtf8("lblSpeed"));
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(true);
        lblSpeed->setFont(font3);
        lblSpeed->setAlignment(Qt::AlignmentFlag::AlignCenter);

        laySpeed->addWidget(lblSpeed);

        pbSpeed = new QProgressBar(grpSpeed);
        pbSpeed->setObjectName(QString::fromUtf8("pbSpeed"));
        pbSpeed->setMaximum(100);
        pbSpeed->setValue(0);
        pbSpeed->setTextVisible(false);

        laySpeed->addWidget(pbSpeed);


        layDashboard->addWidget(grpSpeed);

        grpUltrasonic = new QGroupBox(pageDashboard);
        grpUltrasonic->setObjectName(QString::fromUtf8("grpUltrasonic"));
        layUltrasonic = new QGridLayout(grpUltrasonic);
        layUltrasonic->setObjectName(QString::fromUtf8("layUltrasonic"));
        lblUltraLeftName = new QLabel(grpUltrasonic);
        lblUltraLeftName->setObjectName(QString::fromUtf8("lblUltraLeftName"));

        layUltrasonic->addWidget(lblUltraLeftName, 0, 0, 1, 1);

        pbUltraLeft = new QProgressBar(grpUltrasonic);
        pbUltraLeft->setObjectName(QString::fromUtf8("pbUltraLeft"));
        pbUltraLeft->setMaximum(200);
        pbUltraLeft->setValue(0);
        pbUltraLeft->setTextVisible(false);

        layUltrasonic->addWidget(pbUltraLeft, 0, 1, 1, 1);

        lblUltraLeft = new QLabel(grpUltrasonic);
        lblUltraLeft->setObjectName(QString::fromUtf8("lblUltraLeft"));

        layUltrasonic->addWidget(lblUltraLeft, 0, 2, 1, 1);

        lblUltraFrontName = new QLabel(grpUltrasonic);
        lblUltraFrontName->setObjectName(QString::fromUtf8("lblUltraFrontName"));

        layUltrasonic->addWidget(lblUltraFrontName, 1, 0, 1, 1);

        pbUltraFront = new QProgressBar(grpUltrasonic);
        pbUltraFront->setObjectName(QString::fromUtf8("pbUltraFront"));
        pbUltraFront->setMaximum(200);
        pbUltraFront->setValue(0);
        pbUltraFront->setTextVisible(false);

        layUltrasonic->addWidget(pbUltraFront, 1, 1, 1, 1);

        lblUltraFront = new QLabel(grpUltrasonic);
        lblUltraFront->setObjectName(QString::fromUtf8("lblUltraFront"));

        layUltrasonic->addWidget(lblUltraFront, 1, 2, 1, 1);

        lblUltraRightName = new QLabel(grpUltrasonic);
        lblUltraRightName->setObjectName(QString::fromUtf8("lblUltraRightName"));

        layUltrasonic->addWidget(lblUltraRightName, 2, 0, 1, 1);

        pbUltraRight = new QProgressBar(grpUltrasonic);
        pbUltraRight->setObjectName(QString::fromUtf8("pbUltraRight"));
        pbUltraRight->setMaximum(200);
        pbUltraRight->setValue(0);
        pbUltraRight->setTextVisible(false);

        layUltrasonic->addWidget(pbUltraRight, 2, 1, 1, 1);

        lblUltraRight = new QLabel(grpUltrasonic);
        lblUltraRight->setObjectName(QString::fromUtf8("lblUltraRight"));

        layUltrasonic->addWidget(lblUltraRight, 2, 2, 1, 1);


        layDashboard->addWidget(grpUltrasonic);

        spDashboard = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        layDashboard->addItem(spDashboard);

        stackMain->addWidget(pageDashboard);
        pageDriver = new QWidget();
        pageDriver->setObjectName(QString::fromUtf8("pageDriver"));
        layPageDriver = new QVBoxLayout(pageDriver);
        layPageDriver->setObjectName(QString::fromUtf8("layPageDriver"));
        grpCamInternal = new QGroupBox(pageDriver);
        grpCamInternal->setObjectName(QString::fromUtf8("grpCamInternal"));
        layCamInternal = new QVBoxLayout(grpCamInternal);
        layCamInternal->setObjectName(QString::fromUtf8("layCamInternal"));
        lblCamInternal = new QLabel(grpCamInternal);
        lblCamInternal->setObjectName(QString::fromUtf8("lblCamInternal"));
        lblCamInternal->setMinimumSize(QSize(320, 240));
        lblCamInternal->setFrameShape(QFrame::Shape::Box);
        lblCamInternal->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layCamInternal->addWidget(lblCamInternal);

        lblRecogResult = new QLabel(grpCamInternal);
        lblRecogResult->setObjectName(QString::fromUtf8("lblRecogResult"));
        lblRecogResult->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layCamInternal->addWidget(lblRecogResult);


        layPageDriver->addWidget(grpCamInternal);

        grpDriverManage = new QGroupBox(pageDriver);
        grpDriverManage->setObjectName(QString::fromUtf8("grpDriverManage"));
        layDriverManage = new QVBoxLayout(grpDriverManage);
        layDriverManage->setObjectName(QString::fromUtf8("layDriverManage"));
        btnRecogToggle = new QPushButton(grpDriverManage);
        btnRecogToggle->setObjectName(QString::fromUtf8("btnRecogToggle"));
        btnRecogToggle->setMinimumSize(QSize(0, 40));
        btnRecogToggle->setCheckable(true);

        layDriverManage->addWidget(btnRecogToggle);

        layDriverInput = new QHBoxLayout();
        layDriverInput->setObjectName(QString::fromUtf8("layDriverInput"));
        editDriverName = new QLineEdit(grpDriverManage);
        editDriverName->setObjectName(QString::fromUtf8("editDriverName"));

        layDriverInput->addWidget(editDriverName);

        btnRegisterFace = new QPushButton(grpDriverManage);
        btnRegisterFace->setObjectName(QString::fromUtf8("btnRegisterFace"));

        layDriverInput->addWidget(btnRegisterFace);

        btnDriverList = new QPushButton(grpDriverManage);
        btnDriverList->setObjectName(QString::fromUtf8("btnDriverList"));

        layDriverInput->addWidget(btnDriverList);


        layDriverManage->addLayout(layDriverInput);


        layPageDriver->addWidget(grpDriverManage);

        spDriver = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        layPageDriver->addItem(spDriver);

        stackMain->addWidget(pageDriver);
        pageCamRear = new QWidget();
        pageCamRear->setObjectName(QString::fromUtf8("pageCamRear"));
        layPageCamRear = new QVBoxLayout(pageCamRear);
        layPageCamRear->setObjectName(QString::fromUtf8("layPageCamRear"));
        grpCamRear = new QGroupBox(pageCamRear);
        grpCamRear->setObjectName(QString::fromUtf8("grpCamRear"));
        layCamRear = new QVBoxLayout(grpCamRear);
        layCamRear->setObjectName(QString::fromUtf8("layCamRear"));
        lblCamRear = new QLabel(grpCamRear);
        lblCamRear->setObjectName(QString::fromUtf8("lblCamRear"));
        lblCamRear->setMinimumSize(QSize(320, 240));
        lblCamRear->setFrameShape(QFrame::Shape::Box);
        lblCamRear->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layCamRear->addWidget(lblCamRear);

        layCamRearBtns = new QHBoxLayout();
        layCamRearBtns->setObjectName(QString::fromUtf8("layCamRearBtns"));
        lblRearWarning = new QLabel(grpCamRear);
        lblRearWarning->setObjectName(QString::fromUtf8("lblRearWarning"));

        layCamRearBtns->addWidget(lblRearWarning);

        spCamRear = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layCamRearBtns->addItem(spCamRear);

        btnCamRearToggle = new QPushButton(grpCamRear);
        btnCamRearToggle->setObjectName(QString::fromUtf8("btnCamRearToggle"));
        btnCamRearToggle->setCheckable(true);

        layCamRearBtns->addWidget(btnCamRearToggle);


        layCamRear->addLayout(layCamRearBtns);


        layPageCamRear->addWidget(grpCamRear);

        stackMain->addWidget(pageCamRear);
        pageControl = new QWidget();
        pageControl->setObjectName(QString::fromUtf8("pageControl"));
        layPageControl = new QVBoxLayout(pageControl);
        layPageControl->setObjectName(QString::fromUtf8("layPageControl"));
        layControlTop = new QHBoxLayout();
        layControlTop->setObjectName(QString::fromUtf8("layControlTop"));
        btnModeToggle = new QPushButton(pageControl);
        btnModeToggle->setObjectName(QString::fromUtf8("btnModeToggle"));
        btnModeToggle->setMinimumSize(QSize(0, 48));
        btnModeToggle->setCheckable(true);

        layControlTop->addWidget(btnModeToggle);

        btnHazard = new QPushButton(pageControl);
        btnHazard->setObjectName(QString::fromUtf8("btnHazard"));
        btnHazard->setMinimumSize(QSize(0, 48));
        btnHazard->setCheckable(true);

        layControlTop->addWidget(btnHazard);


        layPageControl->addLayout(layControlTop);

        grpDirection = new QGroupBox(pageControl);
        grpDirection->setObjectName(QString::fromUtf8("grpDirection"));
        layDirection = new QGridLayout(grpDirection);
        layDirection->setObjectName(QString::fromUtf8("layDirection"));
        btnUp = new QPushButton(grpDirection);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        btnUp->setMinimumSize(QSize(80, 60));

        layDirection->addWidget(btnUp, 0, 1, 1, 1);

        btnLeft = new QPushButton(grpDirection);
        btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
        btnLeft->setMinimumSize(QSize(80, 60));

        layDirection->addWidget(btnLeft, 1, 0, 1, 1);

        btnStop = new QPushButton(grpDirection);
        btnStop->setObjectName(QString::fromUtf8("btnStop"));
        btnStop->setMinimumSize(QSize(80, 60));

        layDirection->addWidget(btnStop, 1, 1, 1, 1);

        btnRight = new QPushButton(grpDirection);
        btnRight->setObjectName(QString::fromUtf8("btnRight"));
        btnRight->setMinimumSize(QSize(80, 60));

        layDirection->addWidget(btnRight, 1, 2, 1, 1);

        btnDown = new QPushButton(grpDirection);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        btnDown->setMinimumSize(QSize(80, 60));

        layDirection->addWidget(btnDown, 2, 1, 1, 1);


        layPageControl->addWidget(grpDirection);

        spControl = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        layPageControl->addItem(spControl);

        stackMain->addWidget(pageControl);
        pageEnv = new QWidget();
        pageEnv->setObjectName(QString::fromUtf8("pageEnv"));
        layPageEnv = new QVBoxLayout(pageEnv);
        layPageEnv->setObjectName(QString::fromUtf8("layPageEnv"));
        grpFan = new QGroupBox(pageEnv);
        grpFan->setObjectName(QString::fromUtf8("grpFan"));
        layFan = new QHBoxLayout(grpFan);
        layFan->setObjectName(QString::fromUtf8("layFan"));
        btnFanOn = new QPushButton(grpFan);
        groupFan = new QButtonGroup(MainWindow);
        groupFan->setObjectName(QString::fromUtf8("groupFan"));
        groupFan->addButton(btnFanOn);
        btnFanOn->setObjectName(QString::fromUtf8("btnFanOn"));
        btnFanOn->setMinimumSize(QSize(0, 48));
        btnFanOn->setCheckable(true);

        layFan->addWidget(btnFanOn);

        btnFanOff = new QPushButton(grpFan);
        groupFan->addButton(btnFanOff);
        btnFanOff->setObjectName(QString::fromUtf8("btnFanOff"));
        btnFanOff->setMinimumSize(QSize(0, 48));
        btnFanOff->setCheckable(true);
        btnFanOff->setChecked(true);

        layFan->addWidget(btnFanOff);

        btnFanAuto = new QPushButton(grpFan);
        groupFan->addButton(btnFanAuto);
        btnFanAuto->setObjectName(QString::fromUtf8("btnFanAuto"));
        btnFanAuto->setMinimumSize(QSize(0, 48));
        btnFanAuto->setCheckable(true);

        layFan->addWidget(btnFanAuto);


        layPageEnv->addWidget(grpFan);

        grpThreshold = new QGroupBox(pageEnv);
        grpThreshold->setObjectName(QString::fromUtf8("grpThreshold"));
        layThreshold = new QHBoxLayout(grpThreshold);
        layThreshold->setObjectName(QString::fromUtf8("layThreshold"));
        spinThreshold = new QDoubleSpinBox(grpThreshold);
        spinThreshold->setObjectName(QString::fromUtf8("spinThreshold"));
        spinThreshold->setMinimumSize(QSize(110, 40));
        spinThreshold->setDecimals(1);
        spinThreshold->setMaximum(50.000000000000000);
        spinThreshold->setSingleStep(0.500000000000000);
        spinThreshold->setValue(27.000000000000000);

        layThreshold->addWidget(spinThreshold);

        btnApplyThreshold = new QPushButton(grpThreshold);
        btnApplyThreshold->setObjectName(QString::fromUtf8("btnApplyThreshold"));

        layThreshold->addWidget(btnApplyThreshold);

        spThreshold = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layThreshold->addItem(spThreshold);


        layPageEnv->addWidget(grpThreshold);

        spEnv = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        layPageEnv->addItem(spEnv);

        stackMain->addWidget(pageEnv);

        layBody->addWidget(stackMain);


        layRoot->addLayout(layBody);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(navList, SIGNAL(currentRowChanged(int)), stackMain, SLOT(setCurrentIndex(int)));

        navList->setCurrentRow(-1);
        stackMain->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Vehicle HMI", nullptr));
        lblMode->setText(QCoreApplication::translate("MainWindow", "\353\252\250\353\223\234: \354\210\230\353\217\231", nullptr));
        lblAuth->setText(QCoreApplication::translate("MainWindow", "\354\235\270\354\246\235: \353\257\270\354\235\270\354\246\235", nullptr));
        lblStripTemp->setText(QCoreApplication::translate("MainWindow", "--.- \302\260C / -- %", nullptr));

        const bool __sortingEnabled = navList->isSortingEnabled();
        navList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = navList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "\352\263\204\352\270\260\355\214\220", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = navList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "\354\240\204\353\260\251 \354\271\264\353\251\224\353\235\274", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = navList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "\355\233\204\353\260\251 \354\271\264\353\251\224\353\235\274", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = navList->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "\354\232\264\354\240\204\354\236\220 \354\235\270\354\213\235", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = navList->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "\354\243\274\355\226\211 \354\241\260\354\236\221", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = navList->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("MainWindow", "\355\231\230\352\262\275 \354\204\244\354\240\225", nullptr));
        navList->setSortingEnabled(__sortingEnabled);

        grpCabin->setTitle(QCoreApplication::translate("MainWindow", "\354\260\250\354\213\244 \355\231\230\352\262\275", nullptr));
        lblCabinTemp->setText(QCoreApplication::translate("MainWindow", "--.- \302\260C", nullptr));
        lblHumidity->setText(QCoreApplication::translate("MainWindow", "\354\212\265\353\217\204 -- %", nullptr));
        grpSpeed->setTitle(QCoreApplication::translate("MainWindow", "\354\243\274\355\226\211 \354\206\215\353\217\204", nullptr));
        lblSpeed->setText(QCoreApplication::translate("MainWindow", "0 cm/s", nullptr));
        grpUltrasonic->setTitle(QCoreApplication::translate("MainWindow", "\354\264\210\354\235\214\355\214\214 \352\261\260\353\246\254", nullptr));
        lblUltraLeftName->setText(QCoreApplication::translate("MainWindow", "\354\242\214\354\270\241", nullptr));
        lblUltraLeft->setText(QCoreApplication::translate("MainWindow", "--- cm", nullptr));
        lblUltraFrontName->setText(QCoreApplication::translate("MainWindow", "\354\240\204\353\260\251", nullptr));
        lblUltraFront->setText(QCoreApplication::translate("MainWindow", "--- cm", nullptr));
        lblUltraRightName->setText(QCoreApplication::translate("MainWindow", "\354\232\260\354\270\241", nullptr));
        lblUltraRight->setText(QCoreApplication::translate("MainWindow", "--- cm", nullptr));
        grpCamInternal->setTitle(QCoreApplication::translate("MainWindow", "\353\202\264\354\236\245 \354\271\264\353\251\224\353\235\274 (\354\235\270\354\213\235 \354\213\234)", nullptr));
        lblCamInternal->setText(QCoreApplication::translate("MainWindow", "\353\214\200\352\270\260 \354\244\221", nullptr));
        lblRecogResult->setText(QCoreApplication::translate("MainWindow", "\354\235\270\354\213\235 \352\262\260\352\263\274: -", nullptr));
        grpDriverManage->setTitle(QCoreApplication::translate("MainWindow", "\354\232\264\354\240\204\354\236\220 \353\223\261\353\241\235 / \354\241\260\355\232\214", nullptr));
        btnRecogToggle->setText(QCoreApplication::translate("MainWindow", "\354\235\270\354\213\235 \354\213\234\354\236\221", nullptr));
        editDriverName->setPlaceholderText(QCoreApplication::translate("MainWindow", "\354\232\264\354\240\204\354\236\220 \354\235\264\353\246\204", nullptr));
        btnRegisterFace->setText(QCoreApplication::translate("MainWindow", "\354\226\274\352\265\264 \353\223\261\353\241\235", nullptr));
        btnDriverList->setText(QCoreApplication::translate("MainWindow", "\354\232\264\354\240\204\354\236\220 \354\241\260\355\232\214", nullptr));
        grpCamRear->setTitle(QCoreApplication::translate("MainWindow", "\355\233\204\353\260\251 \354\271\264\353\251\224\353\235\274", nullptr));
        lblCamRear->setText(QCoreApplication::translate("MainWindow", "NO SIGNAL", nullptr));
        lblRearWarning->setText(QCoreApplication::translate("MainWindow", "\355\233\204\353\260\251 \352\261\260\353\246\254: --- cm", nullptr));
        btnCamRearToggle->setText(QCoreApplication::translate("MainWindow", "\355\233\204\353\260\251 \354\271\264\353\251\224\353\235\274 \354\274\234\352\270\260", nullptr));
        btnModeToggle->setText(QCoreApplication::translate("MainWindow", "\353\252\250\353\223\234 \354\240\204\355\231\230", nullptr));
        btnHazard->setText(QCoreApplication::translate("MainWindow", "\353\271\204\354\203\201\353\223\261", nullptr));
        grpDirection->setTitle(QCoreApplication::translate("MainWindow", "\353\260\251\355\226\245 \354\241\260\354\236\221", nullptr));
        btnUp->setText(QCoreApplication::translate("MainWindow", "\342\226\262", nullptr));
        btnLeft->setText(QCoreApplication::translate("MainWindow", "\342\227\200", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        btnRight->setText(QCoreApplication::translate("MainWindow", "\342\226\266", nullptr));
        btnDown->setText(QCoreApplication::translate("MainWindow", "\342\226\274", nullptr));
        grpFan->setTitle(QCoreApplication::translate("MainWindow", "\354\204\240\355\222\215\352\270\260", nullptr));
        btnFanOn->setText(QCoreApplication::translate("MainWindow", "ON", nullptr));
        btnFanOff->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        btnFanAuto->setText(QCoreApplication::translate("MainWindow", "AUTO", nullptr));
        grpThreshold->setTitle(QCoreApplication::translate("MainWindow", "\354\236\204\352\263\204 \354\230\250\353\217\204", nullptr));
        spinThreshold->setSuffix(QCoreApplication::translate("MainWindow", " \302\260C", nullptr));
        btnApplyThreshold->setText(QCoreApplication::translate("MainWindow", "\354\240\201\354\232\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
