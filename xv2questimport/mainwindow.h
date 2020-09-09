#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "Xv2QuestCompiler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool Initialize();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionAbout_triggered();
    void on_questTreeWidget_itemSelectionChanged();

    void onContextMenu();

private:
    Ui::MainWindow *ui;

    Xv2QuestCompiler qc;
    QAction *compile_action;
    QAction *decompile_action;

    bool ProcessShutdown();

    bool LoadQuests(int type);
    void RemoveActions();

    bool CheckForLegendPatrol(const QString &name);

    void Compile(const QString &quest);
    void Decompile(const QString &quest);
};

#endif // MAINWINDOW_H
