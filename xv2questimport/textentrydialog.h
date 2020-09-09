#ifndef TEXTENTRYDIALOG_H
#define TEXTENTRYDIALOG_H

#include <QDialog>

namespace Ui {
class TextEntryDialog;
}

class TextEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextEntryDialog(QWidget *parent = 0);
    ~TextEntryDialog();

private:
    Ui::TextEntryDialog *ui;
};

#endif // TEXTENTRYDIALOG_H
