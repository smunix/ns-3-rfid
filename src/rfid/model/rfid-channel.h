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

#ifndef RFID_CHANNEL_H_
#define RFID_CHANNEL_H_

#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include <vector>
#include <algorithm>

namespace ns3
{
  class RfidChannel : public Channel
  {
  public:
    static TypeId
    GetTypeId (void);

    virtual
    ~RfidChannel (void);

    /**
     * \returns the number of NetDevices connected to this Channel.
     *
     * This method must be implemented by subclasses.
     */
    virtual uint32_t
    GetNDevices (void) const;
    /**
     * \param i index of NetDevice to retrieve
     * \returns one of the NetDevices connected to this channel.
     *
     * This method must be implemented by subclasses.
     */
    virtual Ptr<NetDevice>
    GetDevice (uint32_t i) const;

    /**
     * \returns the channel number id.
     *
     * The channel number id can be used to tell apart several
     * channels in a simulation.
     */
    virtual uint16_t
    GetChannelNumber (void) const;

    virtual void
    Add (Ptr<class RfidPhy> phy);

    virtual void
    Send (Ptr<class Packet> pkt, Ptr<class RfidPhy> phy);
  private:
    template<typename T, typename R = bool>
    struct Unary : public std::unary_function<Ptr<T>, R>
    {
      typedef std::unary_function<Ptr<T>, R> UF;
      typedef typename UF::result_type result_type;
      typedef typename UF::argument_type argument_type;
      Ptr<T>  m_p;
      void Set (argument_type const &p) { m_p = p; }
      result_type operator() (argument_type const &arg) { return m_p == arg; }
    };

    template<typename T, typename R = void>
    struct Sender : public Unary<T, R>
    {
      typedef Unary<T, R> UF;
      typedef typename UF::result_type result_type;
      typedef typename UF::argument_type argument_type;
      void Set (argument_type const &p, Ptr<Packet> pkt)
      {
        UF::Set (p);
        m_pkt = pkt;
      }
      result_type operator() (argument_type const &arg)
      {
        if (this->m_p != arg)
          {
            Simulator::ScheduleNow (&T::Recv, arg, m_pkt->Copy ());
          }
      }
    private:
      Ptr<Packet> m_pkt;
    };

    template<typename T>
    Unary<T> MakeFinder (Ptr<T> p)
    {
      Unary<T> f;
      f.Set (p);
      return f;
    }
    template<typename T>
    Sender<T> MakeSender (Ptr<T> p, Ptr<Packet> pkt)
    {
      Sender<T> s;
      s.Set (p, pkt);
      return s;
    }
  private:
    template<typename T>
      static T
      MakeUid (void)
      {
        static T val = 0;
        return val++;
      }

    typedef std::vector<Ptr<class RfidPhy> > RfidPhys;
    typedef RfidPhys::iterator RfidPhyIt;

    RfidPhys m_phys;
  };
} // namespace ns3

#endif /* RFID_CHANNEL_H_ */
