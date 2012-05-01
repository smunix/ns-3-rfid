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

#ifndef RFID_TAG_IDENTIFICATION_H_
#define RFID_TAG_IDENTIFICATION_H_

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/mac16-address.h"

namespace ns3
{
  namespace rfid
  {
    class TagIdentification : public Object
    {
    public:
      typedef Callback<void, Ptr<Packet>, Mac16Address, Mac16Address, uint16_t> ForwardUpCb;
    public:
      static TypeId
      GetTypeId (void);

      TagIdentification(void);
      virtual
      ~TagIdentification (void);

      virtual void
      SetAddress (Address address);
      virtual Address
      GetAddress (void) const;

      virtual void
      SetQueue (Ptr<class Queue> queue);
      virtual Ptr<class Queue>
      GetQueue (void) const;

      virtual void
      SetRfidPhy (Ptr<class RfidPhy> phy);
      Ptr<class RfidPhy>
      GetRfidPhy (void) const;

      void SetForwardUpCallback (ForwardUpCb const &cb);

      void Receive (Ptr<Packet> packet);
      bool Send (Ptr<Packet> packet, const Address& dest,
            uint16_t protocolNumber);
    private:
      void AddEpcHeader (Ptr<Packet> packet, Address const &from, Address const &dest, uint16_t protocolNumber);
    private:
      ForwardUpCb m_forwardUp;
      Mac16Address m_address;
      Ptr<class RfidPhy> m_phy;
      Ptr<class Queue> m_queue;
    };
  }
}

#endif /* RFID_TAG_IDENTIFICATION_H_ */
