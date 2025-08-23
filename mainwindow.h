#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>

#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_timeout();

private:
    void setWindowUI();
    void initTextEdit();
    void initTimer();

    Ui::MainWindow *ui;
    QTimer* _timer;
    QLabel* _labelTime;

    unsigned long long _second;
};
#endif // MAINWINDOW_H
