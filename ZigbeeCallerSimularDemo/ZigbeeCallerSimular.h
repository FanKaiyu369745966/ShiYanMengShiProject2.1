/*!
 * @file ZigbeeCallerSimular.c
 * @brief Zigbee����������ͨ���ļ�
 * @author FanKaiyu
 * @date 2019-04-16
 * @version 1.1
 *
 * @update FanKaiyu 2019-04-25 1������ PACKAGE_HEAD��PACKAGE_TAIL��PACKAGE_TRANS 2���޸�ProcessData��Translation��Retranslation��CreatePackage����
 * @update FanKaiyu 2019-05-04 �޸ı���:��ԭJSON���ı��16��ֹ����.�����ĸ�Ϊ"���+������+����"����ɷ�ʽ.
*/

/*! 
 * @brief ���Ľ���
 * 
 * 1����������
 * ��� 0x01-0xFF
 * ������ 0x1F
 * ���� ��
*/

#include "CRC.c"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _C_ZIGBEECALLER
#define _C_ZIGBEECALLER

#define PACKAGE_HEAD 0xBA
#define PACKAGE_TAIL 0xBE
#define PACKAGE_TRANS 0xB0

#define MAX_RECEIVE_LEN 1000
#define MAX_PACKAGE_LEN 1000
#define MAX_SEND_LEN 1000

/*! @brief ������ */
enum FUNC_TYPE
{
	FUNC_HEARTBEAT = 0x1F,
	FUNC_RESET = 0x2F,
	FUNC_SETSIGNAL = 0x3F,
};

/*! @brief ���������� */
enum SIGNAL_TYPE
{
	CALL_SIGNAL = 1,
	CANCELCALL_SIGNAL,
	PASS_SIGNAL,
	CANCELPASS_SIGNAL,
	NG_SIGNAL,
	CANCELNG_SIGNAL,
	SCREAM,
};

/*! @brief �������źſ��� */
enum CALL_SIGNAL
{
	SIGNAL_OFF,
	SIGNAL_ON,
};

extern char g_achReceive[MAX_RECEIVE_LEN];		/*!< ���ݽ��ջ�����:���Դ�Ž��յ����� */
extern char g_achPackage[MAX_PACKAGE_LEN];		/*!< ���������建����:���Դ�Ž�������JSON�ַ��� */
extern char g_achSend[MAX_SEND_LEN];			/*!< ���ݷ��ͻ�����:���Դ��Ҫ���͸������� */

extern unsigned char g_byNo;					/*!< ��� */
extern unsigned char g_byCall;					/*!< ���б�ʶ */
extern unsigned char g_byCancelCall;			/*!< ȡ�����б�ʶ */
extern unsigned char g_byPass;					/*!< ���б�ʶ */
extern unsigned char g_byCancelPass;			/*!< ȡ�����б�ʶ */
extern unsigned char g_byNG;					/*!< NG��ʶ */
extern unsigned char g_byCancelNG;				/*!< ȡ��NG��ʶ */
extern unsigned char g_byScream;				/*!< ��ͣ��ʶ */

/*!
 * @brief ��ʼ��
 *
 * ��ʼ����������š�״̬
 * @param unsigned char ���������
*/
void InitCaller(unsigned char byNo);

/*!
 * @brief ת��
 * @param const char* ��ת�������
 * @param const int ��ת������ݳ���
 * @param char* ת��������
 * @param int& ת�������ݳ���
*/
void Translation(const char* pSourceData, int nSourceLen, char* pTransData, int* pTransLen);

/*!
 * @brief ��ת��
 * @param const char* �跴ת�������
 * @param const int �跴ת������ݳ���
 * @param char*Դ����
 * @param int& Դ���ݳ���
*/
void Retranslation(const char* pTransData, int pTransLen, char* pSourceData, int* nSourceLen);

/*!
 * @brief ���Ľ���
 *
 * �����յ��ı�������������
 * @param const unsigned char* ��Ž������ݵ�����
 * @param unsigned int ���鳤��
 * @param unsigned char* ��Ž������ı��ĵ�����ָ��(Json�ַ���)
 * @param const unsigned int �������ݳ���
 * @return int ����ʵ�ʽ������ı��ĳ���(Json�ַ�������),����0��δ�յ���������,����-1�򲻴�����Ч����
*/
int ProcessData(const unsigned char* pProcessData, const unsigned int MAX_LEN, unsigned char* pJson, const unsigned int MAX_JSON_LEN);

void CreatePackage(const char* lpszStr, const int nLen, char* pData, int* nSize);

/*!
 * @brief ������������
 *
 * ���Է�����������
 * @param const char* ������λ���ı���(Json�ַ���)
 * @param const int ���ĳ���
 * @param char* ��ŷ������ĵ�����ָ��
 * @param const int ����ĳ���
 * @return int ʵ�ʱ��ĳ���
*/
int FeedbackHeartbeat(const char* pSource, const int pSourceLen, char* pPack, const int MAX_PACK_LEN);

/*!
 * @brief ������������
 *
 * ���Է���������ҪAGV�����ı���
 * @param const char* ������λ���ı���(Json�ַ���)
 * @param const int ���ĳ���
 * @param char* ��ŷ������ĵ�����ָ��
 * @param const int ����ĳ���
 * @return int ʵ�ʱ��ĳ���
*/
int FeedbackOther(const char* pSource, const int pSourceLen, char* pPack, const int MAX_PACK_LEN);

#endif //!_C_ZIGBEECALLER

