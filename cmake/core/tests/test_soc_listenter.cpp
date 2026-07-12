#define BOOST_TEST_NO_MAIN

#include "SocDatagram.h"

#include <boost/test/unit_test.hpp>
#include <SocSession.h>
#include <ranges>
#include <thread>
#include <fmt/format.h>

using namespace std::chrono_literals;
using namespace net;

bool wait_for_condition(const std::function<bool()>& condition,
						std::chrono::milliseconds timeout = 100ms)
{
	const auto start = std::chrono::steady_clock::now();
	while (!condition())
	{
		const auto now = std::chrono::steady_clock::now();
		if (now - start > timeout)
			return false;
		std::this_thread::sleep_for(100ms);
	}
	return true;
}

BOOST_AUTO_TEST_SUITE(TestNetworking)

BOOST_AUTO_TEST_CASE(TestSocListenerSetAddress)
{
	uint16_t port = 23214;
	{
		SocSession listener;
		listener.SetAddress(port);
		std::unique_ptr<SocSession::SocConnectedSession> socSession;
		std::mutex socLocker;
		listener.Listen(
			[&](SocSession::SocConnectedSession* session)
			{
				std::lock_guard lock(socLocker);
				socSession.reset(session);
			});
		BOOST_CHECK(SocSession::IsBound(port));

		SocSession::SocConnectedSession clientSocket("127.0.0.1", port);
		BOOST_REQUIRE(wait_for_condition(
			[&]()
			{
				std::lock_guard lock(socLocker);
				return socSession != nullptr;
			},
			5s));
		std::string msg("Hello");
		socSession->Write(std::vector<uint8_t>(msg.begin(), msg.end()));
		auto res = clientSocket.Read();
		BOOST_CHECK_EQUAL(std::string(res.begin(), res.end()), msg);
	}
	BOOST_CHECK(!SocSession::IsBound(port));
}

BOOST_AUTO_TEST_CASE(TestMultipleConnections)
{
	uint16_t port = 33213;
	SocSession listener;
	listener.SetAddress(port);
	std::mutex sessionsLocker;
	std::vector<std::unique_ptr<SocSession::SocConnectedSession> > socSessions;
	listener.Listen(
		[&](SocSession::SocConnectedSession* soc_session)
		{
			std::lock_guard lock(sessionsLocker);
			socSessions.push_back(std::unique_ptr<SocSession::SocConnectedSession>(soc_session));
		});
	BOOST_CHECK(SocSession::IsBound(port));
	BOOST_CHECK(socSessions.empty());
	auto clientSockets = std::vector<SocSession::SocConnectedSession>();
	clientSockets.reserve(2);
	clientSockets.emplace_back("127.0.0.1", port);
	clientSockets.emplace_back("127.0.0.1", port);
	BOOST_REQUIRE(clientSockets.size() == 2);
	BOOST_CHECK(
		wait_for_condition([&]() { return socSessions.size() == clientSockets.size(); }, 5s));
	for (int i = 0; i < clientSockets.size(); i++)
	{
		auto msgFromClientView
			= fmt::format("Hello from client {}", i)
			  | std::ranges::views::transform([](char c) -> uint8_t { return c; });
		auto msgFromServerView
			= fmt::format("Hello from server {}", i)
			  | std::ranges::views::transform([](char c) -> uint8_t { return c; });
		socSessions[i]->Write(std::vector(msgFromServerView.begin(), msgFromServerView.end()));
		clientSockets[i].Write(std::vector(msgFromClientView.begin(), msgFromClientView.end()));
		auto msgFromServer = clientSockets[i].Read();
		auto msgFromClient = socSessions[i]->Read();
		BOOST_CHECK(std::ranges::equal(
			msgFromClient, std::vector(msgFromClientView.begin(), msgFromClientView.end())));
		BOOST_CHECK(std::ranges::equal(
			msgFromServer, std::vector(msgFromServerView.begin(), msgFromServerView.end())));
	}
}

BOOST_AUTO_TEST_CASE(TestDatagramSocketConnection)
{
	SocDatagram socListener;
	socListener.Listen("127.0.0.1", 12345);
	BOOST_CHECK(SocDatagram::IsBound("127.0.0.1", 12345));
	SocDatagram socConnected;
	socConnected.Connect("127.0.0.1", 12345);
	BOOST_REQUIRE(true);
	const auto buf = std::vector<uint8_t>{ 1, 2, 3, 4, 5 };
	socConnected.Write(buf);
	const auto in_buf = socListener.Read();
	BOOST_CHECK(buf == in_buf);
}
BOOST_AUTO_TEST_SUITE_END()
