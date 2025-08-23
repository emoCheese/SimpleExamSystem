#ifndef ANSWERITEM_H
#define ANSWERITEM_H

#include <QObject>
#include <QLabel>
#include <QVector>
#include <QPair>

class AnswerItem : public QObject
{
    Q_OBJECT
public:
    explicit AnswerItem(QObject *parent = nullptr);

    // 题目标签
    AnswerItem(const QString& label);

    enum Option {
        Defualt = 0,  // 空选
        A = 1,
        B = 2,
        C = 3,
        D = 4,
        True = 5,
        False = 6
    };

    // 添加选项，如A, B, C, D
    void addItem(const QString& item);
    void addItem(const QString& item, AnswerItem::Option option);

    QVector<QPair<QString, AnswerItem::Option>>& get_items();

    void isValid();
    // 多选 答案
    void addAnswer(const QStringList& list);

    // 单选或判断 答案
    void addAnswer(const QString& answer);

signals:

private:
    int _total_score;
    int _score;

    int _num;

    QString _label; // 题目标签
    bool _is_singleType;
    QVector<QPair<QString, AnswerItem::Option>> _userAnswers;   // 用户答案兼容多选
    QStringList _answerList;


};

#endif // ANSWERITEM_H
