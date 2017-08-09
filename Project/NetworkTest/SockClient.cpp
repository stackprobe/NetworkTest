#include "all.h"

#define RECVSIZE_MAX 1000000

static char *GetServerHost(void) // ret: bind
{
	return "localhost";
}
static int GetServerPort(void)
{
	return 80;
}
static char *GetServiceName(void) // ret: bind
{
	return "EchoTest";
}

// ---- DoSend, DoRecv ----

static int DoWait(int sock, int millis, int forWrite)
{
	int retval;

	uncritical();
	{
		fd_set fs;
		struct timeval tv;

		FD_ZERO(&fs);
		FD_SET(sock, &fs);
		tv.tv_sec = millis / 1000;
		tv.tv_usec = (millis % 1000) * 1000;

		retval = select(sock + 1,
			forWrite ? NULL : &fs,
			forWrite ? &fs : NULL,
			NULL,
			&tv
			);
	}
	critical();

	return retval;
}
static int DoSendData(int sock, void *data, int dataSize)
{
	return send(sock, (const char *)data, dataSize, 0);
}
static int DoRecvData(int sock, void *data, int dataSize)
{
	return recv(sock, (char *)data, dataSize, 0);
}
static int DoTransmit(int sock, void *data, int dataSize, int millis, int forWrite) // ret: 送信したバイト数, -1 == エラー
{
	if(dataSize == 0)
		return 0;

	int retval = DoWait(sock, millis, forWrite);

	if(retval <= 0)
		return retval;

	retval = (forWrite ? DoSendData : DoRecvData)(sock, data, dataSize);

	if(retval == 0) // ? 対岸から切断された。
		retval = -1;

	return retval;
}
static int DoSend(int sock, void *data, int dataSize, int millis)
{
	return DoTransmit(sock, data, dataSize, millis, 1);
}
static int DoRecv(int sock, void *data, int dataSize, int millis)
{
	return DoTransmit(sock, data, dataSize, millis, 0);
}
static int DoTransmitAll(int sock, void *data, int dataSize, int forWrite) // ? 成功
{
	int index = 0;

	while(index < dataSize)
	{
		if(!KeepThread)
			return 0;

		int retval = (forWrite ? DoSend : DoRecv)(sock, (uchar *)data + index, dataSize - index, 15);

		if(retval == -1)
			return 0;

		index += retval;
	}
	return 1;
}
static int DoSendAll(int sock, void *data, int dataSize)
{
	return DoTransmitAll(sock, data, dataSize, 1);
}
static int DoRecvAll(int sock, void *data, int dataSize)
{
	return DoTransmitAll(sock, data, dataSize, 0);
}
static int DoSendAll(int sock, char *str)
{
	return DoSendAll(sock, str, strlen(str));
}
static int DoSendInt(int sock, int value)
{
	uchar buff[4];

	buff[0] = value % 256;
	value /= 256;
	buff[1] = value % 256;
	value /= 256;
	buff[2] = value % 256;
	buff[3] = value / 256;

	return DoSendAll(sock, buff, 4);
}
static int DoRecvInt(int sock, int defval = -1)
{
	uchar buff[4];

	if(!DoRecvAll(sock, buff, 4))
		return defval;

	int value = buff[3];
	value *= 256;
	value += buff[2];
	value *= 256;
	value += buff[1];
	value *= 256;
	value += buff[0];

	return value;
}

// ---- Hash ----

static void MakeHash(autoList<uchar> *data, uchar hash[16])
{
	autoList<uchar> *sha512 = sha512_block(data);
	memcpy(hash, sha512->ElementAt(0), 16);
	delete sha512;
}
static void PutHash(autoList<uchar> *data)
{
	uchar hash[16];
	MakeHash(data, hash);
	data->AddElements(hash, 16);
}
static int UnputHash(autoList<uchar> *data) // ret: ? 正常なデータ
{
	if(data->GetCount() < 16)
		return 0;

	uchar hash[16];
	memcpy(hash, data->ElementAt(data->GetCount() - 16), 16);
	data->SetCount(data->GetCount() - 16);

	uchar realHash[16];
	MakeHash(data, realHash);

	return !memcmp(hash, realHash, 16);
}

// ----

#define SEND_RECV_ERROR { LOGPOS(); goto endFunc; }

static autoList<uchar> *SendRecv(int sock, autoList<uchar> *sendData) // ret: NULL == 失敗
{
	autoList<uchar> *recvData = NULL;
	uchar clSeed[16];
	uchar svSeed[16];
	uchar encCounter[16];

	if(!DoSendAll(sock, GetServiceName()))
		SEND_RECV_ERROR

	if(!DoSendAll(sock, "\n", 1))
		SEND_RECV_ERROR

	if(!DoSendInt(sock, 16))
		SEND_RECV_ERROR

	// set clSeed
	{
		static time_t randVals[4] = { -1 };

		if(randVals[0] == -1)
		{
			randVals[0] = (time_t)GetTickCount();
			randVals[1] = time(NULL);
			randVals[2] = (time_t)SendRecv;
			randVals[3] = (time_t)clSeed;
		}
		else
			randVals[0]++;

		LOG("randVals: %I64d, %I64d, %I64d, %I64d\n", randVals[0], randVals[1], randVals[2], randVals[3]);

		autoList<uchar> *randData = sha512_block(randVals, sizeof(randVals));
		memcpy(clSeed, randData->ElementAt(0), 16);
		delete randData;
	}

	if(!DoSendAll(sock, clSeed, 16))
		SEND_RECV_ERROR

	if(DoRecvInt(sock) != 16)
		SEND_RECV_ERROR

	if(!DoRecvAll(sock, svSeed, 16))
		SEND_RECV_ERROR

	PutHash(sendData);
	CRPC_Crypt(sendData, clSeed, svSeed, encCounter);

#if LOG_ENABLED
	{
		LOG("clSeed: ");
		for(int i = 0; i < 16; i++) LOG("%02x", clSeed[i]);
		LOG("\n");

		LOG("svSeed: ");
		for(int i = 0; i < 16; i++) LOG("%02x", svSeed[i]);
		LOG("\n");

		LOG("encCounter: ");
		for(int i = 0; i < 16; i++) LOG("%02x", encCounter[i]);
		LOG("\n");
	}
#endif

	/*
		もう不要なので、早めに破壊
	*/
	memset(&clSeed, 0x00, 16);
	memset(&svSeed, 0x00, 16);

	if(!DoSendInt(sock, sendData->GetCount()))
		SEND_RECV_ERROR

	if(!DoSendAll(sock, sendData->ElementAt(0), sendData->GetCount()))
		SEND_RECV_ERROR

	int recvSize = DoRecvInt(sock);

	if(!m_isRange(recvSize, 0, RECVSIZE_MAX))
		SEND_RECV_ERROR

	void *buff = memAlloc(recvSize);

	if(!DoRecvAll(sock, buff, recvSize))
	{
		memFree(buff);
		SEND_RECV_ERROR
	}
	recvData = new autoList<uchar>((uchar *)buff, recvSize);

	CRPC_Crypt(recvData, NULL, NULL, encCounter);

	if(!UnputHash(recvData))
	{
		delete recvData;
		recvData = NULL;
		SEND_RECV_ERROR
	}
endFunc:
	memset(&clSeed, 0x00, 16);
	memset(&svSeed, 0x00, 16);
	memset(&encCounter, 0x00, 16);
	return recvData;
}

static autoList<uchar> *SData;
static autoList<uchar> *RData;

static char *ServerHost;
static int ServerPort;
static char *ServerIP;

static char *InfoMessage = "NONE";
static char *ErrorMessage = "";

// ---- Comm_Successful, Comm_Error ----

static int Comm_ErrorCount;

static void Comm_Successful(void)
{
	LOGPOS();
	Comm_ErrorCount = 0;
}
static void Comm_Error(void)
{
	LOGPOS();
	Comm_ErrorCount++;

	if(3 <= Comm_ErrorCount)
	{
		LOGPOS();
		Comm_ErrorCount = 0;
		delete SData;
		SData = NULL;
	}
	doThreads(5000);
}

// ----

static void Thread(void)
{
	ServerHost = GetServerHost();
	ServerPort = GetServerPort();

	// ソケット初期化
	{
		WORD ver = MAKEWORD(2, 2);
		WSADATA wsd;
		WSAStartup(ver, &wsd);
	}

	while(KeepThread)
	{
		if(!SData) // ? 送信データ無し -> 次フレームまで待つ
		{
			InfoMessage = "STANDBY_OK";
			ErrorMessage = "";

			doThreads(15);
			continue;
		}

		if(!ServerIP)
		{
			struct hostent *host;

			InfoMessage = "RESOLVE_HOSTNAME";
			ErrorMessage = "";

			uncritical();
			{
				host = gethostbyname(ServerHost);
			}
			critical();

			if(!host || !host->h_addr_list[0])
			{
				InfoMessage = "CANNOT_RESOLVE_HOSTNAME_WAIT";
				ErrorMessage = "CANNOT_RESOLVE_HOSTNAME";

				Comm_Error();
				continue;
			}
			struct in_addr addr;
			addr.s_addr = *(u_long *)host->h_addr_list[0];
			ServerIP = strx(inet_ntoa(addr));
		}

		int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(sock == -1)
		{
			InfoMessage = "SOCKET_ERROR_WAIT";
			ErrorMessage = "SOCKET_ERROR";

			Comm_Error();
			continue;
		}

		// connect
		{
			struct sockaddr_in sa;

			memset(&sa, 0x00, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = inet_addr(ServerIP);
			sa.sin_port = htons((unsigned short)ServerPort);

			if(sa.sin_addr.s_addr == INADDR_NONE)
				goto connectError;

			int retval;

			InfoMessage = "CONNECT";
			ErrorMessage = "";
		
			uncritical();
			{
				retval = connect(sock, (struct sockaddr *)&sa, sizeof(sa));
			}
			critical();

			if(retval == -1)
			{
connectError:
				InfoMessage = "CONNECT_ERROR_WAIT";
				ErrorMessage = "CONNECT_ERROR";

				closesocket(sock);
				Comm_Error();
				continue;
			}
		}

		/*
			通信エラーのとき、
			ここまではリトライする。
			ここからは失敗する。
		*/

		autoList<uchar> *recvData = SendRecv(sock, SData);

		// disconnect
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);
		}

		if(!recvData) // ? 失敗
		{
			InfoMessage = "TRANSMIT_ERROR_WAIT";
			ErrorMessage = "TRANSMIT_ERROR";

			Comm_Error();
			continue;
		}
		delete SData;
		SData = NULL;
		RData = recvData;
		Comm_Successful();
	}

	// ソケット開放
	{
		WSACleanup();
	}
}
static void Init(void)
{
	// once
	{
		static int inited;

		if(inited)
			return;

		inited = 1;
	}
	StartThread(Thread);
}
void SockSend(autoList<uchar> *sendData) // sendData: bind
{
	errorCase(!sendData);

	Init();

	if(SData || RData)
	{
		delete sendData;
		return;
	}
	SData = sendData;
}
int IsSockSending(void) // ? 送受信中 || (送受信完了 && 受信データ未取得)
{
	return SData || RData;
}
autoList<uchar> *SockRecv(void) // ret: NULL == (未送受信 || 送受信失敗)
{
	if(!RData)
		return NULL;

	autoList<uchar> *recvData = RData;
	RData = NULL;
	return recvData;
}
char *GetSockInfoMessage(void) // ret: bind
{
	return InfoMessage;
}
char *GetSockErrorMessage(void) // ret: bind
{
	return ErrorMessage;
}
