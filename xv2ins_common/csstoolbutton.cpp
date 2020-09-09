#include "csstoolbutton.h"

#include "Utils.h"
#include "debug.h"

#define DRAG_PREFIX "[CSS_SLOT]"

CssToolButton::CssToolButton(QWidget *parent) : QToolButton(parent)
{
    setAcceptDrops(true);
}

void CssToolButton::mousePressEvent(QMouseEvent *event)
{
    QToolButton::mousePressEvent(event);

    if (event->button() == Qt::RightButton || event->button() == Qt::MiddleButton)
    {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(DRAG_PREFIX + QString("%1").arg(property("slot_index").toInt()));
        drag->setMimeData(mimeData);


        drag->exec();
    }

    event->accept();
}

void CssToolButton::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain") && event->mimeData()->text().startsWith(DRAG_PREFIX))
        event->acceptProposedAction();
}

void CssToolButton::dropEvent(QDropEvent *event)
{
    QString text = event->mimeData()->text();

    if (!text.startsWith(DRAG_PREFIX))
        event->ignore();

    int src = text.mid((int)strlen(DRAG_PREFIX)).toInt();
    int dst = property("slot_index").toInt();

    //printf("%d to %d\n", src, dst);

    if (src == dst)
        return;

    drag_src = src;
    drop_dst = dst;

    QMenu *menu = new QMenu(this);
    QAction *swap_action = new QAction("Swap with this", menu);
    QAction *mb_action = new QAction("Move before this",menu);
    QAction *ma_action = new QAction("Move after this", menu);
    QAction *merge_action = new QAction("Merge with this", menu);

    menu->addAction(swap_action);
    menu->addAction(mb_action);
    menu->addAction(ma_action);
    menu->addAction(merge_action);
    menu->addAction("Cancel");

    menu->popup(mapToGlobal(QPoint(width()/2, height()/2)));

    connect(swap_action, SIGNAL(triggered(bool)), this, SLOT(onDropMenuSwap()));
    connect(mb_action, SIGNAL(triggered(bool)), this, SLOT(onDropMenuMoveBefore()));
    connect(ma_action, SIGNAL(triggered(bool)), this, SLOT(onDropMenuMoveAfter()));
    connect(merge_action, SIGNAL(triggered(bool)), this, SLOT(onDropMenuMerge()));

    event->acceptProposedAction();
}

void CssToolButton::onDropMenuSwap()
{
    emit swapSlot(drag_src, drop_dst);
}

void CssToolButton::onDropMenuMoveBefore()
{
    emit moveSlot(drag_src, drop_dst);
}

void CssToolButton::onDropMenuMoveAfter()
{
    emit moveSlot(drag_src, drop_dst+1);
}

void CssToolButton::onDropMenuMerge()
{
    emit mergeSlot(drag_src, drop_dst);
}
