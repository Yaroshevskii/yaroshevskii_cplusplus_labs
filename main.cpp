#include <iostream>
#include <QApplication>
#include <gtest/gtest.h>
#include <math.h>
#include "renderArea.h"





int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    mWindow w;
    w.show();

    return app.exec();
}