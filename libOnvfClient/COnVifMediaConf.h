#ifndef _CONVIFMEDIACONF_H_
#define _CONVIFMEDIACONF_H_

#include "fOnVifToolBase.h"

class COnVifMediaConf
    :public fOnVifToolBase
{

    bool parser_VideoSourceConfiguration(xmlNodePtr pNode,TOnVif_VideoSourceConfiguration  &Info);
    bool parser_AudioSourceConfiguration(xmlNodePtr pNode,TOnVif_AudioSourceConfiguration &Info);
    bool parser_VideoSources(xmlNodePtr pNode,TOnVif_VideoSources  &Info);
    bool parser_VideoEncoderConfiguration(xmlNodePtr pNode,TOnVif_VideoEncoderConfiguration &Info);
    bool parser_AudioEncoderConfiguration(xmlNodePtr pNode,TOnVif_AudioEncoderConfiguration &Info);
    bool parser_PTZConfiguration(xmlNodePtr pNode,TOnVif_PTZConfiguration &Info);
    bool parser_MetadataConfiguration(xmlNodePtr pNode,TOnVif_MetadataConfiguration &Info);
    bool parser_Multicast(xmlNodePtr pConfigItem,TOnVif_Multicast &Info);
public:
    COnVifMediaConf(CUrlTool *pUrl);
    virtual ~COnVifMediaConf();
    bool GetProfiles(TOnVif_Profiles  &Info);
    bool GetVideoSourceConfiguration(std::string strToken,TOnVif_VideoSourceConfiguration  &Info);
    bool GetVideoSources(TOnVif_VideoSources  &Info);
	bool GetMetadataConfigurations(TOnVif_GetMetadataConfigurations  &Info);
	bool GetStreamUri(std::string strToken, TOnVif_StreamUri  &Info);
	bool GetSnapshotUri(std::string strToken, TOnVif_StreamUri  &Info);

};
#endif
