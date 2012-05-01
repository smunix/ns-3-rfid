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

#ifndef RFID_MAC_ADDRESS_H_
#define RFID_MAC_ADDRESS_H_

#include <stdint.h>
#include <ostream>
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include "ipv4-address.h"
#include "ipv6-address.h"

namespace ns3
{
  class Address;

  /**
   * \ingroup address
   *
   * \brief an Rfid/EPC address
   *
   * This class can contain 16 bit EPC addresses.
   */
  class Mac16Address
  {
  public:
    template<typename T, size_t N>
    struct BufferHelper_t
    {
      typedef T Buffer_t[N];
      static size_t const SIZE = N;
      static size_t const LENGTH = sizeof (Buffer_t);
      static int MemCmp (Buffer_t const &a, Buffer_t const &b)
      {
        return ::memcmp(a, b, LENGTH);
      }
      BufferHelper_t (Buffer_t &buffer) :
        m_buffer (buffer)
      {}
      BufferHelper_t (Mac16Address &addr) :
        m_buffer (addr.m_address)
      {}
      T & operator[] (uint32_t ndx)
      {
        NS_ASSERT_MSG (ndx <= N, "Can not access slot pass the size of BufferHelper");
        return m_buffer[ndx];
      }
    private:
      Buffer_t &m_buffer;
    };

    typedef BufferHelper_t<uint8_t, 2> BufferHelper;
    typedef BufferHelper::Buffer_t Buffer_t;
    static size_t const LENGTH = sizeof(Buffer_t);
  public:
    Mac16Address (void);
    /**
     * \param str a string representing the new RfidMacAddress
     *
     * The format of the string is "xx:xx"
     */
    Mac16Address (const char *str);

    /**
     * \param buffer address in network order
     *
     * Copy the input address to our internal buffer.
     */
    void CopyFrom (const Buffer_t buffer);
    /**
     * \param buffer address in network order
     *
     * Copy the internal address to the input buffer.
     */
    void CopyTo (Buffer_t buffer) const;

    /**
     * \returns a new Address instance
     *
     * Convert an instance of this class to a polymorphic Address instance.
     */
    operator Address () const;
    /**
     * \param address a polymorphic address
     * \returns a new RfidMacAddress from the polymorphic address
     *
     * This function performs a type check and asserts if the
     * type of the input address is not compatible with an
     * RfidMacAddress.
     */
    static Mac16Address ConvertFrom (const Address &address);
    /**
     * \param address address to test
     * \returns true if the address matches, false otherwise.
     */
    static bool IsMatchingType (const Address &address);
    /**
     * Allocate a new RfidMacAddress.
     */
    static Mac16Address Allocate (void);

    /**
     * \returns true if this is a broadcast address, false otherwise.
     */
    bool IsBroadcast (void) const;

    /**
     * \returns true if the group bit is set, false otherwise.
     */
    bool IsGroup (void) const;

    /**
     * \returns the broadcast address
     */
    static Mac16Address GetBroadcast (void);

    /**
     * \param address base IPv4 address
     * \returns a multicast address
     */
    static Mac16Address GetMulticast (Ipv4Address address);

    /**
     * \brief Get multicast address from IPv6 address.
     * \param address base IPv6 address
     * \returns a multicast address
     */
    static Mac16Address GetMulticast (Ipv6Address address);

    /**
     * \returns the multicast prefix (01:00:5e:00:00:00).
     */
    static Mac16Address GetMulticastPrefix (void);

    /**
     * \brief Get the multicast prefix for IPv6 (33:33:00:00:00:00).
     * \returns a multicast address.
     */
    static Mac16Address GetMulticast6Prefix (void);
  private:
    /**
     * \returns a new Address instance
     *
     * Convert an instance of this class to a polymorphic Address instance.
     */
    Address ConvertTo (void) const;
    static uint8_t GetType (void);
    friend bool operator < (const Mac16Address &a, const Mac16Address &b);
    friend bool operator == (const Mac16Address &a, const Mac16Address &b);
    friend bool operator != (const Mac16Address &a, const Mac16Address &b);
    friend std::istream& operator>> (std::istream& is, Mac16Address & address);

    static int MemCmp (Mac16Address const &a, Mac16Address const &b)
    {
      return BufferHelper::MemCmp(a.m_address, b.m_address);
    }
    Buffer_t m_address;
  };

  /**
   * \class ns3::Mac48AddressValue
   * \brief hold objects of type ns3::Mac48Address
   */

  ATTRIBUTE_HELPER_HEADER (Mac16Address);

  inline bool operator == (const Mac16Address &a, const Mac16Address &b)
  {
    return Mac16Address::MemCmp(a, b) == 0;
  }
  inline bool operator != (const Mac16Address &a, const Mac16Address &b)
  {
    return Mac16Address::MemCmp(a, b) != 0;
  }
  inline bool operator < (const Mac16Address &a, const Mac16Address &b)
  {
    return Mac16Address::MemCmp(a, b) < 0;
  }

  std::ostream& operator<< (std::ostream& os, const Mac16Address & address);
  std::istream& operator>> (std::istream& is, Mac16Address & address);
}

#endif /* RFID_MAC_ADDRESS_H_ */
