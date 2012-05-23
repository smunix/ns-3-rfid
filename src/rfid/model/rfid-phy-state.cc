/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 ABID
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
#include "rfid-phy-state.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <iostream>


NS_LOG_COMPONENT_DEFINE ("RfidPhyState");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RfidPhyState);

TypeId
RfidPhyState::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidPhyState")
    .SetParent<Object> ()
    .AddConstructor<RfidPhyState> ()
  ;
  return tid;
}

RfidPhyState::RfidPhyState ()
  : 
    m_endTx (Seconds (0)),
    m_endRx (Seconds (0))

{
  NS_LOG_FUNCTION (this);
}

RfidPhyState::~RfidPhyState ()
{
  
}

bool
RfidPhyState::IsStateIdle (void)
{
  return (GetState () == IDLE);
}

bool
RfidPhyState::IsStateRx (void)
{
  return (GetState () == RX);
}

bool
RfidPhyState::IsStateTx (void)
{
  return (GetState () == TX);
}

enum RfidPhyState::State
RfidPhyState::GetState (void)
{
  if (m_endTx > Simulator::Now ())
    {
      return TX;
    }
  else if (m_endRx > Simulator::Now ())
    {
      return RX;
    }
  else
    {
      return IDLE;
    }
}


void
RfidPhyState::SwitchToTx (Time txDuration)
{
  m_startTx = Simulator::Now ();
  m_endTx = m_startTx + txDuration;
  NS_ASSERT (IsStateTx ());
}

void
RfidPhyState::SwitchToRx (Time rxDuration)
{
  m_startRx = Simulator::Now ();
  m_endRx = m_startRx + rxDuration;
  NS_ASSERT (IsStateRx ());
}



} // namespace ns3
