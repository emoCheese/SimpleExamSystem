#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMouseEvent>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>
#include <QFile>
#include <cstdio>
#include <QTextStream>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // 设置关闭即释放资源
    // 设置无边框
    // 设置鼠标移动实现拖动效果
    setAttribute(Qt::WA_DeleteOnClose);
    // setWindowState(Qt::WindowFullScreen);
    setWindowFlags(Qt::FramelessWindowHint);

    this->setMaximumHeight(540);
    this->setMaximumWidth(960);

    _moving = false;



    QFont font = ui->lineEditUsername->font();
    font.setPointSize(15);
    font.setFamily("微软雅黑");
    ui->lineEditPassword->setFont(font);
    ui->lineEditUsername->setFont(font);

    ui->lineEditPassword->setMinimumHeight(30);
    ui->lineEditUsername->setMinimumHeight(30);

    ui->lineEditPassword->setPlaceholderText("请输入密码");
    ui->lineEditUsername->setPlaceholderText("请输入用户名");

    // "^[a-zA-Z][\w]{5,17}@([A-Za-z0-9\-]+\.)+[A-Za-z]{2,6}$"
    // 在C/C++ 中需要使用 转义字符 \ 来表示 \，如\\，正则表达式中\也是转义字符
    _regExpEmail = new QRegExp("^[a-zA-Z][\\w]{3,17}@([A-Za-z0-9\\-]+\\.)+[A-Za-z]{2,6}$");
    QRegExpValidator* validator = new QRegExpValidator(*_regExpEmail,  this);
    ui->lineEditUsername->setValidator(validator);

    // 开发中使用
    ui->lineEditUsername->setText("admin@admin.com");
    ui->lineEditPassword->setText("admin");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    // 用户名格式检查
    QString username = ui->lineEditUsername->text();
    if (!username.isEmpty() && !_regExpEmail->exactMatch(username)) {
        QMessageBox::information(this, "通知", "邮箱错误");
        ui->lineEditUsername->clear();
        ui->lineEditUsername->setFocus();
        return;
    }

    // 密码非空检查
    QString password = ui->lineEditPassword->text();
    if (password.isEmpty()) {
        QMessageBox::information(this, "通知", "请输入密码");
        return;
    }

    // 打开文件并进行失败检测
    QFile file(acountDataPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "用户账号文件打开失败";
        perror("失败原因");
        return;
    }
    QTextStream stream(&file);

    // 读取账户名和密码 同时进行对比
    while(!stream.atEnd()) {
        QString strline = stream.readLine();
        QStringList acount = strline.split(',', Qt::SkipEmptyParts);
        if (acount.count() != 2) {
            return;
        }
        if (username == acount[0] && password == acount[1]) {
            accept();
            return;
        }
        // qDebug() << "用户名: " << acount[0];
        // qDebug() << "密码: " << acount[1];
    }
    file.close();
    QMessageBox::information(this, "通知", "用户名或密码错误");
    return;
}


void LoginDialog::on_btnExit_clicked()
{
    close();
}


void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    //鼠标在窗口的左键按下
    //event->button() 返回这个鼠标事件 event 对应的鼠标按键
    if(event->button() == Qt::LeftButton) {
        _moving = true;
        //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
        //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
        _lastPosition = event->globalPos() - this->pos();
    }
    QDialog::mousePressEvent(event); //继续传递给父类
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _moving = false;
}

void LoginDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog::mouseDoubleClickEvent(event);
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标按下左键移动
    //因为已经使用 m_moving ，可以判断出已经按下左键，就不需要使用 event->buttons() && Qt::LeftButton
    //pos()函数是用于获取窗口的位置。它返回一个 QPoint 对象，表示窗口的左上角在父窗口坐标系中的坐标。
    if (_moving)
    {
        //参数是窗口的左上角相对于屏幕的位置
        move(event->globalPos()-_lastPosition);
        _lastPosition = event->globalPos() - pos();
    }
    QDialog::mouseMoveEvent(event);
}

