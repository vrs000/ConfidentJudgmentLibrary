# ConfidentJudgmentLibrary
## Как установить?
Имеетя возможность подключить через исходные коды, либо через dll.
+ **Использование исходников**<br>
В `src` находятся исходные файлы библиотеки в папке `ConfidentJudgment`.<br>
Необходимо скачать папку с исходными файлами и положить её в папку с проектом.<br>
Затем в `.pro` файле прописать
 ```
 include(ConfidentJudgment/ConfidentJudgment.pri)
``` 
+ **Использование dll**<br>
В папке `release` находbтся `.dll` файл и папка `include` c заголовками `.h`<br>
Папку `include` необходимо поместить в папку с проектом 
и в `.pro` файле дописать
 ```
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

HEADERS += \
    include/ConfJudgLib_global.h \
    include/ImportanceGroups.h \
    include/computation_thread.h \
    include/exceptions.h \
    include/relation.h \
    include/solution.h
``` 
`.dll` файл необходимо поместить в папке со сборкой `Release` или `Debug`
<br>В `.pro` файле дописать
 ```
LIBS += ../build-Test_project-Desktop_Qt_5_15_2_MinGW_64_bit-Release/release/ConfJudgLib.dll
 ```

## Использование
Первым делом необходимо подключить заголовок `solution.h`, в нем располагается класс данных, используемый для расчётов.
```C++
#include <solution.h>
using namespace CJ;
```
Все классы заключены в простарнстве имен `CJ`.


Затем необходимо сформировать исходные данные
```
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

```
Создание экземпляра класса и задание ИД.
```
    solution sol;
    sol.SetBaseTable(base);
    sol.SetIndicatorNames(indicator);
    sol.SetProjectNames(projects);
    sol.SetStep(0.1);
    sol.SetIndicatorTrends(trends);
```

Для задания групп важности необходимо создать экземпляр класса `ImportanceGroups`, и установить соотвествующие группы, 

Например, 
```
// Первый способ
QList<Relation> list = {
        Relation(4, 2, Relation::equivalence), // 4 = 2
        Relation(0, 1, Relation::superiority)  // 0 ≥ 1
    };
ImportanceGroups groups(list);
```
Список возможных отношений
* equivalence (=)
* superiority (≥)
* strict_superiorit (>)

```
// Второй способ
ImportanceGroups groups("4=2;0≥1;"); // Вместо `≥` можно использовать `b`
ImportanceGroups groups("4=2;0b1;")
```

Далее нужно установить созданные группы для расчёта
```
sol.SetIndicatorImportanceGroups(groups); // Задание групп важности для показателей
sol.SetProjectImportanceGroups(groups);   // Задание групп важности для проектов
```

Далее необходимо запустить расчёты
```
sol.start(); // Запуск в многопоточном режиме
sol.join();  // Ожидание завершения работы потоков
```
sol.start() может принимать 2 параметра: 
* `ThreadCount` : количество потоков которое следует использовать при расчёте.<br> 
По умолчанию подбирается автоматически в зависимости от сложности расчётов 
* `MaxThreadCount` :  максимально возноможное количество потоков.<br>
По умолчанию равно количеству потоков процессора.

`void start(int ThreadCount = -1, int MaxThreadCount = QThread::idealThreadCount());`
    
По завершению расчётов имеется возможность получить данные полученные в процессе
~~~
    //Рейтинги HR+SR
    QList<double>& GetHardRating();
    QList<double>& GetSoftRating();

    //Названия проектов и показателей
    const QList<QString>& GetProjectNames();
    const QList<QString>& GetIndicatorNames();

    //Таблица с исходными и нормированными показателями
    const QList<QList<double>>& GetBaseTable();
    const QList<QList<double>>& GetNormalizedTable();

    //Время зактраченное на вычисление
    int GetElapsedTimeInMS();
    const QString GetParsedElapsedTime();

    //Количество использованных потоков
    int GetUsedThreadCount();

    //Количество итераций
    int GetRealIterationCount();
    int GetMissedIterationCount();

    //Множество Парето
    const QList<int>& GetParetoOptimalProjectIndexes();
    const QList<QString> GetParetoOptimalProjectNames();
    bool IsParetoUsed();

    //Группы важности
    ImportanceGroups& GetProjectImportanceGroup();
    ImportanceGroups& GetIndicatorImportanceGroup();
~~~

Полный пример находится в `/example`