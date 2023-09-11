import std;

import Sandcore.Network.Socket.Async;
import Sandcore.Network.Socket;
import Sandcore.Network.Connector;
import Sandcore.Network.Executor;

using namespace Sandcore;

void example1() { // Test sockets for client-server connection
	Socket client;
	Socket server;
	Connector connector(13);

	std::thread b([&client]() {
		client.connect("127.0.0.1", 13);
		std::println("connected");
	});

	std::thread a([&connector, &server]() {
		server = std::move(connector.accept());
		});

	if (a.joinable()) a.join();
	if (b.joinable()) b.join();

	client.send("Hello world");

	std::string buffer(11, 0);
	server.recv(buffer);

	std::println("{}:accepted", buffer);
	std::println("finalzed!");
}

void example2() { // Executor example (aka Thread Pool)
	Executor t(8);
	t.add([] {std::println("a"); });
	t.add([] {std::println("b"); });
	t.add([] {std::println("c"); });
	t.add([] {std::println("d"); });
	t.add([] {std::println("e"); });
	t.add([] {std::println("f"); });
	t.add([] {std::println("g"); });
	t.add([] {std::println("h"); });
	t.add([] {std::println("i"); });
}

void example3() { // Async send/recv example
	Socket client;
	Socket server;
	Connector connector(13);

	std::string messageForSend;
	std::string messageForRecv(5, 0);

	std::thread b([&client, &messageForSend]() {
		client.connect("127.0.0.1", 13);
		while (true) {
			std::cin >> messageForSend;
			client.send(messageForSend);
		}

		// asyncSend(client, messageForSend, [&messageForSend] {messageForSend = ""; });
		std::println("connected");
	});

	std::thread a([&connector, &server, &messageForRecv]() {
		server = std::move(connector.accept());
		asyncRecv(server, messageForRecv, [&messageForRecv] {std::println("MESSAGE: {}", messageForRecv); });
	});

	if (a.joinable()) a.join();
	if (b.joinable()) b.join();


	while (true);
}

int main() {
	example3();
}