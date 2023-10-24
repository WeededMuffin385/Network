export module Sandcore.Network.Connection;

import std;

import Sandcore.Network.ThreadSafeQueue;
import Sandcore.Network.Socket.Async;
import Sandcore.Network.Socket;

import Sandcore.Network.Message.Byte;

export namespace Sandcore::Network {
	class Connection {
	public:
		using LengthHeader = std::uint32_t;

		explicit Connection() {
			launch();
		}

		explicit Connection(Socket&& other) : socket(std::move(other)) {
			launch();
		}

		void connect(std::string address, std::uint16_t port) {
			socket.connect(address, port);
		}

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
		void launch() {
			recvHelper();
			sendHelper();
		}

		void recvHelper() {
			asyncRecv(
				socket,
				header,
				[this](int bytes) {
					std::println("[Connection] Recv: {} head bytes | {} body bytes", bytes, unbyte<LengthHeader>(header));
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
						std::println("[Connection] Send: {} total bytes!", bytes);
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

		std::string header = std::string(sizeof(LengthHeader), 0);

		ThreadSafeQueue<std::string> messagesRecv;
		ThreadSafeQueue<std::string> messagesSend;

		Socket socket;
	};
}