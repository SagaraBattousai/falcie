package com.calotechnologies.faldroid.dataset.cifar10

import java.nio.FloatBuffer

enum class Cifar10Classes {
    Airplane,
    Automobile,
    Bird,
    Cat,
    Deer,
    Dog,
    Frog,
    Horse,
    Ship,
    Truck;

    companion object {
        private val ordinalMap: Array<Cifar10Classes> by lazy { Cifar10Classes.values() }

        //Pre: probabilities must be of length 10 exactly
        fun mapProbabilityToClass(probabilities: FloatBuffer): String {
            var max = probabilities[0]
            var index = 0

            for (i in 1..9) {
                if (max < probabilities[i]) {
                    max = probabilities[i]
                    index = i
                }
            }
            return ordinalMap[index].name
        }
        fun labelToClass(label: Long): String = ordinalMap[label.toInt()].name
    }


}