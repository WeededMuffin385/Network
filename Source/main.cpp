import std;
import Sandcore.Network.Connector;
import Sandcore.Network.Connection;

using namespace Sandcore;

int main() {
	Connector connector(13);
	Connection b(connector.accept());


	std::println("Hello world");
}