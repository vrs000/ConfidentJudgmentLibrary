# ConfidentJudgmentLibrary
## Как установить?
Имеетя возможность подллючить через исходные коды, либо через dll.
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

## Как пользоваться
```C++

```