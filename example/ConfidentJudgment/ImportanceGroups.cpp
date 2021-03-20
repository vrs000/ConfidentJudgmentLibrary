#include "ImportanceGroups.h"

using namespace CJ;

ImportanceGroups::ImportanceGroups()
{

}

ImportanceGroups::ImportanceGroups(QString chain)
{
    QList<Relation> RelationList;
    int first;
    int second;

    QStringList pairs = chain.remove(chain.size()-1, 1).split(";");

    int RelCharCount = 0;
    foreach (auto pair, pairs)
    {
        RelCharCount = 0;

        foreach (auto ch, pair)
            RelCharCount += !ch.isDigit() ? 1 : 0;

        if (RelCharCount == 1)
        {
            if (pair.contains(">"))
            {
                first = pair.split(">").first().toInt();
                second = pair.split(">").last().toInt();
                RelationList << Relation(first, second, Relation::strict_superiority);
            }

            if (pair.contains("≥") || pair.contains("b"))
            {
                first = pair.split(pair.contains("≥") ? "≥" : "b").first().toInt();
                second = pair.split(pair.contains("≥") ? "≥" : "b").last().toInt();
                RelationList << Relation(first, second, Relation::superiority);
            }

            if (pair.contains("="))
            {
                first = pair.split("=").first().toInt();
                second = pair.split("=").last().toInt();
                RelationList << Relation(first, second, Relation::equivalence);

            }
        }

        if (RelCharCount > 1)
        {
            // 1>2>3>4>5
            // 1>2>3
            //TODO !!!
            // Сделать через регулярные выражения парсинг строчки 0b1<2<3
        }
    }


    this->RelationList = RelationList;
}

ImportanceGroups::ImportanceGroups(QList<Relation> RelationList): RelationList(RelationList)
{
//    this->RelationList = RelationList;
}

QString ImportanceGroups::ToQString()
{
    QString result = "";

    foreach (auto rel, RelationList)
        result += rel.To_QString();

    return result;
}

void ImportanceGroups::SetRelationList(QList<Relation> RelationList)
{
    this->RelationList = RelationList;
}

const QList<Relation> &ImportanceGroups::GetRelationList()
{
    return RelationList;
}

int *ImportanceGroups::GetPreffered()
{
    int* prefferd = new int[RelationList.size()];

    for (int i=0; i<RelationList.size(); i++)
        prefferd[i] = RelationList[i].first;

    return prefferd;
}

int *ImportanceGroups::GetRejected()
{
    int* rejected = new int[RelationList.size()];

    for (int i=0; i<RelationList.size(); i++)
        rejected[i] = RelationList[i].second;

    return rejected;
}

char *ImportanceGroups::GetRelation()
{
    char* relations = new char[RelationList.size()];

    for (int i=0; i<RelationList.size(); i++)
    {
        switch (RelationList[i].type)
        {
        case Relation::superiority:
            relations[i] = '>';
            break;
        case Relation::strict_superiority:
            relations[i] = 'b';
            break;
        case Relation::equivalence:
            relations[i] = '=';
            break;
        }
    }

    return relations;
}
