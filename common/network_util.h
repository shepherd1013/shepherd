/*
 * network_util.h
 *
 *  Created on: Jun 24, 2015
 *      Author: Jacken
 */

#ifndef NETWORK_UTIL_H_
#define NETWORK_UTIL_H_

#include <list>
#include <string>
#include <vector>
#include <netinet/in.h>

using namespace std;

class NetworkUtil
{
public:
	static bool GetInterfaceMAC(const char* sIfName, unsigned char* sMAC, unsigned int* uMACLen);
	static bool Multicast(const char* sLocalIP, const char* sTargetAddr, unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize);
	static bool IPBinaryToText(int nAddressFamily, const void* stAddr, char* sIP);
	static bool GetInterfaceIPIPv4Text(const char *sIfName, char* sIP, unsigned int uIPLen);
	static bool GetInterfaceIPIPv4Binary(const char *sIfName, struct in_addr* pAddr);
	static bool GetInterfaceNetmaskIPIPv4Binary(const char *sIfName, struct in_addr* pAddr);
	static bool Broadcast(unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize);
	static bool BroadcastSubnet(const char* sIfName, bool bIsBindInterface, unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize);
	static bool IPv4AddressTextToBinary(const char *sIP, struct in_addr *sAddr);
	static bool IPv4AddressBinaryToText(struct in_addr sAddr, string &sIP);
	static bool IPv4AddressBinaryToText(struct in_addr sAddr, char *sIP, unsigned int uIPLen);
	static bool EnumerateAllInterfaceIPv4(list<string> &lList);
	static bool EnumerateUpInterfaceIPv4(list<string> &lList);
	static bool InterfaceIndexToName(int nIfIndex, char *sIfName);
	static unsigned int HostToNetworkByteOrder(unsigned int uHost);
	static unsigned short HostToNetworkByteOrder(unsigned short uHost);
	static unsigned int  NetworkToHostByteOrder (unsigned int uHost);
	static unsigned short  NetworkToHostByteOrder (unsigned short uHost);
	static bool GetFlagsIPv4(const char* sInterfaceName, short int* nIFFlags);
/*
	nIFFlags 的各項旗標和說明：
		IFF_UP              裝置正在運作
		IFF_BROADCAST       有效的廣播位址
		IFF_DEBUG           Debug 標誌
		IFF_LOOPBACK        這是 Loopback 裝置
		IFF_POINTOPOINT     這是點到點的網路裝置介面
		IFF_RUNNING         資源已分配
		IFF_NOARP           無arp協議，沒有設置第二層目的地址
		IFF_PROMISC         介面為雜湊(promiscuous)模式
		IFF_NOTRAILERS      避免使用 trailer
		IFF_ALLMULTI        接收所有群組廣播(multicast)封包資料
		IFF_MASTER          主負載平衡群(bundle)
		IFF_SLAVE           從負載平衡群(bundle)
		IFF_MULTICAST       支持群組廣播(multicast)
		IFF_PORTSEL         可以通過 ifmap 選擇 media 類型
		IFF_AUTOMEDIA       自動選擇 media
		IFF_DYNAMIC         裝置介面關閉時丟棄地址
*/
	static bool IsBridgeEnabled(bool *bIsEnabled, string &sBridgeIfName);
	static bool IsBridgeEnabled(bool *bIsEnabled, char *sBridgeIfName, unsigned int uIfNameSize);
	static bool EnumerateRunningInterfaceIPv4(list<string> &lList);
	static bool IsIPv4FormateValid(const char *sIP);
	static bool IsIPv4ValueValid(int nIP);
	static bool IsSingleInterface(bool *bIsSingleIf);
	static bool GetNetTotalFlow(const char* sIfName, unsigned long int *uRx, unsigned long int *uTx);
	static bool GetNetFlow(const char* sIfName, unsigned int tIntervalMS, double *dRxFlow, double *dTxFlow);
};

#endif /* NETWORK_UTIL_H_ */
