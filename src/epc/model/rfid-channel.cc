#include "rfid-channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/net-device.h"
#include "rfid-phy-reader.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include <iostream>
NS_LOG_COMPONENT_DEFINE ("RFIDCHANNEL");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RfidChannel);

TypeId 
RfidChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidChannel")
    .SetParent<Channel> ()
  
  ;
  return tid;
}

RfidChannel::RfidChannel ()
{
}

RfidChannel::~RfidChannel ()
{
}

void
RfidChannel::Sendto (Ptr<Packet> packet)
{

 // uint32_t j = 0;

void (RfidChannel::*fp)(uint32_t i, Ptr<Packet> packet) = &RfidChannel::Receive;

Simulator::ScheduleNow (fp, this,1,packet->Copy ());

  /*for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++, j++)
    {
     std::cout << "fuck27" << std::endl; 
          Ptr<Packet> copy = packet->Copy ();
std::cout << "fuck26" << std::endl;
          uint32_t dstNode;
std::cout << "fuck27" << std::endl;
          dstNode = m_phyList[j]->GetDevice ()->GetObject<NetDevice> ()->GetNode ()->GetId ();
std::cout << "fuck28" << std::endl;
     //     Simulator::ScheduleNow (RfidChannel::Receive,this, dstNode, copy);
          // j'ai un problème avec la dérnière commande :(
	 Simulator::ScheduleNow (&RfidChannel::Receive, this,
                                  m_phyList[j]->GetDevice ()->GetObject<NetDevice> ()->GetNode ()->GetId (),
                                  packet->Copy ());*/
	
    //}
}

void
RfidChannel::Receive (uint32_t i, Ptr<Packet> packet)
{  
  m_phyList[i]->Receive (packet);
}

uint32_t
RfidChannel::GetNDevices (void) const
{
  return m_phyList.size ();
}


Ptr<NetDevice>
RfidChannel::GetDevice (uint32_t i) const
{
  return m_phyList[i]->GetDevice ()->GetObject<NetDevice> ();
}


void
RfidChannel::Add (Ptr<RfidPhyReader> phy)
{   
  m_phyList.push_back (phy);
}


} // namespace ns3
