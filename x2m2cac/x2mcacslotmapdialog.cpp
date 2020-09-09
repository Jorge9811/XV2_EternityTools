#include "x2mcacslotmapdialog.h"
#include "ui_x2mcacslotmapdialog.h"

#include <QLabel>

X2mCacSlotMapDialog::X2mCacSlotMapDialog(X2mFile *x2m, std::map<uint32_t, uint32_t> *costume_maps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::X2mCacSlotMapDialog),
    costume_maps(costume_maps)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for (int i = 0; i < 8; i++)
    {
        QLabel *label = findChild<QLabel *>(QString("x2mSlot%1Label").arg(i+1));
        QComboBox *combo = findChild<QComboBox *>(QString("cacSlot%1Combo").arg(i+1));

        if (i < x2m->GetNumSlotEntries())
        {
            const X2mSlotEntry &entry = x2m->GetSlotEntry(i);
            label->setText(Utils::StdStringToQString(entry.costume_name[XV2_LANG_ENGLISH], false));

            combo->addItem("None");
            combo->addItem("Current clothing");

            for (int j = 1; j < 8; j++)
            {
                combo->addItem(QString("Preset %1").arg(j));
            }

            combo->setCurrentIndex(i+1);
        }
        else
        {
            label->setText("Empty");
            label->setDisabled(true);

            combo->setDisabled(true);
        }
    }

    num_slots = (x2m->GetNumSlotEntries() > 8) ? 8 : (int)x2m->GetNumSlotEntries();
}

X2mCacSlotMapDialog::~X2mCacSlotMapDialog()
{
    delete ui;
}

void X2mCacSlotMapDialog::on_buttonBox_accepted()
{
    costume_maps->clear();

    bool used[8];
    memset(used, 0, sizeof(used));

    for (int i = 0; i < num_slots; i++)
    {
        QComboBox *combo = findChild<QComboBox *>(QString("cacSlot%1Combo").arg(i+1));
        int index = combo->currentIndex() - 1;

        if (index >= 0 && index < 8)
        {
            if (!used[index])
            {
                (*costume_maps)[i] = (uint32_t)index;
                used[index] = true;
            }
            else
            {
                DPRINTF("%s is assigned more than once.\n", Utils::QStringToStdString(combo->currentText()).c_str());
                return;
            }
        }
    }

    if (costume_maps->size() == 0)
    {
        DPRINTF("All entries are set to None.");
        return;
    }

    accept();
}
