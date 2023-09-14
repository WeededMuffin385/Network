export module Sandcore.Network.Socket.Async;

import std;
import Sandcore.Network.Executor;
import Sandcore.Network.Socket;

import Sandcore.Network.Connector;

export namespace Sandcore {
	using CommunicationCallback = std::function<void(int)>; // void callback(int bytes)
	using Callback = std::function<void()>;
	Executor executor;

	void asyncAccept(Connector& connector, Socket& socket, Callback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&connector, &socket, callback] {
				socket = connector.accept();
				callback();
			}
		);
	}

	void asyncSend(Socket& socket, std::string& buffer, CommunicationCallback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&socket, &buffer, callback] {
				callback(socket.send(buffer));
			}
		);
	}

	void asyncRecv(Socket& socket, std::string& buffer, CommunicationCallback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&socket, &buffer, callback] {
				if (socket.empty()) throw std::exception("Socket is empty!");
				callback(socket.recv(buffer));
			}
		);
	}
}