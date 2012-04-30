#ifndef RFID_MAC_READER_H
#define RFID_MAC_READER_H
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"

namespace ns3 {
class RfidLogicReader;
class RfidPhyReader;
class RfidChannel;

class RfidMacReader : public Object 
{
public:
  static TypeId GetTypeId (void);

RfidMacReader ();
virtual ~RfidMacReader ();

void Send (void);
void Receive (Ptr<Packet> p);


void SetAddress (Mac48Address address);
Mac48Address GetAddress (void);
// void NextStep (void);

private:
Ptr<RfidLogicReader> logic;
Ptr<RfidPhyReader> phy;
Ptr<RfidChannel> cha;

Mac48Address m_add;
};

} //namespace ns3

#endif // RFID_MAC_READER_H
