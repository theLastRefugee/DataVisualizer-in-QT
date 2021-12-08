#ifndef FILEVISUALIZER_H
#define FILEVISUALIZER_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include <thread_function.h>
#include <QWidget>
#include <QDialog>

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

private slots:
    void on_exitButton_clicked();

private:
    Ui::FileVisualizer *ui;
};

#endif // FILEVISUALIZER_H
