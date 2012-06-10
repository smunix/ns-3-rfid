/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 ABID
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
 * Author: ABID AYOUB <abid.ayoub@gmail.com>
 */

#ifndef RFID_IDENTIFICATION_H_
#define RFID_IDENTIFICATION_H_

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/mac16-address.h"

namespace ns3
{
  namespace rfid
  {
    enum RfidEquipement 
    {
    TAG=0,
    READER
    };
    
    enum TagStates 
    {
    IDLE_TAG=0,
    READY,
    ARBITRATE,
    REPLY,
    ACKNOWLEDGED
    };

    enum ReaderStates 
    {
    IDLE_READER=0,
    SELECT,
    QUERY,
    QUERY_REP,
    QUERY_ADJUST,
    ACK
    };

    enum SessionStates
    {
    S0=0,
    S1,
    S2,
    S3,
    SL
    };

    enum SessionValues
    {
    A=0,
    B
    };

    enum MemoryBank
    {
    RESERVED=0,
    EPC,
    TAGID,
    USER
    };

    class Identification : public Object
    {
    public:
      typedef Callback<void, Ptr<Packet>, uint16_t> ForwardUpCb;
    public:
      static TypeId
      GetTypeId (void);

      
      virtual
      ~Identification (void);

      virtual void
      SetAddress (Address address) = 0;
      virtual Address
      GetAddress (void) const = 0;

      virtual void
      SetQueue (Ptr<class Queue> queue) = 0;
      virtual Ptr<class Queue>
      GetQueue (void) const = 0;

      virtual void
      SetRfidPhy (Ptr<class RfidPhy> phy) = 0;
      virtual Ptr<class RfidPhy>
      GetRfidPhy (void) const = 0;

      virtual void SetForwardUpCallback (ForwardUpCb const &cb) = 0;

      virtual void Receive (Ptr<Packet> packet) = 0;
      virtual bool Send (Ptr<Packet> packet) = 0;

      virtual void SetEquipement (int eq) = 0;
      virtual int GetEquipement (void) const = 0;

      virtual void SetState (int sta) = 0;
      virtual int GetState (void) const = 0;

      virtual void SetEquipementState (Ptr<Packet> packet, uint16_t header) = 0;

      virtual void SetReceiving (bool rcv) = 0;
      virtual bool GetReceiving (void) const = 0;

      virtual void SetCollision (bool collision) = 0;
      virtual bool GetCollision (void) const = 0; 

      double GetPacketDuration ( int size, uint32_t packet);

      void SetTagNumber (int tag);
      int GetTagNumber (void) const ;

   private:
     int m_tagNumber;
     int m_session;
     int m_sessionValue;
     bool m_sl;
    };
  }
}

#endif /* RFID_IDENTIFICATION_H_ */
