
import sys
sys.path.insert(0, "c:\\users\\james\\falcie\\build\\etro\\python\\RelWithDebInfo")

import falpy
import numpy as np
import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt

ds_split, info = tfds.load("penguins/processed", 
                            split=["train[:20%]", "train[20%:]"],
                            as_supervised=True, with_info=True)

ds_test = ds_split[0]
ds_train = ds_split[1]

ds_train_batch = ds_train.batch(32)
ds_train_batch2 = ds_train.batch(32)

loss_object = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)
optimizer1 = tf.keras.optimizers.SGD(learning_rate=0.01)
optimizer2 = tf.keras.optimizers.SGD(learning_rate=0.01)

model1 = tf.keras.Sequential([
    tf.keras.layers.Dense(10, activation=tf.nn.relu, input_shape=(4,)),
    tf.keras.layers.Dense(10, activation=tf.nn.relu),
    tf.keras.layers.Dense(3)
    ])

model2 = tf.keras.Sequential([
    tf.keras.layers.Dense(10, activation=tf.nn.relu, input_shape=(4,)),
    tf.keras.layers.Dense(10, activation=tf.nn.relu),
    tf.keras.layers.Dense(3)
    ])


print("SO:", np.all(model1.get_weights()[0] == model1.trainable_variables[0]))



def loss(model, x, y, training):
  y_ = model(x, training=training)

  return loss_object(y_true=y, y_pred=y_)

def grad(model, inputs, targets):
  with tf.GradientTape() as tape:
    loss_value = loss(model, inputs, targets, training=True)
  return loss_value, tape.gradient(loss_value, model.trainable_variables)

train_loss_results = []
train_accuracy_results = []

num_epochs = 201

###################
### Falpy Setup ###
###################

bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
bc = falpy.Blockchain(bb, 32)

nu1 = falpy.NetworkUpdate()
nu2 = falpy.NetworkUpdate()

model1_examples_seen = 0
model2_examples_seen = 0

block = bb.build()

#####################

for epoch in range(num_epochs):
  
  batch = [(x, y) for x, y in ds_train_batch2]
  b1 = batch[:len(batch)//2]
  b2 = batch[len(batch)//2:]
  print(len(ds_train_batch2))
  print(len(b1))
  print(len(b2))
  # for i, (x, y) in enumerate(ds_train_batch):
  for (x, y), (i, j) in b1, b2:
    #print(np.all(batch[i][0] == x.numpy()))
    #print(np.all(batch[i][1] == y.numpy()))
    print("index:", i, end=", ")
  print()

  if epoch % 50 == 0:
    print(epoch)
