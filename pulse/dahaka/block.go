package dahaka

type Block interface {
	Header() BlockHeader
}

type BlockHeader interface {
	//The Id of the block on the Blockchain (Also called height)
	Id() uint32

	//The timestamp in milliseconds since the Unix Epoch
	//(good for ~ 292 Million years)
	Timestamp() int64

	//Version uint32 //Unused for now

	Prevhash() [32]byte

	Transhash() [32]byte

	Target() uint32

	Nonce() uint32 //Could go back to uint64 but since time etc can change no need

	//Or no args??
	//GenerateHash(prevhash []byte) []byte
}

/*
func Mine[T Block](block T, difficulty cactuar.Cactuar) T {
	difficultyArray := difficulty.As256Bit()
	//Debug:
	var i int = 0
	fmt.Println()

	for bytes.Compare(b.hash, difficultyArray[:]) != -1 {
		b.nonce += 1
		b.hash = b.GenerateHash(b.prevhash)
		//Debug:
		i++
		fmt.Printf("%X,\t %x,\t %d\r", b.hash, difficultyArray, i)
	}
	fmt.Println()

	return b
}
*/
