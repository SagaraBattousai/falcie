package main

import (
	"fmt"

	"github.com/sagarabattousai/falcie/pulse/anima"
	"github.com/sagarabattousai/falcie/pulse/dahaka"
)

const (
	neta             float32 = 0.001
	epsilon          float32 = 0.001
	TrainingDataSize         = 20
)

func getTrainingData() ([]float32, []float32) {
	x := make([]float32, TrainingDataSize)
	y := make([]float32, TrainingDataSize)

	var acc float32 = -7.0

	for i := 0; i < TrainingDataSize; i++ {
		x[i] = acc
		//x³ - 8x² - 7x + 8
		//y[i] = acc*(acc*(acc-8)-7) + 8

		//8x² - 7x + 8
		// y[i] = acc*((8*acc)-7) + 8

		//-7x + 8
		y[i] = -7*acc + 8
		acc = (-7.0 + (1.5 * float32(i+1))) + (acc / 2.0)
	}

	return x, y
}

func trainNonFederated(x, y []float32) *anima.BasicRegression {

	reg := anima.NewBasicRegression()

	errsig, its := anima.BasicRegresionTrain(reg, x, y, neta, 0.001, 5000)

	fmt.Printf("Non federated model trained in %d steps and ended with an "+
		"error of %f\n\n", its, errsig)

	return reg
}

func main() {

	x, y := getTrainingData()
	reg := trainNonFederated(x, y)

	firstPartition := int(TrainingDataSize / 2)
	secondPartition := TrainingDataSize - firstPartition

	x1 := make([]float32, firstPartition)
	y1 := make([]float32, firstPartition)

	x2 := make([]float32, secondPartition)
	y2 := make([]float32, secondPartition)

	var j int
	//slower than slice arithmetic but more evenly spread (Cheating :D)
	for i := 0; i < len(x1); i++ {
		j = i << 1
		x1[i] = x[j]
		y1[i] = y[j]

		x2[i] = x[j+1]
		y2[i] = y[j+1]
	}

	fReg1 := anima.NewBasicRegression()
	fReg2 := anima.NewBasicRegression()

	a := dahaka.NewBlockchain(dahaka.GenisisFederatedBlock())

	var blockWithAnUpdate *dahaka.FederatedBlock
	var delta1, delta2 []float32
	var c1, c2 int

	for i := 0; i < 500; i++ {
		blockWithAnUpdate = dahaka.NewFederatedBlock(0xFF)

		delta1, c1 = anima.BasicRegresionFederationLocalUpdate(fReg1, x1, y1)
		delta2, c2 = anima.BasicRegresionFederationLocalUpdate(fReg2, x2, y2)

		blockWithAnUpdate.AddLocalUpdate(delta1, c1)
		blockWithAnUpdate.AddLocalUpdate(delta2, c2)
		a.AddBlock(blockWithAnUpdate)

		globalUpdate := a.GetLast().GlobalUpdate

		fReg1.Update(globalUpdate[0], globalUpdate[1], neta)
		fReg2.Update(globalUpdate[0], globalUpdate[1], neta)

	}

	a.ForEach(func(f *dahaka.FederatedBlock) { fmt.Println(*f) })

	fmt.Println(a.Validate())

	_, errsig := anima.BasicRegressionForwardPass(fReg1, x, y)
	fmt.Println("****************", errsig)

	o1, e1 := anima.BasicRegressionForwardPass(fReg1, []float32{-27.0},
		[]float32{197.0})

	o2, e2 := anima.BasicRegressionForwardPass(fReg2, []float32{-27.0},
		[]float32{197.0})

	op, ep := anima.BasicRegressionForwardPass(reg, []float32{-27.0},
		[]float32{197.0})

	fmt.Printf("******\n%f, %f\n%f, %f\n%f, %f\n********\n",
		o1[0], e1, o2[0], e2, op[0], ep)

	// tp := dahaka.NewTransactionPool()
	// tp.AddTransaction(dahaka.NewTransaction([20]byte{0x34, 0x65}, [20]byte{}, 5))
	// tp.AddTransaction(dahaka.NewTransaction([20]byte{0xAB, 0xCD, 0xEF, 0x07}, [20]byte{}, 20))
	// tp.AddTransaction(dahaka.NewTransaction([20]byte{0x07, 0xEF, 0xCD, 0xAB}, [20]byte{0x07, 0xDF, 0xAC, 0xBE}, 37))
	// tp.ForEach(func(f *dahaka.PooledTransaction) { fmt.Println(f) })

	// fmt.Printf("Actual:\n\t%X\n\n******\n", tp.MerkelHash())
}
