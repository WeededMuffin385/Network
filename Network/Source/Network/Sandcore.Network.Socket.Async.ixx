export module Sandcore.Network.Socket.Async;

import std;
import Sandcore.Network.Executor;
import Sandcore.Network.Socket;

import Sandcore.Network.Connector;

export namespace Sandcore::Network {
	using CommunicationCallback = std::function<void(int)>; // void callback(int bytes)
	using Callback = std::function<void()>;
	Executor executor;

	void asyncAccept(Connector& connector, Socket& socket, Callback callback, Executor& executor = Sandcore::Network::executor) {
		executor.add(
			[&connector, &socket, callback]() -> bool {
				socket = connector.accept();
				callback();
				return true;
			}
		);
	}

	void asyncSend(Socket& socket, std::string& buffer, CommunicationCallback callback, Executor& executor = Sandcore::Network::executor) {
		executor.add(
			[&socket, &buffer, callback]() -> bool {
				callback(socket.send(buffer));
				return true;
			}
		);
	}

	void asyncRecv(Socket& socket, std::string& buffer, CommunicationCallback callback, Executor& executor = Sandcore::Network::executor) {
		executor.add(
			[&socket, &buffer, callback]() -> bool {
				if (socket.empty()) return false;
				callback(socket.recv(buffer));
				return true;
			}
		);
	}
}