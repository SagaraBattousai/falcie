package com.calotechnologies.faldroid.utils

import android.content.res.AssetFileDescriptor
import android.content.res.AssetManager;
import android.graphics.BitmapFactory
import org.tensorflow.lite.Interpreter
import java.io.FileInputStream
import java.nio.FloatBuffer
import java.nio.IntBuffer
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel
import java.text.Normalizer

fun loadModelFile(assetManager: AssetManager, modelPath: String) : MappedByteBuffer
{
    val fileDesc: AssetFileDescriptor = assetManager.openFd(modelPath)
    val inputStream: FileInputStream = FileInputStream(fileDesc.fileDescriptor)

    val fileChannel: FileChannel = inputStream.channel
    val startOffset: Long = fileDesc.startOffset
    val declaredLength: Long = fileDesc.declaredLength

    val modelFile = fileChannel.map(FileChannel.MapMode.READ_ONLY, startOffset, declaredLength);

    inputStream.close()
    fileDesc.close()

    return modelFile
}

fun loadImage(assetManager: AssetManager, imagePath: String,
              imageWidth: Int, imageHeight: Int, imageChannels: Int = 3,
              normalize: Boolean = false) : FloatBuffer
{
    val bitmap = BitmapFactory.decodeStream(assetManager.open(imagePath))
    val imageArea: Int = imageWidth * imageHeight

    val imageBuffer: FloatBuffer =
        FloatBuffer.allocate(imageArea * imageChannels)

    val readBuffer: IntBuffer =
        IntBuffer.allocate(imageArea) //as 32bit for some reason

    bitmap.copyPixelsToBuffer(readBuffer)

    var r: Int
    var g: Int
    var b: Int

    //To avoid putting the if inside the loop
    if (normalize)
    {
        for (i in 0 until imageArea) {
            b = (readBuffer[i] and 0xFF0000) ushr 16
            g = (readBuffer[i] and 0x00FF00) ushr 8
            r = (readBuffer[i] and 0x0000FF)

            imageBuffer.put(r.toFloat() / 255.0f)
            imageBuffer.put(g.toFloat() / 255.0f)
            imageBuffer.put(b.toFloat() / 255.0f)
        }
    }
    else {
        for (i in 0 until imageArea) {
            b = (readBuffer[i] and 0xFF0000) ushr 16
            g = (readBuffer[i] and 0x00FF00) ushr 8
            r = (readBuffer[i] and 0x0000FF)

            imageBuffer.put(r.toFloat())
            imageBuffer.put(g.toFloat())
            imageBuffer.put(b.toFloat())
        }
    }
    return imageBuffer
}