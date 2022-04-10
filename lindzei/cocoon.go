package main

import (
	"fmt"

	"github.com/sagarabattousai/falcie/pulse/dahaka"
)

func main() {
	a := dahaka.NewBlockchain(dahaka.GenisisFederatedBlock())
	a.AddBlock(dahaka.NewFederatedBlock(0x01))
	a.AddBlock(dahaka.NewFederatedBlock(0x02))
	a.AddBlock(dahaka.NewFederatedBlock(0x03))
	a.AddBlock(dahaka.NewFederatedBlock(0x03))

	a.ForEach(func(f *dahaka.FederatedBlock) { fmt.Println(*f) })

}
