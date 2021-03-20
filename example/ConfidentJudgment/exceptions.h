#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <QString>
#include <QDebug>

namespace CJ {


//interface
//===============================================
class CustomException:public std::exception{
protected:
    std::string msg;
public:
    CustomException(std::string msg):msg(msg){
        qDebug() << QString(msg.data());
    }
    const char* what() const noexcept;
    virtual ~CustomException() = 0;
};
//===============================================


// Invalid sizes
//------------------------------------------------------------------
class SizeMismatchException: public CustomException{
public:
    SizeMismatchException(std::string msg):CustomException(msg){};
};
//------------------------------------------------------------------


// Invalid trends
//------------------------------------------------------------------
class InvalidTrendsException : public CustomException{
public:
    InvalidTrendsException(std::string msg):CustomException(msg){}
};
//------------------------------------------------------------------


// Invalid base table
//------------------------------------------------------------------
class InvalidBaseTableException : public CustomException{
public:
    InvalidBaseTableException(std::string msg):CustomException(msg){}
};
//------------------------------------------------------------------


// Invalid indicator names
//------------------------------------------------------------------
class InvalidIndicatorNamesException : public CustomException{
public:
    InvalidIndicatorNamesException(std::string msg):CustomException(msg){}
};
//------------------------------------------------------------------


// Invalid project names
//------------------------------------------------------------------
class InvalidProjectNamesException : public CustomException{
public:
    InvalidProjectNamesException(std::string msg):CustomException(msg){}
};
//------------------------------------------------------------------


// Invalid normalized table
//------------------------------------------------------------------
class InvalidNormalizedTableException : public CustomException{
public:
    InvalidNormalizedTableException(std::string msg):CustomException(msg){}
};
//------------------------------------------------------------------
}

#endif // EXCEPTIONS_H
