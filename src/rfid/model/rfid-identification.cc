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
#include "rfid-identification.h"
#include "rfid-configuration.h"

NS_LOG_COMPONENT_DEFINE("RfidIdentification");

namespace ns3
{
  namespace rfid
  {

    NS_OBJECT_ENSURE_REGISTERED(Identification);

    TypeId
    Identification::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::rfid::Identification").SetParent<Object>();
      return tid;
    }
    Identification::~Identification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    } 

    double
    Identification::GetPacketDuration ( int size, uint32_t packet)
    {
      RfidConfiguration conf;
      int counter = 0;
      double duration = 0;
      while (packet != 0 )
       {
         duration += (packet % 2) ? conf.GetData1 () : conf.GetData0 () ;
         packet /=2;
         counter +=1;
       }    
      if (counter < size ) 
       { 
         duration += (size - counter) * conf.GetData0 ();
       }
      return duration;
    }
    void 
    Identification::SetTagNumber (int tagNumber)
    {
      m_tagNumber = tagNumber;
    }
    int 
    Identification::GetTagNumber (void) const 
    {
      return m_tagNumber;
    }


  }
}

