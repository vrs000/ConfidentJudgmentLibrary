#include "exceptions.h"

CJ::CustomException::~CustomException() = default;

const char *CJ::CustomException::what() const noexcept
{
   return msg.data();
}
