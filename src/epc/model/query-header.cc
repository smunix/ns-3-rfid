#include "query-header.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (QueryHeader);

QueryHeader::QueryHeader()
	:m_header(8) // 1000
{}

QueryHeader::~QueryHeader()
{}

uint8_t
QueryHeader::GetHeader() const
{
  return m_header;
}

TypeId 
QueryHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QueryHeader")
    .SetParent<Header> ()
    .AddConstructor<QueryHeader> ()
  ;
  return tid;
}

TypeId 
QueryHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}


void 
QueryHeader::Print (std::ostream &os) const
{
  os << " Query" << std::hex << m_header << std::dec;
}


uint32_t 
QueryHeader::GetSerializedSize (void) const
{
	return 1;
}

void
QueryHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_header);
}
uint32_t
QueryHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_header = i.ReadU8 ();

  return GetSerializedSize ();
}


}//namespace ns3
