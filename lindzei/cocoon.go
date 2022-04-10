package main

import (
	"fmt"

	"github.com/sagarabattousai/falcie/pulse/dahaka"
)

func main() {
	a := dahaka.NewBlockchain(dahaka.CreateBasicBlockGenisis, dahaka.BBFactory)
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xff, 0x01}))
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xfe, 0x02}))
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xfd, 0x03}))
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xfc, 0x03}))

	a.ForEach(func(f dahaka.BasicBlock) { fmt.Println(f) })

}
