import math
import numpy as np

def padImage(x, paddingX, paddingY):
    h, w = x.shape

    paddedArr = np.zeros((paddingY * 2 + h, paddingX * 2 + w), dtype=x.dtype)
    paddedArr[paddingY:paddingY + h, paddingX:paddingX + w] = x

    return paddedArr

def conv2d(x, kernel):
    kheight, kwidth = kernel.shape
    h, w = x.shape

    paddingY = math.floor(kheight / 2)
    paddingX =  math.floor(kwidth / 2)

    paddedArr = padImage(x, paddingX, paddingY)

    out = np.zeros_like(x)

    for i in range(paddingY, paddingY + h):
        for j in range(paddingX, paddingX + w):
            out[i-paddingY, j-paddingX] = np.sum(
                    paddedArr[i-paddingY:i+paddingY+1, j-paddingX:j+paddingX+1]
                    * kernel)
    return out






