#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QTime>

#include <map>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Xenoverse2.h"
#include "IniFile.h"
#include "Config.h"
#include "debug.h"

#define PROGRAM_NAME    "XV2 Quest Importer"
#define PROGRAM_VERSION "0.61"

#define INSTALLED_MODS_PATH "XV2INS/Installed"

enum TopQuestTypeIndex
{
    TOP_QUEST_CTP,
    TOP_QUEST_TMQ,
    TOP_QUEST_HLQ,
    TOP_QUEST_BAQ,
    TOP_QUEST_TCQ,
    TOP_QUEST_CHQ,
    TOP_QUEST_OSQ,
    TOP_QUEST_TPQ,
    TOP_QUEST_RBQ,
    TOP_QUEST_LEQ,
    TOP_QUEST_TTQ,
    TOP_QUEST_TNB,
    TOP_QUEST_TFB,
    TOP_QUEST_PRB,

    TOP_QUEST_MAX
};

enum ContextMenuTypes
{
    CTX_MENU_DECOMPILE = 0x300,
    CTX_MENU_COMPILE
};

static const std::vector<std::string> quest_files =
{
    "TPQ/tpq_data.qxd",
    "TMQ/tmq_data.qxd",
    "HLQ/hlq_data.qxd",
    "BAQ/baq_data.qxd",
    "TCQ/tcq_data.qxd",
    "CHQ/chq_data.qxd",
    "OSQ/osq_data.qxd",
    "TPQ/tpq_data.qxd",
    "RBQ/rbq_data.qxd",
    "LEQ/leq_data.qxd",
    "TTQ/ttq_data.qxd",
    "TNB/tnb_data.qxd",
    "TFB/tfb_data.qxd",
    "PRB/prb_data.qxd"
};

static const std::vector<std::string> quest_titles =
{
    "qs_title",
    "qe_title",
    "qh_title",
    "qb_title",
    "qt_title",
    "qch_title",
    "qo_title",
    "qs_title",
    "qr_title",
    "ql_title",
    "ttq_title",
    "tnb_title",
    "tfb_title",
    "prb_title"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    decompile_action = nullptr;
    compile_action = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

static bool mod_visitor(const std::string &path, bool, void *param)
{
    Xv2QuestCompiler *qc = (Xv2QuestCompiler *)param;

    if (Utils::EndsWith(path, ".x2d", false))
    {
        XQ_X2mMod mod;
        X2mFile x2m;

        if (!x2m.LoadFromFile(path))
            return true;

        mod.path = path;
        mod.name = x2m.GetModName();
        mod.type = x2m.GetType();

        if (mod.type == X2mType::NEW_CHARACTER)
        {
            mod.code = x2m.GetEntryName();
            CmsEntry *cms = game_cms->FindEntryByName(mod.code);

            if (!cms)
                return true;

            mod.id = cms->id;
        }
        else if (mod.type == X2mType::NEW_SKILL)
        {
            mod.code = x2m.GetSkillEntry().name;

            std::vector<CusSkill *> skills;

            if (x2m.GetSkillType() == X2mSkillType::SUPER)
            {
                game_cus->FindSuperSkillByName(mod.code, skills);
            }

            else if (x2m.GetSkillType() == X2mSkillType::ULTIMATE)
            {
                game_cus->FindUltimateSkillByName(mod.code, skills);
            }

            else if (x2m.GetSkillType() == X2mSkillType::EVASIVE)
            {
                game_cus->FindEvasiveSkillByName(mod.code, skills);
            }

            else if (x2m.GetSkillType() == X2mSkillType::BLAST)
            {
                game_cus->FindBlastSkillByName(mod.code, skills);
            }

            else if (x2m.GetSkillType() == X2mSkillType::AWAKEN)
            {
                game_cus->FindAwakenSkillByName(mod.code, skills);
            }

            mod.id = 0xFFFFFFFF;

            for (CusSkill *skill : skills)
            {
                CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(skill->id2 / 10));
                if (!cms || cms->unk_10 != X2M_DUMMY_ID)
                    continue;

                std::string ini_path = Utils::MakePathString(X2mFile::GetSkillDirectory(*skill, x2m.GetSkillType()), X2M_SKILL_INI);
                IniFile ini;
                std::string guid_str;

                if (!xv2fs->LoadFile(&ini, ini_path))
                    continue;

               if (!ini.GetStringValue("General", "GUID", guid_str))
                   continue;

                if (Utils::ToLowerCase(guid_str) != Utils::ToLowerCase(x2m.GetModGuid()))
                    continue;

                mod.id = skill->id;
                break;
            }

            if (mod.id == 0xFFFFFFFF)
                return true;
        }
        else if (mod.type == X2mType::NEW_COSTUME)
        {
            X2mCostumeEntry *costume = game_costume_file->FindCostume(x2m.GetModGuid());
            if (!costume)
                return true;

            mod.costume = *costume;
        }
        else if (mod.type == X2mType::NEW_STAGE)
        {
            mod.code = x2m.GetStageDef().code;

            size_t id;
            Xv2Stage *stage = game_stage_def->GetStageByCode(mod.code, &id);

            if (!stage)
                return true;

            mod.id = (uint32_t)id;
        }
        else if (mod.type == X2mType::NEW_QUEST)
        {
            // TODO
            return true;
        }

        qc->PushMod(Utils::ToLowerCase(x2m.GetModGuid()), mod);
    }

    return true;
}

bool MainWindow::Initialize()
{
    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    CreateMutexA(nullptr, FALSE, "XV2QUIMP_INSTANCE");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        UPRINTF("An instance of the XV2 Quest Importer is already running.\n");
        return false;
    }

    HANDLE handle = CreateMutexA(nullptr, FALSE, "XV2INSTALLER_INSTANCE");
    bool installer_opened = (GetLastError() == ERROR_ALREADY_EXISTS);
    if (handle)
        CloseHandle(handle);

    if (installer_opened)
    {
        UPRINTF("XV2 Quest Importer cannot run at the same time than the XV2 Mods Installer.\n");
        return false;
    }

    config.Load();

    if (!config.ConfigureRequirements())
    {
        DPRINTF("ConfigureRequirements failed.\n");
        return false;
    }

    Xenoverse2::InitFs(Utils::QStringToStdString(config.game_directory));

    if (!Xenoverse2::InitSystemFiles())
    {
        DPRINTF("InitSystemFiles failed.\n");
        return false;
    }

    if (!Xenoverse2::InitPreBaked())
    {
        DPRINTF("InitPreBaked failed.\n");
        return false;
    }

    if (!Xenoverse2::InitIdb(true, true, true, true, true, true))
    {
        DPRINTF("InitIdb failed.\n");
        return false;
    }

    if (!Xenoverse2::InitCharaNames())
    {
        DPRINTF("InitCharaNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitCharaCostumeNames())
    {
        DPRINTF("InitCharaCostumeNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitSkillNames())
    {
        DPRINTF("InitSkillNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitTalismanNames())
    {
        DPRINTF("InitTalismanNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitCacCostumeNames())
    {
        DPRINTF("InitCacCostumeNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitCostumeFile())
    {
        DPRINTF("InitCostumeFile failed.\n");
        return false;
    }

    if (!Xenoverse2::InitMaterialNames())
    {
        DPRINTF("InitMaterialNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitBattleNames())
    {
        DPRINTF("InitBattleNames failed.\n");
        return false;
    }

    if (!Xenoverse2::InitLobby())
    {
        DPRINTF("InitLobby failed.\n");
        return false;
    }

    if (!Xenoverse2::InitLobbyText())
    {
        DPRINTF("InitLobbyText failed.\n");
        return false;
    }

    Utils::VisitDirectory(Utils::GetAppDataPath(INSTALLED_MODS_PATH), true, false, false, mod_visitor, &qc);

    for (int i = 0; i < TOP_QUEST_MAX; i++)
    {
        if (!LoadQuests(i))
            return false;
    }

    return true;
}

bool MainWindow::ProcessShutdown()
{
    config.Save();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ProcessShutdown())
        event->accept();
    else
        event->ignore();
}

bool MainWindow::LoadQuests(int type)
{
    QxdFile qxd;
    MsgFile msg;

    const std::string qpath = "data/quest/" + quest_files[(size_t)type];
    QTreeWidgetItem *top = ui->questTreeWidget->topLevelItem(type);

    if (!xv2fs->LoadFile(&qxd, qpath))
    {
        DPRINTF("Failed to load %s.\n", qpath.c_str());
        return false;
    }

    const std::string mpath = "data/msg/" + quest_titles[(size_t)type] + "_en.msg";

    if (!xv2fs->LoadFile(&msg, mpath))
    {
        DPRINTF("Failed to load %s.\n", mpath.c_str());
        return false;
    }

    for (size_t i = 0; i < qxd.GetNumQuests(); i++)
    {
        const QxdQuest &quest = qxd.GetQuest(i);

        if (quest.update_requirement == QXD_UPDATE_DEVELOPER || Utils::BeginsWith(quest.name, "empty", false))
            continue;

        if (type == TOP_QUEST_CTP && !Utils::BeginsWith(quest.name, "CTP", false))
            continue;
        else if (type == TOP_QUEST_TPQ && !Utils::BeginsWith(quest.name, "TPQ", false))
            continue;

        std::string text = quest.name;

        if (quest.msg_entries.size() > 0)
        {
            MsgEntry *msg_entry = msg.FindEntryByName(quest.msg_entries[0]);

            if (msg_entry && msg_entry->lines.size() > 0 && msg_entry->lines[0].length() > 0)
            {
                text += " (" + Xenoverse2::UnescapeHtml(msg_entry->lines[0]) + ")";
            }
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(top, 1);
        item->setText(0, Utils::StdStringToQString(text, false));
        item->setData(0, Qt::UserRole, QVariant(Utils::StdStringToQString(quest.name, false)));
    }

    return true;
}

static int quest_counter = 0;

static const std::unordered_set<std::string> test_black_list =
{
    // These quests are allocated but don't have files
    "TFB_5200",
    "TFB_5300",
    "TFB_5400",
    "TFB_5500",
    "TFB_5600",
    "TFB_5700",
    "TFB_5800",
    "TFB_5900",
    "TFB_6000",
    "TFB_6100",
    "TFB_6200",
    "TFB_6300",
    "TFB_6400",
    "TFB_6500",
    "TFB_6600",
    "TFB_6700",
    "TFB_6800",
    "TFB_6900",
    "TFB_7000",
    "TFB_7100",
    "FEP_00_00",
    "FEP_00_01",
};

void quest_compiler_test_multi(Xv2QuestCompiler &comp, const QxdFile &qxd, const std::string &base_path)
{
    std::vector<QxdQuest> quests = qxd.GetQuests(); // Copy, since recompile may add quest

    for (QxdQuest &quest : quests)
    {
        if (quest.update_requirement == QXD_UPDATE_DEVELOPER || Utils::BeginsWith(quest.name, "empty", false))
            continue;

        if (test_black_list.find(quest.name) != test_black_list.end())
            continue;

        QmlFile qml;
        QbtFile qbt;
        QslFile qsl;
        std::vector<QedFile> qeds;

        std::string path = Utils::MakePathString(base_path, quest.name);
        std::ostringstream qoss, coss, doss, poss;
        std::vector<std::string> svec, sfvec;

        std::string base_qpath = "data/quest/";
        if (Utils::BeginsWith(quest.name, "CTP_", false))
        {
            base_qpath += "TPQ";
        }
        else if (Utils::BeginsWith(quest.name, "L_RBQ_", false))
        {
            base_qpath += "RBQ";
        }
        else
        {
            base_qpath += quest.name.substr(0, 3);
        }

        //printf("%s\n", base_qpath.c_str());

        base_qpath.push_back('/');
        base_qpath += quest.name;
        base_qpath.push_back('/');
        base_qpath += quest.name;

        if (!xv2fs->LoadFile(&qml, base_qpath + ".qml"))
        {
            DPRINTF("Failed to load qml %s (quest: %s)\n", base_qpath.c_str(), quest.name.c_str());
            exit(-1);
        }

        if (!xv2fs->LoadFile(&qbt, base_qpath + ".qbt"))
        {
            DPRINTF("Failed to load qbt %s\n", base_qpath.c_str());
            exit(-1);
        }

        if (!xv2fs->LoadFile(&qsl, base_qpath + ".qsl"))
        {
            DPRINTF("Failed to load qsl %s\n", base_qpath.c_str());
            exit(-1);
        }

        //DPRINTF("Processing %s\n", quest.name.c_str());
        if (!comp.DecompileQuest(quest.name, qoss, coss, doss, poss, svec))
        {
            DPRINTF("Failed to decompile %s\n", quest.name.c_str());
            exit(-1);
        }

        std::string qbuf = qoss.str();

        if (!Utils::WriteFileBool(Utils::MakePathString(path, "quest.x2qs"), (const uint8_t *)qbuf.c_str(), qbuf.length(), true, true))
        {
            DPRINTF("Write file failed.\n");
            exit(-1);
        }

        std::string cbuf = coss.str();

        if (!Utils::WriteFileBool(Utils::MakePathString(path, "chars.x2qs"), (const uint8_t *)cbuf.c_str(), cbuf.length(), true, true))
        {
            DPRINTF("Write file failed.\n");
            exit(-1);
        }

        std::string dbuf = doss.str();

        if (!Utils::WriteFileBool(Utils::MakePathString(path, "dialogue.x2qs"), (const uint8_t *)dbuf.c_str(), dbuf.length(), true, true))
        {
            DPRINTF("Write file failed.\n");
            exit(-1);
        }

        std::string pbuf = poss.str();

        if (!Utils::WriteFileBool(Utils::MakePathString(path, "positions.x2qs"), (const uint8_t *)pbuf.c_str(), pbuf.length(), true, true))
        {
            DPRINTF("Write file failed.\n");
            exit(-1);
        }

        sfvec.resize(svec.size());
        qeds.resize(svec.size());
        for (size_t i = 0; i < svec.size(); i++)
        {
            std::string spath = "script";

            if (i > 0)
                spath += Utils::ToString(i);

            spath += ".x2qs";
            sfvec[i] = spath;

            if (!Utils::WriteFileBool(Utils::MakePathString(path, spath), (const uint8_t *)svec[i].c_str(), svec[i].length()))
            {
                DPRINTF("Write file failed.\n");
                exit(-1);
            }

            // Load of qed for comparison
            std::string qed_path = base_qpath;

            if (i > 0)
            {
                qed_path.push_back('_');
                qed_path += Utils::ToString(i);
            }

            if (!xv2fs->LoadFile(&qeds[i], qed_path + ".qed"))
            {
                DPRINTF("Failed to load qed %s\n", qed_path.c_str());
                exit(-1);
            }
        }

        if (!comp.CompileQuest(qbuf, "quest.x2qs", cbuf, "chars.x2qs", dbuf, "dialogue.x2qs", pbuf, "positions.x2qs", svec, sfvec))
        {
            DPRINTF("Compile failed at %s.\n", quest.name.c_str());
            exit(-1);
        }

        if (qxd != comp.DebugGetActiveQxd())
        {
            comp.DebugGetActiveQxd().SaveToFile("fail.qxd");
            DPRINTF("QXD comparison failed at %s.\n", quest.name.c_str());
            exit(-1);
        }

        if (!qml.SpecialCompare(comp.DebugGetActiveQml(), comp.DebugGetActiveQxd()))
        {
            comp.DebugGetActiveQml().SaveToFile("fail.qml");
            DPRINTF("QML comparison failed at %s.\n", quest.name.c_str());
            exit(-1);
        }

        if (!qbt.PartialCompare(comp.DebugGetActiveQbt()))
        {
            comp.DebugGetActiveQbt().SaveToFile("fail.qbt");
            DPRINTF("QBT comparison failed at %s.\n", quest.name.c_str());
            exit(-1);
        }

        if (!qsl.SpecialCompare(comp.DebugGetActiveQsl()))
        {
            comp.DebugGetActiveQsl().SaveToFile("fail.qsl");
            DPRINTF("QSL comparison failed at %s.\n", quest.name.c_str());
            exit(-1);
        }

        for (size_t i = 0; i < qeds.size(); i++)
        {
            if (qeds[i] != *comp.DebugGetActiveQed(i))
            {
                DPRINTF("Comparison failed in qed %Id (quest %s).\n", i, quest.name.c_str());
                comp.DebugGetActiveQed(i)->SaveToFile("fail.qed");
                exit(-1);
            }
        }

        comp.DebugDumpTitle(Utils::MakePathString(base_path, "TITLE"));
        comp.DebugDumpDialogue(Utils::MakePathString(base_path, "DIALOGUE"));

        quest_counter++;
    }
}

void quest_compiler_test_multi(Xv2QuestCompiler &qc)
{
    std::vector<std::string> qxd_files = { "BAQ/baq_data.qxd", "CHQ/chq_data.qxd", "HLQ/hlq_data.qxd",
                                           "LEQ/leq_data.qxd", "OSQ/osq_data.qxd", "RBQ/rbq_data.qxd",
                                           "TCQ/tcq_data.qxd", "TFB/tfb_data.qxd", "TMQ/tmq_data.qxd",
                                           "TNB/tnb_data.qxd", "TPQ/tpq_data.qxd", "TTQ/ttq_data.qxd",
                                           "PRB/prb_data.qxd" };

    for (const std::string &f : qxd_files)
    {
        std::string qxd_path = "data/quest/" + f;
        std::string out_path = "C:/Users/MUU/basurero/quests/" + f.substr(0, 3);

        QxdFile qxd;

        if (!xv2fs->LoadFile(&qxd, qxd_path))
            return;

        DPRINTF("Processing %s\n", qxd_path.c_str());
        quest_compiler_test_multi(qc, qxd, out_path);
    }

    UPRINTF("Processed %d quests.\n", quest_counter); // 576
}

void MainWindow::on_actionAbout_triggered()
{
    //quest_compiler_test_multi(qc);

    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));


    box.exec();
}

void MainWindow::RemoveActions()
{
    if (compile_action)
    {
        ui->questTreeWidget->removeAction(compile_action);
        compile_action = nullptr;
    }

    if (decompile_action)
    {
        ui->questTreeWidget->removeAction(decompile_action);
        decompile_action = nullptr;
    }
}

bool MainWindow::CheckForLegendPatrol(const QString &name)
{
    if (!name.startsWith("TPQ_", Qt::CaseInsensitive))
        return true;

    if (!xv2fs->FileExists("cpk/data_d4_5_xv1.cpk", false, true))
    {
        DPRINTF("Legend patrol cpk is required to import/export legend patrol quests.\n");
        return false;
    }

    return true;
}

void MainWindow::on_questTreeWidget_itemSelectionChanged()
{
    QTreeWidgetItem *item = ui->questTreeWidget->currentItem();

    if (!item)
    {
        RemoveActions();
        return;
    }

    QString str = item->data(0, Qt::UserRole).toString();

    if (str.isEmpty())
    {
        RemoveActions();
        return;
    }

    if (compile_action)
    {
        compile_action->setData(QVariant(str));
    }
    else
    {
        compile_action = new QAction("Import");
        compile_action->setData(QVariant(str));
        compile_action->setProperty("action_type", QVariant(CTX_MENU_COMPILE));
        ui->questTreeWidget->addAction(compile_action);

        connect(compile_action, SIGNAL(triggered(bool)), this, SLOT(onContextMenu()));
    }

    if (decompile_action)
    {
        decompile_action->setData(QVariant(str));
    }
    else
    {
        decompile_action = new QAction("Export");
        decompile_action->setData(QVariant(str));
        decompile_action->setProperty("action_type", QVariant(CTX_MENU_DECOMPILE));
        ui->questTreeWidget->addAction(decompile_action);

        connect(decompile_action, SIGNAL(triggered(bool)), this, SLOT(onContextMenu()));
    }
}

static std::string compiler_output;

static void CompilerOutput(const char *dbg)
{
    compiler_output += dbg;
}

void MainWindow::Compile(const QString &quest)
{
    if (!CheckForLegendPatrol(quest))
        return;

    QString dir = QFileDialog::getExistingDirectory(this, "Select directory", config.ld_quest_import);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::QStringToStdString(dir);
    config.ld_quest_import = dir;

    std::string qs, cs, ds, ps;
    std::vector<std::string> svec, sfvec;

    if (!Utils::ReadTextFile(Utils::MakePathString(dir_std, "quest.x2qs"), qs))
        return;

    if (!Utils::ReadTextFile(Utils::MakePathString(dir_std, "chars.x2qs"), cs))
        return;

    if (!Utils::ReadTextFile(Utils::MakePathString(dir_std, "dialogue.x2qs"), ds))
        return;

    if (!Utils::ReadTextFile(Utils::MakePathString(dir_std, "positions.x2qs"), ps))
        return;

    for (int i = 0; i < 4; i++)
    {
        std::string spath = Utils::MakePathString(dir_std, "script");
        std::string script;

        if (i != 0)
            spath += Utils::ToString(i);

        spath += ".x2qs";

        if (!Utils::ReadTextFile(spath, script, (i == 0)))
        {
            if (i == 0)
                return;

            break;
        }

        svec.push_back(script);
        sfvec.push_back(Utils::GetFileNameString(spath));
    }

    qc.SetResourceLoad(dir_std, nullptr);
    qc.SetTestMode(true);

    compiler_output.clear();
    redirect_dprintf(CompilerOutput);
    bool ret = qc.CompileQuest(qs, "quest.x2qs", cs, "chars.x2qs", ds, "dialogue.x2qs", ps, "positions.x2qs", svec, sfvec);
    redirect_dprintf((RedirectFunc)nullptr);

    if (!ret)
    {
        if (compiler_output.length() > 0)
        {
            DPRINTF("%s", compiler_output.c_str());
        }

        DPRINTF("Compilation of quest failed!\n");
        return;
    }

    std::string quest_std = Utils::QStringToStdString(quest, false);
    std::string compiled_quest = qc.GetCompiledQuestName();

    if (quest_std != compiled_quest)
    {
        DPRINTF("The quest to import must be called the same!\n"
                "Was expecting \"%s\" but got \"%s\".\n", quest_std.c_str(), compiled_quest.c_str());
        return;
    }

    qc.SetTestMode(false);

    if (!qc.CompileQuest(qs, "quest.x2qs", cs, "chars.x2qs", ds, "dialogue.x2qs", ps, "positions.x2qs", svec, sfvec))
    {
        DPRINTF("Quest import failed!\n");
        return;
    }

    if (!qc.Maintenance())
        return;

    if (!qc.CommitCompiledFiles())
        return;

    if (!qc.CommitTitle())
        return;

    if (!qc.CommitDialogue())
        return;

    if (!qc.CommitDialogueAudio())
        return;

    if (!qc.CommitActiveQxd())
        return;

    UPRINTF("Quest imported succesfully.\n");
}

void MainWindow::Decompile(const QString &quest)
{
    if (!CheckForLegendPatrol(quest))
        return;

    QString dir = QFileDialog::getExistingDirectory(this, "Select directory", config.ld_quest_export);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::QStringToStdString(dir);
    config.ld_quest_export = dir;

    if (!Utils::IsDirectoryEmpty(dir_std, true))
    {
        DPRINTF("Please, select an empty directory. That one is not.\n");
        return;
    }

    std::string quest_std = Utils::QStringToStdString(quest, false);

    std::ostringstream qoss, coss, doss, poss;
    std::vector<std::string> svec;

    if (!qc.DecompileQuest(quest_std, qoss, coss, doss, poss, svec))
    {
        DPRINTF("Quest export failed.\n");
        return;
    }

    if (!Utils::WriteFileBool(Utils::MakePathString(dir_std, "quest.x2qs"), (const uint8_t *)qoss.str().c_str(), qoss.str().length()))
    {
        DPRINTF("Write file failed (quest.x2qs).\n");
        return;
    }

    if (!Utils::WriteFileBool(Utils::MakePathString(dir_std, "chars.x2qs"), (const uint8_t *)coss.str().c_str(), coss.str().length()))
    {
        DPRINTF("Write file failed (chars.x2qs).\n");
        return;
    }

    if (!Utils::WriteFileBool(Utils::MakePathString(dir_std, "dialogue.x2qs"), (const uint8_t *)doss.str().c_str(), doss.str().length()))
    {
        DPRINTF("Write file failed (dialogue.x2qs).\n");
        return;
    }

    if (!Utils::WriteFileBool(Utils::MakePathString(dir_std, "positions.x2qs"), (const uint8_t *)poss.str().c_str(), poss.str().length()))
    {
        DPRINTF("Write file failed (positions.x2qs).\n");
        return;
    }

    for (size_t i = 0; i < svec.size(); i++)
    {
        std::string file = "script";

        if (i != 0)
        {
            file += Utils::ToString(i);
        }

        file += ".x2qs";

        if (!Utils::WriteFileBool(Utils::MakePathString(dir_std, file), (const uint8_t *)svec[i].c_str(), svec[i].length()))
        {
            DPRINTF("Write file failed (%s).\n", file.c_str());
            return;
        }
    }

    UPRINTF("Export completed succesfully.\n");
}

void MainWindow::onContextMenu()
{
    QAction *action = dynamic_cast<QAction *>(QObject::sender());
    if (!action)
        return;

    if (action->property("action_type") == CTX_MENU_DECOMPILE)
        Decompile(action->data().toString());

    else if (action->property("action_type") == CTX_MENU_COMPILE)
        Compile(action->data().toString());
}


