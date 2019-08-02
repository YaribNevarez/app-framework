//------------------------------------------------------------------------------
/**
 *
 * @file: uart_channel.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - ComChannel,
 *
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
#include <com_channel.h>
#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */

#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>

namespace item {
namespace snn {
namespace hardware {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
ComChannel::ComChannel(char * driver_name):
    HardwareInterface(driver_name),
    dispatch_process_(this)
{
  dispatch_process_.start();
}

ComChannel::~ComChannel()
{
  // TODO Auto-generated destructor stub
}

Result ComChannel::start(void)
{

  return OK;
}

bool  ComChannel::isReady(void)
{
  return false;
}

ComChannel::DispatchProcess::DispatchProcess(HardwareInterface * hardware_interface):
hardware_interface_(hardware_interface)
{
}

int ComChannel::DispatchProcess::run()
{
  while (1)
  {
    for (uint8_t i = 0; i < query_bundle_.getNumberOfPackets(); i ++)
    {
      BundlePacket::Packet * packet = query_bundle_.getPacket(i);
      hardware_interface_->write(packet->data, packet->size);
    }
  }
  return OK;
}

bool ComChannel::DispatchProcess::isReady(void)
{
  return false;
}

BundlePacket & ComChannel::DispatchProcess::getQueryBundle(void)
{
  return query_bundle_;
}

BundlePacket & ComChannel::DispatchProcess::getReplyBundle(void)
{
  return reply_bundle_;
}

BundlePacket & ComChannel::getQueryBundle(void)
{
  return dispatch_process_.getQueryBundle();
}

BundlePacket & ComChannel::getReplyBundle(void)
{
  return dispatch_process_.getReplyBundle();
}


} /* namespace hardware */
} /* namespace snn */
} /* namespace item */
