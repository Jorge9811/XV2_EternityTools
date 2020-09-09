#ifndef EMBDIALOG_H
#define EMBDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "Xenoverse2.h"

namespace Ui {
class EmbDialog;
}

class EmbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmbDialog(EmbFile *emb, QWidget *parent = 0);
    ~EmbDialog();

    void SetImageSize(int width, int height);

    inline QString GetResult() const { return result; }
    inline int GetResultInteger() const { return result_integer; }

private slots:
    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_buttonBox_accepted();

private:
    Ui::EmbDialog *ui;

    QString result;
    int result_integer=-1;
};

#endif // EMBDIALOG_H
