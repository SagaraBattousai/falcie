package com.calotechnologies.faldroid.utils

import android.graphics.Bitmap
import android.graphics.Bitmap.createBitmap
import java.nio.FloatBuffer
import java.nio.IntBuffer

private const val NUMBER_OF_CHANNELS = 3
private const val SOLID_ALPHA: Int = -16777216 //AKA 0xFF000000 but since we have to use signed

fun bitmapFromFloatBuffer(
    pixels: FloatBuffer, width: Int,
    hieght: Int, isNormalized: Boolean = false
): Bitmap {
    val bitmap = createBitmap(width, hieght, Bitmap.Config.ARGB_8888)

    val numPixels = width * hieght
    val img = IntBuffer.allocate(numPixels)

    //Cant use absolute indexing as buffer may have non zero position (i.e. for batch)
    //But lets not move the position by doing this :)
    val pos: Int = pixels.position()
    for (pixelCount in 0 until numPixels * NUMBER_OF_CHANNELS step NUMBER_OF_CHANNELS) {
        val argb = SOLID_ALPHA or
                   ((pixels[pos + pixelCount + 2].toInt() and 0xFF) shl 16) or
                   ((pixels[pos + pixelCount + 1].toInt() and 0xFF) shl 8) or
                   (pixels[pos + pixelCount].toInt() and 0xFF)

        img.put(argb)
    }
    img.rewind()
    bitmap.copyPixelsFromBuffer(img)
    return bitmap
}