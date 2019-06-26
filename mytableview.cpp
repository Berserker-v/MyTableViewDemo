#include "mytableview.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QtDebug>

MyTableView::MyTableView(QWidget *parent)
    : QTableView (parent)
{
    setAcceptDrops(true);

    mLabel = new QLabel(this);
    mLabel->setFixedHeight(2);
    mLabel->setGeometry(0, 1, width(), 2);
    mLabel->setStyleSheet("border: 1px solid #8B7500");
    mLabel->hide();

    mModel = nullptr;
    mSelectModel = nullptr;
    mRowHeight = 30;
    mValidPress = false;
    mRowFrom = 0;
    mRowTo = 0;
}

void MyTableView::addRow()
{
    int curRow = mSelectModel->currentIndex().row();
    mModel->insertRow(curRow+1);
}

void MyTableView::delRow()
{
    int curRow = mSelectModel->currentIndex().row();
    mModel->removeRow(curRow);
}

void MyTableView::clearRow()
{
    mModel->removeRows(0,mModel->rowCount());
}

void MyTableView::SetModel(QStandardItemModel *model)
{
    mModel = model;
    QTableView::setModel(model);
}

void MyTableView::SetSelectionModel(QItemSelectionModel *selectmodel)
{
    mSelectModel = selectmodel;
    QTableView::setSelectionModel(selectmodel);
}

void MyTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        QModelIndex topIndex = indexAt(QPoint(30,0));
        int pixelOffset = SumOffset();
        //每次点击清除上次保存List
        mDragText.clear();
        if (index.isValid()) {
            mValidPress = true;
            mDragPoint = event->pos();

            for ( int i=0; i<mModel->columnCount(); ++i) {
                if (mModel->item(index.row(),i)) {
                    mDragText.append(mModel->item(index.row(),i)->text());
                }
            }
            int y = (index.row() - topIndex.row())*mRowHeight - pixelOffset;
            mDragPointAtItem = mDragPoint - QPoint(0, y);
            mRowFrom = index.row();
        }
        else {
            mValidPress = false;
        }
    }
    //将其与操作交给继承对象
    QTableView::mousePressEvent(event);
}

int MyTableView::SumOffset()
{
    QModelIndex index = indexAt(QPoint(15,0));
    int topRow = index.row();
    for (int colpixel = 0; colpixel < mRowHeight; ++colpixel) {
        index = indexAt(QPoint(15,colpixel));
        if (index.row() - topRow) {
            return  (mRowHeight-colpixel);
        }
    }
    return 0;
}

void MyTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (!mValidPress) {
        return;
    }
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - mDragPoint).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    mLabel->show();

    DoDrag();

    mLabel->hide();
    mValidPress = false;
}

void MyTableView::DoDrag()
{
    QDrag *drag = new QDrag(this);
    QMimeData *mimedata = new QMimeData;
    mimedata->setText("mDragText");
    drag->setMimeData(mimedata);

    //设置拖动时阴影图像追送
    QPixmap drag_img(width(), mRowHeight);
    drag_img.fill(QColor(255,255,255,100));
    QPainter painter(&drag_img);
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(QRectF(40,0,width(),mRowHeight), mDragText.join("      "), QTextOption(Qt::AlignVCenter));

    drag->setPixmap(drag_img);
    drag->setHotSpot(mDragPointAtItem);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        selectionModel()->clearSelection();
    }
}

void MyTableView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
    else {
        event->ignore();
        QTableView::dragEnterEvent(event);
    }
}

void MyTableView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasText()) {
        int nCurRow = 0;
        QModelIndex index = indexAt(event->pos());      //鼠标拖动位置表格索引
        QModelIndex topIndex = indexAt(QPoint(30,0));   //当前视图顶部表格索引
        int pixelOffset = SumOffset();
        int y = 0;
        if (index.isValid()) {
            y = (index.row() - topIndex.row() )*mRowHeight - pixelOffset;
            if (event->pos().y() - y >= mRowHeight/2) {
                nCurRow = index.row() + 1;
            }
            else {
                nCurRow = index.row();
            }
        }
        else {
            nCurRow = mModel->rowCount();
        }

        mRowTo = nCurRow;
        y = (mRowTo - topIndex.row() +1)*mRowHeight - pixelOffset;
        mLabel->setGeometry(1, y, width()-2, 2);
        event->acceptProposedAction();
        return;
    }

    event->ignore();
    QTableView::dragMoveEvent(event);
}

void MyTableView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        if (mRowTo != mRowFrom) {
            MoveRow(mRowFrom, mRowTo);
            mDragText.clear();
        }
        event->acceptProposedAction();
        return;
    }

    event->ignore();
    QTableView::dropEvent(event);
}

void MyTableView::ResetOrder()
{
    for (int i = 0; i < mModel->rowCount(); i++) {
        mModel->item(i, 0)->setText( QString::number(i + 1) );
        mModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }
}

void MyTableView::MoveRow(int from, int to)
{
    if (from == to) {
        return;
    }
    QStandardItem *item = mModel->item(from, 1);
    if (item) {
        QString strText = item->text();

        QList<QStandardItem*> items;
        QStandardItem *item = nullptr;

        for (int i=0; i<mModel->columnCount(); ++i) {
            item = new QStandardItem(mDragText.at(i));
            items.append(item);
        }

        mModel->insertRow(to, items);
        if (from < to) {
            mModel->removeRow(from);
            selectRow(to-1);
        }
        else {
            mModel->removeRow(from+1);
            selectRow(to);
        }
        ResetOrder();
        emit sigRowChange(mRowFrom, mRowTo);
    }
}
























