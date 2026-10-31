#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //获取串口上面的串口类型放进QList数组里面，比如说是 COM1之类的
    serialPort = new QSerialPort(this);
    QList<QSerialPortInfo> serialList =  QSerialPortInfo::availablePorts();
    for(QSerialPortInfo serialInfo : serialList){
        qDebug()<<serialInfo.portName();   
        ui->comboBox->addItem(serialInfo.portName());  //将串口名称打印在控件上面让客户选择
    } 

   // connect(ui->pushButton_17,&QMainWindow::clicked,&QMainWindow,QMainWindow::OnOpenCom);
    connect(ui->pushButton_17, &QPushButton::clicked, this, &MainWindow::OnOpenCom);
    connect(ui->pushButton_27, &QPushButton::clicked, this, &MainWindow::SendMsg);
    // 4. 核心：绑定串口接收信号
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::ReadData);
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete serialPort;
}

void MainWindow::OnOpenCom(){
    //先进行判断，此时时打开串口还是关闭串口
    if(!serialPort->isOpen())
    {
        //调用打开串口的逻辑函数
        OpenComTest();
    }
    else{
        ui->pushButton_17->setText("打开串口");
        //调用关闭串口逻辑函数
        CloseComTest();
    }
}

void MainWindow::OpenComTest(){
    //初始化波特率等基础设置
    serialPort->setPortName(ui->comboBox->currentText());     //设置名称
    serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());   //波特率：9600
    
     // 3. 获取数据位：字符串转枚举
    QString dataStr = ui->comboBox_3->currentText();
    if(dataStr == "5") serialPort->setDataBits(QSerialPort::Data5);
    else if(dataStr == "6") serialPort->setDataBits(QSerialPort::Data6);
    else if(dataStr == "7") serialPort->setDataBits(QSerialPort::Data7);
    else serialPort->setDataBits(QSerialPort::Data8); // 默认8

    // 4. 获取校验位：字符串转枚举（注意大小写，看你界面上怎么写的）
    QString parityStr = ui->comboBox_4->currentText();
    if(parityStr == "Even" || parityStr == "偶校验") 
        serialPort->setParity(QSerialPort::EvenParity);
    else if(parityStr == "Odd" || parityStr == "奇校验") 
        serialPort->setParity(QSerialPort::OddParity);
    else 
        serialPort->setParity(QSerialPort::NoParity); // 默认无校验

    // 5. 获取停止位
    QString stopStr = ui->comboBox_5->currentText();
    if(stopStr == "2" || stopStr == "TwoStop") 
        serialPort->setStopBits(QSerialPort::TwoStop);
    else 
        serialPort->setStopBits(QSerialPort::OneStop);

    // 流控如果你界面上有下拉框，同理处理，暂时可以写死无流控
    serialPort->setFlowControl(QSerialPort::NoFlowControl);


    // serialPort->setDataBits(QSerialPort::Data8);
    // serialPort->setParity(QSerialPort::NoParity);    //校验位：无
    // serialPort->setStopBits(QSerialPort::OneStop);
    // serialPort->setFlowControl(QSerialPort::NoFlowControl);

    //显示在界面上
    // ui->comboBox_2

     // 真正尝试打开，并判断是否成功
    if(serialPort->open(QIODeviceBase::ReadWrite)){
        ui->pushButton_17->setText("关闭串口");
        ui->comboBox->setEnabled(false); // 打开后禁止修改串口号
        ui->comboBox_2->setEnabled(false); // 打开后禁止修改波特率
        ui->comboBox_3->setEnabled(false); // 打开后禁止修改数据位
        ui->comboBox_4->setEnabled(false); // 打开后禁止修改校验位
        ui->comboBox_5->setEnabled(false); // 打开后禁止修改停止位
        ui->comboBox_6->setEnabled(false); // 打开后禁止修改流控
        qDebug() << "串口打开成功！";
    } else {
        QMessageBox::warning(this, "错误", "无法打开串口: " + serialPort->errorString());
    }
}

void MainWindow::CloseComTest(){
    serialPort->close();
    ui->pushButton_17->setText("打开串口");
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true); // 打开后禁止修改波特率
    ui->comboBox_3->setEnabled(true); // 打开后禁止修改数据位
    ui->comboBox_4->setEnabled(true); // 打开后禁止修改校验位
    ui->comboBox_5->setEnabled(true); // 打开后禁止修改停止位
    ui->comboBox_6->setEnabled(true); // 打开后禁止修改流控
    qDebug() << "串口已关闭！";
}

void MainWindow::SendMsg(){

    if(!serialPort->isOpen())
        return;

    //获取数据，并写入到缓存区
    QByteArray data = ui->lineEdit_15->text().toUtf8();
    qint64 bytesWritten = serialPort->write(data);
    if (bytesWritten == -1) {
        qDebug() << "写入失败:" << serialPort->errorString();
    } else {
        qDebug() << "成功写入" << bytesWritten << "字节";
    }

    // //阻塞线程，等待数据实际发送完成
    // if(serialPort->waitForBytesWritten(3000)){
    //     qDebug() << "数据已发送到串口";
    // }else{
    //     qDebug() << "发送超时";
    // }
}

void MainWindow::ReadData(){
     QByteArray data = serialPort->readAll();

if (data.isEmpty()) return;

    // 1. 将字节流转成字符串（如果遇到中文乱码，把 fromLocal8Bit 换成 fromUtf8）
    QString str = QString::fromLocal8Bit(data);

    // 2. 如果是普通文本模式，直接追加到末尾
    // appendPlainText 会自动换行，如果你希望收到的数据连续不断开，用 insertPlainText
    ui->plainTextEdit->moveCursor(QTextCursor::End); // 光标移到末尾
    //ui->plainTextEdit->insertPlainText(str);         // 插入文本（不强制换行）
    ui->plainTextEdit->appendPlainText(str);
    
    // 3. (可选) 如果你想限制最多显示多少行，防止内存爆掉：
    // ui->plainTextEdit_Receive->setMaximumBlockCount(1000); // 放在构造函数里
}