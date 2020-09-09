#include "embdialog.h"
#include "ui_embdialog.h"

EmbDialog::EmbDialog(EmbFile *emb, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmbDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for (size_t i = 0; i < emb->GetNumFiles(); i++)
    {
        const EmbContainedFile &file = (*emb)[i];

        QPixmap pixmap;
        const uint8_t *dds_buf = file.GetData();
        bool is_dds = false;

        if (dds_buf && memcmp(dds_buf, "DDS", 3) == 0)
        {
            pixmap.loadFromData(dds_buf, (uint)file.GetSize());
            is_dds = true;
        }

        if (emb->HasFileNames())
        {
            ui->listWidget->addItem(Utils::StdStringToQString(file.GetName(), false));
        }
        else
        {
            if (is_dds)
            {
                ui->listWidget->addItem(QString("%1.dds").arg(i));
            }
            else
            {
                ui->listWidget->addItem(QString("%1.bin").arg(i));
            }
        }

        QListWidgetItem *item = ui->listWidget->item((int)i);

        if (is_dds)
            item->setIcon(QIcon(pixmap));

        item->setToolTip(QString("%1").arg(i));

        //item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

EmbDialog::~EmbDialog()
{
    delete ui;
}

void EmbDialog::SetImageSize(int width, int height)
{
    QSize sz;
    sz.setWidth(width); sz.setHeight(height);
    ui->listWidget->setIconSize(sz);
}

void EmbDialog::on_listWidget_itemActivated(QListWidgetItem *item)
{
    result = item->text();
    result_integer = ui->listWidget->currentIndex().row();
    accept();
}

void EmbDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> list = ui->listWidget->selectedItems();

    if (list.size() != 1)
        return;

    result = list[0]->text();
    result_integer = ui->listWidget->currentIndex().row();
}
