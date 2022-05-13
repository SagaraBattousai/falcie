#include <stdlib.h>
#include <string.h>
#include <pulse/anima.h>
#include <pulse/linalg.h>

/*
def feedforward(inp,weights):
    prev_layer=inp;
    prev_layer=np.append(prev_layer,1);#add the bias
    curlayers=[prev_layer];
    bcurlayers=[prev_layer];#current neuron states (before activation)
    nolayers=len(weights);
    for l in range(nolayers):
        no_neurons=weights[l].shape[0]
        thislayer=np.zeros(no_neurons);
        bthislayer=np.zeros(no_neurons);#before activation
        for i in range(no_neurons):
            bthislayer[i]=np.dot(prev_layer,weights[l][i]);#(before activation)
            thislayer[i]=sigmoid(bthislayer[i]);
        prev_layer=thislayer;
        curlayers.append(np.array(thislayer));
        bcurlayers.append(np.array(bthislayer));#add to the layers (before activation)
    return curlayers,bcurlayers;
*/
int feedforward(float *input, float **weights, activation_t act, float **layers, float **pre_act_layers,
    unsigned int input_length, unsigned int num_layers, unsigned int *weight_dims)
{
    unsigned int prev_layer_length = (input_length + 1);
    float *prev_layer = (float *)malloc(sizeof(float) * prev_layer_length);
    if (prev_layer == NULL)
    {
        return -1;
    }
    *prev_layer = 1.f; //The bias term

    memcpy(prev_layer + 1, input, sizeof(input));
    layers[0] = prev_layer;
    pre_act_layers = prev_layer;
    float *thislayer;
    float *bthislayer;

    for (unsigned int i = 0; i < num_layers; i++)
    {
        //num_neurons = weight_dims[i]
        thislayer = (float *)malloc(sizeof(float) * weight_dims[i]);
        if (thislayer == NULL)
        {
            return -1;
        }
        bthislayer = (float *)calloc(weight_dims[i], sizeof(float)); //zeroed out
        if (bthislayer == NULL)
        {
            return -1;
        }
        matrix_multiply(prev_layer, weights[i], bthislayer, 1, prev_layer_length, weight_dims[i]);
        for (unsigned int j = 0; j < weight_dims[i]; j++)
        {
            thislayer[j] = act(bthislayer[j]);
        }
        prev_layer = thislayer;
        layers[i + 1] = thislayer;
        pre_act_layers[i + 1] = bthislayer;
        prev_layer_length = weight_dims[i];
    }
    return 0;
}
