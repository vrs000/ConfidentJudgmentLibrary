#include "solution.h"

using namespace CJ;

solution &solution::operator=(const solution other)
{
    ProjectNames = other.ProjectNames;
    IndicatorNames = other.IndicatorNames;

    ProjectImportanceGroups = other.ProjectImportanceGroups;
    IndicatorImportanceGroups = other.IndicatorImportanceGroups;

    ParetoSet = other.ParetoSet;
    IsParetoCriterionUsed = other.IsParetoCriterionUsed;

    BaseTable = other.BaseTable;
    NormalizedTable = other.NormalizedTable;

    HardRating = other.HardRating;
    SoftRating = other.SoftRating;

    NumberOfTriggring = other.NumberOfTriggring;
    Step = other.Step;

    ThreadInstances = other.ThreadInstances;
    ThreadUsed = other.ThreadUsed;

    missedIterationCount = other.missedIterationCount;

    timer = other.timer;

    return *this;
}

solution::solution(solution &other)
{
    *this = other;
}

solution::solution(QObject *parent) : QObject(parent)
{
    //passed
}

solution::solution(QList<QList<double> > BaseTable, QList<QString> IndicatorNames, QList<QString> ProjectNames)
{
    if (IndicatorNames.size() == 0 || IndicatorNames.size() == 1) throw 1;
    if (ProjectNames.size() == 0 || ProjectNames.size() == 1) throw 1;
    if (IndicatorNames.size() != BaseTable.first().size()) throw 1;
    if (ProjectNames.size() != BaseTable.size()) throw 1;

    int ElementCount=0;
    foreach (auto p, BaseTable)
        ElementCount += p.size();

    if (ElementCount != IndicatorNames.size()*ProjectNames.size()) throw 1;


    ProjectCount = ProjectNames.size();
    IndicatorCount = IndicatorNames.size();

    this->BaseTable=BaseTable;
    this->IndicatorNames = IndicatorNames;
    this->ProjectNames = ProjectNames;
}

void solution::SaveAsJson(const QString &filePath)
{
    auto GetQJsonArrayFromQList = [](QList<QString> list){
       QJsonArray array;

       for (auto name:list)
           array << name;

       return array;
    };
    auto GetQJsonArrayFromQListInt = [](QList<int> list){
       QJsonArray array;

       for (auto name:list)
           array << name;

       return array;
    };
    auto GetQJsonArrayFromQListDouble= [](QList<double> list){
       QJsonArray array;

       for (auto name:list)
           array << name;

       return array;
    };
    auto GetQJsonArrayFromDoubleQList = [](QList<QList<double>> table){
        QJsonArray array;

        for (int row = 0; row < table.size(); row++){
            QJsonArray row_array;

            for (int column = 0; column < table.first().size(); column++)
                row_array << table[row][column];

            array << row_array;
        }

        return array;
    };
    auto GetQJsonArrayFromQListTrend = [](QList<Trend> list){
      QJsonArray array;

      for (auto trend:list)
          array << (trend == max ? "max" : "min");

      return array;
    };

    QFile json(filePath);
    if (!json.open(QIODevice::WriteOnly))return;
    QJsonObject main;


    //Step
    main["Step"] = Step;

    //Theoretical_Iteration_Count
    main["Theorecital_iteration_count"] = FindTheoreticalIterationCount(IndicatorNames.size(), Step);

    //Missed_Iteration_Count
    main["Missed_iteration_count"] = missedIterationCount;

    //Elapsed_time
    main["Elapsed_time"] = ElapsedTimeInMS;

    //Project_names
    main["Project_names"] = GetQJsonArrayFromQList(ProjectNames);

    //Indicator_names
    main["Indicator_names"] = GetQJsonArrayFromQList(IndicatorNames);

    //Base_table
    main["Base_table"] = GetQJsonArrayFromDoubleQList(BaseTable);

    //Normalized_table
    main["Normalized_table"] = GetQJsonArrayFromDoubleQList(NormalizedTable);

    //Pareto set
    main["Is_Pareto_used"] = IsParetoCriterionUsed;
    main["Pareto_optimal_project_names"] = GetQJsonArrayFromQList(GetParetoOptimalProjectNames());
    main["Pareto_optimal_project_indexes"] = GetQJsonArrayFromQListInt(GetParetoOptimalProjectIndexes());

    //Indicator trends
    main["Indicator trends"] = GetQJsonArrayFromQListTrend(IndicatorTrends);

    //ImportanceGroups
    main["Project_importance_groups"] = ProjectImportanceGroups.ToQString();
    main["Indicator_importance_groups"] = IndicatorImportanceGroups.ToQString();

    //Hard Ratings
    main["HR"] = GetQJsonArrayFromQListDouble(HardRating);

    //Soft Ratings
    main["SR"] = GetQJsonArrayFromQListDouble(SoftRating);


    json.write(QJsonDocument(main).toJson(QJsonDocument::Indented));
    json.close();
}

void solution::SaveAsCsv(const QString &filePath, QChar sep)
{
   QFile file(filePath);

   if (!file.open(QIODevice::WriteOnly)){
       file.close();
       return;
   }

   QTextStream out(&file);
/*	;i1;i2;
    1;2;
    2;2;
*/
   //first string
   QString first = "Проекты;";
   for (auto indicator:IndicatorNames)
       first += tr("%1;").arg(indicator);
   out << first << "\n";
}

QList<double> &solution::GetHardRating()
{
    return HardRating;
}

QList<double> &solution::GetSoftRating()
{
    return SoftRating;
}

const QList<QString> &solution::GetProjectNames()
{
    return ProjectNames;
}

const QList<QString> &solution::GetIndicatorNames()
{
    return IndicatorNames;
}

const QList<QList<double> > &solution::GetBaseTable()
{
    return BaseTable;
}

const QList<QList<double> > &solution::GetNormalizedTable()
{
    return NormalizedTable;
}

int solution::GetElapsedTimeInMS()
{
    return ElapsedTimeInMS;
}

const QString solution::GetParsedElapsedTime()
{
    QString time = "";
    int ms = ElapsedTimeInMS;
    int m, sec, min, hour;

    m = ms % 1000;
    sec = ms / 1000;

    min = sec / 60;
    sec = sec % 60;

    hour = min / 60;
    min = min % 60;

    if (hour > 9) time += QString::number(hour);
    else time += "0" + QString::number(hour);

    if (min > 9) time +=":" + QString::number(min);
    else time +=":0" + QString::number(min);

    if (sec > 9) time +=":" + QString::number(sec);
    else time +=":0" + QString::number(sec);

    if (m > 100) time +=":" + QString::number(m);
    else if (m > 9) time +=":0" + QString::number(m);
    else time +=":00" + QString::number(m);

    return time;
}

int solution::GetUsedThreadCount()
{
    return ThreadUsed;
}

int solution::GetRealIterationCount()
{
    return FindTheoreticalIterationCount(IndicatorNames.size(), Step) - missedIterationCount;
}

int solution::GetMissedIterationCount()
{
    return missedIterationCount;
}

const QList<int> &solution::GetParetoOptimalProjectIndexes()
{
    return ParetoSet;
}

const QList<QString> solution::GetParetoOptimalProjectNames()
{
    QList<QString> names;

    foreach (auto index, ParetoSet)
        names << ProjectNames[index];

    return names;
}

bool solution::IsParetoUsed()
{
    return IsParetoCriterionUsed;
}

ImportanceGroups& solution::GetProjectImportanceGroup()
{
    return ProjectImportanceGroups;
}

ImportanceGroups &solution::GetIndicatorImportanceGroup()
{
    return IndicatorImportanceGroups;
}

double *solution::FindLinearConvlutionResult(const double *weights)
{
    double* results = new double[ProjectCount];
    double sum;

    for (int i = 0; i < ProjectCount; i++)
    {
        sum = 0;

        for (int j = 0; j < IndicatorCount; j++)
            sum += normTable[ParetoSet.size() == 0 ? i : ParetoSet[i]][j] * weights[j];

        results[i] = sum;
    }

    return results;
}

void solution::SetStep(const double value)
{
    const QList<double> list = {
        0.001, 0.002, 0.004, 0.005, 0.008,
        0.01, 0.02, 0.025, 0.04, 0.05, 0.1,
        0.125, 0.2, 0.25, 0.5};

    //TODO: Проверка на целое значение
    if (!list.contains(value)) throw 1;
    this->Step = value;
}

void solution::SetProjectImportanceGroups(ImportanceGroups groups)
{
    this->ProjectImportanceGroups = groups;
}

void solution::SetIndicatorImportanceGroups(ImportanceGroups groups)
{
    this->IndicatorImportanceGroups = groups;
}

void solution::SetProjectNames(const QList<QString> ProjectNames)
{
    ProjectCount = ProjectNames.size();
    this->ProjectNames = ProjectNames;
}

void solution::SetIndicatorNames(const QList<QString> IndicatorNames)
{
    IndicatorCount = IndicatorNames.size();
    this->IndicatorNames = IndicatorNames;
}

void solution::SetBaseTable(const QList<QList<double> > BaseTable)
{
    //    if (BaseTable.size() != ProjectNames.size()) throw 1;
    //    if (BaseTable[0].size() != IndicatorNames.size()) throw 1;

    int elementsCount = 0;
    foreach (auto p, BaseTable)
        elementsCount += p.size();

    //    if (elementsCount != ProjectNames.size()*IndicatorNames.size()) throw 1;

    this->BaseTable = BaseTable;
}

void solution::SetNormalizedTable(const QList<QList<double> > NormalizedTable)
{
    this->normTable = Convert(NormalizedTable);
    this->NormalizedTable=NormalizedTable;
}

void solution::SetIndicatorTrends(const QList<Trend> trendList)
{
    //TODO: проверка на размер
    if (trendList.size() != IndicatorNames.size()) throw 1;

    this->IndicatorTrends = trendList;
}

void solution::SetParetoCriterionEnabled(const bool enable)
{
    this->IsParetoCriterionUsed = enable;
}

double **solution::Convert(QList<QList<double>> NormalizedTable)
{
    double **data = new double* [NormalizedTable.size()];

    for (int i=0; i<NormalizedTable.size(); i++)
        data[i] = new double[NormalizedTable.first().size()];


    for (int i=0; i<NormalizedTable.size() ; i++)
        for (int j=0; j<NormalizedTable.first().size() ; j++)
            data[i][j] = NormalizedTable[i][j];


    return data;
}

QList<QList<double> > solution::FindNormalizedTable(const QList<QList<double> > &BaseTable, const QList<Trend> &trendList)
{
    QList<QList<double>> normalizedTable(BaseTable);
    double max_value;
    double min_value;


    for (int column = 0; column < BaseTable.first().size(); column++)
    {
        max_value = BaseTable.first()[column];
        min_value = BaseTable.first()[column];


        //find max and min values
        //--------------------------------------------------------------------------------------
        for (int row = 0; row < BaseTable.size(); row++)
        {
            max_value = BaseTable[row][column] > max_value ? BaseTable[row][column] : max_value;
            min_value = BaseTable[row][column] < min_value ? BaseTable[row][column] : min_value;
        }
        //--------------------------------------------------------------------------------------


        //normalize column
        //---------------------------------------------------------------------------------
        for (int row = 0; row < BaseTable.size(); row++)
        {
            normalizedTable[row][column] = trendList[column] == max ?
                        (BaseTable[row][column] - min_value)/(max_value-min_value) :
                        normalizedTable[row][column];


            normalizedTable[row][column] = trendList[column] == min ?
                        1 - (BaseTable[row][column] - min_value)/(max_value-min_value) :
                        normalizedTable[row][column];
        }
        //---------------------------------------------------------------------------------
    }


    normTable = Convert(normalizedTable);

    return normalizedTable;
}

QList<int> solution::FindParetoOptimalProjectIndexes(const QList<QList<double> > &NormalizedTable)
{
    QList<int> ParetoSet;

    QList<int> WorstProjectIndexes;
    bool IsBetter;


    for (int i = 0; i < NormalizedTable.size(); i++)
    {
        if (WorstProjectIndexes.contains(i)) continue;

        for (int j = 0; j < NormalizedTable.size(); j++)
        {
            IsBetter = true;

            if (j == i) continue;
            if (WorstProjectIndexes.contains(j)) continue;

            for (int paramIndex = 0; paramIndex < NormalizedTable[0].size(); paramIndex++ )
            {
                IsBetter = IsBetter && (NormalizedTable[i][paramIndex] >= NormalizedTable[j][paramIndex]);
            }

            if (IsBetter)
                WorstProjectIndexes << j;
        }
    }

    for (int i = 0; i < NormalizedTable.size(); i++)
        if (!WorstProjectIndexes.contains(i))
            ParetoSet << i;

    return ParetoSet;
}

double solution::FindTheoreticalIterationCount(int IndicatorsCount, double step)
{
    return std::floor(Factorial(std::floor(1 / step) + IndicatorsCount - 1)
                      / (Factorial(IndicatorsCount - 1) * Factorial(std::floor(1 / step))));
}

QList<ComputationThread *> solution::MakeThreadList(int ThreadCount, int MaxThreadCount)
{
    QList<ComputationThread*> threads;

    auto setListOfThreads = [&](int ThreadsCount){
        ThreadUsed = ThreadsCount;
        int iterCount = FindTheoreticalIterationCount(IndicatorNames.size(), Step);

        int step = iterCount / ThreadsCount;
        int a = 1;
        int b = ThreadsCount == 1 ? iterCount : a+step;
        for (int i=0; i < ThreadsCount; i++)
        {
            threads << new ComputationThread(a, b, Step, *this);

            //signals
            //--------------------------------------------------------------------------------
            connect(threads.last(), SIGNAL(CountChanged(int)), this, SLOT(SendTotalCount()));
            //            connect(threads.last(), SIGNAL(finished()), this, SLOT(FinishedAllThreads()));
            //--------------------------------------------------------------------------------


            a = b + 1;
            b = i == ThreadsCount-1 ? iterCount : b+step;
        }
    };

    //Если не задано количество потоков в которых нужно призвести вычисления
    if (ThreadCount == -1)
    {
        const unsigned int AlgorithmComplexity =  FindTheoreticalIterationCount(IndicatorNames.size(), Step) * ProjectNames.size() * IndicatorNames.size();
        const int AvailableThreadCount = QThread::idealThreadCount();


        if ((AlgorithmComplexity < 500000) || (AvailableThreadCount == 1))
        {
            setListOfThreads(1);
            return threads;
        }


        if ((AlgorithmComplexity >= 500000 && AlgorithmComplexity < 1000000 && AvailableThreadCount >= 2)
                || (AlgorithmComplexity >= 1000000 && AvailableThreadCount == 2))
        {
            setListOfThreads(2);
            return threads;

        }


        if ((AlgorithmComplexity >= 1000000 && AlgorithmComplexity < 2000000 && AvailableThreadCount >= 4)
                || (AlgorithmComplexity >= 2000000 && AvailableThreadCount == 4))
        {
            setListOfThreads(4);
            return threads;

        }


        if ((AlgorithmComplexity >= 2000000 && AvailableThreadCount >= 8))
        {
            setListOfThreads(MaxThreadCount);
            return threads;
        }
    }

    if (ThreadCount != -1)
        setListOfThreads(ThreadCount);

    return threads;
}

double solution::Factorial(double value)
{
    double ans = 1;

    value = qRound(value);

    for (int i = 1; i <= value; ++i)
    {
        ans *= i;
    }

    return ans;
}

void solution::start(int ThreadCount, int MaxThreadCount)
{
    // sizes validation
    //---------------------------------------------------------------------------------
    if (ProjectNames.size() == 0) throw InvalidProjectNamesException("Zero size of project names list");
    if (IndicatorNames.size() == 0) throw InvalidIndicatorNamesException("Zero size of indicator names list");
    if (BaseTable.size() == 0) throw InvalidBaseTableException("0x0 sizes of base table matrix");
//    if (NormalizedTable.size() == 0) throw InvalidNormalizedTableException("0x0 sizes of normalized table matrix");
    if (ProjectNames.size() != BaseTable.size()) throw SizeMismatchException("Project names list size not equal to row count of base table matrix");
    if (IndicatorNames.size() != BaseTable.first().size()) throw SizeMismatchException("Indicator names list size not equal to column count of base table matrix");
    if (IndicatorTrends.size() == 0) throw InvalidTrendsException("Zero trend list");
    if (IndicatorTrends.size() != IndicatorNames.size()) throw InvalidTrendsException("Indicator trends list size not equal to indicator names list size");
    //---------------------------------------------------------------------------------


    NormalizedTable = FindNormalizedTable(BaseTable, IndicatorTrends);
    normTable = Convert(NormalizedTable);

    if (IsParetoCriterionUsed)
    {
        ParetoSet = FindParetoOptimalProjectIndexes(NormalizedTable);
        ProjectCount = ParetoSet.size() == 0 ? ProjectNames.size() : ParetoSet.size();
    }
    ThreadInstances = MakeThreadList(ThreadCount, MaxThreadCount);

    for (int i=0; i<ThreadInstances.size(); i++)
    {
        ThreadInstances[i]->start();
    }
    timer.start();
}

void solution::terminate()
{
    foreach (auto thread, ThreadInstances)
        thread->terminate();
}

void solution::join()
{
    foreach (auto thread, ThreadInstances)
        thread->wait();

    ElapsedTimeInMS = timer.elapsed();
    FinishedAllThreads();
}

void solution::FinishedAllThreads()
{
    bool isAllFinished = true;

    foreach (auto thread, ThreadInstances)
        isAllFinished = isAllFinished && thread->isFinished();


    if ((NumberOfTriggring == 0) && isAllFinished)
    {

        NumberOfTriggring++;
        //        TimeElapsed = time.elapsed();

        QVector<double> hardRatings(ProjectCount, 0);
        QVector<double> softRatings(ProjectCount, 0);

        QVector<double> partOfHardRating;
        QVector<double> partOfSoftRating;

        int MissedCount = 0;


        foreach (auto thread, ThreadInstances)
        {
            MissedCount += thread->GetMissed();

            partOfHardRating = thread->GetHardRatings();
            partOfSoftRating = thread->GetSoftRatings();

            for (int i=0; i < ProjectCount; i++)
            {
                hardRatings[i] += partOfHardRating[i];
                softRatings[i] += partOfSoftRating[i];
            }
        }


        double count = 0;
        foreach (auto value, hardRatings)
            count += value;


        for (int i = 0; i < ProjectCount; i++)
        {
            hardRatings[i] *= 1.0 / count;
            softRatings[i] *= 1.0 / count;
        }

        missedIterationCount = MissedCount;

        HardRating = hardRatings.toList();
        SoftRating = softRatings.toList();

        is_finished = true;
        //        timeElapsedParsed = GetPassedTimeElapsed(TimeElapsed);
    }

}

void solution::SendTotalCount()
{
    int totalCount=0;

    foreach (auto thread, ThreadInstances)
    {
        totalCount += thread->GetCount();
    }

    emit currentIterationCount(totalCount);
}
