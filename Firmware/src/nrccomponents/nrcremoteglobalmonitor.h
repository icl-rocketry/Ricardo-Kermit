#pragma once

#include <librrc/remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include "nrcremoteredline.h"

class NRCRemoteGlobalMonitor: public NRCRemoteSensorBase<NRCRemoteGlobalMonitor>
{
    public:


        NRCRemoteGlobalMonitor(RnpNetworkManager& networkmanager
                    ): 
            NRCRemoteSensorBase(networkmanager)
            {};

        std::vector<NRCRemoteRedline> redlinesVect;
        void update(){
            _value = 1;
                for(uint8_t i=0; ++i; i<redlinesVect.size()-1){
                    if(redlinesVect[i].getValue() == 0){
                        _value = 0; //check all of the limits and if any of them exceed their max value abort the test.
                    }
                }
        };
        

    protected:

        friend class NRCRemoteSensorBase<NRCRemoteGlobalMonitor>;

        
        

};