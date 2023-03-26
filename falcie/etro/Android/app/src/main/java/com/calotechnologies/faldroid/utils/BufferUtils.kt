package com.calotechnologies.faldroid.utils

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import java.nio.DoubleBuffer
import java.nio.IntBuffer
import java.nio.LongBuffer
import java.nio.CharBuffer
import java.nio.ShortBuffer


fun directAllocateNativeFloatBuffer(capacity: Int): FloatBuffer =
    ByteBuffer.allocateDirect(capacity * Float.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asFloatBuffer()

fun directAllocateNativeDoubleBuffer(capacity: Int): DoubleBuffer =
    ByteBuffer.allocateDirect(capacity * Double.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asDoubleBuffer()

fun directAllocateNativeIntBuffer(capacity: Int): IntBuffer =
    ByteBuffer.allocateDirect(capacity * Int.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asIntBuffer()

fun directAllocateNativeLongBuffer(capacity: Int): LongBuffer =
    ByteBuffer.allocateDirect(capacity * Long.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asLongBuffer()

fun directAllocateNativeCharBuffer(capacity: Int): CharBuffer =
    ByteBuffer.allocateDirect(capacity * Char.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asCharBuffer()

fun directAllocateNativeShortBuffer(capacity: Int): ShortBuffer =
    ByteBuffer.allocateDirect(capacity * Short.SIZE_BYTES)
        .order(ByteOrder.nativeOrder())
        .asShortBuffer()