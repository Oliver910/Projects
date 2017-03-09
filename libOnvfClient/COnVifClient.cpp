#include "COnVifClient.h"

COnVifClient::COnVifClient()
    :COnVifDeviceMngmt(&m_sUrl),m_sEvents(&m_sUrl)
    ,m_sImagingConf(&m_sUrl),m_sMediaConf(&m_sUrl)
    ,m_sVifPTZ(&m_sUrl)
{
}
COnVifClient::~COnVifClient()
{
}

