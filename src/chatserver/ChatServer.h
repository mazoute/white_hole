#pragma once
#include "Chat.h"
#include "Client.h"
#include <vector>

#define SERVER_NAME				"Makaki"
#define SERVER_MOTD				"Sisi la famille"
#define MAX_CLIENTS				15					// max client allowed on server
#define MAX_S_PACKETSEND_RETRY	5					// if 5+ packets are not received by the client, then drop it
#define PING_IDLE_TIME			120					// if nothing received for 120s from the client, then ping it.
#define PING_TIMEOUT			100					// client must PONG within 100s
#define AUTH_TIMEOUT			30					// 30s to authenticate

// -------------
// --- CLASS ---
// -------------

// Client To Server - Chat
struct BroadcastCondition
{
	BroadcastCondition(
		sf::Uint64 ignoreClientID = 0,
		ClientState clientState = ClientState::UNKNOWN_CS,
		ClientAttributes clientHasAttributes = ClientAttributes::ATTR_NONE
		)
		: ignoreClientID(ignoreClientID), clientState(clientState),	clientHasAttributes(clientHasAttributes)
	{}

	sf::Uint64 ignoreClientID;
	ClientState clientState;
	ClientAttributes clientHasAttributes;
	
};

class ChatServer
{
public:
	ChatServer(void);
	void create(void);
	void create(unsigned short p_port);
	void addClient(std::shared_ptr<Client> p_client);
	void authenticate(std::shared_ptr<Client> p_client, C2S_Auth p_auth);
	void dropClient(std::shared_ptr<Client> p_client);
	void handlePacket(sf::Packet& p_packet, std::shared_ptr<Client> p_client);
	bool sendPacket(sf::Packet& p_packet, std::shared_ptr<Client> p_client, bool dropClientIfFailed = false);
	void broadcast(sf::Packet& p_packet, BroadcastCondition& p_broadcastCond);
	void disconnect(void);

private:
	sf::SocketSelector selector;
	sf::TcpListener listener;
	std::vector< std::shared_ptr<Client> > clients;	// List to store the future clients
	unsigned short mPort;
	bool mRunning;
	void mRunServer(void);
};