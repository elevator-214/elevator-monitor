#ifndef DIALOG_2_H
#define DIALOG_2_H

#include <QDialog>

#include <QMessageBox>

namespace Ui {
class Dialog_2;
}

class Dialog_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_2(QWidget *parent = 0);
    ~Dialog_2();
    void load_text();//点开对话框的时候加载参数

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog_2 *ui;
};

#endif // DIALOG_2_H
