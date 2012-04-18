#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"

#include "ns3/epc-module.h"


NS_LOG_COMPONENT_DEFINE ("EpcExemple");


using namespace ns3; 


int main (int argc, char *argv[])
{

  NodeContainer c;
  c.Create (2);





  NetDeviceContainer devices;

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<RfidNetDevice> device = CreateObject<RfidNetDevice>();
      
      Ptr<RfidMacReader> mac = CreateObject<RfidMacReader>();
      Ptr<RfidPhyReader> phy = CreateObject<RfidPhyReader>();
     
      device->SetMac (mac);
      device->SetPhy (phy);

      node->AddDevice (device);

      devices.Add (device);
      NS_LOG_DEBUG ("node=" << node << "created");
    }




  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 10.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);



for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
   Ptr<Node> node = *i;
   Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
   node->AggregateObject (factory);
    }



  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  
  Ptr<Socket> recv = Socket::CreateSocket (c.Get (0), tid);
 
  Ptr<Socket> source = Socket::CreateSocket (c.Get (1), tid);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
