//------------------------------------------------------------------------------
/**
 *
 * @file: Template.cpp
 *
 * @Created on: July 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Template application
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
#include "template.h"

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
#include <termios.h> /* POSIX terminal control definitions */

namespace item {
namespace snn {
namespace apps {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
Template::Template(int argc, char * argv[]) :
    Application(argc, argv)
{

}

Template::~Template()
{

}

Result Template::initialize(void)
{
  if (0 < passed_arg("-daemon")) // shell argument to become daemon
  {
    // To print on the screen use: Application::BD_NO_REOPEN_STD_FDS
    becomeDaemon(Application::BD_BACKGROUND);
  }

  start_threads(); // Start the registered threads

  network_interface_ = new network::TcpSocket(host_address_, port_);

  return OK;
}

Result Template::templateProcess()
{
  Result result = OK;
  std::string input_text;
  std::string received_text;
  bool connection_loop = true;

  network::TcpSocket * socket =
      dynamic_cast<network::TcpSocket *>(network_interface_);

  if (socket == nullptr)
    return ERROR;

  std::cout << "__ Server Tester __" << std::endl;

  while (connection_loop)
  {
    std::cout << "Trying connection..." << std::endl;
    if (OK == socket->open())
    {
      std::cout << "Connection established" << std::endl;
      std::cout
          << "To close connection type 'exit!' as input text, otherwise type message:"
          << std::endl;

      do
      {
        std::cin >> input_text;

        socket->send(input_text);

        socket->receive(received_text);

        std::cout << "Received: " << received_text << std::endl;

        connection_loop = input_text.compare("exit!") != 0;
      } while (connection_loop);

    } else
    {
      std::cout << "Connection error" << std::endl;
      std::cout << "Retry [n/y]? " << std::endl;
      std::cin >> input_text;
      if (input_text.compare("n") == 0)
        connection_loop = false;
    }
  }

  socket->close();

  return result;
}

int Template::run(void)
{
  initialize();

  return templateProcess();
}

} // namespace apps
} // namespace snn
} // namespace item

int main(int argc, char * argv[])
{
  item::snn::apps::Template app(argc, argv);

  return app.run();
}
