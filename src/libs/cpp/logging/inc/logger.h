//------------------------------------------------------------------------------
/**
 *
 * @file: com_channel_interface.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - ComChannel
 *
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2014-2019] Carbon Robotics Inc.
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

// IFNDEF ----------------------------------------------------------------------
#ifndef LOGGING_H_
#define LOGGING_H_

#define CALLER_LOCATION __FILE__, __FUNCTION__,__LINE__

#define REC_MASTER_ERROR(RESULT, ...) ::item::snn::logging::Logger::record(RESULT, ::item::snn::logging::Logger::Verbosity::ERROR, CALLER_LOCATION, __VA_ARGS__)

// INCLUDES --------------------------------------------------------------------
#include <result.h>

namespace item {
namespace snn {
namespace logging {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * Logging Class, Interface for communication channel
 */
class Logger
{
public:
  enum class Verbosity {
    // Main enum values
    FATAL = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,

    MIN = FATAL,
    MAX = TRACE,
    NUM = MAX + 1
  };

  Logger();
  virtual ~Logger();


  static void record(Result result, Verbosity verbosity_level, const char *file_name,
      const char *function_name, int line_number, const char *format, ...);
};

} // namespace logging
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* LOGGING_H_ */
