#include <stdbool.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "../../../modules/nau7802_loadcell/drivers/sensor/nau7802_loadcell/nau7802_loadcell.h"
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#include "trackpad_force.h"

/* Declare the membership in registered logging module*/
LOG_MODULE_REGISTER(FORCE_SENSORS, CONFIG_SENSOR_LOG_LEVEL);

const struct device *const force_sensors[] = {
	DEVICE_DT_GET(DT_ALIAS(nau7802_adc_0)),
	DEVICE_DT_GET(DT_ALIAS(nau7802_adc_1)),
	DEVICE_DT_GET(DT_ALIAS(nau7802_adc_2)),
	DEVICE_DT_GET(DT_ALIAS(nau7802_adc_3)),
};

double force_value;
bool trackpad_down = false;

static int process_force_sensors(void) {
	force_value = 0.0;

	for (int i = 0; i <= 3; i++) {
		struct sensor_value force;
		int ret;
		
		ret = sensor_sample_fetch(force_sensors[i])
		if (ret != 0) {
			LOG_ERR("Sensor %d fetch error %d", i, ret);
			return ret;
		};

		ret = sensor_channel_get(force_sensors[i], SENSOR_CHAN_FORCE, &force);
		if (ret != 0) {
			LOG_ERR("%d Cannot read NAU7802 force channel", ret);
			return ret;
		};

		if (sensor_value_to_double(&force) > force_value) {
			force_value = sensor_value_to_double(&force);
		};
	};

	LOG_INF("Force:%f", force_value);

	return 0;
};

static void force_sensor_main(void) {
	ret = process_force_sensors()
	if (ret != 0) {
		return ret
	}

	if (trackpad_down == false && force_value > TRACKPAD_FORCE_THRESHOLD_DOWN) {
		trackpad_down = true;
		input_report_key(force_sensors[0], INPUT_BTN_0, true, false, K_FOREVER);
	} else if (trackpad_down == true && force_value < TRACKPAD_FORCE_THRESHOLD_UP) {
		trackpad_down = false;
		input_report_key(force_sensors[0], INPUT_BTN_0, false, false, K_FOREVER);
	};
}

for (int i = 0; i <= 3; i++) {
	struct sensor_trigger trig = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ALL,
	};
	if (sensor_trigger_set(nau7802, &trig, force_sensor_main) < 0) {
		LOG_ERR("Cannot configure trigger");
		return;
	}
}