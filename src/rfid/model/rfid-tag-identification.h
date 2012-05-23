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
#include "rfid-identification.h"
#include "ns3/mac16-address.h"


namespace ns3
{
  namespace rfid
  {
   
    class TagIdentification : public Identification
    {
    public:
      typedef Callback<void, Ptr<Packet>, uint16_t> ForwardUpCb;
    public:
      static TypeId
      GetTypeId (void);

      TagIdentification(void);
      virtual
      ~TagIdentification (void);

      virtual void
      SetAddress (Address address) ;
      virtual Address
      GetAddress (void) const ;

      virtual void
      SetQueue (Ptr<class Queue> queue) ;
      virtual Ptr<class Queue>
      GetQueue (void) const ;

      virtual void
      SetRfidPhy (Ptr<class RfidPhy> phy) ;
      virtual Ptr<class RfidPhy>
      GetRfidPhy (void) const ;

      virtual void SetForwardUpCallback (ForwardUpCb const &cb) ;

      virtual void Receive (Ptr<Packet> packet) ;
      virtual bool Send (Ptr<Packet> packet) ;

      virtual void SetEquipement (int eq) ;
      virtual int GetEquipement (void) const ;

      virtual void SetState (int sta) ;
      virtual int GetState (void) const ;

      virtual void SetEquipementState (Ptr<Packet> packet, uint16_t header) ;
     
      virtual void SetReceiving (bool rcv);
      virtual bool GetReceiving (void) const;
   
    private:
      void AddEpcHeader (Ptr<Packet> packet,uint16_t header);
      uint16_t RemoveEpcHeader (Ptr<Packet> packet);
      void SetInitialConfiguration (void);
    private:
      ForwardUpCb m_forwardUp;
      Mac16Address m_address;
      Ptr<class RfidPhy> m_phy;
      Ptr<class Queue> m_queue;
      int m_eq;
      int m_sta;
      uint16_t m_header;
      uint16_t m_rn;
      bool m_next;
      bool m_first;
      uint16_t m_slot_counter;
      double m_duration;
      int m_preamble;
      bool m_rcv;
    };
  }
}

#endif /* RFID_TAG_IDENTIFICATION_H_ */
