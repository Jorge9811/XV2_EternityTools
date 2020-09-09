#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QClipboard>

#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QAudioOutput>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listdialog.h"
#include "cssdialog.h"
#include "embdialog.h"
#include "gatedialog.h"
#include "waitdialog.h"

#include "Xenoverse2.h"
#include "xv2ins_common.h"
#include "Config.h"
#include "VirtualStageEmb.h"
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
    // Files tab
    ui->lightingButton->addAction(ui->actionSelectFileLight);
    ui->lightingButton->addAction(ui->actionFromGameLight);
    ui->selPortButton->addAction(ui->actionSelectFileSel);
    ui->selPortButton->addAction(ui->actionFromGameSel);
    ui->questPortButton->addAction(ui->actionSelectFileQst);
    ui->questPortButton->addAction(ui->actionFromGameQst);
    ui->selBackButton->addAction(ui->actionSelectFileBack);
    ui->selBackButton->addAction(ui->actionFromGameBack);
    // Def tab
    ui->f6Edit->setValidator(new QDoubleValidator(this));
    ui->unk5Edit->setValidator(new QIntValidator(this));
    ui->bgmEdit->setValidator(new QIntValidator(0, 0xFFFF, this));
    // Patch tab
    ui->farClipEdit->setValidator(new QDoubleValidator(this));
    ui->limitEdit->setValidator(new QDoubleValidator(this));

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    Bootstrap(false, false);

    x2m = new X2mFile();
    x2m->SetType(X2mType::NEW_STAGE);

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

    if (x2m->GetType() != X2mType::NEW_STAGE)
    {
        DPRINTF("This kind of x2m is not supported by this editor.\n");
        return false;
    }

    ProcessX2m();
    return true;
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

void MainWindow::ProcessX2m()
{
    // Info tab
    ui->modNameEdit->setText(Utils::StdStringToQString(x2m->GetModName(), false));
    ui->modAuthorEdit->setText(Utils::StdStringToQString(x2m->GetModAuthor(), false));
    ui->modVersionEdit->setText(QString::number(x2m->GetModVersion()));
    ui->modGuidEdit->setText(Utils::StdStringToQString(x2m->GetModGuid()));
    ui->stageNameEdit->setText(Utils::StdStringToQString(x2m->GetStageName(ui->stageNameLangComboBox->currentIndex()), false));

    // Files tab
    if (x2m->StageDirectoryExists())
    {
        ui->stageFilesEdit->setText(INTERNAL_DATA);
        ui->stageFilesEdit->setEnabled(false);
    }
    else
    {
        ui->stageFilesEdit->setText("");
        ui->stageFilesEdit->setEnabled(true);
    }

    if (x2m->StageLightingExists())
    {
        ui->lightingEdit->setText(INTERNAL_DATA);
        ui->lightingEdit->setEnabled(false);
    }
    else
    {
        ui->lightingEdit->setText("");
        ui->lightingEdit->setEnabled(true);
    }

    if (x2m->StageSelPortraitExists())
    {
        ui->selPortEdit->setText(INTERNAL_DATA);
        ui->selPortEdit->setEnabled(false);
    }
    else
    {
        ui->selPortEdit->setText("");
        ui->selPortEdit->setEnabled(true);
    }

    if (x2m->StageQstPortraitExists())
    {
        ui->questPortEdit->setText(INTERNAL_DATA);
        ui->questPortEdit->setEnabled(false);
    }
    else
    {
        ui->questPortEdit->setText("");
        ui->questPortEdit->setEnabled(true);
    }

    if (x2m->StageSelBackgroundExists())
    {
        ui->selBackEdit->setText(INTERNAL_DATA);
        ui->selBackEdit->setEnabled(false);
    }
    else
    {
        ui->selBackEdit->setText("");
        ui->selBackEdit->setEnabled(true);
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

    // Def tab
    DefToGui(x2m->GetStageDef());

    // Slots tab
    ui->slotsCheck->setChecked(x2m->AddStageSlot());
    ui->slotsLocalCheck->setChecked(x2m->AddStageSlotLocal());
    ui->slotsLocalCheck->setEnabled(x2m->AddStageSlot());

    // Patch tab
    // Already done in def tab

    // Vfx tab
    ui->vfxBGEdit->setText(Utils::StdStringToQString(x2m->GetStageBgEepk()));
    ui->vfxStageEdit->setText(Utils::StdStringToQString(x2m->GetStageEepk()));

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

    if (Utils::IsEmptyString(x2m->GetStageName(XV2_LANG_ENGLISH)))
    {
        DPRINTF("[INFO] Stage name cannot be empty for english language.\n");
        return false;
    }

    // Def tab (must be validated before files)
    QString code = ui->codeEdit->text().trimmed();
    if (code.isEmpty())
    {
        DPRINTF("[DEF] Code cannot be empty.\n");
        return false;
    }

    if (code.length() != 5)
    {
        DPRINTF("[DEF] Code must be 5 characters long.\n");
        return false;
    }

    std::string code_std = Utils::QStringToStdString(code);
    if (!Utils::IsAlphaNumeric(code_std))
    {
        DPRINTF("[DEF] Code must be alpha-numeric only.\n");
        return false;
    }

    if (Xenoverse2::IsForbiddenNewStageName(code_std))
    {
        DPRINTF("[DEF] Code conflicts with original game data, please use another name.\n");
        return false;
    }

    if (code.startsWith("ICO", Qt::CaseInsensitive))
    {
        DPRINTF("[DEF] Code cannot start with \"ICO\"");
        return false;
    }

    if (code.startsWith("X2M", Qt::CaseInsensitive))
    {
        DPRINTF("[DEF] Code cannot start with \"X2M\"");
        return false;
    }

    if (ui->dirEdit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] Dir cannot be empty.\n");
        return false;
    }

    if (ui->str4Edit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] Str4 cannot be empty.\n");
        return false;
    }

    if (ui->eveEdit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] Eve cannot be empty.\n");
        return false;
    }

    if (ui->f6Edit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] F6 cannot be empty.\n");
        return false;
    }

    /*if (ui->seEdit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] SE cannot be empty.\n");
        return false;
    }*/

    if (ui->bgmEdit->text().trimmed().isEmpty())
    {
        DPRINTF("[DEF] Bgm Cue Id cannot be empty.\n");
        return false;
    }

    // Files tab
    QString stage_files = ui->stageFilesEdit->text().trimmed();
    if (stage_files.isEmpty())
    {
        DPRINTF("[FILES] Stage files cannot be empty.\n");
        return false;
    }

    if (ui->stageFilesEdit->isEnabled())
    {
        std::string stage_files_std = Utils::NormalizePath(Utils::QStringToStdString(stage_files));

        if (!Utils::DirExists(stage_files_std))
        {
            DPRINTF("[FILES] Directory %s doesn't exist.\n", stage_files_std.c_str());
            return false;
        }
        else if (Utils::IsDirectoryEmpty(stage_files_std, true))
        {
            DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", stage_files_std.c_str());
            return false;
        }
        else
        {
            std::string map_file = Utils::MakePathString(stage_files_std, code_std + ".map");
            if (!Utils::FileExists(map_file))
            {
                DPRINTF("[FILES] Map file doesnt exist (%s)\n", map_file.c_str());
                return false;
            }

            temp_path = stage_files_std;
            if (!Utils::VisitDirectory(stage_files_std, true, false, true, ValidateFilesVisitor, this, true))
                return false;
        }
    }

    QString lighting = ui->lightingEdit->text().trimmed();
    if (lighting.isEmpty())
    {
        DPRINTF("[FILES] Lighting cannot be empty.\n");
        return false;
    }

    if (ui->lightingEdit->isEnabled())
    {
        std::string lighting_std = Utils::QStringToStdString(lighting);

        if (!Utils::FileExists(lighting_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", lighting_std.c_str());
            return false;
        }
    }

    QString sel_port = ui->selPortEdit->text().trimmed();
    if (sel_port.isEmpty())
    {
        DPRINTF("[FILES] Select portrait cannot be empty.\n");
        return false;
    }

    if (ui->selPortEdit->isEnabled())
    {
        std::string sel_port_std = Utils::QStringToStdString(sel_port);

        if (!Utils::FileExists(sel_port_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", sel_port_std.c_str());
            return false;
        }
    }

    QString qst_port = ui->questPortEdit->text().trimmed();
    if (qst_port.isEmpty())
    {
        DPRINTF("[FILES] Quest portrait cannot be empty.\n");
        return false;
    }

    if (ui->questPortEdit->isEnabled())
    {
        std::string qst_port_std = Utils::QStringToStdString(qst_port);

        if (!Utils::FileExists(qst_port_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", qst_port_std.c_str());
            return false;
        }
    }

    QString sel_back = ui->selBackEdit->text().trimmed();
    if (sel_back.isEmpty())
    {
        DPRINTF("[FILES] Select background cannot be empty.\n");
        return false;
    }

    if (ui->selBackEdit->isEnabled())
    {
        std::string sel_back_std = Utils::QStringToStdString(sel_back);

        if (!Utils::FileExists(sel_back_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", sel_back_std.c_str());
            return false;
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

    // Patch tab
    if (ui->farClipEdit->text().isEmpty())
        x2m->GetStageDef().override_far_clip = 0.0f;

    return true;
}

bool MainWindow::Build()
{
    // Info
    x2m->SetModName(Utils::QStringToStdString(ui->modNameEdit->text(), false));
    x2m->SetModAuthor(Utils::QStringToStdString(ui->modAuthorEdit->text(), false));
    x2m->SetModVersion(ui->modVersionEdit->text().toFloat());

    // Files
    QString stage_files = ui->stageFilesEdit->text().trimmed();

    if (ui->stageFilesEdit->isEnabled())
    {
        const std::string stage_files_std = Utils::NormalizePath(Utils::QStringToStdString(stage_files));
        std::string entry_name;

        x2m->DeleteStageDirectory();

        if (!x2m->AddExternalDirectory(stage_files_std, X2M_STAGE))
        {
            DPRINTF("AddExternalDirectory failed on stage files.\n");
            return false;
        }
    }
    else
    {
        if (stage_files != INTERNAL_DATA)
        {
            DPRINTF("%s: Bug in implementation (1).\n", FUNCNAME);
            return false;
        }

        // Do nothing, keep the data that already exists on the x2m
    }

    QString lighting = ui->lightingEdit->text().trimmed();

    if (ui->lightingEdit->isEnabled())
    {
        std::string lighting_std = Utils::QStringToStdString(lighting);
        uint8_t *buf;
        size_t size;

        if (lighting.endsWith(".xml", Qt::CaseInsensitive))
        {
            EmbFile emb;

            if (!emb.CompileFromFile(lighting_std))
            {
                DPRINTF("Failed to compile .emb.xml");
                return false;
            }

            buf = emb.Save(&size);
        }
        else
        {
            buf = Utils::ReadFile(lighting_std, &size);
        }

        if (!buf)
            return false;

        bool ret = x2m->WriteFile(X2M_STAGE_LIGHTING, buf, size);
        delete[] buf;

        if (!ret)
            return false;
    }
    else
    {
        if (lighting == INTERNAL_DATA)
        {
            // Do nothing
        }
        else if (lighting.startsWith(GAME_PREFIX))
        {
            QString game_emb = lighting.mid((int)strlen(GAME_PREFIX));

            size_t size;
            uint8_t *buf;

            buf = GetGameLighting(game_emb, &size);
            if (!buf)
            {
                DPRINTF("Failed getting lighting from game.\n");
                return false;
            }

            bool ret = x2m->WriteFile(X2M_STAGE_LIGHTING, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else
        {
            DPRINTF("%s: Bug in implementation (3).\n", FUNCNAME);
            return false;
        }
    }

    {
        QString sel_port = ui->selPortEdit->text().trimmed();

        if (ui->selPortEdit->isEnabled())
        {
            size_t size;
            uint8_t *buf;

            buf = Utils::ReadFile(Utils::QStringToStdString(sel_port), &size);
            if (!buf)
                return false;

            bool ret = x2m->WriteFile(X2M_STAGE_SEL_PORTRAIT, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else
        {
            if (sel_port == INTERNAL_DATA)
            {
                // Nothing to do
            }
            else if (sel_port.startsWith(GAME_PREFIX))
            {
                QString game_dds = sel_port.mid((int)strlen(GAME_PREFIX));

                size_t size;
                uint8_t *buf;

                buf = GetGameSelPortrait(game_dds, &size);
                if (!buf)
                {
                    DPRINTF("Failed getting sel portrait from game.\n");
                    return false;
                }

                bool ret = x2m->WriteFile(X2M_STAGE_SEL_PORTRAIT, buf, size);
                delete[] buf;

                if (!ret)
                    return false;
            }
            else
            {
                DPRINTF("%s: Bug in implementation (2).\n", FUNCNAME);
                return false;
            }
        }
    }

    {

        QString qst_port = ui->questPortEdit->text().trimmed();

        if (ui->questPortEdit->isEnabled())
        {
            size_t size;
            uint8_t *buf;

            buf = Utils::ReadFile(Utils::QStringToStdString(qst_port), &size);
            if (!buf)
                return false;

            bool ret = x2m->WriteFile(X2M_STAGE_QST_PORTRAIT, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else
        {
            if (qst_port == INTERNAL_DATA)
            {
                // Nothing to do
            }
            else if (qst_port.startsWith(GAME_PREFIX))
            {
                QString game_dds = qst_port.mid((int)strlen(GAME_PREFIX));

                size_t size;
                uint8_t *buf;

                buf = GetGameQstPortrait(game_dds, &size);
                if (!buf)
                {
                    DPRINTF("Failed getting quest portrait from game.\n");
                    return false;
                }

                bool ret = x2m->WriteFile(X2M_STAGE_QST_PORTRAIT, buf, size);
                delete[] buf;

                if (!ret)
                    return false;
            }
            else
            {
                DPRINTF("%s: Bug in implementation (2).\n", FUNCNAME);
                return false;
            }
        }
    }

    {
        QString sel_back = ui->selBackEdit->text().trimmed();

        if (ui->selBackEdit->isEnabled())
        {
            size_t size;
            uint8_t *buf;

            buf = Utils::ReadFile(Utils::QStringToStdString(sel_back), &size);
            if (!buf)
                return false;

            bool ret = x2m->WriteFile(X2M_STAGE_SEL_BACKGROUND, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else
        {
            if (sel_back == INTERNAL_DATA)
            {
                // Nothing to do
            }
            else if (sel_back.startsWith(GAME_PREFIX))
            {
                QString game_dds = sel_back.mid((int)strlen(GAME_PREFIX));

                size_t size;
                uint8_t *buf;

                buf = GetGameSelBackground(game_dds, &size);
                if (!buf)
                {
                    DPRINTF("Failed getting select background from game.\n");
                    return false;
                }

                bool ret = x2m->WriteFile(X2M_STAGE_SEL_BACKGROUND, buf, size);
                delete[] buf;

                if (!ret)
                    return false;
            }
            else
            {
                DPRINTF("%s: Bug in implementation (2).\n", FUNCNAME);
                return false;
            }
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
            DPRINTF("%s: Bug in implementation (4).\n", FUNCNAME);
            return false;
        }
    }

    // Def
    GuiToDef(x2m->GetStageDef());

    // Slots
    x2m->SetAddStageSlot(ui->slotsCheck->isChecked());
    x2m->SetAddStageSlotLocal(ui->slotsLocalCheck->isEnabled() && ui->slotsLocalCheck->isChecked());

    // Vfx
    x2m->SetStageBgEepk(Utils::QStringToStdString(ui->vfxBGEdit->text().trimmed()));
    x2m->SetStageEepk(Utils::QStringToStdString(ui->vfxStageEdit->text().trimmed()));

    return true;
}

bool MainWindow::ValidateFilesVisitor(const std::string &path, bool, void *param)
{
    MainWindow *pthis = (MainWindow *)param;

    if (path.length() <= pthis->temp_path.length())
        return true;

    std::string rel_path = path.substr(pthis->temp_path.length());
    while (rel_path[0] == '/')
        rel_path = rel_path.substr(1);

    std::string game_path ="data/stage/" + rel_path;

    if (xv2fs->FileExists(game_path, true, false))
    {
        DPRINTF("(In stage files) File \"%s\" collides with original game files.\n", rel_path.c_str());
        return false;
    }

    return true;
}

uint8_t *MainWindow::GetGameLighting(const QString &file, size_t *psize)
{
    EmbFile emb;

    if (!file.endsWith(".emb", Qt::CaseInsensitive))
    {
        DPRINTF("%s: Bug.\n", FUNCNAME);
        return nullptr;
    }

    std::string code = Utils::QStringToStdString(file.left(file.lastIndexOf(".emb", -1, Qt::CaseInsensitive)));

    if (!xv2fs->LoadFile(&emb, "data/lighting/environment/" + Utils::QStringToStdString(file)))
        return nullptr;

    return emb.Save(psize);
}

uint8_t *MainWindow::GetGameSelPortrait(const QString &name, size_t *psize)
{
    uint16_t idx = VirtualStageEmb::portraits.FindIndexByName(Utils::QStringToStdString(name));
    if (idx == 0xFFFF)
        return nullptr;

    const EmbContainedFile &file = VirtualStageEmb::portraits[idx];

    *psize = file.GetSize();
    uint8_t *buf = new uint8_t[*psize];

    memcpy(buf, file.GetData(), *psize);
    return buf;
}

uint8_t *MainWindow::GetGameQstPortrait(const QString &name, size_t *psize)
{
    uint16_t idx = game_stage01_emb->FindIndexByName(Utils::QStringToStdString(name));
    if (idx == 0xFFFF)
        return nullptr;

    const EmbContainedFile &file = (*game_stage01_emb)[idx];

    *psize = file.GetSize();
    uint8_t *buf = new uint8_t[*psize];

    memcpy(buf, file.GetData(), *psize);
    return buf;
}

uint8_t *MainWindow::GetGameSelBackground(const QString &name, size_t *psize)
{
    uint16_t idx = VirtualStageEmb::backgrounds.FindIndexByName(Utils::QStringToStdString(name));
    if (idx == 0xFFFF)
        return nullptr;

    const EmbContainedFile &file = VirtualStageEmb::backgrounds[idx];

    *psize = file.GetSize();
    uint8_t *buf = new uint8_t[*psize];

    memcpy(buf, file.GetData(), *psize);
    return buf;
}

void MainWindow::DefToGui(const Xv2Stage &stage)
{
    ui->codeEdit->setText(Utils::StdStringToQString(stage.code));
    ui->dirEdit->setText(Utils::StdStringToQString(stage.dir));
    ui->str4Edit->setText(Utils::StdStringToQString(stage.str4));
    ui->eveEdit->setText(Utils::StdStringToQString(stage.eve));
    ui->unk5Edit->setText(QString("%1").arg(stage.unk5));
    ui->f6Edit->setText(QString("%1").arg((double)stage.ki_blast_size_limit));
    ui->seEdit->setText(Utils::StdStringToQString(stage.se));
    ui->bgmEdit->setText(QString("%1").arg(stage.bgm_cue_id));

    // Patch tab
    ui->farClipEdit->setText(QString("%1").arg((double)stage.override_far_clip));
    ui->limitEdit->setText(QString("%1").arg((double)stage.limit));
}

void MainWindow::GuiToDef(Xv2Stage &stage)
{
    stage.code = Utils::QStringToStdString(ui->codeEdit->text().trimmed());
    stage.dir = Utils::QStringToStdString(ui->dirEdit->text().trimmed());
    stage.str4 = Utils::QStringToStdString(ui->str4Edit->text().trimmed());
    stage.eve = Utils::QStringToStdString(ui->eveEdit->text().trimmed());
    stage.unk5 = ui->unk5Edit->text().toUInt();
    stage.ki_blast_size_limit = ui->f6Edit->text().toFloat();
    stage.se = Utils::QStringToStdString(ui->seEdit->text().trimmed());
    stage.bgm_cue_id = ui->bgmEdit->text().toUInt();

    // Patch tab
    stage.override_far_clip = ui->farClipEdit->text().toFloat();
    stage.limit = ui->limitEdit->text().toFloat();
}

void MainWindow::PlayAudio(HcaFile *hca)
{
    static QBuffer *audio_buffer = nullptr;
    static QAudioOutput *audio_output = nullptr;

    if (audio_playing)
    {
        audio_output->reset();
        audio_playing = false;
    }

    int bits;
    size_t size;

    uint8_t *buf = hca->Decode(&bits, &size);
    if (!buf)
    {
        DPRINTF("%s: hca decode failed.\n", FUNCNAME);
        return;
    }

    QAudioFormat format;

    format.setSampleRate(hca->GetSampleRate());
    format.setChannelCount(hca->GetNumChannels());
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);

    if (bits == AUDIO_FORMAT_FLOAT)
    {
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::Float);
    }
    else
    {
        format.setSampleSize(bits);
        format.setSampleType(QAudioFormat::SignedInt);
    }

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
    {
        DPRINTF("Cannot play that format.\n");
        delete[] buf;
        return;
    }

    if (!audio_buffer)
    {
        audio_buffer = new QBuffer();
    }
    else
    {
        delete audio_buffer;
        audio_buffer = new QBuffer();
    }

    audio_buffer->setData((const char *)buf, (int)size);
    delete[] buf;

    audio_buffer->open(QIODevice::ReadOnly);

    if (!audio_output)
    {
        audio_output = new QAudioOutput(format, this);
    }
    else
    {
        delete audio_output;
        audio_output = new QAudioOutput(format, this);
    }

    audio_playing = true;
    audio_output->start(audio_buffer);
}

void MainWindow::PlayAudio(AcbFile *acb, AwbFile *awb, uint32_t cue_id)
{
    uint32_t cue_idx = acb->CueIdToCueIndex(cue_id);
    if (cue_idx == (uint32_t)-1)
    {
        DPRINTF("%s: CueIdToCueIndex failed.\n", FUNCNAME);
        return;
    }

    bool external;
    uint32_t awb_idx = acb->CueIndexToAwbIndex(cue_idx, &external);

    if (awb_idx == (uint32_t)-1)
    {
        DPRINTF("%s: CueIndexToAwbIndex failed.\n", FUNCNAME);
        return;
    }

    Afs2File internal_awb;

    if (!external)
    {
        uint32_t awb_size;
        uint8_t *awb_buf = acb->GetAwb(&awb_size);

        if (!awb_buf)
        {
            DPRINTF("%s internal awb couldn't be extracted.\n", FUNCNAME);
            return;
        }

        if (!internal_awb.Load(awb_buf, awb_size))
        {
            DPRINTF("%s: Failed to load internal awb.\n", FUNCNAME);
            return;
        }

        awb = &internal_awb;
    }

    uint64_t size;
    uint8_t *hca_buf = awb->ExtractFile(awb_idx, &size);

    if (!hca_buf)
    {
        DPRINTF("%s: AwbFile::ExtractFile failed.\n", FUNCNAME);
        return;
    }

    HcaFile hca;

    if (!hca.Load(hca_buf, size))
    {
        DPRINTF("%s: HcaFile::Load failed.\n", FUNCNAME);
        return;
    }

    delete[] hca_buf;
    PlayAudio(&hca);
}

void MainWindow::ExecGateDialog(Xv2StageGate *gate)
{
    GateDialog dialog(gate, this);
    dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
    if (ProcessShutdown())
        qApp->exit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME_STAGECREAT);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME_STAGECREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME_STAGECREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));


    box.exec();
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

    if (new_x2m->GetType() != X2mType::NEW_STAGE)
    {
        DPRINTF("That x2m is not of new stage type!\n");
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
    WaitDialog wait;

    if (show_wait_onsave)
    {
        wait.show();
        qApp->processEvents();
    }

    if (!Build())
    {
        if (show_wait_onsave)
            wait.close();

        DPRINTF("Build failed.\n");
        return;
    }

    bool ret = x2m->SaveToFile(Utils::QStringToStdString(file));

    if (show_wait_onsave)
        wait.close();

    if (!ret)
    {
        DPRINTF("x2m save failed.\n");        
    }
    else
    {
        UPRINTF("File has been succesfully written.\n");
        show_wait_onsave = false;
    }
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

void MainWindow::on_stageNameLangComboBox_currentIndexChanged(int index)
{
    ui->stageNameEdit->setText(Utils::StdStringToQString(x2m->GetStageName(index), false));
}

void MainWindow::on_stageNameEdit_textEdited(const QString &arg1)
{
    x2m->SetStageName(Utils::QStringToStdString(arg1, false), ui->stageNameLangComboBox->currentIndex());
}

void MainWindow::on_stageNameCopyButton_clicked()
{
    ListDialog dialog(ListMode::STAGE, this);

    if (!dialog.exec())
        return;

    const Xv2Stage &stage = (*game_stage_def)[dialog.GetResultInteger()];

    std::vector<std::string> name;
    name.resize(XV2_LANG_NUM);

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        Xenoverse2::GetStageName(stage.code, name[i], i);
    }

    if (name[0].length() == 0)
        return;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        x2m->SetStageName(name[i], i);
    }

    int lang = ui->stageNameLangComboBox->currentIndex();

    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        ui->stageNameEdit->setText(Utils::StdStringToQString(x2m->GetStageName(lang), false));
    }
}

void MainWindow::on_stageFilesButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose stage directory", config.ld_stage_dir);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::NormalizePath(Utils::QStringToStdString(dir));
    size_t pos = dir_std.rfind('/');

    if (pos == std::string::npos)
        return;

    //std::string dir_name = dir_std.substr(pos+1);

    config.ld_stage_dir = dir;
    x2m->DeleteStageDirectory();

    ui->stageFilesEdit->setText(dir);
    ui->stageFilesEdit->setEnabled(true);
    show_wait_onsave = true;
}

void MainWindow::on_lightingButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameLight)
    {
        ListDialog dialog(ListMode::STAGE_LIGHTING, this);

        if (dialog.exec())
        {
            ui->lightingEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->lightingEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileLight)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select emb file", config.lf_stage_lighting, "EMB Files (*.emb *.emb.xml)");

        if (file.isNull())
            return;

        config.lf_stage_lighting = file;

        ui->lightingEdit->setText(file);
        ui->lightingEdit->setEnabled(true);
    }
}

void MainWindow::on_selPortButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameSel)
    {
        if (VirtualStageEmb::portraits.GetNumFiles() == 0)
        {
            if (!VirtualStageEmb::Build())
                return;
        }

        EmbDialog dialog(&VirtualStageEmb::portraits, this);
        dialog.SetImageSize(160, 56);

        if (dialog.exec())
        {
            ui->selPortEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->selPortEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileSel)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select dds file", config.lf_sel_port_stage, "DDS Files (*.dds)");

        if (file.isNull())
            return;

        config.lf_sel_port_stage = file;

        ui->selPortEdit->setText(file);
        ui->selPortEdit->setEnabled(true);
    }
}

void MainWindow::on_questPortButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameQst)
    {
        if (!game_stage01_emb)
        {
            if (!Xenoverse2::InitStageEmb())
                return;
        }

        EmbDialog dialog(game_stage01_emb, this);
        dialog.SetImageSize(160, 40);

        if (dialog.exec())
        {
            ui->questPortEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->questPortEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileQst)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select dds file", config.lf_qst_port_stage, "DDS Files (*.dds)");

        if (file.isNull())
            return;

        config.lf_qst_port_stage = file;

        ui->questPortEdit->setText(file);
        ui->questPortEdit->setEnabled(true);
    }
}

void MainWindow::on_selBackButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameBack)
    {
        if (VirtualStageEmb::backgrounds.GetNumFiles() == 0)
        {
            if (!VirtualStageEmb::Build())
                return;
        }

        EmbDialog dialog(&VirtualStageEmb::backgrounds, this);
        dialog.SetImageSize(160, 40);

        if (dialog.exec())
        {
            ui->selBackEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->selBackEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileBack)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select dds file", config.lf_sel_back_stage, "DDS Files (*.dds)");

        if (file.isNull())
            return;

        config.lf_sel_back_stage = file;

        ui->selBackEdit->setText(file);
        ui->selBackEdit->setEnabled(true);
    }
}

void MainWindow::on_addDataButton_clicked()
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
                                  "what defeats the philosophy of a new stage mod.\n\n"
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

void MainWindow::on_defCopyButton_clicked()
{
    ListDialog dialog(ListMode::STAGE, this);

    if (!dialog.exec())
        return;

    Xv2Stage temp;
    GuiToDef(temp);

    Xv2Stage &def = x2m->GetStageDef();
    def = (*game_stage_def)[dialog.GetResultInteger()];
    def.code = temp.code;
    def.override_far_clip = temp.override_far_clip;
    def.name = temp.name;

    DefToGui(def);
}

void MainWindow::on_bgmButton_clicked()
{
    AcbFile bgm_acb;
    Afs2File bgm_awb;

    bool ok;
    uint32_t cue_id = ui->bgmEdit->text().toUInt(&ok);

    if (!ok)
    {
        DPRINTF("Bgm Cue Id not specified.\n");
        return;
    }

    if (ui->bgmEdit->text().trimmed().isEmpty())
    {
        DPRINTF("Bgm Cue Id is empty, nothing to listen.\n");
        return;
    }

    if (!xv2fs->LoadFile(dynamic_cast<CriwareAudioContainer *>(&bgm_acb), "data/sound/BGM/CAR_BGM.acb"))
    {
        DPRINTF("Failed to load CAR_BGM.acb\n");
        return;
    }

    if (!xv2fs->LoadFile(&bgm_awb, "data/sound/BGM/CAR_BGM.awb"))
    {
        DPRINTF("Failed to load CAR_BGM.awb\n");
        return;
    }

    PlayAudio(&bgm_acb, &bgm_awb, cue_id);
}

void MainWindow::on_gate1Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[0]);
}

void MainWindow::on_gate2Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[1]);
}

void MainWindow::on_gate3Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[2]);
}

void MainWindow::on_gate4Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[3]);
}

void MainWindow::on_gate5Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[4]);
}

void MainWindow::on_gate6Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[5]);
}

void MainWindow::on_gate7Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[6]);
}

void MainWindow::on_gate8Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[7]);
}

void MainWindow::on_gate9Button_clicked()
{
    ExecGateDialog(&x2m->GetStageDef().gates[8]);
}

void MainWindow::on_slotsCheck_clicked()
{
    ui->slotsLocalCheck->setEnabled(ui->slotsCheck->isChecked());
}

void MainWindow::on_guidCopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->modGuidEdit->text());
}

void MainWindow::on_vfxCopyButton_clicked()
{
    ListDialog dialog(ListMode::STAGE, this);

    if (dialog.exec())
    {
        size_t idx = (size_t)dialog.GetResultInteger();

        if (!game_ers)
        {
            if (!Xenoverse2::InitVfx())
            {
                DPRINTF("InitVfx failed.\n");
                return;
            }
        }

        ui->vfxBGEdit->setText(Utils::StdStringToQString(game_ers->GetStageBgEepk(idx)));
        ui->vfxStageEdit->setText(Utils::StdStringToQString(game_ers->GetStageEepk(idx)));
    }
}


