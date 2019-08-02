//------------------------------------------------------------------------------
/**
 *
 * @file: tcp_socket.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - TcpSocket class, Implements a TCP/IP bridge, provides interface for
 * opening/closing connection, sending/receiving data, and encapsulates operational
 * mechanisms.
 *
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019] ITEM
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
#ifndef TCP_SOCKET_HPP_
#define TCP_SOCKET_HPP_

// INCLUDES --------------------------------------------------------------------
#include <net_interface.h>

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace item {
namespace snn {
namespace network {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------
typedef std::vector<uint8_t> ByteVector;

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 *  A class. A more elaborate class description.
 */
class TcpSocket: public NetInterface
{
public:
	TcpSocket(const char * host_address, uint16_t port);
	TcpSocket(int client_socket, struct sockaddr_in client_address);
	virtual ~TcpSocket(void);

	virtual Result  open(void);
	virtual void   close(void);
	virtual bool   isOpen(void);

	virtual size_t receive(void * buffer, size_t length);
	virtual size_t receive(std::string & string);
	virtual size_t receive(ByteVector & vector);

	virtual size_t send(const void * buffer, size_t length);
	virtual size_t send(const char * array);
	virtual size_t send(std::string & string);
	virtual size_t send(ByteVector &vector);

	virtual void   setBufferLength(size_t length);
	virtual size_t getBufferLength(void);

protected:
	enum
	{
		DEFAULT_BUFFER_LENGTH = 255
	};

	char host_address_[12] = "00.00.00.00";
	uint16_t port_ = 0;

	int    client_socket_ = -1;
	bool   recv_error_flag_ = false;
	size_t buffer_length_ = 0;
	struct sockaddr_in server_address_;
	struct sockaddr_in client_address_;
};

} // namespace network
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* TCP_SOCKET_HPP_ */
