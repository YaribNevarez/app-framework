//------------------------------------------------------------------------------
/**
 *
 * @file: bundle_buffer.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - BundlePacket buffer object implementation
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2014-2019] Carbon Robotics Inc.
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

// INCLUDES --------------------------------------------------------------------
#include "bundle_packet.h"
#include <string.h>

namespace item {
namespace snn {
namespace utilities {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
BundlePacket::BundlePacket (void)
{
  clear ();
}

Result BundlePacket::addPacketBuffer (int packet_id, void * buffer, size_t length)
{
  if (buffer == nullptr)
    return EINVALIDARGUMENT;

  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;
  Packet * packet = bundle_packet->packet_buffer;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 < bundle_packet->num_packets)
    for (uint8_t i = 0; i < bundle_packet->num_packets; i++)
      packet = (Packet *) &packet->data[packet->size];

  packet->ID = packet_id;
  packet->size = length;

  memcpy (packet->data, buffer, length);

  bundle_packet->num_packets++;

  need_update_ = true;

  return OK;
}

void BundlePacket::clear (void)
{
  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);
  need_update_ = true;
  memset (bundle_buffer_, 0, sizeof(bundle_buffer_));
  ((BundlePacketData *) bundle_buffer_)->signature = SIGNATURE;
}

uint8_t BundlePacket::getNumberOfPackets ()
{
  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  return bundle_packet->num_packets;
}

Result BundlePacket::getPacketBuffer (uint8_t packet_index, void * buffer,
                               size_t & length)
{
  if (buffer == nullptr)
    return EINVALIDARGUMENT;

  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 == bundle_packet->num_packets)
    return EMISSINGDATA;

  Packet * packet = bundle_packet->packet_buffer;

  uint8_t i;
  for (i = 0; (i < bundle_packet->num_packets) && (i != packet_index); i++)
    packet = (Packet *) &packet->data[packet->size];

  if (i != packet_index)
    return EABSENTDATA;

  memcpy (buffer, packet->data, packet->size);

  length = packet->size;

  return OK;
}

Result BundlePacket::getPacketBuffer (uint16_t packet_id, void * buffer,
                               size_t & length)
{
  if (buffer == nullptr)
    return EINVALIDARGUMENT;

  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 == bundle_packet->num_packets)
    return EMISSINGDATA;

  Packet * packet = bundle_packet->packet_buffer;

  for (uint8_t i = 0;
      (i < bundle_packet->num_packets) && (packet->ID != packet_id); i++)
    packet = (Packet *) &packet->data[packet->size];

  if (packet->ID != packet_id)
    return EABSENTDATA;

  memcpy (buffer, packet->data, packet->size);

  length = packet->size;

  return OK;
}

Result BundlePacket::takePacketBuffer (uint16_t packet_id, void * buffer,
                                size_t & length)
{
  if (buffer == nullptr)
    return EINVALIDARGUMENT;

  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 == bundle_packet->num_packets)
    return EMISSINGDATA;

  Packet * packet = bundle_packet->packet_buffer;

  for (uint8_t i = 0;
      (i < bundle_packet->num_packets) && (packet->ID != packet_id); i++)
    packet = (Packet *) &packet->data[packet->size];

  if (packet->ID != packet_id)
    return EABSENTDATA;

  memcpy (buffer, packet->data, packet->size);

  length = packet->size;

  // This is the memory chunk to move to take the packet out of the buffer
  size_t mem_size =
      bundle_packet->buffer_size
          - (&packet->data[packet->size]
              - (uint8_t *) bundle_packet->packet_buffer);

  bundle_packet->num_packets--;
  bundle_packet->buffer_size -= sizeof(Packet) - sizeof(uint8_t) + packet->size;

  // Move the buffer chunk and take/"remove" the selected packet
  memcpy (packet, &packet->data[packet->size], mem_size);

  need_update_ = true;

  return OK;
}

Result BundlePacket::getBundleBuffer (void *& buffer, size_t & length)
{
  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 == bundle_packet->num_packets)
    return EMISSINGDATA;

  if (need_update_)
  {
    Packet * last_packet = bundle_packet->packet_buffer;

    for (uint8_t i = 0; i < bundle_packet->num_packets; i++)
      last_packet = (Packet *) &last_packet->data[last_packet->size];

    bundle_packet->buffer_size = (uint16_t) (
        (uint8_t *) last_packet - (uint8_t *) bundle_packet->packet_buffer);

    bundle_packet->crc = calculateCRC ((uint8_t *) bundle_packet->packet_buffer,
                                       bundle_packet->buffer_size);
  }

  length = sizeof(BundlePacketData) - sizeof(Packet) + bundle_packet->buffer_size;

  buffer = bundle_buffer_;

  return OK;
}

Result BundlePacket::setBundleBuffer (void * buffer, size_t length)
{
  if (buffer == nullptr)
    return EINVALIDARGUMENT;

  BundlePacketData * bundle_packet = (BundlePacketData *) buffer;

  if (bundle_packet->signature != SIGNATURE) // Ensure we have a BundlePacketData
    return EFORMAT;

  Packet * last_packet = bundle_packet->packet_buffer;

  for (uint8_t i = 0; i < bundle_packet->num_packets; i++)
    last_packet = (Packet *) &last_packet->data[last_packet->size];

  uint16_t buffer_size = (uint16_t) (
      (uint8_t *) last_packet - (uint8_t *) bundle_packet->packet_buffer);

  // Validate buffer sizes
  if (bundle_packet->buffer_size != buffer_size)
    return EFORMAT;

  // Validate entire buffer length
  if (length
      != (sizeof(BundlePacketData) - sizeof(Packet) + bundle_packet->buffer_size))
    return EFORMAT;

  // Finally validate CRC
  if (bundle_packet->crc
      != calculateCRC ((uint8_t *) bundle_packet->packet_buffer, buffer_size))
    return EFORMAT;

  // Clear the internal buffer
  clear ();

  // Now take the given buffer
  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);
  memcpy (bundle_buffer_, buffer, length);

  return OK;
}

BundlePacket::Packet * BundlePacket::getPacket(uint16_t packet_id)
{
  BundlePacketData * bundle_packet = (BundlePacketData *) bundle_buffer_;

  std::lock_guard < std::mutex > lock (bundle_buffer_mutex_);

  if (0 == bundle_packet->num_packets)
    return nullptr;

  Packet * packet = bundle_packet->packet_buffer;

  for (uint8_t i = 0;
      (i < bundle_packet->num_packets) && (packet->ID != packet_id); i++)
    packet = (Packet *) &packet->data[packet->size];

  if (packet->ID != packet_id)
    return nullptr;

  return packet;
}

Result BundlePacket::addPacket(Packet * packet)
{
  if (packet)
    return addPacketBuffer(packet->ID, packet->data, packet->size);
  return EINVALIDARGUMENT;
}

BundlePacket& BundlePacket::operator=(BundlePacket& other)
{
  void * buffer = nullptr;
  size_t length = 0;

  if (OK == other.getBundleBuffer(buffer, length))
    setBundleBuffer(buffer, length);

  return *this;
}

uint16_t BundlePacket::calculateCRC (uint8_t * buffer, uint16_t length)
{
  if (buffer == nullptr || length == 0)
    return 0;

  uint16_t crc = 0;
  for (uint16_t i = 0; i < length; i++)
    crc += buffer[i];

  return crc;
}

} // namespace utilities
} // namespace snn
} // namespace item

