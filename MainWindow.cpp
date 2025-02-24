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
    QString selected_file = QFileDialog::getOpenFileName(this, "Open .dem file...", {}, "Wavefront files (*.dem)");

    if (selected_file.isEmpty())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->viewport->loadDem(selected_file);
    QApplication::restoreOverrideCursor(); // for restoring

    ui->viewport->update();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->viewport->settings.method = (Method) index;
    ui->viewport->updateModels();
}

void MainWindow::on_spinbox_s_valueChanged(double value){
    ui->viewport->settings.s = value;
    ui->viewport->updateModels();
}
void MainWindow::on_spinbox_e_valueChanged(double value){
    ui->viewport->settings.e = value;
    ui->viewport->updateModels();
}

void MainWindow::on_doubleSpinBox_valueChanged(double value) {
    ui->viewport->settings.t = value;
    ui->viewport->updateModels();
}

void MainWindow::on_verticalScaleSpinBox_valueChanged(double value)
{
    ui->viewport->settings.vertical_scale = value;
    ui->viewport->updateModels();
}

