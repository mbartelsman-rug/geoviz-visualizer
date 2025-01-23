#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadMeshButton_clicked()
{
    QString selected_file = QFileDialog::getOpenFileName(this, "Open mesh file...", {}, "Wavefront files (*.obj)");

    if (selected_file.isEmpty())
        return;

    // ui->viewport->loadMesh(selected_file);
}

