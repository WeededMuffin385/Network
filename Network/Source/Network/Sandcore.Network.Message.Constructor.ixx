export module Sandcore.Network.Message.Constructor;

import std;
import Sandcore.Network.Message.Byte;

using Identification = std::uint32_t;

namespace Sandcore::Network::Message {
	template <typename T>
	void compileHelper(std::string& message, T& arg) {
		message += byte(arg);
	}

	template <>
	void compileHelper(std::string& message, std::string& arg) {
		message += byte<std::uint32_t>(arg.size()) + arg;
	}

	template <typename T, typename ...Args>
	void compileHelper(std::string& message, T& arg, Args&...args) {
		compileHelper(message, arg);
		compileHelper(message, args...);
	}

	export template <typename ...Args>
	std::string compile(Identification id, Args...args) {
		std::string message = byte(id);
		compileHelper(message, args...);
		return message;
	}

	export
	std::string compile(Identification id) {
		return byte(id);
	}
}

namespace Sandcore::Network::Message {
	template <typename T>
	void decompileHelper(std::string& message, T& arg) {
		arg = unbyte<T>(message.substr(0, sizeof(T)));
		message.erase(0, sizeof(T));
	}

	template <>
	void decompileHelper(std::string& message, std::string& arg) {
		std::uint32_t length = unbyte<std::uint32_t>(message.substr(0, sizeof(std::uint32_t)));
		message.erase(0, sizeof(std::uint32_t));
		arg = message.substr(0, length);
		message.erase(0, length);
	}

	template <typename T, typename ...Args>
	void decompileHelper(std::string& message, T& arg, Args&...args) {
		decompileHelper(message, arg);
		decompileHelper(message, args...);
	}


	export template <typename ...Args>
		void decompile(std::string message, Args&...args) {
		Identification id;
		decompileHelper(message, id, args...);
	}
}

export namespace Sandcore::Network::Message {
	Identification getMessageIdentification(const std::string message) {
		return *reinterpret_cast<const Identification*>(message.data());
	}
}