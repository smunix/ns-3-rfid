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
 * Author: ABID AYOUB <abid.ayoub@gmail.com>
 */

#include "ns3/log.h"
#include "rfid-configuration.h"
#include "rfid-identification.h"
#include <algorithm>


NS_LOG_COMPONENT_DEFINE("RfidConfiguration");

namespace ns3
{

    NS_OBJECT_ENSURE_REGISTERED(RfidConfiguration);

    TypeId
    RfidConfiguration::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::RfidConfiguration")
        .SetParent<Object>()
;
      return tid;
    }
    RfidConfiguration::RfidConfiguration (void)
            :m_data0 (25.0),
             m_x (1.0)

    {
      NS_LOG_FUNCTION_NOARGS();
    }
    RfidConfiguration::~RfidConfiguration (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    } 

      void
      RfidConfiguration::SetData0 (double data0)
      {
        m_data0 = data0;
      } 
      double 
      RfidConfiguration::GetData0 (void) const
      {
        return ( m_data0 >= 6.25 && m_data0 <= 25) ? m_data0 : 25 ;
      }

      void 
      RfidConfiguration::SetX (double x)
      {
        m_x = x ;
      }
      double 
      RfidConfiguration::GetX (void) const 
      {
        return ( m_x >= 0.5 && m_x <= 1.0 ) ? m_x : 1.0 ;
      }

      double 
      RfidConfiguration::GetDelimiter (void) const 
      {
        return 12.5;
      }
      double 
      RfidConfiguration::GetData1 (void) const 
      {
        return GetData0 () * ( 1 + GetX ());   
      }
      double 
      RfidConfiguration::GetRTcal (void) const 
      {
        return (GetData0 () + GetData1 ());
      } 
      double 
      RfidConfiguration::GetTRcal (void) const 
      {
        return (GetRTcal () * 2);
      }
      double 
      RfidConfiguration::GetPreambleDuration (void) const 
      {
        return ( GetDelimiter () + GetData0 () + GetRTcal () + GetTRcal () );
      }
      double 
      RfidConfiguration::GetFrameSyncDuration (void) const  
      {
        return ( GetDelimiter () + GetData0 () + GetRTcal () );
      }      
      void 
      RfidConfiguration::SetReaderFrequency (double readerFrequency)
      {
        m_readerFrequency = readerFrequency;
      }
      double 
      RfidConfiguration::GetReaderFrequency (void) const 
      {
        return m_readerFrequency;
      }

      void 
      RfidConfiguration::SetTagFrequency (double tagFrequency)
      {
        m_tagFrequency = tagFrequency;
      }
      double 
      RfidConfiguration::GetTagFrequency (void) const 
      {
        return m_tagFrequency;
      }

      void 
      RfidConfiguration::SetDR (double dr)
      {
        m_dr = dr ;
      }
      double 
      RfidConfiguration::GetDR (void) const 
      {
        return m_dr;
      }
      void 
      RfidConfiguration::SetM (double m)
      {
        m_m = m ;
      }
      double 
      RfidConfiguration::GetM (void) const 
      {
        return m_m;
      }

      double 
      RfidConfiguration::GetBLF (void) const 
      {
        return GetDR () / GetTRcal () ;
      }
      double 
      RfidConfiguration::GetTpri (void) const
      {
        return 1 / GetBLF () ;
      }
      double 
      RfidConfiguration::GetT1 (void) const 
      {
        return std::max ( GetRTcal () , 10 * GetTpri () );
      }

      void 
      RfidConfiguration::SetT2 (double t2)
      {
        m_t2 = t2 ;
      }
      double 
      RfidConfiguration::GetT2 (void) const
      {
        return ( m_t2 >= 3*GetTpri () && m_t2 <= 20*GetTpri () ) ? m_t2 : 5*GetTpri () ;
      }
      void 
      RfidConfiguration::SetT3 (double t3)
      {
        m_t3 = t3 ;
      }
      double 
      RfidConfiguration::GetT3 (void) const
      {
        return ( m_t3 >= 0 ) ? m_t3 : 0 ;
      }
      void 
      RfidConfiguration::SetT4 (double t4)
      {
        m_t4 = t4 ;
      }
      double 
      RfidConfiguration::GetT4 (void) const
      {
        return ( m_t4 >= (GetT1 () + GetT3 ()) && m_t4 >= 2*GetRTcal ()) ? m_t4 : std::max (GetT1 () + GetT3 () ,  2*GetRTcal () ) ;
      }

      double 
      RfidConfiguration::GetTagRate (void) const 
      {
        return GetBLF () / GetM () ;
      }
      double 
      RfidConfiguration::GetReaderRate (void) const 
      {
        return 0;
      }
      
      double 
      RfidConfiguration::GetTagPreamble (int trext) const
      { 
        if (GetM () == 1 )
        {
          if (trext == 0 )
             return ( GetData1 () * 4 + GetData0 () * 3 );
            else return ( GetData1 () * 4 + GetData0 () * 15 );
        }
          else 
           {
             if (trext == 0 )
               return ( GetData1 () * 4 + GetData0 () * 2 + 4 * GetM () / GetBLF () );
              else return ( GetData1 () * 4 + GetData0 () * 2 + 16 * GetM () / GetBLF () );
           }
       }
      double 
      RfidConfiguration::GetTagEndSignaling (void) const
      {
        return GetData0 () + GetData1 ();
      }

      uint16_t 
      RfidConfiguration::ModulationToHeader ( int m)
      {
        switch (m)
        {
           case (1):
              return 0x0;
           case (2):
              return 0x1;
           case (4):
              return 0x2;
           default:
              return 0x3;
        }
      }
      int 
      RfidConfiguration::HeaderToModulation (uint16_t  header)
      {
        switch (header)
        {
           case (0x0):
              return 1;
           case (0x1):
              return 2;
           case (0x2):
              return 4;
           default:
              return 8;
        }
      }
      uint16_t  
      RfidConfiguration::RatioToHeader ( double dr)
      {
        if (dr == 8.0) 
          return 0x0;
        else return 0x1;
      }
      double  
      RfidConfiguration::HeaderToRatio ( uint16_t header)
      {
        if ( header == 0x0)
          return 8.0;
        else return 64/3; 
      }

}

