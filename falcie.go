package main

import (
	/*"io"
	"os"
	"strings"

	"github.com/sagarabattousai/falcie/menrva"*/
	"fmt"

	"github.com/sagarabattousai/falcie/dahaka"
	//"github.com/sagarabattousai/bits/bitstring"
)

func main() {
	/*s := strings.NewReader("Hi ya")
	r := menrva.NewLzw(s, 2)
	io.Copy(os.Stdout, r)*/

	// b := bitstring.New()
	// fmt.Println(b.BitCount())
	// bitstring.T()

	//node, _ := atomos.NewP2PNode()

	//fmt.Printf("%+v\n", node)

	a := dahaka.NewBlockchain(dahaka.CreateBasicBlockGenisis, dahaka.BBFactory)
	//b := a.GetBlock(0)
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xAB}))
	a.AddBlock(dahaka.NewBasicBlock([]byte{0xAB, 0xCD}))

	fmt.Println(a.Validate())
	var f = func(b dahaka.BasicBlock) { fmt.Println(b) }
	a.ForEach(f)
	var x float32 = 0.1234565432
	b := dahaka.EncodeFloat32(x)
	y := dahaka.DecodeFloat32[float32](b)
	fmt.Printf("%f\n%b\n%f\n\n", x, b, y)

}
