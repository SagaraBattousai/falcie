
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
    tf.keras.layers.Dense(50, activation=tf.nn.relu, input_shape=(4,)),
    tf.keras.layers.Dense(10, activation=tf.nn.relu),
    tf.keras.layers.Dense(3)
    ])

model2 = tf.keras.Sequential([
    tf.keras.layers.Dense(10, activation=tf.nn.relu, input_shape=(4,)),
    tf.keras.layers.Dense(50, activation=tf.nn.relu, input_shape=(4,)),
    tf.keras.layers.Dense(10, activation=tf.nn.relu),
    tf.keras.layers.Dense(3)
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

num_epochs = 401
epoch_blockchain_submission = 25
print("The Blockchain is updated every", epoch_blockchain_submission, "epochs") 

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
      epoch_loss_avg2.update_state(loss(model2, x, y, training=True))
      epoch_accuracy1.update_state(y, model1(x, training=True))
      epoch_accuracy2.update_state(y, model2(x, training=True))

      model1_examples_seen += len(x)

    else:
      loss_value2, grads2 = grad(model2, x, y)
      if np.isnan(loss_value2.numpy()):
        print("Model 2's loss is NaN")
        exit()

      if functools.reduce(any_nan, grads2, False):
        print("Gradents for M2 contain NaN's")
        exit()

      optimizer2.apply_gradients(zip(grads2, model2.trainable_variables))

      if functools.reduce(any_nan, model2.get_weights(), False):
        print("Model 2 Weights are NaN")
        exit()

      epoch_loss_avg2.update_state(loss_value2)
      epoch_loss_avg1.update_state(loss(model1, x, y, training=True))
      epoch_accuracy2.update_state(y, model2(x, training=True))
      epoch_accuracy1.update_state(y, model1(x, training=True))

      model2_examples_seen += len(x)

  train_loss_results.append(epoch_loss_avg1.result())
  train_accuracy_results.append(epoch_accuracy1.result())

  ### Falpy ###
  if epoch % epoch_blockchain_submission == 0:

    block = bb.build()

    nu1 = falpy.NetworkUpdate()
    nu2 = falpy.NetworkUpdate()


    #For now lets cheat and assume every variable is trainable
    #Also may not be a cheat since it is possible that 
    #if all not trainable vars are static then it will average to itself

    # zip(model1.get_weights(), model2.get_weights()):
    for m1_weights, m2_weights in \
        zip(model1.trainable_variables, model2.trainable_variables):
      nu1.add_weight(falpy.Weights(m1_weights.numpy()))
      nu2.add_weight(falpy.Weights(m2_weights.numpy()))

    nu1.set_examples_seen(model1_examples_seen)
    nu2.set_examples_seen(model2_examples_seen)

    block.add_local_update(nu1)
    block.add_local_update(nu2)
    bc.add(block) ##Being moved not copied, could cause issues
    # last = bc.get_last()
    gu = bc.get_last().get_global_update()

    #TODO: This (Hopefully) Is the last thing to fix!
    #Much like other bugs gu.get_weights MUST be assigned for some
    #(as yet) unknown reason!
    throw = gu.get_weights()
    
    blockchain_weights = [w.g_array for w in throw]

    if functools.reduce(any_nan, blockchain_weights, False):
      print("NaN found in blockchain_weights")
      exit()

    model1.set_weights(blockchain_weights)
    model2.set_weights(blockchain_weights)
    ##New variables version of ^^
    # for i, bcW in enumerate(blockchain_weights):
    #   model1.trainable_variables[i].assign(bcW)
    #   model1.trainable_variables[i].assign(bcW)

    model1_examples_seen = 0
    model2_examples_seen = 0

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

