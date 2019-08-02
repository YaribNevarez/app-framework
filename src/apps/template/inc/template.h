//------------------------------------------------------------------------------
/**
 *
 * @file: gateway.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Template application
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
#ifndef TEMPLATE_APP_H_
#define TEMPLATE_APP_H_

// INCLUDES --------------------------------------------------------------------
#include "application.h"
#include "hardware_interface.h"
#include "net_interface.h"
#include "bundle_packet.h"

#include <vector>
#include <chrono>
#include <mutex>
#include <unistd.h>  /* UNIX standard function definitions */
#include <map>

// TODO: Create a JSON config file
#define HOST_IP_ADDRESS  "172.20.04.82"
#define HOST_SOCKET_PORT 5001

namespace item {
namespace snn {
namespace apps {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------
typedef utilities::BundlePacket     BundlePacket;
typedef network::NetInterface       NetInterface;
typedef hardware::HardwareInterface HardwareInterface;

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 *  Gateway. Gateway service application
 */
class Template: public app_framework::Application
{
public:
  Template(int argc = 0, char * argv[] = NULL);
	~Template();

	virtual int run(void);

protected:

	virtual Result initialize(void);

	virtual Result templateProcess(void);

	const char * host_address_ = HOST_IP_ADDRESS;
	const uint16_t port_ = HOST_SOCKET_PORT;
	static const uint16_t byte_timeout_ms_ = 50;

	NetInterface * network_interface_ = nullptr;
};

} // namespace apps
} // namespace snn
} // namespace item
#endif /* TEMPLATE_APP_H_ */
