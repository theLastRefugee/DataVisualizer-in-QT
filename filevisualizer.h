#ifndef FILEVISUALIZER_H
#define FILEVISUALIZER_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include <thread_function.h>
#include <QWidget>
#include <QDialog>
#include <algorithm>
#include <utils.h>
#include <QtCharts>

namespace Ui {
class FileVisualizer;
}

class FileVisualizer : public QDialog
{
    Q_OBJECT

public:
    explicit FileVisualizer(QWidget *parent = nullptr);
    ~FileVisualizer();
    QString filename;

    void read_file();

    void draw_barchart_for_top10(QVector<QPair<int, int> > *top10_vector, int user_id_lower, int user_id_upper);

    void set_default_value();

private slots:
    void on_exitButton_clicked();

    void on_top10Button_clicked();

    void on_clearButton_clicked();

    void on_checkinButton_clicked();

    void on_compare2locationButton_clicked();

    void on_compare2userButton_clicked();

    void on_usertrajectoryButton_clicked();

    void on_heatmapButton_clicked();

private:
    Ui::FileVisualizer *ui;
};

#endif // FILEVISUALIZER_H
