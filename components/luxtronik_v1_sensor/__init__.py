import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID
from . import luxtronik_v1_sensor  # Import your Python class

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

CONF_LUXTRONIK_V1_ID = "luxtronik_v1_id"
CONF_UART_ID = "uart_id"

LuxtronikV1Sensor = luxtronik_v1_sensor.LuxtronikV1Sensor

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(LuxtronikV1Sensor),
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))
