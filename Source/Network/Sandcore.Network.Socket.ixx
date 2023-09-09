module;
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
export module Sandcore.Network.Socket;

import std;

export namespace Sandcore {
	class Socket {
	public:
		Socket() {
			socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (socket == INVALID_SOCKET) throw std::exception("Socket creation failed!");
		}

		Socket(SOCKET socket) : socket(socket) {}

		Socket(Socket&& right) { std::swap(socket, right.socket); }
		void operator=(Socket&& right) { std::swap(socket, right.socket); }

		virtual ~Socket() {
			std::println("Socket killed: {}!", socket);
			close();
		}

		auto getAddress() {
			sockaddr_in address;
			int length;
			::getsockname(socket, (sockaddr*)(&address), &length);

			std::string result(INET_ADDRSTRLEN, 0);
			inet_ntop(AF_INET, &address.sin_addr, result.data(), INET_ADDRSTRLEN);
			return result;
		}

		void connect(std::string ip, std::uint16_t port) {
			sockaddr_in address{};
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			
			if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) != 1) throw std::exception("The address is ill formed!");

			auto result = ::connect(socket, (sockaddr*)(&address), sizeof(address));
			if (result == SOCKET_ERROR) throw std::exception("Unable to connect!");
		}

		void close() {
			::closesocket(socket);
		}

		void send(std::string buffer) {
			int flags = 0;
			auto result = ::send(socket, buffer.data(), buffer.length(), flags);
			if (result == SOCKET_ERROR) {
				auto error = WSAGetLastError();
				throw std::exception("Sending failed!");
			}
		}

		void recv(std::string& buffer) {
			int flags = 0;
			auto result = ::recv(socket, buffer.data(), buffer.length(), flags);
			if (result == SOCKET_ERROR) {
				auto error = WSAGetLastError();
				throw std::exception("Recieving failed!");
			}
		}

		void bind(std::uint16_t port) {
			sockaddr_in name{};
			name.sin_family = AF_INET;
			name.sin_addr.s_addr = htonl(INADDR_ANY);
			name.sin_port = htons(port);

			::bind(socket, (sockaddr*)(&name), sizeof(name));
		}
	protected:
		SOCKET socket;
	};
}