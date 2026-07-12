//
// Created by ivan on 7/4/26.
//

#include "SocDatagram.h"

#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <fmt/format.h>
#include <netinet/in.h>
#include <sys/socket.h>
namespace
{

sockaddr_in create_addr(const std::string& ip, uint16_t port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	return addr;
}

} // namespace
namespace net
{
void SocDatagram::Listen(const std::optional<std::string>& address, uint16_t port)
{
	if (mSocFd.has_value())
	{
		close(mSocFd.value());
		mSocFd.reset();
	}

	mSocFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSocFd.value() == -1)
		throw std::runtime_error(
			fmt::format("Failed to create datagram socket: {}", strerror(errno)));

	const auto addr = create_addr(address.value_or("0.0.0.0"), port);

	auto res = bind(mSocFd.value(), reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
	if (res == -1)
		throw std::runtime_error(
			fmt::format("Failed to bind datagram socket: {}", strerror(errno)));
}

void SocDatagram::Connect(const std::string& ip_addr, uint16_t port)
{
	if (mSocFd.has_value())
		close(mSocFd.value());
	mSocFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSocFd.value() == -1)
		throw std::runtime_error(
			fmt::format("Failed to create datagram socket: {}", strerror(errno)));
	auto addr = create_addr(ip_addr, port);
	connect(mSocFd.value(), reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
}

std::vector<uint8_t> SocDatagram::Read()
{
	if (!mSocFd.has_value())
		throw std::runtime_error("Socket was not initialized");
	std::vector<uint8_t> buf(512);

	size_t len;
	if ((len = recv(mSocFd.value(), reinterpret_cast<void*>(buf.data()), buf.size(), 0)) == -1)
		throw std::runtime_error(
			fmt::format("Failed to read from datagram socket: {}", strerror(errno)));
	return std::vector<uint8_t>(buf.data(), buf.data() + len);
}

void SocDatagram::Write(std::vector<uint8_t> data)
{
	if (!mSocFd.has_value())
		throw std::runtime_error("Socket was not initialized");
	if (send(mSocFd.value(), reinterpret_cast<const void*>(data.data()), data.size(), 0)
		!= data.size())
		throw std::runtime_error(
			fmt::format("Failed to write datagram socket: {}", strerror(errno)));
}
bool SocDatagram::IsBound(const std::optional<std::string>& addr, uint16_t port)
{
	auto soc = socket(AF_INET, SOCK_DGRAM, 0);
	const auto _addr = create_addr(addr.value_or("0.0.0.0"), port);
	const auto res = bind(soc, reinterpret_cast<const sockaddr*>(&_addr), sizeof(_addr));
	const auto already_bound = (res == -1 && errno == EADDRINUSE);
	close(soc);
	return already_bound;
}
} // namespace net
