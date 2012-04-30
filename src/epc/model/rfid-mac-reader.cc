#include "rfid-mac-reader.h"
#include "rfid-logic-reader.h"
#include "rfid-phy-reader.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include <iostream>

#include "rfid-channel.h"
namespace ns3 
{



NS_OBJECT_ENSURE_REGISTERED (RfidMacReader);

TypeId 
RfidMacReader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidMacReader")
    .SetParent<Object> ();
  return tid;
}

RfidMacReader::RfidMacReader () {}
RfidMacReader::~RfidMacReader (){}




void
RfidMacReader::Send()
{

Ptr<Packet> p = Create<Packet>();

p = logic->Select();

cha->Sendto(p);
}
/////////////////////////////////

void
RfidMacReader::Receive(Ptr<Packet> p)
{
logic->NextStepReader(p);
}

void
RfidMacReader::SetAddress (Mac48Address address)
{
  m_add=address;
}

Mac48Address
RfidMacReader::GetAddress ()
{
  return m_add;
}
/*cette partie est provisoir puisque pour une première étape , on doit simuler le bon fonctionnment entre les couches et 
tester le fonctionnement du socket*/




}//namespace ns3
