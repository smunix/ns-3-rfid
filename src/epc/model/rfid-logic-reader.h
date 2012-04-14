#ifndef RFID_LOGIC_READER_H
#define RFID_LOGIC_READER_H
#include "ns3/object.h"
#include "ns3/packet.h"
namespace ns3 {




class RfidLogicReader : public Object 
{
public:
  static TypeId GetTypeId (void);

RfidLogicReader ();
virtual ~RfidLogicReader ();


Ptr<Packet> Select (void);
Ptr<Packet> Query (void);
Ptr<Packet> Ack (void);
/* Ptr<Packet> */ void NextStepReader (Ptr<Packet> p);


private:


};

} //namespace ns3

#endif // RFID_LOGIC_READER_H
