import os

os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"
import math
import argparse
import sys

sys.path.insert(
    0, "c:\\users\\james\\falcie\\build\\etro\\python\\RelWithDebInfo"
)

import falpy
import numpy as np
import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt
from functools import partial

baseConv2D = partial(tf.keras.layers.Conv2D, 
        kernel_size=(3,3),
        padding="same",
        activation="relu")

# Model configuration/builder
def build_model():
    model = tf.keras.Sequential(
        [
            baseConv2D(filters=64, input_shape=224, 224),
            baseConv2D(filters=64),
            tf.keras.layers.MaxPooling2D(pool_size=(2, 2)),

            baseConv2D(filters=128),
            baseConv2D(filters=128),
            tf.keras.layers.MaxPooling2D(pool_size=(2, 2)),

            baseConv2D(filters=256),
            baseConv2D(filters=256),
            baseConv2D(filters=256),
            tf.keras.layers.MaxPooling2D(pool_size=(2, 2)),

            baseConv2D(filters=512),
            baseConv2D(filters=512),
            baseConv2D(filters=512),
            tf.keras.layers.MaxPooling2D(pool_size=(2, 2)),

            baseConv2D(filters=512),
            baseConv2D(filters=512),
            baseConv2D(filters=512),
            tf.keras.layers.MaxPooling2D(pool_size=(2, 2), name="vgg16"),

            tf.keras.layers.Flatten(),
            
            tf.keras.layers.Dense(64, activation="relu"),
            tf.keras.layers.Dense(2),
        ]
    )

    optimizer = tf.keras.optimizers.SGD(learning_rate=0.01)

    return model, optimizer


def loss(model, loss_object, x, y, training):
    y_ = model(x, training=training)
    return loss_object(y_true=y, y_pred=y_)


def grad(model, loss_object, inputs, targets):
    with tf.GradientTape() as tape:
        loss_value = loss(model, loss_object, inputs, targets, training=True)
    return loss_value, tape.gradient(loss_value, model.trainable_variables)


###########################################################################
### Actual Training Functions (essentially the main and sub functions). ###
###########################################################################


def run_against_test_set(model, test_dataset, batch_size=10):
    test_accuracy = tf.keras.metrics.Accuracy()
    ds_test_batch = test_dataset.batch(batch_size)

    for x, y in ds_test_batch:
        logits = model(x, training=False)
        prediction = tf.math.argmax(logits, axis=1, output_type=tf.int64)
        test_accuracy(prediction, y)

    print("Test set accuracy: {:.3%}".format(test_accuracy.result()))


# Still a tad too specific but meh Needed to be split for single and
# blockchain to work concurrently
def get_train_and_test_data(
    dataset_name="cifar10",
    train_percent=1.0,
    normalize_dataset=True,
    normalize_value=1.0 / 255,
):
    ####################################
    ### Dataset Setup ##################
    ####################################

    ds_train, ds_test = tfds.load(
        dataset_name,
        split=["train[:{:.0%}]".format(train_percent), "test"],
        as_supervised=True,
        with_info=False,
    )

    if normalize_dataset:
        normalization_layer = tf.keras.layers.Rescaling(normalize_value)
        normalize = lambda x, y: (normalization_layer(x), y)

        ds_train = ds_train.map(normalize)
        ds_test = ds_test.map(normalize)

    return ds_train, ds_test


def train_single_model(
    num_epochs=151, batch_size=32, train_percent=1.0, stats_after_epochs=50
):

    loss_object = tf.keras.losses.SparseCategoricalCrossentropy(
        from_logits=True
    )

    ds_train, ds_test = get_train_and_test_data(train_percent=train_percent)
    ds_train_batch = ds_train.batch(batch_size)

    model, optimizer = build_model()

    train_loss_results = []
    train_accuracy_results = []

    for epoch in range(num_epochs):
        epoch_loss_avg = tf.keras.metrics.Mean()
        epoch_accuracy = tf.keras.metrics.SparseCategoricalAccuracy()

        for x, y in ds_train_batch:
            # Train Model
            model, optimizer = models[model_index]
            loss_value, grads = grad(model, loss_object, x, y)
            optimizer.apply_gradients(zip(grads, model.trainable_variables))

            epoch_loss_avg.update_state(loss_value)
            epoch_accuracy.update_state(y, model(x, training=True))

        train_loss_results.append(epoch_loss_avg.result())
        train_accuracy_results.append(epoch_accuracy.result())

        if epoch % stats_after_epochs == 0:
            print(
                "Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format(
                    epoch, epoch_loss_avg.result(), epoch_accuracy.result()
                )
            )

    print("Training Finished:")
    run_against_test_set(model, ds_test)


def print_config(
    epoch_blockchain_submission, num_models, batch_size, train_percent
):

    blockchain_msg = (
        f"The Blockchain is updated every {epoch_blockchain_submission} epochs"
    )
    model_count_msg = f"Containing {num_models} participant models"
    batch_size_msg = f"With a batch size of {batch_size}"
    training_percentage_msg = (
        f"And trained by using {train_percent * 100 :.4}% of the data."
    )

    header_count = max(
        len(blockchain_msg),
        len(model_count_msg),
        len(batch_size_msg),
        len(training_percentage_msg),
    )

    blockchain_msg += " " * (header_count - len(blockchain_msg))
    model_count_msg += " " * (header_count - len(model_count_msg))
    batch_size_msg += " " * (header_count - len(batch_size_msg))
    training_percentage_msg += " " * (
        header_count - len(training_percentage_msg)
    )

    # +8 = "###  ###" (six hashtags and two spaces)
    print("*" * (header_count + 8))
    print("***", blockchain_msg, "***")
    print("***", model_count_msg, "***")
    print("***", batch_size_msg, "***")
    print("***", training_percentage_msg, "***")
    print("*" * (header_count + 8))

    ############################################


def train_federated_model(
    num_models=2,
    epoch_blockchain_submission=25,
    num_epochs=151,
    batch_size=32,
    dataset_name="cifar10",
    train_percent=1.0,
    stats_after_epochs=50,
    verbose=False,
):

    loss_object = tf.keras.losses.SparseCategoricalCrossentropy(
        from_logits=True
    )

    ds_train, ds_test = get_train_and_test_data(
        train_percent=train_percent, dataset_name=dataset_name
    )
    ds_train_batch = ds_train.batch(batch_size)

    models = [build_model()] * num_models

    if verbose:
        print_config(
            epoch_blockchain_submission, num_models, batch_size, train_percent
        )

    ###################
    ### Falpy Setup ###
    ###################

    bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
    bc = falpy.Blockchain(bb, 32)

    model_examples_seen = [0] * num_models

    ############################################

    batch_splits = [
        math.floor(bs * len(ds_train_batch) / num_models)
        for bs in range(1, num_models + 1)
    ]

    train_loss_results = []
    train_accuracy_results = []

    for epoch in range(num_epochs):
        epoch_loss_avg = tf.keras.metrics.Mean()
        epoch_accuracy = tf.keras.metrics.SparseCategoricalAccuracy()

        model_index = 0

        for i, (x, y) in enumerate(ds_train_batch):

            # Federated Parts
            # Not garunteed if uneven splits right?
            if i >= batch_splits[model_index]:
                model_index += 1

            model, optimizer = models[model_index]

            # Update examples seen for this model
            model_examples_seen[model_index] += len(x)

            # Train Model
            loss_value, grads = grad(model, loss_object, x, y)
            optimizer.apply_gradients(zip(grads, model.trainable_variables))

            # Generate metrics for model 1 (including examples it hasn't seen)
            epoch_loss_avg.update_state(
                loss(model, loss_object, x, y, training=True)
            )
            epoch_accuracy.update_state(y, model(x, training=True))

        train_loss_results.append(epoch_loss_avg.result())
        train_accuracy_results.append(epoch_accuracy.result())

        ### Falpy ###
        if epoch != 0 and epoch % epoch_blockchain_submission == 0:

            block = bb.build()

            # Seems to make refs
            # network_updates = [falpy.NetworkUpdate()] * num_models
            # ^^ Makes num_model references to the same network update!! :(

            network_updates = [falpy.NetworkUpdate() for _ in range(num_models)]

            # For now lets cheat and assume every variable is trainable
            # Also may not be a cheat since it is possible that
            # if all not trainable vars are static then it will average 
            # to itself

            for i, (model, _) in enumerate(models):
                for weight in model.get_weights():
                    network_updates[i].add_weight(falpy.Weights(weight))
                    ##Should this be in the inner loop?
                    network_updates[i].set_examples_seen(model_examples_seen[i])

            for i in range(len(network_updates)):
                block.add_local_update(network_updates[i])

            bc.add(block)  ##Being moved not copied, could cause issues
            # last = bc.get_last()
            gu = bc.get_last().get_global_update()

            # TODO: This (Hopefully) Is the last thing to fix!
            # Much like other bugs gu.get_weights MUST be assigned for some
            # (as yet) unknown reason!
            throw = gu.get_weights()

            blockchain_weights = [w.g_array for w in throw]

            for model, _ in models:
                model.set_weights(blockchain_weights)

            model_examples_seen = [0] * num_models

        if epoch % stats_after_epochs == 0 and epoch != 0:
            print(
                "Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format(
                    epoch, epoch_loss_avg.result(), epoch_accuracy.result()
                )
            )

    print("Training Finished:")
    run_against_test_set(models[0][0], ds_test)


if __name__ == "__main__":
    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.1,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=25,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=50,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=2,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=4,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )

    train_federated_model(
        num_models=8,
        epoch_blockchain_submission=75,
        num_epochs=151,
        batch_size=32,
        train_percent=0.25,
        stats_after_epochs=50,
        verbose=True,
    )
