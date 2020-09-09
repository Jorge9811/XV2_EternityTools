#include "textentrydialog.h"
#include "ui_textentrydialog.h"

TextEntryDialog::TextEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEntryDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

TextEntryDialog::~TextEntryDialog()
{
    delete ui;
}
