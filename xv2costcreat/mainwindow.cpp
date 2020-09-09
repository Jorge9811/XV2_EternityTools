#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QClipboard>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listdialog.h"
#include "cssdialog.h"
#include "embdialog.h"
#include "idbeffectdialog.h"

#include "Xenoverse2.h"
#include "xv2ins_common.h"
#include "Config.h"
#include "debug.h"

#define INTERNAL_DATA "Internal package content"
#define GAME_PREFIX "GAME:///"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QLocale::setDefault(QLocale::c());
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (x2m)
        delete x2m;

    delete ui;
}

bool MainWindow::Initialize()
{
    ui->actionOpen->setIcon(ui->mainToolBar->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(ui->mainToolBar->style()->standardIcon(QStyle::SP_DialogSaveButton));

    // Main info tab
    ui->modVersionEdit->setValidator(new QDoubleValidator(this));
    // Hum tab
    ui->humCopyButton->addAction(ui->actionFromGameHum);
    ui->humCopyButton->addAction(ui->actionFromExternalHum);
    // Huf tab
    ui->hufCopyButton->addAction(ui->actionFromGameHuf);
    ui->hufCopyButton->addAction(ui->actionFromExternalHuf);
    // Nmc tab
    ui->nmcCopyButton->addAction(ui->actionFromGameNmc);
    ui->nmcCopyButton->addAction(ui->actionFromExternalNmc);
    // Fri tab
    ui->friCopyButton->addAction(ui->actionFromGameFri);
    ui->friCopyButton->addAction(ui->actionFromExternalFri);
    // Mam tab
    ui->mamCopyButton->addAction(ui->actionFromGameMam);
    ui->mamCopyButton->addAction(ui->actionFromExternalMam);
    // Maf tab
    ui->mafCopyButton->addAction(ui->actionFromGameMaf);
    ui->mafCopyButton->addAction(ui->actionFromExternalMaf);
    // Idb tab
    ui->idbStarsEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0AEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0CEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0EEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbBuyEdit->setValidator(new QIntValidator(this));
    ui->idbSellEdit->setValidator(new QIntValidator(this));
    ui->idbTpEdit->setValidator(new QIntValidator(this));
    //ui->idbModelEdit->setValidator(new QIntValidator(this));
    ui->idbU24Edit->setValidator(new QIntValidator(this));
    ui->idbU28Edit->setValidator(new QIntValidator(this));
    ui->idbU2CEdit->setValidator(new QIntValidator(this));
    ui->idbCopyButton->addAction(ui->actionFromGameCostumeTop);
    ui->idbCopyButton->addAction(ui->actionFromGameCostumeBottom);
    ui->idbCopyButton->addAction(ui->actionFromGameCostumeGloves);
    ui->idbCopyButton->addAction(ui->actionFromGameCostumeShoes);
    ui->idbCopyButton->addAction(ui->actionFromGameAccessory);
    ui->idbNameCopyButton->addAction(ui->actionFromGameCostumeTopName);
    ui->idbNameCopyButton->addAction(ui->actionFromGameCostumeBottomName);
    ui->idbNameCopyButton->addAction(ui->actionFromGameCostumeGlovesName);
    ui->idbNameCopyButton->addAction(ui->actionFromGameCostumeShoesName);
    ui->idbNameCopyButton->addAction(ui->actionFromGameAccessoryName);
    ui->idbDescCopyButton->addAction(ui->actionFromGameCostumeTopDesc);
    ui->idbDescCopyButton->addAction(ui->actionFromGameCostumeBottomDesc);
    ui->idbDescCopyButton->addAction(ui->actionFromGameCostumeGlovesDesc);
    ui->idbDescCopyButton->addAction(ui->actionFromGameCostumeShoesDesc);
    ui->idbDescCopyButton->addAction(ui->actionFromGameAccessoryDesc);

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    Bootstrap(false, false);

    x2m = new X2mFile();
    x2m->SetType(X2mType::NEW_COSTUME);

    // Handle arguments
    if (qApp->arguments().size() == 2)
    {
        QString file = qApp->arguments()[1];

        config.lf_editor_open = file;
        X2mFile *new_x2m = new X2mFile();

        if (new_x2m->LoadFromFile(Utils::QStringToStdString(file)))
        {
            delete x2m;
            x2m = new_x2m;
            config.lf_editor_save = file;
        }
        else
        {
            delete new_x2m;
        }
    }

    if (x2m->GetType() != X2mType::NEW_COSTUME)
    {
        DPRINTF("This kind of x2m is not supported by this editor.\n");
        return false;
    }

    ProcessX2m();
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open file", config.lf_editor_open, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_editor_open = file;

    X2mFile *new_x2m = new X2mFile();

    if (!new_x2m->LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Load file failed.\n");
        delete new_x2m;
        return;
    }

    config.lf_editor_save = file;

    if (new_x2m->GetType() != X2mType::NEW_COSTUME)
    {
        DPRINTF("That x2m is not of new costume type!\n");
        delete new_x2m;
        return;
    }

    delete x2m;
    x2m = new_x2m;
    ProcessX2m();
}

void MainWindow::on_actionSave_triggered()
{
    if (!Validate())
        return;

    QString file = QFileDialog::getSaveFileName(this, "Save file", config.lf_editor_save, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_editor_save = file;

    if (!Build())
    {
        DPRINTF("Build failed.\n");
        return;
    }

    if (!x2m->SaveToFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("x2m save failed.\n");
        return;
    }

    UPRINTF("File has been succesfully written.\n");
}

void MainWindow::on_actionExit_triggered()
{
    if (ProcessShutdown())
        qApp->exit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME_COSTCREAT);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME_COSTCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
   //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME_COSTCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));

    box.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ProcessShutdown())
        event->accept();
    else
        event->ignore();
}

bool MainWindow::ProcessShutdown()
{
    config.Save();
    return true;
}

int MainWindow::GetMaxNumPartSets()
{
    int max = 0;

    if (hum_bcs.size() > max)
        max = hum_bcs.size();

    if (huf_bcs.size() > max)
        max = huf_bcs.size();

    if (nmc_bcs.size() > max)
        max = nmc_bcs.size();

    if (fri_bcs.size() > max)
        max = fri_bcs.size();

    if (mam_bcs.size() > max)
        max = mam_bcs.size();

    if (maf_bcs.size() > max)
        max = maf_bcs.size();

    return max;
}

bool MainWindow::CheckDirVisitor(const std::string &path, bool, void *custom_param)
{
    MainWindow *pthis = (MainWindow *)custom_param;

    const std::string name = Utils::GetFileNameString(path);
    int num = pthis->GetMaxNumPartSets();

    if (num == 0) // This shouldn't happen at this point
        return true;

    bool add_it = false;

    num += 10000;
    for (int i = 10000; i < num; i++)
    {
        if (name.find(Utils::ToString(i)) != std::string::npos)
        {
            add_it = true;
            break;
        }
    }

    if (!add_it)
    {
        QString text = "The file \"" + Utils::StdStringToQString(path) + "\" doesn't follow the naming rules and won't be included.\n"
                                                                         "\nDo You want to continue with the build?\n";

        if (QMessageBox::question(pthis, "Continue?", text, QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No), QMessageBox::No)
            != QMessageBox::Yes)
        {
            return false;
        }
    }

    return true;
}

bool MainWindow::CheckDirectoryContent(const std::string &dir)
{
    return Utils::VisitDirectory(dir, true, false, false, CheckDirVisitor, this, true);
}

static uint8_t temp_race;

bool MainWindow::AddDirVisitor(const std::string &path, bool, void *custom_param)
{
    MainWindow *pthis = (MainWindow *)custom_param;

    const std::string name = Utils::GetFileNameString(path);
    int num = pthis->GetMaxNumPartSets();

    if (num == 0) // This shouldn't happen at this point
        return true;

    bool add_it = false;

    num += 10000;
    for (int i = 10000; i < num; i++)
    {
        if (name.find(Utils::ToString(i)) != std::string::npos)
        {
            add_it = true;
            break;
        }
    }

    if (add_it)
    {
        if (!pthis->x2m->AddExternalFile(path, x2m_cr_code[temp_race] + "/" + name))
            return false;
    }

    return true;
}

bool MainWindow::AddDirectoryContent(const std::string &dir, uint8_t race)
{
    temp_race = race;
    return Utils::VisitDirectory(dir, true, false, false, AddDirVisitor, this, true);
}

void MainWindow::ProcessX2m()
{
    // Info tab
    ui->modNameEdit->setText(Utils::StdStringToQString(x2m->GetModName(), false));
    ui->modAuthorEdit->setText(Utils::StdStringToQString(x2m->GetModAuthor(), false));
    ui->modVersionEdit->setText(QString::number(x2m->GetModVersion()));
    ui->modGuidEdit->setText(Utils::StdStringToQString(x2m->GetModGuid()));

    // Files tab
    if (x2m->CostumeDirectoryExists(X2M_CR_HUM_SYM))
    {
        ui->humEdit->setText(INTERNAL_DATA);
        ui->humEdit->setEnabled(false);
    }
    else
    {
        ui->humEdit->setText("");
        ui->humEdit->setEnabled(true);
    }

    if (x2m->CostumeDirectoryExists(X2M_CR_HUF_SYF))
    {
        ui->hufEdit->setText(INTERNAL_DATA);
        ui->hufEdit->setEnabled(false);
    }
    else
    {
        ui->hufEdit->setText("");
        ui->hufEdit->setEnabled(true);
    }

    if (x2m->CostumeDirectoryExists(X2M_CR_NMC))
    {
        ui->nmcEdit->setText(INTERNAL_DATA);
        ui->nmcEdit->setEnabled(false);
    }
    else
    {
        ui->nmcEdit->setText("");
        ui->nmcEdit->setEnabled(true);
    }

    if (x2m->CostumeDirectoryExists(X2M_CR_FRI))
    {
        ui->friEdit->setText(INTERNAL_DATA);
        ui->friEdit->setEnabled(false);
    }
    else
    {
        ui->friEdit->setText("");
        ui->friEdit->setEnabled(true);
    }

    if (x2m->CostumeDirectoryExists(X2M_CR_MAM))
    {
        ui->mamEdit->setText(INTERNAL_DATA);
        ui->mamEdit->setEnabled(false);
    }
    else
    {
        ui->mamEdit->setText("");
        ui->mamEdit->setEnabled(true);
    }

    if (x2m->CostumeDirectoryExists(X2M_CR_MAF))
    {
        ui->mafEdit->setText(INTERNAL_DATA);
        ui->mafEdit->setEnabled(false);
    }
    else
    {
        ui->mafEdit->setText("");
        ui->mafEdit->setEnabled(true);
    }

    if (x2m->JungleExists())
    {
        ui->addDataEdit->setText(INTERNAL_DATA);
        ui->addDataEdit->setEnabled(false);
    }
    else
    {
        ui->addDataEdit->setText("");
        ui->addDataEdit->setEnabled(true);
    }

    // Hum
    int num_hum = (int)x2m->GetNumCostumePartSets(X2M_CR_HUM_SYM);
    if (num_hum > 0)
    {
        hum_bcs.resize(num_hum);

        for (int i = 0; i < num_hum; i++)
        {
            hum_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_HUM_SYM, i).bcs);
        }

        ui->humEnableCheck->setChecked(true);
    }
    else
    {
        ui->humEnableCheck->setChecked(false);
    }

    ui->humBcsEdit->clear();
    on_humEnableCheck_clicked();

    // Huf
    int num_huf = (int)x2m->GetNumCostumePartSets(X2M_CR_HUF_SYF);
    if (num_huf > 0)
    {
        huf_bcs.resize(num_huf);

        for (int i = 0; i < num_huf; i++)
        {
            huf_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_HUF_SYF, i).bcs);
        }

        ui->hufEnableCheck->setChecked(true);
    }
    else
    {
        ui->hufEnableCheck->setChecked(false);
    }

    ui->hufBcsEdit->clear();
    on_hufEnableCheck_clicked();

    // Nmc
    int num_nmc = (int)x2m->GetNumCostumePartSets(X2M_CR_NMC);
    if (num_nmc > 0)
    {
        nmc_bcs.resize(num_nmc);

        for (int i = 0; i < num_nmc; i++)
        {
            nmc_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_NMC, i).bcs);
        }

        ui->nmcEnableCheck->setChecked(true);
    }
    else
    {
        ui->nmcEnableCheck->setChecked(false);
    }

    ui->nmcBcsEdit->clear();
    on_nmcEnableCheck_clicked();

    // Fri
    int num_fri = (int)x2m->GetNumCostumePartSets(X2M_CR_FRI);
    if (num_fri > 0)
    {
        fri_bcs.resize(num_fri);

        for (int i = 0; i < num_fri; i++)
        {
            fri_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_FRI, i).bcs);
        }

        ui->friEnableCheck->setChecked(true);
    }
    else
    {
        ui->friEnableCheck->setChecked(false);
    }

    ui->friBcsEdit->clear();
    on_friEnableCheck_clicked();

    // Mam
    int num_mam = (int)x2m->GetNumCostumePartSets(X2M_CR_MAM);
    if (num_mam > 0)
    {
        mam_bcs.resize(num_mam);

        for (int i = 0; i < num_mam; i++)
        {
            mam_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_MAM, i).bcs);
        }

        ui->mamEnableCheck->setChecked(true);
    }
    else
    {
        ui->mamEnableCheck->setChecked(false);
    }

    ui->mamBcsEdit->clear();
    on_mamEnableCheck_clicked();

    // Maf
    int num_maf = (int)x2m->GetNumCostumePartSets(X2M_CR_MAF);
    if (num_maf > 0)
    {
        maf_bcs.resize(num_maf);

        for (int i = 0; i < num_maf; i++)
        {
            maf_bcs[i] = BcsToString(x2m->GetCostumePartSet(X2M_CR_MAF, i).bcs);
        }

        ui->mafEnableCheck->setChecked(true);
    }
    else
    {
        ui->mafEnableCheck->setChecked(false);
    }

    ui->mafBcsEdit->clear();
    on_mafEnableCheck_clicked();

    // Idb tab
    if (x2m->HasCostumeItem())
    {
        ui->idbEnableCheck->setChecked(true);
    }
    else
    {
        ui->idbEnableCheck->setChecked(false);
    }

    // Put dummy entry to gui
    X2mItem item;
    ItemToGui(item);

    on_idbEnableCheck_clicked();

    // Update to new format
    x2m->SetFormatVersion(x2m->X2M_CURRENT_VERSION);
}

bool MainWindow::Validate()
{
    // Info tab
    if (ui->modNameEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod name cannot be empty.\n");
        return false;
    }

    if (ui->modAuthorEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod author cannot be empty.\n");
        return false;
    }

    if (ui->modVersionEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod version cannot be empty.\n");
        return false;
    }

    // Bcs tabs (must be before files)
    int num_sets = GetMaxNumPartSets();

    if (num_sets == 0)
    {
        DPRINTF("At least one of the races tabs must be enabled.\n");
        return false;
    }

    if (hum_bcs.size() != 0 && hum_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (huf_bcs.size() != 0 && huf_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (nmc_bcs.size() != 0 && nmc_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (fri_bcs.size() != 0 && fri_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (mam_bcs.size() != 0 && mam_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (maf_bcs.size() != 0 && maf_bcs.size() != num_sets)
    {
        DPRINTF("All enabled races must have similar number of partsets.\n");
        return false;
    }

    if (ui->humEnableCheck->isChecked())
    {
        int idx = ui->humComboBox->currentIndex();
        if (idx < 0 || idx >= hum_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in hum.\n");
            return false;
        }

        hum_bcs[idx] = ui->humBcsEdit->toPlainText();

        for (int i = 0; i < hum_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(hum_bcs[i], dummy, "On HUM/SYM PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    if (ui->hufEnableCheck->isChecked())
    {
        int idx = ui->hufComboBox->currentIndex();
        if (idx < 0 || idx >= huf_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in huf.\n");
            return false;
        }

        huf_bcs[idx] = ui->hufBcsEdit->toPlainText();

        for (int i = 0; i < huf_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(huf_bcs[i], dummy, "On HUF/SYF PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    if (ui->nmcEnableCheck->isChecked())
    {
        int idx = ui->nmcComboBox->currentIndex();
        if (idx < 0 || idx >= nmc_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in nmc.\n");
            return false;
        }

        nmc_bcs[idx] = ui->nmcBcsEdit->toPlainText();

        for (int i = 0; i < nmc_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(nmc_bcs[i], dummy, "On NMC PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    if (ui->friEnableCheck->isChecked())
    {
        int idx = ui->friComboBox->currentIndex();
        if (idx < 0 || idx >= fri_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in fri.\n");
            return false;
        }

        fri_bcs[idx] = ui->friBcsEdit->toPlainText();

        for (int i = 0; i < fri_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(fri_bcs[i], dummy, "On FRI PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    if (ui->mamEnableCheck->isChecked())
    {
        int idx = ui->mamComboBox->currentIndex();
        if (idx < 0 || idx >= mam_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in mam.\n");
            return false;
        }

        mam_bcs[idx] = ui->mamBcsEdit->toPlainText();

        for (int i = 0; i < mam_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(mam_bcs[i], dummy, "On MAM PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    if (ui->mafEnableCheck->isChecked())
    {
        int idx = ui->mafComboBox->currentIndex();
        if (idx < 0 || idx >= maf_bcs.size())
        {
            DPRINTF("Internal error: ComboBox out of bounds in maf.\n");
            return false;
        }

        maf_bcs[idx] = ui->mafBcsEdit->toPlainText();

        for (int i = 0; i < maf_bcs.size(); i++)
        {
            BcsPartSet dummy;

            if (!StringToBcs(maf_bcs[i], dummy, "On MAF PartSet " + Utils::ToString(i)))
                return false;
        }
    }

    // Files tab
    if (ui->humEdit->isEnabled())
    {
        QString hum = ui->humEdit->text().trimmed();

        if (!hum.isEmpty())
        {
            std::string hum_std = Utils::QStringToStdString(hum);

            if (!Utils::IsEmptyString(hum_std))
            {
                if (!Utils::DirExists(hum_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", hum_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(hum_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", hum_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(hum_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->hufEdit->isEnabled())
    {
        QString huf = ui->hufEdit->text().trimmed();

        if (!huf.isEmpty())
        {
            std::string huf_std = Utils::QStringToStdString(huf);

            if (!Utils::IsEmptyString(huf_std))
            {
                if (!Utils::DirExists(huf_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", huf_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(huf_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", huf_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(huf_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->nmcEdit->isEnabled())
    {
        QString nmc = ui->nmcEdit->text().trimmed();

        if (!nmc.isEmpty())
        {
            std::string nmc_std = Utils::QStringToStdString(nmc);

            if (!Utils::IsEmptyString(nmc_std))
            {
                if (!Utils::DirExists(nmc_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", nmc_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(nmc_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", nmc_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(nmc_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->friEdit->isEnabled())
    {
        QString fri = ui->friEdit->text().trimmed();

        if (!fri.isEmpty())
        {
            std::string fri_std = Utils::QStringToStdString(fri);

            if (!Utils::IsEmptyString(fri_std))
            {
                if (!Utils::DirExists(fri_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", fri_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(fri_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", fri_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(fri_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->mamEdit->isEnabled())
    {
        QString mam = ui->mamEdit->text().trimmed();

        if (!mam.isEmpty())
        {
            std::string mam_std = Utils::QStringToStdString(mam);

            if (!Utils::IsEmptyString(mam_std))
            {
                if (!Utils::DirExists(mam_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", mam_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(mam_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", mam_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(mam_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->mafEdit->isEnabled())
    {
        QString maf = ui->mafEdit->text().trimmed();

        if (!maf.isEmpty())
        {
            std::string maf_std = Utils::QStringToStdString(maf);

            if (!Utils::IsEmptyString(maf_std))
            {
                if (!Utils::DirExists(maf_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", maf_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(maf_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", maf_std.c_str());
                    return false;
                }
                else if (!CheckDirectoryContent(maf_std))
                {
                    return false;
                }
            }
        }
    }

    if (ui->addDataEdit->isEnabled())
    {
        QString add_data = ui->addDataEdit->text().trimmed();

        if (!add_data.isEmpty())
        {
            std::string add_data_std = Utils::QStringToStdString(add_data);

            if (!Utils::IsEmptyString(add_data_std))
            {
                if (!Utils::DirExists(add_data_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", add_data_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(add_data_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", add_data_std.c_str());
                    return false;
                }
            }
        }
    }

    // IDB tab
    if (ui->idbEnableCheck->isChecked())
    {
        for (size_t i = 0; i < x2m->GetNumCostumeItems(); i++)
        {
            const X2mItem &item = x2m->GetCostumeItem(i);

            if (Utils::IsEmptyString(item.item_name[XV2_LANG_ENGLISH]))
            {
                DPRINTF("[IDB] Name cannot be empty for english language (on Entry %Id)\n", i);
                return false;
            }

            if (Utils::IsEmptyString(item.item_desc[XV2_LANG_ENGLISH]))
            {
                DPRINTF("[IDB] Description cannot be empty for english language (on Entry %Id)\n", i);
                return false;
            }
        }

        if (ui->idbStarsEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Stars cannot be empty.\n");
            return false;
        }

        if (ui->idbU0AEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0A cannot be empty.\n");
            return false;
        }

        if (ui->idbU0CEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0C cannot be empty.\n");
            return false;
        }

        if (ui->idbU0EEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0E cannot be empty.\n");
            return false;
        }

        if (ui->idbBuyEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Buy cannot be empty.\n");
            return false;
        }

        if (ui->idbSellEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Sell cannot be empty.\n");
            return false;
        }

        if (ui->idbTpEdit->text().isEmpty())
        {
            DPRINTF("[IDB] TP cannot be empty.\n");
            return false;
        }

        if (ui->idbU24Edit->text().isEmpty())
        {
            DPRINTF("[IDB] U_24 cannot be empty.\n");
            return false;
        }

        if (ui->idbU28Edit->text().isEmpty())
        {
            DPRINTF("[IDB] U_28 cannot be empty.\n");
            return false;
        }

        if (ui->idbU2CEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_2C cannot be empty.\n");
            return false;
        }
    }

    return true;
}

bool MainWindow::Build()
{
    // Info
    x2m->SetModName(Utils::QStringToStdString(ui->modNameEdit->text(), false));
    x2m->SetModAuthor(Utils::QStringToStdString(ui->modAuthorEdit->text(), false));
    x2m->SetModVersion(ui->modVersionEdit->text().toFloat());

    // Files
    QString hum = ui->humEdit->text().trimmed();

    if (ui->humEdit->isEnabled())
    {
        std::string hum_std = Utils::QStringToStdString(hum);

        x2m->DeleteCostumeDirectory(X2M_CR_HUM_SYM);

        if (!Utils::IsEmptyString(hum_std) && !AddDirectoryContent(hum_std, X2M_CR_HUM_SYM))
        {
            DPRINTF("AddExternalDirectory failed on HUM/SYM.\n");
            return false;
        }
    }
    else
    {
        if (hum == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (1).\n", FUNCNAME);
            return false;
        }
    }


    QString huf = ui->hufEdit->text().trimmed();

    if (ui->hufEdit->isEnabled())
    {
        std::string huf_std = Utils::QStringToStdString(huf);

        x2m->DeleteCostumeDirectory(X2M_CR_HUF_SYF);

        if (!Utils::IsEmptyString(huf_std) && !AddDirectoryContent(huf_std, X2M_CR_HUF_SYF))
        {
            DPRINTF("AddExternalDirectory failed on HUF/SYF.\n");
            return false;
        }
    }
    else
    {
        if (huf == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (2).\n", FUNCNAME);
            return false;
        }
    }

    QString nmc = ui->nmcEdit->text().trimmed();

    if (ui->nmcEdit->isEnabled())
    {
        std::string nmc_std = Utils::QStringToStdString(nmc);

        x2m->DeleteCostumeDirectory(X2M_CR_NMC);

        if (!Utils::IsEmptyString(nmc_std) && !AddDirectoryContent(nmc_std, X2M_CR_NMC))
        {
            DPRINTF("AddExternalDirectory failed on NMC.\n");
            return false;
        }
    }
    else
    {
        if (nmc == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (3).\n", FUNCNAME);
            return false;
        }
    }

    QString fri = ui->friEdit->text().trimmed();

    if (ui->friEdit->isEnabled())
    {
        std::string fri_std = Utils::QStringToStdString(fri);

        x2m->DeleteCostumeDirectory(X2M_CR_FRI);

        if (!Utils::IsEmptyString(fri_std) && !AddDirectoryContent(fri_std, X2M_CR_FRI))
        {
            DPRINTF("AddExternalDirectory failed on FRI.\n");
            return false;
        }
    }
    else
    {
        if (fri == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (4).\n", FUNCNAME);
            return false;
        }
    }

    QString mam = ui->mamEdit->text().trimmed();

    if (ui->mamEdit->isEnabled())
    {
        std::string mam_std = Utils::QStringToStdString(mam);

        x2m->DeleteCostumeDirectory(X2M_CR_MAM);

        if (!Utils::IsEmptyString(mam_std) && !AddDirectoryContent(mam_std, X2M_CR_MAM))
        {
            DPRINTF("AddExternalDirectory failed on MAM.\n");
            return false;
        }
    }
    else
    {
        if (mam == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (5).\n", FUNCNAME);
            return false;
        }
    }

    QString maf = ui->mafEdit->text().trimmed();

    if (ui->mafEdit->isEnabled())
    {
        std::string maf_std = Utils::QStringToStdString(maf);

        x2m->DeleteCostumeDirectory(X2M_CR_MAF);

        if (!Utils::IsEmptyString(maf_std) && !AddDirectoryContent(maf_std, X2M_CR_MAF))
        {
            DPRINTF("AddExternalDirectory failed on MAF.\n");
            return false;
        }
    }
    else
    {
        if (maf == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (5).\n", FUNCNAME);
            return false;
        }
    }

    QString add_data = ui->addDataEdit->text().trimmed();

    if (ui->addDataEdit->isEnabled())
    {
        std::string add_data_std = Utils::QStringToStdString(add_data);

        x2m->DeleteJungle();

        if (!Utils::IsEmptyString(add_data_std) && !x2m->AddExternalDirectory(add_data_std, X2M_JUNGLE))
        {
            DPRINTF("AddExternalDirectory failed on additional data.\n");
            return false;
        }
    }
    else
    {
        if (add_data == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (6).\n", FUNCNAME);
            return false;
        }
    }

    // Bcs tabs    
    x2m->ClearAllCostumePartSets();

    if (ui->humEnableCheck->isChecked())
    {
        for (int i = 0; i < hum_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_HUM_SYM;

            if (!StringToBcs(hum_bcs[i], set.bcs, "On HUM/SYM bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        hum_bcs.clear();        
    }

    if (ui->hufEnableCheck->isChecked())
    {
        for (int i = 0; i < huf_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_HUF_SYF;

            if (!StringToBcs(huf_bcs[i], set.bcs, "On HUF/SYF bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        huf_bcs.clear();        
    }

    if (ui->nmcEnableCheck->isChecked())
    {
        for (int i = 0; i < nmc_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_NMC;

            if (!StringToBcs(nmc_bcs[i], set.bcs, "On NMC bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        nmc_bcs.clear();       
    }

    if (ui->friEnableCheck->isChecked())
    {
        for (int i = 0; i < fri_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_FRI;

            if (!StringToBcs(fri_bcs[i], set.bcs, "On FRI bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        fri_bcs.clear();        
    }

    if (ui->mamEnableCheck->isChecked())
    {
        for (int i = 0; i < mam_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_MAM;

            if (!StringToBcs(mam_bcs[i], set.bcs, "On MAM bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        mam_bcs.clear();        
    }

    if (ui->mafEnableCheck->isChecked())
    {
        for (int i = 0; i < maf_bcs.size(); i++)
        {
            X2mPartSet set;

            set.race = X2M_CR_MAF;

            if (!StringToBcs(maf_bcs[i], set.bcs, "On MAF bcs"))
                return false;

            x2m->AddCostumePartSet(set);
        }
    }
    else
    {
        maf_bcs.clear();        
    }

    // IDB
    if (ui->idbEnableCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int idb_idx = ui->idbComboBox->currentIndex();
        if (idb_idx < 0 || idb_idx >= x2m->GetNumCostumeItems())
        {
            DPRINTF("%s: idb entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        X2mItem &item = x2m->GetCostumeItem(idb_idx);
        GuiToItem(item);
    }
    else
    {
        while (x2m->GetNumCostumeItems() != 0)
            x2m->RemoveCostumeItem(0);
    }

    return true;
}

void MainWindow::on_guidButton_clicked()
{
    uint8_t guid[16];
    std::string guid_str;

    Utils::GetRandomData(guid, sizeof(guid));
    guid_str = Utils::GUID2String(guid);

    x2m->SetModGuid(guid_str);
    ui->modGuidEdit->setText(Utils::StdStringToQString(guid_str));
}

void MainWindow::on_humButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select HUM directory", config.ld_cost_hum);

    if (dir.isNull())
        return;

    config.ld_cost_hum = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_HUM_SYM);

    ui->humEdit->setText(dir);
    ui->humEdit->setEnabled(true);
}

void MainWindow::on_hufButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select HUF directory", config.ld_cost_huf);

    if (dir.isNull())
        return;

    config.ld_cost_huf = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_HUF_SYF);

    ui->hufEdit->setText(dir);
    ui->hufEdit->setEnabled(true);
}

void MainWindow::on_nmcButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select NMC directory", config.ld_cost_nmc);

    if (dir.isNull())
        return;

    config.ld_cost_nmc = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_NMC);

    ui->nmcEdit->setText(dir);
    ui->nmcEdit->setEnabled(true);
}

void MainWindow::on_friButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select FRI directory", config.ld_cost_fri);

    if (dir.isNull())
        return;

    config.ld_cost_fri = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_FRI);

    ui->friEdit->setText(dir);
    ui->friEdit->setEnabled(true);
}

void MainWindow::on_mamButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select MAM directory", config.ld_cost_mam);

    if (dir.isNull())
        return;

    config.ld_cost_mam = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_MAM);

    ui->mamEdit->setText(dir);
    ui->mamEdit->setEnabled(true);
}

void MainWindow::on_mafButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select MAF directory", config.ld_cost_maf);

    if (dir.isNull())
        return;

    config.ld_cost_maf = dir;
    x2m->DeleteCostumeDirectory(X2M_CR_MAF);

    ui->mafEdit->setText(dir);
    ui->mafEdit->setEnabled(true);
}

void MainWindow::on_addEditButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose directory that conteins data directory", config.ld_add_data);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::NormalizePath(Utils::QStringToStdString(dir));

    if (dir_std.length() != 0 && !Utils::EndsWith(dir_std, "/"))
        dir_std += '/';

    if (!Utils::DirExists(dir_std + "data"))
    {
        if (QMessageBox::question(this, "Use that directory?",
                                  "That directory doesn't contain a directory called \"data\" inside. "
                                  "Are you sure that it is the correct directory?",
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                  QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }
    else if (Utils::DirExists(dir_std + "data/system"))
    {
        if (QMessageBox::question(this, "Use that directory?",
                                  "That directory contains a system directory that will affect the whole game, "
                                  "what defeats the philosophy of a new character mod.\n\n"
                                  "Are you sure you want to use that directory?",
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                  QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }

    config.ld_add_data = dir;
    x2m->DeleteJungle();

    ui->addDataEdit->setText(dir);
    ui->addDataEdit->setEnabled(true);
}

void MainWindow::UpdateIdbPartset()
{
    int count_idb = ui->idbPartSetComboBox->count();
    int count_set = GetMaxNumPartSets();

    if (count_idb == count_set)
        return;

    if (count_idb < count_set)
    {
        for (int i = count_idb; i < count_set; i++)
        {
            ui->idbPartSetComboBox->addItem(QString("%1").arg(i));
        }

        if (count_idb == 0)
        {
            if (ui->idbEnableCheck->isChecked()) // 1.0: added this condition to fix bug
            {
                for (size_t n = 0; n < x2m->GetNumCostumeItems(); n++)
                {
                    x2m->GetCostumeItem(n).idb.model = 0;
                }
            }

            ui->idbPartSetComboBox->setCurrentIndex(0);
        }
    }
    else
    {
        for (int i = count_set; i < count_idb; i++)
        {
            ui->idbPartSetComboBox->removeItem(ui->idbPartSetComboBox->count()-1);
        }

        for (size_t n = 0; n < x2m->GetNumCostumeItems(); n++)
        {
            X2mItem &item = x2m->GetCostumeItem(n);

            if (item.idb.model >= (uint)count_set)
            {
                item.idb.model = (uint32_t) (count_set-1);
            }
        }

        if (ui->idbPartSetComboBox->currentIndex() >= count_set)
            ui->idbPartSetComboBox->setCurrentIndex(count_set-1);
    }
}

QString MainWindow::BcsToString(const BcsPartSet &bcs)
{
    TiXmlElement *bcs_root = bcs.Decompile(nullptr, -1);

    if (!bcs_root)
    {
        DPRINTF("%s: Internal error, cannot continue.\n", FUNCNAME);
        qApp->exit();
        return QString();
    }

    TiXmlPrinter printer;
    bcs_root->Accept(&printer);

    const char *xml_buf = printer.CStr();
    return QString(xml_buf);
}

bool MainWindow::StringToBcs(const QString &str, BcsPartSet &bcs, const std::string &error_prefix)
{
    TiXmlDocument doc;

    if (str.trimmed().isEmpty())
    {
        DPRINTF("%s: bcs text is empty!\n", error_prefix.c_str());
        return false;
    }

    doc.Parse(Utils::QStringToStdString(str, false).c_str());

    if (doc.ErrorId() != 0)
    {
        DPRINTF("%s: Xml parse error on line %d, col %d\n"
                "Tinyxml error id/description: %d - %s\n",
                error_prefix.c_str(), doc.ErrorRow(), doc.ErrorCol(),
                 doc.ErrorId(), doc.ErrorDesc());

        return false;
    }

    TiXmlHandle handle(&doc);
    TiXmlElement *root = Utils::FindRoot(&handle, "PartSet");
    if (!root)
    {
        DPRINTF("%s: cannot find <PartSet>\n", error_prefix.c_str());
        return false;
    }

    if (!bcs.Compile(root))
    {
        DPRINTF("%s: bcs compilation error.\n", error_prefix.c_str());
        return false;
    }

    if (bcs.valid && bcs.parts.size() != 10)
    {
        DPRINTF("%s: <PartSet> must have 10 <Parts> or none.\n", error_prefix.c_str());
        return false;
    }

    return true;
}

void MainWindow::on_humEnableCheck_clicked()
{
     bool checked = ui->humEnableCheck->isChecked();

     ui->humComboBox->setEnabled(checked);
     ui->humAddButton->setEnabled(checked);
     ui->humRemoveButton->setEnabled(checked);
     ui->humCopyButton->setEnabled(checked);
     ui->humBcsEdit->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_hum_index = -1;
        ui->humComboBox->clear();

        int num_hum_entries = hum_bcs.size();

        if (num_hum_entries == 0)
        {
            hum_bcs.push_back("<PartSet>\n</PartSet>");
            num_hum_entries++;
        }

        for (int i = 0; i < num_hum_entries; i++)
        {
            ui->humComboBox->addItem(QString("PartSet %1").arg(i));
        }

        if (num_hum_entries == 1)
        {
            ui->humRemoveButton->setDisabled(true);
        }
    }
    else
    {
        hum_bcs.clear();
    }

    UpdateIdbPartset();
}

void MainWindow::on_humComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= hum_bcs.size())
        return;

    if (prev_hum_index >= 0 && prev_hum_index < hum_bcs.size())
    {
        hum_bcs[prev_hum_index] = ui->humBcsEdit->toPlainText();
    }

    ui->humBcsEdit->setText(hum_bcs[index]);
    prev_hum_index = index;
}

void MainWindow::on_humAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->humComboBox->currentIndex();
    if (idx >= 0 && idx < hum_bcs.size())
    {
        partset = ui->humBcsEdit->toPlainText();
    }

    int pos = hum_bcs.size();
    hum_bcs.push_back(partset);

    ui->humComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->humComboBox->setCurrentIndex(pos);

    if (hum_bcs.size() > 1)
        ui->humRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_humRemoveButton_clicked()
{
    int index = ui->humComboBox->currentIndex();

    if (index < 0 || index >= hum_bcs.size())
        return;

    hum_bcs.remove(index);

    if (prev_hum_index > index)
        prev_hum_index--;
    else
        prev_hum_index = -1;

    ui->humComboBox->removeItem(index);

    for (int i = 0; i < ui->humComboBox->count(); i++)
    {
        ui->humComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (hum_bcs.size() == 1)
        ui->humRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_hufEnableCheck_clicked()
{
    bool checked = ui->hufEnableCheck->isChecked();

    ui->hufComboBox->setEnabled(checked);
    ui->hufAddButton->setEnabled(checked);
    ui->hufRemoveButton->setEnabled(checked);
    ui->hufCopyButton->setEnabled(checked);
    ui->hufBcsEdit->setEnabled(checked);

   if (checked)
   {
       // Clear combobox first
       prev_huf_index = -1;
       ui->hufComboBox->clear();

       int num_huf_entries = huf_bcs.size();

       if (num_huf_entries == 0)
       {
           huf_bcs.push_back("<PartSet>\n</PartSet>");
           num_huf_entries++;
       }

       for (int i = 0; i < num_huf_entries; i++)
       {
           ui->hufComboBox->addItem(QString("PartSet %1").arg(i));
       }

       if (num_huf_entries == 1)
       {
           ui->hufRemoveButton->setDisabled(true);
       }
   }
   else
   {
       huf_bcs.clear();
   }

   UpdateIdbPartset();
}

void MainWindow::on_hufComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= huf_bcs.size())
        return;

    if (prev_huf_index >= 0 && prev_huf_index < huf_bcs.size())
    {
        huf_bcs[prev_huf_index] = ui->hufBcsEdit->toPlainText();
    }

    ui->hufBcsEdit->setText(huf_bcs[index]);
    prev_huf_index = index;
}

void MainWindow::on_hufAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->hufComboBox->currentIndex();
    if (idx >= 0 && idx < huf_bcs.size())
    {
        partset = ui->hufBcsEdit->toPlainText();
    }

    int pos = huf_bcs.size();
    huf_bcs.push_back(partset);

    ui->hufComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->hufComboBox->setCurrentIndex(pos);

    if (huf_bcs.size() > 1)
        ui->hufRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_hufRemoveButton_clicked()
{
    int index = ui->hufComboBox->currentIndex();

    if (index < 0 || index >= huf_bcs.size())
        return;

    huf_bcs.remove(index);

    if (prev_huf_index > index)
        prev_huf_index--;
    else
        prev_huf_index = -1;

    ui->hufComboBox->removeItem(index);

    for (int i = 0; i < ui->hufComboBox->count(); i++)
    {
        ui->hufComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (huf_bcs.size() == 1)
        ui->hufRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_nmcEnableCheck_clicked()
{
    bool checked = ui->nmcEnableCheck->isChecked();

    ui->nmcComboBox->setEnabled(checked);
    ui->nmcAddButton->setEnabled(checked);
    ui->nmcRemoveButton->setEnabled(checked);
    ui->nmcCopyButton->setEnabled(checked);
    ui->nmcBcsEdit->setEnabled(checked);

   if (checked)
   {
       // Clear combobox first
       prev_nmc_index = -1;
       ui->nmcComboBox->clear();

       int num_nmc_entries = nmc_bcs.size();

       if (num_nmc_entries == 0)
       {
           nmc_bcs.push_back("<PartSet>\n</PartSet>");
           num_nmc_entries++;
       }

       for (int i = 0; i < num_nmc_entries; i++)
       {
           ui->nmcComboBox->addItem(QString("PartSet %1").arg(i));
       }

       if (num_nmc_entries == 1)
       {
           ui->nmcRemoveButton->setDisabled(true);
       }
   }
   else
   {
       nmc_bcs.clear();
   }

   UpdateIdbPartset();
}

void MainWindow::on_nmcComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= nmc_bcs.size())
        return;

    if (prev_nmc_index >= 0 && prev_nmc_index < nmc_bcs.size())
    {
        nmc_bcs[prev_nmc_index] = ui->nmcBcsEdit->toPlainText();
    }

    ui->nmcBcsEdit->setText(nmc_bcs[index]);
    prev_nmc_index = index;
}

void MainWindow::on_nmcAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->nmcComboBox->currentIndex();
    if (idx >= 0 && idx < nmc_bcs.size())
    {
        partset = ui->nmcBcsEdit->toPlainText();
    }

    int pos = nmc_bcs.size();
    nmc_bcs.push_back(partset);

    ui->nmcComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->nmcComboBox->setCurrentIndex(pos);

    if (nmc_bcs.size() > 1)
        ui->nmcRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_nmcRemoveButton_clicked()
{
    int index = ui->nmcComboBox->currentIndex();

    if (index < 0 || index >= nmc_bcs.size())
        return;

    nmc_bcs.remove(index);

    if (prev_nmc_index > index)
        prev_nmc_index--;
    else
        prev_nmc_index = -1;

    ui->nmcComboBox->removeItem(index);

    for (int i = 0; i < ui->nmcComboBox->count(); i++)
    {
        ui->nmcComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (nmc_bcs.size() == 1)
        ui->nmcRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_friEnableCheck_clicked()
{
    bool checked = ui->friEnableCheck->isChecked();

    ui->friComboBox->setEnabled(checked);
    ui->friAddButton->setEnabled(checked);
    ui->friRemoveButton->setEnabled(checked);
    ui->friCopyButton->setEnabled(checked);
    ui->friBcsEdit->setEnabled(checked);

   if (checked)
   {
       // Clear combobox first
       prev_fri_index = -1;
       ui->friComboBox->clear();

       int num_fri_entries = fri_bcs.size();

       if (num_fri_entries == 0)
       {
           fri_bcs.push_back("<PartSet>\n</PartSet>");
           num_fri_entries++;
       }

       for (int i = 0; i < num_fri_entries; i++)
       {
           ui->friComboBox->addItem(QString("PartSet %1").arg(i));
       }

       if (num_fri_entries == 1)
       {
           ui->friRemoveButton->setDisabled(true);
       }
   }
   else
   {
       fri_bcs.clear();
   }

   UpdateIdbPartset();
}

void MainWindow::on_friComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= fri_bcs.size())
        return;

    if (prev_fri_index >= 0 && prev_fri_index < fri_bcs.size())
    {
        fri_bcs[prev_fri_index] = ui->friBcsEdit->toPlainText();
    }

    ui->friBcsEdit->setText(fri_bcs[index]);
    prev_fri_index = index;
}

void MainWindow::on_friAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->friComboBox->currentIndex();
    if (idx >= 0 && idx < fri_bcs.size())
    {
        partset = ui->friBcsEdit->toPlainText();
    }

    int pos = fri_bcs.size();
    fri_bcs.push_back(partset);

    ui->friComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->friComboBox->setCurrentIndex(pos);

    if (fri_bcs.size() > 1)
        ui->friRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_friRemoveButton_clicked()
{
    int index = ui->friComboBox->currentIndex();

    if (index < 0 || index >= fri_bcs.size())
        return;

    fri_bcs.remove(index);

    if (prev_fri_index > index)
        prev_fri_index--;
    else
        prev_fri_index = -1;

    ui->friComboBox->removeItem(index);

    for (int i = 0; i < ui->friComboBox->count(); i++)
    {
        ui->friComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (fri_bcs.size() == 1)
        ui->friRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_mamEnableCheck_clicked()
{
    bool checked = ui->mamEnableCheck->isChecked();

    ui->mamComboBox->setEnabled(checked);
    ui->mamAddButton->setEnabled(checked);
    ui->mamRemoveButton->setEnabled(checked);
    ui->mamCopyButton->setEnabled(checked);
    ui->mamBcsEdit->setEnabled(checked);

   if (checked)
   {
       // Clear combobox first
       prev_mam_index = -1;
       ui->mamComboBox->clear();

       int num_mam_entries = mam_bcs.size();

       if (num_mam_entries == 0)
       {
           mam_bcs.push_back("<PartSet>\n</PartSet>");
           num_mam_entries++;
       }

       for (int i = 0; i < num_mam_entries; i++)
       {
           ui->mamComboBox->addItem(QString("PartSet %1").arg(i));
       }

       if (num_mam_entries == 1)
       {
           ui->mamRemoveButton->setDisabled(true);
       }
   }
   else
   {
       mam_bcs.clear();
   }

   UpdateIdbPartset();
}

void MainWindow::on_mamComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= mam_bcs.size())
        return;

    if (prev_mam_index >= 0 && prev_mam_index < mam_bcs.size())
    {
        mam_bcs[prev_mam_index] = ui->mamBcsEdit->toPlainText();
    }

    ui->mamBcsEdit->setText(mam_bcs[index]);
    prev_mam_index = index;
}

void MainWindow::on_mamAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->mamComboBox->currentIndex();
    if (idx >= 0 && idx < mam_bcs.size())
    {
        partset = ui->mamBcsEdit->toPlainText();
    }

    int pos = mam_bcs.size();
    mam_bcs.push_back(partset);

    ui->mamComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->mamComboBox->setCurrentIndex(pos);

    if (mam_bcs.size() > 1)
        ui->mamRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_mamRemoveButton_clicked()
{
    int index = ui->mamComboBox->currentIndex();

    if (index < 0 || index >= mam_bcs.size())
        return;

    mam_bcs.remove(index);

    if (prev_mam_index > index)
        prev_mam_index--;
    else
        prev_mam_index = -1;

    ui->mamComboBox->removeItem(index);

    for (int i = 0; i < ui->mamComboBox->count(); i++)
    {
        ui->mamComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (mam_bcs.size() == 1)
        ui->mamRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_mafEnableCheck_clicked()
{
    bool checked = ui->mafEnableCheck->isChecked();

    ui->mafComboBox->setEnabled(checked);
    ui->mafAddButton->setEnabled(checked);
    ui->mafRemoveButton->setEnabled(checked);
    ui->mafCopyButton->setEnabled(checked);
    ui->mafBcsEdit->setEnabled(checked);

   if (checked)
   {
       // Clear combobox first
       prev_maf_index = -1;
       ui->mafComboBox->clear();

       int num_maf_entries = maf_bcs.size();

       if (num_maf_entries == 0)
       {
           maf_bcs.push_back("<PartSet>\n</PartSet>");
           num_maf_entries++;
       }

       for (int i = 0; i < num_maf_entries; i++)
       {
           ui->mafComboBox->addItem(QString("PartSet %1").arg(i));
       }

       if (num_maf_entries == 1)
       {
           ui->mafRemoveButton->setDisabled(true);
       }
   }
   else
   {
       maf_bcs.clear();
   }

   UpdateIdbPartset();
}

void MainWindow::on_mafComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= maf_bcs.size())
        return;

    if (prev_maf_index >= 0 && prev_maf_index < maf_bcs.size())
    {
        maf_bcs[prev_maf_index] = ui->mafBcsEdit->toPlainText();
    }

    ui->mafBcsEdit->setText(maf_bcs[index]);
    prev_maf_index = index;
}

void MainWindow::on_mafAddButton_clicked()
{
    QString partset = "<PartSet>\n</PartSet>";

    int idx = ui->mafComboBox->currentIndex();
    if (idx >= 0 && idx < maf_bcs.size())
    {
        partset = ui->mafBcsEdit->toPlainText();
    }

    int pos = maf_bcs.size();
    maf_bcs.push_back(partset);

    ui->mafComboBox->addItem(QString("PartSet %1").arg(pos));
    ui->mafComboBox->setCurrentIndex(pos);

    if (maf_bcs.size() > 1)
        ui->mafRemoveButton->setEnabled(true);

    UpdateIdbPartset();
}

void MainWindow::on_mafRemoveButton_clicked()
{
    int index = ui->mafComboBox->currentIndex();

    if (index < 0 || index >= maf_bcs.size())
        return;

    maf_bcs.remove(index);

    if (prev_maf_index > index)
        prev_maf_index--;
    else
        prev_maf_index = -1;

    ui->mafComboBox->removeItem(index);

    for (int i = 0; i < ui->mafComboBox->count(); i++)
    {
        ui->mafComboBox->setItemText(i, QString("PartSet %1").arg(i));
    }

    if (maf_bcs.size() == 1)
        ui->mafRemoveButton->setDisabled(true);

    UpdateIdbPartset();
}

void MainWindow::CopyBcs(QTextEdit *edit, BcsFile &bcs, uint8_t race_lock)
{
    ListDialog dialog(ListMode::BCS, this, &bcs, race_lock);

    if (!dialog.exec())
        return;

    int idx = dialog.GetResultInteger();
    if (idx < 0 || idx >= bcs.GetPartSets().size())
        return;

    edit->setText(BcsToString(bcs.GetPartSets()[idx]));
}

void MainWindow::on_humCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameHum)
    {
        CopyBcs(ui->humBcsEdit, *game_hum_bcs, IDB_RACE_HUM|IDB_RACE_SYM);
    }
    else if (arg1 == ui->actionFromExternalHum)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->humBcsEdit, bcs, 0);
    }
}

void MainWindow::on_hufCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameHuf)
    {
        CopyBcs(ui->hufBcsEdit, *game_huf_bcs, IDB_RACE_HUF|IDB_RACE_SYF);
    }
    else if (arg1 == ui->actionFromExternalHuf)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->hufBcsEdit, bcs, 0);
    }
}

void MainWindow::on_nmcCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameNmc)
    {
        CopyBcs(ui->nmcBcsEdit, *game_nmc_bcs, IDB_RACE_NMC);
    }
    else if (arg1 == ui->actionFromExternalNmc)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->nmcBcsEdit, bcs, 0);
    }
}

void MainWindow::on_friCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameFri)
    {
        CopyBcs(ui->friBcsEdit, *game_fri_bcs, IDB_RACE_FRI);
    }
    else if (arg1 == ui->actionFromExternalFri)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->friBcsEdit, bcs, 0);
    }
}

void MainWindow::on_mamCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameMam)
    {
        CopyBcs(ui->mamBcsEdit, *game_mam_bcs, IDB_RACE_MAM);
    }
    else if (arg1 == ui->actionFromExternalMam)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->mamBcsEdit, bcs, 0);
    }
}

void MainWindow::on_mafCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameMaf)
    {
        CopyBcs(ui->mafBcsEdit, *game_maf_bcs, IDB_RACE_MAF);
    }
    else if (arg1 == ui->actionFromExternalMaf)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        CopyBcs(ui->mafBcsEdit, bcs, 0);
    }
}

void MainWindow::ItemToGui(const X2mItem &item)
{
    if (item.item_type == X2mItemType::TOP)
    {
        ui->idbTypeComboBox->setCurrentIndex(0);
    }
    else if (item.item_type == X2mItemType::BOTTOM)
    {
        ui->idbTypeComboBox->setCurrentIndex(1);
    }
    else if (item.item_type == X2mItemType::GLOVES)
    {
        ui->idbTypeComboBox->setCurrentIndex(2);
    }
    else if (item.item_type == X2mItemType::SHOES)
    {
        ui->idbTypeComboBox->setCurrentIndex(3);
    }
    else
    {
        ui->idbTypeComboBox->setCurrentIndex(4);
    }

    int idx = ui->idbNameLangComboBox->currentIndex();
    if (idx >= 0 && idx < XV2_LANG_NUM)
    {
        ui->idbNameEdit->setText(Utils::StdStringToQString(item.item_name[idx], false));
    }

    idx = ui->idbDescLangComboBox->currentIndex();
    if (idx >= 0 && idx < XV2_LANG_NUM)
    {
        ui->idbDescEdit->setText(Utils::StdStringToQString(item.item_desc[idx], false));
    }

    ui->idbStarsEdit->setText(QString("%1").arg((int16_t)item.idb.stars));
    ui->idbU0AEdit->setText(QString("%1").arg((int16_t)item.idb.unk_0A));
    ui->idbU0CEdit->setText(QString("%1").arg((int16_t)item.idb.unk_0C));
    ui->idbU0EEdit->setText(QString("%1").arg((int16_t)item.idb.unk_0E));
    ui->idbBuyEdit->setText(QString("%1").arg((int32_t)item.idb.buy));
    ui->idbSellEdit->setText(QString("%1").arg((int32_t)item.idb.sell));
    ui->idbTpEdit->setText(QString("%1").arg((int32_t)item.idb.tp));
    //ui->idbModelEdit->setText(QString("%1").arg((int32_t)item.idb.model));
    ui->idbU24Edit->setText(QString("%1").arg((int32_t)item.idb.unk_24[0]));
    ui->idbU28Edit->setText(QString("%1").arg((int32_t)item.idb.unk_24[1]));
    ui->idbU2CEdit->setText(QString("%1").arg((int32_t)item.idb.unk_24[2]));

    if (ui->idbPartSetComboBox->count() < GetMaxNumPartSets())
    {
        for (int i = ui->idbPartSetComboBox->count(); i < GetMaxNumPartSets(); i++)
        {
            ui->idbPartSetComboBox->addItem(QString("%1").arg(i));
        }
    }

    if (item.idb.model >= (uint)ui->idbPartSetComboBox->count())
    {
        ui->idbPartSetComboBox->setCurrentIndex(ui->idbPartSetComboBox->count()-1);
    }
    else
    {
        ui->idbPartSetComboBox->setCurrentIndex(item.idb.model);
    }

    ui->idbHumCheck->setChecked(item.idb.racelock & IDB_RACE_HUM);
    ui->idbHufCheck->setChecked(item.idb.racelock & IDB_RACE_HUF);
    ui->idbSymCheck->setChecked(item.idb.racelock & IDB_RACE_SYM);
    ui->idbSyfCheck->setChecked(item.idb.racelock & IDB_RACE_SYF);
    ui->idbNmcCheck->setChecked(item.idb.racelock & IDB_RACE_NMC);
    ui->idbFriCheck->setChecked(item.idb.racelock & IDB_RACE_FRI);
    ui->idbMamCheck->setChecked(item.idb.racelock & IDB_RACE_MAM);
    ui->idbMafCheck->setChecked(item.idb.racelock & IDB_RACE_MAF);

    if (item.item_type != X2mItemType::ACCESSORY)
    {
        ui->idbFlag4Check->setChecked((item.idb.type & 4));
    }
    else
    {
        ui->idbFlag4Check->setChecked(false);
    }
}

void MainWindow::GuiToItem(X2mItem &item)
{
    int idx = ui->idbTypeComboBox->currentIndex();

    if (idx == 0)
    {
        item.item_type = X2mItemType::TOP;
    }
    else if (idx == 1)
    {
        item.item_type = X2mItemType::BOTTOM;
    }
    else if (idx == 2)
    {
        item.item_type = X2mItemType::GLOVES;
    }
    else if (idx == 3)
    {
        item.item_type = X2mItemType::SHOES;
    }
    else
    {
        item.item_type = X2mItemType::ACCESSORY;
    }

    // Name and Desc are handled by the text events

    item.idb.stars = (uint16_t) ui->idbStarsEdit->text().toInt();
    item.idb.unk_0A = (uint16_t) ui->idbU0AEdit->text().toInt();
    item.idb.unk_0C = (uint16_t) ui->idbU0CEdit->text().toInt();
    item.idb.unk_0E = (uint16_t) ui->idbU0EEdit->text().toInt();
    item.idb.buy = (uint32_t) ui->idbBuyEdit->text().toInt();
    item.idb.sell = (uint32_t) ui->idbSellEdit->text().toInt();
    item.idb.tp = (uint32_t) ui->idbTpEdit->text().toInt();
    //item.idb.model = (uint32_t) ui->idbModelEdit->text().toInt();
    item.idb.model = ui->idbPartSetComboBox->currentIndex();
    item.idb.unk_24[0] = (uint32_t) ui->idbU24Edit->text().toInt();
    item.idb.unk_24[1] = (uint32_t) ui->idbU28Edit->text().toInt();
    item.idb.unk_24[2] = (uint32_t) ui->idbU2CEdit->text().toInt();

    item.idb.racelock = 0;

    if (ui->idbHumCheck->isChecked())
        item.idb.racelock |= IDB_RACE_HUM;

    if (ui->idbHufCheck->isChecked())
        item.idb.racelock |= IDB_RACE_HUF;

    if (ui->idbSymCheck->isChecked())
        item.idb.racelock |= IDB_RACE_SYM;

    if (ui->idbSyfCheck->isChecked())
        item.idb.racelock |= IDB_RACE_SYF;

    if (ui->idbNmcCheck->isChecked())
        item.idb.racelock |= IDB_RACE_NMC;

    if (ui->idbFriCheck->isChecked())
        item.idb.racelock |= IDB_RACE_FRI;

    if (ui->idbMamCheck->isChecked())
        item.idb.racelock |= IDB_RACE_MAM;

    if (ui->idbMafCheck->isChecked())
        item.idb.racelock |= IDB_RACE_MAF;

    if (item.item_type == X2mItemType::TOP)
    {
        item.idb.type = (ui->idbFlag4Check->isChecked()) ? 4 : 0;
    }
    else if (item.item_type == X2mItemType::BOTTOM)
    {
        item.idb.type = (ui->idbFlag4Check->isChecked()) ? 5 : 1;
    }
    else if (item.item_type == X2mItemType::GLOVES)
    {
        item.idb.type = (ui->idbFlag4Check->isChecked()) ? 6 : 2;
    }
    else if (item.item_type == X2mItemType::SHOES)
    {
        item.idb.type = (ui->idbFlag4Check->isChecked()) ? 7 : 3;
    }
    else
    {
        item.idb.type = 8;
    }
}

void MainWindow::EditIdbEffect(IdbEffect &effect)
{
    IdbEffectDialog dialog(this, effect);

    if (!dialog.exec())
        return;

    effect = dialog.GetEffect();
}

void MainWindow::on_idbEnableCheck_clicked()
{
    bool checked = ui->idbEnableCheck->isChecked();

    ui->idbComboBox->setEnabled(checked);
    ui->idbAddButton->setEnabled(checked);
    ui->idbRemoveButton->setEnabled(checked);
    ui->idbTypeComboBox->setEnabled(checked);
    ui->idbNameEdit->setEnabled(checked);
    ui->idbNameLangComboBox->setEnabled(checked);
    ui->idbNameCopyButton->setEnabled(checked);
    ui->idbDescEdit->setEnabled(checked);
    ui->idbDescLangComboBox->setEnabled(checked);
    ui->idbDescCopyButton->setEnabled(checked);
    ui->idbCopyButton->setEnabled(checked);
    ui->idbHumCheck->setEnabled(checked);
    ui->idbHufCheck->setEnabled(checked);
    ui->idbSymCheck->setEnabled(checked);
    ui->idbSyfCheck->setEnabled(checked);
    ui->idbNmcCheck->setEnabled(checked);
    ui->idbFriCheck->setEnabled(checked);
    ui->idbMamCheck->setEnabled(checked);
    ui->idbMafCheck->setEnabled(checked);
    ui->idbStarsEdit->setEnabled(checked);
    ui->idbU0AEdit->setEnabled(checked);
    ui->idbU0CEdit->setEnabled(checked);
    ui->idbU0EEdit->setEnabled(checked);
    ui->idbBuyEdit->setEnabled(checked);
    ui->idbSellEdit->setEnabled(checked);
    ui->idbTpEdit->setEnabled(checked);
    ui->idbPartSetComboBox->setEnabled(checked);
    ui->idbU24Edit->setEnabled(checked);
    ui->idbU28Edit->setEnabled(checked);
    ui->idbU2CEdit->setEnabled(checked);
    ui->idbFlag4Check->setEnabled(checked);
    ui->idbEff1Button->setEnabled(checked);
    ui->idbEff2Button->setEnabled(checked);
    ui->idbEff3Button->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_idb_index = -1;
        ui->idbComboBox->clear();

        size_t num_item_entries = x2m->GetNumCostumeItems();

        if (num_item_entries == 0)
        {
            X2mItem item;

            GuiToItem(item);
            x2m->AddCostumeItem(item);
            num_item_entries++;
        }

        for (size_t i = 0; i < num_item_entries; i++)
        {
            ui->idbComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_item_entries == 1)
        {
            ui->idbRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumCostumeItems() != 0)
            x2m->RemoveCostumeItem(0);
    }
}

void MainWindow::on_idbComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumCostumeItems())
        return;

    if (prev_idb_index >= 0 && prev_idb_index < x2m->GetNumCostumeItems())
    {
        X2mItem &item = x2m->GetCostumeItem(prev_idb_index);
        GuiToItem(item);
    }

    const X2mItem &item = x2m->GetCostumeItem(index);    
    ItemToGui(item);

    prev_idb_index = index;
}

void MainWindow::on_idbAddButton_clicked()
{
    X2mItem item;

    int idx = ui->idbComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumCostumeItems())
    {
        item = x2m->GetCostumeItem(idx);
        GuiToItem(item);
    }

    size_t pos = x2m->AddCostumeItem(item);

    ui->idbComboBox->addItem(QString("Entry %1").arg(pos));
    ui->idbComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumCostumeItems() > 1)
        ui->idbRemoveButton->setEnabled(true);
}

void MainWindow::on_idbRemoveButton_clicked()
{
    int index = ui->idbComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumCostumeItems())
        return;

    x2m->RemoveCostumeItem(index);

    if (prev_idb_index > index)
        prev_idb_index--;
    else
        prev_idb_index = -1;

    ui->idbComboBox->removeItem(index);

    for (int i = 0; i < ui->idbComboBox->count(); i++)
    {
        ui->idbComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumCostumeItems() == 1)
        ui->idbRemoveButton->setDisabled(true);
}

void MainWindow::on_idbEff1Button_clicked()
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    EditIdbEffect(x2m->GetCostumeItem(idx).idb.effects[0]);
}

void MainWindow::on_idbEff2Button_clicked()
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    EditIdbEffect(x2m->GetCostumeItem(idx).idb.effects[1]);
}

void MainWindow::on_idbEff3Button_clicked()
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    EditIdbEffect(x2m->GetCostumeItem(idx).idb.effects[2]);
}

void MainWindow::on_idbNameEdit_textEdited(const QString &arg1)
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    X2mItem &item = x2m->GetCostumeItem(idx);

    idx = ui->idbNameLangComboBox->currentIndex();
    if (idx < 0 || idx >= XV2_LANG_NUM)
        return;

    item.item_name[idx] = Utils::QStringToStdString(arg1, false);
}

void MainWindow::on_idbDescEdit_textChanged()
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    X2mItem &item = x2m->GetCostumeItem(idx);

    idx = ui->idbDescLangComboBox->currentIndex();
    if (idx < 0 || idx >= XV2_LANG_NUM)
        return;

    item.item_desc[idx] = Utils::QStringToStdString(ui->idbDescEdit->toPlainText(), false);
}

void MainWindow::CopyIdb(IdbFile *idb, X2mItemType type)
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    X2mItem &item = x2m->GetCostumeItem(idx);

    ListDialog dialog((type == X2mItemType::ACCESSORY) ? ListMode::ACCESSORY_IDB : ListMode::COSTUME_IDB, this, idb);
    if (!dialog.exec())
        return;

    idx = dialog.GetResultInteger();
    if (idx < 0 || idx >= idb->GetNumEntries())
        return;

    bool copy_effects = true;
    IdbEffect effects[3];

    QMessageBox box(this);

    box.addButton("Copy effects too", QMessageBox::YesRole);
    box.addButton("Don't copy effects", QMessageBox::NoRole);
    box.setText("Do you want to copy the effects too?");
    box.setIcon(QMessageBox::Icon::Question);

    if (box.exec() != 0)
    {
        copy_effects = false;
        effects[0] = item.idb.effects[0];
        effects[1] = item.idb.effects[1];
        effects[2] = item.idb.effects[2];
    }

    uint32_t model = item.idb.model;
    item.idb = (*idb)[idx];
    item.idb.model = model;
    item.item_type = type;

    if (!copy_effects)
    {
        item.idb.effects[0] = effects[0];
        item.idb.effects[1] = effects[1];
        item.idb.effects[2] = effects[2];
    }

    ItemToGui(item);
}

void MainWindow::CopyName(IdbFile *idb, X2mItemType type)
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    X2mItem &item = x2m->GetCostumeItem(idx);

    ListDialog dialog((type == X2mItemType::ACCESSORY) ? ListMode::ACCESSORY_IDB : ListMode::COSTUME_IDB, this, idb);
    if (!dialog.exec())
        return;

    idx = dialog.GetResultInteger();
    if (idx < 0 || idx >= idb->GetNumEntries())
        return;

    const IdbEntry &entry = (*idb)[idx];

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string name;

        if (type == X2mItemType::ACCESSORY)
            Xenoverse2::GetAccesoryName(entry.name_id, name, i);
        else
            Xenoverse2::GetCacCostumeName(entry.name_id, name, i);

        item.item_name[i] = name;
    }

    ItemToGui(item);
}

void MainWindow::CopyDesc(IdbFile *idb, X2mItemType type)
{
    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    X2mItem &item = x2m->GetCostumeItem(idx);

    ListDialog dialog((type == X2mItemType::ACCESSORY) ? ListMode::ACCESSORY_IDB : ListMode::COSTUME_IDB, this, idb);
    if (!dialog.exec())
        return;

    idx = dialog.GetResultInteger();
    if (idx < 0 || idx >= idb->GetNumEntries())
        return;

    const IdbEntry &entry = (*idb)[idx];

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string desc;

        if (type == X2mItemType::ACCESSORY)
            Xenoverse2::GetAccesoryDesc(entry.desc_id, desc, i);
        else
            Xenoverse2::GetCacCostumeDesc(entry.desc_id, desc, i);

        item.item_desc[i] = desc;
    }

    ItemToGui(item);
}

void MainWindow::on_idbCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameCostumeTop)
    {
        CopyIdb(game_top_idb, X2mItemType::TOP);
    }
    else if (arg1 == ui->actionFromGameCostumeBottom)
    {
        CopyIdb(game_top_idb, X2mItemType::BOTTOM);
    }
    else if (arg1 == ui->actionFromGameCostumeGloves)
    {
        CopyIdb(game_top_idb, X2mItemType::GLOVES);
    }
    else if (arg1 == ui->actionFromGameCostumeShoes)
    {
        CopyIdb(game_top_idb, X2mItemType::SHOES);
    }
    else if (arg1 == ui->actionFromGameAccessory)
    {
        CopyIdb(game_accesory_idb, X2mItemType::ACCESSORY);
    }
}

void MainWindow::on_idbNameCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameCostumeTopName)
    {
        CopyName(game_top_idb, X2mItemType::TOP);
    }
    else if (arg1 == ui->actionFromGameCostumeBottomName)
    {
        CopyName(game_top_idb, X2mItemType::BOTTOM);
    }
    else if (arg1 == ui->actionFromGameCostumeGlovesName)
    {
        CopyName(game_top_idb, X2mItemType::GLOVES);
    }
    else if (arg1 == ui->actionFromGameCostumeShoesName)
    {
        CopyName(game_top_idb, X2mItemType::SHOES);
    }
    else if (arg1 == ui->actionFromGameAccessoryName)
    {
        CopyName(game_accesory_idb, X2mItemType::ACCESSORY);
    }
}

void MainWindow::on_idbDescCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameCostumeTopDesc)
    {
        CopyDesc(game_top_idb, X2mItemType::TOP);
    }
    else if (arg1 == ui->actionFromGameCostumeBottomDesc)
    {
        CopyDesc(game_top_idb, X2mItemType::BOTTOM);
    }
    else if (arg1 == ui->actionFromGameCostumeGlovesDesc)
    {
        CopyDesc(game_top_idb, X2mItemType::GLOVES);
    }
    else if (arg1 == ui->actionFromGameCostumeShoesDesc)
    {
        CopyDesc(game_top_idb, X2mItemType::SHOES);
    }
    else if (arg1 == ui->actionFromGameAccessoryDesc)
    {
        CopyDesc(game_accesory_idb, X2mItemType::ACCESSORY);
    }
}

void MainWindow::on_idbNameLangComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= XV2_LANG_NUM)
        return;

    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    ui->idbNameEdit->setText(Utils::StdStringToQString(x2m->GetCostumeItem(idx).item_name[index], false));
}

void MainWindow::on_idbDescLangComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= XV2_LANG_NUM)
        return;

    int idx = ui->idbComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCostumeItems())
        return;

    ui->idbDescEdit->setText(Utils::StdStringToQString(x2m->GetCostumeItem(idx).item_desc[index], false));
}

void MainWindow::on_guidCopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->modGuidEdit->text());
}
