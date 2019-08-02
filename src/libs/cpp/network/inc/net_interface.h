//------------------------------------------------------------------------------
/**
 *
 * @file: tcp_server.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - NetInterface,
 *
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019] ITEM
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
#ifndef NET_INTERFACE_H_
#define NET_INTERFACE_H_

// INCLUDES --------------------------------------------------------------------
#include <result.h>

#include <string.h>
#include <stdint.h>

namespace item {
namespace snn {
namespace network {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * NetInterface Class,
 */
class NetInterface
{
public:
  NetInterface(void);
  virtual ~NetInterface(void);

  virtual Result open(void) = 0;
  virtual void  close(void) = 0;
  virtual bool isOpen(void) = 0;

  virtual size_t receive(void * buffer, size_t length) = 0;
  virtual size_t send(const void * buffer, size_t length) = 0;

protected:

};

} // namespace network
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* NET_INTERFACE_H_ */
