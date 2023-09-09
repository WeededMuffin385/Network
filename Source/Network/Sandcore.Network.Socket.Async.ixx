export module Sandcore.Network.Socket.Async;

import std;
import Sandcore.Network.Socket;
import Sandcore.Network.Executor;

export namespace Sandcore {
	using Callback = std::function<void()>;
	Executor executor;


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
				socket.recv(buffer);
				callback();
			}
		);
	}
}