package dahaka

import (
	"crypto/sha256"
	"fmt"

	"github.com/sagarabattousai/falcie/pulse/cactuar"
	"golang.org/x/crypto/ripemd160"
)

const (
	transactionPrintf = "{\n" +
		"\tFrom Address: %X\n" +
		"\tTo Address:   %X\n" +
		"\tAmount:       %d\n" +
		"}"
)

type Transaction struct {
	from   [ripemd160.Size]byte
	to     [ripemd160.Size]byte
	amount int32
}

func NewTransaction(from, to [ripemd160.Size]byte, amount int32) *Transaction {
	return &Transaction{from: from, to: to, amount: amount}
}

//RIPEMD160 of SHA256 Hash
//SHA256 of SHA256 Hash
func (transaction *Transaction) Hash() [sha256.Size]byte {
	hash := sha256.New()

	//Encode not casting using unsafe ptr as MUST be little endian
	amountBytes := cactuar.Encode32(transaction.amount)

	hash.Write(transaction.from[:])
	hash.Write(transaction.to[:])
	hash.Write(amountBytes[:])

	return sha256.Sum256(hash.Sum(nil))

}

func (t *Transaction) From() [20]byte {
	return t.from
}

func (t *Transaction) To() [20]byte {
	return t.to
}

func (t *Transaction) Amount() int32 {
	return t.amount
}

func (transaction *Transaction) String() string {
	return fmt.Sprintf(transactionPrintf+"\n",
		transaction.from,
		transaction.to,
		transaction.amount)
}
