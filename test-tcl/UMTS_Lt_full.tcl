#            Test topology
#
#                AP(n1)---
#              /           \
#        MN(n0)         Server(n3)
#              \           /
#                AP(n2)---

# Scen1_UMTS_ latency full authentication

#Create a simulator object

set ns [new Simulator]

# dynamic route change
#$ns rtproto DV 

#Open a trace file
set nf [open out.nam w]
set nt [open out.tr w]
$ns namtrace-all $nf 
$ns trace-all $nt

$ns color 1 Red
$ns color 2 Blue

#Define a 'finish' procedure
proc finish {} {
        global ns nf nt
        $ns flush-trace
        close $nf 
    	close $nt
#        exec nam out.nam &
        exit 0
}

set av [lindex $argv 0]

# random number
set MyR1 [new RNG]
$MyR1 seed 0

# range  0 - 5 ms
set r1 [new RandomVariable/Uniform]
$r1 use-rng $MyR1
$r1 set min_ 0.0
$r1 set max_ 0.005

set rd [ expr [ $r1 value ] ]


#Create three nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]


#Connect the nodes with two links

# WLAN access
#$ns duplex-link $n0 $n1 5Mb 10ms DropTail
$ns duplex-link $n1 $n3 10Mb 5ms DropTail

#UMTS access
$ns duplex-link $n0 $n2 0.384Mb 300ms DropTail
$ns duplex-link $n2 $n3 10Mb 5ms DropTail

# EAP ERP agent
set p0 [new Agent/EAP-ERP]
$ns attach-agent $n0 $p0

set p1 [new Agent/EAP-ERP]
$ns attach-agent $n3 $p1

#Connect the two agents
$ns connect $p0 $p1

$p0 set fid_ 1
$p1 set fid_ 2

#Schedule events

$ns at 0.0 "$p0 set cryptJitter_ $rd"
$ns at 0.0 "$p1 set cryptJitter_ $rd"
$ns at 0.0 "$p0 set cryptDelay_ $av"
$ns at 0.0 "$p1 set cryptDelay_ $av"

# EAP-AKA full
$ns at 1.0 "$p1 start"

$ns at 10.0 "finish"

#Run the simulation
$ns run
