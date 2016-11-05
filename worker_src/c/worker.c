#include <pebble_worker.h>

static void worker_init() {
  // Initialize the worker here
}

static void worker_deinit() {
  // Deinitialize the worker here
}

int main(void) {
  worker_init();
  worker_event_loop();
  worker_deinit();
}
 