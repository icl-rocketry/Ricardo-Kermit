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
#include "default_packets/simplecommandpacket.h"
#include "processedsensorpacket.h"
#include "rawadcpacket.h"

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

	ProcessedSensorPacket processedSensorPacket;

	// auto raw_sensors = _sm->AnalogSensors;

	processedSensorPacket.header.type = static_cast<uint8_t>(CommandHandler::PACKET_TYPES::PROCESSED_SENSORS_RESPONSE);
	processedSensorPacket.header.source = _sm->networkmanager.getAddress();
	processedSensorPacket.header.source_service = serviceID;
	processedSensorPacket.header.destination = commandpacket.header.source;
	processedSensorPacket.header.destination_service = commandpacket.header.source_service;
	processedSensorPacket.header.uid = commandpacket.header.uid;
	processedSensorPacket.system_time = millis();

	// change the following with sensor commands:
	processedSensorPacket.ch0sens = _sm->loadcell0.getWeight();
	//Serial.println(_sm->loadcell0.getWeight());
	processedSensorPacket.ch1sens = 0;
	processedSensorPacket.ch2sens = _sm->ntctemp0.getTempLinear();
	//Serial.println(_sm->ntctemp0.getTempLinear());
	processedSensorPacket.ch3sens = _sm->ntctemp1.getTempLinear();
	//Serial.println(_sm->ntctemp1.getTempLinear());
	processedSensorPacket.ch4sens = 0;
	processedSensorPacket.ch5sens = 0;
	processedSensorPacket.ch6sens = _sm->ptap1.getPressure();
	processedSensorPacket.ch7sens = 0;
	processedSensorPacket.ch8sens = 0;
	processedSensorPacket.ch9sens = _sm->ptap0.getPressure();

	processedSensorPacket.system_status = _sm->systemstatus.getStatus();

	_sm->networkmanager.sendPacket(processedSensorPacket);
}

void CommandHandler::rawADCCommand(const RnpPacketSerialized &packet)
{
	SimpleCommandPacket commandpacket(packet);

	RawADCPacket ADCraw;

	ADS1219& D0 = _sm->ADS0;
	ADS1219& D1 = _sm->ADS1;
	ADS1219& D2 = _sm->ADS2;

	ADCraw.header.type = static_cast<uint8_t>(CommandHandler::PACKET_TYPES::RAW_ADC_RESPONSE);
	ADCraw.header.source = _sm->networkmanager.getAddress();
	ADCraw.header.source_service = serviceID;
	ADCraw.header.destination = commandpacket.header.source;
	ADCraw.header.destination_service = commandpacket.header.source_service;
	ADCraw.header.uid = commandpacket.header.uid;
	ADCraw.system_time = millis();

	// change the following with sensor commands:
	ADCraw.ch0 = D0.readAdjusted(10);
	ADCraw.ch1 = D0.readAdjusted(10);
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
	MessagePacket_Base<0, static_cast<uint8_t>(PACKET_TYPES::MESSAGE_RESPONSE)> message("FreeRam: " + std::to_string(esp_get_free_heap_size()));
	message.header.source_service = serviceID;
	message.header.destination_service = packet.header.source_service;
	message.header.source = packet.header.destination;
	message.header.destination = packet.header.source;
	message.header.uid = packet.header.uid;
	_sm->networkmanager.sendPacket(message);
}
