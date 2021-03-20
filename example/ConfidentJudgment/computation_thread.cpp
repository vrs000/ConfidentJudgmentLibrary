#include "computation_thread.h"

using namespace CJ;

double ComputationThread::sum(double set[], int index)
{
    double Sum = 0.0;

    for (int i = 0; i < index; ++i)
    {
        Sum += set[i];
    }

    return Sum;
}
double ComputationThread::getMax(double *list)
{
    double max = list[0];

    for (int i = 0; i < ProjectsCount; i++)
        max = list[i] > max ? list[i] : max;

    return max;
}
double ComputationThread::getMin(double *list)
{
    double min = list[0];

    for (int i = 0; i < ProjectsCount; i++)
        min = list[i] < min ? list[i] : min;

    return min;
}
int ComputationThread::COUNT(double *list, double value)
{
    int k = 0;

    for (int i = 0; i < ProjectsCount; i++)
        if (list[i] == value) k++;

    return k;
}
int ComputationThread::IndexOf(double *list, double value)
{
    for (int i = 0; i < ProjectsCount; i++)
        if (list[i] == value) return i;

    return -1;
}

ComputationThread::ComputationThread(int firstIterationIndex, int lastIterationIndex, double step, solution& sol):
    solInstance(&sol), FirstIterationIndex(firstIterationIndex), LastIterationIndex(lastIterationIndex), h(step)
{
//    solInstance = &sol;
//    FirstIterationIndex = firstIterationIndex;
//    LastIterationIndex = lastIterationIndex;
//    h = step;

    ProjectsCount = sol.IsParetoUsed() ? sol.GetParetoOptimalProjectNames().size() : sol.GetProjectNames().size();


    HardRatings = QVector<double>(ProjectsCount, 0);
    SoftRatings = QVector<double>(ProjectsCount, 0);

    hardRatings = QVector<double>(ProjectsCount, 0);
    softRatings = QVector<double>(ProjectsCount, 0);


    ProjectsGroupCount = sol.GetProjectImportanceGroup().GetRelationList().size();
    IndicatorsGroupCount = sol.GetIndicatorImportanceGroup().GetRelationList().size();


    //Показатели
    //---------------------------------------------------------------
    PrefferedMetricsV2 = sol.GetIndicatorImportanceGroup().GetPreffered();
    RejectedMetricsV2 = sol.GetIndicatorImportanceGroup().GetRejected();
    IndicatorsRelation = sol.GetIndicatorImportanceGroup().GetRelation();
    //---------------------------------------------------------------

    //Проекты
    //---------------------------------------------------------------
    PrefferedProjectsV2 = sol.GetProjectImportanceGroup().GetPreffered();
    RejectedProjectsV2 = sol.GetProjectImportanceGroup().GetRejected();
    ProjectsRelation = sol.GetProjectImportanceGroup().GetRelation();
    //---------------------------------------------------------------


    IndicatorsGroupCount = IndicatorsGroupCount == 0 ? -1 : IndicatorsGroupCount;
    ProjectsGroupCount = ProjectsGroupCount == 0 ? -1 : ProjectsGroupCount;
}

ComputationThread::ComputationThread(const ComputationThread &)
{

}

ComputationThread::~ComputationThread()
{
    delete [] PrefferedMetricsV2;
    delete [] RejectedMetricsV2;
    delete [] PrefferedProjectsV2;
    delete [] RejectedProjectsV2;
}

QVector<double> ComputationThread::GetHardRatings()
{
    return HardRatings;
}

QVector<double> ComputationThread::GetSoftRatings()
{
    return SoftRatings;
}

int ComputationThread::GetMissed()
{
    return missed;
}

int ComputationThread::GetCount()
{
    return Count;
}

void ComputationThread::Calculate(double currentSet[], int maxN, int curPosIndex)
{
    if((curPosIndex < maxN - 1))
    {
        double lim = 1 - sum(currentSet, curPosIndex);

        for(double i = 0; i <= lim + 0.00001; i += h)
        {
            currentSet[curPosIndex] = i;
            Calculate(currentSet, maxN, curPosIndex + 1);
        }
    }
    else
        if(curPosIndex == maxN - 1)
        {
            double rest = 1 - sum(currentSet, curPosIndex);
            if (rest - 0.00001 < 0)
                rest = 0;

            if (fabs(static_cast<double>(rest - h)) < 0.00001)
                rest = h;

            currentSet[curPosIndex] = rest;

            GeneratedIterCount++;

            //Если индекс не входит в пределы, то прервать
            if(GeneratedIterCount < FirstIterationIndex || GeneratedIterCount > LastIterationIndex) return;


            Count++;


            if (Count % 10000 == 0)
                emit CountChanged(Count);


            double* res = solInstance->FindLinearConvlutionResult(currentSet);
            bool IsSuitable = true;


            //Проверка проектов
            //---------------------------------------------------------------------------
            if (ProjectsGroupCount != -1)
            {
                for (int i = 0; i < ProjectsGroupCount; i++)
                {
                    if (ProjectsRelation[i] == '=')
                        if (!(res[PrefferedProjectsV2[i]] == res[RejectedProjectsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }

                    if (ProjectsRelation[i] == '>')
                        if (!(res[PrefferedProjectsV2[i]] > res[RejectedProjectsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }

                    if (ProjectsRelation[i] == 'b')
                        if (!(res[PrefferedProjectsV2[i]] >= res[RejectedProjectsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }
                }

            }
            //---------------------------------------------------------------------------


            //Проверка индикаторов
            //--------------------------------------------------------------------------------------
            if (IndicatorsGroupCount != -1)
            {
                for (int i = 0; i < IndicatorsGroupCount; i++)
                {

//                    qDebug() << currentSet[PrefferedMetricsV2[i]] << currentSet[RejectedMetricsV2[i]] << PrefferedMetricsV2[i] << RejectedMetricsV2[i] << IndicatorsRelation[i] << IndicatorsGroupCount;
                    if (IndicatorsRelation[i] == '=')
                        if (!(currentSet[PrefferedMetricsV2[i]] == currentSet[RejectedMetricsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }

                    if (IndicatorsRelation[i] == '>')
                        if (!(currentSet[PrefferedMetricsV2[i]] > currentSet[RejectedMetricsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }

                    if (IndicatorsRelation[i] == 'b')
                        if (!(currentSet[PrefferedMetricsV2[i]] >= currentSet[RejectedMetricsV2[i]]))
                        {
                            IsSuitable = false;
                            break;
                        }
                }
            }
            //--------------------------------------------------------------------------------------


            if (IsSuitable)
            {
                //Мягкий рейтинг
                //-----------------------------------------------
                for (int i = 0; i < ProjectsCount; i++)
                {

                    softRatings[i] = res[i] / (getMax(res));
                    hardRatings[i] = 0;
                }
                //-----------------------------------------------


                //Жёсткий рейтинг
                //-----------------------------------------------------------
                if (COUNT(res, getMax(res)) > 1)
                {
                    int q = COUNT(res, getMax(res));
                    for (int i = 0; i < ProjectsCount; i++)
                        hardRatings[i] = (res[i] == getMax(res)) ? 1.0/q : 0;
                }
                else
                {
                    hardRatings[IndexOf(res, getMax(res))] = 1;
                }
                //-----------------------------------------------------------


                //Занесение результата
                //---------------------------------------
                for (int i = 0; i < ProjectsCount; i++)
                {
                    HardRatings[i] += hardRatings[i];

                    if (softRatings[0] == softRatings[0])
                        SoftRatings[i] += softRatings[i];
                }
                //---------------------------------------
            }
            else missed++;

            delete res;
        }
}

void ComputationThread::run()
{    
    double *set = new double[solInstance->GetIndicatorNames().size()];

    for (int i = 0; i < solInstance->GetIndicatorNames().size(); i++)
        set[i] = 0;

    Calculate(set, solInstance->GetIndicatorNames().size(), 0);

    delete[] set;
}
