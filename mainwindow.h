#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread_function.h>
#include <global_variable.h>
#include <filevisualizer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    FileVisualizer *my_file_visualizer;


    void give_filename_to_file_manager(QString file_name)
    {
        my_file_visualizer->filename = file_name;
    }

private slots:
    void on_pushButton_clicked();



    void on_exitButton_clicked();

    void on_checkBox_longitude_stateChanged(int arg1);

    void on_checkBox_locationid_stateChanged(int arg1);

    void on_checkBox_time_stateChanged(int arg1);

    void on_checkBox_latitude_stateChanged(int arg1);

    void on_readButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
