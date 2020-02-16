#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include "cifx/cifxlinux.h"


#define BIT_INPUT_PITCH 20
#define NUMBER_OF_BIT_INPUTS 4
class TbitInput : public QCheckBox
{
    Q_OBJECT

public:
    TbitInput(int iIndex = -1, QWidget *parent = nullptr);
    ~TbitInput();
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_Btn_TestBoard_clicked();

    void on_Btn_Data_clicked();

    void on_btnTestIo_clicked();

private:
    Ui::MainWindow *ui;
    CIFXHANDLE hChannel;
    int iTimerID;
    int32_t iCommOK;

public:
    TbitInput *bitInputs[NUMBER_OF_BIT_INPUTS];
};




#endif // MAINWINDOW_H
