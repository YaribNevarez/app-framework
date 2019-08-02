//------------------------------------------------------------------------------
/**
 *
 * @file: bundle.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Bundle buffer object implementation
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2014-2019] ITEM.
 * All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains,
 * the property of Carbon Robotics Inc. The intellectual
 * and technical concepts contained herein are proprietary to
 * Carbon Robotics Inc., may be covered by U.S. and Foreign
 * Patents or patents in process, and are protected by trade secret or
 * copyright law. Dissemination, reproduction, or use of this material is
 * strictly forbidden unless prior written permission is obtained from
 * Carbon Robotics Inc.
 *
 */
//------------------------------------------------------------------------------

// IFNDEF ----------------------------------------------------------------------
#ifndef BUNDLE_H_
#define BUNDLE_H_

// INCLUDES --------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <mutex>
#include <result.h>

namespace item {
namespace snn {
namespace utilities {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 *  BundlePacket. Bundle buffer object implementation.
 */
class BundlePacket
{
public:

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
  typedef struct
  {
    uint16_t ID;        // Packet ID
    size_t   size;      // Data size
    uint8_t  data[1];   // Buffer data
  } Packet;
#pragma pack(pop)   /* restore original alignment from stack */

protected:

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
  typedef struct
  {
    uint8_t  signature;
    uint8_t  num_packets;
    size_t   buffer_size;
    uint16_t crc;
    Packet   packet_buffer[1];
  } BundlePacketData;
#pragma pack(pop)   /* restore original alignment from stack */

  // Bundle buffer configurable parameters, adjust these as needed
  static const uint8_t  SIGNATURE          = 0x5A; // BundleBuffer Identifier
  static const uint16_t MAX_NUM_PACKETS    = 6;
  static const uint16_t MAX_PACKET_SIZE    = 0xFF;

  // Do not modify this constant value
  // This is the size of the total transmitting buffer
  static const uint16_t BUNDLE_BUFFER_SIZE = MAX_NUM_PACKETS
      * ((sizeof(Packet) - sizeof(uint8_t)) + MAX_PACKET_SIZE)
      + (sizeof(BundlePacketData) - sizeof(Packet));

public:

  BundlePacket(void);

  virtual Result addPacketBuffer(int packet_id, void * buffer, size_t length);

  virtual void clear(void);

  virtual uint8_t getNumberOfPackets();

  virtual Result getPacketBuffer(uint8_t packet_index, void * buffer, size_t & length);

  virtual Result getPacketBuffer(uint16_t packet_id, void * buffer, size_t & length);

  virtual Result takePacketBuffer(uint16_t packet_id, void * buffer, size_t & length);

  virtual Result getBundleBuffer(void *& buffer, size_t & length);

  virtual Result setBundleBuffer(void * buffer, size_t length);

  virtual Packet * getPacket(uint16_t packet_id);

  virtual Result addPacket(Packet * packet);

  virtual BundlePacket& operator=(BundlePacket& other);

private:

  uint16_t calculateCRC(uint8_t * buffer, uint16_t length);

  uint8_t bundle_buffer_[BUNDLE_BUFFER_SIZE];
  std::mutex bundle_buffer_mutex_;
  bool need_update_ = false;
};

} // namespace utilities
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* BUNDLE_BUFFER_H_ */
