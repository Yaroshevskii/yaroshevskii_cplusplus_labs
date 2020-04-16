//
// Created by andy on 30.03.2020.
//

#ifndef QTTEST_RENDERAREA_H
#define QTTEST_RENDERAREA_H



#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QtCharts>


class mWindow : public QWidget
{
public:
    explicit mWindow(QWidget *parent = nullptr);
    ~mWindow();
    void draw();
    QSize sizeHint() const override;

private:
    void set_coef(QLineEdit *edit, qreal *item);
    QChart *m_chart;
    QChartView *m_chartView;
    QGridLayout *mainLayout;
    QLineEdit *a_val, *b_val, *c_val;

};





#endif //QTTEST_RENDERAREA_H
