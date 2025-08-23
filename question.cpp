#include "question.h"
#include "ui_question.h"
#include <QDebug>

Question::Question(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Question)
{
    ui->setupUi(this);

    _vLayout = new QVBoxLayout(this);
    _labelTile = new QLabel(this);

    this->setLayout(_vLayout);

    _vLayout->addWidget(_labelTile);

    _type = QuestionType::Defualt;
}

Question::Question(const QString &title, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Question)
{
    ui->setupUi(this);

    _vLayout = new QVBoxLayout(this);
    _labelTile = new QLabel(title, this);

    this->setLayout(_vLayout);

    _vLayout->addWidget(_labelTile);
    _type = QuestionType::Defualt;
}

Question::Question(const QString &title, int num, QuestionType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Question)
{
    ui->setupUi(this);

    _vLayout = new QVBoxLayout(this);
    _labelTile = new QLabel(title, this);

    this->setLayout(_vLayout);

    _vLayout->addWidget(_labelTile);

    _type = type;
    switch (type) {
    case QuestionType::MultipleChoice: {

        break;
    }
    case QuestionType::SingleChoice: {
        break;
    }
    case QuestionType::TrueFalse: {
        break;
    }
    default: {
        break;
    }
    }
}

Question::Question(const QString &title, QuestionType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Question)
{
    ui->setupUi(this);
    _vLayout = new QVBoxLayout(this);
    _labelTile = new QLabel(title, this);

    this->setLayout(_vLayout);

    _vLayout->addWidget(_labelTile);

    _type = type;
}

Question::~Question()
{
    for(auto button : std::as_const(_btns)) {
        delete button;
    }
    delete ui;
}

void Question::setTile(const QString &title)
{
    _labelTile->setText(title);
}

void Question::setType(QuestionType type)
{
    _type = type;
}

void Question::addOption(const QString& text, QAbstractButton * btn)
{
    switch (_type) {
        case QuestionType::MultipleChoice: {
            auto _btn = static_cast<QCheckBox*>(btn);
            _btn->setText(text);
            _btns.push_back(_btn);
            _vLayout->addWidget(_btn);
            break;
        }
        case QuestionType::SingleChoice: {
            auto _btn = static_cast<QRadioButton*>(btn);
            _btn->setText(text);
            _btns.push_back(_btn);
            _vLayout->addWidget(_btn);
            break;
        }
        case QuestionType::TrueFalse: {
            auto _btn = static_cast<QRadioButton*>(btn);
            _btn->setText(text);
            _btns.push_back(_btn);
            _vLayout->addWidget(_btn);
            break;
        }
        default: {
            break;
        }
    }
}

void Question::addOption(QAbstractButton *btn)
{
    switch (_type) {
    case QuestionType::MultipleChoice: {
        auto _btn = static_cast<QCheckBox*>(btn);
        _btns.push_back(_btn);
        _vLayout->addWidget(_btn);
        break;
    }
    case QuestionType::SingleChoice: {
        auto _btn = static_cast<QRadioButton*>(btn);
        _btns.push_back(_btn);
        _vLayout->addWidget(_btn);
        break;
    }
    case QuestionType::TrueFalse: {
        auto _btn = static_cast<QRadioButton*>(btn);
        _btns.push_back(_btn);
        _vLayout->addWidget(_btn);
        break;
    }
    default: {
        qDebug() << "未设置问题类型";
        break;
    }
    }
}

QString Question::getSelectedChoice() const
{
    switch (_type) {
        case QuestionType::MultipleChoice: {
            for(auto button : _btns) {
                auto btn = static_cast<QCheckBox*>(button);
                if (btn->checkState() == Qt::Checked)
                    return btn->text();
            }
            break;
        }
        case QuestionType::SingleChoice: {
            for(auto button : _btns) {
                auto btn = static_cast<QRadioButton*>(button);
                if (btn->isChecked())
                    return btn->text();
            }
            break;
        }
        case QuestionType::TrueFalse: {
            for(auto button : _btns) {
                auto btn = static_cast<QRadioButton*>(button);
                if (btn->isChecked())
                    return btn->text();
            }
            break;
        }
        default: {
            qDebug() << "未设置问题类型";
            return QString("");
            break;
        }
    }
    return QString("");
}
