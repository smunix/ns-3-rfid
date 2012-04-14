#include "rfid-logic-reader.h"
#include <iostream>
#include "select-header.h"
#include "query-header.h"
#include "ack-header.h"

namespace ns3 
{



NS_OBJECT_ENSURE_REGISTERED (RfidLogicReader);

TypeId 
RfidLogicReader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidLogicReader")
    .SetParent<Object> ();
  return tid;
}

RfidLogicReader::RfidLogicReader (){}
RfidLogicReader::~RfidLogicReader (){}

Ptr<Packet>
RfidLogicReader::Select()
{
Ptr<Packet> p=Create<Packet>(0);
SelectHeader header;
p->AddHeader(header);
return p;
}

Ptr<Packet>
RfidLogicReader::Query()
{
Ptr<Packet> p=Create<Packet>(0);
QueryHeader header;
p->AddHeader(header);
return p;
}
Ptr<Packet>
RfidLogicReader::Ack()
{
Ptr<Packet> p=Create<Packet>(0);
AckHeader header;
p->AddHeader(header);
return p;
}

//Ptr<Packet>
void
RfidLogicReader::NextStepReader(Ptr<Packet> p)
{
// Acompléter selon le packet reçu
std::cout << "Emission et réception faite avec réussite" << std::endl; 
}
}//namespace ns3
