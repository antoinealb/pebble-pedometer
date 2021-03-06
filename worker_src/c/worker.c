#include <pebble_worker.h>
#include "pedometer.h"
#include "messages.h"

static pedometer_t meter;
static float filter_buffer[10];
static float threshold_buffer[25];

static void accelerometer_init(void);
static void accelerometer_cb(AccelData *data, uint32_t num_samples);
static void worker_message_cb(uint16_t type, AppWorkerMessage *message);

static bool meter_running = true;

static void accelerometer_init(void)
{
    // Allow accelerometer event
    accel_data_service_subscribe(10, accelerometer_cb);

    // Define accelerometer sampling rate
    accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
}

static void accelerometer_cb(AccelData *data, uint32_t num_samples)
{
    uint32_t n;

    // Send data from y axis to the podometer algorithm
    if (meter_running) {
        for (n = 0; n < num_samples; n++) {
            pedometer_process(&meter, data[n].z);
        }
    }

    AppWorkerMessage message = {
        .data0 = pedometer_get_step_count(&meter),
        .data1 = meter_running,
    };

    // Send the data to the background app
    app_worker_send_message(MESSAGE_STEP_COUNT, &message);
}

static void worker_message_cb(uint16_t type, AppWorkerMessage *message)
{
    switch (type) {
        case MESSAGE_STEP_RESET:
            pedometer_reset_step_count(&meter);
            break;

        case MESSAGE_PLAY_PAUSE:
            meter_running = !meter_running;
            break;

        default:
            break;
    }
}

static void worker_init()
{
    // Zeroes data processing filters
    memset(filter_buffer, 0, sizeof(filter_buffer));
    memset(threshold_buffer, 0, sizeof(threshold_buffer));

    // Configure pedometer algorithm.
    pedometer_init(&meter, filter_buffer, sizeof(filter_buffer) / sizeof(float),
                   threshold_buffer, sizeof(threshold_buffer) / sizeof(float));
    pedometer_set_hysteresis(&meter, 75);

    accelerometer_init();

    app_worker_message_subscribe(worker_message_cb);

    meter_running = true;
}


static void worker_deinit()
{
    // Stop Accelerometer
    accel_data_service_unsubscribe();
}

int main(void)
{
    worker_init();
    worker_event_loop();
    worker_deinit();
}
