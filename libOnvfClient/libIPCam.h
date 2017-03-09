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
 *  \brief 說明：創建fIPCamBase物件。
 *
 *  \return 成功，fIPCamBase指標，失敗，回傳null
 */
LIBIPCAM_API fIPCambase * IPCAM_CreateObject(void);

/*! \ingroup Global
 *	\brief 說明：fIPCamBase解構與釋放記憶體。
 *
 *  \param pObj要解構與釋放記憶體之物件指標。
 *  \return 無
 */
LIBIPCAM_API void IPCAM_ReleaseObject(fIPCambase * pObj);

/*! \ingroup Global
 *	\brief 說明：創建fIPCamBase物件。
 *
 *  取得fIPCamBase Base class版本，再載入軟體時需檢查
 *  ，以防止fIPCamBase不同不相容造成程式庫不相容，可能引起當機問題；
 *  當修改fIPCamBase、CIPCamClient、fSocketTool需要推進版本。
 *
 *  \return 整數，表示fIPCambase的版本。
 */
LIBIPCAM_API int IPCAM_GetIPCambaseVersion(void);

/*! \ingroup Global
 * \brief 函式庫初始化函數。
 *
 *  \return 0:成功，其他失敗
 */
LIBIPCAM_API int IPCAM_InitLibrary(void);

/*! \ingroup Global
 *  \brief 釋放函式庫資原。
 *
 *  \return 0：表成功，其他失敗
 */
LIBIPCAM_API int IPCAM_DeinitLibrary(void);


#endif
