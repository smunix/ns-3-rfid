#include "rfid-mac-reader.h"
#include "rfid-logic-reader.h"
#include "rfid-phy-reader.h"
#include "ns3/packet.h"
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
phy->Send(p);
}


void
RfidMacReader::Receive(Ptr<Packet> p)
{
logic->NextStepReader(p);
}

/*cette partie est provisoir puisque pour une première étape , on doit simuler le bon fonctionnment entre les couches et 
tester le fonctionnement du socket*/




}//namespace ns3
