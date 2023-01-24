
import sys
sys.path.insert(0, "c:\\users\\james\\falcie\\build\\etro\\python\\RelWithDebInfo")

import falpy
import numpy as np
import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt
#TMP DEBUG
import functools
any_nan = lambda acc, elem: acc or (np.any(np.isnan(elem)))
min_max_weight = lambda a, b: (min(a[0], np.amin(b[0])), max(a[1], np.amax(b[1])))
##

ds_split = tfds.load('cifar10', 
                     split=["train[:50%]", "test"],
                     as_supervised=True, with_info=False)

normalization_layer = tf.keras.layers.Rescaling(1./255)

ds_train = ds_split[0].map(lambda x, y: (normalization_layer(x), y))

ds_test = ds_split[1].map(lambda x, y: (normalization_layer(x), y))

ds_train_batch = ds_train.batch(32)

loss_object = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)
optimizer1 = tf.keras.optimizers.SGD(learning_rate=0.01)

model1 = tf.keras.Sequential([
    tf.keras.layers.Conv2D(32, (3,3),
                           activation='relu', input_shape=(32, 32, 3)),
    tf.keras.layers.MaxPooling2D((2,2)),
    tf.keras.layers.Conv2D(64, (3,3), activation='relu'),
    tf.keras.layers.MaxPooling2D((2,2)),
    tf.keras.layers.Conv2D(64, (3,3), activation='relu'),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(10),
    ])

def loss(model, x, y, training):
  y_ = model(x, training=training)
  
  if np.any(np.isnan(y_.numpy())):
    print("Predicted a nan")
    exit()

  return loss_object(y_true=y, y_pred=y_)

def grad(model, inputs,  targets):
  with tf.GradientTape() as tape:
    loss_value = loss(model, inputs, targets, training=True)
  return loss_value, tape.gradient(loss_value, model.trainable_variables)



train_loss_results = []
train_accuracy_results = []

num_epochs = 151

for epoch in range(num_epochs):
  epoch_loss_avg1 = tf.keras.metrics.Mean()
  epoch_loss_avg2 = tf.keras.metrics.Mean()
  epoch_accuracy1 = tf.keras.metrics.SparseCategoricalAccuracy()
  epoch_accuracy2 = tf.keras.metrics.SparseCategoricalAccuracy()

  for x, y in ds_train_batch:
    loss_value1, grads1 = grad(model1, x, y)
    if np.isnan(loss_value1.numpy()):
      print("Model 1's loss is NaN")
      exit()

    if functools.reduce(any_nan, grads1, False):
      print("Gradents for M1 contain NaN's")
      exit()

    optimizer1.apply_gradients(zip(grads1, model1.trainable_variables))
    
    if functools.reduce(any_nan, model1.get_weights(), False):
      print("Model 1 Weights are NaN")
      exit()

    epoch_loss_avg1.update_state(loss_value1)
    epoch_accuracy1.update_state(y, model1(x, training=True))

  train_loss_results.append(epoch_loss_avg1.result())
  train_accuracy_results.append(epoch_accuracy1.result())

  if epoch % 50 == 0:
    print("Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format
          (epoch,
           epoch_loss_avg1.result(),
           epoch_accuracy1.result()))


print("Training Finished:")

test_accuracy = tf.keras.metrics.Accuracy()
ds_test_batch = ds_test.batch(10)

for (x, y) in ds_test_batch:
  logits = model1(x, training=False)
  prediction  = tf.math.argmax(logits, axis=1, output_type=tf.int64)
  test_accuracy(prediction, y)

print("Test set accuracy: {:.3%}".format(test_accuracy.result()))

