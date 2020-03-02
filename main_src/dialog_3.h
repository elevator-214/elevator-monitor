#ifndef DIALOG_3_H
#define DIALOG_3_H

#include <QDialog>

#include <QMessageBox>

namespace Ui {
class Dialog_3;
}

class Dialog_3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_3(QWidget *parent = 0);
    ~Dialog_3();
    void load_text();//点开对话框的时候加载参数

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog_3 *ui;
};

#endif // DIALOG_3_H
