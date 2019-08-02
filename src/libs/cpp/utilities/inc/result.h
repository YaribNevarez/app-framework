/*
 * result.h
 *
 *  Created on: Jul 29, 2019
 *      Author: nevarez
 */

#ifndef LIBS_CPP_UTILITIES_INC_RESULT_H_
#define LIBS_CPP_UTILITIES_INC_RESULT_H_

#include <errno.h>

typedef int Result;

enum // Custom error codes
{
  OK               = 0x00000000,
  ERROR            = 0xFFFFFFFF,
  EINVALIDARGUMENT = 0xFFFF0000,
  EMISSINGDATA     = 0xFFFF0100,
  EABSENTDATA      = 0xFFFF0200,
  EFORMAT          = 0xFFFF0300,
  ERESOURCE        = 0xFFFF0400
};


#endif /* LIBS_CPP_UTILITIES_INC_RESULT_H_ */
