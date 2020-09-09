#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::Append(const QString &str)
{
    QString msg = str;

    if (!next_color.isEmpty())
    {
        msg = "<font color='" + next_color + "'>" + msg + "</font>";
        next_color.clear();
    }

    if (next_bold)
    {
        msg = "<b>" + msg + "</b>";
        next_bold = false;
    }

    ui->textEdit->append(msg);
}

void LogDialog::Resize(int increment_w, int increment_h)
{
    int ww = this->width();
    int wh = this->height();
    int tw = ui->textEdit->width();
    int th = ui->textEdit->height();

    setFixedWidth(ww+increment_w);
    setFixedHeight(wh+increment_h);
    ui->textEdit->setFixedWidth(tw+increment_w);
    ui->textEdit->setFixedHeight(th+increment_w);
}

