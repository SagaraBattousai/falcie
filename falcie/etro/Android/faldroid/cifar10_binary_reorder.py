
IMG_DATA_SIZE = (32 * 32 * 3) + 1
DATA_FILE_SIZE = 10000 * IMG_DATA_SIZE
LABEL_BYTES = 1
PIXELS_PER_CHANNEL = 1024

def convert_files(inFilenames: [str], outFilenames: [str]):

    for i in range(len(inFilenames)):
        inBinaryFile = open(inFilenames[i], "rb")
        outBinaryFile = open(outFilenames[i], "wb")

        while inBinaryFile.tell() < DATA_FILE_SIZE:

            label = inBinaryFile.read(LABEL_BYTES)
            reds = inBinaryFile.read(PIXELS_PER_CHANNEL)
            greens = inBinaryFile.read(PIXELS_PER_CHANNEL)
            blues = inBinaryFile.read(PIXELS_PER_CHANNEL)
        
            img = bytes([b for pixel in zip(reds, greens, blues) 
                           for b in pixel])

            outBinaryFile.write(label)
            outBinaryFile.write(img)

        inBinaryFile.close()
        outBinaryFile.close()

def convert_file_range(training_indicies: [int], include_test: bool = False):
    baseFileName = "data_batch_{}.{}"

    inFilenames = [baseFileName.format(i, "bin") for i in training_indicies]
    outFilenames = [baseFileName.format(i, "rgb") for i in training_indicies]

    if include_test:
        inFilenames.append("test_batch.bin")
        outFilenames.append("test_batch.rgb")

    convert_files(inFilenames, outFilenames)

if __name__ == "__main__":

    convert_file_range([1,2,3,4,5], True)
