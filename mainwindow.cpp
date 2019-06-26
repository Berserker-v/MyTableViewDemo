#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytableview.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitTable();
    /*----------------Printf Information-----------------*/
    qDebug() << ui->tableView->dragDropMode();
    /*---------------------------------------------------*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitTable()
{
    MyTableView* t = ui->tableView;

    //设置各种属性
    t->verticalHeader()->hide();                            //隐藏行头
    t->verticalHeader()->setDefaultSectionSize(30);         //默认列高度
    t->horizontalHeader()->setStretchLastSection(true);     //最后一列自适应宽度
    t->setEditTriggers(QTableView::DoubleClicked);          //双击编辑
    t->setSelectionBehavior(QTableView::SelectRows);		//一次选中整行
    t->setSelectionMode(QTableView::SingleSelection);       //单行选中
    t->setAlternatingRowColors(true);                       //行间隔色
    t->setDragDropMode(QAbstractItemView::DropOnly);        //使能表格drop

    //添加数据
//    QStandardItemModel *model = new QStandardItemModel(3,2,this);
    QStandardItemModel *model = new QStandardItemModel();
    QItemSelectionModel *selectmodel = new QItemSelectionModel(model);

    t->horizontalHeader()->setMinimumHeight(30);

    model->setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("序号")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("代号")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("任务")));

    QStringList mark, mission;
    mark<<"111"<<"222"<<"333"<<"444"<<"555"<<"AAA"<<"BBB"<<"CCC"<<"DDD"<<"EEE";
    mission<<"0x00"<<"0x01"<<"0x02"<<"0x03"<<"0x04"<<"0x05"<<"0x06"<<"0x07"<<"0x08"<<"0x09";
    for(int i=0;i<mark.size();i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::number(i+1)));
        model->setItem(i, 1, new QStandardItem(mark[i]));
        model->setItem(i, 2, new QStandardItem(mission[i]));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }
    t->SetModel(model);
    t->SetSelectionModel(selectmodel);
    //添加右键菜单
    QList<QAction*> actions;
    actions << ui->addRow << ui->delRow << ui->clearTable;
    addRightMenu(ui->tableView, actions);
    //设置样式
    t->setStyleSheet("QTableView {border: 1px solid gray;background: #E8E8E8;}\
                      QTableView::item{color:black;}\
                      QTableView::item:selected{color:black;background: #63B8FF;}");

    t->setColumnWidth(0,80);
//    t->selectRow(0);
}

void MainWindow::addRightMenu(MyTableView *table, const QList<QAction*> &actions)
{
    table->setContextMenuPolicy(Qt::ActionsContextMenu);
    table->addActions(actions);
}

void MainWindow::on_addRow_triggered()
{
    ui->tableView->addRow();
}

void MainWindow::on_delRow_triggered()
{
    ui->tableView->delRow();
}

void MainWindow::on_clearTable_triggered()
{
    ui->tableView->clearRow();
}



















