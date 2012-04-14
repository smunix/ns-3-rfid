#ifndef SELECT_HEADER_H
#define SELECT_HEADER_H

#include "ns3/header.h"

namespace ns3 
{

class SelectHeader: public Header
{
public:

  SelectHeader ();
  ~SelectHeader ();
  
  uint8_t GetHeader () const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  uint8_t m_header;

};
}//namespace ns3
#endif /* SELECT_HEADER_H */
