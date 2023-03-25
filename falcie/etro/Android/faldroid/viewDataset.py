import numpy as np
import matplotlib.pyplot as plt

IMG_DATA_SIZE = (32 * 32 * 3) + 1
DATA_FILE_SIZE = 10000 * IMG_DATA_SIZE
LABEL_BYTES = 1
PIXELS_PER_CHANNEL = 1024
NUMBER_OF_CHANNELS = 3
PIXELS_PER_IMAGE = PIXELS_PER_CHANNEL * NUMBER_OF_CHANNELS

def clamp(low, val, high):
    if val < low:
        return low
    return val if val < high else high

#File must be at the correct position and be a file
def readImage(file) -> (np.array, int):
    label = list(file.read(1))[0]
    pixels = np.array(list(file.read(PIXELS_PER_IMAGE))).reshape(32,32,3)
    return (pixels, label)

#One indexed index
def viewImage(filename: str, image_index: int):
    index = clamp(1, image_index, 10000) - 1
    file = open(filename, "rb")

    file.seek(index * IMG_DATA_SIZE)

    pixels, label = readImage(file)
    
    file.close()

    print("Label:", label)
    plt.imshow(pixels)
    plt.show()



if __name__ == "__main__":

    viewImage("data_batch_4.rgb", 37)
