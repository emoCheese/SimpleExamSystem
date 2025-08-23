#ifndef SELECTFORM_H
#define SELECTFORM_H

#include <QWidget>

namespace Ui {
class SelectForm;
}

class SelectForm : public QWidget
{
    Q_OBJECT

public:
    explicit SelectForm(QWidget *parent = nullptr);
    ~SelectForm();

private:
    Ui::SelectForm *ui;
};

#endif // SELECTFORM_H
