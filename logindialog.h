#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // QWidget interface 最后要记得传递给父类
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_btnLogin_clicked();

    void on_btnExit_clicked();

private:
    Ui::LoginDialog *ui;
    const QString acountDataPath = ":/data/data/account.txt";

    QRegExp* _regExpEmail;
    // QRegExp* _regExpPassword;
    QPoint _lastPosition;
    bool _moving;


};

#endif // LOGINDIALOG_H
