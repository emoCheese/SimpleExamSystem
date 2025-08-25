#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QDebug>
#include <QTextStream>
#include <QSpacerItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _second = 0;
    _questionNum = 0;

    initTextEdit();
    initButtons();
    setWindowUI();
    setWindowTitle("考试用时: --:--:--");
    initTimer();

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


    // 状态机进行读取
    bool needRead = true;
    State currentState = State::BEGIN;
    QString line;
    QChar cval;
    while(1) {
        if (needRead) {
            if (stream.atEnd()) {
                break;
            }
            line = stream.readLine().trimmed(); // 读取行并去除首尾空格
            if (line.isEmpty())
                continue;
            cval = line.at(0);
        } else {
            needRead = true;
        }
        switch (currentState) {
        case State::BEGIN:
            if(cval == '#')  { // 读注释
                _noteList.append(line.remove(0,1));
            } else if (cval == 'Q') {  // 若当前读到的行是 题目，则下个循环不进行读取，needRead 置为 false;
                currentState = State::READ_QUESTION;
                needRead = false;
            }
            break;
        case State::READ_QUESTION:    // 读问题
            if (cval == 'Q') {
                QString txt;
                if (isFirst) {
                    txt = line.remove(0,1);
                    qDebug() << txt;
                    _que.append(txt);
                    isFirst = false;
                    ui->plainTextEdit->appendPlainText(txt);
                    _questionNum++;
                    currentState = State::READ_OPTION;
                    break;
                }
                txt = line.remove(0,1);
                qDebug() << txt;
                _que.append(txt);
                txt = "\n" + txt;
                ui->plainTextEdit->appendPlainText(txt);
                _questionNum++;
                currentState = State::READ_OPTION;
            } else {
                // to do 报错
            }
            break;
        case State::READ_OPTION:    // 读选项
            if (cval == '.') {
                qDebug() << line;
                ui->plainTextEdit->appendPlainText(line.remove(0,1));
            } else if (cval == '!') {   // 若当前读到的行是 答案，则下个循环不进行读取，needRead 置为 false;
                needRead = false;
                currentState = State::READ_ANSWER;
            }
            break;
        case State::READ_ANSWER:   // 读答案
            if (cval == '!') {
                QString ans = line.remove(0,3).trimmed();
                qDebug() << "答案:" <<ans;
                _ansList.append(ans);
                currentState = State::READ_QUESTION;
            }
            break;
        case State::END:

            break;
        default:

            break;
        }
    }


    /*
    // 循环进行读取
    // while(!stream.atEnd()) {
    //     QString line = stream.readLine().trimmed(); // 读取行并去除首尾空格
    //     if (line.isEmpty())
    //         continue;
    //     QChar cval = line.at(0);
    //     if (cval == '!') {    // 答案
    //         line.remove(0,1);
    //         qDebug() << line;
    //         _ansList.append(line);
    //         continue;
    //     } else if (cval == '#') {  // 注释
    //         _noteList.append(line);
    //         qDebug() << line;
    //         continue;
    //     } else if (cval == 'Q') {   // 题目
    //         QString txt;
    //         if (isFirst) {
    //             txt = line.remove(0,1);
    //             _que.append(txt);
    //             isFirst = false;
    //             ui->plainTextEdit->appendPlainText(txt);
    //             _questionNum++;
    //             continue;
    //         }
    //         txt = line.remove(0,1);
    //         _que.append(txt);
    //         txt = "\n" + txt;
    //         ui->plainTextEdit->appendPlainText(txt);
    //         _questionNum++;
    //         continue;
    //     } else if (cval == '.') {   // 选项
    //         ui->plainTextEdit->appendPlainText(line.remove(0,1));
    //     }
    // }
    */
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

    qDebug() << "_ansList.count(): " << _ansList.count();
    for(int i = 0; i < _ansList.count(); ++i) {
        // 构造 Question 并设置 UI
        Question* question = new Question("题目 " + QString::number(i + 1));
        question->setQuestion(_que[i]);
        question->setAnswer(_ansList[i]);

        _questions.push_back(question);
        ui->horizontalLayout->addWidget(question, i);
        QSpacerItem* hspacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui->horizontalLayout->addSpacerItem(hspacer);

        // 添加选项
        QString ans = _ansList[i];
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
                QSpacerItem* vspacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
                _questions[i]->addSpacerItem(vspacer);
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
    // _ansList.count()
}

void MainWindow::on_btnCommit_clicked()
{
    int totalScore = 0;
    for(const auto& val : qAsConst(_questions)) {
        totalScore += val->getScore();
    }
    // qDebug() << "-----------------";
    // for(int i = 0; i < 10; ++i) {
    //     totalScore += _questions[i]->getScore();
    //     qDebug() << _ansList[i] << " " << _questions[i]->getAnswer();
    // }
    QMessageBox::information(this, "成绩", QString::asprintf("您的成绩是: %d", totalScore));
}

