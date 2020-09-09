#include "x2mconvdialog.h"
#include "ui_x2mconvdialog.h"

#include "Xenoverse2.h"
#include "debug.h"

X2mConvDialog::X2mConvDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::X2mConvDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

X2mConvDialog::~X2mConvDialog()
{
    delete ui;
}

QString X2mConvDialog::GetCode() const
{
    return ui->codeEdit->text().toUpper();
}

int X2mConvDialog::GetGuidMode() const
{
    return ui->guidComboBox->currentIndex();
}

bool X2mConvDialog::UseCssVoice() const
{
    return ui->voiceCheck->isChecked();
}

void X2mConvDialog::on_buttonBox_accepted()
{
    QString code = ui->codeEdit->text();

    if (code.isEmpty())
    {
        DPRINTF("Code field is empty!\n");
        return;
    }

    if (code.length() != 3)
    {
        DPRINTF("Code must be a 3-letter code.\n");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        char c = code[i].toLatin1();

        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        {
            // Ok
        }
        else
        {
            DPRINTF("Code can only use capital letters or digits.\n");
            return;
        }

    }

    if (Xenoverse2::IsOriginalChara(Utils::QStringToStdString(code)))
    {
        DPRINTF("This char code can't be used because it is in use by the game.\n");
        return;
    }

    accept();
}

void X2mConvDialog::on_buttonBox_rejected()
{
    reject();
}

void X2mConvDialog::on_codeButton_clicked()
{
    std::string code;

    do
    {
        code.clear();

        while (code.length() != 3)
        {
            char c;

            Utils::GetRandomData(&c, 1);

            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
                code += c;
        }

    } while (Xenoverse2::IsOriginalChara(code) || game_cms->FindEntryByName(code));

    ui->codeEdit->setText(Utils::StdStringToQString(code));
}
