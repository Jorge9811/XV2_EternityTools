#ifndef CSSTOOLBUTTON_H
#define CSSTOOLBUTTON_H

#include <QToolButton>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMenu>


class CssToolButton : public QToolButton
{
    Q_OBJECT

protected:

    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent * event);

    void dropEvent(QDropEvent *event);

public:
    explicit CssToolButton(QWidget *parent=nullptr);

private slots:

    void onDropMenuSwap();
    void onDropMenuMoveBefore();
    void onDropMenuMoveAfter();
    void onDropMenuMerge();

signals:

    void swapSlot(int src, int dst);
    void moveSlot(int src, int dst);
    void mergeSlot(int src, int dst);

private:

    int drag_src, drop_dst;
};

#endif // CSSTOOLBUTTON_H
