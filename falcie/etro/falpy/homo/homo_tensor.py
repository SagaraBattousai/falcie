import numpy as np

import tensorflow as tf

class DenseHomoLayer(tf.keras.layers.Layer):
  def __init__(self, num_outputs):
    super(DenseHomoLayer, self).__init__()
    self.num_outputs = num_outputs

  def build(self, input_shape: int):
    self.kernel = self.add_weight("kernel",
                                  shape=[int(input_shape[-1]),
                                         self.num_outputs])
    self.bias = self.add_weight("bias", shape=[self.num_outputs,])

  def call(self, inputs):
    return (inputs @ self.kernel) + self.bias

  def homo_call(self, homo_inputs):
      pass

layer = DenseHomoLayer(10)

print(layer(np.array([[5,4]]).astype(np.float32)))
print(layer(tf.zeros([1,2])))


