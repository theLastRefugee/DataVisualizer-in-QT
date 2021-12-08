#include "timer.h"
#include <QLayout>
#include <QTimer>


Timer::Timer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();

    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(FileLineTotal);

    exitButton = new QPushButton();
    exitButton->setText("stop reading");


    layout->addWidget(progressBar);
    layout->addWidget(exitButton);
    QObject::connect(exitButton, &QPushButton::clicked, this, &Timer::onClickExitButton);

    setLayout(layout);



    setWindowTitle(tr("progress of reading file"));
    resize(200, 200);
}

void Timer::updateProgress()
{
    progressBar->setValue(FileLineCount);
}


void Timer::onClickExitButton()
{
    exit(0);
}

