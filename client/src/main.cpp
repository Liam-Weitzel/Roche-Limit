#include "game_state.h"
#include "utils.h"

typedef void (*client_main_fn)(GameState*);

struct ClientAPI {
  client_main_fn main;
};

int main() {
  LOG_TRACE("Starting client...");

  DynamicLib<ClientAPI> client;

  while(1) {
    if (!client.load(CLIENT_LIB_PATH)) {
      LOG_ERROR("Failed to load client library");
      return 1;
    }

    client.symbols->main = client.get_symbol<client_main_fn>("client_main");
    if (!client.symbols->main) {
      LOG_ERROR("Failed to load client symbols");
      return 1;
    }

    GameState state{};
    client.symbols->main(&state);

    client.unload();
    LOG_TRACE("Reload complete");
  }

  return 0;
}
