//------------------------------------------------------------------------------
/**
 *
 * @file: JoystickController.cpp
 *
 * @Created on: July 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - JoystickController application
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019] ITEM.
 * All Rights Reserved.
 *
 *
 */
//------------------------------------------------------------------------------
// INCLUDES --------------------------------------------------------------------
#include "net_interface.h"
#include "application.h"
#include "tcp_socket.h"
#include "logger.h"
#include "joystick.h"

#include <result.h>
#include <com_channel.h>

#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */

#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <joystick_controller.h>
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <fcntl.h>


namespace item {
namespace snn {
namespace apps {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
JoystickController::JoystickController(int argc, char * argv[]) :
    Application(argc, argv)
{

}

JoystickController::~JoystickController()
{

}

Result JoystickController::initialize(void)
{
  if (0 < passed_arg("-daemon")) // shell argument to become daemon
  {
    // To print on the screen use: Application::BD_NO_REOPEN_STD_FDS
    becomeDaemon(Application::BD_BACKGROUND);
  }

  start_threads(); // Start the registered threads

  return OK;
}

Result JoystickController::joystickControllerProcess()
{
  Result result = OK;

  std::cout << "__ Joystick controller application __" << std::endl;

  Joystick js(JOY_DEV);
  unsigned int buttons, axis;
    int kbhit = 0;

  fputs("\e[?25l", stdout);
  system("clear");
  std::cout << "___ Scanning Joystick. Press ENTER to stop ___";

  js.open_device();
  buttons = js.get_number_of_buttons();
  axis = js.get_number_of_axis();

  for(;kbhit == 0;)
  {
    for (unsigned i = 0; i < buttons; i ++)
    {
      printf("%c[%d;%df B[%d]=%d ", 0x1B, i + 2, 10,
           i, js.get_button_value(i));
    }
    for (unsigned i = 0; i < axis; i ++)
    {
      printf("%c[%d;%df A[%d]=%d     ", 0x1B, i + 2 + buttons, 10,
           i, js.get_axis_value(i));
    }
    ioctl(0, FIONREAD, &kbhit);

    /*
    std::cout << (js.get_axis_value(0)>>4);
    std::cout << (js.get_axis_value(2)>>4);
    std::cout << (js.get_axis_value(1)>>4);
    std::cout << (js.get_button_value(0));
    std::cout << (js.get_button_value(1));
    std::cout << (js.get_button_value(2));
    std::cout << (js.get_button_value(3));
    std::cout << (js.get_button_value(4));
    std::cout << (js.get_button_value(5));
    std::cout << (js.get_button_value(6));
    std::cout << (js.get_button_value(7));
    */
  }
  js.close_device();
  getchar();
  fputs("\e[?25h", stdout); /* show the cursor */
  system("clear");

  return result;
}

int JoystickController::run(void)
{
  initialize();

  return joystickControllerProcess();
}

} // namespace apps
} // namespace snn
} // namespace item

int main(int argc, char * argv[])
{
  item::snn::apps::JoystickController app(argc, argv);

  return app.run();
}
