#ifndef RFID_RAW_SOCKET_H
#define RFID_RAW_SOCKET_H


#include "ns3/socket.h"
namespace ns3{

class RfidRawSocket: public Socket
{
public:
static TypeId GetTypeId (void);
RfidRawSocket();
~RfidRawSocket();

 
private:



};
} //namespace ns3
#endif /* RFID_RAW_SOCKET_H */
