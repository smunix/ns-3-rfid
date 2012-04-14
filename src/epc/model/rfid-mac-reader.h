#ifndef RFID_MAC_READER_H
#define RFID_MAC_READER_H
#include "ns3/object.h"
#include "ns3/packet.h"
namespace ns3 {
class RfidLogicReader;
class RfidPhyReader;


class RfidMacReader : public Object 
{
public:
  static TypeId GetTypeId (void);

RfidMacReader ();
virtual ~RfidMacReader ();

void Send (void);
void Receive (Ptr<Packet> p);
// void NextStep (void);

private:
Ptr<RfidLogicReader> logic;
Ptr<RfidPhyReader> phy;
};

} //namespace ns3

#endif // RFID_MAC_READER_H
