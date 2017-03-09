#ifndef _CONVIFDEVICEMNGMT_H_
#define _CONVIFDEVICEMNGMT_H_

#include "fOnVifToolBase.h"

class COnVifDeviceMngmt
    :public fOnVifToolBase
{
public:
    COnVifDeviceMngmt(CUrlTool *pUrl);
    virtual ~COnVifDeviceMngmt();
//
    bool GetDeviceInformation(TOnVif_DeviceInformation &Info);
    bool GetCapabilities(TOnVif_Capabilities &Info);
    bool GetScopes(TOnVif_Scopes  &Info);
    bool GetDiscoveryMode(TOnVif_DiscoveryMode  &Info);
    bool GetDNS(TOnVif_DNSInformation  &Info);
    bool GetHostname(TOnVif_Hostname  &Info);
    bool GetNetworkDefaultGateway(TOnVif_NetworkGateway  &Info);
    bool GetNetworkInterfaces(TOnVif_NetworkInterfaces  &Info);
    bool GetNetworkProtocols(TOnVif_NetworkProtocols  &Info);
    bool GetNTP(TOnVif_NTPInformation  &Info);
    bool GetGetSystemDateAndTime(TOnVif_SystemDateAndTime  &Info);

};
#endif
