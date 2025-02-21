#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFileDialog>

#include "initializer/demloader.h"
#include "models/Plane.h"
#include "models/Terrain.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Scene * scene = ui->viewport->getScene();

    Phong * phong = new Phong();
    Plane * terrain = new Plane(phong);
    scene->materials.push_back(phong);
    scene->models.push_back(terrain);

    ui->viewport->initScene();
    ui->viewport->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadMeshButton_clicked()
{
    QString selected_file = QFileDialog::getOpenFileName(this, "Open mesh file...", {}, "Digital elevation model (*.dem)");

    if (selected_file.isEmpty())
        return;

    loadFile(selected_file);
}

void MainWindow::loadFile(QString fileName)
{
    DemLoader demLoader(fileName);
    Displacement * dismap = demLoader.getDisplacement();

    Plane *terrain = new Plane(dismap);

    Scene * scene = ui->viewport->getScene();

    for (auto material: scene->materials) { delete material; }
    for (auto model: scene->models) { delete model; }

    scene->materials.clear();
    scene->models.clear();

    scene->materials.push_back(dismap);
    scene->models.push_back(terrain);

    ui->viewport->initScene();
    ui->viewport->update();
}

