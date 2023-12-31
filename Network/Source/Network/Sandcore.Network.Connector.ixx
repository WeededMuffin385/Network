module;
#include "Sandcore.Network.Winsock.hpp"
export module Sandcore.Network.Connector;

import std;
import Sandcore.Network.Socket;

export namespace Sandcore::Network {
	class Connector : private Socket {
	public:
		Connector() { }

		~Connector() {
			std::println("Socket: {} was connector!", socket);
		}

		void listen(std::uint16_t port) {
			bind(port);
			::listen(socket, SOMAXCONN);
		}

		Socket accept() {
			sockaddr_in address;
			int length = sizeof(address);

			auto result = ::accept(socket, (sockaddr*)(&address), &length);
			if (result == INVALID_SOCKET) throw std::exception("Acception failed!");
			return Socket(result);
		}
	};
}