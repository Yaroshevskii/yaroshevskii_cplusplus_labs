//
// Created by andy on 30.03.2020.
//

#include "renderArea.h"

void mWindow::set_coef(QLineEdit *edit, qreal *item)
{
    if (edit->text().isEmpty())
    {
        *item = 0;
        edit->setText("0,0");
    } else
    {
        std::string ret = edit->text().toStdString();

        *item = stod(ret);
    }
}

void mWindow::draw()
{
    m_chart = new QChart();
    qreal xmin = -10;
    qreal xmax = 10;
    qreal dx = 0.1;
    qreal a, b, c;


    set_coef(a_val, &a);
    set_coef(b_val, &b);
    set_coef(c_val, &c);


    auto f = [a, b, c](qreal x) { return a*x*x + b*x + c; };

    QLineSeries *series = new QLineSeries();
    for (qreal x = xmin; x < xmax; x += dx)
    {
        series->append(x, f(x));
    }

    m_chart->addSeries(series);
    m_chart->legend()->markers(series)[0]->setVisible(false);


    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("X");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Y");
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumSize(640, 480);
    mainLayout->addWidget(m_chartView, 0, 1, 1, 1);
}

mWindow::mWindow(QWidget *parent) :
        QWidget(parent)
{
    this->setWindowTitle("lab5");
    mainLayout = new QGridLayout;
    mainLayout->setColumnStretch( 0, 0 ) ;
    mainLayout->setColumnStretch( 1, 1 ) ;



    QPushButton *plot_button =  new QPushButton("Calculate");
    connect(plot_button, &QPushButton::clicked, this, &mWindow::draw);
    mainLayout->addWidget(plot_button, 1, 0);



    QValidator *valid = new QDoubleValidator();
    a_val = new QLineEdit();
    b_val = new QLineEdit();
    c_val = new QLineEdit();

    a_val->setValidator(valid);
    b_val->setValidator(valid);
    c_val->setValidator(valid);

    QFormLayout *input_layout = new QFormLayout();
    input_layout->addRow("A:", a_val);
    input_layout->addRow("B:", b_val);
    input_layout->addRow("C:", c_val);
    input_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    input_layout->setFormAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->addLayout(input_layout, 0, 0);



    m_chartView = new QChartView();
    m_chartView->setMinimumSize(640, 480);
    mainLayout->addWidget(m_chartView, 0, 1, 1, 1);


    setLayout(mainLayout);
}

QSize mWindow::sizeHint() const
{
    return QSize(640, 480);
}


mWindow::~mWindow()
{
    delete m_chart;
    delete m_chartView;
}
