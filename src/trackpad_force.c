#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "../../../modules/nau7802_loadcell/drivers/sensor/nau7802_loadcell/nau7802_loadcell.h"

#include "trackpad_force.h"

/* Declare the membership in registered logging module*/
LOG_MODULE_REGISTER(FORCE_SENSORS, CONFIG_SENSOR_LOG_LEVEL);

const struct device *const force_0 = DEVICE_DT_GET(DT_ALIAS(nau7802_adc_0));
const struct device *const force_1 = DEVICE_DT_GET(DT_ALIAS(nau7802_adc_1));
const struct device *const force_2 = DEVICE_DT_GET(DT_ALIAS(nau7802_adc_2));
const struct device *const force_3 = DEVICE_DT_GET(DT_ALIAS(nau7802_adc_3));


struct sensor_value force;

static int process_force_sensors(void) {
	
	struct sensor_value force_new;
	int ret;

	for (int i = 0; i <= 3; i++) {

	}
	ret = sensor_sample_fetch(dev);
	if (ret != 0) {
		LOG_ERR("%d Sensor sample update error", ret);
		return ret;
	}

	ret = sensor_channel_get(dev, SENSOR_CHAN_FORCE, &force);
	if (ret != 0) {
		LOG_ERR("%d Cannot read NAU7802 force channel", ret);
		return ret;
	}

	LOG_INF("Force:%f", );

	if (force <)

	return 0;
};

while (true) {
	process_force_sensors();
	if (sensor_value_to_double(&force) > TRACKPAD_FORCE_THRESHOLD_DOWN) {
		input_report_key()
		LOG_INF("Click down")
	} else if (sensor_value_to_double(&force) < TRACKPAD_FORCE_THRESHOLD_UP) {

	}
	k_sleep(K_MSEC(100));
}