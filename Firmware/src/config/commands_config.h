#pragma once

#include <stdint.h>
#include <unordered_map>
#include <functional>
#include <initializer_list>

#include <libriccore/commands/commandhandler.h>
#include <librnp/rnp_packet.h>

#include "Config/forward_decl.h"
#include "Commands/commands.h"


namespace Commands
{
    enum class ID : uint8_t
    {
        NoCommand = 0,
        GetProcessedData = 8,
        GetRawADC = 9,
        setChamberP = 69,
        setFuelP = 70,
        Free_Ram = 250
    };

    inline std::initializer_list<ID> defaultEnabledCommands = {ID::Free_Ram, ID::GetProcessedData, ID::GetRawADC, ID::setChamberP, ID::setFuelP};

    inline std::unordered_map<ID, std::function<void(ForwardDecl_SystemClass &, const RnpPacketSerialized &)>> command_map{
        {ID::Free_Ram, FreeRamCommand}, {ID::GetProcessedData, TelemetryCommand}, {ID::GetRawADC, rawADCCommand}, {ID::setChamberP, setChamberP}, {ID::setFuelP, setFuelP}};



};