//------------------------------------------------------------------------------
/**
 *
 * @file: hardware_interface.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - HardwareInterface
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
#ifndef COM_CHANNEL_H_
#define COM_CHANNEL_H_

// INCLUDES --------------------------------------------------------------------
#include <result.h>

#include <string>
#include <stdio.h>


namespace item {
namespace snn {
namespace hardware {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * HardwareInterface Class, Interface for hardware access
 */
class HardwareInterface
{
public:
  HardwareInterface(char * driver_name);
  virtual ~HardwareInterface();

  virtual Result open();
  virtual Result close();
  virtual bool isOpen();
  virtual ssize_t write(const void * buffer, size_t size);
  virtual ssize_t read(void * buffer, size_t size);

protected:
  char driver_name_[50] = "";
  int file_descriptor_ = -1;
};

} // namespace hardware
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* COM_CHANNEL_H_ */
