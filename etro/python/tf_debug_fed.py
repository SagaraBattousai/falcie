
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
  #Debug
  # print(np.hstack((y.numpy().reshape(-1,1), y_.numpy())))
  # mi, ma = functools.reduce(min_max_weight, 
  #                  zip(model.get_weights(), model.get_weights()),
  #                  (10000,-10000))

  # print("loss:", mi, ma)

  # print("Input:\n", x)
  # print("-----------------")
  # print("Model:\n", model.get_weights())
  # print("-----------------")
  # print("Preds:\n", y_)
  # print("-----------------\n\n\n")

  if np.any(np.isnan(y_.numpy())):
    print("Predicted a nan")
    # print()
    # print("True =", y, "\nPred =", y_)
    exit()
  #

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

      #vals = zip(model1.trainable_variables, model1.get_weights())
      #for x, y in vals:
      #  #for z in model1.get_weights(): #vals:
      #  x
        # print("T is W?", np.all(x.numpy() == y))

      epoch_loss_avg1.update_state(loss_value1)
      epoch_accuracy1.update_state(y, model1(x, training=True))

      # print("Len(x):",len(x))
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
      epoch_accuracy2.update_state(y, model2(x, training=True))

      # print("Len(x)2:", len(x))
      model2_examples_seen += len(x)

  train_loss_results.append(epoch_loss_avg1.result())
  train_accuracy_results.append(epoch_accuracy1.result())

  ### Falpy ###
  if epoch != 0 and epoch % 5 == 0:

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
        # zip(model1.get_weights(), model2.get_weights()):
    for m1_weights, m2_weights in \
        zip(model1.trainable_variables, model2.trainable_variables):
      nu1.add_weight(falpy.Weights(m1_weights.numpy()))
      nu2.add_weight(falpy.Weights(m2_weights.numpy()))

    # print("m1_before_set:", model1.get_weights()[0][0])
    # print("m2_before_set:", model2.get_weights()[0][0])
    
    nu1.set_examples_seen(model1_examples_seen)
    nu2.set_examples_seen(model2_examples_seen)

    block.add_local_update(nu1)
    block.add_local_update(nu2)
    bc.add(block) ##Being moved not copied, could cause issues
    # last = bc.get_last()
    gu = bc.get_last().get_global_update()

    # print("GU EG_SEEN:", gu.get_examples_seen())
    # print("M1 EG_SEEN:", nu1.get_examples_seen())
    # print("M2 EG_SEEN:", nu2.get_examples_seen())

    blockchain_weights = [w.array for w in gu.get_weights()]

    # print("*****\nM1:")
    # print(model1.get_weights())
    # print("\n*****\nM2:")
    # print(model2.get_weights())
    # print("\n*****\nBC:")
    # print(blockchain_weights)

    # print("Printing C version from python:")
    # print(gu.get_weights())
    # for w in gu.get_weights():
    #   print(w)
    #   print("\nI think it's from C\n")


    print("\n\n")
    print("gu last matrix ", gu.get_weights()[-1])
    print("gu last array  ", gu.get_weights()[-1].array)
    print("blockchain last", blockchain_weights[-1])
    print("type blockchain", blockchain_weights[-1].dtype)
    print("gu last matrix ", gu.get_weights()[-1])
    print("gu last array  ", gu.get_weights()[-1].array)



    # print("BcW:", blockchain_weights[0][0])
    # bcw_min, bcw_max = functools.reduce(min_max_weight, 
    #                zip(blockchain_weights, blockchain_weights),
    #                (10000,-10000))

    # print("bcw_mm:", bcw_min, bcw_max)

    if functools.reduce(any_nan, blockchain_weights, False):
      print("NaN found in blockchain_weights")
      # print("m1:", model1.get_weights()[0][0])
      # print("m2:", model2.get_weights()[0][0])
      # print("bw:", blockchain_weights[0][0])
      # print("ls1:", loss_value1)
      # print("gr1:", grads1[0][0])
      # print("ls2:", loss_value2)
      # print("gr2:", grads2[0][0])
      exit()
    # for i, tw in enumerate(model1.trainable_weights):
    #   tw = ws[i]


    #model1.set_weights(blockchain_weights)
    #model2.set_weights(blockchain_weights)
    ##New variables version of ^^
    for i, bcW in enumerate(blockchain_weights):
      model1.trainable_variables[i].assign(bcW)
      model1.trainable_variables[i].assign(bcW)




    # print("m1_after_set:", model1.get_weights()[0][0])
    # print("m2_after_set:", model2.get_weights()[0][0])
    m1w_min, m1w_max = functools.reduce(min_max_weight, 
                   zip(model1.get_weights(), model1.get_weights()),
                   (10000,-10000))

    # print("m1w_mm:", m1w_min, m1w_max)

    model1_examples_seen = 0
    model2_examples_seen = 0

  if epoch % 50 == 0:
    print("Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format
          (epoch,
           epoch_loss_avg1.result(),
           epoch_accuracy1.result()))
