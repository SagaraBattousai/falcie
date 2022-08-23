#ifndef __ETRO_NETWORK_UPDATE_H__
#define __ETRO_NETWORK_UPDATE_H__

#ifdef __cplusplus
#include <cstdint>
#include <orphan/federated_network.h>
#else
#include <stdint.h>
#endif

#include <etro/matrix.h>

#ifdef __cplusplus
typedef lindzei::NetworkUpdate network_update_t;
#else
typedef struct network_update network_update_t;
#endif

#ifdef __cplusplus 
extern "C" {
#endif

	network_update_t* new_network_update();

	void set_network_update_examples_seen(network_update_t *update, int64_t examples_seen);

	int64_t get_network_update_examples_seen(network_update_t *update);

	int add_matrix_to_update(network_update_t *update, matrix_t *matrix);

	int get_network_update_weights(const network_update_t *update, matrix_t ***matrices, int64_t *count);

	void delete_network_update(network_update_t *update);


#ifdef __cplusplus
}
#endif

#endif