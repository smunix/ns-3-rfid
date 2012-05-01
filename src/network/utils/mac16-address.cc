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

#include "ns3/address.h"
#include "ns3/assert.h"
#include <iomanip>
#include <iostream>
#include <string.h>
#include "mac16-address.h"

namespace ns3
{
  ATTRIBUTE_HELPER_CPP(Mac16Address);

  // The following was taken verbatim from RfidMacAddress

#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

  static char
  AsciiToLowCase (char c)
  {
    if (c >= ASCII_a && c <= ASCII_z)
      {
        return c;
      }
    else if (c >= ASCII_A && c <= ASCII_Z)
      {
        return c + (ASCII_a - ASCII_A);
      }
    else
      {
        return c;
      }
  }

  Mac16Address::Mac16Address ()
  {
    memset (m_address, 0, LENGTH);
  }
  Mac16Address::Mac16Address (const char *str)
  {
    uint16_t i = 0;
    BufferHelper bh = BufferHelper(m_address);

    while (*str != 0 && i < LENGTH)
      {
        uint8_t byte = 0;
        while (*str != ASCII_COLON && *str != 0)
          {
            byte <<= 4; // TODO : smunix : maybe a potential bug source
            char low = AsciiToLowCase (*str);
            if (low >= ASCII_a)
              {
                byte |= low - ASCII_a + 10;
              }
            else
              {
                byte |= low - ASCII_ZERO;
              }
            str++;
          }
        bh[i] = byte;
        i++;
        if (*str == 0)
          {
            break;
          }
        str++;
      }
    NS_ASSERT (i == LENGTH);
  }
  void
  Mac16Address::CopyFrom (const Buffer_t buffer)
  {
    memcpy (m_address, buffer, LENGTH);
  }
  void
  Mac16Address::CopyTo (Buffer_t buffer) const
  {
    memcpy (buffer, m_address, LENGTH);
  }

  bool
  Mac16Address::IsMatchingType (const Address &address)
  {
    return address.CheckCompatible (GetType (), LENGTH);
  }
  Mac16Address::operator Address () const
  {
    return ConvertTo ();
  }
  Address
  Mac16Address::ConvertTo (void) const
  {
    return Address (GetType (), m_address, LENGTH);
  }
  Mac16Address
  Mac16Address::ConvertFrom (const Address &address)
  {
    NS_ASSERT (address.CheckCompatible (GetType (), LENGTH));
    Mac16Address retval;
    address.CopyTo (retval.m_address);
    return retval;
  }
  Mac16Address
  Mac16Address::Allocate (void)
  {
    static uint16_t id = 0;
    id++;
    Mac16Address address;
    BufferHelper bh = BufferHelper(address);
    bh[0] = (id >> 8) & 0xff;
    bh[1] = (id >> 0) & 0xff;
    return address;
  }
  uint8_t
  Mac16Address::GetType (void)
  {
    static uint8_t type = Address::Register ();
    return type;
  }

  bool
  Mac16Address::IsBroadcast (void) const
  {
    return *this == GetBroadcast ();
  }
  bool
  Mac16Address::IsGroup (void) const
  {
    return false;
  }
  Mac16Address
  Mac16Address::GetBroadcast (void)
  {
    static Mac16Address broadcast = Mac16Address ("ff:ff");
    return broadcast;
  }
  Mac16Address
  Mac16Address::GetMulticastPrefix (void)
  {
    static Mac16Address multicast = Mac16Address ("ff:00");
    return multicast;
  }
  Mac16Address
  Mac16Address::GetMulticast6Prefix (void)
  {
    static Mac16Address multicast = Mac16Address ("33:00");
    return multicast;
  }
  Mac16Address
  Mac16Address::GetMulticast (Ipv4Address multicastGroup)
  {
    Mac16Address epcAddr = Mac16Address::GetMulticastPrefix ();
    //
    // We now have the multicast address in an abstract 16-bit container.  We
    // need to pull it out so we can play with it.  When we're done, we have the
    // high order bits in epcBuffer[0], etc.
    //
    Buffer_t epcBuffer;
    epcAddr.CopyTo (epcBuffer);

    //
    // Now we need to pull the raw bits out of the Ipv4 destination address.
    //
    uint8_t ipBuffer[4];
    multicastGroup.Serialize (ipBuffer);

    // TODO : smunix : I state that this is the way we can map an Ipv4 address to an
    // epc 16-address.
    //
    BufferHelper(epcBuffer)[1] |= ipBuffer[1] & 0x7f;

    //
    // Now, epcBuffer has the desired ethernet multicast address.  We have to
    // suck these bits back into the Mac16Address,
    //
    Mac16Address result;
    result.CopyFrom (epcBuffer);
    return result;
  }
  Mac16Address Mac16Address::GetMulticast (Ipv6Address addr)
  {
    Mac16Address epcAddr = Mac16Address::GetMulticast6Prefix ();
    Buffer_t epcBuffer;
    BufferHelper bh = BufferHelper(epcBuffer);
    uint8_t ipBuffer[16];

    /* a MAC multicast IPv6 address is like 33:00 and the last low byte */
    /* for 2001:db8::2fff:fe11:ac10 => 33:10 */
    epcAddr.CopyTo (epcBuffer);
    addr.Serialize (ipBuffer);

    bh[1] = ipBuffer[15];

    epcAddr.CopyFrom (epcBuffer);

    return epcAddr;
  }

  std::ostream& operator<< (std::ostream& os, const Mac16Address & address)
  {
    Mac16Address::Buffer_t ad;
    address.CopyTo (ad);

    os.setf (std::ios::hex, std::ios::basefield);
    os.fill ('0');
    for (uint8_t i=0; i < Mac16Address::BufferHelper::SIZE - 1; i++)
      {
        os << std::setw (2) << (uint32_t)Mac16Address::BufferHelper(ad)[i] << ":";
      }
    // Final byte not suffixed by ":"
    os << std::setw (2) << (uint32_t)Mac16Address::BufferHelper(ad)[Mac16Address::BufferHelper::SIZE - 1];
    os.setf (std::ios::dec, std::ios::basefield);
    os.fill (' ');
    return os;
  }

  static uint8_t
  AsInt (std::string v)
  {
    std::istringstream iss;
    iss.str (v);
    uint32_t retval;
    iss >> std::hex >> retval >> std::dec;
    return retval;
  }

  std::istream& operator>> (std::istream& is, Mac16Address & address)
  {
    Mac16Address::BufferHelper bh = Mac16Address::BufferHelper(address);
    std::string v;
    is >> v;

    std::string::size_type col = 0;
    for (uint8_t i = 0; i < Mac16Address::BufferHelper::SIZE; ++i)
      {
        std::string tmp;
        std::string::size_type next;
        next = v.find (":", col);
        if (next == std::string::npos)
          {
            tmp = v.substr (col, v.size ()-col);
            bh[i] = AsInt (tmp);
            break;
          }
        else
          {
            tmp = v.substr (col, next-col);
            bh[i] = AsInt (tmp);
            col = next + 1;
          }
      }
    return is;
  }

}

