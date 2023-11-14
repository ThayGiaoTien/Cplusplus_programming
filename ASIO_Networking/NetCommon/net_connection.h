#pragma once

#include"net_tsqueue.h"
#include"net_common.h"
#include"net_message.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class connection : public std::enable_shared_from_this<connection<T>>   
		{
		public:
			connection()
			{

			}
			virtual ~connection()
			{

			}
		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

			bool Send(const message<T>& msg);

		protected:
			// Each connection has a unique socket to a remote 
			asio::ip::tcp::socket m_socket; 

			// This context is shared with the whole asio instance
			asio::io_context& m_asioContext;

			// This thread safe queue holds all messages to be sent to the remote side of this connection
			tsqueue<message<T>> m_qMessageOut;
		};
	}
}