from esphome.components import sensor, uart
from .luxtronik_v1_sensor import luxtronik_v1_sensor  # Import your Python class

DEPENDENCIES = ["uart"]

def to_code(config):
    var = yield uart.UARTDevice.new(config)
    yield LuxtronikV1Sensor(config, var)
