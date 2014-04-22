#include "ApplicationUtil.h"

NS_LOG_COMPONENT_DEFINE ("WifiSimpleAdhocGrid");

static void DisplayMessage(Ptr<Socket> socket);

static void DCNET(Ptr<Socket> socket, bool done_dcnet);


static void SendMessage (Ptr<Socket> socket, std::string message, int index, int dest)
{
    Ptr<Packet> sendPacket =Create<Packet> ((uint8_t*)message.c_str(),message.size());
    MyTag sendTag;
    sendTag.SetSimpleValue(index);
    sendPacket->AddPacketTag(sendTag);
    socket->Send (sendPacket);
    stage2SentPacketCount += 1;//increment sent packet counter for stage2
    socket->Close ();
}

std::vector<char> convert_to_binary(const char* input) 
{
	int ascii;           
	int length = strlen(input);        
   	std::vector<char> ascii_binary; 
   	for(int x = 0; x < length; x++) {
		ascii = input[x];        		
		char* binary_reverse = new char [9];     
		int y = 0;   
		while(ascii != 1) {   
			if(ascii % 2 == 0) binary_reverse[y] = '0';  
            else if(ascii % 2 == 1) binary_reverse[y] = '1';   
			ascii /= 2;   
			y++;   
		}
		if(ascii == 1) { 
			binary_reverse[y] = '1';
            y++;
		}
		if(y < 8) { 
			for(; y < 8; y++) {
				binary_reverse[y] = '0';
			}
		}
	    for(int z = 0; z < 8; z++) {
			ascii_binary.push_back(binary_reverse[7 - z] );
		}
        delete [] binary_reverse;    
	}
    return ascii_binary ;
}
int randomBitGeneratorWithProb(double p)
{
    double rndDouble = (double)rand() / RAND_MAX;
    return rndDouble > p;
}

std::string PseudoRandomGenerator(unsigned int SEEDSIZE)
{
	std::string result;
    std::ostringstream convert;
   	for(unsigned int i=0; i<SEEDSIZE; i++) {
   		int randomBit = randomBitGeneratorWithProb(0.5);
       	convert << randomBit;
    }
	result = convert.str();
	//std::cout << "random string is" << result << "\n";
    return result;
	
}
// recieve prgstring
static void ReceiveMessage (Ptr<Socket> socket)
{
	
    Ptr<Packet> recPacket = socket->Recv();
    stage2RecvPacketCount += 1;//increment recv packet counter for stage2
    ApplicationUtil *appUtil = ApplicationUtil::getInstance();

   // Ptr<Node> recvnode = socket->GetNode();
   // int recNodeIndex = ApplicationUtil::getInstance()->getNodeFromMap(recvnode);

    uint8_t *buffer = new uint8_t[recPacket->GetSize()];
   recPacket->CopyData(buffer,recPacket->GetSize());

    std::string recMessage = std::string((char*)buffer);
  //std::cout<<"value random bit received for recNodeIndex "<<recNodeIndex<<" "<<recPacket->GetSize()<<"\n";
    //recMessage = recMessage.substr (0,messageLen-1);
    MyTag recTag;
    recPacket->PeekPacketTag(recTag);
	//recTag.Print(std::cout);
    int srcNodeIndex =int(recTag.GetSourceAddress());
	int recNodeIndex = int(recTag.GetDestAddress());
    //std::cout<<"src node index : "<< srcNodeIndex << " received node index : "<< recNodeIndex<<"\n";
    SecByteBlock key(SHA256::DIGESTSIZE);
    SHA256().CalculateDigest(key, appUtil->getSecretKeyFromGlobalMap(srcNodeIndex,recNodeIndex), 
							appUtil->getSecretKeyFromGlobalMap(srcNodeIndex,recNodeIndex).size());

    //Decryption using the Shared secret key
    std::string decrypted, dec;
	CFB_Mode<AES>::Decryption cfbDecryption;
	cfbDecryption.SetKeyWithIV( key, AESkey.size(), AESiv );
	StringSource ss( recMessage, true, new StreamTransformationFilter( cfbDecryption,new StringSink( dec ))); 
    appUtil->putSecretBitInGlobalMap(srcNodeIndex,recNodeIndex,dec);
    appUtil->putSecretBitInGlobalMap(recNodeIndex,srcNodeIndex,dec);
	
    randomBitCounter--;
    //std::cout << "RandomBitCounter is" <<randomBitCounter <<"\n";
    if(randomBitCounter == 0) {
    	stage1EndTime.push_back(Simulator::Now());
		stage2StartTime.push_back(Simulator::Now());
        Simulator::ScheduleNow (&DisplayMessage,source);
    }

}
static void empty2(int i)
{
    int counter=0;
	while(publicKeyCounter > 0) {
     	
        if(counter==3) break;
        else {
			std::cout <<"current counter is "<< counter << "\n";
			usleep(100000);
			++counter;
		}
    }
	//ranempty=true;
}

// send prgstring
static void SimulatorLoop(Ptr<Socket> socket,TypeId tid, NodeContainer c, Ipv4InterfaceContainer i)
{
	publicKeyCounter = (numNodes * numNodes) - numNodes;
    ApplicationUtil *appUtil = ApplicationUtil::getInstance();
    // Generate a random IV
    rnd.GenerateBlock(iv, AES::BLOCKSIZE);

    //sharing the random bit using dh secret key
    for (int index1 = 0; index1 < (int)numNodes; index1++)
    {
		//std::cout << "index1loop and index1 is" <<index1 << "\n";
        for (int index2 = 0; index2 < (int)numNodes; index2++)
        {
			//std::cout << "index2loop and index2 is" <<index2 << "\n";
            if(index1 < index2)
            {
              //  std::cout << "index2loop\n";
                //int randomBit = randomBitGeneratorWithProb(0.5);
				std::string prgString = PseudoRandomGenerator(MessageLength);

              
				
             	//   std::cout<<"Random bit : "<<randomBit<<" "<<index1<<" "<<index2<<"\n";
              //  std::cout << "prgstring from random function is" << prgString <<"\n";
                
				//put random bit in both the maps - src and dest maps

                appUtil->putSecretBitInGlobalMap(index1,index2,prgString);
                appUtil->putSecretBitInGlobalMap(index2,index1,prgString);

                // Calculate a SHA-256 hash over the Diffie-Hellman session key
                //SecByteBlock key(SHA256::DIGESTSIZE);
                SHA256().CalculateDigest( key, appUtil->getSecretKeyFromGlobalMap(index1,index2), 
										  appUtil->getSecretKeyFromGlobalMap(index1,index2).size());

               	// std::ostringstream ss;
                //ss << randomBit;
                std::string message = prgString;
               	messageLen = (int)strlen(message.c_str()) + 1;
                //  messageLen = (int)strlen(message.c_str());
                // Encrypt

                CFB_Mode<AES>::Encryption cfbEncryption;
			    cfbEncryption.SetKeyWithIV( key, AESkey.size(), AESiv );
				std::string encrypted, enc;
				StringSource( prgString, true, new StreamTransformationFilter( cfbEncryption,
		    				   new StringSink( encrypted )
							  ) // StreamTransformationFilter      
		        );

		       

                 //test decryption
                  
                // end

                //Send the encrypted message
                Ptr<Socket> recvNodeSink = Socket::CreateSocket (c.Get (index2), tid);
                InetSocketAddress localSocket = InetSocketAddress (Ipv4Address::GetAny (), 9801);
                recvNodeSink->Bind (localSocket);
                recvNodeSink->SetRecvCallback (MakeCallback (&ReceiveMessage));

                InetSocketAddress remoteSocket = InetSocketAddress (i.GetAddress (index2, 0), 9801);
                Ptr<Socket> sourceNodeSocket = Socket::CreateSocket (c.Get (index1), tid);
                sourceNodeSocket->Connect (remoteSocket);
                //waitTime += 20.0;
                Simulator::ScheduleNow(&SendMessage, sourceNodeSocket,encrypted,index1,index2);
            }
        }
    }
	Simulator::Schedule(Seconds(10.0),&empty2,1);
}

static void SendPublicKey (Ptr<Socket> socket, SecByteBlock pub, int index)
{
	static int i=0;
	std::cout << "counter in public key is" << i <<"\n";
    Ptr<Packet> sendPacket = Create<Packet> ((uint8_t*)pub.BytePtr(),(uint8_t) pub.SizeInBytes());
	//std::cout<<"Debug : Inside dcnet send public key \n";
    MyTag sendTag;
    sendTag.SetSimpleValue(index);
    sendPacket->AddPacketTag(sendTag);
    socket->Send(sendPacket);
	stage1SentPacketCount += 1;//increment sent packet counter for stage1
    std::string sendData = hexStr(pub.BytePtr(),pub.SizeInBytes());
    socket->Close();
	i++;
	++sentpublickeys;
}

static void ReceivePublicKey (Ptr<Socket> socket)
{
    
	//std::cout<<"Debug : Inside dcnet receive public key \n";
    Ptr<Node> recvnode = socket->GetNode();
    int recNodeIndex = ApplicationUtil::getInstance()->getNodeFromMap(recvnode);
	Ptr<Packet> recPacket = socket->Recv();
    stage1RecvPacketCount +=1; //increment received packet count for stage 1
	//	std::cout<<"Node receiving: "<<recNodeIndex<<"\n";
    uint8_t *buffer = new uint8_t[recPacket->GetSize()];
    recPacket->CopyData(buffer,recPacket->GetSize());
	SecByteBlock pubKey((byte *)buffer,recPacket->GetSize());
	MyTag recTag;
    recPacket->PeekPacketTag(recTag);
    int tagVal =int(recTag.GetSimpleValue());
    std::ostringstream s;
    s<<tagVal;
    std::string ss(s.str());
    int srcNodeIndex = atoi(ss.c_str());
    std::string recvData = hexStr(pubKey.BytePtr(),pubKey.SizeInBytes());
	//	std::cout<<"Node : "<<recNodeIndex<<"  from Node TagID: "<<ss<<"\n";
	DH dh;
    dh.AccessGroupParameters().Initialize(p, q, g);
    SecByteBlock sharedKey(ApplicationUtil::getInstance()->getDhAgreedLength());
	dh.Agree(sharedKey, ApplicationUtil::getInstance()->getPrivateKeyFromMap(recNodeIndex),pubKey);
	ApplicationUtil::getInstance()->putSecretKeyInGlobalMap(recNodeIndex,srcNodeIndex,sharedKey);
	publicKeyCounter--;
	std::cout<<"Public key counter :"<< publicKeyCounter<< "\n";
    if(publicKeyCounter == 0) {
    	//std::cout<<"Debug : calling simulator loop \n";
        Simulator::ScheduleNow (&SimulatorLoop, socket,tid,c,i);
    }
    else {
	   
		//if(sentpublickeys == ((numNodes * numNodes) - numNodes ))
			//Simulator::Schedule(Seconds(10.0),&empty2,1);
		//std::cout << "did not recieve all public keys and currently recieved" << publicKeyCounter << "\n";
	}
}

void generateKeys(int index, ApplicationUtil *appUtil)
{
    try {
        DH dh;
        AutoSeededRandomPool rnd;

        dh.AccessGroupParameters().Initialize(p, q, g);

        if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
            throw runtime_error("Failed to validate prime and generator");


        p = dh.GetGroupParameters().GetModulus();
        q = dh.GetGroupParameters().GetSubgroupOrder();
        g = dh.GetGroupParameters().GetGenerator();

        Integer v = ModularExponentiation(g, q, p);
        if(v != Integer::One())
            throw runtime_error("Failed to verify order of the subgroup");

        //////////////////////////////////////////////////////////////

        SecByteBlock priv(dh.PrivateKeyLength());
        SecByteBlock pub(dh.PublicKeyLength());
        dh.GenerateKeyPair(rnd, priv, pub);

        //////////////////////////////////////////////////////////////

        appUtil->putPrivateKeyInMap(index,priv);
        appUtil->putPublicKeyInMap(index,pub);
        appUtil->setDhAgreedLength(dh.AgreedValueLength());

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << "Crypto error : "<< e.what() << std::endl;
    }

    catch(const std::exception& e)
    {
        std::cerr << "Standard error : "<<e.what() << std::endl;
    }
}

//sending and receiving announcements
static void SendAnnouncement (Ptr<Socket> socket, std::string result, int index)
{	
	std::string message = result;
	Ptr<Packet> sendPacket =
	Create<Packet> ((uint8_t*)message.c_str(),message.size());

	MyTag sendTag;
	sendTag.SetSimpleValue(index);
	sendPacket->AddPacketTag(sendTag);

	socket->Send(sendPacket);
	socket->Close();
}

// final v
void ReceiveAnnouncement (Ptr<Socket> socket)
{
	AnnouncementPacketCount-=1;
	Ptr<Packet> recPacket = socket->Recv();	
    ApplicationUtil *appUtil = ApplicationUtil::getInstance();
	Ptr<Node> recvnode = socket->GetNode();
	int recNodeIndex = ApplicationUtil::getInstance()->getNodeFromMap(recvnode);
    uint8_t *buffer = new uint8_t[recPacket->GetSize()];
	recPacket->CopyData(buffer,recPacket->GetSize());
    std::string recMessage = std::string((char*)buffer);
	recMessage = recMessage.substr (0,messageLen-1);
    MyTag recTag;
	recPacket->PeekPacketTag(recTag);
	int srcNodeIndex =int(recTag.GetSimpleValue());
	appUtil->putAnnouncementInReceivedMap(recNodeIndex, srcNodeIndex, recMessage.c_str());
	if(AnnouncementPacketCount==0)
	{
		for(int round = 0 ; round < MessageLength ; round++)
		{
			int x=0;
			//xoring outputs
            for(int index=0;index<(int)numNodes;index++)
			{
				std::string announcement = appUtil->getAnnouncement(index);
				 x ^= announcement.at(round)-48	;					 		
			}
            sharedMessage<<x;					
		}
		// these values will change
		AnnouncementPacketCount = (numNodes * numNodes) - numNodes;
		publicKeyCounter = (numNodes * numNodes) - numNodes;
		randomBitCounter = (numNodes * (numNodes-1)/2);
		stage2EndTime.push_back(Simulator::Now());
		Simulator::ScheduleNow (&DCNET, source,true);
	}
}
//every node has a prgstring..
//calculate the xor message and announce it to every body

void DisplayMessage(Ptr<Socket> socket)
{
    std::cout << "In DisplayMessage\n";
    ApplicationUtil *appUtil = ApplicationUtil::getInstance();
    std::ostringstream ss;
	for(int index = 0; index < (int)numNodes ; index++) {
		for(int round = 0; round < MessageLength ; round++) {
	 		int bit = Message.at(round)-48 ;
			int result = 0;
			std::string prgsecretstring;
			map<int,std::string> NodeSecretBitMap = appUtil->getSecretBitSubMap(index);
            for (map<int,std::string>::iterator it=NodeSecretBitMap.begin(); it!=NodeSecretBitMap.end(); ++it) {
				//get the adjacent prg string stored in the map
		    	prgsecretstring =  (std::string)it->second;
				//std::cout <<"prgsecretstring is " << prgsecretstring << "\n";
                result ^= (prgsecretstring.at(round) - 48);
				//break;
			}
			if(sender == index) {	//exor result with message
				result ^= bit;
			}
			ss << result;
           // std::cout << "current result is" << ss.str() << "\n";
		}
		appUtil->putAnnouncementInGlobalMap(index, ss.str());
		ss.str("");
	}
	for (int index1 = 0; index1 < (int)numNodes; index1++) {
		for (int index2 = 0; index2 < (int)numNodes; index2++) {
			if(index1 != index2) {
				Ptr<Socket> recvNodeSink = Socket::CreateSocket (c.Get (index2), tid);
				InetSocketAddress localSocket = InetSocketAddress (Ipv4Address::GetAny (),9802);
				recvNodeSink->Bind (localSocket);
				recvNodeSink->SetRecvCallback (MakeCallback (&ReceiveAnnouncement));
                InetSocketAddress remoteSocket = InetSocketAddress (i.GetAddress (index2, 0), 9802);
				Ptr<Socket> sourceNodeSocket = Socket::CreateSocket (c.Get (index1), tid);
				sourceNodeSocket->Connect (remoteSocket);
                Simulator::ScheduleNow (&SendAnnouncement, sourceNodeSocket,appUtil->getAnnouncement(index1), index1);	
			}	
		}
	}
}

void DisplayMeasurements()
{
    std::cout<<"Shared Message after "<<MessageLength<<" rounds is : "<<sharedMessage.str()<<"\n";
    std::cout<<"Sent Packet Count Stage 1: "<<stage1SentPacketCount<<"\n";
    std::cout<<"Sent Packet Count Stage 2: "<<stage2SentPacketCount<<"\n";
    std::cout<<"Sent Recv Count Stage 1: "<<stage1RecvPacketCount<<"\n";
    std::cout<<"Sent Recv Count Stage 2: "<<stage2RecvPacketCount<<"\n";

    stage1Latency = (stage1EndTime.front().GetSeconds() - stage1StartTime.front().GetSeconds());
    std::cout<<"Stage 1 latency: "<<stage1Latency<<"\n";

    stage2Latency = (stage2EndTime.front().GetSeconds() - stage2StartTime.front().GetSeconds());
    std::cout<<"Stage 2 latency: "<<stage2Latency<<"\n";

    totalLatency = (stage1Latency + stage2Latency);
   // std::cout<<"goodPut: "<<goodPut<<"\n";

totalTimeEnd = Simulator::Now();
totalRunningTime = totalTimeEnd.GetSeconds() - totalTimeStart.GetSeconds();
std::cout<<"Total time taken : "<<totalRunningTime<<" seconds\n";

ApplicationUtil *appUtil = ApplicationUtil::getInstance();
//output to csv

	if(option == 1)
		appUtil->writeOutputToFile((char*)"NumNodesvsMeasurements.csv",option,numNodes,MessageLength,totalLatency,totalRunningTime);	
	else if(option == 2)
		appUtil->writeOutputToFile((char*)"MsgLengthvsMeasurements.csv",option,numNodes,MessageLength,totalLatency,totalRunningTime);	
 
}

static void DCNET(Ptr<Socket> socket, bool done_dcnet)
{
		  	ApplicationUtil *appUtil = ApplicationUtil::getInstance();
		if(!done_dcnet) {
        //send the DH public key to everyone
        	for (int index1 = 0; index1 < (int)numNodes; index1++) {
			    if(index1==maxnodes) break;
				for (int index2 = 0; index2 < (int)numNodes; index2++) {
            		if(index1 != index2) {
                	//std::cout<<"Debug : Inside dcnet  1\n";
					if(index2==maxnodes) break;
                    Ptr<Socket> recvNodeSink = Socket::CreateSocket (c.Get (index2), tid);
                    InetSocketAddress localSocket = InetSocketAddress (Ipv4Address::GetAny (),9803+index2+index1);
                    recvNodeSink->Bind (localSocket);
                    recvNodeSink->SetRecvCallback (MakeCallback (&ReceivePublicKey));

                   	Ptr<Socket> sourceNodeSocket = Socket::CreateSocket (c.Get (index1), tid);
					InetSocketAddress remoteSocket = InetSocketAddress (i.GetAddress (index2, 0), 9803+index2+index1);
                    sourceNodeSocket->Connect (remoteSocket);
					Simulator::Schedule (Seconds(index1/1000000.0),&SendPublicKey, sourceNodeSocket,
										 appUtil->getPublicKeyFromMap(index1),index1);
				 
				}
            }
        }
    }
    else {
    	std::cout<<"Debug : Inside dcnet else part\n";
        DisplayMeasurements();
		socket->Close();
       	Simulator::Stop ();
	}
}

int main (int argc, char *argv[])
{
	NS_LOG_UNCOND("Inside Main");
    ApplicationUtil *appUtil = ApplicationUtil::getInstance();

    CommandLine cmd;
	
    //std::cout<<"argc : "<<argc<<"\n";

    cmd.AddValue ("numNodes", "Number of Nodes", numNodes);
    cmd.AddValue ("message", "Actual Message", Message);
    cmd.AddValue ("option", "Changing numnodes or messagelength", option);	  
    cmd.Parse (argc, argv);
    // disable fragmentation for frames below 2200 bytes
    Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
    // turn off RTS/CTS for frames below 2200 bytes
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
    // Fix non-unicast data rate to be the same as that of unicast
    Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",StringValue (phyMode));
    c.Create (numNodes);
	Config::SetDefault ("ns3::ArpCache::PendingQueueSize", UintegerValue (100));
    for(int nodeind = 0; nodeind < numNodes; nodeind++)
    {
        appUtil->putNodeInMap(c.Get(nodeind),nodeind);
    }
    // The below set of helpers will help us to put together the wifi NICs we want
    WifiHelper wifi;
    if (verbose)
    {
        wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

    YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
    // set it to zero; otherwise, gain will be added
    wifiPhy.Set ("RxGain", DoubleValue (0) );
    // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
    wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
    wifiPhy.SetChannel (wifiChannel.Create ());

    // Add a non-QoS upper mac, and disable rate control
    NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
    wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode",StringValue (phyMode),
                                  "ControlMode",StringValue (phyMode));
    // Set it to adhoc mode
    wifiMac.SetType ("ns3::AdhocWifiMac");
    NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);
    MobilityHelper mobility;
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0),
                                   "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (distance),
                                   "DeltaY", DoubleValue (distance),
                                   "GridWidth", UintegerValue (20),
                                   "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (c);
    Ipv4StaticRoutingHelper staticRouting;
    Ipv4ListRoutingHelper list;
    list.Add (staticRouting, 0);
    InternetStackHelper internet;
    internet.SetRoutingHelper (list); // has effect on the next Install ()
    internet.Install (c);

    Ipv4AddressHelper ipv4;
    NS_LOG_INFO ("Assign IP Addresses.");
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    i = ipv4.Assign (devices);

    tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
    //tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
   
    AnnouncementPacketCount = (numNodes * numNodes) - numNodes;
    publicKeyCounter = (numNodes * numNodes) - numNodes;
    randomBitCounter = (numNodes * (numNodes-1)/2);
      
    std::cout<<"Actual Message : "<<Message<<"\n";
    MessageLength = (int)strlen(Message.c_str()) ;
    std::cout<<"Message length:"<<MessageLength<<"\n";
	source = Socket::CreateSocket (c.Get (0), tid);
    stage1StartTime.push_back(Simulator::Now());
    totalTimeStart = Simulator::Now();
	//std::cout<<"Debug : Inside dcnet\n";
    	//Symmetric key generation
        for(int ind =0 ; ind < (int)numNodes; ind++) {
        	SecByteBlock priv, pub;
            generateKeys(ind,appUtil);
        }
    Simulator::ScheduleNow (&DCNET, source, false);
    if (tracing == true)
    {
        AsciiTraceHelper ascii;
        wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("wifi-simple-adhoc-grid.tr"));
        wifiPhy.EnablePcap ("wifi-simple-adhoc-grid", devices);
        // Trace routing tables
        Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("wifi-simple-adhoc-grid.routes", std::ios::out);


        // To do-- enable an IP-level trace that shows forwarding events only
    }
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}
