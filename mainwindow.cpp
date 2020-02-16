#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "stdint.h"

//***********************************
TbitInput::TbitInput(int iIndex, QWidget *parent) : QCheckBox(parent)
{
    this->setText(tr("bit %1").arg(iIndex));
    if (iIndex >= 0)
    {
       this->move(100,iIndex*BIT_INPUT_PITCH);
    }

}

TbitInput::~TbitInput()
{

}
//*********************************

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hChannel = NULL;
    iCommOK = -1;
    iTimerID = this->startTimer(1000);


    QGridLayout *bitInputsLayout = new QGridLayout(ui->centralWidget);

    //test adding widgets
    //TbitInput *bitInput1 = new TbitInput(-1, ui->centralWidget);
    for (int i = 0; i < NUMBER_OF_BIT_INPUTS; ++i)
    {
      bitInputs[i] =  new TbitInput(i);
      //bitInputsLayout->addWidget(bitInputs[i], i, 0);
      ui->inputsLayout->addWidget(bitInputs[i]);
    }
    //bitInputsLayout->setGeometry(QRect(50,50,50,50));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btn_TestBoard_clicked()
{
   qDebug() << "Starting to Test HAT";
   struct CIFX_LINUX_INIT init;
   memset(&init, 0, sizeof(init));
   {
     init.init_options        = CIFX_DRIVER_INIT_AUTOSCAN;
     init.iCardNumber         = 0;
     init.fEnableCardLocking  = 0;
     init.base_dir            = nullptr;
     init.poll_interval       = 0;
     init.poll_StackSize      = 0;   /* set to 0 to use default */
     init.trace_level         = 255;
     init.user_card_cnt       = 0;
     init.user_cards          = nullptr;
   }


   CIFXHANDLE hDriver = nullptr;
   int32_t lRet = CIFX_NO_ERROR;
   lRet = cifXDriverInit(&init);
   qDebug() << "DRV INIT" << lRet <<  QString::number(lRet, 16 ).toUpper();

   lRet = xDriverOpen(&hDriver);
   qDebug() << "DRV OPEN" << lRet <<  QString::number(lRet, 16 ).toUpper();

   DRIVER_INFORMATION tDriverInfo;
   memset(&tDriverInfo, 0, sizeof(tDriverInfo));
   lRet = xDriverGetInformation(nullptr, sizeof(tDriverInfo), &tDriverInfo);
   qDebug() << "DRV INFO" << lRet <<  QString::number(lRet, 16 ).toUpper();

   char szDrvVersion[32] = "";
   lRet = cifXGetDriverVersion(sizeof(szDrvVersion)/sizeof(*szDrvVersion), szDrvVersion);
   qDebug() << sizeof(szDrvVersion) << sizeof(*szDrvVersion);
   qDebug() << "DRV VERS" << lRet <<  QString::number(lRet, 16 ).toUpper();
   qDebug() << tDriverInfo.abDriverVersion << szDrvVersion;
}

void MainWindow::on_Btn_Data_clicked()
{
    int32_t lRet = CIFX_NO_ERROR;
    //Open channel

    lRet = xChannelOpen(nullptr, (char *)("cifX0"), 0, &hChannel);
    qDebug() << "CH OPEN" << lRet <<  QString::number(lRet, 16 ).toUpper();

    //channel info
    CHANNEL_INFORMATION tChannelInfo;
    memset(&tChannelInfo, 0, sizeof(tChannelInfo));
    lRet = xChannelInfo(hChannel, sizeof(CHANNEL_INFORMATION), &tChannelInfo);
    qDebug() << "CH INFO" << lRet <<  QString::number(lRet, 16 ).toUpper();
    qDebug() << tChannelInfo.ulDeviceNumber
             << tChannelInfo.ulSerialNumber
             << tChannelInfo.abFWName
             << tChannelInfo.usFWMajor
             << tChannelInfo.usFWMinor
             << tChannelInfo.usFWRevision
             << tChannelInfo.usFWBuild
             << tChannelInfo.bFWMonth
             << tChannelInfo.bFWDay
             << tChannelInfo.usFWYear
             << tChannelInfo.ulMailboxSize;

    //estabilish communication with channel

    uint32_t ulState;
    do
    {
        lRet = xChannelBusState(hChannel, CIFX_BUS_STATE_ON, &ulState, 10);
        qDebug() << "CH BUS STATE" << lRet <<  QString::number(lRet, 16 ).toUpper();


    } while (lRet == CIFX_DEV_NO_COM_FLAG);
    iCommOK = CIFX_NO_ERROR;

}

void MainWindow::timerEvent(QTimerEvent *event)
{
    return;
    if (event->timerId() == iTimerID)
    {
        qDebug() << "TimerEvent" << hChannel << iCommOK;
        uint8_t abSendData[32];
        memset(abSendData, 0, sizeof(abSendData));
        if (ui->cbBitComm->checkState()) abSendData[0] = 0x80;
        else                             abSendData[0] = 0x00;

        if ((hChannel != nullptr) && (iCommOK == CIFX_NO_ERROR))
        {
           uint32_t lRet = CIFX_NO_ERROR;
           lRet = xChannelIOWrite(hChannel, 0, 0, sizeof(abSendData), abSendData, 10);
           qDebug() << "CH WRITE" << lRet <<  QString::number(lRet, 16 ).toUpper();


        }
    }
}


#define SET_IND(ind, status)\
{\
    if  ((status)) ui->ind->setStyleSheet("background-color:red;");\
    else           ui->ind->setStyleSheet("background-color: green;");\
}

void MainWindow::on_btnTestIo_clicked()
{
    if  (ui->cb_b0->isChecked()) ui->Ind_b0->setStyleSheet("background-color:red;");
    else                         ui->Ind_b0->setStyleSheet("background-color: green;");

    SET_IND(Ind_b1, ui->cb_b1->isChecked());
    SET_IND(Ind_b2, ui->cb_b2->isChecked());
    SET_IND(Ind_b3, ui->cb_b3->isChecked());
}
