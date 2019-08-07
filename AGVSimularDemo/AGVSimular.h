/*!
 * @file AGVSimular.c
 * @brief ��λ��AGV����ͨ���ļ�
 * @author FanKaiyu
 * @date 2019-04-08
 * @version 1.3
 *
 * @update FanKaiyu 2019-04-10 1�����ӱ���g_usNextRFID;2���޸�FeedbackHeartbeat����,����NextRFID�ظ�
 * @update FanKaiyu 2019-04-15 1������״̬_AGVSTA_ARM 2���޸�ProcessData����
 * @update FanKaiyu 2019-04-25 1������_AGVSTA_ALLSCREAM 2������ PACKAGE_HEAD��PACKAGE_TAIL��PACKAGE_TRANS 3���޸�ProcessData��Translation��Retranslation��CreatePackage����
*/
#include "CRC.c"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _C_AGVSIMULAR
#define _C_AGVSIMULAR

/*! @brief AGV����״̬ */
enum _AGVPOWER
{
	_AGVPOWER_EMPTY = 0,		/*!< δ��⵽��� */
	_AGVPOWER_LOW = 20,			/*!< �͵��� */
	_AGVPOWER_NORMAL = 50,		/*!< �������� */
	_AGVPOWER_HIGH = 90,		/*!< �ߵ��� */
	_AGVPOWER_FULL = 100,		/*!< ����*/
};

/*! @brief AGV����״̬ */
enum _AGVSTATUS
{
	_AGVSTA_WAIT,		/*!< ����״̬.\
						��λ�����յ���״̬,��ΪAGV�Ѿ�׼���ò����Խ�����λ���Ŀ��Ƶ���.\
						��ʱ��λ�����Խ��ղ�������λ�����͵��������͵ı���.\
						*/
	_AGVSTA_RUN,		/*!< ����״̬.\
						��λ�����յ���״̬,��ΪAGV���ƶ�������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���ƶ�������������뽫AGV״̬�ָ�Ϊ����״̬.\
						*/
	_AGVSTA_STOP,		/*!< ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV����Ϊֹͣ.�����ֶ�����;��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.�û������ֶ�����AGV,���û�������������뽫AGV״̬�ָ�Ϊ����״̬.\
						*/
	_AGVSTA_SCREAM,		/*!< ��ͣ״̬.\
						��λ�����յ���״̬,��ΪAGV������ֹͣ����,�����ֶ�����.��ʱ��λ�����ϱ��������û�,���ҳ�������״̬�ⲻ��������.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���û�������������뽫AGV״̬�ָ�Ϊ����״̬.
						*/
	_AGVSTA_FIND,		/*!< Ѱ��/��λ״̬.\
						��λ�����յ���״̬,��ΪAGV������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.��λ��Ӧ����Ѱ��/��λ����,��һ��ʱ����Ѱ��/��λʧ�ܺ�,Ӧ��AGV״̬��Ϊֹͣ,�����쳣״̬��Ϊ�Ѵ�/�޷���λ.\
						*/
	_AGVSTA_OBSDOWN,	/*!< �ǽӴ�ʽ���ϼ���״̬.\
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_OBSSTOP,	/*!< �ǽӴ�ʽ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_MOBSSTOP,	/*!< �Ӵ�ʽ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV�����Ӵ�ʽ���ϲ�����ֹͣ״̬,��ʱ��λ���ϱ��쳣���û�,���ҳ��˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_TRAFFIC,	/*!< ��ͨ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV���ڽ�ͨ���Ƶ�ȴ���λ������,��ʱ��λ�����ݽ�ͨ���ƹ��������״̬��AGV�Ƿ�����ͨ��.\
						��ʱ��λ�����˴�����λ�����������������Լ���ͨ���ƿ��Ʊ���,�������ľ����Բ�����.\
						*/
	_AGVSTA_ARM,		/*!< ��е�ַ���.\
						��λ�����յ���״̬,�����е���Ѿ���AGV�������.
						*/
	_AGVSTA_ALLSCREAM,	/*!< ȫ�߼�ͣ */
};

/*! @brief AGV������Ϣ�� */
enum _AGVACTION
{
	_AGVACT_NONE,		/*!< ��/ֹͣ����.
						δִ��״̬:AGV�յ��˶���,��AGV����ִ����������,��ֹͣ������ִ��.
						ִ��״̬:������.
						ִ�����״̬:������.
						*/
	_AGVACT_MOVE,		/*!< �ƶ�����
						δִ��״̬:AGV�յ��˶���,��AGV��ʼ�ƶ�����ֹ�ر꿨.��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.���򽫶���״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�,������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_REMOVE,		/*!< �����ƶ�����
						δִ��״̬:AGV�յ��˶���,��AGV�����ƶ�����ֹ����.��AGV��ǰ��������ֹ������ͬ�����ƶ�ʱ,��AGV�ɽ���ǰ�������ú������ƶ�.������״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_TRAFFIC,	/*!< ��ͨ���ƶ���
						δִ��״̬:AGV�յ��˶���,��AGV����ֹ�ر꿨�ȴ���ͨ����.
						ִ��״̬:������.
						ִ�����״̬:AGV�յ��˶���,��AGV�����뿪��ֹ�ر꿨.
						*/
	_AGVACT_LIFTUP,		/*!< ��������������.
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ����������;
								�Բ泵ʽAGV�����ǻ���ִ��������;
								�Ի�е��AGV�����ǻ�е��ִ��ץȡ����,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ����Ѵ����AGV�Ϻ��ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_LIFTDOWN,	/*!< �������½�
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ���½�����;
								�Բ泵ʽAGV�����ǻ���ִ���½�����;
								�Ի�е��AGV�����ǻ�е��ִ���·Ŷ���,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ������뿪AGV���ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
};

/*! @brief AGV����״̬��Ϣ�� */
enum _AGVACTIONSTATUS
{
	_AGVACTSTA_NONE,	/*!< ����δִ��״̬ */
	_AGVACTSTA_EXE,		/*!< ����ִ����״̬ */
	_AGVACTSTA_FINISH,	/*!< ����ִ�����״̬ */
	_AGVACTSTA_PAUSE,	/*!< ����ִ����ͣ״̬ */
};

/*! @brief AGV�쳣��Ϣ��*/
enum _AGVERROR
{
	_AGVERR_NONE,		/*!< ��/����
						˵��:AGV��������.
						��λ��:������.
						��λ��:������.
						*/
	_AGVERR_MISS,		/*!< �Ѵ�/�޷���λ
						˵��:�����û���AGV�ָ��������,��ֹ���Ѵ���ɵ��������̿���.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_PATH,		/*!< ·���쳣
						˵��:����λ�����͵��ƶ�ָ����󣨼�û�ж�Ӧ���ƶ�·���������.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_LOCATE,		/*!< �����쳣
						˵��:��AGVδ������λ��������ָ��λ�������.��λ��Ӧ���������ƶ����ƶ�����,ʹAGV���µ���ָ��λ��,��ִ֤�ж����İ�ȫ��.
						��λ��:�ϱ��쳣���û�.
						��λ��:����ǰ����������.
						*/
	_AGVERR_DIRECTION,	/*!< �����쳣
						˵��:��ȱ����Գ���������쳣��
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_ACT = -2,	/*!< �����쳣
						˵��:��PC��λ��������ִ��ĳһ������ʱ��δ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
	_AGVERR_COMM,		/*!< ͨ���쳣
						˵��:��PC��λ���������޷����ͱ��Ļ�ʱ��δ���ձ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
};

/*! @brief ������״̬ */
enum _AGVLIFTER
{
	_AGVLIFTER_DOWN,	/*!< ���� */
	_AGVLIFTER_UP,		/*!< ���� */
};

/*! @brief �ػ�״̬ */
enum _AGVCARGO
{
	_AGVCARGO_EMPTY = 0,	/*!< ���� */
	_AGVCARGO_FULL = 255,	/*!< ���� */
};

#define PACKAGE_HEAD 0xBA
#define PACKAGE_TAIL 0xBE
#define PACKAGE_TRANS 0xB0

#define MAX_RECEIVE_LEN 1000
#define MAX_PACKAGE_LEN 1000
#define MAX_SEND_LEN 1000

#define CMD_HEARTBEAT "Heartbeat"
#define CMD_MOVE "Move"
#define CMD_REMOVE "Remove"
#define CMD_TRAFFIC_PASS "TrafficPass"
#define CMD_TRAFFIC_STOP "TrafficStop"
#define CMD_SHUTDOWN "Shutdown"
#define CMD_SCREAM "Scream"
#define CMD_REST "Reset"
#define CMD_RESTART "Restart"
#define CMD_LIFT_UP "LiftUp"
#define CMD_LIFT_DOWN "LiftDown"
#define CMD_STOP "Stop"
#define CMD_ACTION "Other"
#define CMD_SET_STATUS "Set"
#define CMD_PAUSE "Pause"
#define CMD_CONTINUE "Continue"
#define CMD_SPEED_CONTROL "SetSpeed"
#define CMD_ALLSCREAM "AllScream"

extern char g_achReceive[MAX_RECEIVE_LEN];	/*!< ���ݽ��ջ�����:���Դ�Ž��յ����� */
extern char g_achPackage[MAX_PACKAGE_LEN];	/*!< ���������建����:���Դ�Ž�������JSON�ַ��� */
extern char g_achSend[MAX_SEND_LEN];		/*!< ���ݷ��ͻ�����:���Դ��Ҫ���͸������� */

extern unsigned char g_byNo;				/*!< ��� */
extern unsigned char g_byStatus;			/*!< ״̬ */
extern char g_chSpeed;						/*!< �ٶ� */
extern unsigned char g_byPower;				/*!< ���� */
extern unsigned short g_usCurRFID;			/*!< �н���ǰRFID����� */
extern unsigned short g_usNextRFID;			/*!< �н���һRFID����� */
extern unsigned short g_usEndRFID;			/*!< �յ�RFID����� */
extern unsigned char g_byCargo;				/*!< �ػ�״̬ */
extern unsigned char g_byLifter;			/*!< ������״̬ 0���� 1���� */
extern char g_chError;						/*!< �쳣��Ϣ */
extern unsigned short g_usAction;			/*!< ִ�еĶ��� 0�޶��� 1�ƶ� 2�����ƶ� 3��ͨ���� 4���������� 5�������½� */
extern unsigned char g_byActStatus;			/*!< ����״̬  0δִ�� 1ִ���� 2ִ����� */

/*!
 * @brief ��ʼ��
 *
 * ��ʼ��AGV��š�״̬
 * @param unsigned char AGV���
*/
void InitAGV(const unsigned char byNo);

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

#endif //!_C_AGVSIMULAR
