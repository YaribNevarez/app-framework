//------------------------------------------------------------------------------
/**
 *
 * @file: tcp_server.cpp
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
 * @copyright Copyright [2019] ITEM.
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
#include "tcp_server.h"

namespace item {
namespace snn {
namespace network {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------

TcpServer::TcpServer(uint16_t host_port):
host_port_(host_port),
server_socket_(-1),
bound_(false)
{
	memset(&server_address_, 0, sizeof(server_address_));
}

TcpSocket * TcpServer::accept(void)
{
	TcpSocket * client = NULL;

	if (server_socket_ != -1)
	{
		int                client_socket;
		struct sockaddr_in client_address;
		int                addr_len;

		listen(server_socket_, 1);

		addr_len = sizeof(struct sockaddr_in);

		client_socket = ::accept(server_socket_,
							  (struct sockaddr *) &client_address,
							  (socklen_t*)&addr_len);

		if (client_socket != -1)
		{
			client = new TcpSocket(client_socket, client_address);
		}
	}

	return client;
}

int TcpServer::prepare(void)
{
	int result = EXIT_FAILURE;

	if (server_socket_ == -1)
	{
		server_socket_ = socket(AF_INET , SOCK_STREAM , 0);
	}

	if (server_socket_ != -1)
	{
		if (!bound_)
		{
			server_address_.sin_family = AF_INET;
			server_address_.sin_addr.s_addr = INADDR_ANY;
			server_address_.sin_port = htons(host_port_);

			result = bind(server_socket_,
						  (struct sockaddr *)&server_address_,
						  sizeof(server_address_));

			bound_ = result != -1;
		}
		else
		{
			result = EXIT_SUCCESS;
		}
	}

    return result;
}

void TcpServer::close(void)
{
	if (server_socket_ != -1)
	{
		shutdown(server_socket_, SHUT_RDWR);
		::close(server_socket_);
		server_socket_ = -1;
	}
}

TcpServer::~TcpServer()
{
	close();
}

} // namespace network
} // namespace snn
} // namespace item
