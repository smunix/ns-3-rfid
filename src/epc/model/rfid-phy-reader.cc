#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "rfid-channel.h"
#include "ns3/packet.h"
#include <iostream>
namespace ns3 
{



NS_OBJECT_ENSURE_REGISTERED (RfidPhyReader);

TypeId 
RfidPhyReader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidPhyReader")
    .SetParent<Object> ();
  return tid;
}

RfidPhyReader::RfidPhyReader () {}
RfidPhyReader::~RfidPhyReader (){}

void
RfidPhyReader::Send(Ptr<Packet> p)
{

Ptr<Packet> packet=p->Copy ();
m_channel->Sendto(packet);


}

void
RfidPhyReader::Receive(Ptr<Packet> p)
{
m_mac->Receive(p);
}

Ptr<Object>
RfidPhyReader::GetDevice (void)
{
  return m_device;
}

/*cette partie manque encore l'ajout des caractéristique de la couche physiaque et les durées de transmissions des entêtes et du message en fonction du type de transmision et l'atténuation du signal en fonction des phénomènes physiques */
}//namespace ns3
