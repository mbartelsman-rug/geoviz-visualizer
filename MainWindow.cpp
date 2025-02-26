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
    ui->viewport->loadDem(selected_file, ui->statusBar);
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

void MainWindow::on_spinbox_t_valueChanged(double value) {
    ui->viewport->settings.t = value;
    ui->viewport->updateModels();
}

void MainWindow::on_verticalScaleSpinBox_valueChanged(double value)
{
    ui->viewport->settings.vertical_scale = value;
    ui->viewport->updateModels();
}


void MainWindow::on_spinbox_h_valueChanged(double value)
{
    ui->viewport->settings.h = value;
    ui->viewport->updateModels();
}


void MainWindow::on_cDistSpinBox_valueChanged(int value)
{
    ui->viewport->settings.c_mult1 = value;
    ui->viewport->updateModels();
}

void MainWindow::on_cUVSpinBox_valueChanged(int value)
{
    ui->viewport->settings.c_mult2 = value;
    ui->viewport->updateModels();
}

void MainWindow::on_cExponentSpinBox_valueChanged(double value)
{
    ui->viewport->settings.c_exponent = value;
    ui->viewport->updateModels();
}

void MainWindow::on_cDensitySpinBox_valueChanged(double value)
{
    ui->viewport->settings.c_density = value;
    ui->viewport->updateModels();
}

void MainWindow::on_saveImageButton_clicked() {
    ui->viewport->grab().save("../../screenshots/" + QDateTime::currentDateTimeUtc().toString() +".png");
    qDebug() << "Saved screenshot";
}
