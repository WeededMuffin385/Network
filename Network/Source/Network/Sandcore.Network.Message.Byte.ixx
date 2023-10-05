export module Sandcore.Network.Message.Byte;

import std;

export namespace Sandcore::Network {
	template <typename T>
	std::string byte(T value) {
		std::string bytes(sizeof(T), 0);
		std::memcpy(bytes.data(), &value, sizeof(T));
		return bytes;
	}

	template <typename T>
	T unbyte(std::string bytes) {
		T value;
		std::memcpy(&value, bytes.data(), sizeof(T));
		return value;
	}
}