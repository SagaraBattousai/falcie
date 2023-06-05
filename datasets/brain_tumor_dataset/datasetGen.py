import os
import numpy as np
# from typing import Union
import PIL.Image
import matplotlib.pyplot as plt

TOTAL_IMAGES = 253
TEST_IMAGES = 50
NO_TEST_IMAGES = 20
TRAIN_IMAGES = 203

NEW_IMAGE_SIZE = 224

def square_pad_array(image: np.ndarray) -> np.ndarray:
  (l, r, t, b) = (0,0,0,0)
  image_shape = image.shape
  (h, w) = (image_shape[0], image_shape[1])
  if h > w:
    diff = h - w
    l = diff // 2
    r = diff - l
  elif h < w:
    diff = w - h
    t = diff // 2
    b = diff - t
  else:
    return image

  image = np.pad(image, ((t, b), (l, r), (0, 0)), constant_values=0)
  return image

def resizeKeepingRatio(image: PIL.Image.Image, size: int) -> PIL.Image.Image:
    (width, height) = (image.width, image.height)
    (rwidth, rheight) = (width, height)
    
    if width > height:
        rwidth = size
        rheight = int(height * size / width) 
    else:
        rheight = size
        rwidth = int(width * size / height)
        
    return image.resize((rwidth, rheight))

def formatImageFile(imgFilename: str, size: int) -> np.ndarray:
    img = PIL.Image.open(imgFilename)
    img = resizeKeepingRatio(img, size)

    imgArr = np.asarray(img)
    img_shape = imgArr.shape
    (h, w) = (img_shape[0], img_shape[1])
    #maybe set image format to uint8?
    imgArr = square_pad_array(imgArr.reshape(h, w, -1))
    return imgArr[:,:,0]

def writeImageFile(file, imgFilename: str, size: int):
    for row in formatImageFile(imgFilename, size):
        for pixel in row:
            file.write((int(pixel)).to_bytes())

def genDataset():
    noImages = os.listdir("no")
    noImageCount = len(noImages) # 98
    
    yesImages = os.listdir("yes")
    yesImageCount = len(yesImages) # 155
    
    j = 0
    i = 0
    k = 0
    
    testImages = []
    trainImages = []

    testfile = open("TestBatch.bin", "wb")
    
    while k < TEST_IMAGES:
        if j < NO_TEST_IMAGES and k % 2 ==0:
            testfile.write((0).to_bytes())
            writeImageFile(testfile, f"no/{noImages[j]}", NEW_IMAGE_SIZE)
            j += 1
        else:
            testfile.write((1).to_bytes())
            writeImageFile(testfile, f"yes/{yesImages[j]}", NEW_IMAGE_SIZE)
            i += 1
        k += 1

    testfile.close()
    

    trainfile = open("TrainBatch.bin", "wb")

    while k < TOTAL_IMAGES:
        if j < len(noImages) and k % 2 == 0:
            trainfile.write((0).to_bytes())
            writeImageFile(trainfile, f"no/{noImages[j]}", NEW_IMAGE_SIZE)
            j += 1
        else:
            trainfile.write((1).to_bytes())
            writeImageFile(trainfile, f"yes/{yesImages[i]}", NEW_IMAGE_SIZE)
            i += 1
        k += 1

    trainfile.close()

def read_test():
    f = open("TestBatch.bin", "rb")
    f.seek((224*224+1)*9)
    cla = f.read(1)
    data = f.read(224*224*1)
    img = np.array([data[i] for i in range(224*224)])

    img = img.reshape(224,224)

    print(cla[0])
    plt.imshow(img)
    plt.show()

if __name__ == "__main__":
    genDataset() 


