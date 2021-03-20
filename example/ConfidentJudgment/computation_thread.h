#ifndef COMPUTATION_THREAD_H
#define COMPUTATION_THREAD_H


#include <QThread>
#include <QList>
#include <QTime>
#include <solution.h>


namespace  CJ{

class solution;

class ComputationThread : public QThread
{
    Q_OBJECT

private:
    solution *solInstance;

    QVector<double> HardRatings;
    QVector<double> SoftRatings;


    QTime time;


    int FirstIterationIndex;
    int LastIterationIndex;


    int Count = 0;
    int GeneratedIterCount = 0;


    bool IsNeedToFinish = false;


private:
    char* ProjectsRelation;
    char* IndicatorsRelation;

    int *PrefferedMetricsV2;
    int *RejectedMetricsV2;

    int *PrefferedProjectsV2;
    int *RejectedProjectsV2;

    int ProjectsGroupCount = 0;
    int IndicatorsGroupCount = 0;
    //-------------------------


    double h;
    double sum(double set[], int index);


    QVector<double> hardRatings;
    QVector<double> softRatings;


    int PrefferedMetricsCount = 0;
    int RejectedMetricsCount = 0;


    int PrefferedProjectsCount = 0;
    int RejectedProjectsCount = 0;


    int ProjectsCount = 0;

private:
    double getMax(double *list);
    double getMin(double *list);
    int COUNT(double* list, double value);
    int IndexOf(double* list, double value);

public:
    ComputationThread();
    ComputationThread(int firstIterationIndex,  int lastIterationIndex, double step, solution& sol);

    ComputationThread(const ComputationThread&);
//    ComputationThread& operator=(const ComputationThread);

    ~ComputationThread();


    QVector<double> GetHardRatings();
    QVector<double> GetSoftRatings();

    int GetMissed();

    int GetCount();
    int missed = 0;
    void Calculate(double currentSet[], int maxN, int curPosIndex);

signals:
    void CountChanged(int count);

protected:
    void run() override;
};
}
#endif // COMPUTATION_THREAD_H
