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

#include <fstream>

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
  std::cout << "\n==========  SbS Neural Network  ===============" << std::endl;
  std::cout << "\n==========  MNIST example  ====================" << std::endl;

  sbs::Network network;

  // Instantiate SBS Network objects
  sbs::InputLayer input_layer(24, 24, 50);
  network.push_back(&input_layer);

  sbs::Weights P_IN_H1(2 * 5 * 5, 32, "/home/nevarez/Downloads/MNIST/W_X_H1_Iter0.bin");

  sbs::ConvolutionLayer H1(24, 24, 32, 1, sbs::BaseLayer::WeightSectionShift::ROW_SHIFT, 50);
  H1.setEpsilon(0.1);
  H1.setWeights(&P_IN_H1);
  network.push_back(&H1);

  sbs::Weights P_H1_H2(32 * 2 * 2, 32, "/home/nevarez/Downloads/MNIST/W_H1_H2.bin");

  sbs::PoolingLayer H2(12, 12, 32, 2, sbs::BaseLayer::WeightSectionShift::COLUMN_SHIFT, 32);
  H2.setEpsilon(0.1 / 4.0);
  H2.setWeights(&P_H1_H2);
  network.push_back(&H2);

  sbs::Weights P_H2_H3(32 * 5 * 5, 64, "/home/nevarez/Downloads/MNIST/W_H2_H3_Iter0.bin");

  sbs::ConvolutionLayer H3(8, 8, 64, 5, sbs::BaseLayer::WeightSectionShift::COLUMN_SHIFT, 32);
  H3.setEpsilon(0.1 / 25.0);
  H3.setWeights(&P_H2_H3);
  network.push_back(&H3);

  sbs::Weights P_H3_H4(64 * 2 * 2, 64, "/home/nevarez/Downloads/MNIST/W_H3_H4.bin");

  sbs::PoolingLayer H4(4, 4, 64, 2, sbs::BaseLayer::WeightSectionShift::COLUMN_SHIFT, 64);
  H4.setEpsilon(0.1 / 4.0);
  H4.setWeights(&P_H3_H4);
  network.push_back(&H4);

  sbs::Weights P_H4_H5(64 * 4 * 4, 1024, "/home/nevarez/Downloads/MNIST/W_H4_H5_Iter0.bin");

  sbs::FullyConnectedLayer H5(1024, 4, sbs::BaseLayer::WeightSectionShift::ROW_SHIFT, 64);
  H5.setEpsilon(0.1 / 16.0);
  H5.setWeights(&P_H4_H5);
  network.push_back(&H5);

  sbs::Weights P_H5_HY(1024, 10, "/home/nevarez/Downloads/MNIST/W_H5_HY_Iter0.bin");

  sbs::OutputLayer HY(10, sbs::BaseLayer::WeightSectionShift::ROW_SHIFT, 0);
  HY.setEpsilon(0.1);
  HY.setWeights(&P_H5_HY);
  network.push_back(&HY);

  // Perform Network load pattern and update cycle
  network.loadInput("/home/nevarez/Downloads/MNIST/Pattern/Input_33.bin");
  network.updateCycle(1000);

  std::cout << "\n==========  Results ===========================" << std::endl;

  std::cout << "\n Output value: " << network.getOutput() << std::endl;
  std::cout << "\n Label value: " << (int) network.getInputLabel() << std::endl;

  std::cout << "\n==========  Output layer values ===============" << std::endl;

  for (uint16_t i = 0; i < 10; i++)
  {
    std::cout << " [ " << i << " ] " << HY[0][0][i] << std::endl;
  }

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
