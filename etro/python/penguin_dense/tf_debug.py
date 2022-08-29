
import sys
sys.path.insert(0, "c:\\users\\james\\falcie\\build\\etro\\python\\RelWithDebInfo")

import falpy
import numpy as np
import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt

tf.debugging.experimental.enable_dump_debug_info(
    "./tmp/tfdbg2_logdir",
    tensor_debug_mode="FULL_HEALTH",
    circular_buffer_size=-1)


ds_split, info = tfds.load("penguins/processed", 
                            split=["train[:20%]", "train[20%:]"],
                            as_supervised=True, with_info=True)

ds_test = ds_split[0]
ds_train = ds_split[1]

ds_train_batch = ds_train.batch(32)

loss_object = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)

optimizer1 = tf.keras.optimizers.SGD(learning_rate=0.001)
optimizer2 = tf.keras.optimizers.SGD(learning_rate=0.001)

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



def loss(model, x, y, lo, training):
  y_ = model(x, training=training)

  return lo(y_true=y, y_pred=y_)

def grad(model, inputs,  targets):
  with tf.GradientTape() as tape:
    loss_value = loss(model, inputs, targets, loss_object, training=True)
  return loss_value, tape.gradient(loss_value, model.trainable_variables)

train_loss_results = []
train_accuracy_results = []

num_epochs = 201

###################
### Falpy Setup ###
###################

bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
bc = falpy.Blockchain(bb, 32)

model1_examples_seen = 0
model2_examples_seen = 0

#####################

for epoch in range(num_epochs):
  epoch_loss_avg1 = tf.keras.metrics.Mean()
  epoch_loss_avg2 = tf.keras.metrics.Mean()
  epoch_accuracy1 = tf.keras.metrics.SparseCategoricalAccuracy()
  epoch_accuracy2 = tf.keras.metrics.SparseCategoricalAccuracy()

  batch_split = len(ds_train_batch) // 2
  for i, (x, y) in enumerate(ds_train_batch):
    if i < batch_split:
      loss_value1, grads1 = grad(model1, x, y)
      optimizer1.apply_gradients(zip(grads1, model1.trainable_variables))
    
      epoch_loss_avg1.update_state(loss_value1)
      epoch_accuracy1.update_state(y, model1(x, training=True))

      model1_examples_seen += len(x)

    else:
      loss_value2, grads2 = grad(model2, x, y)
      optimizer2.apply_gradients(zip(grads2, model2.trainable_variables))

      epoch_loss_avg2.update_state(loss_value2)
      epoch_accuracy2.update_state(y, model2(x, training=True))

      model2_examples_seen += len(x)

  train_loss_results.append(epoch_loss_avg1.result())
  train_accuracy_results.append(epoch_accuracy1.result())

  ### Falpy ###
  if epoch % 5 == 0:

    block = bb.build()

    nu1 = falpy.NetworkUpdate()
    nu2 = falpy.NetworkUpdate()


    #For now lets cheat and assume every variable is trainable
    #Also may not be a cheat since it is possible that 
    #if all not trainable vars are static then it will average to itself

    # for m1_trainables in model1.get_weights(): #trainable_weights:
    #   nu1.add_weight(falpy.Weights(m1_trainables))
    # for m2_trainables in model2.get_weights(): #trainable_weights
    #   nu2.add_weight(falpy.Weights(t2))

    #Zip is good here as both models shoul have 
    #the same number of trainable weights.
    for m1_weights, m2_weights in \
        zip(model1.get_weights(), model2.get_weights()):
      nu1.add_weight(falpy.Weights(m1_weights))
      nu2.add_weight(falpy.Weights(m2_weights))

    
    nu1.set_examples_seen(model1_examples_seen)
    nu2.set_examples_seen(model2_examples_seen)

    block.add_local_update(nu1)
    block.add_local_update(nu2)
    bc.add(block) ##Being moved not copied, could cause issues
    # last = bc.get_last()
    gu = bc.get_last().get_global_update()

    blockchain_weights = [w.array for w in gu.get_weights()]

    # for i, tw in enumerate(model1.trainable_weights):
    #   tw = ws[i]
    model1.set_weights(blockchain_weights)
    model2.set_weights(blockchain_weights)

    model1_examples_seen = 0
    model2_examples_seen = 0

  if epoch % 50 == 0:
    print("Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format
          (epoch,
           epoch_loss_avg1.result(),
           epoch_accuracy1.result()))
