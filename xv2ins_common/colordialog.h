#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOption>
#include <QMenu>
#include <QRgb>
#include <QTimer>

#include "Utils.h"

namespace Ui {
class ColorDialog;
}

class ColorButton;

class ColorDialog : public QDialog
{
    Q_OBJECT

    friend class ColorButton;

public:
    explicit ColorDialog(const std::vector<uint32_t> &colors_list, uint16_t selected_color, QWidget *parent=nullptr, bool disable_no_color=false);
    ~ColorDialog();

    uint16_t GetSelection() { return selection; }

private slots:
    void on_noColorCheck_clicked();

private:
    Ui::ColorDialog *ui;
    uint16_t selection;
    uint16_t previous_selection;

    QVector<ColorButton *> buttons;
};

class ColorButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ColorButton(uint32_t color, QWidget *parent);

    bool eventFilter(QObject* object, QEvent* event);

private slots:
    void onFrameUpdate();

private:
    QTimer *timer;
    uint32_t animation_index = 0;
    bool direction_up;
};

#endif // COLORDIALOG_H
