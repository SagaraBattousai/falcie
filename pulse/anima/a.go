package anima

import (
	"math"
	"math/rand"

	"golang.org/x/exp/constraints"
)

type Numeric interface {
	constraints.Integer | constraints.Float
}

type BasicRegression struct {
	Bias    [2]float32
	Weights [2]float32
}

func NewBasicRegression() *BasicRegression {
	var w [2]float32
	w[0] = rand.Float32()
	w[1] = rand.Float32()
	return &BasicRegression{Bias: [2]float32{0.0, 0.0}, Weights: w}
}

func BasicRegressionForwardPass[T, U Numeric](br *BasicRegression,
	x []U, y []T) ([]T, float32) {
	out := make([]T, len(y))
	var errsig float32
	var tmp float32
	for i, v := range x {
		tmp = float32(v)*br.Weights[0] + br.Bias[0] + float32(v)*br.Weights[1] + br.Bias[1]
		out[i] = T(tmp)
		errsig += float32(0.5 * math.Pow(float64(tmp-float32(y[i])), 2))
	}

	errsig /= float32(len(y))

	return out, errsig
}

func BasicRegressionFullPass[T, U Numeric](br *BasicRegression,
	x []U, y []T) (float32, float32, float32) {
	out, errsig := BasicRegressionForwardPass(br, x, y)

	var dw float32
	var db float32
	var tmp float32
	for i, v := range y {
		tmp = float32(out[i] - v)
		dw += float32(x[i]) * tmp
		db += tmp
	}
	dw /= float32(len(y))
	db /= float32(len(y))

	return dw, db, errsig
}

//
func (br *BasicRegression) Update(dw, db, neta float32) {

	//Ummmm is dw (and db) a vector / should it be ?? I kinda think so but ....

	br.Bias[0] = br.Bias[0] - neta*db
	br.Bias[1] = br.Bias[1] - neta*db

	br.Weights[0] = br.Weights[0] - neta*dw
	br.Weights[1] = br.Weights[1] - neta*dw

}

func BasicRegresionTrain[T, U Numeric](br *BasicRegression,
	x []U, y []T, neta, epsilon float32, maxiter int) (float32, int) {

	var dw, db, errsig float32
	var i int = 0
	for ; i < maxiter; i++ {
		dw, db, errsig = BasicRegressionFullPass(br, x, y)
		if errsig < epsilon {
			break
		}
		br.Update(dw, db, neta)
	}
	return errsig, i
}

func BasicRegresionFederationLocalUpdate[T, U Numeric](br *BasicRegression,
	x []U, y []T) ([]float32, int) {

	dw, db, _ := BasicRegressionFullPass(br, x, y)

	return []float32{dw, db}, len(y)
}
