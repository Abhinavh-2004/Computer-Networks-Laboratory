set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf 
proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail

#create a udp agent

set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0
$ns attach-agent $n1 $udp0

#create a cbr traffic source and attach it to udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.2
$cbr0 attach-agent $udp0

#cbr is constant bit rate
set null0 [new Agent/Null]
$ns attach-agent $n1 $null0
#null agent is the trafic sink, which is jus the node 1
$ns connect $udp0 $null0

set null1 [new Agent/Null]
$ns attach-agent $n0 $null1
$ns connect $udp0 $null1


$ns at 0.1 "$cbr0 start"
$ns at 0.9 "$cbr0 stop"


$ns at 5.0 "finish"
$ns run





