package atomos

import (
	"errors"
	"fmt"
	"net"
	"runtime"
)

type IPv4 struct{}
type IPv6 struct{}

// type IPProtocol interface {
// 	IPv4 | IPv6
// }

type WindowsNetworkInterfaceError struct{}
type LinuxNetworkInterfaceError struct{}

func (err WindowsNetworkInterfaceError) Error() string {
	//TODO: Reword this to eplain the issues list/location etc
	return "Neither Ethernet Nor Wifi interfaces worked. Please run " +
		"\"netsh interface ip show interfaces\" in powershell followed by " +
		"\"netsh interface show interface\" and post the output to " +
		"the issues list."
}

func (err LinuxNetworkInterfaceError) Error() string {
	return "Neither interfaces with  index 1 (which should be Ethernet) Nor " +
		"index 2 (which is hopefully Wifi) worked. Please post any issues and " +
		"hopefully we can fix it soon"
}

//MAy want to do a double check that (callback) ip is on said interface
func GetNetworkInterface() (*net.Interface, error) {
	var en0 *net.Interface
	var err error

	switch os := runtime.GOOS; os {
	case "windows":
		en0, err = net.InterfaceByName("Ethernet") // index 15, is that universal?
		if err != nil {
			en0, err = net.InterfaceByName("WiFi") // index 10, is that universal?
			if err != nil {
				return nil, WindowsNetworkInterfaceError{}
			}
		}
	case "linux":
		//Should probbly check index Interfaces to not be loopback /lo
		en0, err = net.InterfaceByIndex(2) // Ethernet I hope, is that universal?
		if err != nil {
			en0, err = net.InterfaceByIndex(3) // WiFi I hope, is that universal?
			if err != nil {
				return nil, LinuxNetworkInterfaceError{}
			}
		}
	default:
		return nil, fmt.Errorf("Getting the local address of system running on " +
			"%q is unsupported so far")
	}
	return en0, nil
}

/*
func getWindowsInterface() (*net.Interface, error) {
	en0, err := net.InterfaceByName("Ethernet") // index 15, is that universal?
	if err != nil {
		en0, err = net.InterfaceByName("WiFi") // index 10, is that universal?
		if err != nil {
			return nil, errors.New(err.Error() + ". Neither Ethernet Nor Wifi" +
				"interfaces worked; try running " +
				"\"netsh interface ip show interfaces\" in powershell " +
				"followed by \"netsh interface show interface\" and " +
				"post the output to the issues list.")
		} //TODO: Reword this to eplain the issues list/location etc
	}
	//TODO: Check this -> lets check ethernet first as its better for unix,
	// I think but I genuinly have no idea

	return en0

	//TODO: should I check that the interface includes a local ip starting with
	// 192. .......

		for _, a := range addrs {
			if ipnet, ok := a.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
				if ipnet.IP.To4() != nil {
					os.Stdout.WriteString(ipnet.IP.String() + "\n")
				}
			}
		}

		fmt.Println("\n\n\n\n***************\n\n\n\n")

		for _, a := range addrs {
			fmt.Printf("%+v\n", a)
		}

		fmt.Println("\n\n\n\n***************\n\n\n\n")

		inf, _ := net.Interfaces()
		for _, i := range inf {
			fmt.Printf("%+v\n", i)
		}

		fmt.Println(runtime.GOOS)

}*/

//Probaby Not Needed
func GetLocalAddrForInterface(i *net.Interface,
	ip interface{}) (*net.IPNet, error) {
	addresses, err := i.Addrs()
	if err != nil {
		return nil, err
	}
	addrsLength := len(addresses)
	switch ip.(type) {
	case IPv4:
		//v4's seem to be listed last (at least on windows)
		for i := addrsLength - 1; i >= 0; i-- {
			ipNetwork, ok := addresses[i].(*net.IPNet)
			if ok {
				if !ipNetwork.IP.IsLoopback() && ipNetwork.IP.To4() != nil {
					return ipNetwork, nil
				}
			}
		}
		return nil, errors.New("No IPv4 Address found")
	case IPv6:
		for _, addr := range addresses {
			ipNetwork, ok := addr.(*net.IPNet)
			if ok {
				if !ipNetwork.IP.IsLoopback() {
					//removing v4 more important than checking 16 as 4 passes 16 check.
					if ipNetwork.IP.To4() == nil && ipNetwork.IP.To16() != nil {
						return ipNetwork, nil
					}
				}
			}
		}
		return nil, errors.New("No IPv6 Address found")
	default:
		return nil, errors.New("ip must be either IPv4 or IPv6")
	}
}

/*
func checkIP(addr net.Addr, predicate func[T any](in T) bool) string {
	switch v := addr.(type) {
	default:
		return
	}
}
*/
