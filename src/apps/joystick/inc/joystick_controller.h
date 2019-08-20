//------------------------------------------------------------------------------
/**
 *
 * @file: JoystickController.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - JoystickController application
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2014-2019] Carbon Robotics Inc.
 * All Rights Reserved.
 *
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

namespace item {
namespace snn {
namespace apps {

#define JOY_DEV "/dev/input/js0"
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------
typedef joystick::Joystick Joystick;

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 *  Gateway. Gateway service application
 */
class JoystickController: public app_framework::Application
{
public:
  JoystickController(int argc = 0, char * argv[] = NULL);
	~JoystickController();

	virtual int run(void);

protected:

	virtual Result initialize(void);

	virtual Result joystickControllerProcess(void);
};

} // namespace apps
} // namespace snn
} // namespace item
#endif /* TEMPLATE_APP_H_ */
