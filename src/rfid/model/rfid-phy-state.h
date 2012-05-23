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
#ifndef RFID_PHY_STATE_H
#define RFID_PHY_STATE_H

#include "ns3/object.h"
#include "ns3/simulator.h"


namespace ns3 {


class RfidPhyState : public Object
{
public:
  enum State
  {
    /**
     * The PHY layer is IDLE.
     */
    IDLE,
    /**
     * The PHY layer is sending a packet.
     */
    TX,
    /**
     * The PHY layer is receiving a packet.
     */
    RX
  };
public:
  static TypeId GetTypeId (void);

  RfidPhyState ();
  virtual ~RfidPhyState ();

  bool IsStateIdle (void);
  bool IsStateRx (void);
  bool IsStateTx (void);
  enum RfidPhyState::State GetState (void);
  void SwitchToTx (Time txDuration);
  void SwitchToRx (Time rxDuration);

private:
  Time m_endTx;
  Time m_endRx;
  Time m_startTx;
  Time m_startRx;
};

} // namespace ns3

#endif /* RFID_PHY_STATE_H */
