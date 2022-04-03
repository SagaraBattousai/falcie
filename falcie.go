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

	a := dahaka.NewBlockchain(dahaka.CreateBasicBlockGenisis)
	b := a.GetBlock(0)
	fmt.Println(b)
	a.AddBlock(b)
	fmt.Println(a.GetBlock(1))
	fmt.Printf("\n\n\n%+v\n\n", a.Head)

}
