#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFileDialog>

#include "initializer/demloader.h"
#include "models/Plane.h"

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
    QString selected_file = QFileDialog::getOpenFileName(this, "Open mesh file...", {}, "Wavefront files (*.dem)");

    if (selected_file.isEmpty())
        return;

    DemLoader demLoader(selected_file);

    Displacement *dismap = demLoader.getDisplacement();

    // TODO

    // ui->viewport->loadMesh(selected_file);

    ui->viewport->update();

}

