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

#include "ns3/log.h"
#include "ns3/queue.h"
#include "epc-header.h"
#include "rfid-phy.h"
#include "rfid-reader-identification.h"
#include "ns3/simulator.h"
#include "rfid-preamble.h"
#include "ns3/double.h"

#include <iostream>
#include <ctime>
#include <stdlib.h>



NS_LOG_COMPONENT_DEFINE("RfidReaderIdentification");

namespace ns3
{
  namespace rfid
  {

    NS_OBJECT_ENSURE_REGISTERED(ReaderIdentification);

    TypeId
    ReaderIdentification::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::rfid::ReaderIdentification")
        .SetParent<Object>()
        .AddConstructor<ReaderIdentification> ()
        .AddAttribute ("DivideRatio",
                   "equal to 8 or 64/3",
                   DoubleValue (8.0),
                   MakeDoubleAccessor (&ReaderIdentification::m_dr),
                   MakeDoubleChecker<double> ())
        .AddAttribute ("Modulation",
                   "equal to 1, 2, 4 or 8",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReaderIdentification::m_m),
                   MakeDoubleChecker<double> ())   
        .AddAttribute ("MaxCollision",
                   "Max collision allowed before sending QueryAdjust",
                   DoubleValue (1),
                   MakeDoubleAccessor (&ReaderIdentification::m_maxCollisionAllowed),
                   MakeDoubleChecker<int> ())         
        .AddAttribute ("MaxNoResponse",
                   "Max no response allowed before sending QueryAdjust",
                   DoubleValue (100),
                   MakeDoubleAccessor (&ReaderIdentification::m_maxNoResponseAllowed),
                   MakeDoubleChecker<int> ())      
;
      return tid;
    }
    ReaderIdentification::ReaderIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    ReaderIdentification::~ReaderIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    Address
    ReaderIdentification::GetAddress (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_address;
    }
    void
    ReaderIdentification::SetAddress (Address address)
    {
      NS_LOG_FUNCTION(address);
      m_address = Mac16Address::ConvertFrom (address);
    }
    void
    ReaderIdentification::SetQueue (Ptr<Queue> queue)
    {
      NS_LOG_FUNCTION(queue);
      m_queue = queue;
    }
    Ptr<Queue>
    ReaderIdentification::GetQueue (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_queue;
    }
    void
    ReaderIdentification::SetRfidPhy(Ptr<RfidPhy> phy)
    {
      NS_LOG_FUNCTION(phy);
      m_phy = phy;
    }
    Ptr<RfidPhy>
    ReaderIdentification::GetRfidPhy(void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_phy;
    }
    void
    ReaderIdentification::SetForwardUpCallback (ForwardUpCb const &cb)
    {
      NS_LOG_FUNCTION(&cb);
      m_forwardUp = cb;
    }

    void
    ReaderIdentification::SetEquipement (int eq)
    {
      m_eq=eq;
      m_header = 0x00;
      m_first = true;
      SetReceiving (true);
      m_tag_number = 0;
      m_conf.SetM(m_m);
      m_conf.SetDR(m_dr);
      SetState (IDLE_READER);
      m_noResponseCounter = 0;
    }
    int
    ReaderIdentification::GetEquipement (void) const
    { 
      return m_eq;
    }

    void
    ReaderIdentification::SetState (int sta)
    {
      m_sta=sta;   
    }
    int
    ReaderIdentification::GetState (void) const
    {
      return m_sta;
    }

    void
    ReaderIdentification::SetQForQuery (void)
    {
      srand(time(0));
      m_qf = ((double)rand()/(double)RAND_MAX)* 0x0F;
      m_q = (int)m_qf;
      std::cout << " ***m_q*** " << m_qf << " " << m_q << std::endl ;
    }

    void
    ReaderIdentification::SetQForQueryAdjust (void)
    {
      m_qf -= 0.5 ;
      m_q = (int)m_qf;
      std::cout << " ***m_q*** " << m_qf << " " << m_q << std::endl ;
    }

    void
    ReaderIdentification::SendQueryRepOrAdjust (Ptr<Packet> packet)
    { 
      if (!GetReceiving ()) 
      { 
        std::cout << " no response +++++++++++++++++ " << m_noResponseCounter << " " <<  m_maxNoResponseAllowed << std::endl;
        m_duration = 0;
        if (m_maxNoResponseAllowed !=  m_noResponseCounter)
        {
                std::cout << " ******** QueryRep ******** " << std::endl;
                m_noResponseCounter++;
                SetState(QUERY_REP);
                m_header = 0x00;
                m_duration = GetPacketDuration ( 2 , m_header) + m_conf.GetFrameSyncDuration ();
        }
        else 
        {
                std::cout << " ******** QueryAdjust ******** " << std::endl;
                m_noResponseCounter = 0;
                SetState(QUERY_REP);
                AddEpcHeader (packet,m_q);
                m_header = 0x09;
                m_duration = GetPacketDuration ( 4 , m_header) + GetPacketDuration ( 4 , m_q) + m_conf.GetFrameSyncDuration ();
        }
        AddEpcHeader (packet,m_header);
        Send (packet);
      }
    }

    void
    ReaderIdentification::SetEquipementState (Ptr<Packet> packet, uint16_t header) 
    { 
      m_duration = 0 ; 
      std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
      if (GetEquipement () == READER)
        {
                 switch (GetState ())
                 {

                  case (IDLE_READER):
                  SetState(SELECT);
                  m_header=0x0A;
                  m_duration += GetPacketDuration ( 4 , m_header);
                  break;

                  case (SELECT):
                  SetState(QUERY);
                  m_header=0x08;
                  m_duration += GetPacketDuration ( 4 , m_header) + GetPacketDuration ( 4 , m_q);
                  SetQForQuery();
                  AddEpcHeader (packet,m_q); 
                  m_preamble = RFID_PREAMBLE;
                  break;

                  case (QUERY):
                  SetState(ACK);
                  m_header=0x01;
                  m_duration += GetPacketDuration ( 2 , m_header) + GetPacketDuration ( 16 , RemoveEpcHeader( packet-> Copy ()));
                  m_preamble = RFID_FRAME_SYNC;
                  break;

                  case (QUERY_REP):
                  SetState(ACK);
                  m_header=0x01;
                  m_duration += GetPacketDuration ( 2 , m_header) + GetPacketDuration ( 16 , RemoveEpcHeader( packet-> Copy ()));
                  m_preamble = RFID_FRAME_SYNC;
                  break;

                  default:
                  SetState(QUERY_REP);
                  m_noResponseCounter = 0;
                  m_tag_number += 1; 
                  std::cout << "************************ " << m_tag_number << " tags are identified ************************" << std::endl;
                  Ptr<Packet> p = Create <Packet> ();
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
                  SetReceiving (false);
                  if (m_tag_number != GetTagNumber ()) {SendQueryRepOrAdjust (p);}
                  break; 

                 }
        if (GetState () != QUERY_REP )
           {  
                AddEpcHeader (packet,m_header); 
                if (GetState () != SELECT ) { m_duration += (m_preamble == RFID_PREAMBLE) ? m_conf.GetPreambleDuration () : m_conf.GetFrameSyncDuration (); }
                if (m_first != true) {Send(packet);}
           }
        }
      
    }

    bool ReaderIdentification::Send (Ptr<Packet> packet)
    { 
      NS_LOG_FUNCTION ("packet=" << &packet);
      if (m_first == true ) 
      { 
        SetEquipementState(packet, m_header);
        std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
        m_first = false ;
        SetReceiving (false); 
        GetRfidPhy ()->Send (packet ,m_duration );
        Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT4()), &ReaderIdentification::SetEquipementState , this , packet, m_header);
       }
          else if ( !(m_tag_number == GetTagNumber () && m_eq == READER) ) 
                {
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
                  SetReceiving (false);
                  GetRfidPhy ()->Send (packet ,m_duration );
                  m_id = Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT2()), &ReaderIdentification::SendQueryRepOrAdjust , this , packet);
                }
      
      return true;
    }
 
 void
    ReaderIdentification::AddEpcHeader (Ptr<Packet> packet, uint16_t header)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", header=" << header);
      EpcHeader eh;
      eh.SetHeader(header);
      packet->AddHeader(eh);
    }

    uint16_t
    ReaderIdentification::RemoveEpcHeader (Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION ("packet=" << &packet);
      EpcHeader eh;
      packet->RemoveHeader(eh);
      return eh.GetHeader();
    }

    void
    ReaderIdentification::Receive(Ptr<Packet> packet)
    { 
      Simulator::Cancel (m_id);   
      m_forwardUp (packet, -1);
      SetEquipementState ( packet, 0);
    }

    void 
    ReaderIdentification::SetReceiving (bool rcv)
    {
      m_rcv = rcv;
    }
    bool 
    ReaderIdentification::GetReceiving (void) const
    {
      return m_rcv;
    }

  }
}

