#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGVScene.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void nodeContextMenu(QGVNode* node);
    void nodeDoubleClick(QGVNode* node);

private:
    Ui::MainWindow *ui;
    QGVScene *_scene;
};

#endif // MAINWINDOW_H
