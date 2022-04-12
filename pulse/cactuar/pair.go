package cactuar

type Pair[A, B any] struct {
	Fst A
	Snd B
}

func (p Pair[A, B]) Split() (A, B) {
	return p.Fst, p.Snd
}
