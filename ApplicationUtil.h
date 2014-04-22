#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/aodv-helper.h"
#include "MyTag.h"
#include "ns3/nstime.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
<<<<<<< HEAD
#include "crypto++/base64.h"
#include <iomanip>
using std::hex;
#include <string>
#include "crypto++/aes.h"
#include "crypto++/cryptlib.h"
#include "crypto++/rsa.h" 
#include "crypto++/randpool.h"
=======
#include <string>
#include "crypto++/aes.h" 
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
#include "crypto++/sha.h"
#include "crypto++/modes.h"
#include "crypto++/integer.h"
#include "crypto++/osrng.h"
#include "crypto++/nbtheory.h"
#include <stdexcept>
#include "crypto++/dh.h"
#include "crypto++/secblock.h"
#include <crypto++/hex.h>
#include <crypto++/filters.h>
#include <map>
#include <sstream>
#include <iomanip>
<<<<<<< HEAD
#include <unistd.h>
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::ModularExponentiation;
using CryptoPP::InvertibleRSAFunction;
=======

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::ModularExponentiation;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
using std::runtime_error;
using CryptoPP::DH;
using CryptoPP::SecByteBlock;
using CryptoPP::StringSink;
using CryptoPP::HexEncoder;
using std::map;
using std::pair;
using namespace ns3;
using namespace CryptoPP;

Integer p("0xB10B8F96A080E01DDE92DE5EAE5D54EC52C99FBCFB06A3C6"
		"9A6A9DCA52D23B616073E28675A23D189838EF1E2EE652C0"
		"13ECB4AEA906112324975C3CD49B83BFACCBDD7D90C4BD70"
		"98488E9C219A73724EFFD6FAE5644738FAA31A4FF55BCCC0"
		"A151AF5F0DC8B4BD45BF37DF365C1A65E68CFDA76D4DA708"
		"DF1FB2BC2E4A4371");

Integer g("0xA4D1CBD5C3FD34126765A442EFB99905F8104DD258AC507F"
		"D6406CFF14266D31266FEA1E5C41564B777E690F5504F213"
		"160217B4B01B886A5E91547F9E2749F4D7FBD7D3B9A92EE1"
		"909D0D2263F80A76A6A24C087A091F531DBF0A0169B6A28A"
		"D662A4D18E73AFA32D779D5918D08BC8858F4DCEF97C2A24"
		"855E6EEB22B3B2E5");

Integer q("0xF518AA8781A8DF278ABA4E7D64B7CB9D49462353");

<<<<<<< HEAD

Integer n("0xbeaadb3d839f3b5f"), e("0x11"), d("0x21a5ae37b9959db9");
 

=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
TypeId tid;
Ipv4InterfaceContainer i;
NodeContainer c;
Ptr<Socket> source;
int option;

//int nodeSpeed = 20; //in m/s
//int nodePause = 0; //in s
//double txp = 7.5;

<<<<<<< HEAD
int currentStep = 0;
=======

>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
int rounds = 0;
int MessageLength = 0;
double waitTime = 0;
std::stringstream sharedMessage;
int sender = 0;
<<<<<<< HEAD
std::string Message = "0110000101100010001100110011010100111001011000010110000100110111001101100110000100110110001101110011011100110011011001010110010000110111011000010011100100110011011000100011001000110001001101000110010001100010001100000110001100110010001101010110010000110000001100010011011000110000001110000011000100110111011000100011100001100001001110000011100100110011011000110011000000110000001100010110001100110111001101100011000101100101001100010011100100111000011000010011001100110110001110010011010000110101001100000011100101100101011000100110010100111000000011010000101000110111011000010011010100110011001100010011001101100101001100000011001100110100001110010110010000111001001101010011000000111000001100110110010100110101001101000011000100110010011000110011100101100110011000110011100000110001001101010110001001100110011001000011011000110001011001100011100100110101011001000110010001100101011000110110010100110100001100110011001100110111001101100011010100110101001100000110011001100100011000110011011000110010001101000110010100111001001100100110011001100110001100110011100001100001001101000011000100110101001101110011100000110011011000100011100100110111001100100011011000110001001100100011000000001101000010100011010001100101001100000011010101100100001101100011010100110111001100110011000101100010011000100110000100110001011000110110001101100110011001100011000001100101001110000011010001100011001110000110001101100100001100100011000000111001001101110110001000110111001101010110011001100101011000110110000100110001001100000011001000111001001100100011011000110001011000010110010100110001001110010110000100110011001110000011100101100001001100100110010100110001001101010110010000110010001110010011001100111001001100110011000100110100011000100011000100111000001101000110000101100101011001100011011100110000001101110110001000111000001100100000110100001010011001010110001000111001001101000011010000110001001100100011000000110110001101010011000100111000001100010110010000110010001100110110010100110000001101000110001001100110001100000011011000110101011001100011010001100001011000110011010000110001001100110110011001101000";
//std::string Message = "101100";
=======
//std::string Message = "0110000101100010001100110011010100111001011000010110000100110111001101100110000100110110001101110011011100110011011001010110010000110111011000010011100100110011011000100011001000110001001101000110010001100010001100000110001100110010001101010110010000110000001100010011011000110000001110000011000100110111011000100011100001100001001110000011100100110011011000110011000000110000001100010110001100110111001101100011000101100101001100010011100100111000011000010011001100110110001110010011010000110101001100000011100101100101011000100110010100111000000011010000101000110111011000010011010100110011001100010011001101100101001100000011001100110100001110010110010000111001001101010011000000111000001100110110010100110101001101000011000100110010011000110011100101100110011000110011100000110001001101010110001001100110011001000011011000110001011001100011100100110101011001000110010001100101011000110110010100110100001100110011001100110111001101100011010100110101001100000110011001100100011000110011011000110010001101000110010100111001001100100110011001100110001100110011100001100001001101000011000100110101001101110011100000110011011000100011100100110111001100100011011000110001001100100011000000001101000010100011010001100101001100000011010101100100001101100011010100110111001100110011000101100010011000100110000100110001011000110110001101100110011001100011000001100101001110000011010001100011001110000110001101100100001100100011000000111001001101110110001000110111001101010110011001100101011000110110000100110001001100000011001000111001001100100011011000110001011000010110010100110001001110010110000100110011001110000011100101100001001100100110010100110001001101010110010000110010001110010011001100111001001100110011000100110100011000100011000100111000001101000110000101100101011001100011011100110000001101110110001000111000001100100000110100001010011001010110001000111001001101000011010000110001001100100011000000110110001101010011000100111000001100010110010000110010001100110110010100110000001101000110001001100110001100000011011000110101011001100011010001100001011000110011010000110001001100110110011001101000";
std::string Message = "101";
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
std::string phyMode ("OfdmRate54Mbps");
double distance = 1;  // m
uint32_t packetSize = 1024; // bytes
uint32_t numPackets = 60;
int numNodes = 3;  // by default, 5x5
uint32_t sinkNode = 0;
uint32_t sourceNode = 2;
double interval = 1.0; // seconds
double keyExchangeInterval = 5.0; // seconds
bool verbose = false;
bool tracing = true;
int messageLen=0;	
<<<<<<< HEAD
int sentpublickeys=0;
bool ranempty=false;
SecByteBlock AESkey(0x00, AES::DEFAULT_KEYLENGTH);
byte AESiv[AES::BLOCKSIZE];
AutoSeededRandomPool AESrnd;
=======

>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
int aesKeyLength = SHA256::DIGESTSIZE;
AutoSeededRandomPool rnd;
byte iv[AES::BLOCKSIZE];	
SecByteBlock key(SHA256::DIGESTSIZE);
static std::string msgs[20];
<<<<<<< HEAD
InvertibleRSAFunction params;
//measurement variables
int maxnodes=10;
=======

//measurement variables

>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
int stage1SentPacketCount = 0;
int stage2SentPacketCount = 0;
int stage1RecvPacketCount = 0;
int stage2RecvPacketCount = 0;
int AnnouncementPacketCount = 0;
double stage1Latency;
double stage2Latency;
std::vector<Time> stage1StartTime;
std::vector<Time> stage2StartTime;
std::vector<Time> stage1EndTime;
std::vector<Time> stage2EndTime;
Time totalTimeStart, totalTimeEnd;
double totalRunningTime;
double totalLatency;

<<<<<<< HEAD
int publicKeyCounter = 0;
int randomBitCounter = 0;

static std::string hexStr(byte *data, int len)
{
    std::stringstream ss;
    ss<<std::hex;
    for(int i(0); i<len; ++i)
        ss<<(int)data[i];
    return ss.str();
}

=======


int publicKeyCounter = 0;
int randomBitCounter = 0;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

class ApplicationUtil
{	
	private:
	static bool instanceFlag;
	int dhAgreedLength;
	static ApplicationUtil *appUtil;
	ApplicationUtil()
	{
	 //private constructor
	}

	map<int,SecByteBlock> publicKeyMap;
	map<int,SecByteBlock> privateKeyMap;
	map<int,SecByteBlock> dhSecretKeyMapSub;
	map<int,map<int,SecByteBlock> > dhSecretKeyMapGlobal;
<<<<<<< HEAD
	map<int,std::string> dhSecretBitMapSub;
	map<int,map<int,std::string> > dhSecretBitMapGlobal;
	map<Ptr<Node>,int> nodeMap;
	map<int,std::string> announcement;
	map<int, std::string> receivedAnnouncementSubMap;
	map<int, map<int, std::string> > receivedAnnouncement;

	

	map<int, std::vector<SecByteBlock> > ReceivedAESKeyMap;
	//map<int, map <std::string, SecByteBlock> > ReceivedAESKeyForMsgIdMap;
	map<int, map <std::string, SecByteBlock> > SentAESKeyForMsgIdMap;	

	map<std::string, std::string> msgIdPublicKeyPairSubMap;
	map<int, map<std::string, std::string> > msgIdPublicKeyPairMap;

public:

	void writeOutputToFile(char* fileName, int option, int numNodes,int length, double latency, double totalTime );
=======
	map<int,int> dhSecretBitMapSub;
	map<int,map<int,int> > dhSecretBitMapGlobal;
	map<Ptr<Node>,int> nodeMap;
	map<int,int> announcement;
	map<int, int> receivedAnnouncementSubMap;
	map<int, map<int, int> > receivedAnnouncement;
public:

	void writeOutputToFile(char* fileName, int option, int numNodes,int length, double latency, double totalTime );
	//static int publicKeyPairCount;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
	int getDhAgreedLength()
	{
		return dhAgreedLength;
	}	
	void setDhAgreedLength(int len)
	{
		dhAgreedLength = len;
	}
	SecByteBlock getPublicKeyFromMap(int nodeId);
	void putPublicKeyInMap(int nodeId, SecByteBlock key);
	SecByteBlock getPrivateKeyFromMap(int nodeId);
	void putPrivateKeyInMap(int nodeId, SecByteBlock key);
	SecByteBlock getSecretKeyFromGlobalMap(int nodeId,int destNodeId);
	void putSecretKeyInGlobalMap(int nodeId, int destNodeId, SecByteBlock key);

<<<<<<< HEAD
	std::string getSecretBitFromGlobalMap(int nodeId,int destNodeId);
	void putSecretBitInGlobalMap(int nodeId, int destNodeId, std::string value);
	void eraseSecretBitMapGlobal();
	map<int,std::string> getSecretBitSubMap(int nodeId);

	void putNodeInMap(Ptr<Node> node,int index);
	int getNodeFromMap(Ptr<Node> node);
	void putAnnouncementInGlobalMap(int nodeId,std::string value);
	std::string getAnnouncement(int nodeId);
	void putAnnouncementInReceivedMap(int nodeId, int senderNode, std::string value);
	map<int, std::string> getAnnouncementSubMap(int nodeId);
	std::string getReceivedAnnouncement(int nodeId, int senderNodeId);

	static ApplicationUtil* getInstance();	

=======
	int getSecretBitFromGlobalMap(int nodeId,int destNodeId);
	void putSecretBitInGlobalMap(int nodeId, int destNodeId, int value);
	void eraseSecretBitMapGlobal();
	map<int,int> getSecretBitSubMap(int nodeId);

	void putNodeInMap(Ptr<Node> node,int index);
	int getNodeFromMap(Ptr<Node> node);
void putAnnouncementInGlobalMap(int nodeId,int value);
int getAnnouncement(int nodeId);
void putAnnouncementInReceivedMap(int nodeId, int senderNode, int value);
map<int, int> getAnnouncementSubMap(int nodeId);
int getReceivedAnnouncement(int nodeId, int senderNodeId);

	static ApplicationUtil* getInstance();	
	
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
        ~ApplicationUtil()
        {
	  instanceFlag = false;
        }
<<<<<<< HEAD

	

	

=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
};
bool ApplicationUtil::instanceFlag = false;
ApplicationUtil* ApplicationUtil::appUtil = NULL;

ApplicationUtil* ApplicationUtil::getInstance()
{
	if(!instanceFlag)
        {		
<<<<<<< HEAD
=======
		//publicKeyPairCount = 0;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		appUtil = new ApplicationUtil();
		instanceFlag = true;
	}
        return appUtil;
    
}		
void ApplicationUtil::putNodeInMap(Ptr<Node> node,int index)
{
	nodeMap.insert(pair<Ptr<Node>,int>(node,index));
}

int ApplicationUtil::getNodeFromMap(Ptr<Node> node)
{
	map<Ptr<Node>,int>::iterator p;
	p = nodeMap.find(node);
	if(p != nodeMap.end())
		return p->second;
	else 
		return -1;	
}
SecByteBlock ApplicationUtil::getPublicKeyFromMap(int nodeId)
{
	map<int,SecByteBlock>::iterator p;
	p = publicKeyMap.find(nodeId);
	if(p != publicKeyMap.end())
		return p->second;
	else 
		return SecByteBlock(0);
}

void ApplicationUtil::putPublicKeyInMap(int nodeId, SecByteBlock key)
{
	publicKeyMap.insert(pair<int,SecByteBlock>(nodeId,key));
}

SecByteBlock ApplicationUtil::getPrivateKeyFromMap(int nodeId)
{
	map<int,SecByteBlock>::iterator p;
	p = privateKeyMap.find(nodeId);
	if(p != privateKeyMap.end())
		return p->second;
	else 
		return SecByteBlock(0);
}

void ApplicationUtil::putPrivateKeyInMap(int nodeId, SecByteBlock key)
{
	privateKeyMap.insert(pair<int,SecByteBlock>(nodeId,key));
}	

SecByteBlock ApplicationUtil::getSecretKeyFromGlobalMap(int nodeId, int destNodeId)
{

	map<int,map<int,SecByteBlock> >::iterator p;
	p = dhSecretKeyMapGlobal.find(nodeId);

	if(p != dhSecretKeyMapGlobal.end())
	{
		map<int,SecByteBlock>::iterator p1;
		p1 = p->second.find(destNodeId);
		if(p1 != dhSecretKeyMapSub.end())
			return p1->second;
		else 
		{
<<<<<<< HEAD
			//std::cout<<"hello + secretKey Global Map " << nodeId << " "<< destNodeId<<"\n";
=======
			std::cout<<"hello";
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
			return SecByteBlock(0);
		}
	}
	else 
		{
<<<<<<< HEAD
			//std::cout<<"hello1 + secretKey Global Map "<< nodeId << " "<< destNodeId<<"\n";
=======
			std::cout<<"hello1";
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
			return SecByteBlock(0);
		}	
}

void ApplicationUtil::putSecretKeyInGlobalMap(int nodeId, int destNodeId, SecByteBlock key)
{

	map<int,map<int,SecByteBlock> >::iterator p;
	p = dhSecretKeyMapGlobal.find(nodeId);
	if(p != dhSecretKeyMapGlobal.end())
	{
<<<<<<< HEAD
		//std::cout<<"Inside put secret key global map : if part "<< nodeId << " "<< destNodeId<<"\n";

		map<int,SecByteBlock>::iterator p1;
		p1 = p->second.find(destNodeId);	
		if(p1 != p->second.end())
			p->second[destNodeId] = key;
		else
			p->second.insert(pair<int,SecByteBlock>(destNodeId,key));

	}
	else
	{
				//std::cout<<"Inside put secret key global map : esle part\n";	
=======
		p->second.insert(pair<int,SecByteBlock>(destNodeId,key));
		
	}
	else
	{	
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		map<int,SecByteBlock> tempMap;	
		tempMap.insert(pair<int,SecByteBlock>(destNodeId,key));
		dhSecretKeyMapGlobal.insert(pair<int,map<int,SecByteBlock> >(nodeId,tempMap));
	}	
<<<<<<< HEAD

}	

std::string ApplicationUtil::getSecretBitFromGlobalMap(int nodeId, int destNodeId)
{

	map<int,map<int,std::string> >::iterator p;
=======
	
}	

int ApplicationUtil::getSecretBitFromGlobalMap(int nodeId, int destNodeId)
{

	map<int,map<int,int> >::iterator p;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
	p = dhSecretBitMapGlobal.find(nodeId);

	if(p != dhSecretBitMapGlobal.end())
	{
<<<<<<< HEAD
		map<int,std::string>::iterator p1;
=======
		map<int,int>::iterator p1;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		p1 = p->second.find(destNodeId);
		if(p1 != dhSecretBitMapSub.end())
			return p1->second;
		else 
		{
<<<<<<< HEAD
			//std::cout<<"hello + secretbit Global Map\n";
			return "";
=======
			std::cout<<"hello";
			return -99;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		}
	}
	else 
		{
<<<<<<< HEAD
			//std::cout<<"hello1 + secretbit Global Map\n";
			return "";
		}	
}

void ApplicationUtil::putSecretBitInGlobalMap(int nodeId, int destNodeId, std::string value)
{

	//std::cout << "prgstring inserted is"  << value << "\n";
	map<int,map<int,std::string> >::iterator p;
	p = dhSecretBitMapGlobal.find(nodeId);
	if(p != dhSecretBitMapGlobal.end())
	{
		map<int,std::string>::iterator p1;
=======
			std::cout<<"hello1";
			return -99;
		}	
}

void ApplicationUtil::putSecretBitInGlobalMap(int nodeId, int destNodeId, int value)
{

	map<int,map<int,int> >::iterator p;
	p = dhSecretBitMapGlobal.find(nodeId);
	if(p != dhSecretBitMapGlobal.end())
	{
		map<int,int>::iterator p1;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		p1 = p->second.find(destNodeId);	
		if(p1 != p->second.end())
			p->second[destNodeId] = value;
		else
<<<<<<< HEAD
			p->second.insert(pair<int,std::string>(destNodeId,value));		
	}
	else
	{	
		map<int,std::string> tempMap;	
		tempMap.insert(pair<int,std::string>(destNodeId,value));
		dhSecretBitMapGlobal.insert(pair<int,map<int,std::string> >(nodeId,tempMap));
	}		
}					

map<int,std::string> ApplicationUtil::getSecretBitSubMap(int nodeId)
{
	map<int,map<int,std::string> >::iterator p;
	p = dhSecretBitMapGlobal.find(nodeId);
    return p->second;
=======
			p->second.insert(pair<int,int>(destNodeId,value));		
	}
	else
	{	
		map<int,int> tempMap;	
		tempMap.insert(pair<int,int>(destNodeId,value));
		dhSecretBitMapGlobal.insert(pair<int,map<int,int> >(nodeId,tempMap));
	}	
	
}					

map<int,int> ApplicationUtil::getSecretBitSubMap(int nodeId)
{
	map<int,map<int,int> >::iterator p;
	p = dhSecretBitMapGlobal.find(nodeId);
		
	return p->second;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
}

void ApplicationUtil::eraseSecretBitMapGlobal()
{
<<<<<<< HEAD
	 map<int,map<int,std::string> >::iterator p;
	 dhSecretBitMapGlobal.erase ( p, dhSecretBitMapGlobal.end() );
}

void ApplicationUtil::putAnnouncementInGlobalMap(int nodeId, std::string value)
{
	map<int,std::string>::iterator p;
=======
	 map<int,map<int,int> >::iterator p;
	 dhSecretBitMapGlobal.erase ( p, dhSecretBitMapGlobal.end() );
}

//swati
void ApplicationUtil::putAnnouncementInGlobalMap(int nodeId, int value)
{

	//std::cout<<"Node "<<nodeId<<" stores "<<value<<"\n";

	map<int,int>::iterator p;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
	p = announcement.find(nodeId);
	if(p != announcement.end())
		announcement[nodeId] = value;
	else
<<<<<<< HEAD
	announcement.insert(pair<int,std::string>(nodeId,value));
}					

std::string ApplicationUtil::getAnnouncement(int nodeId)
{
	map<int,std::string>::iterator p;
	p = announcement.find(nodeId);
	return p->second;
}
void ApplicationUtil::putAnnouncementInReceivedMap(int nodeId, int senderNode, std::string value)
{
	map<int,map<int,std::string> >::iterator p;
	p = receivedAnnouncement.find(nodeId);
	if(p != receivedAnnouncement.end())
	{
		map<int,std::string>::iterator p1;
=======
	announcement.insert(pair<int,int>(nodeId,value));
//	std::cout<<"Finds "<<ApplicationUtil::getAnnouncement(nodeId)<<"\n";
}					

int ApplicationUtil::getAnnouncement(int nodeId)
{
	map<int,int>::iterator p;
	p = announcement.find(nodeId);
	//std::cout<<"Node "<<nodeId<<" stores "<<p->second<<"\n";
	return p->second;
}
void ApplicationUtil::putAnnouncementInReceivedMap(int nodeId, int senderNode, int value)
{
	map<int,map<int,int> >::iterator p;
	p = receivedAnnouncement.find(nodeId);
	if(p != receivedAnnouncement.end())
	{
		map<int,int>::iterator p1;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		p1 = p->second.find(senderNode);	
		if(p1 != p->second.end())
			p->second[senderNode] = value;
		else
<<<<<<< HEAD
		p->second.insert(pair<int,std::string>(senderNode,value));

	//	std::cout<<"Inserting "<<nodeId<<","<<senderNode<<","<<value<<"\n";

	}
	else
	{	
		map<int,std::string> tempMap;	
		tempMap.insert(pair<int,std::string>(senderNode,value));
		receivedAnnouncement.insert(pair<int,map<int,std::string> >(nodeId,tempMap));
	//	std::cout<<"Inserting "<<nodeId<<","<<senderNode<<","<<value<<"\n";
	}
}
std::string ApplicationUtil::getReceivedAnnouncement(int nodeId, int senderNodeId)
{
	map<int,map<int,std::string> >::iterator p;
=======
		p->second.insert(pair<int,int>(senderNode,value));

	//	std::cout<<"Inserting "<<nodeId<<","<<senderNode<<","<<value<<"\n";
		
	}
	else
	{	
		map<int,int> tempMap;	
		tempMap.insert(pair<int,int>(senderNode,value));
		receivedAnnouncement.insert(pair<int,map<int,int> >(nodeId,tempMap));
	//	std::cout<<"Inserting "<<nodeId<<","<<senderNode<<","<<value<<"\n";
	}
}
int ApplicationUtil::getReceivedAnnouncement(int nodeId, int senderNodeId)
{
	map<int,map<int,int> >::iterator p;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
	p = receivedAnnouncement.find(nodeId);

	if(p != receivedAnnouncement.end())
	{
<<<<<<< HEAD
		map<int,std::string>::iterator p1;
=======
		map<int,int>::iterator p1;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		p1 = p->second.find(senderNodeId);
		if(p1 != receivedAnnouncementSubMap.end())
			return p1->second;
		else 
		{
<<<<<<< HEAD
			//std::cout<<"hello + received announcement\n";
			return "";
=======
			std::cout<<"hello";
			return -99;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		}
	}
	else 
		{
<<<<<<< HEAD
			//std::cout<<"hello1 + received announcement\n";
			return "";
=======
			std::cout<<"hello1";
			return -99;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
		}	
}


<<<<<<< HEAD
map<int,std::string> ApplicationUtil::getAnnouncementSubMap(int nodeId)
{
	map<int,map<int,std::string> >::iterator p;
	p = receivedAnnouncement.find(nodeId);

	return p->second;
}
=======
map<int,int> ApplicationUtil::getAnnouncementSubMap(int nodeId)
{
	map<int,map<int,int> >::iterator p;
	p = receivedAnnouncement.find(nodeId);
	
	return p->second;
}


//write to csv file

>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
void ApplicationUtil::writeOutputToFile(char* fileName, int option, int numNodes,int length, double latency, double totalTime ) {
   std::ofstream fout;

   fout.open(fileName, std::ios_base::app);

    if(option == 1)
    {
	fout << numNodes << ',' << latency << ','<<totalTime;
    }
    else if(option == 2)
    {
	fout << length << ',' << latency << ','<<totalTime;
    }
<<<<<<< HEAD

=======
	    
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
    fout << "\n";
    fout.close();
}


<<<<<<< HEAD


=======
/*

void readInput(char* fileName) {
    printf("****************ReadInput***************\n");
 
    // read file
    ifstream fin(fileName);
    string tmp = ""; // a line in the input file
    while(getline(fin, tmp)) {
        // add string into vector
        listString.push_back(tmp);
        cout << listString[listString.size() - 1] << endl;
    }
    printf("\n\n");
    fin.close();
}
 

void parseValues() {
    printf("****************ParseValue***************\n");
    for (int i = 0; i < listString.size(); ++i) {
        char tmp[100000];
		
        strcpy(tmp, listString[i].c_str()); // copy string to char array
        stringArray tmpArray;
        // utilize string token to extract data
        char * pch;
        pch = strtok (tmp,",");
        while (pch != NULL) {
            tmpArray.push_back(pch);
           printf ("%s\t",pch); 
            // get the next token
            pch = strtok (NULL, ",");
			
        }
        data.push_back(tmpArray);
		
        printf("\n");
    }
}

*/
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
