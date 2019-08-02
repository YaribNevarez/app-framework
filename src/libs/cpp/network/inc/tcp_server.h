//------------------------------------------------------------------------------
/**
 *
 * @file: tcp_server.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - TcpServer, This class implements a TCP/IP server that handles
 * incoming connections. Implements the operations for preparing the server,
 * accepting and closing connections.
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
#ifndef TCP_SERVER_HPP_
#define TCP_SERVER_HPP_

// INCLUDES --------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "tcp_socket.h"

namespace item {
namespace snn {
namespace network {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * TcpServer Class, This implements a TCP/IP server that handles incoming
 * connections. Implements the operations for preparing the server, accepting
 * and closing connections.
 */
class TcpServer
{
public:
	TcpServer(uint16_t);

	int          prepare(void);
	TcpSocket *  accept(void);
	virtual void close(void);

	virtual ~TcpServer(void);

private:
	uint16_t           host_port_;
	int                server_socket_;
	struct sockaddr_in server_address_;
	bool               bound_;
};

} // namespace network
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* TCP_SERVER_HPP_ */
