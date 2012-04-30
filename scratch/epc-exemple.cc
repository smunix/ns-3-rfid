#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include "ns3/epc-module.h"


NS_LOG_COMPONENT_DEFINE ("EpcExemple");


using namespace ns3; 
/*
static void GenerateTraffic (Ptr<Socket> socket)
{
  
   
      socket->Send (Create<Packet> (0));
      Simulator::Schedule (Seconds(1.0), &GenerateTraffic, 
                           socket);
    
  
}*/


int main (int argc, char *argv[])
{

  NodeContainer c;
  c.Create (2);
  NS_LOG_DEBUG ("***************************Node container created");




  NetDeviceContainer devices;
  Ptr<RfidChannel> channel = CreateObject<RfidChannel>();
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<RfidNetDevice> device = CreateObject<RfidNetDevice>();
      
      Ptr<RfidMacReader> mac = CreateObject<RfidMacReader>();
      Ptr<RfidPhyReader> phy = CreateObject<RfidPhyReader>();

      mac->SetAddress (Mac48Address::Allocate ());

      device->SetMac (mac);
      device->SetPhy (phy);
	
      node->AddDevice (device);
	
	
      devices.Add (device);
      channel->Add (phy);
      NS_LOG_DEBUG ("node= " << node << " created");
    }

  NS_LOG_DEBUG ("****************************Affectation terminée des NetDevice");


  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 10.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  NS_LOG_DEBUG ("****************************Mobilité terminée");

for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
   Ptr<Node> node = *i;
   Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
   node->AggregateObject (factory);
    }


/*
  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  
  Ptr<Socket> recv = Socket::CreateSocket (c.Get (0), tid);
 NS_LOG_DEBUG ("*****************************bind");
  recv->Bind ();
  Ptr<Socket> source = Socket::CreateSocket (c.Get (1), tid);

  NS_LOG_DEBUG ("*****************************socket terminée");*/
   /*Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                  Seconds (1.0), &GenerateTraffic, 
                                  source);*/
  
  //Simulator::Schedule (Seconds(1.0), &PacketSocket::Send);
  PacketSocketAddress socket;
  socket.SetSingleDevice (devices.Get (0)->GetIfIndex ());
  socket.SetPhysicalAddress (devices.Get (1)->GetAddress ());
  socket.SetProtocol (1);
  OnOffHelper onoff ("ns3::PacketSocketFactory", Address (socket));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1.0)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0.0)));
  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
