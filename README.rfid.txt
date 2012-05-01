[ns-3-rfid] ./test.py -s rfid  -v &> README.rfid.txt

Waf: Entering directory `/media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build'
Waf: Leaving directory `/media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build'
'build' finished successfully (2.081s)

Modules built:
antenna                   aodv                      applications             
bridge                    config-store              core                     
csma                      csma-layout (no Python)   dsdv                     
emu                       energy                    flow-monitor             
internet                  lte                       mesh                     
mobility                  mpi                       netanim                  
network                   nix-vector-routing        olsr                     
point-to-point            point-to-point-layout     propagation              
rfid (no Python)          spectrum                  stats                    
tap-bridge                test (no Python)          tools                    
topology-read             uan                       virtual-net-device (no Python)
visualizer                wifi                      wimax                    

Modules not built:
click                     openflow                 

NS3_ENABLED_MODULES == ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-emu', 'ns3-energy', 'ns3-flow-monitor', 'ns3-internet', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-rfid', 'ns3-spectrum', 'ns3-stats', 'ns3-tap-bridge', 'ns3-test', 'ns3-tools', 'ns3-topology-read', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-visualizer', 'ns3-wifi', 'ns3-wimax']
NS3_MODULE_PATH == ['/usr/lib/gcc/x86_64-pc-linux-gnu/4.7.0-alpha20120421', '/media/seagate6/programming/ns3/ns-3-allinone/nsc/lib', '/media/seagate6/programming/ns3/ns-3-allinone/nsc/linux-2.6.26', '/media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build']
NSC_ENABLED == True
ENABLE_REAL_TIME == True
ENABLE_THREADING == True
ENABLE_EXAMPLES == True
ENABLE_TESTS == True
EXAMPLE_DIRECTORIES == ['energy', 'error-model', 'ipv6', 'matrix-topology', 'mobility', 'naming', 'realtime', 'routing', 'socket', 'stats', 'tcp', 'tutorial', 'udp-client-server', 'udp', 'wireless']
ENABLE_PYTHON_BINDINGS == True
ENABLE_CLICK == False
ENABLE_OPENFLOW == False
APPNAME == ns
BUILD_PROFILE == debug
VERSION == 3-dev
PYTHON == ['/usr/bin/python']
Building: ./waf
os.environ["PYTHONPATH"] == /media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build/bindings/python:/media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build/bindings/python
os.environ["LD_LIBRARY_PATH"] == :/usr/lib/gcc/x86_64-pc-linux-gnu/4.7.0-alpha20120421:/media/seagate6/programming/ns3/ns-3-allinone/nsc/lib:/media/seagate6/programming/ns3/ns-3-allinone/nsc/linux-2.6.26:/media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build
Synchronously execute /media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build/utils/ns3-dev-test-runner-debug --print-test-name-list
Return code =  0
stderr =  
Queue rfid
Launch utils/ns3-dev-test-runner-debug --test-name=rfid --stop-on-failure
Synchronously execute /media/seagate6/programming/ns3/ns-3-allinone/ns-3-rfid/build/utils/ns3-dev-test-runner-debug --test-name=rfid --stop-on-failure --xml --tempdir=testpy-output/2012-05-01-23-46-20-CUT --out=testpy-output/2012-05-01-23-46-20-CUT/rfid.xml 
Return code =  0
stderr =  
returncode = 0
---------- begin standard out ----------
Received : dev=04-02-00:02, pkt=0x6ffd00, proto=1, address=00:00
Received : dev=04-02-00:03, pkt=0x6ffd70, proto=1, address=00:00
Received : dev=04-02-00:01, pkt=0x6ffde0, proto=1, address=00:00
Received : dev=04-02-00:03, pkt=0x6ffe50, proto=1, address=00:00
Received : dev=04-02-00:01, pkt=0x6ffee0, proto=1, address=00:00
Received : dev=04-02-00:02, pkt=0x6fff50, proto=1, address=00:00

---------- begin standard err ----------

---------- end standard err ----------
PASS: TestSuite rfid
1 of 1 tests passed (1 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)
