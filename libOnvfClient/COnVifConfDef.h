#ifndef _CONVIFCONFDEF_H_
#define _CONVIFCONFDEF_H_

#include<string>
#include<list>

class TOnVif_FaultResponse
{
public:
    enum enResult {
        enNONE,
        enOK,
        enFAULT,
        enSOAP_HEADER_ERR,
    };
    enum enResult    m_enResult;
    std::string      m_strFaultBody;
    TOnVif_FaultResponse():m_enResult(enNONE) {}
    virtual ~TOnVif_FaultResponse() {}
};

class TOnVif_DeviceInformation
    :public TOnVif_FaultResponse
{
public:
    std::string m_strManufacturer;
    std::string m_strModel;
    std::string m_strFirmwareVersion;
    std::string m_strSerialNumber;
    std::string m_strHardwareId;
    TOnVif_DeviceInformation():TOnVif_FaultResponse() {}
    virtual ~TOnVif_DeviceInformation() {}
};

class TOnVif_Capabilities
    :public TOnVif_FaultResponse
{
public:

    struct Device {
        std::string m_strXAddr;
        struct Network {
            std::string m_strIPFilter;
            std::string m_strZeroConfiguration;
            std::string m_strIPVersion6;
            std::string m_strDynDNS;
        } m_Network;
        struct System {
            std::string m_strDiscoveryResolve;
            std::string m_strDiscoveryBye;
            std::string m_strRemoteDiscovery;
            std::string m_strSystemBackup;
            std::string m_strSystemLogging;
            std::string m_strFirmwareUpgrade;
            struct SupportedVersions {
                std::string m_strMajor;
                std::string m_strMinor;
            } m_SupportedVersions;
        } m_System;
        struct IO {
            std::string m_strInputConnectors;
            std::string m_strRelayOutputs;
        } m_IO;
        struct Security {
            std::string m_strTLS1_1;
            std::string m_strTLS1_2;
            std::string m_strOnboardKeyGeneration;
            std::string m_strAccessPolicyConfig;
            std::string m_strX_509Token;
            std::string m_strSAMLToken;
            std::string m_strKerberosToken;
            std::string m_strRELToken;
        } m_Security;
    } m_Device;

    struct Events {
        std::string m_strXAddr;
        std::string m_strWSSubscriptionPolicySupport;
        std::string m_strWSPullPointSupport;
        std::string m_strWSPausableSubscriptionManagerInterfaceSupport;
    } m_Events;

    struct Imaging {
        std::string m_strXAddr;
    } m_Imaging ;

    struct Media {
        std::string m_strXAddr;
        struct StreamingCapabilities {
            std::string m_strRTPMulticast;
            std::string m_strRTP_TCP;
            std::string m_strRTP_RTSP_TCP;
        } m_StreamingCapabilities;
    } m_Media;

    struct PTZ {
        std::string m_strXAddr;
    } m_PTZ ;

    TOnVif_Capabilities():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Capabilities() {}
};

class TOnVif_ScopeItem
{
public:
    TOnVif_ScopeItem() {}
    std::string m_strScopeDef;
    std::string m_strScopeItem;
};

class TOnVif_Scopes
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_ScopeItem> m_lstItem;
    TOnVif_Scopes():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Scopes() {}
};

class TOnVif_DiscoveryMode
    :public TOnVif_FaultResponse
{
public:
    std::string m_strMode;
    TOnVif_DiscoveryMode():TOnVif_FaultResponse() {}
    virtual ~TOnVif_DiscoveryMode() {}
};

class TOnVif_DNSFromDHCP {
public:
    std::string m_strType;
    std::string m_strIPv4Address;
} ;

class TOnVif_DNSInformation
    :public TOnVif_FaultResponse
{
public:
    std::string m_strFromDHCP;
    std::list<TOnVif_DNSFromDHCP> m_lstDNSFromDHCP;
    TOnVif_DNSInformation():TOnVif_FaultResponse() {}
    virtual ~TOnVif_DNSInformation() {}
};

class TOnVif_Hostname
    :public TOnVif_FaultResponse
{
public:
    std::string m_strFromDHCP;
    std::string m_strName;
    TOnVif_Hostname():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Hostname() {}
};

class TOnVif_NetworkGateway
    :public TOnVif_FaultResponse
{
public:
    std::string m_strIPv4Address;
    TOnVif_NetworkGateway():TOnVif_FaultResponse() {}
    virtual ~TOnVif_NetworkGateway() {}
};

class TOnVif_NetworkInterfaceItem
{
public:
    std::string m_strToken;
    std::string m_strEnabled;
    struct Info {
        std::string m_strName;
        std::string m_strHwAddress;
        std::string m_strMTU;
    } m_Info;
    struct IPv4 {
        std::string m_strEnabled;
        struct Config {
            std::string m_strDHCP;
            struct FromDHCP {
                std::string m_strAddress;
                std::string m_strPrefixLength;
            } m_FromDHCP;
            struct Manual {
                std::string m_strAddress;
                std::string m_strPrefixLength;
            } m_Manual;
        } m_Config;
    } m_IPv4;

};

class TOnVif_NetworkInterfaces
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_NetworkInterfaceItem> m_lstNIF;
    TOnVif_NetworkInterfaces():TOnVif_FaultResponse() {}
    virtual ~TOnVif_NetworkInterfaces() {}
};
class TOnVif_NetworkProtocolItem
{
public:
    std::string m_strName;
    std::string m_strEnabled;
    std::string m_strPort;
};

class TOnVif_NetworkProtocols
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_NetworkProtocolItem> m_lstNP;
    TOnVif_NetworkProtocols():TOnVif_FaultResponse() {}
    virtual ~TOnVif_NetworkProtocols() {}
};

class TOnVif_NTPInformation
    :public TOnVif_FaultResponse
{
public:
    std::string m_strFromDHCP;
    struct NTPManual {
        std::string m_strType;
        std::string m_strIPv4Address;
    } m_NTPManual;
    TOnVif_NTPInformation():TOnVif_FaultResponse() {}
    virtual ~TOnVif_NTPInformation() {}
};
class TOnVif_SystemDateAndTime
    :public TOnVif_FaultResponse
{
public:
    std::string m_strDateTimeType;
    std::string m_strDaylightSavings;
    std::string m_strTimeZone;
    struct UTCDateTime {
        std::string m_strYear;
        std::string m_strMonth;
        std::string m_strDay;
        std::string m_strHour;
        std::string m_strMinute;
        std::string m_strSecond;
    } m_UTCDateTime;
    struct LocalDateTime {
        std::string m_strYear;
        std::string m_strMonth;
        std::string m_strDay;
        std::string m_strHour;
        std::string m_strMinute;
        std::string m_strSecond;
    } m_LocalDateTime;
    TOnVif_SystemDateAndTime():TOnVif_FaultResponse() {}
    virtual ~TOnVif_SystemDateAndTime() {}
};


//==========media=============
class TOnVif_VideoSourceConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string  m_strToken;
    std::string  m_strName;
    std::string  m_strUseCount;
    std::string  m_strSourceToken;
    struct Bounds {
        std::string m_strX;
        std::string m_strY;
        std::string m_strWidth;
        std::string m_strHeight;
    } m_Bounds;
    void dump() {
        printf("====VideoSourceConfiguration Profile ============\n");
        printf("strToken=%s\n",m_strToken.c_str());
        printf("m_strName=%s\n",m_strName.c_str());
        printf("m_strUseCount=%s\n",m_strUseCount.c_str());
        printf("m_strSourceToken=%s\n",m_strSourceToken.c_str());
        printf("Bounds=[%s,%s,%s,%s]\n"
               ,m_Bounds.m_strX.c_str()
               ,m_Bounds.m_strY.c_str()
               ,m_Bounds.m_strWidth.c_str()
               ,m_Bounds.m_strHeight.c_str()
              );
    }
    TOnVif_VideoSourceConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_VideoSourceConfiguration() {}
};


class TOnVif_AudioSourceConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strName;
    std::string m_strUseCount;
    std::string m_strSourceToken;
    TOnVif_AudioSourceConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_AudioSourceConfiguration() {}
    void dump()
    {
        printf("=============AudioSourceConfiguration=========================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strName:%s\n",m_strName.c_str());
        printf("m_strUseCount:%s\n",m_strUseCount.c_str());
        printf("m_strSourceToken:%s\n",m_strSourceToken.c_str());
    }
};

class TOnVif_VideoSource
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strFramerate;
    std::string m_strResWidth;
    std::string m_strResHeight;
    TOnVif_VideoSource():TOnVif_FaultResponse() {}
    virtual ~TOnVif_VideoSource() {}
    void dump()
    {
        printf("=============TOnVif_VideoSource==================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strFramerate:%s\n",m_strFramerate.c_str());
        printf("m_strResWidth:%s\n",m_strResWidth.c_str());
        printf("m_strResHeight:%s\n",m_strResHeight.c_str());
    }
};
class TOnVif_Multicast
    :public TOnVif_FaultResponse
{
public:
    std::string m_strMulticastAddressType;
    std::string m_strMulticastAddressIPv4Address;
    std::string m_strMulticastAddressIPv6Address;
    std::string m_strMulticastPort;
    std::string m_strMulticastTTL;
    std::string m_strMulticastAutoStart;
    TOnVif_Multicast():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Multicast() {}
    void dump()
    {
        printf("=============Multicast==================\n");
        printf("m_strMulticastAddressType:%s\n",m_strMulticastAddressType.c_str());
        printf("m_strMulticastAddressIPv4Address:%s\n",m_strMulticastAddressIPv4Address.c_str());
        printf("m_strMulticastAddressIPv6Address:%s\n",m_strMulticastAddressIPv6Address.c_str());
        printf("m_strMulticastPort:%s\n",m_strMulticastPort.c_str());
        printf("m_strMulticastTTL:%s\n",m_strMulticastTTL.c_str());
        printf("m_strMulticastAutoStart:%s\n",m_strMulticastAutoStart.c_str());
    }
};

class TOnVif_VideoEncoderConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strName;
    std::string m_strUseCount;
    std::string m_strEncoding;
    std::string m_strResolutionWidth;
    std::string m_strResolutionHeight;
    std::string m_strQuality;

    std::string m_strRateControlFrameRateLimit;
    std::string m_strRateControlEncodingInterval;
    std::string m_strRateControlBitrateLimit;

    std::string m_strH264GovLength;
    std::string m_strH264H264Profile;

    TOnVif_Multicast m_Multicast;
    std::string m_strSessionTimeout;

    TOnVif_VideoEncoderConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_VideoEncoderConfiguration() {}
    void dump()
    {
        printf("=============VideoEncoderConfiguration==================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strName:%s\n",m_strName.c_str());
        printf("m_strUseCount:%s\n",m_strUseCount.c_str());
        printf("m_strEncoding:%s\n",m_strEncoding.c_str());

        printf("m_strResolutionWidth:%s\n",m_strResolutionWidth.c_str());
        printf("m_strResolutionHeight:%s\n",m_strResolutionHeight.c_str());
        printf("m_strQuality:%s\n",m_strQuality.c_str());

        printf("m_strRateControlFrameRateLimit:%s\n",m_strRateControlFrameRateLimit.c_str());
        printf("m_strRateControlEncodingInterval:%s\n",m_strRateControlEncodingInterval.c_str());
        printf("m_strRateControlBitrateLimit:%s\n",m_strRateControlBitrateLimit.c_str());

        printf("m_strH264GovLength:%s\n",m_strH264GovLength.c_str());
        printf("m_strH264H264Profile:%s\n",m_strH264H264Profile.c_str());

        printf("m_strSessionTimeout:%s\n",m_strSessionTimeout.c_str());
        m_Multicast.dump();
    }
};

class TOnVif_AudioEncoderConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strName;
    std::string m_strUseCount;
    std::string m_strEncoding;

    std::string m_strBitrate;
    std::string m_strSampleRate;

    TOnVif_Multicast m_Multicast;
    std::string m_strSessionTimeout;


    TOnVif_AudioEncoderConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_AudioEncoderConfiguration() {}
    void dump()
    {
        printf("=============AudioEncoderConfiguration==================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strName:%s\n",m_strName.c_str());
        printf("m_strUseCount:%s\n",m_strUseCount.c_str());
        printf("m_strEncoding:%s\n",m_strEncoding.c_str());
        printf("m_strBitrate:%s\n",m_strBitrate.c_str());
        printf("m_strSampleRate:%s\n",m_strSampleRate.c_str());
        printf("m_strSessionTimeout:%s\n",m_strSessionTimeout.c_str());
        m_Multicast.dump();
    }
};


class TOnVif_PTZConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strName;
    std::string m_strUseCount;
    std::string m_strNodeToken;

    std::string m_strDefaultRelativePanTiltTranslationSpace;
    std::string m_strDefaultRelativeZoomTranslationSpace;
    std::string m_strDefaultContinuousPanTiltVelocitySpace;
    std::string m_strDefaultContinuousZoomVelocitySpace;

    std::string m_strDefaultPTZSpeedPanTiltX;
    std::string m_strDefaultPTZSpeedPanTiltY;
    std::string m_strDefaultPTZSpeedPanTiltSpace;
    std::string m_strDefaultPTZSpeedZoomX;
    std::string m_strDefaultPTZSpeedZoomSpace;
    std::string m_strDefaultPTZTimeout;

    TOnVif_PTZConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_PTZConfiguration() {}
    void dump()
    {
        printf("=============PTZConfiguration==================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strName:%s\n",m_strName.c_str());
        printf("m_strUseCount:%s\n",m_strUseCount.c_str());
        printf("m_strNodeToken:%s\n",m_strNodeToken.c_str());
        printf("m_strDefaultRelativePanTiltTranslationSpace:%s\n",m_strDefaultRelativePanTiltTranslationSpace.c_str());
        printf("m_strDefaultRelativeZoomTranslationSpace:%s\n",m_strDefaultRelativeZoomTranslationSpace.c_str());

        printf("m_strDefaultContinuousPanTiltVelocitySpace:%s\n",m_strDefaultContinuousPanTiltVelocitySpace.c_str());
        printf("m_strDefaultContinuousZoomVelocitySpace:%s\n",m_strDefaultContinuousZoomVelocitySpace.c_str());
        printf("m_strDefaultPTZSpeedPanTiltX:%s\n",m_strDefaultPTZSpeedPanTiltX.c_str());
        printf("m_strDefaultPTZSpeedPanTiltY:%s\n",m_strDefaultPTZSpeedPanTiltY.c_str());
        printf("m_strDefaultPTZSpeedPanTiltSpace:%s\n",m_strDefaultPTZSpeedPanTiltSpace.c_str());
        printf("m_strDefaultPTZSpeedZoomX:%s\n",m_strDefaultPTZSpeedZoomX.c_str());
        printf("m_strDefaultPTZSpeedZoomSpace:%s\n",m_strDefaultPTZSpeedZoomSpace.c_str());
        printf("m_strDefaultPTZTimeout:%s\n",m_strDefaultPTZTimeout.c_str());
    }
};

class TOnVif_MetadataConfiguration
    :public TOnVif_FaultResponse
{
public:
    std::string m_strToken;
    std::string m_strName;
    std::string m_strUseCount;
    TOnVif_Multicast m_Multicast;
    TOnVif_MetadataConfiguration():TOnVif_FaultResponse() {}
    virtual ~TOnVif_MetadataConfiguration() {}
    void dump()
    {
        printf("=============TOnVif_MetadataConfiguration==================\n");
        printf("m_strToken:%s\n",m_strToken.c_str());
        printf("m_strName:%s\n",m_strName.c_str());
        printf("m_strUseCount:%s\n",m_strUseCount.c_str());
        m_Multicast.dump();
    }
};



class TOnVif_VideoSources
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_VideoSource> m_lstVS;
    TOnVif_VideoSources():TOnVif_FaultResponse() {}
    virtual ~TOnVif_VideoSources() {}
    void dump()
    {
        std::list<TOnVif_VideoSource>::iterator it;
        for(it=m_lstVS.begin(); it!=m_lstVS.end(); ++it) {
            it->dump();
        }
    }
};
class TOnVif_Profile
    :public TOnVif_FaultResponse
{
public:
	std::string m_strToken;
    std::string m_strName;
    TOnVif_VideoSourceConfiguration  m_VideoSourceConfiguration;
    TOnVif_AudioSourceConfiguration  m_AudioSourceConfiguration;
    TOnVif_VideoEncoderConfiguration m_VideoEncoderConfiguration;
    TOnVif_AudioEncoderConfiguration m_AudioEncoderConfiguration;
    TOnVif_PTZConfiguration    		 m_PTZConfiguration;
    TOnVif_MetadataConfiguration     m_MetadataConfiguration;
    TOnVif_Profile():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Profile() {}
    void dump()
    {
        printf("====Profile Name:%s============\n",m_strName.c_str());
        m_VideoSourceConfiguration.dump();
        m_AudioSourceConfiguration.dump();
        m_VideoEncoderConfiguration.dump();
        m_AudioEncoderConfiguration.dump();
        m_PTZConfiguration.dump();
        m_MetadataConfiguration.dump();
    }
};
class TOnVif_Profiles
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_Profile> m_lstProfile;
    TOnVif_Profiles():TOnVif_FaultResponse() {}
    virtual ~TOnVif_Profiles() {}
    void dump()
    {

        std::list<TOnVif_Profile>::iterator it;
        for(it=m_lstProfile.begin();
                it!=m_lstProfile.end(); ++it) {
            it->dump();
        }
    }
};


class TOnVif_GetMetadataConfigurations
    :public TOnVif_FaultResponse
{
public:
    std::list<TOnVif_MetadataConfiguration> m_lstMetadataConfiguration;
    TOnVif_GetMetadataConfigurations():TOnVif_FaultResponse() {}
    virtual ~TOnVif_GetMetadataConfigurations() {}
    void dump()
    {

        std::list<TOnVif_MetadataConfiguration>::iterator it;
        for(it=m_lstMetadataConfiguration.begin();
                it!=m_lstMetadataConfiguration.end(); ++it) {
            it->dump();
        }
    }
};

class TOnVif_StreamUri
    :public TOnVif_FaultResponse
{
public:
	std::string m_strMediaUri;
	std::string m_strMediaUriInvalidAfterConnect;
	std::string m_strMediaUriInvalidAfterReboot;
	std::string m_strMediaUriTimeout;
	
    TOnVif_StreamUri():TOnVif_FaultResponse() {}
    virtual ~TOnVif_StreamUri() {}
    void dump()
    {
		printf("=====StreamUri========================================================\n");
		printf("m_strMediaUri:%s\n",m_strMediaUri.c_str());
		printf("m_strMediaUriInvalidAfterConnect:%s\n",m_strMediaUriInvalidAfterConnect.c_str());
		printf("m_strMediaUriInvalidAfterReboot:%s\n",m_strMediaUriInvalidAfterReboot.c_str());
		printf("m_strMediaUriTimeout:%s\n",m_strMediaUriTimeout.c_str());
    }
};



#endif
