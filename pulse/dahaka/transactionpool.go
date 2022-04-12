package dahaka

const (
	transactionPoolChainSize = 32
)

type TransactionPool struct {
	*Chain[*Transaction]
}

func NewTransactionPool() *TransactionPool {
	chain := NewChain[*Transaction](transactionPoolChainSize)
	return &TransactionPool{Chain: chain}
}

func (transactionPool *TransactionPool) MerkelHash() {

}
