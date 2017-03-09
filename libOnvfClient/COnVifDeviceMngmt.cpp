#include "COnVifDeviceMngmt.h"
COnVifDeviceMngmt::COnVifDeviceMngmt(CUrlTool *pUrl)
    :fOnVifToolBase(pUrl)
{
    m_strCmdUrl = "onvif/device_service";
}

COnVifDeviceMngmt::~COnVifDeviceMngmt()
{
}

bool COnVifDeviceMngmt::GetDeviceInformation(TOnVif_DeviceInformation &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_GetDeviceInformation=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">"
        "<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
        " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
        "<GetDeviceInformation xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>"
        "</s:Body>"
        "</s:Envelope>";

    send_onvif_cmd(szOnVifFmt_GetDeviceInformation,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pDeviceInformationResponse = pBody->children;
        if(xmlStrcmp  (pDeviceInformationResponse->name,BAD_CAST "GetDeviceInformationResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        xmlNodePtr pDeviceInformationItem = pDeviceInformationResponse->children;
        while (pDeviceInformationItem!=NULL) {
            //printf("pDeviceInformation->name=%s\n",pDeviceInformationItem->name);
            if(xmlStrcmp  (pDeviceInformationItem->name,BAD_CAST "Manufacturer")==0) {
                get_xml_string(pDeviceInformationItem,Info.m_strManufacturer);
            } else if(xmlStrcmp  (pDeviceInformationItem->name,BAD_CAST "Model")==0) {
                get_xml_string(pDeviceInformationItem,Info.m_strModel);
            } else if(xmlStrcmp  (pDeviceInformationItem->name,BAD_CAST "FirmwareVersion")==0) {
                get_xml_string(pDeviceInformationItem,Info.m_strFirmwareVersion);
            } else if(xmlStrcmp  (pDeviceInformationItem->name,BAD_CAST "SerialNumber")==0) {
                get_xml_string(pDeviceInformationItem,Info.m_strSerialNumber);
            } else if(xmlStrcmp  (pDeviceInformationItem->name,BAD_CAST "HardwareId")==0) {
                get_xml_string(pDeviceInformationItem,Info.m_strHardwareId);
            }
            pDeviceInformationItem =  pDeviceInformationItem->next;
        }
        Info.m_enResult =  TOnVif_FaultResponse::enOK;
        if(xmlReqDoc!=NULL) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
        }
        return true;
    }
    if(xmlReqDoc!=NULL) {
        xmlFreeDoc(xmlReqDoc);
        xmlReqDoc=NULL;
    }
    return false;
}
bool COnVifDeviceMngmt::GetCapabilities(TOnVif_Capabilities &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\""
        " xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\""
        " xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<s:Body>"
        "<tds:GetCapabilities>"
        "<tds:Category>All</tds:Category>"
        "</tds:GetCapabilities>"
        "</s:Body>"
        "</s:Envelope>";

    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pCapabilitiesResponse = pBody->children;
        if(xmlStrcmp(pCapabilitiesResponse->name,BAD_CAST "GetCapabilitiesResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        xmlNodePtr pCapabilitiesItem = pCapabilitiesResponse->children;

        // xmlDocFormatDump(stdout,pCapabilitiesResponse->doc,1);

        while (pCapabilitiesItem!=NULL) {
            //printf("pDeviceInformationItem->name=%s\n",pCapabilitiesItem->name);
            if(xmlStrcmp  (pCapabilitiesItem->name,BAD_CAST "Capabilities")==0) {
                xmlNodePtr pCapabilities = pCapabilitiesItem->children;
                while(pCapabilities!=NULL) {
                    //printf("pCapabilities->name=%s\n",pCapabilities->name);
                    if(xmlStrcmp  (pCapabilities->name,BAD_CAST "Device")==0) {
                        xmlNodePtr pDevic = pCapabilities->children;
                        while(pDevic!=NULL) {
                            //printf("pDevic->name=%s\n",pDevic->name);
                            if(xmlStrcmp  (pDevic->name,BAD_CAST "XAddr")==0) {
                                get_xml_string(pDevic,Info.m_Device.m_strXAddr);
                            }
                            else if(xmlStrcmp  (pDevic->name,BAD_CAST "Network")==0) {
                                xmlNodePtr pNetwork = pDevic->children;
                                while(pNetwork) {
                                    //printf("pNetwork->name=%s\n",pNetwork->name);
                                    if(xmlStrcmp  (pNetwork->name,BAD_CAST "IPFilter")==0) {
                                        get_xml_string(pNetwork,Info.m_Device.m_Network.m_strIPFilter);
                                    }
                                    else if(xmlStrcmp  (pNetwork->name,BAD_CAST "ZeroConfiguration")==0) {
                                        get_xml_string(pNetwork,Info.m_Device.m_Network.m_strZeroConfiguration);
                                    }
                                    else if(xmlStrcmp  (pNetwork->name,BAD_CAST "IPVersion6")==0) {
                                        get_xml_string(pNetwork,Info.m_Device.m_Network.m_strIPVersion6);
                                    }
                                    else if(xmlStrcmp  (pNetwork->name,BAD_CAST "DynDNS")==0) {
                                        get_xml_string(pNetwork,Info.m_Device.m_Network.m_strDynDNS);
                                    }
                                    pNetwork = pNetwork->next;
                                }
                            }
                            else if(xmlStrcmp  (pDevic->name,BAD_CAST "System")==0) {
                                xmlNodePtr pSystem = pDevic->children;
                                while(pSystem) {
                                    //printf("pSystem->name=%s\n",pSystem->name);
                                    if(xmlStrcmp  (pSystem->name,BAD_CAST "DiscoveryResolve")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strDiscoveryResolve);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "DiscoveryBye")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strDiscoveryBye);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "RemoteDiscovery")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strRemoteDiscovery);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "SystemBackup")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strSystemBackup);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "SystemLogging")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strSystemLogging);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "FirmwareUpgrade")==0) {
                                        get_xml_string(pSystem,Info.m_Device.m_System.m_strFirmwareUpgrade);
                                    }
                                    else if(xmlStrcmp  (pSystem->name,BAD_CAST "SupportedVersions")==0) {
                                        xmlNodePtr pVersion = pSystem->children;
                                        while(pVersion) {
                                            //printf("pVersion->name=%s\n",pVersion->name);
                                            if(xmlStrcmp  (pVersion->name,BAD_CAST "Major")==0) {
                                                get_xml_string(pSystem,Info.m_Device.m_System.m_SupportedVersions.m_strMajor);
                                            }
                                            else if(xmlStrcmp  (pVersion->name,BAD_CAST "Minor")==0) {
                                                get_xml_string(pSystem,Info.m_Device.m_System.m_SupportedVersions.m_strMinor);
                                            }
                                            pVersion = pVersion->next;
                                        }
                                    }
                                    pSystem = pSystem->next;
                                }
                            }
                            else if(xmlStrcmp  (pDevic->name,BAD_CAST "IO")==0) {
                                xmlNodePtr pIO = pDevic->children;
                                while(pIO) {
                                    //printf("pIO->name=%s\n",pIO->name);
                                    if(xmlStrcmp  (pIO->name,BAD_CAST "InputConnectors")==0) {
                                        get_xml_string(pIO,Info.m_Device.m_IO.m_strInputConnectors);
                                    }
                                    else if(xmlStrcmp  (pIO->name,BAD_CAST "RelayOutputs")==0) {
                                        get_xml_string(pIO,Info.m_Device.m_IO.m_strRelayOutputs);
                                    }
                                    pIO = pIO->next;
                                }
                            }
                            else if(xmlStrcmp  (pDevic->name,BAD_CAST "Security")==0) {
                                xmlNodePtr pSecurity = pDevic->children;
                                while(pSecurity) {
                                    //printf("pSecurity->name=%s\n",pSecurity->name);
                                    if(xmlStrcmp  (pSecurity->name,BAD_CAST "TLS1.1")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strTLS1_1);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "TLS1.2")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strTLS1_2);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "OnboardKeyGeneration")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strOnboardKeyGeneration);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "AccessPolicyConfig")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strAccessPolicyConfig);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "X.509Token")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strX_509Token);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "SAMLToken")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strSAMLToken);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "KerberosToken")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strKerberosToken);
                                    }
                                    else if(xmlStrcmp  (pSecurity->name,BAD_CAST "RELToken")==0) {
                                        get_xml_string(pSecurity,Info.m_Device.m_Security.m_strRELToken);
                                    }
                                    pSecurity = pSecurity->next;
                                }
                            }
                            pDevic = pDevic->next;
                        }
                    }
                    else if(xmlStrcmp  (pCapabilities->name,BAD_CAST "Events")==0) {
                        xmlNodePtr pEvents = pCapabilities->children;
                        while(pEvents) {
                            //printf("pEvents->name=%s\n",pEvents->name);
                            if(xmlStrcmp  (pEvents->name,BAD_CAST "XAddr")==0) {
                                get_xml_string(pEvents,Info.m_Events.m_strXAddr);
                            }
                            else if(xmlStrcmp  (pEvents->name,BAD_CAST "WSSubscriptionPolicySupport")==0) {
                                get_xml_string(pEvents,Info.m_Events.m_strWSSubscriptionPolicySupport);
                            }
                            else if(xmlStrcmp  (pEvents->name,BAD_CAST "WSPullPointSupport")==0) {
                                get_xml_string(pEvents,Info.m_Events.m_strWSPullPointSupport);
                            }
                            else if(xmlStrcmp  (pEvents->name,BAD_CAST "WSPausableSubscriptionManagerInterfaceSupport")==0) {
                                get_xml_string(pEvents,Info.m_Events.m_strWSPausableSubscriptionManagerInterfaceSupport);
                            }
                            pEvents = pEvents->next;
                        }
                    }
                    else if(xmlStrcmp  (pCapabilities->name,BAD_CAST "Media")==0) {
                        xmlNodePtr pMedia = pCapabilities->children;
                        while(pMedia) {
                            //printf("pMedia->name=%s\n",pMedia->name);
                            if(xmlStrcmp  (pMedia->name,BAD_CAST "XAddr")==0) {
                                get_xml_string(pMedia,Info.m_Media.m_strXAddr);
                            }
                            else if(xmlStrcmp  (pMedia->name,BAD_CAST "StreamingCapabilities")==0) {
                                //get_xml_string(pMedia,Info.m_Media.m_strWSSubscriptionPolicySupport);
                                xmlNodePtr pStreamingCapabilities = pMedia->children;
                                while(pStreamingCapabilities) {
                                    //printf("pStreamingCapabilities->name=%s\n",pStreamingCapabilities->name);
                                    if(xmlStrcmp  (pStreamingCapabilities->name,BAD_CAST "RTPMulticast")==0) {
                                        get_xml_string(pStreamingCapabilities,Info.m_Media.m_StreamingCapabilities.m_strRTPMulticast);
                                    }
                                    else if(xmlStrcmp  (pStreamingCapabilities->name,BAD_CAST "RTP_TCP")==0) {
                                        get_xml_string(pStreamingCapabilities,Info.m_Media.m_StreamingCapabilities.m_strRTP_TCP);
                                    }
                                    else if(xmlStrcmp  (pStreamingCapabilities->name,BAD_CAST "RTP_RTSP_TCP")==0) {
                                        get_xml_string(pStreamingCapabilities,Info.m_Media.m_StreamingCapabilities.m_strRTP_RTSP_TCP);
                                    }
                                    pStreamingCapabilities = pStreamingCapabilities->next;
                                }
                            }
                            pMedia = pMedia->next;
                        }
                    }
                    pCapabilities=pCapabilities->next;
                }
            }
            pCapabilitiesItem =  pCapabilitiesItem->next;
        }

        Info.m_enResult =  TOnVif_FaultResponse::enOK;
        if(xmlReqDoc!=NULL) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
        }
        return true;
    }
    if(xmlReqDoc!=NULL) {
        xmlFreeDoc(xmlReqDoc);
        xmlReqDoc=NULL;
    }
    return false;
}

bool COnVifDeviceMngmt::GetScopes(TOnVif_Scopes  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\""
        " xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\""
        " xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<s:Header>"
        "</s:Header>"
        "<s:Body>"
        "<tds:GetScopes />"
        "</s:Body>"
        "</s:Envelope>";
    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pGetScopesResponse = pBody->children;
        if(xmlStrcmp(pGetScopesResponse->name,BAD_CAST "GetScopesResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pScopes = pGetScopesResponse->children;

        while (pScopes!=NULL) {
            xmlNodePtr pScopesItem = pScopes->children;
            TOnVif_ScopeItem scItem;
            int s32Count = 0;
            while(pScopesItem) {
                if(xmlStrcmp  (pScopesItem->name,BAD_CAST "ScopeDef")==0) {
                    get_xml_string(pScopesItem,scItem.m_strScopeDef);
                    s32Count +=1;
                }
                else if(xmlStrcmp  (pScopesItem->name,BAD_CAST "ScopeItem")==0) {
                    get_xml_string(pScopesItem,scItem.m_strScopeItem);
                    s32Count +=2;
                }
                pScopesItem = pScopesItem->next;
            }
            if(s32Count==3) {
                Info.m_lstItem.push_back(scItem);
            }
            pScopes = pScopes->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }
    if(Info.m_lstItem.size()>0)
        return true;

    return false;
}
bool COnVifDeviceMngmt::GetDiscoveryMode(TOnVif_DiscoveryMode  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetDiscoveryMode />"
        "</soap:Body>"
        "</soap:Envelope>";

    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pGetDiscoveryModeResponse = pBody->children;
        if(xmlStrcmp(pGetDiscoveryModeResponse->name,BAD_CAST "GetDiscoveryModeResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pDiscoveryMode = pGetDiscoveryModeResponse->children;
        if(pDiscoveryMode) {
            if(xmlStrcmp  (pDiscoveryMode->name,BAD_CAST "DiscoveryMode")==0) {
                get_xml_string(pDiscoveryMode,Info.m_strMode);
            }
        }
        xmlFreeDoc(xmlReqDoc);
    }
    if(Info.m_strMode.empty())
        return false;
    return true;
}
bool COnVifDeviceMngmt::GetDNS(TOnVif_DNSInformation  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetDNS />"
        "</soap:Body>"
        "</soap:Envelope>";
    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetDNSResponse = pBody->children;
        if(xmlStrcmp(pGetDNSResponse->name,BAD_CAST "GetDNSResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pDNSInformation = pGetDNSResponse->children;
        if(xmlStrcmp  (pDNSInformation->name,BAD_CAST "DNSInformation")==0) {
            xmlNodePtr pDNSInfor = pDNSInformation->children;
            while(pDNSInfor) {
                //printf("pDNSInformation->name=%s\n",pDNSInfor->name);
                if(xmlStrcmp  (pDNSInfor->name,BAD_CAST "FromDHCP")==0) {
                    get_xml_string(pDNSInfor,Info.m_strFromDHCP);
                }
                else if(xmlStrcmp  (pDNSInfor->name,BAD_CAST "DNSFromDHCP")==0) {
                    //get_xml_string(pDNSInformation,Info.m_strFromDHCP);
                    xmlNodePtr pDNSFromDHCP = pDNSInfor->children;
                    TOnVif_DNSFromDHCP item;
                    while(pDNSFromDHCP) {
                        if(xmlStrcmp  (pDNSFromDHCP->name,BAD_CAST "Type")==0) {
                            get_xml_string(pDNSFromDHCP,item.m_strType);
                        }
                        else if(xmlStrcmp  (pDNSFromDHCP->name,BAD_CAST "IPv4Address")==0) {
                            get_xml_string(pDNSFromDHCP,item.m_strIPv4Address);
                        }
                        pDNSFromDHCP = pDNSFromDHCP->next;
                    }
                    Info.m_lstDNSFromDHCP.push_back(item);
                }
                pDNSInfor = pDNSInfor->next;
            }
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}

bool COnVifDeviceMngmt::GetHostname(TOnVif_Hostname  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\"  "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\">"
        "<s:Body>"
        "<tds:GetHostname />"
        "</s:Body>"
        "</s:Envelope>";

    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetHostnameResponse = pBody->children;
        if(xmlStrcmp(pGetHostnameResponse->name,BAD_CAST "GetHostnameResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);

        xmlNodePtr pHostnameInformation = pGetHostnameResponse->children;
        if(xmlStrcmp  (pHostnameInformation->name,BAD_CAST "HostnameInformation")==0) {
            xmlNodePtr pHostnameInformationItem = pHostnameInformation->children;
            while(pHostnameInformationItem) {
                //printf("pHostnameInformationItem->name=%s\n",pHostnameInformationItem->name);
                if(xmlStrcmp  (pHostnameInformationItem->name,BAD_CAST "FromDHCP")==0) {
                    get_xml_string(pHostnameInformationItem,Info.m_strFromDHCP);
                }
                if(xmlStrcmp  (pHostnameInformationItem->name,BAD_CAST "Name")==0) {
                    get_xml_string(pHostnameInformationItem,Info.m_strName);
                }
                pHostnameInformationItem = pHostnameInformationItem->next;
            }
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}

bool COnVifDeviceMngmt::GetNetworkDefaultGateway(TOnVif_NetworkGateway  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetNetworkDefaultGateway />"
        "</soap:Body>"
        "</soap:Envelope>";

    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetNetworkDefaultGatewayResponse = pBody->children;
        if(xmlStrcmp(pGetNetworkDefaultGatewayResponse->name,BAD_CAST "GetNetworkDefaultGatewayResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);

        xmlNodePtr pNetworkGateway = pGetNetworkDefaultGatewayResponse->children;
        if(xmlStrcmp  (pNetworkGateway->name,BAD_CAST "NetworkGateway")==0) {
            xmlNodePtr pIPv4Address = pNetworkGateway->children;
            while(pIPv4Address) {
                //printf("pHostnameInformationItem->name=%s\n",pHostnameInformationItem->name);
                if(xmlStrcmp  (pIPv4Address->name,BAD_CAST "IPv4Address")==0) {
                    get_xml_string(pIPv4Address,Info.m_strIPv4Address);
                }
                pIPv4Address = pIPv4Address->next;
            }
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}

bool COnVifDeviceMngmt::GetNetworkInterfaces(TOnVif_NetworkInterfaces  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetNetworkInterfaces />"
        "</soap:Body>"
        "</soap:Envelope>";

    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetNetworkInterfacesResponse = pBody->children;
        if(xmlStrcmp(pGetNetworkInterfacesResponse->name,BAD_CAST "GetNetworkInterfacesResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pNetworkInterfaces = pGetNetworkInterfacesResponse->children;
        while(pNetworkInterfaces) {
            TOnVif_NetworkInterfaceItem item;
            if(xmlStrcmp  (pNetworkInterfaces->name,BAD_CAST "NetworkInterfaces")==0) {
                if(get_xml_attr(pNetworkInterfaces,"token",item.m_strToken)==true) {
                    xmlNodePtr pNetworkInterfaceItem = pNetworkInterfaces->children;
                    while(pNetworkInterfaceItem) {
                        if(xmlStrcmp  (pNetworkInterfaceItem->name,BAD_CAST "Enabled")==0) {
                            get_xml_string(pNetworkInterfaceItem,item.m_strEnabled);
                        }
                        else if(xmlStrcmp  (pNetworkInterfaceItem->name,BAD_CAST "Info")==0) {
                            xmlNodePtr pInfoItem = pNetworkInterfaceItem->children;
                            while(pInfoItem) {
                                if(xmlStrcmp  (pInfoItem->name,BAD_CAST "Name")==0) {
                                    get_xml_string(pInfoItem,item.m_Info.m_strName);
                                }
                                if(xmlStrcmp  (pInfoItem->name,BAD_CAST "HwAddress")==0) {
                                    get_xml_string(pInfoItem,item.m_Info.m_strHwAddress);
                                }
                                if(xmlStrcmp  (pInfoItem->name,BAD_CAST "MTU")==0) {
                                    get_xml_string(pInfoItem,item.m_Info.m_strMTU);
                                }
                                pInfoItem = pInfoItem->next;
                            }
                        }
                        else if(xmlStrcmp  (pNetworkInterfaceItem->name,BAD_CAST "IPv4")==0) {
                            xmlNodePtr pIPv4 = pNetworkInterfaceItem->children;
                            while(pIPv4) {

                                if(xmlStrcmp  (pIPv4->name,BAD_CAST "Enabled")==0) {
                                    get_xml_string(pIPv4,item.m_IPv4.m_strEnabled);
                                }
                                if(xmlStrcmp  (pIPv4->name,BAD_CAST "Config")==0) {
                                    xmlNodePtr pConfig = pIPv4->children;
                                    while(pConfig) {
                                        printf("pConfig->name=%s\n",pConfig->name);
                                        if(xmlStrcmp  (pConfig->name,BAD_CAST "FromDHCP")==0) {
                                            xmlNodePtr pFromDHCP = pConfig->children;
                                            while(pFromDHCP) {
                                                if(xmlStrcmp  (pFromDHCP->name,BAD_CAST "Address")==0) {
                                                    get_xml_string(pFromDHCP,item.m_IPv4.m_Config.m_FromDHCP.m_strAddress);
                                                }
                                                if(xmlStrcmp  (pFromDHCP->name,BAD_CAST "PrefixLength")==0) {
                                                    get_xml_string(pFromDHCP,item.m_IPv4.m_Config.m_FromDHCP.m_strPrefixLength);
                                                }
                                                pFromDHCP = pFromDHCP->next;
                                            }
                                        }
                                        if(xmlStrcmp  (pConfig->name,BAD_CAST "Manual")==0) {
                                            xmlNodePtr pManual = pConfig->children;
                                            while(pManual) {
                                                if(xmlStrcmp  (pManual->name,BAD_CAST "Address")==0) {
                                                    get_xml_string(pManual,item.m_IPv4.m_Config.m_Manual.m_strAddress);
                                                }
                                                if(xmlStrcmp  (pManual->name,BAD_CAST "PrefixLength")==0) {
                                                    get_xml_string(pManual,item.m_IPv4.m_Config.m_Manual.m_strPrefixLength);
                                                }
                                                pManual = pManual->next;
                                            }
                                        }
                                        if(xmlStrcmp  (pConfig->name,BAD_CAST "DHCP")==0) {
                                            get_xml_string(pConfig,item.m_IPv4.m_Config.m_strDHCP);
                                        }
                                        pConfig = pConfig->next;
                                    }
                                }
                                pIPv4 = pIPv4->next;
                            }
                        }
                        pNetworkInterfaceItem = pNetworkInterfaceItem->next;
                    }
                }
            }
            Info.m_lstNIF.push_back(item);
            pNetworkInterfaces = pNetworkInterfaces->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }

    return true;
}

bool COnVifDeviceMngmt::GetNetworkProtocols(TOnVif_NetworkProtocols  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetNetworkProtocols />"
        "</soap:Body>"
        "</soap:Envelope>";
    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetNetworkProtocolsResponse = pBody->children;
        if(xmlStrcmp(pGetNetworkProtocolsResponse->name,BAD_CAST "GetNetworkProtocolsResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pNetworkProtocols = pGetNetworkProtocolsResponse->children;
        while(pNetworkProtocols) {
            //printf("1.%s\n",pNetworkProtocols->name);
            if(xmlStrcmp(pNetworkProtocols->name,BAD_CAST "NetworkProtocols")==0) {
                xmlNodePtr pNetworkProtocolItem = pNetworkProtocols->children;
                //printf("%s\n",pNetworkProtocolItem->name);
                TOnVif_NetworkProtocolItem item;
                while(pNetworkProtocolItem) {

                    if(xmlStrcmp(pNetworkProtocolItem->name,BAD_CAST "Name")==0) {
                        get_xml_string(pNetworkProtocolItem,item.m_strName);
                    }
                    if(xmlStrcmp(pNetworkProtocolItem->name,BAD_CAST "Enabled")==0) {
                        get_xml_string(pNetworkProtocolItem,item.m_strEnabled);
                    }
                    if(xmlStrcmp(pNetworkProtocolItem->name,BAD_CAST "Port")==0) {
                        get_xml_string(pNetworkProtocolItem,item.m_strPort);
                    }
                    pNetworkProtocolItem = pNetworkProtocolItem->next;
                }
                Info.m_lstNP.push_back(item);
            }
            pNetworkProtocols = pNetworkProtocols->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}


bool COnVifDeviceMngmt::GetNTP(TOnVif_NTPInformation  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetNTP />"
        "</soap:Body>"
        "</soap:Envelope>";
    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetNTPResponse = pBody->children;
        if(xmlStrcmp(pGetNTPResponse->name,BAD_CAST "GetNTPResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pNTPInformation = pGetNTPResponse->children;
        while(pNTPInformation) {
            //printf("pNTPInformation->name=%s\n",pNTPInformation->name);
            if(xmlStrcmp(pNTPInformation->name,BAD_CAST "NTPInformation")==0) {
                xmlNodePtr pNTPInformationItem = pNTPInformation->children;
                while(pNTPInformationItem) {
                    if(xmlStrcmp(pNTPInformationItem->name,BAD_CAST "FromDHCP")==0) {
                        get_xml_string(pNTPInformationItem,Info.m_strFromDHCP);
                    }
                    if(xmlStrcmp(pNTPInformationItem->name,BAD_CAST "NTPManual")==0) {
                        xmlNodePtr pNTPManual = pNTPInformationItem->children;
                        while(pNTPManual) {
                            if(xmlStrcmp(pNTPManual->name,BAD_CAST "Type")==0) {
                                get_xml_string(pNTPManual,Info.m_NTPManual.m_strType);
                            }
                            if(xmlStrcmp(pNTPManual->name,BAD_CAST "IPv4Address")==0) {
                                get_xml_string(pNTPManual,Info.m_NTPManual.m_strIPv4Address);
                            }
                            pNTPManual = pNTPManual->next;
                        }
                    }
                    pNTPInformationItem = pNTPInformationItem->next;
                }
            }
            pNTPInformation = pNTPInformation->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}

bool COnVifDeviceMngmt::GetGetSystemDateAndTime(TOnVif_SystemDateAndTime  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<tds:GetSystemDateAndTime />"
        "</soap:Body>"
        "</soap:Envelope>";
    send_onvif_cmd(szOnVifFmt_Req,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }

        xmlNodePtr pGetSystemDateAndTimeResponse = pBody->children;
        if(xmlStrcmp(pGetSystemDateAndTimeResponse->name,BAD_CAST "GetSystemDateAndTimeResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr pSystemDateAndTime = pGetSystemDateAndTimeResponse->children;
        while(pSystemDateAndTime) {
            //printf("pSystemDateAndTime->name=%s\n",pSystemDateAndTime->name);
            if(xmlStrcmp(pSystemDateAndTime->name,BAD_CAST "SystemDateAndTime")==0) {
                xmlNodePtr pSystemDateAndTimeItem = pSystemDateAndTime->children;
                while(pSystemDateAndTimeItem) {
                    //printf("pSystemDateAndTimeItem->name=%s\n",pSystemDateAndTimeItem->name);
                    if(xmlStrcmp(pSystemDateAndTimeItem->name,BAD_CAST "DateTimeType")==0) {
                        get_xml_string(pSystemDateAndTimeItem,Info.m_strDateTimeType);
                    }
                    if(xmlStrcmp(pSystemDateAndTimeItem->name,BAD_CAST "DaylightSavings")==0) {
                        get_xml_string(pSystemDateAndTimeItem,Info.m_strDaylightSavings);
                    }
                    if(xmlStrcmp(pSystemDateAndTimeItem->name,BAD_CAST "TimeZone")==0) {
                        xmlNodePtr pTZ = pSystemDateAndTimeItem->children;
                        while(pTZ) {
                            if(xmlStrcmp(pTZ->name,BAD_CAST "TZ")==0) {
                                get_xml_string(pTZ,Info.m_strTimeZone);
                            }
                            pTZ = pTZ->next;
                        }
                    }
                    if(xmlStrcmp(pSystemDateAndTimeItem->name,BAD_CAST "UTCDateTime")==0) {
                        xmlNodePtr pDateTime = pSystemDateAndTimeItem->children;
                        while(pDateTime) {
                            if(xmlStrcmp(pDateTime->name,BAD_CAST "Time")==0) {
                                //get_xml_string(pTZ,Info.m_strTimeZone);
                                xmlNodePtr pTime = pDateTime->children;
                                while(pTime) {
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Hour")==0) {
                                        get_xml_string(pTime,Info.m_UTCDateTime.m_strHour);
                                    }
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Minute")==0) {
                                        get_xml_string(pTime,Info.m_UTCDateTime.m_strMinute);
                                    }
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Second")==0) {
                                        get_xml_string(pTime,Info.m_UTCDateTime.m_strSecond);
                                    }
                                    pTime = pTime->next;
                                }
                            }
                            if(xmlStrcmp(pDateTime->name,BAD_CAST "Date")==0) {
                                //get_xml_string(pTZ,Info.m_strTimeZone);
                                xmlNodePtr pDate = pDateTime->children;
                                while(pDate) {
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Year")==0) {
                                        get_xml_string(pDate,Info.m_UTCDateTime.m_strYear);
                                    }
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Month")==0) {
                                        get_xml_string(pDate,Info.m_UTCDateTime.m_strMonth);
                                    }
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Day")==0) {
                                        get_xml_string(pDate,Info.m_UTCDateTime.m_strDay);
                                    }
                                    pDate = pDate->next;
                                }
                            }
                            pDateTime = pDateTime->next;
                        }
                    }
                    if(xmlStrcmp(pSystemDateAndTimeItem->name,BAD_CAST "LocalDateTime")==0) {
                        xmlNodePtr pDateTime = pSystemDateAndTimeItem->children;
                        while(pDateTime) {
                            if(xmlStrcmp(pDateTime->name,BAD_CAST "Time")==0) {
                                //get_xml_string(pTZ,Info.m_strTimeZone);
                                xmlNodePtr pTime = pDateTime->children;
                                while(pTime) {
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Hour")==0) {
                                        get_xml_string(pTime,Info.m_LocalDateTime.m_strHour);
                                    }
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Minute")==0) {
                                        get_xml_string(pTime,Info.m_LocalDateTime.m_strMinute);
                                    }
                                    if(xmlStrcmp(pTime->name,BAD_CAST "Second")==0) {
                                        get_xml_string(pTime,Info.m_LocalDateTime.m_strSecond);
                                    }
                                    pTime = pTime->next;
                                }
                            }
                            if(xmlStrcmp(pDateTime->name,BAD_CAST "Date")==0) {
                                //get_xml_string(pTZ,Info.m_strTimeZone);
                                xmlNodePtr pDate = pDateTime->children;
                                while(pDate) {
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Year")==0) {
                                        get_xml_string(pDate,Info.m_LocalDateTime.m_strYear);
                                    }
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Month")==0) {
                                        get_xml_string(pDate,Info.m_LocalDateTime.m_strMonth);
                                    }
                                    if(xmlStrcmp(pDate->name,BAD_CAST "Day")==0) {
                                        get_xml_string(pDate,Info.m_LocalDateTime.m_strDay);
                                    }
                                    pDate = pDate->next;
                                }
                            }
                            pDateTime = pDateTime->next;
                        }
                    }
                    pSystemDateAndTimeItem = pSystemDateAndTimeItem->next;
                }
                //get_xml_string(pNTPInformationItem,Info.m_strFromDHCP);
            }
            pSystemDateAndTime = pSystemDateAndTime->next;
        }





        /*
        xmlNodePtr pNTPInformation = pGetNTPResponse->children;
        while(pNTPInformation){
        	//printf("pNTPInformation->name=%s\n",pNTPInformation->name);
        	if(xmlStrcmp(pNTPInformation->name,BAD_CAST "NTPInformation")==0) {
        		xmlNodePtr pNTPInformationItem = pNTPInformation->children;
        		while(pNTPInformationItem){
        			if(xmlStrcmp(pNTPInformationItem->name,BAD_CAST "FromDHCP")==0) {
        				get_xml_string(pNTPInformationItem,Info.m_strFromDHCP);
        			}
        			if(xmlStrcmp(pNTPInformationItem->name,BAD_CAST "NTPManual")==0) {
        				xmlNodePtr pNTPManual = pNTPInformationItem->children;
        				while(pNTPManual){
        					if(xmlStrcmp(pNTPManual->name,BAD_CAST "Type")==0) {
        						get_xml_string(pNTPManual,Info.m_NTPManual.m_strType);
        					}
        					if(xmlStrcmp(pNTPManual->name,BAD_CAST "IPv4Address")==0) {
        						get_xml_string(pNTPManual,Info.m_NTPManual.m_strIPv4Address);
        					}
        					pNTPManual = pNTPManual->next;
        				}
        			}
        			pNTPInformationItem = pNTPInformationItem->next;
        		}
        	}
        	pNTPInformation = pNTPInformation->next;
        }
        */
        xmlFreeDoc(xmlReqDoc);
    }
    return true;
}





