package menrva

import (
	"fmt"
	"io"
)

type Lzw struct {
	r            io.Reader
	initialWidth int
	currentWidth int
	dictionary   map[[]byte][]byte //Could be generic
}

func NewLzw(r io.Reader, initialWidth int) *Lzw {
	dictionary := make(map[[]byte][]byte)
	var i byte = 0
	for ; i < 256; i++ {

	}
	return &Lzw{r: r,
		initialWidth: initialWidth,
		currentWidth: initialWidth,
		dictionary:   dictionary}
}

func (l *Lzw) Read(b []byte) (int, error) {
	fmt.Println("Yup")
	return 0, nil
}
