#ifndef RELATION_H
#define RELATION_H

#include <QString>
#include <ImportanceGroups.h>
#include <QMetaType>

namespace CJ
{
class ImportanceGroups;
class Relation
{
public:
    enum RelationType{
        equivalence,       // ==
        superiority,       // >=
        strict_superiority // >
    };

    Relation();
    Relation(int first, int second, RelationType type);

    RelationType type;
    int first;
    int second;

    QString To_QString();

    bool operator == (const Relation& other) const;
};

}
Q_DECLARE_METATYPE(CJ::Relation)

#endif // RELATION_H
