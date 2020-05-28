//
// Created by andy on 25.05.2020.
//

#ifndef SOLVERFORRIEMMANPROBLEMQT_RENDERAREA_H
#define SOLVERFORRIEMMANPROBLEMQT_RENDERAREA_H

#include "RiemmanSolver.h"
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QtCharts>




class mWindow : public QWidget
{
public:
    explicit mWindow(QWidget *parent = nullptr);
    void InitTimer();
    ~mWindow();
    void draw();
    QSize sizeHint() const override;

private:
    static const size_t FluidNum = 2, NatNum = 3, ParNum = 6, SerNum = 4, TimeNum = 6;
    QGridLayout *mainLayout;
    QTimer *Timer;
    QChartView *m_chartView[SerNum];
    QLineSeries *Series[SerNum];
    QString titles[SerNum] = {"density", "pressure", "velocity", "Temperature" };
    QString parametrsNames[ParNum] = {"Lmin", "Lmax", "dt",  "Nx", "Gamma", "EndTime" };
    RiemmanSolver SodSolver{};
    QPushButton *time_buttons[TimeNum];
    QLabel *TimeLabel;

    QLineEdit *naturals[FluidNum][NatNum];
    QLineEdit *parametrs[ParNum];



    void attach_charts(QChart *chart, QLineSeries *series, QString title);
    void attach_layout(QGridLayout *layout, QChartView *view, QChart *chart, int row, int column);
    void attach_layout(QGridLayout *layout, QChartView *view, int row, int column);
    void LoadNaturalsFromGUI();
    void InitGUI();
    void playBut();
    void toStopHandle();
    void toStartHandle();
    void ForBut();
    void RevBut();
    void StartBut();
    void EndBut();
    void StopBut();
};

#endif //SOLVERFORRIEMMANPROBLEMQT_RENDERAREA_H
