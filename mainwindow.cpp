#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QDebug>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _second = 0;
    initTimer();
    initTextEdit();
    setWindowUI();
    setWindowTitle("考试用时: --:--:--");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_timeout()
{
    _second++;

    // 法1
    // quint64 h = _second / 3600;
    // quint64 m = (_second % 3600) / 60;
    // quint64 s = _second % 60;
    // QString str2 = QString::asprintf("%llu 小时 %llu 分 %llu 秒", h, m, s);

    // _labelTime->setText("考试用时: " + str);

    // 法2
    QTime time(0, 0);
    QString str = time.addSecs(_second).toString("hh 小时 mm 分 ss 秒");
    // _labelTime->setText("考试用时: " + str);
    setWindowTitle("考试用时: " + str);
}

void MainWindow::setWindowUI()
{
    _labelTime = new QLabel(this);
    _labelTime->setText("考试用时: --:--:--");
    ui->statusbar->addWidget(_labelTime);

}

void MainWindow::initTextEdit()
{
    ui->plainTextEdit->setReadOnly(true);
    QFont font = ui->plainTextEdit->font();
    font.setPointSize(15);
    font.setFamily("微软雅黑");
    ui->plainTextEdit->setFont(font);

    QString filepath = ":/data/data/exam.txt";
    QFile file(filepath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "考试题目文件打开失败";
        perror("原因:");
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("utf-8");
    QStringList noteList;
    QStringList ansList;
    bool isFirst = true;
    while(!stream.atEnd()) {
        QString line = stream.readLine().trimmed(); // 读取行并去除首尾空格
        if (line.isEmpty())
            continue;
        QChar cval = line.at(0);
        if (cval == '!') {
            line.remove(0,1);
            qDebug() << line;
            ansList.append(line);
            continue;
        } else if (cval == '#') {
            noteList.append(line);
            qDebug() << line;
            continue;
        } else if (cval == 'Q') {
            QString txt;
            if (isFirst) {
                txt = line.remove(0,1);
                isFirst = false;
                ui->plainTextEdit->appendPlainText(txt);
                continue;
            }
            txt = "\n" + line.remove(0,1);
            ui->plainTextEdit->appendPlainText(txt);
            continue;
        } else if (cval == '.') {
            ui->plainTextEdit->appendPlainText(line.remove(0,1));
        }
    }
}

void MainWindow::initTimer()
{
    _timer = new QTimer(this);
    _timer->setInterval(1000);  // 毫秒
    _timer->setTimerType(Qt::CoarseTimer);

    connect(_timer, &QTimer::timeout, this, &MainWindow::slot_timeout);
    _timer->start();
}
