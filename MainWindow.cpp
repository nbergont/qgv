#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QGVScene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _scene = new QGVScene("TEST", this);
    //_scene->loadLayout("digraph simple{1 -> 2;2 -> 1;}");

    _scene->setGraphAttribute("splines", "ortho");
    _scene->setGraphAttribute("rankdir", "LR");
    //_scene->setGraphAttribute("concentrate", "true");
    _scene->setGraphAttribute("nodesep", "0.4");

    _scene->setNodeAttribute("shape", "box");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    _scene->setNodeAttribute("height", "1");

    _scene->setEdgeAttribute("minlen", "3");

    QGVNode *node1 = _scene->addNode("DSA");
    QGVNode *node2 = _scene->addNode("COT");
    QGVNode *node3 = _scene->addNode("MIT");
    QGVNode *node4 = _scene->addNode("ISS");

    _scene->addEdge(node1, node2, "TTL");
    _scene->addEdge(node1, node2, "SER");
    _scene->addEdge(node1, node3, "RAZ");
    _scene->addEdge(node2, node3, "SECU");

    _scene->addEdge(node2, node4, "STATUS");

    _scene->addEdge(node4, node3, "ACK");

    _scene->addEdge(node4, node2, "ERROR");
    _scene->addEdge(node4, node2, "ETH");
    _scene->addEdge(node4, node2, "RS232");

    _scene->applyLayout();

    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    ui->graphicsView->setScene(_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nodeContextMenu(QGVNode *node)
{
    QMenu menu(node->name());

    menu.addSeparator();
    menu.addAction("Edit");
    menu.addAction("Options");

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;
}
#include <QMessageBox>
void MainWindow::nodeDoubleClick(QGVNode *node)
{
    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->name()));
}
