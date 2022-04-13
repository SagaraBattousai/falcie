package dahaka

import (
	"bytes"
	"crypto/sha256"
	"fmt"
)

const (
	pooledTransactionPrintf = "{\n" +
		"\tHash: %X\n" +
		"\tTransaction: \t%v" +
		"}"
)

type PooledTransaction struct {
	hash        [sha256.Size]byte
	transaction *Transaction
}

func (pooled *PooledTransaction) String() string {
	return fmt.Sprintf(pooledTransactionPrintf+"\n",
		pooled.hash,
		pooled.transaction)
}

//Must be same type but we check for safty
func (pooled *PooledTransaction) Equal(other Orderable) bool {
	otherPooled, ok := other.(*PooledTransaction)
	if !ok {
		return false
	}
	return pooled.hash == otherPooled.hash
}

func (pooled *PooledTransaction) LessThan(other Orderable) bool {
	otherPooled, ok := other.(*PooledTransaction)
	if !ok {
		return false
	}
	return bytes.Compare(pooled.hash[:], otherPooled.hash[:]) == -1
}

func (pooled *PooledTransaction) GreaterThan(other Orderable) bool {
	otherPooled, ok := other.(*PooledTransaction)
	if !ok {
		return false
	}
	return bytes.Compare(pooled.hash[:], otherPooled.hash[:]) == 1
}
