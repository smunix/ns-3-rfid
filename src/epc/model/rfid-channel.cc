#include "rfid-channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/net-device.h"
#include "rfid-phy-reader.h"
#include "ns3/node.h"
#include "ns3/simulator.h"


namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("RFIDCHANNEL");
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
RfidChannel::Send (Ptr<const Packet> packet)
{

  uint32_t j = 0;
  for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++, j++)
    {
      // smunix : The following logic is hairy and borked! Please, fix this!
          Simulator::ScheduleNow (&RfidChannel::Receive, this,
                                  m_phyList[j]->GetDevice ()->GetObject<NetDevice> ()->GetNode ()->GetId (),
                                  packet->Copy ());
    }
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
