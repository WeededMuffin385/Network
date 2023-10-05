export module Sandcore.Network.Connection;

import std;

import Sandcore.Network.ThreadSafeQueue;
import Sandcore.Network.Socket.Async;
import Sandcore.Network.Socket;

import Sandcore.Network.Message.Byte;

export namespace Sandcore::Network {
	class Connection {
	public:
		explicit Connection(std::string ip, std::uint16_t port) : header(sizeof(LengthHeader), 0) {
			socket.connect(ip, port);
			recvHelper();
			sendHelper();
		}

		explicit Connection(Socket&& other) : header(sizeof(LengthHeader), 0), socket(std::move(other)) {
			recvHelper();
			sendHelper();
		}

		using LengthHeader = std::uint32_t;

		void send(std::string message) {
			messagesSend.push(byte<LengthHeader>(message.size()) + message);

			if (sendingStopped) {
				sendingStopped = false;
				sendHelper();
			}
		}

		std::string receive() {
			return messagesRecv.front();
		}

		void pop() {
			messagesRecv.pop();
		}

		bool empty() {
			return messagesRecv.empty();
		}
	private:
		void recvHelper() {
			asyncRecv(
				socket,
				header,
				[this](int bytes) {
					std::println("Recv: {} bytes!", bytes);
					recvHelperCallback();
				}
			);
		}

		void recvHelperCallback() {
			std::string message(unbyte<LengthHeader>(header), 0);
			socket.recv(message);
			messagesRecv.push(message);
			recvHelper();
		}

		void sendHelper() {
			if (!messagesSend.empty()) {
				asyncSend(
					socket,
					messagesSend.front(),
					[this](int bytes) {
						std::println("Send: {} bytes!", bytes);
						sendHelperCallback();
					}
				);
			} else sendingStopped = true;
		}

		void sendHelperCallback() {
			messagesSend.pop();
			sendHelper();
		}

		void checkConnection(int bytes) {
			if (bytes <= 0) {
				connected = false;
				std::println("Connection lost!");
			}
		}

		bool connected = true;

		bool sendingStopped = true;

		std::string header;

		ThreadSafeQueue<std::string> messagesRecv;
		ThreadSafeQueue<std::string> messagesSend;

		Socket socket;
	};
}