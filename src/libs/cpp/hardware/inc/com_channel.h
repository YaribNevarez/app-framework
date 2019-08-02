//------------------------------------------------------------------------------
/**
 *
 * @file: com_channel_interface.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - ComChannel
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

// IFNDEF ----------------------------------------------------------------------
#ifndef UART_CHANNEL_H_
#define UART_CHANNEL_H_

// INCLUDES --------------------------------------------------------------------
#include "hardware_interface.h"
#include "bundle_packet.h"
#include "thread.h"

#include <string>

namespace item {
namespace snn {
namespace hardware {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------
using BundlePacket = utilities::BundlePacket;
using Thread = app_framework::Thread;

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * ComChannel Class, This implements a customized hardware channel
 */
class ComChannel: public HardwareInterface
{
public:
  ComChannel(char * driver_name);
  virtual ~ComChannel();
  virtual Result start(void);
  virtual bool  isReady(void);
  virtual BundlePacket & getQueryBundle(void);
  virtual BundlePacket & getReplyBundle(void);
protected:

  class DispatchProcess: public Thread
  {
  public:
    DispatchProcess(HardwareInterface * hardware_interface);
    virtual int  run(void);
    virtual bool isReady(void);
    virtual BundlePacket & getQueryBundle(void);
    virtual BundlePacket & getReplyBundle(void);
  protected:
    HardwareInterface * hardware_interface_ = nullptr;
    BundlePacket query_bundle_;
    BundlePacket reply_bundle_;
  };

  DispatchProcess dispatch_process_;
};

} /* namespace hardware */
} /* namespace katia_master */
} /* namespace carbon */

// ENDIF -----------------------------------------------------------------------
#endif /* UART_CHANNEL_H_ */
