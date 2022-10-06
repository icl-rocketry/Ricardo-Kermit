#include "commandHandler.h"

#include <vector>
#include <functional>
#include <memory>


#include "stateMachine.h"


#include "commands.h"

#include "flags.h"




#include "Storage/logController.h"

#include "rnp_packet.h"
#include "rnp_interface.h"
#include "commandpacket.h"
#include "TelemetryPacket.h"




CommandHandler::CommandHandler(stateMachine* sm):
_sm(sm)
{};

void CommandHandler::handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr) {


	switch (static_cast<COMMANDS>(CommandPacket::getCommand(*packetptr))) {
		case COMMANDS::Free_Ram:
		{
			FreeRamCommand(*packetptr);
			break;
		}
		case COMMANDS::Telemetry:{
			TelemetryCommand(*packetptr);
			break;
		}
		default:
			//invalid command issued DELETE IT 
			break;
			
	};
	
}

std::function<void(std::unique_ptr<RnpPacketSerialized>)> CommandHandler::getCallback() 
{
	return std::bind(&CommandHandler::handleCommand, this, std::placeholders::_1);
};

void CommandHandler::TelemetryCommand(const RnpPacketSerialized& packet) 
{
	SimpleCommandPacket commandpacket(packet);

	TelemetryPacket telemetry;

	auto raw_sensors = _sm->AnalogSensors;

	telemetry.header.type = static_cast<uint8_t>(CommandHandler::PACKET_TYPES::TELEMETRY_RESPONSE);
	telemetry.header.source = _sm->networkmanager.getAddress();
	telemetry.header.source_service = serviceID;
	telemetry.header.destination = commandpacket.header.source;
	telemetry.header.destination_service = commandpacket.header.source_service;
	telemetry.header.uid = commandpacket.header.uid; 
	telemetry.system_time = millis();

	telemetry.ch0sens = raw_sensors.ch0;
	telemetry.ch1sens = raw_sensors.ch1;
	telemetry.ch2sens = raw_sensors.ch2;
	telemetry.ch3sens = raw_sensors.ch3;
	telemetry.ch4sens = raw_sensors.ch4;
	telemetry.ch5sens = raw_sensors.ch5;
	telemetry.ch6sens = raw_sensors.ch6;
	telemetry.ch7sens = raw_sensors.ch7;
	telemetry.ch8sens = raw_sensors.ch8;
	telemetry.ch9sens = raw_sensors.ch9;


	telemetry.system_status = _sm->systemstatus.getStatus();
	
	_sm->networkmanager.sendPacket(telemetry);

}

void CommandHandler::FreeRamCommand(const RnpPacketSerialized& packet)
{	
	//avliable in all states
	//returning as simple string packet for ease
	//currently only returning free ram
	MessagePacket_Base<0,static_cast<uint8_t>(CommandPacket::TYPES::MESSAGE_RESPONSE)> message("FreeRam: " + std::to_string(esp_get_free_heap_size()));
	message.header.source_service = serviceID;
	message.header.destination_service = packet.header.source_service;
	message.header.source = packet.header.destination;
	message.header.destination = packet.header.source;
	message.header.uid = packet.header.uid;
	_sm->networkmanager.sendPacket(message);
	
}
