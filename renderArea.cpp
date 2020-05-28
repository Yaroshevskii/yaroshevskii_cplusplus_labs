//
// Created by andy on 25.05.2020.
//

#include <iostream>
#include <math.h>
#include "renderArea.h"

typedef void(mWindow::*mFunc)();

void mWindow::InitTimer()
{
    Timer = new QTimer(this);
    connect(Timer, &QTimer::timeout, this, &mWindow::draw);
}

void mWindow::attach_charts(QChart *chart, QLineSeries *series, QString title)
{
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;

    chart->addSeries(series);
    chart->legend()->markers(series)[0]->setVisible(false);
    chart->setTitle(title);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}

void mWindow::attach_layout(QGridLayout *layout, QChartView *view, QChart *chart, int row, int column)
{
    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(view, row, column, 1, 1);
}

void mWindow::attach_layout(QGridLayout *layout, QChartView *view, int row, int column)
{
    view = new QChartView();
    view->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(view, row, column, 1, 1);
}


void mWindow::draw()
{
    resolve_type X;
    QChart *m_chart[SerNum];

    for(uint16_t i = 0; i < SerNum; i++)
    {
        m_chart[i] = new QChart();
        Series[i] = new QLineSeries();
    }

    SodSolver.vsolve();
    TimeLabel->setText("Time: " + QString::number(SodSolver.gTime));

    if (Timer->isActive() && (SodSolver.ENDTIME - SodSolver.gTime) >= (SodSolver.dt - 1e-3) )
    {
        SodSolver.gTime += SodSolver.dt;
    } else if (std::fabs(SodSolver.ENDTIME - SodSolver.gTime) >= 1e-9)
    {
        toStopHandle();
    }

    for(size_t i=0; i < SodSolver.CELLS; i++)
    {
        X = SodSolver.vresolve[XPOS][i];

        for(uint16_t j=0; j < SerNum; j++)
        {
            Series[j]->append(X, SodSolver.vresolve[j][i]);
        }
    }

    for(uint16_t i = 0; i < SerNum; i++)
    {
        attach_charts(m_chart[i], Series[i], titles[i]);
        attach_layout(mainLayout, m_chartView[i], m_chart[i], (( (i == 0) || (i == 2) ) ? 0 : 1), (( (i == 0) || (i == 1) ) ? 1 : 2));
    }
}

void mWindow::LoadNaturalsFromGUI()
{
    uint16_t k = 0;

    for(uint16_t i=0;i<FluidNum;i++)
    {
        for(uint16_t j=0;j<NatNum;j++)
        {
            SodSolver.LoadParametrs( k, (resolve_type)stod(naturals[i][j]->text().toStdString()) );
            k++;
        }
    }

    for(uint16_t i=0;i<ParNum;i++)
    {
        SodSolver.LoadParametrs( k, (resolve_type)stod(parametrs[i]->text().toStdString()) );
        k++;
    }

    SodSolver.InitHelpVars();
}

void mWindow::InitGUI()
{
    uint16_t k = 0;

    for(uint16_t i=0;i<FluidNum;i++)
    {
        for(uint16_t j=0;j<NatNum;j++)
        {
            naturals[i][j]->setText(QString::fromStdString( std::to_string(*SodSolver.parametrs[k]) ) ) ;
            k++;
        }
    }

    for(uint16_t i=0;i<ParNum;i++) {
        parametrs[i]->setText(QString::fromStdString( std::to_string(*SodSolver.parametrs[k]) ) );
        k++;
    }
}

void mWindow::toStopHandle()
{
    Timer->stop();
    time_buttons[2]->setIcon(QIcon("icons/2.png"));
}

void mWindow::toStartHandle()
{
    Timer->start(50);
    time_buttons[2]->setIcon(QIcon("icons/pause.png"));
}

void mWindow::playBut()
{
    if (Timer->isActive() )
    {
        toStopHandle();
        if (SodSolver.gTime > 0)
        {
            SodSolver.gTime -= SodSolver.dt;
        }
    }
    else
    {
        toStartHandle();
    }
}

void mWindow::ForBut()
{
    toStopHandle();
    if (SodSolver.gTime < SodSolver.ENDTIME)
    {
        SodSolver.gTime += SodSolver.dt;
    }
    draw();
}

void mWindow::RevBut()
{
    toStopHandle();
    if (SodSolver.gTime > 1e-6)
    {
        SodSolver.gTime -= SodSolver.dt;
    }
    draw();
}

void mWindow::StartBut()
{
    SodSolver.gTime = 0;
    toStartHandle();
}

void mWindow::EndBut()
{
    toStopHandle();
    SodSolver.gTime = SodSolver.ENDTIME;
    draw();
}

void mWindow::StopBut()
{
    toStopHandle();
    SodSolver.gTime = 0;
    draw();
}


mWindow::mWindow(QWidget *parent) :
        QWidget(parent)
{
    SodSolver.gTime = 0;

    this->setWindowTitle("Solver for Riemman problem");
    mainLayout = new QGridLayout;
    mainLayout->setColumnStretch( 0, 0 ) ;
    mainLayout->setColumnStretch( 1, 1 ) ;
    mainLayout->setColumnStretch( 2, 1 ) ;


    QValidator *valid = new QDoubleValidator();
    QFormLayout *input_layout = new QFormLayout();

    for (uint16_t j =0; j<FluidNum; j++)
    {
        for (uint16_t i =0; i<NatNum; i++)
        {
            naturals[j][i] = new QLineEdit();
            naturals[j][i]->setValidator(valid);
            input_layout->addRow(titles[i] + QString::number(j+1) + ": ", naturals[j][i]);
        }
    }

    input_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    input_layout->setFormAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->addLayout(input_layout, 0, 0);


    QFormLayout *input_layout2 = new QFormLayout();

    for (uint16_t i =0; i<ParNum; i++)
    {
        parametrs[i] = new QLineEdit();
        parametrs[i]->setValidator(valid);
        input_layout2->addRow(parametrsNames[i] + ": ", parametrs[i]);
    }

    QPushButton *plot_button =  new QPushButton("Calculate");
    connect(plot_button, &QPushButton::clicked, this, [&](){LoadNaturalsFromGUI(); StopBut();});
    input_layout2->addRow(plot_button);

    TimeLabel = new QLabel("Time: 0.0");
    input_layout2->addRow(TimeLabel);
    mainLayout->addLayout(input_layout2, 1, 0);


    for(uint16_t i = 0; i < SerNum; i++)
    {
        attach_layout(mainLayout, m_chartView[i], (( (i == 0) || (i == 2) ) ? 0 : 1), (( (i == 0) || (i == 1) ) ? 1 : 2));
    }

    QGroupBox *horizontalGroupBox = new QGroupBox();
    QHBoxLayout *layout = new QHBoxLayout;
    mFunc funcs[TimeNum] = {&mWindow::StartBut, &mWindow::RevBut, &mWindow::playBut, &mWindow::ForBut, &mWindow::EndBut, &mWindow::StopBut };


    for (uint16_t i=0; i<TimeNum; i++)
    {
        time_buttons[i] =  new QPushButton();
        time_buttons[i]->setIcon(QIcon("icons/"+ QString::number(i) +".png"));
        connect(time_buttons[i], &QPushButton::clicked, this, funcs[i]);
        layout->addWidget(time_buttons[i]);
    }

    horizontalGroupBox->setLayout(layout);
    mainLayout->addWidget(horizontalGroupBox, 2, 1, 1, 2);


    setLayout(mainLayout);

    InitTimer();
    InitGUI();
}

QSize mWindow::sizeHint() const
{
    return QSize(1024, 780);
}


mWindow::~mWindow()
{

}