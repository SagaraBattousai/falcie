package main

import (
	"fmt"

	"github.com/sagarabattousai/falcie/pulse/dahaka"
)

func main() {
	// a := dahaka.NewBlockchain(dahaka.GenisisFederatedBlock())
	// a.AddBlock(dahaka.NewFederatedBlock(0x01))
	// a.AddBlock(dahaka.NewFederatedBlock(0x02))
	// a.AddBlock(dahaka.NewFederatedBlock(0x03))
	// a.AddBlock(dahaka.NewFederatedBlock(0x03))

	// a.ForEach(func(f *dahaka.FederatedBlock) { fmt.Println(*f) })

	// fmt.Println(a.Validate())

	tp := dahaka.NewTransactionPool()
	tp.AddTransaction(dahaka.NewTransaction([20]byte{0x34, 0x65}, [20]byte{}, 5))
	tp.AddTransaction(dahaka.NewTransaction([20]byte{0xAB, 0xCD, 0xEF, 0x07}, [20]byte{}, 20))
	tp.AddTransaction(dahaka.NewTransaction([20]byte{0x07, 0xEF, 0xCD, 0xAB}, [20]byte{0x07, 0xDF, 0xAC, 0xBE}, 37))
	tp.ForEach(func(f *dahaka.PooledTransaction) { fmt.Println(f) })

	fmt.Printf("Actual:\n\t%X\n\n******\n", tp.MerkelHash())
}
