#ifndef TIMER_H
#define TIMER_H

#include <QWidget>
#include <QProgressBar>
#include <global_variable.h>
#include <QPushButton>

class Timer : public QWidget
{
    Q_OBJECT

public:
    Timer(QWidget *parent = 0);

public slots:
    void updateProgress();
    void onClickExitButton();

private:
    QProgressBar *progressBar;
    QPushButton *exitButton;
};

#endif // TIMER_H
