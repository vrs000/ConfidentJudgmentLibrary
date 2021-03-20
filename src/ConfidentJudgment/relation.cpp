#include "relation.h"

using namespace CJ;

Relation::Relation()
{

}

Relation::Relation(int first, int second, Relation::RelationType type):first(first), second(second), type(type)
{
//    this->first = first;
//    this->second = second;
//    this->type = type;
}

QString Relation::To_QString()
{
    QString symb;

    switch (type)
    {
    case strict_superiority:
        symb = ">";
        break;
    case superiority:
        symb = "≥";
        break;
    case equivalence:
        symb = "=";
        break;
    }

    return QString("%1%2%3;").arg(first)
                            .arg(symb)
            .arg(second);
}

bool Relation::operator ==(const Relation &other) const
{
    return ((first == other.first) && (second == other.second) && (type == other.type));
}
