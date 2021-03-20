#ifndef SOLUTION_H
#define SOLUTION_H

#include <QObject>
#include <cmath>
#include <QDebug>
#include <ImportanceGroups.h>
#include <computation_thread.h>
#include <exceptions.h>
#include <QList>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


namespace CJ {

enum Trend{
    max,
    min
};

class ComputationThread;

class solution : public QObject
{
    Q_OBJECT

private:
    int NumberOfTriggring = 0;
    int missedIterationCount;
    bool is_finished = false;

public:

    solution& operator=(const solution);
    solution(solution &other);
    explicit solution(QObject *parent = nullptr);
    explicit solution(QList<QList<double>> BaseTable,
                      QList<QString> IndicatorNames,
                      QList<QString> ProjectNames);


    QList<ComputationThread*> ThreadInstances;

    //TODO: Destruct in desctructor
    double **normTable;

private:
    QList<QList<double>> BaseTable;
    QList<QList<double>> NormalizedTable;

    QList<QString> IndicatorNames;
    QList<QString> ProjectNames;

    QList<double> HardRating;
    QList<double> SoftRating;

    QList<Trend> IndicatorTrends;

    bool IsParetoCriterionUsed = false;
    QList<int> ParetoSet;


    ImportanceGroups ProjectImportanceGroups;
    ImportanceGroups IndicatorImportanceGroups;

    double Step = 0.5;

    QTime timer;
    int ElapsedTimeInMS;

    int ThreadUsed;

    int ProjectCount;
    int IndicatorCount;

public:
    //Export
    //=================================
    void SaveAsJson(const QString& filePath = "./file.json");
    void SaveAsXml(const QString& filePath = "./file.xml");
    void SaveAsExcel(const QString& filePath = "./file.xlsx");
    void SaveAsCsv(const QString& filePath = "./file.csv", QChar sep = ';');
    //=================================



    //getters
    //---------------------------------------------------------

    //Ratings HR+SR
    QList<double>& GetHardRating();
    QList<double>& GetSoftRating();

    //Projects + Indicators
    const QList<QString>& GetProjectNames();
    const QList<QString>& GetIndicatorNames();

    //Base + Norm tables
    const QList<QList<double>>& GetBaseTable();
    const QList<QList<double>>& GetNormalizedTable();

    //Performance indicators
    int GetElapsedTimeInMS();
    const QString GetParsedElapsedTime();
    int GetUsedThreadCount();

    //Iterations
    int GetRealIterationCount();
    int GetMissedIterationCount();

    //Pareto set
    const QList<int>& GetParetoOptimalProjectIndexes();
    const QList<QString> GetParetoOptimalProjectNames();
    bool IsParetoUsed();

    //Importance Group
    ImportanceGroups& GetProjectImportanceGroup();
    ImportanceGroups& GetIndicatorImportanceGroup();

    //---------------------------------------------------------

    //setters
    //----------------------------------------------------------
    void SetStep(const double value);

    void SetProjectImportanceGroups(ImportanceGroups groups);
    void SetIndicatorImportanceGroups(ImportanceGroups groups);

    void SetProjectNames(const QList<QString> ProjectNames);
    void SetIndicatorNames(const QList<QString> IndicatorNames);
    void SetBaseTable(const QList<QList<double>> BaseTable);
    void SetNormalizedTable(const QList<QList<double>> NormalizedTable);

    void SetIndicatorTrends(const QList<Trend> trendList);
    void SetParetoCriterionEnabled(const bool enable);
    //----------------------------------------------------------


    //TODO: write unit tests
    //computing functions
    //-------------------------------------------------------------------------------------------------------------
    double* FindLinearConvlutionResult(const double *weights);
    double ** Convert(QList<QList<double>> NormalizedTable);
    QList<QList<double>> FindNormalizedTable(const QList<QList<double>>& BaseTable, const QList<Trend>& trendList);
    QList<int> FindParetoOptimalProjectIndexes(const QList<QList<double>>& NormalizedTable);
    double FindTheoreticalIterationCount(int IndicatorsCount, double step);
    QList<ComputationThread*> MakeThreadList(int ThreadCount = -1, int MaxThreadCount = QThread::idealThreadCount());
    double Factorial(double value);
    //-------------------------------------------------------------------------------------------------------------


    //Methods for managing multithreaded computations
    //----------------------------------------------------------------------------------
    void start(int ThreadCount = -1, int MaxThreadCount = QThread::idealThreadCount());
    void terminate();
    void join();
    //----------------------------------------------------------------------------------


signals:
    void finished();
    void currentIterationCount(int);


private slots:
    void FinishedAllThreads();
    void SendTotalCount();
};
}
#endif // SOLUTION_H
