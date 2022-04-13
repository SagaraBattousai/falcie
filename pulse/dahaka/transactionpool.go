package dahaka

import (
	"crypto/sha256"
)

type TransactionPool struct {
	*OrderedLinkedList[*PooledTransaction]
	//Could go up to 64 or store as varInt which we'll convert to anyway
	transactionCount uint32
}

func NewTransactionPool() *TransactionPool {
	oll := new(OrderedLinkedList[*PooledTransaction])
	return &TransactionPool{OrderedLinkedList: oll, transactionCount: 0}
}

func (transactionPool *TransactionPool) AddTransaction(t *Transaction) {
	hash := t.Hash()
	pooledTransaction := &PooledTransaction{hash: hash, transaction: t}
	transactionPool.OrderedLinkedList.Add(pooledTransaction)
	transactionPool.transactionCount++
}

func (transactionPool *TransactionPool) MerkelHash() [sha256.Size]byte {
	node := transactionPool.OrderedLinkedList.head
	if node == nil {
		return [sha256.Size]byte{}
	}

	hashCount := transactionPool.transactionCount
	//+1 incase hashcount is odd
	merkelPool := make([][sha256.Size]byte, hashCount, hashCount+1)

	//Already know there are hashCount many nodes but best be safe
	for index := 0; node != nil; index++ {
		merkelPool[index] = node.elem.hash
		node = node.next
	}

	if hashCount%2 == 1 {
		merkelPool = append(merkelPool, merkelPool[hashCount-1])
	}

	var inIndex uint32
	var outIndex uint32

	//Will always be non-zero even
	var lastRowCount uint32 = uint32(len(merkelPool))
	var rowCount uint32 = lastRowCount >> 1

	for rowCount != 0 {
		inIndex = 0
		outIndex = 0

		for ; inIndex < lastRowCount-1; inIndex += 2 {
			outIndex = inIndex >> 1

			merkelPool[outIndex] = CombineMerkelHash(&merkelPool[inIndex],
				&merkelPool[inIndex+1])
		}

		if rowCount%2 == 1 {
			merkelPool[outIndex+1] = merkelPool[inIndex]
		}
		lastRowCount = rowCount
		rowCount >>= 1
	}

	return merkelPool[0]

}

func CombineMerkelHash(hashA, hashB *[sha256.Size]byte) [sha256.Size]byte {
	combinatorPool := make([]byte, sha256.Size<<1)
	copy(combinatorPool[:sha256.Size], (*hashA)[:])
	copy(combinatorPool[sha256.Size:], (*hashB)[:])

	innerHash := sha256.Sum256(combinatorPool)
	return sha256.Sum256(innerHash[:])

}
