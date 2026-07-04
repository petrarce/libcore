//
// Created by ivan on 6/27/26.
//

#ifndef LIBCORE_SOCLISTENER_H
#define LIBCORE_SOCLISTENER_H
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <thread>

namespace net
{
/**
 *
* ---
title: Listening Socket
---
stateDiagram-v2
	[*] --> ConfigureAddress
		note right of ConfigureAddress
			At this state socket is being created.
			As a result SOCK_READY is emitted and socket
			moves to the next state.
		end note
	ConfigureAddress --> Binding: SOCK_READY
		note right of Binding
			At this state socket is bound and is waiting session initiations from outside.
			As a result SOCK_BOUND is emitted.
		end note
	Binding --> Listening: SOCK_BOUND

	Listening --> Accepted: SOCK_ACCEPTED
		note right of Listening
		As soon as connection is accepted the socket will move to accepted state
		by emitting SOCK_ACCEPTED signal.
		end note
	Accepted --> Listening: SICKET_CONTINUE_LISTENING
		note right of Accepted
		In normal case socket will move back to listening for connection.
		Otherwice, if listening criteria will not be reached it will move to stale state.
		end note
	Accepted --> Stale: SOCK_STALE
		note right of Accepted
		in case if socket reached stale condition it will move to stale state.
		In stale state it will unbind ind avoid not listen for new connections anymore.
		end note
	Stale --> Binding: SOCK_UNSTALE
		note right of Stale
		when stale condition is not true anymore the socket will bind back.
		end note


 */
class SocSession
{
public:
	class SocConnectedSession
	{
	public:
		SocConnectedSession() = delete;
		SocConnectedSession(const SocSession&) = delete;
		SocConnectedSession& operator=(const SocSession&) = delete;
		SocConnectedSession(SocConnectedSession&& other)
		{
			mSessionFd = other.mSessionFd;
			other.mSessionFd = -1;
		};
		SocConnectedSession(const std::string& address, uint16_t port);
		~SocConnectedSession();

		[[nodiscard]] std::vector<uint8_t> Read() const;
		void Write(const std::vector<uint8_t>& data) const;

	private:
		friend class SocSession;
		SocConnectedSession(int session_fd);

		int mSessionFd{ -1 };
	};
	SocSession() = default;
	SocSession(SocSession&&) = default;
	~SocSession();
	// non-copyable
	SocSession(const SocSession&) = delete;
	SocSession& operator=(const SocSession&) = delete;

	void SetAddress(uint16_t port);
	void Listen(std::function<void(SocConnectedSession*)> onSessionInitiated);
	static bool IsBound(uint16_t port);
	static bool CanConnect(const std::string& ipAddr, uint16_t port);

private:
	std::optional<int> mListeningSocket{ std::nullopt };
	std::optional<uint16_t> mPort{ std::nullopt };
	std::set<SocSession> mSessions;

	std::atomic<bool> mIsListening{ false };
	std::unique_ptr<std::thread> mListenerThread;
};
} // namespace net

#endif // LIBCORE_SOCLISTENER_H
