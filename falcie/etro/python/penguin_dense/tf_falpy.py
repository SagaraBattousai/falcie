
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


bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
bc = falpy.Blockchain(bb, 32)


for epoch in range(num_epochs):
  epoch_loss_avg1 = tf.keras.metrics.Mean()
  epoch_loss_avg2 = tf.keras.metrics.Mean()
  epoch_accuracy1 = tf.keras.metrics.SparseCategoricalAccuracy()
  epoch_accuracy2 = tf.keras.metrics.SparseCategoricalAccuracy()

  for x, y in ds_train_batch:
    loss_value1, grads1 = grad(model1, x, y)
    loss_value2, grads2 = grad(model2, x, y)

    optimizer1.apply_gradients(zip(grads1, model1.trainable_variables))
    optimizer2.apply_gradients(zip(grads2, model2.trainable_variables))

    epoch_loss_avg1.update_state(loss_value1)
    epoch_loss_avg2.update_state(loss_value2)

    epoch_accuracy1.update_state(y, model1(x, training=True))
    epoch_accuracy2.update_state(y, model2(x, training=True))


    #For now lets cheat and assume every variable is trainable
    #Also may not be a cheat since it is possible that 
    #if all not trainable vars are static then it will average to itself
    trainables1 = model1.get_weights() #trainable_weights
    trainables2 = model2.get_weights() #trainable_weights

    ##############################################
    #Falpy Part

    b = bb.build()
    nu1 = falpy.NetworkUpdate()
    nu2 = falpy.NetworkUpdate()

    for t1 in trainables1:
      nu1.add_weight(falpy.Weights(t1))

    for t2 in trainables2:
      nu2.add_weight(falpy.Weights(t2))
    

    nu1.set_examples_seen(len(x))
    nu2.set_examples_seen(len(x))
    b.add_local_update(nu1)
    b.add_local_update(nu2)
    bc.add(b)
    last = bc.get_last()
    gu = last.get_global_update()
    
    l = gu.get_weights()

    ws = [w.array for w in l]

    for i, tw in enumerate(model1.trainable_weights):
      tw = ws[i]

    model1.set_weights(ws)
    model2.set_weights(ws)

    
    #trainables[5].assign(np.array([1,2,3]), read_value=False)

    #exit(0)

  train_loss_results.append(epoch_loss_avg1.result())
  train_accuracy_results.append(epoch_accuracy1.result())

  print("epoch:", epoch, end="\r")

  if epoch % 50 == 0:
    print("Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format
          (epoch,
           epoch_loss_avg1.result(),
           epoch_accuracy1.result()))
