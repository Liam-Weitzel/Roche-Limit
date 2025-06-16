#include "game_state.h"
#include "utils_test.h"
#include "utils.h"

//typedef void (*client_main_fn)(GameState*);

struct ClientAPI {
  //client_main_fn main;
};

int main() {
  LOG_TRACE("Starting tests...");

  DynamicLib<ClientAPI> client;

  if (!client.load(CLIENT_LIB_PATH)) {
    LOG_ERROR("Failed to load library");
    return 1;
  }

  // Run all tests
  iterators_arrays_CT_test();
  iterators_arrays_RT_test();
  create_and_fetch_arena_in_different_scope_CT_test();
  create_and_fetch_arena_in_different_scope_RT_test();
  create_hashmap_in_arena_CT_test();
  create_hashmap_in_arena_RT_test();
  quicksort_test();
  create_arena_clear_test();
  gen_sparse_set_ct_test();
  gen_sparse_set_rt_test();
  gen_sparse_set_ct_determinism_test();
  gen_sparse_set_rt_determinism_test();
  file_io_test();

  client.unload();
  return 0;
}
