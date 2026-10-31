#pragma once

#include <QMainWindow>
#include <QSerialPort>
#include <QIODeviceBase>
#include <QIODevice>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OnOpenCom();
    void OpenComTest(); 
    void CloseComTest();
    void SendMsg();     //发送数据
    void ReadData();
    QByteArray FormatData(const QString &rawText);   
    QByteArray ParseHexData(const QString &input, bool *ok, QString *errorMsg);   //格式加工厂

    //bool isOpenCom;

private:
    Ui::MainWindow *ui;
    QSerialPort* serialPort;
};
