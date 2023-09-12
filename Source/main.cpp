import std;

import Sandcore.Network.Socket.Async;
import Sandcore.Network.Socket;
import Sandcore.Network.Connector;
import Sandcore.Network.Connection;
import Sandcore.Network.Executor;

using namespace Sandcore;

int main() {
	Connector connector(13);

	Socket a;
	Connection b(connector.accept());


	std::println("Hello world");
}