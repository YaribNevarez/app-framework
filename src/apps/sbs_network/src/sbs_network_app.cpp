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

  // Instantiate SBS Network objects
  sbs::InputLayer       input_layer(24, 24, 50);

  input_layer.load("/home/nevarez/Downloads/MNIST/Pattern/Input_7.bin");

  sbs::Weights          P_IN_H1(2*5*5, 32);

  P_IN_H1.load("/home/nevarez/Downloads/MNIST/W_X_H1_Iter0.bin");

  sbs::ConvolutionLayer H1(24, 24, 32, 1, 1, 0);

  H1.setEpsilon(0.1);

  sbs::Weights          P_H1_H2(32*2*2, 32);

  P_H1_H2.load("/home/nevarez/Downloads/MNIST/W_H1_H2.bin");

  sbs::PoolingLayer     H2(12, 12, 32, 2, false, 32);

  H2.setEpsilon(0.1/4.0);

  sbs::Weights          P_H2_H3(32*5*5, 64);

  P_H2_H3.load("/home/nevarez/Downloads/MNIST/W_H2_H3_Iter0.bin");

  sbs::ConvolutionLayer H3(8, 8, 64, 5, true, 32);

  H3.setEpsilon(0.1/25.0);

  sbs::Weights          P_H3_H4(64*2*2, 64);

  P_H3_H4.load("/home/nevarez/Downloads/MNIST/W_H3_H4.bin");

  sbs::PoolingLayer     H4(4, 4, 64, 2, false, 64);

  H4.setEpsilon(0.1/4.0);

  sbs::Weights          P_H4_H5(64*4*4, 1024);

  P_H4_H5.load("/home/nevarez/Downloads/MNIST/W_H4_H5_Iter0.bin");

  sbs::FullyConnectedLayer H5(1024, 4, false, 64);

  H5.setEpsilon(0.1/16.0);

  sbs::Weights          P_H5_HY(1024, 10);

  P_H5_HY.load("/home/nevarez/Downloads/MNIST/W_H5_HY_Iter0.bin");

  sbs::OutputLayer      HY(10, true, 0);

  HY.setEpsilon(0.1);

  // Assign weights to the layers
  H1.giveWeights(&P_IN_H1);

  H2.giveWeights(&P_H1_H2);

  H3.giveWeights(&P_H2_H3);

  H4.giveWeights(&P_H3_H4);

  H5.giveWeights(&P_H4_H5);

  HY.giveWeights(&P_H5_HY);


  // Update process
  sbs::Spikes spikes_x;
  sbs::Spikes spikes_1;
  sbs::Spikes spikes_2;
  sbs::Spikes spikes_3;
  sbs::Spikes spikes_4;
  sbs::Spikes spikes_5;

  // TODO: Load new Input pattern on the 'input_layer'

  H1.initialize();
  H2.initialize();
  H3.initialize();
  H4.initialize();
  H5.initialize();
  HY.initialize();

  for (uint16_t T = 0; T < 1000; T++)
  {
    spikes_x = input_layer.generateSpikes ();
    spikes_1 = H1.generateSpikes ();
    spikes_2 = H2.generateSpikes ();
    spikes_3 = H3.generateSpikes ();
    spikes_4 = H4.generateSpikes ();
    spikes_5 = H5.generateSpikes ();
//        std::cout << spikes_x.at(0).at(0)+1 << "#";
//    std::cout << spikes_1.at(0).at(0)+1 << "#";
//    std::cout << spikes_2.at(0).at(0)+1 << "#";
//        std::cout << spikes_3.at(0).at(0)+1 << "#";

//    for (uint16_t i = 0; i < 64; i++){
//        std::cout << T << "," << i << ":" << H3.at(0)->at(0)->at(i) << " ";
//    }
//    std::cout << "\n";
//    std::cout << "\n";

    H1.update (spikes_x);
    H2.update (spikes_1);
    H3.update (spikes_2);
    H4.update (spikes_3);
    H5.update (spikes_4);
    HY.update (spikes_5);
  }

     for (uint16_t i = 0; i < 64; i++){
        std::cout << H3.at(0)->at(0)->at(i) << " ";
   }


//       for (uint16_t i = 0; i < 32; i++){
//          std::cout << H2.at(6)->at(6)->at(i) << " ";
//       }


  // Output
  //HY[0][0].data() position of the maximum value is the output

  std::cout << "\n Output value: " << HY.getOutput() << std::endl;
  std::cout << "\n Label value: " << (int)input_layer.getLabel() << std::endl;

  std::cout << std::endl;

  for (uint8_t i = 0; i < 10; i++){
      std::cout << HY.at(0)->at(0)->at(i) << " ";
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
