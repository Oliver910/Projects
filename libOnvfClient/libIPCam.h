#ifndef _LIBIPCAM_H_
#define _LIBIPCAM_H_

#include "hPlatform.h"
#include "fIPCamBase.h"

typedef fIPCambase * (*FP_IPCAM_CreateObject)(void);
typedef void (*FP_IPCAM_ReleaseObject)(fIPCambase * pObj);
typedef int  (*FP_IPCAM_GetIPCambaseVersion)(void);
typedef int  (*FP_IPCAM_InitLibrary)(void);
typedef int  (*FP_IPCAM_DeinitLibrary)(void);


/**
 * \defgroup Global
 */

/*! \ingroup Global
 *  \fn IPCAM_CreateObject
 *  \brief �����G�Ы�fIPCamBase����C
 *
 *  \return ���\�AfIPCamBase���СA���ѡA�^��null
 */
LIBIPCAM_API fIPCambase * IPCAM_CreateObject(void);

/*! \ingroup Global
 *	\brief �����GfIPCamBase�Ѻc�P����O����C
 *
 *  \param pObj�n�Ѻc�P����O���餧������СC
 *  \return �L
 */
LIBIPCAM_API void IPCAM_ReleaseObject(fIPCambase * pObj);

/*! \ingroup Global
 *	\brief �����G�Ы�fIPCamBase����C
 *
 *  ���ofIPCamBase Base class�����A�A���J�n��ɻ��ˬd
 *  �A�H����fIPCamBase���P���ۮe�y���{���w���ۮe�A�i��ް_������D�F
 *  ��ק�fIPCamBase�BCIPCamClient�BfSocketTool�ݭn���i�����C
 *
 *  \return ��ơA���fIPCambase�������C
 */
LIBIPCAM_API int IPCAM_GetIPCambaseVersion(void);

/*! \ingroup Global
 * \brief �禡�w��l�ƨ�ơC
 *
 *  \return 0:���\�A��L����
 */
LIBIPCAM_API int IPCAM_InitLibrary(void);

/*! \ingroup Global
 *  \brief ����禡�w���C
 *
 *  \return 0�G���\�A��L����
 */
LIBIPCAM_API int IPCAM_DeinitLibrary(void);


#endif
