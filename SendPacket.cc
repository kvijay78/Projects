#include "ApplicationUtil.h"
NS_LOG_COMPONENT_DEFINE ("WifiSimpleAdhocGrid");

std::string hexStr(byte *data, int len)
{
	std::stringstream ss;
	ss<<std::hex;
	for(int i(0);i<len;++i)
	ss<<(int)data[i];
	return ss.str();
}
void ReceivePacket (Ptr<Socket> socket)
{
	Ptr<Packet> recPacket = socket->Recv();
	uint8_t *buffer = new uint8_t[recPacket->GetSize()];
	recPacket->CopyData(buffer,recPacket->GetSize());
	std::string recData = std::string((char*)buffer);

	MyTag recTag;
	recPacket->PeekPacketTag(recTag);
	int tagVal =int(recTag.GetSimpleValue());
	std::ostringstream s;
	s<<tagVal;
	std::string ss(s.str());
	   
//	NS_LOG_UNCOND ("Received one packet: Data: " +recData+"   TagID: "+ss);
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, 
                  uint32_t pktCount, Time pktInterval , int i)
{
	if (pktCount > 0)
	{
		std::string msgx = msgs[i]; 

		Ptr<Packet> sendPacket = Create<Packet> ((uint8_t*)msgx.c_str(),pktSize);

		MyTag sendTag;
		sendTag.SetSimpleValue(i);
		sendPacket->AddPacketTag(sendTag); 
		socket->Send (sendPacket);
	//	NS_LOG_UNCOND ("Sending one packet: "+msgx);  
		Simulator::Schedule (pktInterval, &GenerateTraffic, 
			       socket, pktSize,pktCount-1, pktInterval, i+1);
	}
	else
	{
		socket->Close ();
	}
}


static void SendMessage (Ptr<Socket> socket, std::string message, int index, int dest)
{
	Ptr<Packet> sendPacket =
	Create<Packet> ((uint8_t*)message.c_str(),message.size());

	MyTag sendTag;
	sendTag.SetSimpleValue(index);
	sendPacket->AddPacketTag(sendTag); 
	socket->Send (sendPacket);
	socket->Close ();
}

void ReceiveMessage (Ptr<Socket> socket)
{
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
	std::ostringstream s;
	s<<srcNodeIndex;
	std::string ss(s.str());
	std::ostringstream s1;
	s1<<recNodeIndex;
	std::string ss1(s1.str());

	SecByteBlock key(SHA256::DIGESTSIZE);
	SHA256().CalculateDigest(key, appUtil->getSecretKeyFromGlobalMap(srcNodeIndex,recNodeIndex), appUtil->getSecretKeyFromGlobalMap(srcNodeIndex,recNodeIndex).size()); 

	//Decryption using the Shared secret key
	CFB_Mode<AES>::Decryption cfbDecryption(key, aesKeyLength, iv);
	cfbDecryption.ProcessData((byte*)recMessage.c_str(), (byte*)recMessage.c_str(), messageLen);
	
	// std::cout<<"message 4: "<<recMessage<<"\n";
//	NS_LOG_UNCOND ("Received message packet: Data: " +recMessage+"   TagID: "+ss + " to "+ss1+"\n");

	int value = atoi(recMessage.c_str());
//	std::cout<<"Value :"<<value<<"\n";
	//put in node's map

	appUtil->putSecretBitInGlobalMap(srcNodeIndex,recNodeIndex,value);
	appUtil->putSecretBitInGlobalMap(recNodeIndex,srcNodeIndex,value);
}


int randomBitGeneratorWithProb(double p) 
{
	double rndDouble = (double)rand() / RAND_MAX;
	return rndDouble > p;
}

static void SimulatorLoop(Ptr<Socket> socket,TypeId tid, NodeContainer c, Ipv4InterfaceContainer i, double waitTime)	
{
	ApplicationUtil *appUtil = ApplicationUtil::getInstance();
	// Generate a random IV
	rnd.GenerateBlock(iv, AES::BLOCKSIZE);
	
	//sharing the random bit using dh secret key
	for (int index1 = 0; index1 < (int)numNodes; index1++)
	{

		for (int index2 = 0; index2 < (int)numNodes; index2++)
		{
			if(index1 < index2)
			{			
				int randomBit = randomBitGeneratorWithProb(0.5);
	//			std::cout<<"Random bit : "<<randomBit<<" "<<index1<<" "<<index2<<"\n";

				//put random bit in both the maps - src and dest maps

				appUtil->putSecretBitInGlobalMap(index1,index2,randomBit);
				appUtil->putSecretBitInGlobalMap(index2,index1,randomBit);
	
				// Calculate a SHA-256 hash over the Diffie-Hellman session key
				SecByteBlock key(SHA256::DIGESTSIZE);
				SHA256().CalculateDigest(key, appUtil->getSecretKeyFromGlobalMap(index1,index2), appUtil->getSecretKeyFromGlobalMap(index1,index2).size()); 

				std::ostringstream ss;
				ss << randomBit;
				std::string message = ss.str();
				messageLen = (int)strlen(message.c_str()) + 1;

				// Encrypt

				CFB_Mode<AES>::Encryption cfbEncryption(key, aesKeyLength, iv);
				cfbEncryption.ProcessData((byte*)message.c_str(), (byte*)message.c_str(), messageLen);
				
				//Send the encrypted message
				Ptr<Socket> recvNodeSink = Socket::CreateSocket (c.Get (index2), tid);
				InetSocketAddress localSocket = InetSocketAddress (Ipv4Address::GetAny (), 82);
				recvNodeSink->Bind (localSocket);
				recvNodeSink->SetRecvCallback (MakeCallback (&ReceiveMessage));
							      
				InetSocketAddress remoteSocket = InetSocketAddress (i.GetAddress (index2, 0), 82);
				Ptr<Socket> sourceNodeSocket = Socket::CreateSocket (c.Get (index1), tid);
				sourceNodeSocket->Connect (remoteSocket);
				Simulator::Schedule (Seconds (waitTime + 10.0), &SendMessage, sourceNodeSocket,message,index1,index2);
			}
		}
	}


}

static void SendPublicKey (Ptr<Socket> socket, SecByteBlock pub, int index)
{	
	Ptr<Packet> sendPacket = Create<Packet> ((uint8_t*)pub.BytePtr(),(uint8_t) pub.SizeInBytes());

//	std::cout<<"Node : "<<index<<" sending public key data\n";
	MyTag sendTag;
	sendTag.SetSimpleValue(index);
	sendPacket->AddPacketTag(sendTag);

	socket->Send(sendPacket);
	std::string sendData = hexStr(pub.BytePtr(),pub.SizeInBytes());

	socket->Close();
}

void ReceivePublicKey (Ptr<Socket> socket)
{

	Ptr<Node> recvnode = socket->GetNode();
	int recNodeIndex = ApplicationUtil::getInstance()->getNodeFromMap(recvnode);

	Ptr<Packet> recPacket = socket->Recv();
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
}

void generateKeys(int index, ApplicationUtil *appUtil)
{
	try{
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

	//	std::cout<<"Dh key length "<< index <<" : "<<dh.AgreedValueLength()<<"\n";
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

void DisplayMessage(Ptr<Socket> socket)
{	
	
	ApplicationUtil *appUtil = ApplicationUtil::getInstance();
			
		int result = 0;
		int bit = Message.at(rounds)-48 ;
		
		for(int index = 0; index < (int)numNodes ; index++)
		{
		
			map<int,int> NodeSecretBitMap = appUtil->getSecretBitSubMap(index);
		
			for (map<int,int>::iterator it=NodeSecretBitMap.begin(); it!=NodeSecretBitMap.end(); ++it)
	    		{
				//Exor the adjacent node bits stored in the map			
				result ^= (int)it->second;
			
			}	
			if(sender == index)	//exor result with message
			{				
				result ^= bit;	
			}
		}
			
		sharedMessage<<result;

	
	socket->Close();
}
  
void DCNET(Ptr<Socket> socket, double waitTime, int numRounds)
{
	ApplicationUtil *appUtil = ApplicationUtil::getInstance(); 
	if(numRounds < MessageLength)
	{
	 rounds = numRounds;	
		
	//Symmetric key generation
	for(int ind =0 ; ind < (int)numNodes; ind++)
	{	
		SecByteBlock priv, pub;
		generateKeys(ind,appUtil);		
	}

	//send the public key to everyone
	for (int index1 = 0; index1 < (int)numNodes; index1++)
	{
		  
		for (int index2 = 0; index2 < (int)numNodes; index2++)
		{
			if(index1 != index2)
			{
				Ptr<Socket> recvNodeSink = Socket::CreateSocket (c.Get (index2), tid);
				      InetSocketAddress localSocket = InetSocketAddress (Ipv4Address::GetAny (), 81);
				      recvNodeSink->Bind (localSocket);
				      recvNodeSink->SetRecvCallback (MakeCallback (&ReceivePublicKey));
				    				      
				      InetSocketAddress remoteSocket = InetSocketAddress (i.GetAddress (index2, 0), 81);
				Ptr<Socket> sourceNodeSocket = Socket::CreateSocket (c.Get (index1), tid);
				      sourceNodeSocket->Connect (remoteSocket);
	Simulator::Schedule (Seconds (keyExchangeInterval), &SendPublicKey, sourceNodeSocket,appUtil->getPublicKeyFromMap(index1),index1);
			}	
		}
	}	
      
	waitTime +=  (2.0 * numNodes * keyExchangeInterval)  + 5.0;
	Ptr<Socket> source = Socket::CreateSocket (c.Get (0), tid);
	Simulator::Schedule (Seconds (waitTime), &SimulatorLoop, source,tid,c,i, waitTime);
//	std::cout<<"Wait time : "<<waitTime;
	waitTime = 1.5 * waitTime + 5.0;
	Simulator::Schedule (Seconds (waitTime), &DisplayMessage,source);
//	std::cout<<"Wait time 2: "<<waitTime;
	
	Simulator::Schedule (Seconds (waitTime+5.0), &DCNET, source,waitTime+5.0, numRounds+1);
	}
	else
	{
		std::cout<<"Shared Message after "<<MessageLength<<" rounds is : "<<sharedMessage.str()<<"\n";
		socket->Close();
		Simulator::Stop ();
	}
}  
    int main (int argc, char *argv[])
    {

      NS_LOG_UNCOND("Inside Main");
      msgs[0]="UCLA";
      msgs[1]="MIT"; msgs[2]="Stanford"; msgs[3]="Berkley";
      msgs[4]="UC Irvine"; msgs[5]="UC San Diego"; msgs[6]="USC";
      msgs[7]="Carnegie Mellon University"; msgs[8]="UPenn";
      msgs[9]="Columbia University"; msgs[10]="Cornell University";
      msgs[11]="Georgia Tech"; msgs[12]="TAMU"; msgs[13]="NYU";
      msgs[14]="UT Austin"; msgs[15]="Arizona State University";
      msgs[16]="NCSU"; msgs[17]="University of Maryland,College Park";
      msgs[18]="Stony Brook University"; 
      msgs[19]="University of Indiana,Bloomington";

	ApplicationUtil *appUtil = ApplicationUtil::getInstance();     

    
      CommandLine cmd;
    
      cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
      cmd.AddValue ("distance", "distance (m)", distance);
      cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
      cmd.AddValue ("numPackets", "number of packets generated", numPackets);
      cmd.AddValue ("interval", "interval (seconds) between packets", interval);
      cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
      cmd.AddValue ("tracing", "turn on ascii and pcap tracing", tracing);
      cmd.AddValue ("numNodes", "number of nodes", numNodes);
      cmd.AddValue ("sinkNode", "Receiver node number", sinkNode);
      cmd.AddValue ("sourceNode", "Sender node number", sourceNode);
    
      cmd.Parse (argc, argv);
      // Convert to time object
      Time interPacketInterval = Seconds (interval);
    
      // disable fragmentation for frames below 2200 bytes
      Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
      // turn off RTS/CTS for frames below 2200 bytes
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
      // Fix non-unicast data rate to be the same as that of unicast
      Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                          StringValue (phyMode));
    
     
      c.Create (numNodes);
	for(int nodeind = 0; nodeind < numNodes;nodeind++)
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
      wifiPhy.Set ("RxGain", DoubleValue (-10) ); 
      // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
      wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 
    
      YansWifiChannelHelper wifiChannel;
      wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
      wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
      wifiPhy.SetChannel (wifiChannel.Create ());
    
      // Add a non-QoS upper mac, and disable rate control
      NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
      wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
      wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                    "DataMode",StringValue (phyMode),
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
                                     "GridWidth", UintegerValue (5),
                                     "LayoutType", StringValue ("RowFirst"));
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (c);
    
      // Enable OLSR
      OlsrHelper olsr;
      Ipv4StaticRoutingHelper staticRouting;
    
      Ipv4ListRoutingHelper list;
      list.Add (staticRouting, 0);
      list.Add (olsr, 10);
    
      InternetStackHelper internet;
      internet.SetRoutingHelper (list); // has effect on the next Install ()
      internet.Install (c);
    
      Ipv4AddressHelper ipv4;
      NS_LOG_INFO ("Assign IP Addresses.");
      ipv4.SetBase ("10.1.1.0", "255.255.255.0");
      i = ipv4.Assign (devices);
    
      tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
 
 /*     Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (sinkNode), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
      recvSink->Bind (local);
      recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
    
      Ptr<Socket> source = Socket::CreateSocket (c.Get (sourceNode), tid);
      InetSocketAddress remote = InetSocketAddress (i.GetAddress (sinkNode, 0), 80);
      source->Connect (remote);

    */
	
	std::cout<<"Actual Message : "<<Message<<"\n";
	MessageLength = (int)strlen(Message.c_str()) ;
	Ptr<Socket> source = Socket::CreateSocket (c.Get (0), tid);
	Simulator::Schedule (Seconds (1.0), &DCNET, source, 1.0, 0);
	
	
		
      if (tracing == true)
        {
          AsciiTraceHelper ascii;
          wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("wifi-simple-adhoc-grid.tr"));
          wifiPhy.EnablePcap ("wifi-simple-adhoc-grid", devices);
          // Trace routing tables
          Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("wifi-simple-adhoc-grid.routes", std::ios::out);
          olsr.PrintRoutingTableAllEvery (Seconds (2), routingStream);
    
          // To do-- enable an IP-level trace that shows forwarding events only
        }	
    
      // Give OLSR time to converge-- 30 seconds perhaps
  //    Simulator::Schedule (Seconds (1.0), &GenerateTraffic, source, packetSize, numPackets, interPacketInterval,0);
 
      // Output what we are doing
 //     NS_LOG_UNCOND ("Testing from node " << sourceNode << " to " << sinkNode << " with grid distance " << distance);
    
      //Simulator::Stop (Seconds (3000.0));
      Simulator::Run ();
      Simulator::Destroy ();
    
      return 0;
    }
