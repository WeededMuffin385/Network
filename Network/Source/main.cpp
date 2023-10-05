import std;
import Sandcore.Network.Connector;
import Sandcore.Network.Connection;

using namespace Sandcore;

int main() {
	Network::Connector connector(13);
	Network::Connection b(connector.accept());


	std::println("Hello world");
}