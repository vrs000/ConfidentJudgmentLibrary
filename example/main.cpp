#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <solution.h>
#include <random>
#include <ImportanceGroups.h>
#include <QFile>


const int Projects = 6;
const int Indicators = 5;
const double h = 0.01;

using namespace CJ;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    // Задание начальных данных
    // --------------------------------------------------
    QList<QList<double>> base = {
        {8, 2400, 175, 8, 70000},
        {6, 2000, 160, 6, 58000},
        {6, 1700, 163, 4, 20000},
        {4, 1500, 150, 4, 17000},
        {4, 1300, 170, 3, 12000},
        {2, 1000, 155, 2, 8000},
    };

    QList<QString> indicator;
    QList<QString> projects;

    QList<Trend> trends = {max, max, min, max, min};

    for (int i = 0 ; i < Projects; i++)
        projects << QString("Project%1").arg(i);

    for (int i = 0 ; i < Indicators; i++)
        indicator << QString("Indicator%1").arg(i);

    // --------------------------------------------------


    solution sol(base, indicator, projects);

    QList<Relation> list = {
        Relation(4, 2, Relation::equivalence)
    };
    ImportanceGroups groups(list);

    //    sol.SetIndicatorImportanceGroups(groups);
    sol.SetStep(h);
    sol.SetIndicatorTrends(trends);

//    sol.SetParetoCriterionEnabled(true);

    sol.start();
    sol.join();

    sol.SaveAsJson();
//    sol.SaveAsJson();
    //    qDebug() << "Time elapsed:" << sol.GetParsedElapsedTime();
    //    qDebug() <<"Threads:" << sol.GetUsedThreadCount();
    //    qDebug()<< sol.GetRealIterationCount();
        qDebug()<< "HR:" << sol.GetHardRating();
        qDebug()<< "SF:" << sol.GetSoftRating();

//    if (sol.IsParetoUsed())
//        qDebug() << "Pareto optimal projects:" << sol.GetParetoOptimalProjectNames();

//    json_test();

    return 0;
}
