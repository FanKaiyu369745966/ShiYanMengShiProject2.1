#include "AGVSimular.h"

#ifdef _C_AGVSIMULAR

char g_achReceive[MAX_RECEIVE_LEN];					/*!< ���ݽ��ջ�����:���Դ�Ž��յ����� */
char g_achPackage[MAX_PACKAGE_LEN];					/*!< ���������建����:���Դ�Ž�������JSON�ַ��� */
char g_achSend[MAX_SEND_LEN];						/*!< ���ݷ��ͻ�����:���Դ��Ҫ���͸������� */

unsigned char g_byNo = 1;							/*!< ��� */
unsigned char g_byStatus = _AGVSTA_WAIT;			/*!< ״̬ */
char g_chSpeed = 0;									/*!< �ٶ� */
unsigned char g_byPower = _AGVPOWER_EMPTY;			/*!< ���� */
unsigned short g_usCurRFID = 0;						/*!< �н���ǰRFID����� */
unsigned short g_usNextRFID = 0;					/*!< �н���һRFID����� */
unsigned short g_usEndRFID = 0;						/*!< �յ�RFID����� */
unsigned char g_byCargo = _AGVCARGO_EMPTY;			/*!< �ػ�״̬ */
unsigned char g_byLifter = _AGVLIFTER_DOWN;			/*!< ������״̬ 0���� 1���� */
char g_chError = _AGVERR_NONE;						/*!< �쳣��Ϣ */
unsigned short g_usAction = _AGVACT_NONE;			/*!< ִ�еĶ��� 0�޶��� 1�ƶ� 2�����ƶ� 3��ͨ���� 4���������� 5�������½� */
unsigned char g_byActStatus = _AGVACTSTA_NONE;		/*!< ����״̬  0δִ�� 1ִ���� 2ִ����� */

void InitAGV(const unsigned char byNo)
{
	g_byNo = byNo;
	g_byStatus = _AGVSTA_WAIT;
	g_chSpeed = 0;
	g_byPower = _AGVPOWER_EMPTY;
	g_usCurRFID = 0;
	g_usNextRFID = 0;
	g_usEndRFID = 0;
	g_byCargo = _AGVCARGO_EMPTY;
	g_byLifter = _AGVLIFTER_DOWN;
	g_chError = _AGVERR_NONE;
	g_usAction = _AGVACT_NONE;
	g_byActStatus = _AGVACTSTA_NONE;

	memset(g_achReceive, 0, MAX_RECEIVE_LEN);
	memset(g_achPackage, 0, MAX_PACKAGE_LEN);
	memset(g_achSend, 0, MAX_SEND_LEN);
}

void Translation(const char* pSourceData, int nSourceLen, char* pTransData, int* pTransLen)
{
	/*!
	 * �����������е���0xBA������ת��Ϊ0xB0+0x01
	 * �����������е���0xBE������ת��Ϊ0xB0+0x02
	 * �����������е���0xB0������ת��Ϊ0xB0+0x03
	*/
	size_t max_tmp = (size_t)nSourceLen * 2;
	char* lpszTmp = (char*)malloc(max_tmp);	/*!<  ��ʱ���� */

	int nT = 0;	/*!< ת������ݳ��� */
	int nS = 0;

	if (lpszTmp == NULL)
	{
		return;
	}

	if (max_tmp < 4)
	{
		free(lpszTmp);
		return;
	}

	// ��ʼ��
	memset(lpszTmp, 0, max_tmp);

	for (nS = 0; nS < nSourceLen; ++nS, ++nT)
	{
		switch (*(pSourceData + nS) & 0xFF)
		{
		case PACKAGE_HEAD:
		{
			lpszTmp[nT] = (char)PACKAGE_TRANS;
			lpszTmp[++nT] = (char)0x01;
			break;
		}
		case PACKAGE_TAIL:
		{
			lpszTmp[nT] = (char)PACKAGE_TRANS;
			lpszTmp[++nT] = (char)0x02;
			break;
		}
		case PACKAGE_TRANS:
		{
			lpszTmp[nT] = (char)PACKAGE_TRANS;
			lpszTmp[++nT] = (char)0x03;
			break;
		}
		default:
		{
			lpszTmp[nT] = *(pSourceData + nS) & 0xFF;
			break;
		}
		}
	}

	*pTransLen = nT;
	// ����ת������
	memcpy(pTransData, lpszTmp, nT);

	// �ͷ��ڴ�
	free(lpszTmp);

	return;
}

void Retranslation(const char* pTransData, int pTransLen, char* pSourceData, int* nSourceLen)
{
	char* lpszTmp = (char*)malloc(pTransLen);	/*!< ��ʱ���� */
	int nS = 0;	/*!< Դ���ݳ��� */
	int nT = 0;

	if (lpszTmp == NULL)
	{
		return;
	}

	// ��ʼ��
	memset(lpszTmp, 0, pTransLen);


	for (nT = 0; nT < pTransLen; ++nT, ++nS)
	{
		if ((*(pTransData + nT) & 0xFF) == PACKAGE_TRANS)
		{
			++nT;
			switch (*(pTransData + nT))
			{
			case 0x01:
			{
				lpszTmp[nS] = (char)PACKAGE_HEAD;
				break;
			}
			case 0x02:
			{
				lpszTmp[nS] = (char)PACKAGE_TAIL;
				break;
			}
			case 0x03:
			{
				lpszTmp[nS] = (char)PACKAGE_TRANS;
				break;
			}
			}
		}
		else
		{
			lpszTmp[nS] = *(pTransData + nT) & 0xFF;
		}
	}

	*nSourceLen = nS;
	// ����Դ����
	memcpy(pSourceData, lpszTmp, nS);

	// �ͷ��ڴ�
	free(lpszTmp);

	return;
}

int ProcessData(const unsigned char* pProcessData, const unsigned int MAX_LEN, unsigned char* pJson, const unsigned int MAX_JSON_LEN)
{
	char* pLeftData = (char*)pProcessData;	/*!< ʣ������ */
	int nLeftDataLen = MAX_LEN;				/*!< ʣ�����ݳ��� */
	char* pHead = NULL;						/*!< ����ͷ��ַ */
	char* pTail = NULL;						/*!< ����β��ַ */
	int nBody = 0;							/*!< �����峤�� */
	char* pSourceData = NULL;				/*!< Դ���� */
	int nSourceLen = 0;						/*!< Դ���ݳ��� */
	unsigned short usPackLen = 0;			/*!< ���ĳ��� */
	unsigned short usPackCheck = 0;			/*!< У��ֵ */
	unsigned short usRealLen = 0;			/*!< ʵ�ʳ��� */
	unsigned short usRealCheck = 0;			/*!< ʵ��У��ֵ */

	pHead = (char*)memchr(pLeftData, PACKAGE_HEAD, nLeftDataLen);

	// ʣ���������ޱ���ͷ
	if (pHead == NULL)
	{
		return -1;
	}

	pTail = (char*)memchr(pHead + 1, PACKAGE_TAIL, MAX_LEN - (pHead - (char*)pProcessData) - 1);

	// ʣ���������ޱ���β
	if (pTail == NULL)
	{
		return 0;
	}

	nBody = pTail - pHead - 1;
	pSourceData = (char*)malloc(nBody);
	nSourceLen = 0;

	if (pSourceData == NULL)
	{
		return -1;
	}

	if (nBody < 2)
	{
		free(pSourceData);

		return -1;
	}

	// ��ʼ��
	memset(pSourceData, 0, nBody);
	// ��ת��
	Retranslation(pHead + 1, nBody, pSourceData, &nSourceLen);

	usPackLen = 0;
	usPackLen = ((pSourceData[0] & 0xFF) << 8) | (pSourceData[1] & 0xFF);

	usRealLen = nSourceLen - 4;

	// ���ݳ��Ȳ���ͬ
	if (usPackLen != usRealLen)
	{
		// �ͷ�Դ����
		free(pSourceData);

		return -1;
	}

	usPackCheck = 0;
	usPackCheck = (*(pSourceData + 2 + usPackLen) & 0xFF) << 8 | (*(pSourceData + 3 + usPackLen) & 0xFF);

	usRealCheck = CRC16((unsigned char*)pSourceData, usRealLen + 2);

	// У��ֵ��ͬ
	if (usPackCheck != usRealCheck)
	{
		// �ͷ�Դ����
		free(pSourceData);

		memset(g_achReceive, 0, MAX_RECEIVE_LEN);

		return -1;
	}

	// ��ʼ��
	memset(pJson, 0, MAX_JSON_LEN);

	// ����������
	memcpy(pJson, pSourceData + 2, usPackLen);

	// �ͷ�Դ����
	free(pSourceData);

	return usPackLen;
}

void CreatePackage(const char* lpszStr, const int nLen, char* pData, int* nSize)
{
	int nSourceLen = nLen + 4;						/*!< Դ���ݳ��� ��������λ */
	int nTransLen = nSourceLen * 2 + 2;				/*!< ת����������ݳ��� ������λ */
	unsigned short usCheck = 0;						/*!< У��ֵ */
	char* pTrans = NULL;							/*!< ����ת�������� */
	int nTrans = 0;									/*!< ת����ʵ�����ݳ��� */

	char* lpszSource = (char*)malloc(nSourceLen);	/*!< ������Ҫת���Դ���� */

	if (lpszSource == NULL)
	{
		return;
	}

	if (nSourceLen < 5)
	{
		free(lpszSource);
		return;
	}

	// ��ʼ��Դ����
	memset(lpszSource, 0, nSourceLen);
	// �������ݳ��ȣ���λ��
	lpszSource[0] = (nLen >> 8) & 0xFF;
	// �������ݳ��ȣ���λ��
	lpszSource[1] = nLen & 0xFF;
	// ������������
	memcpy(&lpszSource[2], lpszStr, nLen);

	usCheck = CRC16((unsigned char*)lpszSource, nLen + 2);
	// ����У��ֵ����λ��
	lpszSource[nSourceLen - 2] = (usCheck >> 8) & 0xFF;
	// ����У��ֵ(��λ)
	lpszSource[nSourceLen - 1] = usCheck & 0xFF;

	pTrans = (char*)malloc(nTransLen);
	nTrans = 0;

	if (pTrans == NULL)
	{
		free(lpszSource);
		return;
	}

	// ��ʼ��ת������
	memset(pTrans, 0, nTransLen);
	// ����ת��
	Translation(lpszSource, nSourceLen, &pTrans[1], &nTrans);
	//  �ͷ�Դ�����ڴ�
	free(lpszSource);

	pTrans[0] = PACKAGE_HEAD;
	pTrans[nTrans + 1] = PACKAGE_TAIL;

	*nSize = nTrans + 2;
	// ����ת������
	memcpy(pData, pTrans, *nSize);

	// �ͷ�ת�������ڴ� 
	free(pTrans);

	return;
}

int FeedbackHeartbeat(const char* pSource, const int pSourceLen, char* pPack, const int MAX_PACK_LEN)
{
	char* lpszStr = (char*)malloc(MAX_PACK_LEN);	/*!< JSON�ַ��� */
	int nPackLen = 0;								/*!< ���ݰ����� */

	if (lpszStr == NULL)
	{
		return -1;
	}

	// ��ʼ��
	memset(lpszStr, 0, MAX_PACK_LEN);

	// �ϳ�JSON�ַ���
	sprintf(lpszStr,
		"{\"Message\":%s,\"Result\":{\"Status\":%d,\"Speed\":%d,\"Power\":%d,\"RFID\":{\"Cur\":%d,\"Next\":%d,\"End\":%d},\"Cargo\":%d,\"Lifter\":%d,\"Error\":%d,\"Action\":%d,\"ActStatus\":%d}}"
		, pSource, g_byStatus, g_chSpeed, g_byPower, g_usCurRFID, g_usNextRFID, g_usEndRFID, g_byCargo, g_byLifter, g_chError, g_usAction, g_byActStatus);

	// ��ʼ��
	memset(pPack, 0, MAX_PACK_LEN);

	CreatePackage(lpszStr, strlen(lpszStr), pPack, &nPackLen);

	// �ͷ��ڴ�
	free(lpszStr);

	return nPackLen;
}

int FeedbackOther(const char* pSource, const int pSourceLen, char* pPack, const int MAX_PACK_LEN)
{
	char* lpszStr = (char*)malloc(MAX_PACK_LEN);	/*!< JSON�ַ��� */
	int nPackLen = 0;								/*!< ���ݰ����� */

	if (lpszStr == NULL)
	{
		return -1;
	}

	// ��ʼ��
	memset(lpszStr, 0, MAX_PACK_LEN);

	// �ϳ�JSON�ַ���
	sprintf(lpszStr,
		"{\"Message\":%s,\"Result\":\"Success\"}"
		, pSource);

	// ��ʼ��
	memset(pPack, 0, MAX_PACK_LEN);

	CreatePackage(lpszStr, strlen(lpszStr), pPack, &nPackLen);

	// �ͷ��ڴ�
	free(lpszStr);

	return nPackLen;
}

#endif //_C_AGVSIMULAR