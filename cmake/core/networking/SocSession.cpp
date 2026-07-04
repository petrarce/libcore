//
// Created by ivan on 6/27/26.
//

#include "SocSession.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <fmt/format.h>
#include <sys/socket.h>
#include <netinet //in.h>
#include <arpa/inet.h>
#include <glm/ext/scalar_uint_sized.hpp>

using namespace std::chrono_literals;

namespace net
{

void SocSession::Listen(std::function<void(SocConnectedSession*)> onSessionInitiated)
{
	assert(mPort.has_value());
	if (mListeningSocket.has_value())
		throw std::runtime_error(fmt::format("Socket was already created"));
	auto soc = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (soc == -1)
		throw std::runtime_error(fmt::format("Cannot create socket: {}", strerror(errno)));

	mListeningSocket = soc;

	sockaddr_in _addr{};
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(mPort.value());
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	auto res = bind(mListeningSocket.value(), reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr));
	if (res == -1)
		throw std::runtime_error(fmt::format("Cannot bind socket: {}", strerror(errno)));
	listen(mListeningSocket.value(), SOMAXCONN);

	mIsListening.store(true);
	mListenerThread = std::make_unique<std::thread>(
		[&]()
		{
			while (mIsListening.load() == true)
			{
				sockaddr addr{};
				socklen_t addr_len{ 0 };
				const auto session_fd = accept(mListeningSocket.value(), &addr, &addr_len);
				if (session_fd == -1)
				{
					std::this_thread::sleep_for(100ms);
					continue;
				}

				std::cout << fmt::format("Connection initiated ") << std::endl;
				onSessionInitiated(new SocConnectedSession{ session_fd });
			}
		});
}

bool SocSession::IsBound(uint16_t port)
{
	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw std::runtime_error(fmt::format("Cannot create socket: {}", strerror(errno)));

	sockaddr_in _addr{};
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	auto res = bind(sock, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr));

	close(sock);
	return res != 0;
}
bool SocSession::CanConnect(const std::string& ipAddr, uint16_t port)
{
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(addr.sin_family, ipAddr.c_str(), &addr.sin_addr.s_addr);

	auto soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc == -1)
		return false;
	auto res = connect(soc, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	close(soc);
	return res == 0;
}

SocSession::SocConnectedSession::SocConnectedSession(const std::string& address, uint16_t port)
{
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(addr.sin_family, address.c_str(), &addr.sin_addr.s_addr);

	auto soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc <= 0)
	{
		std::cout << fmt::format("Failed to create socket to connect {}", strerror(errno))
				  << std::endl;
		return;
	}
	auto res = connect(soc, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (res == -1)
	{
		std::cout << fmt::format("Failed to connect to socket {}", strerror(errno));
		return;
	}
	mSessionFd = soc;
}

SocSession::SocConnectedSession::~SocConnectedSession()
{
	if (mSessionFd != -1)
		close(mSessionFd);
}

std::vector<uint8_t> SocSession::SocConnectedSession::Read() const
{
	std::vector<uint8_t> buffer(1024, 0);
	const auto bytes = recv(mSessionFd, buffer.data(), buffer.size(), 0);
	if (bytes == -1)
		throw std::runtime_error(fmt::format("Cannot read from socket {}", strerror(errno)));
	buffer.resize(bytes);
	return buffer;
}

void SocSession::SocConnectedSession::Write(const std::vector<uint8_t>& data) const
{
	const auto res = send(mSessionFd, data.data(), data.size(), 0);
	if (res == -1)
		throw std::runtime_error(fmt::format("Cannot write to socket {}", strerror(errno)));
	if (res != data.size())
		throw std::runtime_error(
			fmt::format("Sent less then expected: {} instead of {}", res, data.size()));
}
SocSession::SocConnectedSession::SocConnectedSession(int session_fd)
	: mSessionFd(session_fd)
{
}
SocSession::~SocSession()
{
	mIsListening.store(false);
	if (mListenerThread)
		mListenerThread->join();

	if (mListeningSocket.has_value())
		close(mListeningSocket.value());
}
void SocSession::SetAddress(uint16_t port) { mPort = port; }
} // namespace net
