export module Sandcore.Network.Socket.Async;

import std;
import Sandcore.Network.Socket;
import Sandcore.Network.Executor;
import Sandcore.Network.Connector;

export namespace Sandcore {
	using Callback = std::function<void()>;
	Executor executor;

	void asyncAccept(Connector& connector, Socket& socket, Callback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&connector, &socket, callback] {
				socket = std::move(connector.accept());
				callback();
			}
		);
	}

	void asyncSend(Socket& socket, std::string& buffer, Callback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&socket, &buffer, callback] {
				socket.send(buffer);
				callback();
			}
		);
	}

	void asyncRecv(Socket& socket, std::string& buffer, Callback callback, Executor& executor = Sandcore::executor) {
		executor.add(
			[&socket, &buffer, callback] {
				if (socket.empty()) throw std::exception("Socket is empty!");
				socket.recv(buffer);
				callback();
			}
		);
	}
}