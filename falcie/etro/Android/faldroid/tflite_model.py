# import os
# os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

# sys.path.insert(
    # 0, "c:\\users\\james\\falcie\\build\\etro\\python\\RelWithDebInfo"
# )
# import falpy

import numpy as np
import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt
import PIL.Image

IMG_SIZE = 32

IMG_CHANNELS = 3

SAVED_MODEL_DIR_FORMAT = "./trained_models/{}epochs/"


class Model(tf.Module):

    def __init__(self):
        self.model = tf.keras.Sequential([
            tf.keras.layers.Conv2D(
                32, (3, 3), activation="relu",
                input_shape=(IMG_SIZE, IMG_SIZE, IMG_CHANNELS),
                name="Convolve_1"),
            tf.keras.layers.MaxPooling2D((2, 2), name="Max_Pooling_1"),
            tf.keras.layers.Conv2D(64, (3, 3), activation="relu",
                    name="Convolve_2"),
            tf.keras.layers.MaxPooling2D((2, 2), name="Max_Pooling_2"),
            tf.keras.layers.Conv2D(64, (3, 3), activation="relu",
                    name="Convolve_3"),
            tf.keras.layers.Flatten(name="flatten_1"),
            tf.keras.layers.Dense(64, activation="relu", name="Dense_1"),
            tf.keras.layers.Dense(10, name="Dense_2"),
        ])

        self.model.compile(
            optimizer=tf.keras.optimizers.SGD(learning_rate=0.01),
            loss=tf.keras.losses.SparseCategoricalCrossentropy(
                    from_logits=True))


    @tf.function(input_signature=[
        tf.TensorSpec([None, IMG_SIZE, IMG_SIZE, IMG_CHANNELS], tf.float32),
        tf.TensorSpec([None], tf.int64)
    ])
    def train(self, x, y):
        with tf.GradientTape() as tape:
            prediction = self.model(x, training=True)
            loss = self.model.loss(y_true=y, y_pred=prediction)
        gradients = tape.gradient(loss, self.model.trainable_variables)
        self.model.optimizer.apply_gradients(
                zip(gradients, self.model.trainable_variables))
        result = {"loss": loss}
        return result

    @tf.function(input_signature=[
        tf.TensorSpec([None, IMG_SIZE, IMG_SIZE, IMG_CHANNELS], tf.float32),
    ])
    def infer(self, x):
        logits = self.model(x)
        probabilities = tf.nn.softmax(logits, axis=-1)
        return { "output": probabilities,
                 "logits": logits
               }


    @tf.function(input_signature=[tf.TensorSpec(shape=[], dtype=tf.string)])
    def save(self, checkpoint_path):
        tensor_names = [weight.name for weight in self.model.weights]
        tensors_to_save = [weight.read_value() for weight in self.model.weights]
        tf.raw_ops.Save(
                filename=checkpoint_path, tensor_names=tensor_names,
                data=tensors_to_save, name="save")
        return { "checkpoint_path": checkpoint_path }

    @tf.function(input_signature=[tf.TensorSpec(shape=[], dtype=tf.string)])
    def restore(self, checkpoint_path):
        restored_tensors = {}
        for var in self.model.weights:
            restored = tf.raw_ops.Restore(
                    file_pattern=checkpoint_path, tensor_name=var.name,
                    dt=var.dtype, name='restore')
            var.assign(restored)
            restored_tensors[var.name] = restored
        return restored_tensors


    @tf.function(input_signature=[tf.TensorSpec(shape=[], dtype=tf.string)])
    def getWeights(self, unused):
        weights = {}
        weight_iter = enumerate(self.model.weights) 
        #Should use try but we know its even for this case
        for i, w in weight_iter: 
            weights[f"layer_{(i // 2) + 1}_kernel"] = w
            (i, w) = next(weight_iter)
            weights[f"layer_{(i // 2) + 1}_bias"] = w

        return weights

    @tf.function(
            input_signature=[
                    tf.TensorSpec(shape=[], dtype=tf.string),
                    tf.TensorSpec(shape=[3,3,3,32], dtype=tf.float32),
                    tf.TensorSpec(shape=[32], dtype=tf.float32),
                    tf.TensorSpec(shape=[3,3,32,64], dtype=tf.float32),
                    tf.TensorSpec(shape=[64], dtype=tf.float32),
                    tf.TensorSpec(shape=[3,3,64,64], dtype=tf.float32),
                    tf.TensorSpec(shape=[64], dtype=tf.float32),
                    tf.TensorSpec(shape=[1024,64], dtype=tf.float32),
                    tf.TensorSpec(shape=[64], dtype=tf.float32),
                    tf.TensorSpec(shape=[64,10], dtype=tf.float32),
                    tf.TensorSpec(shape=[10], dtype=tf.float32),
                    ])
    def saveFedWeights(self, checkpoint_path,
            layer_1_kernel,
            layer_1_bias,
            layer_2_kernel,
            layer_2_bias,
            layer_3_kernel,
            layer_3_bias,
            layer_4_kernel,
            layer_4_bias,
            layer_5_kernel,
            layer_5_bias
            ):
        tensor_names = [weight.name for weight in self.model.weights]
        tensors_to_save = [
                layer_1_kernel,
                layer_1_bias,
                layer_2_kernel,
                layer_2_bias,
                layer_3_kernel,
                layer_3_bias,
                layer_4_kernel,
                layer_4_bias,
                layer_5_kernel,
                layer_5_bias
                ]

        tf.raw_ops.Save(
                filename=checkpoint_path, tensor_names=tensor_names,
                data=tensors_to_save, name="save")
        return { "checkpoint_path": checkpoint_path }

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


def preTraining(model,
        num_epochs: int = 100,
        batch_size: int = 100,
        train_percent: float = 0.25
        ):
    epochs = np.arange(1, num_epochs + 1, 1)
    losses = np.zeros([num_epochs])

    ds_train, _ = get_train_and_test_data(train_percent=train_percent)
    ds_train = ds_train.batch(batch_size)

    for i in range(num_epochs):
        for x, y in ds_train:
            result = model.train(x, y)

        losses[i] = result['loss']
        if (i + 1) % 10 == 0:
            print(f"Finished {i + 1} epochs")
            print(f"\tloss: {losses[i]:.3f}")

def preTrainAndSave(saved_model_dir: str = None, num_epochs: int = 100):
    if saved_model_dir is None:
        saved_model_dir = SAVED_MODEL_DIR_FORMAT.format(num_epochs)
    
    model = Model()
    preTraining(model, num_epochs=num_epochs)
    tf.saved_model.save(
            model,
            saved_model_dir,
            signatures={
                    'atrain': #Hacky but awesome fix
                        model.train.get_concrete_function(),
                    'infer':
                        model.infer.get_concrete_function(),
                    'save':
                        model.save.get_concrete_function(),
                    'restore':
                        model.restore.get_concrete_function(),
                    'getWeights':
                        model.getWeights.get_concrete_function(),
                    'saveFedWeights':
                        model.saveFedWeights.get_concrete_function(),
            })

def convert_dir_to_tflite(saved_model_dir):
    converter = tf.lite.TFLiteConverter.from_saved_model(saved_model_dir)
    converter.target_spec.supported_ops = [
            tf.lite.OpsSet.TFLITE_BUILTINS, # enable TF Lite ops.
            tf.lite.OpsSet.SELECT_TF_OPS # enable TF ops.
    ]
    converter.experimental_enable_resource_variables = True
    tflite_model = converter.convert()

    with open(f"{saved_model_dir}/cifar10.tflite", "wb") as f:
        f.write(tflite_model)

if __name__ == '__main__':
    
    # model = Model()
    # num_epochs = 100
    # model.restore(
    #   f"{SAVED_MODEL_DIR_FORMAT.format{num_epochs}/cifar10.ckpt")
    # print(model.model.weights[0][0][0][0])
    
    num_epochs=100
    
    preTrainAndSave(SAVED_MODEL_DIR_FORMAT.format(num_epochs), num_epochs)
    convert_dir_to_tflite(SAVED_MODEL_DIR_FORMAT.format(num_epochs))

    # model = Model()
    # model.restore(f"{SAVED_MODEL_DIR}/25_cifar10.ckpt")

    # x = model.infer(np.array(PIL.Image.open("1.png")).reshape(1, 32, 32, 3))

    # print(x)
