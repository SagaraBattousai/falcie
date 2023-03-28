package com.calotechnologies.faldroid.buffer

import java.nio.*

//Weird class try and remember why its useful later and why almost all funcs ignore the underlying
//I think this is no longer needed as I no longer need the array :)
class FloatDataBuffer(capacity: Int) : Comparable<FloatBuffer> {

    private val nativeBuffer: ByteBuffer = ByteBuffer.allocateDirect(capacity * Float.SIZE_BYTES)
    .order(ByteOrder.nativeOrder())

    //constructor(capacity: Int) :
            //this(
                //ByteBuffer.allocateDirect(capacity * Float.SIZE_BYTES)
                    //.order(ByteOrder.nativeOrder())
            //)

    /*
    companion object {
        fun withByteCapacity(capacity: Int): FloatDataBuffer =
            FloatDataBuffer(ByteBuffer.allocateDirect(capacity).order(ByteOrder.nativeOrder()))
    }

     */

    val floatBuffer: FloatBuffer = nativeBuffer.asFloatBuffer()

    val hasArray = nativeBuffer.hasArray()

    //Note: Native array requires array Offset to know where array starts!!!!!!
    val array: ByteArray get() = nativeBuffer.array()

    val arrayOffset: Int get() = nativeBuffer.arrayOffset()

    fun clear(): Buffer = nativeBuffer.clear()

    override fun compareTo(that: FloatBuffer): Int = floatBuffer.compareTo(that)

    fun get(dst: FloatArray): FloatBuffer = floatBuffer.get(dst)

    fun get(): Float = floatBuffer.get()

    fun get(dst: FloatArray, offset: Int, length: Int): FloatBuffer =
        floatBuffer.get(dst, offset, length)

    operator fun get(index: Int): Float = floatBuffer.get(index)

    fun position(newPosition: Int): Buffer = floatBuffer.position(newPosition)

    fun put(src: FloatArray, offset: Int, length: Int): FloatBuffer =
        floatBuffer.put(src, offset, length)

    fun putRaw(src: FloatDataBuffer, offset: Int, length: Int): FloatBuffer {

        val byteOffset = (offset * Float.SIZE_BYTES) + src.arrayOffset
        val byteLength = length * Float.SIZE_BYTES

        //TMP CHECK
        val tmp = nativeBuffer.put(src.array, byteOffset, byteLength)

        //check tmp.asFloatBuffer == floatBuffer

        return tmp.asFloatBuffer()
    }

    fun put(src: FloatBuffer): FloatBuffer = floatBuffer.put(src)

    fun put(f: Float): FloatBuffer = floatBuffer.put(f)

    fun put(index: Int, f: Float): FloatBuffer = floatBuffer.put(index, f)

    operator fun set(index: Int, f: Float): FloatBuffer = floatBuffer.put(index, f)

    fun put(src: FloatArray): FloatBuffer = floatBuffer.put(src)

    fun reset(): Buffer = floatBuffer.reset()

    fun rewind(): Buffer = floatBuffer.rewind()

}