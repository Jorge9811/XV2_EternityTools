#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();

    void Append(const QString &str);

    inline void Append(const char *str)
    {
        Append(QString(str));
    }

    inline void SetNextBold(bool bold = true)
    {
        next_bold = bold;
    }

    inline void SetNextColor(QString color)
    {
        next_color = color;
    }

    void Resize(int increment_w, int increment_h);

private:
    Ui::LogDialog *ui;

    bool next_bold = false;
    QString next_color;


};

#endif // LOGDIALOG_H
