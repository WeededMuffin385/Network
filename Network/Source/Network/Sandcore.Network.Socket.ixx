module;
#include "Sandcore.Network.Winsock.hpp"
export module Sandcore.Network.Socket;

import std;

struct WinsockInit {
	WinsockInit() : init(WSAStartup(MAKEWORD(2, 2), &data) == 0){
		if (!init) throw std::exception("Unable to init winsock!");
	}

	~WinsockInit() {
		WSACleanup();
	}
	WSAData data;
	bool init = false;
} winsockInit;

export namespace Sandcore::Network {
	const bool debug = true;

	class Socket {
	public:
		Socket() {
			socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (socket == INVALID_SOCKET) throw std::exception("Socket creation failed!");
		}
		virtual ~Socket() {
			clean();
		}

		explicit Socket(SOCKET socket) : socket(socket) {}

		explicit Socket(Socket&& other) {
			if (&other != this) {
				socket = other.socket;
				other.socket = INVALID_SOCKET;
			}
		}
		Socket& operator=(Socket&& other) { 
			if (&other != this){
				clean();
				socket = other.socket;
				other.socket = INVALID_SOCKET;
			}
			return *this;
		}

		Socket(const Socket& other) = delete;
		Socket& operator=(const Socket& other) = delete;

		void clean() {
			if (socket == INVALID_SOCKET) return;
			if constexpr (debug) std::println("Socket killed: {}!", socket);


			auto result = ::closesocket(socket);
			if (result != 0) throw std::exception("Socket was closed with error");
		}


		void connect(std::string ip, std::uint16_t port) {
			sockaddr_in address{};
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			
			if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) != 1) throw std::exception("The address is ill formed!");

			auto result = ::connect(socket, (sockaddr*)(&address), sizeof(address));
			if (result == SOCKET_ERROR) throw std::exception("Unable to connect!");
		}

		auto send(std::string buffer) {
			int flags = 0;
			auto result = ::send(socket, buffer.data(), buffer.length(), flags);
			if (result == SOCKET_ERROR) {
				auto error = WSAGetLastError();
				throw std::exception("Sending failed!");
			}
			return result;
		}

		auto recv(std::string& buffer) {
			int flags = MSG_WAITALL;
			auto result = ::recv(socket, buffer.data(), buffer.length(), flags);
			if (result == SOCKET_ERROR) {
				auto error = WSAGetLastError();
				throw std::exception("Receiving failed!");
			}
			return result;
		}

		void bind(std::uint16_t port) {
			sockaddr_in name{};
			name.sin_family = AF_INET;
			name.sin_addr.s_addr = htonl(INADDR_ANY);
			name.sin_port = htons(port);

			::bind(socket, (sockaddr*)(&name), sizeof(name));
		}

		bool empty() {
			u_long result = -1;
			if (ioctlsocket(socket, FIONREAD, &result) == SOCKET_ERROR) throw std::exception("Wasn't able to check if recv is empty!");
			return result == 0;
		}

		auto get(){
			return socket;
		}

	protected:
		SOCKET socket = INVALID_SOCKET;
	};
}