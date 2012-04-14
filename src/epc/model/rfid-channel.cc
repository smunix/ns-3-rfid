#include "rfid-channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"


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
RfidChannel::Send(Ptr<Packet> p)
{
}


} // namespace ns3
