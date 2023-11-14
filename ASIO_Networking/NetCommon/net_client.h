#pragma once
#include"net_common.h"
#include"net_connection.h"
#include"net_tsqueue.h"
#include"net_message.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class client_interface
		{
			client_interface() : m_socket(m_context)
			{
				// Initialize the socket with the io context, so it can do stuff
			}
			virtual ~client_interface()
			{
				// If the client is destroyed, always try and disconnect from server.
				Disconnect();
			}

		public:

			// Connect to the server
			bool Connect(const std::string host, const uint16_t port)
			{
				try
				{
					// Create connection
					m_connection = std::make_unique<connection<T>>(); // TODO

					// Resolve hostname/ip-address into tangiable physical address.
					asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					// Tell the connection object to connect to server
					m_connection->ConnectToServer(m_endpoints);

					// Start context thread
					thrContext = std::thread([this]() {m_context.run(); });
				}
				catch(std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}
				return true;
			}
			// Disconnect from server
			void Disconnect()
			{
				// If connection exists, and it's connected them...
				if (IsConnected())
				{
					// ... disconnect from server gracefully
					m_connection->Disconnect();
				}

				// Either way, we're also done with the asio context... 
				m_context.stop();
				// and its thread
				if (thrContext.joinable())
					thrContext.join();

				// Realease connection unique pointer
				m_connection.release();
			}
			// Check if client actually connected to a server
			bool IsConnected()
			{
				return false;
			}

			// Retrieve queue of msg from server
			tsqueue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}
		protected:
			// asio context handles the data transfer...
			asio::io_context m_context;
			// but neeeds a thread of its own to execute its work commands
			std::thread thrContext;
			// This is the hardware socket that is connected to the server
			asio::ip::tcp::socket m_socket;
			// the client has a single instance of a "connection" object, which handles data transfer.
			std::unique_ptr<connection<T>> m_connection;

		private:
			// This is the thread safe queue of incomming msg from server
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}