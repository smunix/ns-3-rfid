/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 SMUNIX
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Providence M. SALUMU <Providence.Salumu@smunix.com>
 */

#ifndef EPC_HEADER_H_
#define EPC_HEADER_H_

#include "ns3/header.h"
#include <string>
#include "ns3/mac16-address.h"

namespace ns3
{
  class EpcHeader : public Header
  {
  public:
    static TypeId GetTypeId (void);
    TypeId GetInstanceTypeId (void) const;

    EpcHeader (void);
    virtual ~EpcHeader (void);
  public:
    /**
     * \returns the expected size of the header.
     *
     * This method is used by Packet::AddHeader
     * to store a header into the byte buffer of a packet. This method
     * should return the number of bytes which are needed to store
     * the full header data by Serialize.
     */
    virtual uint32_t GetSerializedSize (void) const;
    /**
     * \param start an iterator which points to where the header should
     *        be written.
     *
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     * The data written
     * is expected to match bit-for-bit the representation of this
     * header in a real network.
     */
    virtual void Serialize (Buffer::Iterator start) const;
    /**
     * \param start an iterator which points to where the header should
     *        written.
     * \returns the number of bytes read.
     *
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     * The data read is expected to
     * match bit-for-bit the representation of this header in real
     * networks.
     */
    virtual uint32_t Deserialize (Buffer::Iterator start);
    /**
     * \param os output stream
     * This method is used by Packet::Print to print the
     * content of a trailer as ascii data to a c++ output stream.
     * Although the trailer is free to format its output as it
     * wishes, it is recommended to follow a few rules to integrate
     * with the packet pretty printer: start with flags, small field
     * values located between a pair of parens. Values should be separated
     * by whitespace. Follow the parens with the important fields,
     * separated by whitespace.
     * i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
     */
    virtual void Print (std::ostream &os) const;



    void SetHeader (uint16_t header);
    uint16_t GetHeader (void) const;

  private:

    uint16_t m_header;
  };
}

#endif /* EPC_HEADER_H_ */
