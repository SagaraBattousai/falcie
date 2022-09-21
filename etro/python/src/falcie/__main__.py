import argparse

from falcie.falpy import *


# Main
parser = argparse.ArgumentParser(
    description="Falcie Script for training Neural Networks"
    " in a federated system distributed via Blockchain"
)
parser.add_argument(
    "-n",
    "--num_models",
    type=int,
    default=2,
    help="The number of models participating in the federated system",
)
parser.add_argument(
    "-s",
    "--submission",
    type=int,
    default=25,
    help="The number of epochs before a network is added to the blockchain"
    " for federated averaging.",
)
parser.add_argument(
    "-e",
    "--epochs",
    type=int,
    default=151,
    help="The number of epochs to run for.",
)
parser.add_argument(
    "-bs",
    "--batch_size",
    type=int,
    default=32,
    help="The batch size to split the data into.",
)
parser.add_argument(
    "-t",
    "--train_percent",
    type=float,
    default=1.0,
    help="The percentage of the training set to use",
)
parser.add_argument(
    "--stats",
    type=int,
    default=50,
    help="The number of epochs to run before printing the networks statistics.",
)


subparsers = parser.add_subparsers(
    help="Subcommands allow users to specify what Neural Network framework they"
    " want to use for the federated system."
)

tf_parser = subparsers.add_parser(
    "tensorflow",
    aliases=["tf"],
    help="Falcie will expect Tensorflow Networks and use tensorflow as"
    " the framework for local training.",
)

args = parser.parse_args()
