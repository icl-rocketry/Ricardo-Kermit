/**
 * @file commands.cpp
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Implementation of commands for system
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "commands.h"

#include <librnp/rnp_packet.h>
#include <libriccore/commands/commandhandler.h>

#include "system.h"
#include "commands/processedsensorpacket.h"
#include "commands/rawADCPacket.h"

void Commands::FreeRamCommand(System& sm, const RnpPacketSerialized& packet)
{	
	//avliable in all states
	//returning as simple string packet for ease
	//currently only returning free ram
	MessagePacket_Base<0,static_cast<uint8_t>(decltype(System::commandhandler)::PACKET_TYPES::MESSAGE_RESPONSE)> message("FreeRam: " + std::to_string(esp_get_free_heap_size()));
	// this is not great as it assumes a single command handler with the same service ID
	// would be better if we could pass some context through the function paramters so it has an idea who has called it
	// or make it much clearer that only a single command handler should exist in the system
	message.header.source_service = sm.commandhandler.getServiceID(); 
	
	
	message.header.destination_service = packet.header.source_service;
	message.header.source = packet.header.destination;
	message.header.destination = packet.header.source;
	message.header.uid = packet.header.uid;
	sm.networkmanager.sendPacket(message);
	
}

void Commands::TelemetryCommand(System& sm, const RnpPacketSerialized& packet)
{
	SimpleCommandPacket commandpacket(packet);

	ProcessedSensorPacket processedSensorPacket;

	// auto raw_sensors = _sm->AnalogSensors;

	processedSensorPacket.header.type = 103;
	processedSensorPacket.header.source = sm.networkmanager.getAddress();
	processedSensorPacket.header.source_service = sm.commandhandler.getServiceID();
	processedSensorPacket.header.destination = commandpacket.header.source;
	processedSensorPacket.header.destination_service = commandpacket.header.source_service;
	processedSensorPacket.header.uid = commandpacket.header.uid;
	processedSensorPacket.system_time = millis();

	processedSensorPacket.ch0sens = sm.CPT0.getValue();
	processedSensorPacket.ch1sens = sm.CPT1.getValue();
	processedSensorPacket.ch2sens = sm.LC0.getWeight();
	processedSensorPacket.ch3sens = sm.LC1.getWeight();
	processedSensorPacket.ch4sens = sm.ADC0.getOutput(4);
	processedSensorPacket.ch5sens = sm.ADC0.getOutput(5);
	processedSensorPacket.ch6sens = sm.VPT0.getValue(); 
	processedSensorPacket.ch7sens = sm.VPT1.getValue();
	processedSensorPacket.ch8sens = sm.VPT2.getValue();
	processedSensorPacket.ch9sens = sm.VPT3.getValue();
	processedSensorPacket.ch10sens = sm.VPT4.getValue(); 
	processedSensorPacket.ch11sens = sm.VPT5.getValue();

	processedSensorPacket.temp0 = sm.TC0.getTemp();
	processedSensorPacket.temp1 = sm.TC1.getTemp();
	processedSensorPacket.temp2 = sm.TC2.getTemp(); 
	processedSensorPacket.temp3 = sm.TC3.getTemp();

	processedSensorPacket.system_status = sm.systemstatus.getStatus();

	sm.networkmanager.sendPacket(processedSensorPacket);
}

void Commands::rawADCCommand(System& sm, const RnpPacketSerialized& packet)
{
	SimpleCommandPacket commandpacket(packet);

	RawADCPacket rawSensors;

	rawSensors.header.type = 104;
	rawSensors.header.source = sm.networkmanager.getAddress();
	rawSensors.header.source_service = sm.commandhandler.getServiceID();
	rawSensors.header.destination = commandpacket.header.source;
	rawSensors.header.destination_service = commandpacket.header.source_service;
	rawSensors.header.uid = commandpacket.header.uid;
	rawSensors.system_time = millis();

	rawSensors.ch0 = sm.ADC1.getOutput(5);
	rawSensors.ch1 = sm.ADC1.getOutput(4);
	rawSensors.ch2 = sm.ADC1.getOutput(3); 
	rawSensors.ch3 = sm.ADC1.getOutput(2);
	rawSensors.ch4 = sm.ADC1.getOutput(1);
	rawSensors.ch5 = sm.ADC1.getOutput(0);
	rawSensors.ch6 = sm.ADC0.getOutput(5); 
	rawSensors.ch7 = sm.ADC0.getOutput(4);
	rawSensors.ch8 = sm.ADC0.getOutput(3);
	rawSensors.ch9 = sm.ADC0.getOutput(2);
	rawSensors.ch10 = sm.ADC0.getOutput(1); 
	rawSensors.ch11 = sm.ADC0.getOutput(0);

	rawSensors.system_status = sm.systemstatus.getStatus();

	sm.networkmanager.sendPacket(rawSensors);
}