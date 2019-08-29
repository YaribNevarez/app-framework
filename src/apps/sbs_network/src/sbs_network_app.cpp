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

  // ********** Create SBS Neural Network **********

  // Instantiate SBS Network objects
  sbs::InputLayer       input_layer(28, 28, 2);

  sbs::Weights          P_IN_H1(2*5*5, 32);

  sbs::ConvolutionLayer H1(24, 24, 32, 5);

  sbs::Weights          P_H1_H2(32*2*2, 32);

  sbs::PoolingLayer     H2(12, 12, 32, 2);

  sbs::Weights          P_H2_H3(32*5*5, 64);

  sbs::ConvolutionLayer H3(8, 8, 64, 5);

  sbs::Weights          P_H3_H4(64*2*2, 64);

  sbs::PoolingLayer     H4(4, 4, 64, 2);

  sbs::Weights          P_H4_H5(64, 1024);

  sbs::FullyConnectedLayer H5(1, 1, 1024);

  sbs::Weights          P_H5_HY(1024, 10);

  sbs::OutputLayer      HY(10);

  // Assign weights to the layers
  H1.giveWeights(&P_IN_H1);

  H2.giveWeights(&P_H1_H2);

  H3.giveWeights(&P_H2_H3);

  H4.giveWeights(&P_H3_H4);

  H5.giveWeights(&P_H4_H5);

  HY.giveWeights(&P_H5_HY);


  // Update process
  sbs::Spikes spikes;

  spikes = input_layer.generateSpikes();

  H1.update(spikes);
  spikes = H1.generateSpikes();

  H2.update(spikes);
  spikes = H2.generateSpikes();

  H3.update(spikes);
  spikes = H3.generateSpikes();

  H4.update(spikes);
  spikes = H4.generateSpikes();

  H5.update(spikes);
  spikes = H5.generateSpikes();

  HY.update(spikes);
  spikes = HY.generateSpikes();

  // Output
  sbs::SpikeID id =  spikes[0][0];

  std::cout << "\n Output value: " << (int)id << std::endl;

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
