#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>



//***********************************
TbitInput::TbitInput(int iIndex, QWidget *parent) : QCheckBox(parent)
{
    this->setText(tr("B%1").arg(iIndex));
}

bool TbitInput::GetStatus()
{
    return this->isChecked();
}

void TbitInput::Reset()
{
   this->setChecked(false);
}

TbitInput::~TbitInput()
{

}

TbitOutput::TbitOutput(int iIndex, QWidget *parent) : QLineEdit(parent)
{

    this->setText(tr("B%1").arg(iIndex));
    this->setFixedSize(30,15);
    this->setStyleSheet("background-color:white;");

}

void TbitOutput::SetStatus(bool boStatus)
{
   if(boStatus) this->setStyleSheet("background-color:green;color: white;");
   else         this->setStyleSheet("background-color:white;color: black;");
}

TbitOutput::~TbitOutput()
{

}









//*********************************

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //init
    boDriverInitOK  = false;
    boDriverOpenOK    = false;
    boChannelOpenOK = false;
    hDriver         = nullptr;
    hChannel        = nullptr;
    memset(abSendData, 0, sizeof(abSendData));
    memset(abRecvData, 0, sizeof(abSendData));
    InputData.u64 = 0;
    OutputData.u64 = 0;

    CleanUpAll();

    ui->setupUi(this);
    ComStatus = this->statusBar();

    iCommOK = -1;
    iTimerID = this->startTimer(333);


    QGridLayout *bitInputsLayout = new QGridLayout(ui->groupBox_inputs);
    QGridLayout *bitOutputsLayout = new QGridLayout(ui->groupBox_outputs);



    //test adding widgets
    //TbitInput *bitInput1 = new TbitInput(-1, ui->centralWidget);
    int x=0;
    int y=0;
    for (int i = 0; i < NUMBER_OF_BIT_INPUTS; ++i)
    {
      bitInputs[i] =  new TbitInput(i);
      bitOutputs[i] = new TbitOutput(i);
      bitInputsLayout->addWidget(bitInputs[i], x, y);
      bitOutputsLayout->addWidget(bitOutputs[i], x, y);
      //ui->inputsLayout->addWidget(bitInputs[i]);

      //new position
      if ((i+1)%8 == 0)
      {
        x=0;
        y++;
      }
      else
      {
        x++;
      }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

//Init Driver
void MainWindow::on_btn_InitDrv_clicked()
{
    qDebug() << endl <<  __FUNCTION__;
    CleanUpAll();

#ifndef SIMULATOR
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


    //Initialize driver
    int32_t lRet = CIFX_NO_ERROR;
    lRet = cifXDriverInit(&init);
    qDebug() << "DRV INIT" << lRet <<  QString::number(lRet, 16 ).toUpper();
    if(lRet != CIFX_NO_ERROR)
    {
        CleanUpAll();
        return;
    }

    boDriverInitOK = true;
#endif
}

//Display driver information
void MainWindow::on_Btn_TestBoard_clicked()
{

   qDebug() << endl <<  __FUNCTION__;
#ifndef SIMULATOR



   int32_t lRet = CIFX_NO_ERROR;

   //Check if drive is initialised
   if(!boDriverInitOK)
   {
       qDebug() << "Driver is not initialised";
       return;
   }



   //Open Driver
   if (!boDriverOpenOK)
   {
       lRet = xDriverOpen(&hDriver);
       qDebug() << "DRV OPEN" << lRet <<  QString::number(lRet, 16 ).toUpper() << hDriver;
       if(lRet != CIFX_NO_ERROR)
       {
           CleanUpAll();
           return;
       }
       boDriverOpenOK = true;
   }


   //Display Driver Informatin
   DRIVER_INFORMATION tDriverInfo;
   memset(&tDriverInfo, 0, sizeof(tDriverInfo));
   lRet = xDriverGetInformation(nullptr, sizeof(tDriverInfo), &tDriverInfo);
   qDebug() << "DRV INFO" << lRet <<  QString::number(lRet, 16 ).toUpper();
   if(lRet != CIFX_NO_ERROR)
   {
       CleanUpAll();
       return;
   }

   //Display Driver Version
   char szDrvVersion[32] = "";
   lRet = cifXGetDriverVersion(sizeof(szDrvVersion)/sizeof(*szDrvVersion), szDrvVersion);
   //qDebug() << sizeof(szDrvVersion) << sizeof(*szDrvVersion) << sizeof(szDrvVersion[0]) << sizeof(&szDrvVersion);
   qDebug() << "DRV VERS" << lRet <<  QString::number(lRet, 16 ).toUpper();
   qDebug() << tDriverInfo.abDriverVersion << szDrvVersion;

   if(lRet != CIFX_NO_ERROR)
   {
       CleanUpAll();
       return;
   }

   //Close drive again
   boChannelOpenOK = false;
   hChannel = nullptr;

   boDriverOpenOK   = false;
   xDriverClose(hDriver);
   hDriver = nullptr;


#endif
}


void MainWindow::on_btn_OpenChannel_clicked()
{
    qDebug() << endl <<  __FUNCTION__;
#ifndef SIMULATOR
    int32_t lRet = CIFX_NO_ERROR;

    //Check if drive is initialised
    if(!boDriverInitOK)
    {
        qDebug() << "Driver is not initialised";
        return;
    }



    //Open Driver
    if (!boDriverOpenOK)
    {
        lRet = xDriverOpen(&hDriver);
        qDebug() << "DRV OPEN" << lRet <<  QString::number(lRet, 16 ).toUpper() << hDriver;
        if(lRet != CIFX_NO_ERROR)
        {
            CleanUpAll();
            return;
        }
        boDriverOpenOK = true;
    }


    if (!boChannelOpenOK)
    {
        //Open Channel
        char CIFX_DEV[] = "cifX0";  //cookies for next function - 2nd parameter must be char*, not const char *
        lRet = xChannelOpen(nullptr, CIFX_DEV, 0, &hChannel);
        qDebug() << "CH OPEN" << lRet <<  QString::number(lRet, 16 ).toUpper();
        if(lRet != CIFX_NO_ERROR)
        {
            CleanUpAll();
            return;
        }
    }
    boChannelOpenOK = true;


    //Show channel information
    //channel info
    CHANNEL_INFORMATION tChannelInfo;
    memset(&tChannelInfo, 0, sizeof(tChannelInfo));
    lRet = xChannelInfo(hChannel, sizeof(CHANNEL_INFORMATION), &tChannelInfo);
    qDebug() << "CH INFO" << lRet <<  QString::number(lRet, 16 ).toUpper();
    if(lRet != CIFX_NO_ERROR)
    {
        CleanUpAll();
        return;
    }


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



#endif
}

int MainWindow::CleanUpAll()
{
    boBusCommOK = false;

    boChannelOpenOK = false;
    if(hChannel != nullptr) xChannelClose(hChannel);
    hChannel = nullptr;

    boDriverOpenOK = false;
    if(hDriver != nullptr) xDriverClose(hDriver);
    hDriver = nullptr;

    boDriverInitOK = false;
    cifXDriverDeinit();
  return 0;
}

void MainWindow::on_Btn_Data_clicked()
{
#ifndef SIMULATOR
    int32_t lRet = CIFX_NO_ERROR;
    //Open channel


    //estabilish communication with channel

    uint32_t ulState;
    do
    {
        lRet = xChannelBusState(hChannel, CIFX_BUS_STATE_ON, &ulState, 10);
        qDebug() << "CH BUS STATE" << lRet <<  QString::number(lRet, 16 ).toUpper();


    } while (lRet == CIFX_DEV_NO_COM_FLAG);
    iCommOK = CIFX_NO_ERROR;
#endif
}

void MainWindow::timerEvent(QTimerEvent *event)
{

    if (event->timerId() == iTimerID)
    {

        //read checkboxes status
        for (int i = 0; i < NUMBER_OF_BIT_INPUTS; ++i)
        {

          if(bitInputs[i]->GetStatus())  InputData.u64 |= (U64(1)<<i);
          else                           InputData.u64 &= ~(U64(1)<<i);


        }





#ifndef SIMULATOR
        int32_t lRet = CIFX_NO_ERROR;

        //if all is opened
        if (boDriverInitOK && boDriverOpenOK && boChannelOpenOK)
        {

           //Wait for communication to be estabilished
            uint32_t ulState;
            lRet = xChannelBusState(hChannel, CIFX_BUS_STATE_ON, &ulState, 10);
            //qDebug() << "CH BUS STATE" << lRet <<  QString::number(lRet, 16 ).toUpper();
            if(lRet == CIFX_DEV_NO_COM_FLAG)
            {
                boBusCommOK = false;
                ComStatus->showMessage("Waiting for Bus communication!");
            }
            else
            {
                boBusCommOK = true;
                ComStatus->showMessage("Bus communication running!");
            }

            //Data communication
            if (boBusCommOK)
            {
               /* Read Data from network */
               lRet = xChannelIORead(hChannel, 0, 0, sizeof(abRecvData), abRecvData, 10);
               if(lRet == CIFX_NO_ERROR)
               {
                   OutputData.u64 = *(reinterpret_cast<U64*>(abRecvData));
                   //OutputData.u64 = *((U64*)&abRecvData);
               }
               else
               {
                  ComStatus->showMessage("Error read data!");
               }

               /* Write Data to network */
               memset(abSendData, 0, sizeof(abSendData));  //clear
               *(reinterpret_cast<U64*>(abSendData)) = InputData.u64;  //Fill From Checkboxes
               lRet = xChannelIOWrite(hChannel, 0, 0, sizeof(abSendData), abSendData, 10);  //send
               if (lRet == CIFX_NO_ERROR)
               {

               }
               else
               {
                  ComStatus->showMessage("Error write data!");
               }
            }


        }
        else
        {

            ComStatus->showMessage("Driver or Chanel are not ready!");
        }



#else
        qDebug() << InputData.u64;
        OutputData = InputData;
#endif


        //write to indicators
        for (int i = 0; i < NUMBER_OF_BIT_INPUTS; ++i)
        {

          if((OutputData.u64 >> i)& 0x1 ) bitOutputs[i]->SetStatus(true);
          else                            bitOutputs[i]->SetStatus(false);

        }



    }
}




void MainWindow::on_btnResetInputs_clicked()
{
    for (int i = 0; i < NUMBER_OF_BIT_INPUTS; ++i)
    {

      bitInputs[i]->Reset();
    }
}


