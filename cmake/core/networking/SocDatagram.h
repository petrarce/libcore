//
// Created by ivan on 7/4/26.
//

#ifndef LIBCORE_SOCDATAGRAM_H
#define LIBCORE_SOCDATAGRAM_H

#include <vector>
#include <optional>
#include <cstdint>
#include <string>
#include <unistd.h>

namespace net
{

class SocDatagram
{
public:
	SocDatagram() = default;
	~SocDatagram()
	{
		if (mSocFd.has_value())
			close(mSocFd.value());
	}
	// bind to the udp address.
	void Listen(const std::optional<std::string>& address, uint16_t port);
	// Connect to remote address udp address.
	void Connect(const std::string& ip_addr, uint16_t port);
	// Read data from remote.
	std::vector<uint8_t> Read();
	// Write data from remote.
	void Write(std::vector<uint8_t> data);

	static bool IsBound(const std::optional<std::string>& addr, uint16_t port);

private:
	std::optional<int> mSocFd{ std::nullopt };
};

} // namespace net

#endif // LIBCORE_SOCDATAGRAM_H
