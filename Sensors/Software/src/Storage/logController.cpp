#include "logController.h"
#include "config.h"

#include "flags.h"
#include <string>
#include <Arduino.h>
#include <vector>


#include <rnp_networkmanager.h>
#include <rnp_packet.h>

LogController::LogController(RnpNetworkManager& netman):
_netman(netman)
{};

void LogController::setup(){}




void LogController::log(const std::string &message) {

    MessagePacket_Base<0,100> msg(message);
	msg.header.source = _netman.getAddress();
	msg.header.destination = 1;
	msg.header.uid = 0;
	_netman.sendPacket(msg);
    
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {	

    MessagePacket_Base<0,100> msg("flag: " + std::to_string(flag) + " " + message);
	msg.header.source = _netman.getAddress();
	msg.header.destination = 1;
	msg.header.uid = 0;
	_netman.sendPacket(msg);
}


