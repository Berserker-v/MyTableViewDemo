#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QLabel>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QTableView>



class MyTableView : public QTableView
{
    Q_OBJECT

public:
    MyTableView(QWidget *parent = Q_NULLPTR);

    void SetModel(QStandardItemModel *model);

signals:
    void sigRowChange(int from, int to);

protected:
    //鼠标事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //拖放事件
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void DoDrag();                      //执行拖放
    void ResetOrder();                  //更新表格序号
    void MoveRow(int from, int to);     //移动行
    int  SumOffset();                   //计算像素补偿：用于解决当顶部未显示一整行时候产生的像素偏差

private:
    QStandardItemModel *mModel;
    QLabel *mLabel;                     //指示线
    int mRowHeight;                     //行高

    bool mValidPress;                   //判断移动之前是否点击有效行
    int  mRowFrom;                      //拖放起始行
    int  mRowTo;                        //拖放目标行

    QStringList mDragText;              //拖放过程中随鼠标显示内容
    QPoint mDragPoint;                  //拖放起点
    QPoint mDragPointAtItem;            //
};

#endif // MYTABLEVIEW_H
