#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qpushbutton.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadMeshButton_clicked();
    void on_spinbox_s_valueChanged(double value);
    void on_spinbox_e_valueChanged(double value);
    void on_spinbox_t_valueChanged(double value);
    void on_spinbox_h_valueChanged(double value);

    void on_tabWidget_currentChanged(int index);

    void on_verticalScaleSpinBox_valueChanged(double value);

    void on_cDistSpinBox_valueChanged(int value);
    void on_cUVSpinBox_valueChanged(int value);
    void on_cExponentSpinBox_valueChanged(double value);
    void on_cDensitySpinBox_valueChanged(double value);

    void on_saveImageButton_clicked();

    void on_water1Btn_clicked();
    void on_water2Btn_clicked();
    void on_land1Btn_clicked();
    void on_land2Btn_clicked();

private:
    Ui::MainWindow *ui;
    QColor colorBtnClicked(QPushButton *btn);
    void setBtnColor(QPushButton *btn, QColor const &color);
};
#endif // MAINWINDOW_H
