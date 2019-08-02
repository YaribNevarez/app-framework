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
#include "tcp_socket.h"

#include <unistd.h>

namespace item {
namespace snn {
namespace network {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------

TcpSocket::TcpSocket(const char * host_address, uint16_t port):
client_socket_(-1),
buffer_length_(DEFAULT_BUFFER_LENGTH),
port_(port)
{
  if (host_address)
    strcpy(host_address_, host_address);

	memset(&server_address_, 0, sizeof(server_address_));
	memset(&client_address_, 0, sizeof(client_address_));
}

TcpSocket::TcpSocket(int client_socket, struct sockaddr_in client_address):
buffer_length_(DEFAULT_BUFFER_LENGTH),
port_(-1)
{
	TcpSocket::client_socket_ = client_socket;
	TcpSocket::client_address_ = client_address;
}

TcpSocket::~TcpSocket(void)
{
	close();
}


Result TcpSocket::open()
{
	int result = -1;
	client_socket_ = socket(AF_INET , SOCK_STREAM , 0);

	if (client_socket_ != -1)
	{
		server_address_.sin_addr.s_addr = inet_addr(host_address_);
		server_address_.sin_family = AF_INET;
		server_address_.sin_port = htons(port_);

		result = connect(client_socket_,
						 (const struct sockaddr *) &server_address_,
						 sizeof(server_address_));
	}

	return result != -1 ? OK: ERROR;
}

void TcpSocket::close(void)
{
	if (client_socket_ != -1)
	{
		shutdown(client_socket_, SHUT_RDWR);
		::close(client_socket_);
		client_socket_ = -1;
	}
}

size_t TcpSocket::receive(void * buffer, size_t length)
{
  int recv_size = recv(client_socket_, buffer, length, 0);
  recv_error_flag_ = recv_size == -1;
	return recv_size;
}

bool TcpSocket::isOpen(void)
{
  return (client_socket_ != -1) && !recv_error_flag_;
}

size_t TcpSocket::receive(std::string & string)
{
	char * i_string;
	int result = -1;

	i_string = new char [buffer_length_];

	if (i_string != NULL)
	{
		memset(i_string, 0, buffer_length_);
		result = receive((void *)i_string, buffer_length_);

		if (result != -1)
		{
			string = i_string;
			string.resize(result);
		}

		delete [] i_string;
	}

	return result;
}

size_t TcpSocket::receive(ByteVector & vector)
{
	int result;

	ByteVector i_vector(buffer_length_);

	result = receive(i_vector.data(), i_vector.size());

	if (result != -1)
	{
		vector = i_vector;
		vector.resize(result);
	}

	return result;
}

size_t TcpSocket::send(const void * buffer, size_t length)
{
	return ::send(client_socket_, buffer, length, 0);
}

size_t TcpSocket::send(const char * array)
{
	return send((void *) array, strlen(array));
}

size_t TcpSocket::send(std::string & string)
{
	return send((void *)string.c_str(), string.size());
}

size_t TcpSocket::send(ByteVector &vector)
{
	return send((void *)vector.data(), vector.size());
}


void TcpSocket::setBufferLength(size_t length)
{
	buffer_length_ = length;
}

size_t TcpSocket::getBufferLength(void)
{
	return buffer_length_;
}

} // namespace network
} // namespace snn
} // namespace item

