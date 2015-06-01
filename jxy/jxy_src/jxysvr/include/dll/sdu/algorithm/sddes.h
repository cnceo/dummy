
#ifndef SDDES_H
#define SDDES_H
/**
* @file sddes.h

* @brief ����DES�����
*
**/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <time.h>
#include "sdtype.h"
#include <openssl/des.h>
namespace SGDP
{
    /**
    * @defgroup groupcrypto �ӽ��ܲ���
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief DES enc/dec type macros
    * DES����/�����
    */
#define DES_ENCRYPT  1
#define DES_DECRYPT  0

    /**
    * @brief typedef DES context structure
    * key schedule table
    * ����DES�����Ľṹ��
    * �ؼ����ȱ�
    */
    typedef struct tagDESContext
    {
        INT32		nEnDecMode;			/**<encrypt/decrypt*/
        UINT32      aSubKey[32];		/**<DES sub-keys*/
    } SDESContext;

    /**
    * @brief typedef DES block structure
    * 64bits key (equal to IV)
    * ����DES��ṹ
    * 64λkey������IV��
    */
    typedef struct tagDESBlock
    {
        UCHAR acBlock[8];
    } SDESBlock;

    /**
    * @brief typedef 3-DES context structure
    * key schedule table
    * ����3��DES�����Ľṹ��
    * �ؼ����ȱ�
    */
    typedef struct tagDES3Context
    {
        INT32		nEnDecMode;			/**<encrypt/decrypt*/
        UINT32      aSubKey[96];		/**<DES sub-keys*/
    } SDES3Context;

    /**
    * @brief typedef 3-DES block structure
    * 192bits key (equal to IV)
    * ����3DES��ṹ��
    * 192λkey������IV��
    */
    typedef struct tagDES3Block
    {
        UCHAR acBlock[24];
    } SDES3Block;


    typedef struct tagDesKey
    {
        DES_cblock          stDesKey;
        DES_key_schedule    stKeySche;
    }SDDES_KEY;
    /**
    * @brief des �ӽ��ܷ�װ�֧࣬��3��DES��DES���ַ�ʽ
    */
    class CSDDes
    {
    public:
        /**
        * @brief
        * initialize des block ��ʼ��DES��
        * @param pIV : [in/out]DES block structure to be initialized
        * �������������������ʼ����DES��ṹ
        * @return VOID
        */
        VOID  DESGenKey(SDESBlock *pIV);

        /**
        * @brief
        * initialize des-3 block ��ʼ��3��DES��
        * @param pIV : [in/out]DES-3 block structure to be initialized
        * �������������������ʼ����3��DES��ṹ
        * @return VOID
        */
        VOID  DESGenKey(SDES3Block *pIV);

        /**
        * @brief
        * Generate DES encrypt key ����DES�����
        * @param pCtx : [in/out]DES context to be initialized
        * �������������������ʼ����DES������
        * @param oKey : [in]des block [�������]DES��
        * @return VOID
        */
        VOID  DESGenEncKeySche(SDESContext *pCtx, SDESBlock &oKey);

        /**
        * @brief
        * Generate DES-3 encrypt key ����3��DES�����
        * @param pCtx : [in/out]DES-3 context to be initialized
        * [�����������]������ʼ����DES������
        * @param oKey : [in]des block [�������]DES��
        * @return VOID
        */
        VOID  DESGenEncKeySche(SDES3Context *pCtx, SDES3Block &oKey);

        /**
        * @brief
        * Generate DES decrypt key ����DES�����
        * @param pCtx : [in/out]DES context to be initialized
        * [�����������]������ʼ����DES������
        * @param oKey : [in]des block [�������]DES��
        * @return VOID
        */
        VOID  DESGenDecKeySche(SDESContext *pCtx, SDESBlock &oKey);

        /**
        * @brief
        * Generate DES-3 decrypt key ����3��DES�����
        * @param pCtx : [in/out]DES-3 context to be initialized
        * [�����������]������ʼ����3��DES������
        * @param oKey : [in]des block [�������]DES��
        * @return VOID
        */
        VOID  DESGenDecKeySche(SDES3Context *pCtx, SDES3Block &oKey);

        /**
        * @brief
        * DES buffer encryption DES�������
        * @param pCtx : [in]DES context [�������]DES������
        * @param pIV :[in] DES block [�������]DES��
        * @param nInLen : [in]input buffer len [�������]���뻺��������
        * @param pInput : [in]input buffer [�������]���뻺����
        * @param pOutput : [in/out]output buffer [�����������]���������
        * @return VOID
        */
        VOID  DESEncryptNCBC(SDESContext *pCtx,
                                  SDESBlock *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES-3 buffer encryption 3��DES�������
        * @param pCtx : [in]DES-3 context [�������]3��DES������
        * @param pIV : [in]DES-3 block[�������] 3��DES��
        * @param nInLen : [in]input buffer len [�������]���뻺��������
        * @param pInput : [in]input buffer [�������]���뻺����
        * @param pOutput : [in/out]output buffer [�����������]���������
        * @return VOID
        */
        VOID  DESEncryptNCBC(SDES3Context *pCtx,
                                  SDES3Block *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES buffer decryption DES�������
        * @param pCtx : [in]DES context [�������]DES������
        * @param pIV : [in]DES block [�������]DES��
        * @param nInLen : [in]input buffer len [�������]���뻺��������
        * @param pInput : [in]input buffer [�������]���뻺����
        * @param pOutput : [in/out]output buffer [�����������]���������
        * @return VOID
        */
        VOID  DESDecryptNCBC(SDESContext *pCtx,
                                  SDESBlock *pIV,
                                  INT32  nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES-3 buffer decryption 3��DES�������
        * @param pCtx : [in]DES-3 context [�������]3��DES������
        * @param pIV : [in]DES-3 block [�������]3��DES��
        * @param nInLen : [in]input buffer len [�������]���뻺�峤��
        * @param pInput : [in]input buffer [�������]���뻺����
        * @param pOutput : [in/out]output buffer [�����������]���������
        * @return VOID
        */
        VOID  DESDecryptNCBC(SDES3Context *pCtx,
                                  SDES3Block *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);



    };

}

#endif //SD_DES_H

