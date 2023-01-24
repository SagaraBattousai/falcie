import math
import sys
import numpy as np

####################################
### Confiuration Settings ##########
####################################

batch_size = 32

num_epochs = 151

epoch_blockchain_submission = 25

num_models = 8

#Model configuration/builder

####################################
### Dataset Setup ##################
####################################


############################################

models = [i for i in range(num_models)]

print("The Blockchain is updated every", epoch_blockchain_submission, "epochs") 
print("There are", num_models, "participant models in this chain")

model_examples_seen = [0] * num_models

batch_splits = [math.floor(bs * 9 / num_models) \
                for bs in range(1, num_models + 1) ]

for epoch in range(5):

  model_index = 0

  for i in range(9):

    if i >= batch_splits[model_index]:
      model_index += 1

    print(models[model_index])
    model_examples_seen[model_index] += 1
  
  ### Falpy ###
  if epoch % 2 == 0:
    print("\n**\nblock adding time:\n")
    print("Exampels seen =", model_examples_seen)

    model_examples_seen = [0] * num_models

  print("\n&&&&&\n")




