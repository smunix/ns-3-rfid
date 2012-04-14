#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "rfid-channel.h"
#include "ns3/packet.h"

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
channel->Send(p);
}

void
RfidPhyReader::Receive(Ptr<Packet> p)
{
mac->Receive(p);
}

/*cette partie manque encore l'ajout des caractéristique de la couche physiaque et les durées de transmissions des entêtes et du message en fonction du type de transmision et l'atténuation du signal en fonction des phénomènes physiques */
}//namespace ns3
