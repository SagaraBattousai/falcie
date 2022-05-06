package atomos

import (
	"net"
	"strconv"

	"golang.org/x/net/ipv4"
)

const (
	BROADCAST_IP_ADDR string = "224.0.0.237" //Do I need adhoc? "244.0.2.237"
	BROADCAST_IP_PORT int    = 37147
)

type P2PNetworkNode interface {
}

type P2PNode struct {
	Interface        *net.Interface
	PacketConnection *ipv4.PacketConn
}

func NewP2PNode() (*P2PNode, error) {
	conn, err := net.ListenPacket("udp", ":"+strconv.Itoa(BROADCAST_IP_PORT))
	en0, err := GetNetworkInterface()
	if err != nil {
		return nil, err
	}
	packetConn := ipv4.NewPacketConn(conn)

	packetConn.JoinGroup(en0, &net.UDPAddr{
		IP:   net.ParseIP(BROADCAST_IP_ADDR),
		Port: BROADCAST_IP_PORT,
	})
	node := &P2PNode{Interface: en0, PacketConnection: packetConn}
	return node, nil
	// for _, v := range addr {
	// 	fmt.Printf("%+v\n\n***\n", v)
	// }
	// b := make([]byte, 1500)
	// for {
	// 	p.ReadFrom(b)
	// 	fmt.Println(string(b))
	// }
}
