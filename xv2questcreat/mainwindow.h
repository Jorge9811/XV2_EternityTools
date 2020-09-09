#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include "X2mFile.h"
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
    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_guidButton_clicked();

    void on_guidCopyButton_clicked();

    void on_questFilesButton_clicked();

    void on_audioFilesButton_clicked();

    void on_atmodsButton_clicked();

    void on_addDataButton_clicked();

private:
    Ui::MainWindow *ui;
    X2mFile *x2m;
    Xv2QuestCompiler qc;

    bool show_wait_onsave=false;

    bool ProcessShutdown();

    void ProcessX2m();
    bool Validate();
    bool Build();
};

#endif // MAINWINDOW_H
