#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QDebug>
#include <QTextStream>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _second = 0;
    _questionNum = 0;
    initTimer();
    initTextEdit();
    initButtons();
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
    bool isFirst = true;
    while(!stream.atEnd()) {
        QString line = stream.readLine().trimmed(); // 读取行并去除首尾空格
        if (line.isEmpty())
            continue;
        QChar cval = line.at(0);
        if (cval == '!') {    // 答案
            line.remove(0,1);
            qDebug() << line;
            _ansList.append(line);
            continue;
        } else if (cval == '#') {  // 注释
            _noteList.append(line);
            qDebug() << line;
            continue;
        } else if (cval == 'Q') {   // 题目
            QString txt;
            if (isFirst) {
                txt = line.remove(0,1);
                isFirst = false;
                ui->plainTextEdit->appendPlainText(txt);
                _questionNum++;
                continue;
            }
            txt = "\n" + line.remove(0,1);
            ui->plainTextEdit->appendPlainText(txt);
            _questionNum++;
            continue;
        } else if (cval == '.') {   // 选项
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

void MainWindow::initButtons()
{
    // qDebug() << "问题数量: " << _questionNum;
    if (_questionNum != _ansList.count()) {
        qDebug() << "问题数量与答案数量不一致";
        return;
    }
    for(int i = 0; i < _questionNum; ++i) {
        Question* question = new Question("题目 " + QString::number(i + 1));
        _questions.push_back(question);
        ui->horizontalLayout->addWidget(question, i);
    }

    for(int i = 0; i < _ansList.count(); ++i) {
        QStringList line = _ansList[i].split(' ', Qt::SkipEmptyParts);
        QString ans = line.at(1);
        if (ans.isEmpty()) {
            continue;
        }
        // qDebug() << ans;
        QString str = "ABCD";
        if (ans.count() == 1) { // 单选
            _questions[i]->setType(QuestionType::SingleChoice);
            for(int j = 0; j < 4; ++j) {
                QRadioButton* rbtn = new QRadioButton();
                rbtn->setText(str.at(j));
                _questions[i]->addOption(rbtn);
            }
        } else {
            if (ans == "正确" || ans == "错误") { // 判断
                _questions[i]->setType(QuestionType::TrueFalse);
                for(int j = 0; j < 2; ++j) {
                    QRadioButton* rbtn = new QRadioButton();
                    if (j == 0) rbtn->setText("正确");
                    else rbtn->setText("错误");
                    _questions[i]->addOption(rbtn);
                }
                QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
                _questions[i]->addSpacerItem(spacer);
            } else {  // 多选
                _questions[i]->setType(QuestionType::MultipleChoice);
                for(int j = 0; j < 4; ++j) {
                    auto chkBox = new QCheckBox();
                    chkBox->setText(str.at(j));
                    _questions[i]->addOption(chkBox);
                }
            }
        }
    }
}

void MainWindow::on_btnCommit_clicked()
{
    // QVector<AnswerItem*> items;

    // // 获取 单选，判断的题
    // int count = _groups.count();
    // for(int i = 0; i < count; ++i) {
    //     auto group = _groups[i];
    //     auto list = group->buttons();  // QAbstractButton* List
    //     for(int i = 0; i < list.count(); ++i) {
    //         auto rbtn = static_cast<QRadioButton*>(list[i]);
    //         // 获取选项
    //         rbtn->text();
    //         AnswerItem* item = new AnswerItem();
    //     }
    // }

}

