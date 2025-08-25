#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

#include <QVector>
#include <QMap>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include "question.h"
// #include "answeritem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class State {
    BEGIN,
    READ_NOTE,
    READ_QUESTION,
    READ_OPTION,
    READ_ANSWER,
    END
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_timeout();

    void on_btnCommit_clicked();

private:
    void setWindowUI();
    void initTextEdit();
    void initTimer();
    void initButtons();

    Ui::MainWindow *ui;
    QTimer* _timer;
    QLabel* _labelTime;

    QStringList _que;        // 题目
    QStringList _noteList;   // 注释
    QStringList _ansList;    // 答案 按顺序

    unsigned long long _second;
    int _questionNum;

    QVector<Question*> _questions;
};
#endif // MAINWINDOW_H
