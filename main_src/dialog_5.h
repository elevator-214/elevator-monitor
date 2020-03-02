#ifndef DIALOG_5_H
#define DIALOG_5_H

#include <QDialog>

#include <QMessageBox>

namespace Ui {
class Dialog_5;
}

class Dialog_5 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_5(QWidget *parent = 0);
    ~Dialog_5();
    void load_text();//点开对话框的时候加载参数

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog_5 *ui;
};

#endif // DIALOG_5_H
