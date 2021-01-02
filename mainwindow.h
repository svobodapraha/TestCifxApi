#define SIMULATOR

#ifdef __arm__
  #undef SIMULATOR
#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include "stdint.h"
#include "stdtypes.h"

#ifndef SIMULATOR
  #include "cifx/cifxlinux.h"
#endif


#define BIT_INPUT_PITCH 20
#define NUMBER_OF_BIT_INPUTS 64
class TbitInput : public QCheckBox
{
    Q_OBJECT

public:
    TbitInput(int iIndex = -1, QWidget *parent = nullptr);
    ~TbitInput();
    bool GetStatus();
    void Reset();
};

class TbitOutput : public QLineEdit
{
    //Q_OBJECT
public:
    TbitOutput(int iIndex = -1, QWidget *parent = nullptr);
    ~TbitOutput();
    void SetStatus(bool boStatus);
};



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_Btn_TestBoard_clicked();

    void on_Btn_Data_clicked();



    void on_btnResetInputs_clicked();

    void on_btn_InitDrv_clicked();

    void on_btn_OpenChannel_clicked();

private:
    Ui::MainWindow *ui;
#ifndef SIMULATOR
    CIFXHANDLE hChannel;
    CIFXHANDLE hDriver;
#else
    int * hChannel;
    int * hDriver;
#endif

    int iTimerID;
    int32_t iCommOK;

public:
    TbitInput *bitInputs[NUMBER_OF_BIT_INPUTS];
    TbitOutput *bitOutputs[NUMBER_OF_BIT_INPUTS];
    UU64 InputData;
    UU64 OutputData;
    uint8_t  abSendData[32];
    uint8_t  abRecvData[32];

    bool boDriverInitOK;
    bool boDriverOpenOK;
    bool boChannelOpenOK;
    bool boBusCommOK;

    int CleanUpAll();

    QStatusBar * ComStatus;
};




#endif // MAINWINDOW_H
