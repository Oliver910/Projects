#include<stdio.h>
#include "CUrlTool.h"
#include "COnVifClient.h"


void dump_getDeviceMngmt(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_DeviceInformation deviceInformation;
    bRet = Onvif.getDeviceMngmt()->GetDeviceInformation(deviceInformation);

    printf("bRet=%d %d\n",bRet,deviceInformation.m_enResult);
    printf("m_strManufacturer=%s\n",deviceInformation.m_strManufacturer.c_str());
    printf("m_strModel=%s\n",deviceInformation.m_strModel.c_str());
    printf("m_strFirmwareVersion=%s\n",deviceInformation.m_strFirmwareVersion.c_str());
    printf("m_strSerialNumber=%s\n",deviceInformation.m_strSerialNumber.c_str());
    printf("m_strHardwareId=%s\n",deviceInformation.m_strHardwareId.c_str());
}
void dump_GetCapabilities(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_Capabilities Info;
    bRet = Onvif.getDeviceMngmt()->GetCapabilities(Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("Info.m_Device.m_strXAddr=%s\n",Info.m_Device.m_strXAddr.c_str());
    printf("Info.m_Events.m_strXAddr=%s\n",Info.m_Events.m_strXAddr.c_str());
    printf("Info.m_Media.m_strXAddr=%s\n",Info.m_Media.m_strXAddr.c_str());
}

void dump_GetScopes(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_Scopes Info;
    bRet = Onvif.getDeviceMngmt()->GetScopes(Info);
    printf("bRet=%d %d %ld\n",bRet,Info.m_enResult,Info.m_lstItem.size());
    std::list<TOnVif_ScopeItem>::iterator it;//m_lstItem;
    for(it=Info.m_lstItem.begin(); it!=Info.m_lstItem.end(); ++it) {
        printf("=========================================\n");
        printf("ScopeDef:%s\n",it->m_strScopeDef.c_str());
        printf("m_strScopeItem:%s\n",it->m_strScopeItem.c_str());
    }
}
void dump_GetDiscoveryMode(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_DiscoveryMode Info;
    bRet = Onvif.getDeviceMngmt()->GetDiscoveryMode(Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("Mode=>%s\n",Info.m_strMode.c_str());
}

void dump_GetDNS(COnVifClient  &Onvif)
{
    bool bRet;
    std::list<TOnVif_DNSFromDHCP>::iterator it,itEnd;

    TOnVif_DNSInformation Info;
    bRet = Onvif.getDeviceMngmt()->GetDNS(Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("FromDHCP=>%s\n",Info.m_strFromDHCP.c_str());
    it = Info.m_lstDNSFromDHCP.begin();
    itEnd = Info.m_lstDNSFromDHCP.end();
    for(it; it!=itEnd; ++it) {
        printf("=========================================\n");
        printf("Type:%s\n",it->m_strType.c_str());
        printf("IPv4Address:%s\n",it->m_strIPv4Address.c_str());
    }
}

void dump_GetHostname(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_Hostname Info;
    bRet = Onvif.getDeviceMngmt()->GetHostname(Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("FromDHCP:%s\n",Info.m_strFromDHCP.c_str());
    printf("Name:%s\n",Info.m_strName.c_str());
}

void dump_GetNetworkDefaultGateway(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_NetworkGateway Info;
    bRet = Onvif.getDeviceMngmt()->GetNetworkDefaultGateway(Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("IPv4Address=>%s\n",Info.m_strIPv4Address.c_str());
}

void dump_GetNetworkInterfaces(COnVifClient  &Onvif)
{
    bool bRet;
    std::list<TOnVif_NetworkInterfaceItem>::iterator it;
    TOnVif_NetworkInterfaces Info;
    bRet = Onvif.getDeviceMngmt()->GetNetworkInterfaces(Info);
    printf("bRet=%d %d %lu\n",bRet,Info.m_enResult,Info.m_lstNIF.size());
    //printf("IPv4Address=>%s\n",Info.m_strIPv4Address.c_str());
    for(it = Info.m_lstNIF.begin(); it!=Info.m_lstNIF.end(); ++it) {
        printf("=====================================================\n");
        printf("Token:%s\n",it->m_strToken.c_str());
        printf("Enabled:%s\n",it->m_strEnabled.c_str());
        printf("info.Name:%s\n",it->m_Info.m_strName.c_str());
        printf("info.HwAddress:%s\n",it->m_Info.m_strHwAddress.c_str());
        printf("info.MTU:%s\n",it->m_Info.m_strMTU.c_str());

        printf("IPv4.Enabled:%s\n",it->m_IPv4.m_strEnabled.c_str());
        printf("IPv4.Config.DHCP:%s\n",it->m_IPv4.m_Config.m_strDHCP.c_str());
        printf("IPv4.FromDHCP.Address:%s\n",it->m_IPv4.m_Config.m_FromDHCP.m_strAddress.c_str());
        printf("IPv4.FromDHCP.PrefixLength:%s\n",it->m_IPv4.m_Config.m_FromDHCP.m_strPrefixLength.c_str());
        printf("IPv4.Manual.Address:%s\n",it->m_IPv4.m_Config.m_Manual.m_strAddress.c_str());
        printf("IPv4.Manual.PrefixLength:%s\n",it->m_IPv4.m_Config.m_Manual.m_strPrefixLength.c_str());
    }
}

void dump_GetNetworkProtocols(COnVifClient  &Onvif)
{
    bool bRet;
    std::list<TOnVif_NetworkProtocolItem>::iterator it;
    TOnVif_NetworkProtocols Info;
    bRet = Onvif.getDeviceMngmt()->GetNetworkProtocols(Info);
    printf("bRet=%d %d %lu\n",bRet,Info.m_enResult,Info.m_lstNP.size());

    for(it = Info.m_lstNP.begin(); it!=Info.m_lstNP.end(); ++it) {
        printf("=====================================================\n");
        printf("Name:%s\n",it->m_strName.c_str());
        printf("Enabled:%s\n",it->m_strEnabled.c_str());
        printf("Port:%s\n",it->m_strPort.c_str());
    }
}

void dump_GetNTP(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_NTPInformation Info;
    bRet = Onvif.getDeviceMngmt()->GetNTP(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    printf("FromDHCP=>%s\n",Info.m_strFromDHCP.c_str());
    printf("NTPManual.Type=>%s\n",Info.m_NTPManual.m_strType.c_str());
    printf("NTPManual.IPv4Address=>%s\n",Info.m_NTPManual.m_strIPv4Address.c_str());
    //printf("IPv4Address=>%s\n",Info.m_strIPv4Address.c_str());
}

void dump_GetGetSystemDateAndTime(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_SystemDateAndTime Info;
    bRet = Onvif.getDeviceMngmt()->GetGetSystemDateAndTime(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);

    printf("m_strDateTimeType=>%s\n",Info.m_strDateTimeType.c_str());
    printf("m_strDaylightSavings=>%s\n",Info.m_strDaylightSavings.c_str());
    printf("m_strTimeZone=>%s\n",Info.m_strTimeZone.c_str());
    printf("UTCDateTime=>%s-%s-%s %s:%s:%s\n"
           ,Info.m_UTCDateTime.m_strYear.c_str()
           ,Info.m_UTCDateTime.m_strMonth.c_str()
           ,Info.m_UTCDateTime.m_strDay.c_str()
           ,Info.m_UTCDateTime.m_strHour.c_str()
           ,Info.m_UTCDateTime.m_strMinute.c_str()
           ,Info.m_UTCDateTime.m_strSecond.c_str()
          );

    printf("LocalDateTime=>%s-%s-%s %s:%s:%s\n"
           ,Info.m_LocalDateTime.m_strYear.c_str()
           ,Info.m_LocalDateTime.m_strMonth.c_str()
           ,Info.m_LocalDateTime.m_strDay.c_str()
           ,Info.m_LocalDateTime.m_strHour.c_str()
           ,Info.m_LocalDateTime.m_strMinute.c_str()
           ,Info.m_LocalDateTime.m_strSecond.c_str()
          );
}

void dump_GetProfiles(COnVifClient  &Onvif)
{
//bool COnVifMediaConf::GetProfiles(TOnVif_Profiles  &Info)
    bool bRet;
    TOnVif_Profiles Info;
    bRet = Onvif.getMediaConf()->GetProfiles(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    Info.dump();

}
void dump_GetVideoSourceConfiguration(COnVifClient  &Onvif)
{
//bool COnVifMediaConf::GetProfiles(TOnVif_Profiles  &Info)
    bool bRet;
    TOnVif_VideoSourceConfiguration Info;
    bRet = Onvif.getMediaConf()->GetVideoSourceConfiguration("VSC_Token_0",Info);
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    Info.dump();

}
void dump_GetVideoSources(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_VideoSources Info;
    bRet = Onvif.getMediaConf()->GetVideoSources(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    Info.dump();
}
void dump_GetMetadataConfigurations(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_GetMetadataConfigurations Info;
    bRet = Onvif.getMediaConf()->GetMetadataConfigurations(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
    Info.dump();
}
void dump_GetStreamUri(COnVifClient  &Onvif)
{
 //   bool bRet;
 //   TOnVif_GetMetadataConfigurations Info;
 //   bRet = Onvif.getMediaConf()->GetMetadataConfigurations(Info)	;
 //   printf("bRet=%d %d\n",bRet,Info.m_enResult);
 //   Info.dump();
    bool bRet;
    TOnVif_Profiles Info;
	TOnVif_StreamUri  InfoUri;
	std::list<TOnVif_Profile>::iterator it;
    bRet = Onvif.getMediaConf()->GetProfiles(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
	for(it=Info.m_lstProfile.begin();it!=Info.m_lstProfile.end();++it){
		printf("profile=>%s\n",it->m_strToken.c_str());
		bRet = Onvif.getMediaConf()->GetStreamUri(it->m_strToken,InfoUri)	;
		InfoUri.dump();
	}
	//GetStreamUri(std::string strToken, TOnVif_StreamUri  &Info)
    //Info.dump();

}

void dump_GetSnapshotUri(COnVifClient  &Onvif)
{
    bool bRet;
    TOnVif_Profiles Info;
	TOnVif_StreamUri  InfoUri;
	std::list<TOnVif_Profile>::iterator it;
    bRet = Onvif.getMediaConf()->GetProfiles(Info)	;
    printf("bRet=%d %d\n",bRet,Info.m_enResult);
	for(it=Info.m_lstProfile.begin();it!=Info.m_lstProfile.end();++it){
		printf("profile=>%s\n",it->m_strToken.c_str());
		bRet = Onvif.getMediaConf()->GetSnapshotUri(it->m_strToken,InfoUri)	;
		InfoUri.dump();
	}
}
int main(const int argc, const char *argv[])
{
    COnVifClient  Onvif;
    Onvif.set_host("192.168.0.15",80);
    //dump_getDeviceMngmt(Onvif);
    //dump_GetCapabilities(Onvif);
    //dump_GetScopes(Onvif);
    //dump_GetDiscoveryMode(Onvif);
    //dump_GetDNS(Onvif);
    //dump_GetHostname(Onvif);
    //dump_GetNetworkDefaultGateway(Onvif);
    //dump_GetNetworkInterfaces(Onvif);
    //dump_GetNetworkProtocols(Onvif);
    //dump_GetNTP(Onvif);
    //dump_GetGetSystemDateAndTime(Onvif);
    //dump_GetProfiles(Onvif);
    //dump_GetVideoSourceConfiguration(Onvif);
    //dump_GetVideoSources(Onvif);
    //dump_GetMetadataConfigurations(Onvif);
    //dump_GetStreamUri(Onvif);
    dump_GetSnapshotUri(Onvif);
}
