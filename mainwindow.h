#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MyTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitTable();
    void addRightMenu(MyTableView *table, const QList<QAction*> &actions);

private slots:
    void on_addRow_triggered();
    void on_delRow_triggered();
    void on_clearTable_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
