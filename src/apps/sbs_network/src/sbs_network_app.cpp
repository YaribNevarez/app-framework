//------------------------------------------------------------------------------
/**
 *
 * @file: SbSNetworkApp.cpp
 *
 * @Created on: August 27th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - SbSNetworkApp application
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

#include <result.h>
#include <com_channel.h>

#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */

#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <sbs_network_app.h>
#include <termios.h> /* POSIX terminal control definitions */

namespace item {
namespace snn {
namespace apps {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
SbSNetworkApp::SbSNetworkApp(int argc, char * argv[]) :
    Application(argc, argv)
{

}

SbSNetworkApp::~SbSNetworkApp()
{

}

Result SbSNetworkApp::initialize(void)
{
  if (0 < passed_arg("-daemon")) // shell argument to become daemon
  {
    // To print on the screen use: Application::BD_NO_REOPEN_STD_FDS
    becomeDaemon(Application::BD_BACKGROUND);
  }

  start_threads(); // Start the registered threads


  return OK;
}

Result SbSNetworkApp::appProcess()
{
  Result result = OK;

  sbs::InputLayer input_layer(28, 28, 2);
  sbs::InferencePopulation IP(32);

  sbs::Weights * p_matrix = new sbs::Weights(50, 32);

  float * array = IP.data();
  uint16_t N = IP.size();

  return result;
}

int SbSNetworkApp::run(void)
{
  initialize();

  return appProcess();
}

} // namespace apps
} // namespace snn
} // namespace item

int main(int argc, char * argv[])
{
  item::snn::apps::SbSNetworkApp app(argc, argv);

  return app.run();
}
