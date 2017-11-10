#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cifx/cifxlinux.h"

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

private:
    Ui::MainWindow *ui;
    CIFXHANDLE hChannel;
    int iTimerID;
    int32_t iCommOK;
};

#endif // MAINWINDOW_H
