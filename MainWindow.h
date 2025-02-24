#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_tabWidget_currentChanged(int index);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_verticalScaleSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
