//------------------------------------------------------------------------------
/**
 *
 * @file: logging.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Logging library
 *
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019] ITEM.
 * All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains,
 * the property of Carbon Robotics Inc. The intellectual
 * and technical concepts contained herein are proprietary to
 * Carbon Robotics Inc., may be covered by U.S. and Foreign
 * Patents or patents in process, and are protected by trade secret or
 * copyright law. Dissemination, reproduction, or use of this material is
 * strictly forbidden unless prior written permission is obtained from
 * Carbon Robotics Inc.
 *
 */
//------------------------------------------------------------------------------

// INCLUDES --------------------------------------------------------------------
#include "logger.h"

#include "stdio.h"
#include <cstdarg>

namespace item {
namespace snn {
namespace logging {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
Logger::Logger()
{
  // TODO Auto-generated constructor stub

}

Logger::~Logger()
{
  // TODO Auto-generated destructor stub
}

void Logger::record(Result result, Verbosity verbosity_level,
    const char *file_name, const char *function_name, int line_number,
    const char *format, ...)
{
  static char buffer[256];

  va_list argptr;
  va_start(argptr, format);
  vsnprintf(buffer, sizeof(buffer), format, argptr);
  va_end(argptr);

  printf("\n%s, %s [%d] %s\n", file_name, function_name, line_number, buffer);
}

} /* namespace logging */
} /* namespace snn */
} /* namespace item */
