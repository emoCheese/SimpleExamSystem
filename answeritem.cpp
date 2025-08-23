#include "answeritem.h"

AnswerItem::AnswerItem(QObject *parent)
    : QObject{parent}
{
    _num = 0;
    _score = 0;
}

AnswerItem::AnswerItem(const QString &label)
{
    _num = 0;
    _score = 0;
}

void AnswerItem::addItem(const QString &item)
{
    for(int i = 0; i < _userAnswers.count(); ++i) {
        if (_userAnswers[i].first == item) {
            return;
        }
    }
    QPair<QString, AnswerItem::Option> pitem(item, AnswerItem::Defualt);
    _userAnswers.push_back(pitem);
}

void AnswerItem::addItem(const QString &item, Option option)
{
    for(int i = 0; i < _userAnswers.count(); ++i) {
        if (_userAnswers[i].first == item) {
            return;
        }
    }
    QPair<QString, AnswerItem::Option> pitem(item, option);
    _userAnswers.push_back(pitem);
}

QVector<QPair<QString, AnswerItem::Option> > &AnswerItem::get_items()
{
    return _userAnswers;
}

void AnswerItem::isValid()
{

}
