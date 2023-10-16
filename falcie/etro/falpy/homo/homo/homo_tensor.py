
import numpy as np
import tensorflow as tf

# from homo import cipher as C
import cipher as C

class DenseHomoLayer(tf.keras.layers.Layer):
    def __init__(self, num_outputs):
        super(DenseHomoLayer, self).__init__()
        self.num_outputs = num_outputs

    def build(self, input_shape):
        # self.input_shape = input_shape
        self.kernel = self.add_weight("kernel",
                                      shape=[int(input_shape[-1]),
                                         self.num_outputs])
        self.bias = self.add_weight("bias", shape=[self.num_outputs,])

    def call(self, inputs):
        return (inputs @ self.kernel) + self.bias

    def homo_call(self, homo_inputs: np.array):
        outputs = C.matmul_plain(homo_inputs, self.kernel, self.num_outputs)

        for i in range(len(outputs)):
            outputs[i] = outputs[i].add_plain(np.array([self.bias[i]]))

        return outputs


        #return C.matmul_plain(homo_inputs, self.kernel, self.num_outputs)
        #How on earth did ^ fix it? isnt it the same below???!!!????
        #output = [None] * self.num_outputs
        ##  reshape vector to be safe (i,e flatten)
        #homo_inputs = homo_inputs.reshape(-1)
        ##since vector matrix mul theres no need for outer most loop
        #for i in range(homo_inputs.shape[0]):
        #    val = homo_inputs[i]
        #    print("i:", i, "val:", val)
        #    for j in range(self.num_outputs):
        #        out = output[j]
        #        if out is None:
        #            output[j] = val.mul_plain(np.array([self.kernel[i][j]]))

        #            # does this mod homo_inputs? is ref?
        #            #val.mul_plain(np.array([self.kernel[i][j]])) 
        #            #output[j] = val
        #        else:
        #            output[j] = C.add_cipher(output[j], 
        #                    val.mul_plain(np.array([self.kernel[i][j]])))

        #            #val.mul_plain(np.array([self.kernel[i][j]]))
        #            #output[j] = C.add_cipher(output[j], val)

        ## return output
        #for index, o in enumerate(output):
        #    o.add_plain(np.array([self.bias[index]]))
        #    #needed?
        #    output[index] = o

        #return output

    def plain_call(self, plain_inputs: np.array):
        output = [None] * self.num_outputs
        #  reshape vector to be safe (i,e flatten)
        plain_inputs = plain_inputs.reshape(-1)
        #since vector matrix mul theres no need for outer most loop
        for i in range(plain_inputs.shape[0]):
            val = plain_inputs[i]
            for j in range(self.num_outputs):
                out = output[j]
                if out is None:
                    output[j] = val * self.kernel[i][j]
                    # val.mul_plain(np.array([self.kernel[i][j]]))
                    # output[j] = val
                else:
                    output[j] = output[j] + (val * self.kernel[i][j])
                    # val.mul_plain(np.array([self.kernel[i][j]]))
                    # output[j] = C.add_cipher(output[j], val)

        return np.array(output) + self.bias
        

layer = DenseHomoLayer(10)

print(layer(np.array([[1,0]]).astype(np.float32)))
print(layer(np.array([[1,0]]).astype(np.float32)))


keys = C.CKKSKeys(1, 13, (2 ** 15)) #issue that array size has to be pow 2 :(
I1 = np.array([1])
I2 = np.array([0])

C1 = keys.encrypt(I1)
C2 = keys.encrypt(I2)

ho = layer.homo_call(np.array([C1,C2]))

for x in ho:
    print(keys.decrypt(x, None, True))# * keys.scale)

po = layer.plain_call(np.array([1,0]))

print()
print()

for y in po:
    print(y)

print()
