#ifndef IMPORTANCEGROUPS_H
#define IMPORTANCEGROUPS_H

#include <relation.h>
#include <QList>


namespace CJ {
class Relation;

//TODO: сделать ввод групп важности с помощью строки не парсенной
class ImportanceGroups
{
private:
    QList<Relation> RelationList;

public:
    ImportanceGroups();
    ImportanceGroups(QString chain);
    ImportanceGroups(QList<Relation> RelationList);


    QString ToQString();

    //setters
    void SetRelationList(QList<Relation> RelationList);

    //getters
    const QList<Relation>& GetRelationList();
    int* GetPreffered();
    int* GetRejected();
    char* GetRelation();
};
//Q_DECLARE_METATYPE(ImportanceGroups*)
}

#endif // IMPORTANCEGROUPS_H
