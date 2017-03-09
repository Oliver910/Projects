#include "COnVifMediaConf.h"

COnVifMediaConf::COnVifMediaConf(CUrlTool *pUrl)
    :fOnVifToolBase(pUrl)
{
    m_strCmdUrl = "onvif/media_service";
}
COnVifMediaConf::~COnVifMediaConf()
{
}
bool COnVifMediaConf::GetProfiles(TOnVif_Profiles  &Info)
{
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<trt:GetProfiles />"
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
        xmlNodePtr pGetProfilesResponse = pBody->children;
        if(xmlStrcmp(pGetProfilesResponse->name,BAD_CAST "GetProfilesResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlNodePtr pCapabilitiesItem = pCapabilitiesResponse->children;
        //xmlDocFormatDump(stdout,xmlReqDoc->doc,1);
        xmlNodePtr pProfiles = pGetProfilesResponse->children;
        while (pProfiles!=NULL) {
            //printf("pProfiles->name=%s\n",pProfiles->name);
            if(xmlStrcmp  (pProfiles->name,BAD_CAST "Profiles")==0) {
                xmlNodePtr pProfileItem = pProfiles->children;
                TOnVif_Profile profile;
		get_xml_attr(pProfiles,"token",profile.m_strToken);
                bool bExist = false;
                while(pProfileItem) {
                    //printf("pProfileItem->name=%s\n",pProfileItem->name);
                    if(xmlStrcmp  (pProfileItem->name,BAD_CAST "Name")==0) {
                        get_xml_string(pProfileItem,profile.m_strName);
                        bExist = true;
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "VideoSourceConfiguration")==0) {
                        parser_VideoSourceConfiguration(pProfileItem
                                                        ,profile.m_VideoSourceConfiguration);
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "AudioSourceConfiguration")==0) {
                        parser_AudioSourceConfiguration(pProfileItem
                                                        ,profile.m_AudioSourceConfiguration);
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "VideoEncoderConfiguration")==0) {
                        parser_VideoEncoderConfiguration(pProfileItem
                                                         ,profile.m_VideoEncoderConfiguration);
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "AudioEncoderConfiguration")==0) {
                        parser_AudioEncoderConfiguration(pProfileItem
                                                         ,profile.m_AudioEncoderConfiguration);
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "PTZConfiguration")==0) {
                        parser_PTZConfiguration(pProfileItem,profile.m_PTZConfiguration);
                    }
                    else if(xmlStrcmp  (pProfileItem->name,BAD_CAST "MetadataConfiguration")==0) {
                        parser_MetadataConfiguration(pProfileItem,profile.m_MetadataConfiguration);
                    }
                    pProfileItem = pProfileItem->next;
                }
                if(bExist) {
                    Info.m_lstProfile.push_back(profile);
                }
                //get_xml_string(pDeviceInformationItem,Info.m_strManufacturer);
            }
            pProfiles = pProfiles->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }
}
bool COnVifMediaConf::parser_VideoSourceConfiguration(xmlNodePtr pNode,TOnVif_VideoSourceConfiguration  &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "VideoSourceConfiguration")==0  ) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            //printf("pConfigItem=%s\n",pConfigItem->name);
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "SourceToken")==0) {
                get_xml_string(pConfigItem,Info.m_strSourceToken);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Bounds")==0) {
                //get_xml_string(pConfigItem,Info.strName);
                get_xml_attr(pConfigItem,"x",Info.m_Bounds.m_strX);
                get_xml_attr(pConfigItem,"y",Info.m_Bounds.m_strY);
                get_xml_attr(pConfigItem,"width",Info.m_Bounds.m_strWidth);
                get_xml_attr(pConfigItem,"height",Info.m_Bounds.m_strHeight);
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}
bool COnVifMediaConf::parser_AudioSourceConfiguration(xmlNodePtr pNode,TOnVif_AudioSourceConfiguration &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "AudioSourceConfiguration")==0  ) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "SourceToken")==0) {
                get_xml_string(pConfigItem,Info.m_strSourceToken);
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}
bool COnVifMediaConf::parser_Multicast(xmlNodePtr pConfigItem,TOnVif_Multicast &Info)
{
    xmlNodePtr pItem= pConfigItem->children;
    while(pItem) {
        //printf("pRes=%s\n",pRes->name);
        if(xmlStrcmp  (pItem->name,BAD_CAST "Address")==0) {
            //get_xml_string(pItem,Info.m_strH264GovLength);
            xmlNodePtr pAddress= pItem->children;
            while(pAddress) {
                //printf("pRes=%s\n",pRes->name);
                if(xmlStrcmp  (pAddress->name,BAD_CAST "Type")==0) {
                    get_xml_string(pAddress,Info.m_strMulticastAddressType);
                }
                if(xmlStrcmp  (pAddress->name,BAD_CAST "IPv4Address")==0) {
                    get_xml_string(pAddress,Info.m_strMulticastAddressIPv4Address);
                }
                if(xmlStrcmp  (pAddress->name,BAD_CAST "IPv6Address")==0) {
                    get_xml_string(pAddress,Info.m_strMulticastAddressIPv6Address);
                }
                pAddress = pAddress->next;
            }
        }
        if(xmlStrcmp  (pItem->name,BAD_CAST "Port")==0) {
            get_xml_string(pItem,Info.m_strMulticastPort);
        }
        if(xmlStrcmp  (pItem->name,BAD_CAST "TTL")==0) {
            get_xml_string(pItem,Info.m_strMulticastTTL);
        }
        if(xmlStrcmp  (pItem->name,BAD_CAST "AutoStart")==0) {
            get_xml_string(pItem,Info.m_strMulticastAutoStart);
        }
        pItem = pItem->next;
    }
}



bool COnVifMediaConf::parser_VideoEncoderConfiguration(xmlNodePtr pNode,TOnVif_VideoEncoderConfiguration &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "VideoEncoderConfiguration")==0  ) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Encoding")==0) {
                get_xml_string(pConfigItem,Info.m_strEncoding);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Resolution")==0) {
                //t_xml_string(pConfigItem,Info.m_strSourceToken);
                xmlNodePtr pRes = pConfigItem->children;
                while(pRes) {
                    //printf("pRes=%s\n",pRes->name);
                    if(xmlStrcmp  (pRes->name,BAD_CAST "Width")==0) {
                        get_xml_string(pRes,Info.m_strResolutionWidth);
                    }
                    if(xmlStrcmp  (pRes->name,BAD_CAST "Height")==0) {
                        get_xml_string(pRes,Info.m_strResolutionHeight);
                    }
                    pRes = pRes->next;
                }
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Quality")==0) {
                get_xml_string(pConfigItem,Info.m_strQuality);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "RateControl")==0) {
                //t_xml_string(pConfigItem,Info.m_strSourceToken);
                xmlNodePtr pItem= pConfigItem->children;
                while(pItem) {
                    //printf("pRes=%s\n",pRes->name);
                    if(xmlStrcmp  (pItem->name,BAD_CAST "FrameRateLimit")==0) {
                        get_xml_string(pItem,Info.m_strRateControlFrameRateLimit);
                    }
                    if(xmlStrcmp  (pItem->name,BAD_CAST "EncodingInterval")==0) {
                        get_xml_string(pItem,Info.m_strRateControlEncodingInterval);
                    }
                    if(xmlStrcmp  (pItem->name,BAD_CAST "BitrateLimit")==0) {
                        get_xml_string(pItem,Info.m_strRateControlBitrateLimit);
                    }
                    pItem = pItem->next;
                }
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "H264")==0) {
                //t_xml_string(pConfigItem,Info.m_strSourceToken);
                xmlNodePtr pItem= pConfigItem->children;
                while(pItem) {
                    //printf("pRes=%s\n",pRes->name);
                    if(xmlStrcmp  (pItem->name,BAD_CAST "GovLength")==0) {
                        get_xml_string(pItem,Info.m_strH264GovLength);
                    }
                    if(xmlStrcmp  (pItem->name,BAD_CAST "H264Profile")==0) {
                        get_xml_string(pItem,Info.m_strH264H264Profile);
                    }
                    pItem = pItem->next;
                }
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Multicast")==0) {
                parser_Multicast(pConfigItem,Info.m_Multicast);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "SessionTimeout")==0) {
                get_xml_string(pConfigItem,Info.m_strSessionTimeout);
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}

bool COnVifMediaConf::parser_AudioEncoderConfiguration(xmlNodePtr pNode,TOnVif_AudioEncoderConfiguration &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "AudioEncoderConfiguration")==0  ) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Encoding")==0) {
                get_xml_string(pConfigItem,Info.m_strEncoding);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Bitrate")==0) {
                get_xml_string(pConfigItem,Info.m_strBitrate);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "SampleRate")==0) {
                get_xml_string(pConfigItem,Info.m_strSampleRate);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Multicast")==0) {
                parser_Multicast(pConfigItem,Info.m_Multicast);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "SessionTimeout")==0) {
                get_xml_string(pConfigItem,Info.m_strSessionTimeout);
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}


bool COnVifMediaConf::parser_PTZConfiguration(xmlNodePtr pNode,TOnVif_PTZConfiguration &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "PTZConfiguration")==0  ) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "NodeToken")==0) {
                get_xml_string(pConfigItem,Info.m_strNodeToken);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultRelativePanTiltTranslationSpace")==0) {
                get_xml_string(pConfigItem,Info.m_strDefaultRelativePanTiltTranslationSpace);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultRelativeZoomTranslationSpace")==0) {
                get_xml_string(pConfigItem,Info.m_strDefaultRelativeZoomTranslationSpace);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultContinuousPanTiltVelocitySpace")==0) {
                get_xml_string(pConfigItem,Info.m_strDefaultContinuousPanTiltVelocitySpace);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultContinuousZoomVelocitySpace")==0) {
                get_xml_string(pConfigItem,Info.m_strDefaultContinuousZoomVelocitySpace);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultPTZTimeout")==0) {
                get_xml_string(pConfigItem,Info.m_strDefaultPTZTimeout);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "DefaultPTZSpeed")==0) {
                xmlNodePtr pItem = pConfigItem->children;
                while(pItem) {
                    if(xmlStrcmp  (pItem->name,BAD_CAST "PanTilt")==0) {
                        get_xml_attr(pItem,"x",Info.m_strDefaultPTZSpeedPanTiltX);
                        get_xml_attr(pItem,"y",Info.m_strDefaultPTZSpeedPanTiltY);
                        get_xml_attr(pItem,"space",Info.m_strDefaultPTZSpeedPanTiltSpace);
                    }
                    if(xmlStrcmp  (pItem->name,BAD_CAST "Zoom")==0) {
                        get_xml_attr(pItem,"x",Info.m_strDefaultPTZSpeedZoomX);
                        get_xml_attr(pItem,"space",Info.m_strDefaultPTZSpeedZoomSpace);

                    }
                    pItem = pItem->next;
                }
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}

bool COnVifMediaConf::parser_MetadataConfiguration(xmlNodePtr pNode,TOnVif_MetadataConfiguration &Info)
{
    //printf("pNode=%s\n",pNode->name);
    if(xmlStrcmp  (pNode->name,BAD_CAST "Configuration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "MetadataConfiguration")==0 ||
            xmlStrcmp  (pNode->name,BAD_CAST "Configurations")==0) {
        get_xml_attr(pNode,"token",Info.m_strToken);
        xmlNodePtr pConfigItem = pNode->children;
        while(pConfigItem) {
            if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Name")==0) {
                get_xml_string(pConfigItem,Info.m_strName);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "UseCount")==0) {
                get_xml_string(pConfigItem,Info.m_strUseCount);
            }
            else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Multicast")==0) {
                parser_Multicast(pConfigItem,Info.m_Multicast);
            }
            pConfigItem = pConfigItem->next;
        }
    }
    return true;
}
bool COnVifMediaConf::GetVideoSourceConfiguration(std::string strToken,TOnVif_VideoSourceConfiguration  &Info)
{
    bool bRet=false;
    std::string strResponse;
    char szOnVifFmt_Req_cmd[1024];
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<trt:GetVideoSourceConfiguration>"
        "<trt:ConfigurationToken>%s</trt:ConfigurationToken>"
        "</trt:GetVideoSourceConfiguration>"
        "</soap:Body>"
        "</soap:Envelope>";

    sprintf(szOnVifFmt_Req_cmd,szOnVifFmt_Req,strToken.c_str());

    send_onvif_cmd(szOnVifFmt_Req_cmd,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pGetVideoSourceConfigurationResponse = pBody->children;
        if(xmlStrcmp(pGetVideoSourceConfigurationResponse->name
                     ,BAD_CAST "GetVideoSourceConfigurationResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
        xmlNodePtr ptVideoSourceConfiguratione = pGetVideoSourceConfigurationResponse->children;
        while(ptVideoSourceConfiguratione) {
            bRet = parser_VideoSourceConfiguration(ptVideoSourceConfiguratione,Info);
            ptVideoSourceConfiguratione = ptVideoSourceConfiguratione->next;
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return bRet;
}
bool COnVifMediaConf::parser_VideoSources(xmlNodePtr pNode,TOnVif_VideoSources  &Info)
{
    //printf("pNode=%s\n",pNode->name);
    while(pNode) {
        // printf("pNode=%s\n",pNode->name);
        if(xmlStrcmp  (pNode->name,BAD_CAST "VideoSources")==0) {
            TOnVif_VideoSource item;
            get_xml_attr(pNode,"token",item.m_strToken);
            xmlNodePtr pConfigItem = pNode->children;
            while(pConfigItem) {
                //printf("pConfigItem=%s\n",pConfigItem->name);
                if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Framerate")==0) {
                    get_xml_string(pConfigItem,item.m_strFramerate);
                }
                else if(xmlStrcmp  (pConfigItem->name,BAD_CAST "Resolution")==0) {
                    xmlNodePtr pRes = pConfigItem->children;
                    while(pRes) {
                        //printf("pRes=%s\n",pRes->name);
                        if(xmlStrcmp  (pRes->name,BAD_CAST "Width")==0) {
                            get_xml_string(pRes,item.m_strResWidth);
                        }
                        if(xmlStrcmp  (pRes->name,BAD_CAST "Height")==0) {
                            get_xml_string(pRes,item.m_strResHeight);
                        }
                        pRes = pRes->next;
                    }
                }
                pConfigItem = pConfigItem->next;
            }
            Info.m_lstVS.push_back(item);
        }
        pNode = pNode->next;
    }
    return true;
}

bool COnVifMediaConf::GetVideoSources(TOnVif_VideoSources  &Info)
{
    bool bRet = false;
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<trt:GetVideoSources />"
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
        xmlNodePtr pGetVideoSourcesResponse = pBody->children;
        if(xmlStrcmp(pGetVideoSourcesResponse->name,BAD_CAST "GetVideoSourcesResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        xmlDocFormatDump(stdout,xmlReqDoc->doc,1);
        xmlNodePtr pGetVideoSources = pGetVideoSourcesResponse->children;
        if(pGetVideoSources) {
            bRet = parser_VideoSources(pGetVideoSources,Info);
        }
        xmlFreeDoc(xmlReqDoc);
    }
    return bRet;
}

bool COnVifMediaConf::GetMetadataConfigurations(TOnVif_GetMetadataConfigurations  &Info)
{
    bool bRet = false;
    std::string strResponse;
    const char *szOnVifFmt_Req=
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
        "<soap:Body>"
        "<trt:GetMetadataConfigurations />"
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
        xmlNodePtr pGetMetadataConfigurationsResponse = pBody->children;
        if(xmlStrcmp(pGetMetadataConfigurationsResponse->name,BAD_CAST "GetMetadataConfigurationsResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        xmlDocFormatDump(stdout,xmlReqDoc,1);
		
        xmlNodePtr pGetMetadataConfigurations = pGetMetadataConfigurationsResponse->children;
        while(pGetMetadataConfigurations) {
           // printf("pGetMetadataConfigurations:%s\n",pGetMetadataConfigurations->name);
        	if(xmlStrcmp(pGetMetadataConfigurations->name
                     ,BAD_CAST "Configurations")==0) {
				TOnVif_MetadataConfiguration cfg;
				parser_MetadataConfiguration(pGetMetadataConfigurations,cfg);
				Info.m_lstMetadataConfiguration.push_back(cfg);            	
        	}
            pGetMetadataConfigurations = pGetMetadataConfigurations->next;
        }        
        xmlFreeDoc(xmlReqDoc);
    }
    return bRet;
}

bool COnVifMediaConf::GetStreamUri(std::string strToken, TOnVif_StreamUri  &Info)
{
    bool bRet = false;
    std::string strResponse;
	    char szOnVifFmt_Req_cmd[1024];
    const char *szOnVifFmt_Req=
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
			"xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
			"xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
		  "<soap:Body>"
		    "<GetStreamUri xmlns=\"http://www.onvif.org/ver10/media/wsdl\">"
		      "<StreamSetup>"
		        "<Stream xmlns=\"http://www.onvif.org/ver10/schema\">RTP-Unicast</Stream>"
		        "<Transport xmlns=\"http://www.onvif.org/ver10/schema\">"
		          "<Protocol>UDP</Protocol>"
		        "</Transport>"
		      "</StreamSetup>"
		      "<ProfileToken>%s</ProfileToken>"
		    "</GetStreamUri>"
		  "</soap:Body>"
		"</soap:Envelope>";

	sprintf(szOnVifFmt_Req_cmd,szOnVifFmt_Req,strToken.c_str());

    send_onvif_cmd(szOnVifFmt_Req_cmd,strResponse);
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
            return false;
        }
        xmlNodePtr pGetStreamUriResponse = pBody->children;
        if(xmlStrcmp(pGetStreamUriResponse->name,BAD_CAST "GetStreamUriResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
            return false;
        }
        //xmlDocFormatDump(stdout,xmlReqDoc,1);
		xmlNodePtr pGetStreamUri = pGetStreamUriResponse->children;
		while(pGetStreamUri){
        	if(xmlStrcmp(pGetStreamUri->name
                     ,BAD_CAST "MediaUri")==0) {
				xmlNodePtr pGetStreamUriItem = pGetStreamUri->children;
				while(pGetStreamUriItem){
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "Uri")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUri);
		        	}
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "InvalidAfterConnect")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriInvalidAfterConnect);
		        	}					
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "InvalidAfterReboot")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriInvalidAfterReboot);
		        	}	
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "Timeout")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriTimeout);
		        	}					
					pGetStreamUriItem = pGetStreamUriItem->next;
				}							
        	}			
			pGetStreamUri = pGetStreamUri->next;
		}
		
		xmlFreeDoc(xmlReqDoc);
    }	
    return bRet;
}

bool COnVifMediaConf::GetSnapshotUri(std::string strToken, TOnVif_StreamUri  &Info)
{
    bool bRet = false;
    std::string strResponse;
	    char szOnVifFmt_Req_cmd[1024];
    const char *szOnVifFmt_Req=
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
			"xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
			"xmlns:tt=\"http://www.onvif.org/ver10/schema\">"
		"<soap:Body>"
			"<trt:GetSnapshotUri>"
				"<trt:ProfileToken>%s</trt:ProfileToken>"
			"</trt:GetSnapshotUri>"
		 "</soap:Body>"
		"</soap:Envelope>";

	sprintf(szOnVifFmt_Req_cmd,szOnVifFmt_Req,strToken.c_str());
	//printf("szOnVifFmt_Req_cmd=\n%s\n",szOnVifFmt_Req_cmd);	
    send_onvif_cmd(szOnVifFmt_Req_cmd,strResponse);

	//printf("strResponse=\n%s\n",strResponse.c_str());	

	
    xmlNodePtr pBody = NULL;
    xmlDocPtr   xmlReqDoc =		get_onvif_body(strResponse,pBody);
    if(xmlReqDoc && pBody) {
        if(is_fault(pBody,Info)==true) {
            xmlFreeDoc(xmlReqDoc);
            xmlReqDoc=NULL;
			//printf("xxxxxxxxxxxx-1\n");
            return false;
        }
        xmlNodePtr pGetSnapshotUriResponse = pBody->children;
        if(xmlStrcmp(pGetSnapshotUriResponse->name,BAD_CAST "GetSnapshotUriResponse")!=0) {
            xmlFreeDoc(xmlReqDoc);
            Info.m_enResult =  TOnVif_FaultResponse::enSOAP_HEADER_ERR;
			//printf("xxxxxxxxxxxx-2 pGetStreamUriResponse->name=%s\n",pGetSnapshotUriResponse->name);
            return false;
        }
//        xmlDocFormatDump(stdout,xmlReqDoc,1);
		xmlNodePtr pGetStreamUri = pGetSnapshotUriResponse->children;
		while(pGetStreamUri){
        	if(xmlStrcmp(pGetStreamUri->name
                     ,BAD_CAST "MediaUri")==0) {
				xmlNodePtr pGetStreamUriItem = pGetStreamUri->children;
				while(pGetStreamUriItem){
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "Uri")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUri);
		        	}
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "InvalidAfterConnect")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriInvalidAfterConnect);
		        	}					
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "InvalidAfterReboot")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriInvalidAfterReboot);
		        	}	
		        	if(xmlStrcmp(pGetStreamUriItem->name
		                     ,BAD_CAST "Timeout")==0) {	
		                get_xml_string(pGetStreamUriItem,Info.m_strMediaUriTimeout);
		        	}					
					pGetStreamUriItem = pGetStreamUriItem->next;
				}							
        	}			
			pGetStreamUri = pGetStreamUri->next;
		}
		
		xmlFreeDoc(xmlReqDoc);
    }	
    return bRet;
}



