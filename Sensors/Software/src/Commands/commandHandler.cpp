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
#include "rawADCPacket.h"

CommandHandler::CommandHandler(stateMachine *sm) : _sm(sm){};

void CommandHandler::handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr)
{

	switch (static_cast<COMMANDS>(CommandPacket::getCommand(*packetptr)))
	{
	case COMMANDS::Free_Ram:
	{
		FreeRamCommand(*packetptr);
		break;
	}
	case COMMANDS::Telemetry:
	{
		TelemetryCommand(*packetptr);
		break;
	}
	case COMMANDS::rawADC:
	{
		rawADCCommand(*packetptr);
		break;
	}
	default:
		// invalid command issued DELETE IT
		break;
	};
}

std::function<void(std::unique_ptr<RnpPacketSerialized>)> CommandHandler::getCallback()
{
	return std::bind(&CommandHandler::handleCommand, this, std::placeholders::_1);
};

void CommandHandler::TelemetryCommand(const RnpPacketSerialized &packet)
{
	SimpleCommandPacket commandpacket(packet);

	TelemetryPacket telemetry;

	// auto raw_sensors = _sm->AnalogSensors;

	telemetry.header.type = static_cast<uint8_t>(CommandHandler::PACKET_TYPES::TELEMETRY_RESPONSE);
	telemetry.header.source = _sm->networkmanager.getAddress();
	telemetry.header.source_service = serviceID;
	telemetry.header.destination = commandpacket.header.source;
	telemetry.header.destination_service = commandpacket.header.source_service;
	telemetry.header.uid = commandpacket.header.uid;
	telemetry.system_time = millis();

	// change the following with sensor commands:
	telemetry.ch0sens = 0;
	telemetry.ch1sens = 0;
	telemetry.ch2sens = 0;
	telemetry.ch3sens = 0;
	telemetry.ch4sens = 0;
	telemetry.ch5sens = 0;
	telemetry.ch6sens = 0;
	telemetry.ch7sens = 0;
	telemetry.ch8sens = 0;
	telemetry.ch9sens = 0;

	telemetry.system_status = _sm->systemstatus.getStatus();

	_sm->networkmanager.sendPacket(telemetry);
}

void CommandHandler::rawADCCommand(const RnpPacketSerialized &packet)
{
	SimpleCommandPacket commandpacket(packet);

	rawADCPacket ADCraw;

	auto D0 = _sm->ADS0;
	auto D1 = _sm->ADS1;
	auto D2 = _sm->ADS2;

	ADCraw.header.type = static_cast<uint8_t>(CommandHandler::PACKET_TYPES::RAW_ADCs);
	ADCraw.header.source = _sm->networkmanager.getAddress();
	ADCraw.header.source_service = serviceID;
	ADCraw.header.destination = commandpacket.header.source;
	ADCraw.header.destination_service = commandpacket.header.source_service;
	ADCraw.header.uid = commandpacket.header.uid;
	ADCraw.system_time = millis();

	// change the following with sensor commands:
	ADCraw.ch0 = D0.readAdjusted(0);
	ADCraw.ch1 = D0.readAdjusted(1);
	ADCraw.ch2 = D0.readAdjusted(2);
	ADCraw.ch3 = D0.readAdjusted(3);
	ADCraw.ch4 = D1.readAdjusted(0);
	ADCraw.ch5 = D1.readAdjusted(1);
	ADCraw.ch6 = D1.readAdjusted(2);
	ADCraw.ch7 = D1.readAdjusted(3);
	ADCraw.ch8 = D2.readAdjusted(0);
	ADCraw.ch9 = D2.readAdjusted(1);

	ADCraw.system_status = _sm->systemstatus.getStatus();

	_sm->networkmanager.sendPacket(ADCraw);
}

void CommandHandler::FreeRamCommand(const RnpPacketSerialized &packet)
{
	// avliable in all states
	// returning as simple string packet for ease
	// currently only returning free ram
	MessagePacket_Base<0, static_cast<uint8_t>(CommandPacket::TYPES::MESSAGE_RESPONSE)> message("FreeRam: " + std::to_string(esp_get_free_heap_size()));
	message.header.source_service = serviceID;
	message.header.destination_service = packet.header.source_service;
	message.header.source = packet.header.destination;
	message.header.destination = packet.header.source;
	message.header.uid = packet.header.uid;
	_sm->networkmanager.sendPacket(message);
}
