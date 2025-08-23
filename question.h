#ifndef QUESTION_H
#define QUESTION_H

#include <QLabel>
#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QVector>


namespace Ui {
class Question;
}

/**
 * @brief The QuestionType enum
 * SingleChoice 和 TrueFalse 分别表示单选题和判断题，使用 QRadioButton
 * MultipleChoice 表示多选题，使用复选框
 */
enum QuestionType {
    SingleChoice,
    MultipleChoice,
    TrueFalse,
    Defualt
};


class Question : public QWidget
{
    Q_OBJECT

public:
    explicit Question(QWidget *parent = nullptr);

    explicit Question(const QString& title, QWidget *parent = nullptr);
    // 标题，选项类型，父类
    explicit Question(const QString& title, QuestionType type, QWidget *parent = nullptr);
    // 标题，选项数量，选项类型，父类
    explicit Question(const QString& title, int num, QuestionType type, QWidget *parent = nullptr);

    ~Question();

    void setTile(const QString& title);

    void setType(QuestionType type);

    // checkbox radiobutton 的父类都是 QAbstractButton
    void addOption(const QString &text, QAbstractButton* btn);
    void addOption(QAbstractButton* btn);


    QString getSelectedChoice() const;

private:
    Ui::Question *ui;
    QVBoxLayout* _vLayout;
    QLabel* _labelTile;
    QVector<QAbstractButton*> _btns;
    QuestionType _type;
    int _score;
};

#endif // QUESTION_H
