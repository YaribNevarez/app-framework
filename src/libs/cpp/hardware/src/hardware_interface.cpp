//------------------------------------------------------------------------------
/**
 *
 * @file: com_channel_interface.cpp
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

// INCLUDES --------------------------------------------------------------------
#include "hardware_interface.h"

#include "string.h"
#include <fcntl.h>
#include <unistd.h>

namespace item {
namespace snn {
namespace hardware {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------

HardwareInterface::HardwareInterface(char * driver_name)
{
  if (driver_name)
    strncpy(driver_name_, driver_name, sizeof(driver_name_));
}

HardwareInterface::~HardwareInterface()
{
  // TODO Auto-generated destructor stub
}

Result HardwareInterface::open()
{
  if (strlen(driver_name_))
    file_descriptor_ = ::open(driver_name_, O_RDWR | O_NOCTTY | O_NDELAY);

  if (file_descriptor_ == -1)
  {
   /*
    * Could not open the port.
    */
    sprintf(driver_name_, "Unable to open %s", driver_name_);
    perror(driver_name_);
  }

  return file_descriptor_ != -1 ? OK : ERESOURCE;
}

Result HardwareInterface::close()
{
  if ((file_descriptor_ != -1) && (0 == ::close(file_descriptor_)))
      file_descriptor_ = -1;

  return (file_descriptor_ == -1)? OK: ERROR;
}

bool HardwareInterface::isOpen()
{
  return file_descriptor_ != -1;
}

ssize_t HardwareInterface::write(const void * buffer, size_t size)
{
  return ::write(file_descriptor_, buffer, size);
}

ssize_t HardwareInterface::read(void * buffer, size_t size)
{
  return ::read(file_descriptor_, buffer, size);
}

} // namespace hardware
} // namespace snn
} // namespace item

