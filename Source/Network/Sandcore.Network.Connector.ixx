module;
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
export module Sandcore.Network.Connector;

import std;
import Sandcore.Network.Socket;

export namespace Sandcore {
	class Connector : private Socket {
	public:
		Connector(std::uint16_t port) {
			bind(port);
			listen();
		}

		~Connector() {
			std::println("Socket: {} was connector!", socket);
		}

		Socket accept() {
			sockaddr_in address;
			int length = sizeof(address);

			auto result = ::accept(socket, (sockaddr*)(&address), &length);
			if (result == INVALID_SOCKET) throw std::exception("Acception failed!");
			return Socket(result);
		}
	private:
		void listen() {
			::listen(socket, SOMAXCONN);
		}
	};
}