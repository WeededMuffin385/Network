module;
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
export module Sandcore.Network;

import std;

export namespace Sandcore {
	bool initWinsock = []() {
		WSADATA wsadata{};
		if (WSAStartup(MAKEWORD(2, 2), &wsadata) == 0) return true;
		throw std::exception("Unable to init winsock!");
		return false;
	}();
}