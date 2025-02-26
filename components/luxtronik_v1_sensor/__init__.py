import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart, sensor
from esphome import core
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_NONE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
)

# Import the namespace from const.py
from .const import luxtronik_v1_ns 

# Declare the C++ class.
LuxtronikV1Component = luxtronik_v1_ns.class_(
    "LuxtronikV1Component", cg.PollingComponent, uart.UARTDevice
)

DEPENDENCIES = ["uart"]

CONF_UART_ID = "uart_id"
CONF_LUXTRONIK_V1_ID = "luxtronik_v1_id"
CONF_TEMP_VL = "temp_VL"
CONF_TEMP_RL = "temp_RL"
CONF_TEMP_RL_SOLL = "temp_RL_Soll"
CONF_TEMP_HEISSGAS = "temp_Heissgas"
CONF_TEMP_AUSSEN = "temp_Aussen"
CONF_TEMP_BW = "temp_BW"
CONF_TEMP_BW_SOLL = "temp_BW_Soll"
CONF_TEMP_WQ_EIN = "temp_WQ_Ein"
CONF_TEMP_KAELTEKREIS = "temp_Kaeltekreis"
CONF_TEMP_MK1_VORL = "temp_MK1_Vorl"
CONF_TEMP_MK1VL_SOLL = "temp_MK1VL_Soll"
CONF_TEMP_RAUMSTAT = "temp_Raumstat"
CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS = "ein_Abtau_Soledruck_Durchfluss"
CONF_EIN_SPERRZEIT_EVU = "ein_Sperrzeit_EVU"
CONF_EIN_HOCHDRUCKPRESSOSTAT = "ein_Hochdruckpressostat"
CONF_EIN_MOTORSCHUTZ = "ein_Motorschutz"
CONF_EIN_NIEDERDRUCKPRESSOSTAT = "ein_Niederdruckpressostat"
CONF_EIN_FREMDSTROANODE = "ein_Fremdstromanode"
CONF_AUS_ATV = "aus_ATV"
CONF_AUS_BWP = "aus_BWP"
CONF_AUS_FBHP = "aus_FBHP"
CONF_AUS_HZP = "aus_HZP"
CONF_AUS_MISCHER_1_AUF = "aus_Mischer_1_Auf"
CONF_AUS_MISCHER_1_ZU = "aus_Mischer_1_Zu"
CONF_AUS_VENTWP = "aus_VentWP"
CONF_AUS_VENTBRUNNEN = "aus_VentBrunnen"
CONF_AUS_VERDICHTER_1 = "aus_Verdichter_1"
CONF_AUS_VERDICHTER_2 = "aus_Verdichter_2"
CONF_AUS_ZPUMPE = "aus_ZPumpe"
CONF_AUS_ZWE = "aus_ZWE"
CONF_AUS_ZWE_STOERUNG = "aus_ZWE_Stoerung"
CONF_STATUS_ANLAGENTYP = "status_Anlagentyp"
CONF_STATUS_SOFTWAREVERSION = "status_Softwareversion"
CONF_STATUS_BIVALENZSTUFE = "status_Bivalenzstufe"
CONF_STATUS_BETRIEBSZUSTAND = "status_Betriebszustand"
CONF_MODUS_HEIZUNG = "modus_Heizung"
CONF_MODUS_WARMWASSER = "modus_Warmwasser"


def luxtronik_v1_sensor_schema(is_binary=False, unit=UNIT_EMPTY, device_class=DEVICE_CLASS_EMPTY, state_class=STATE_CLASS_NONE):
    if is_binary:
        return sensor.sensor_schema(unit_of_measurement=unit, device_class=device_class, state_class=state_class)
    return sensor.sensor_schema(unit_of_measurement=unit, accuracy_decimals=1, device_class=device_class, state_class=state_class)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        cv.GenerateID(): cv.declare_id(LuxtronikV1Component),
        cv.Optional(CONF_TEMP_VL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_RL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_RL_SOLL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_HEISSGAS): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_AUSSEN): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_BW): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_BW_SOLL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_WQ_EIN): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_KAELTEKREIS): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_MK1_VORL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_MK1VL_SOLL): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_TEMP_RAUMSTAT): luxtronik_v1_sensor_schema(unit=UNIT_CELSIUS, device_class=DEVICE_CLASS_TEMPERATURE, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_EIN_SPERRZEIT_EVU): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_EIN_HOCHDRUCKPRESSOSTAT): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_EIN_MOTORSCHUTZ): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_EIN_NIEDERDRUCKPRESSOSTAT): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_EIN_FREMDSTROANODE): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_ATV): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_BWP): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_FBHP): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_HZP): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_MISCHER_1_AUF): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_MISCHER_1_ZU): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_VENTWP): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_VENTBRUNNEN): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_VERDICHTER_1): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_VERDICHTER_2): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_ZPUMPE): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_ZWE): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_AUS_ZWE_STOERUNG): luxtronik_v1_sensor_schema(is_binary=True),
        cv.Optional(CONF_STATUS_ANLAGENTYP): luxtronik_v1_sensor_schema(),
        cv.Optional(CONF_STATUS_SOFTWAREVERSION): luxtronik_v1_sensor_schema(),
        cv.Optional(CONF_STATUS_BIVALENZSTUFE): luxtronik_v1_sensor_schema(),
        cv.Optional(CONF_STATUS_BETRIEBSZUSTAND): luxtronik_v1_sensor_schema(),
        cv.Optional(CONF_MODUS_HEIZUNG): luxtronik_v1_sensor_schema(),
        cv.Optional(CONF_MODUS_WARMWASSER): luxtronik_v1_sensor_schema(),
    }
).extend(cv.polling_component_schema("10s"))


async def to_code(config):
    # Create the object.
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    # Get the uart component.
    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))

    # Temperature Sensors
    if CONF_TEMP_VL in config:
        conf = config[CONF_TEMP_VL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_VL(sens))
    if CONF_TEMP_RL in config:
        conf = config[CONF_TEMP_RL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_RL(sens))
    if CONF_TEMP_RL_SOLL in config:
        conf = config[CONF_TEMP_RL_SOLL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_RL_Soll(sens))
    if CONF_TEMP_HEISSGAS in config:
        conf = config[CONF_TEMP_HEISSGAS]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_Heissgas(sens))
    if CONF_TEMP_AUSSEN in config:
        conf = config[CONF_TEMP_AUSSEN]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_Aussen(sens))
    if CONF_TEMP_BW in config:
        conf = config[CONF_TEMP_BW]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_BW(sens))
    if CONF_TEMP_BW_SOLL in config:
        conf = config[CONF_TEMP_BW_SOLL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_BW_Soll(sens))
    if CONF_TEMP_WQ_EIN in config:
        conf = config[CONF_TEMP_WQ_EIN]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_WQ_Ein(sens))
    if CONF_TEMP_KAELTEKREIS in config:
        conf = config[CONF_TEMP_KAELTEKREIS]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_Kaeltekreis(sens))
    if CONF_TEMP_MK1_VORL in config:
        conf = config[CONF_TEMP_MK1_VORL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_MK1_Vorl(sens))
    if CONF_TEMP_MK1VL_SOLL in config:
        conf = config[CONF_TEMP_MK1VL_SOLL]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_MK1VL_Soll(sens))
    if CONF_TEMP_RAUMSTAT in config:
        conf = config[CONF_TEMP_RAUMSTAT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temp_Raumstat(sens))

    # Binary Sensors
    if CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS in config:
        conf = config[CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Abtau_Soledruck_Durchfluss(sens))
    if CONF_EIN_SPERRZEIT_EVU in config:
        conf = config[CONF_EIN_SPERRZEIT_EVU]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Sperrzeit_EVU(sens))
    if CONF_EIN_HOCHDRUCKPRESSOSTAT in config:
        conf = config[CONF_EIN_HOCHDRUCKPRESSOSTAT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Hochdruckpressostat(sens))
    if CONF_EIN_MOTORSCHUTZ in config:
        conf = config[CONF_EIN_MOTORSCHUTZ]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Motorschutz(sens))
    if CONF_EIN_NIEDERDRUCKPRESSOSTAT in config:
        conf = config[CONF_EIN_NIEDERDRUCKPRESSOSTAT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Niederdruckpressostat(sens))
    if CONF_EIN_FREMDSTROANODE in config:
        conf = config[CONF_EIN_FREMDSTROANODE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ein_Fremdstromanode(sens))
    if CONF_AUS_ATV in config:
        conf = config[CONF_AUS_ATV]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_ATV(sens))
    if CONF_AUS_BWP in config:
        conf = config[CONF_AUS_BWP]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_BWP(sens))
    if CONF_AUS_FBHP in config:
        conf = config[CONF_AUS_FBHP]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_FBHP(sens))
    if CONF_AUS_HZP in config:
        conf = config[CONF_AUS_HZP]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_HZP(sens))
    if CONF_AUS_MISCHER_1_AUF in config:
        conf = config[CONF_AUS_MISCHER_1_AUF]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_Mischer_1_Auf(sens))
    if CONF_AUS_MISCHER_1_ZU in config:
        conf = config[CONF_AUS_MISCHER_1_ZU]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_Mischer_1_Zu(sens))
    if CONF_AUS_VENTWP in config:
        conf = config[CONF_AUS_VENTWP]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_VentWP(sens))
    if CONF_AUS_VENTBRUNNEN in config:
        conf = config[CONF_AUS_VENTBRUNNEN]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_VentBrunnen(sens))
    if CONF_AUS_VERDICHTER_1 in config:
        conf = config[CONF_AUS_VERDICHTER_1]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_Verdichter_1(sens))
    if CONF_AUS_VERDICHTER_2 in config:
        conf = config[CONF_AUS_VERDICHTER_2]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_Verdichter_2(sens))
    if CONF_AUS_ZPUMPE in config:
        conf = config[CONF_AUS_ZPUMPE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_ZPumpe(sens))
    if CONF_AUS_ZWE in config:
        conf = config[CONF_AUS_ZWE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_ZWE(sens))
    if CONF_AUS_ZWE_STOERUNG in config:
        conf = config[CONF_AUS_ZWE_STOERUNG]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_aus_ZWE_Stoerung(sens))

    # Other Sensors
    if CONF_STATUS_ANLAGENTYP in config:
        conf = config[CONF_STATUS_ANLAGENTYP]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_status_Anlagentyp(sens))
    if CONF_STATUS_SOFTWAREVERSION in config:
        conf = config[CONF_STATUS_SOFTWAREVERSION]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_status_Softwareversion(sens))
    if CONF_STATUS_BIVALENZSTUFE in config:
        conf = config[CONF_STATUS_BIVALENZSTUFE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_status_Bivalenzstufe(sens))
    if CONF_STATUS_BETRIEBSZUSTAND in config:
        conf = config[CONF_STATUS_BETRIEBSZUSTAND]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_status_Betriebszustand(sens))
    if CONF_MODUS_HEIZUNG in config:
        conf = config[CONF_MODUS_HEIZUNG]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_modus_Heizung(sens))
    if CONF_MODUS_WARMWASSER in config:
        conf = config[CONF_MODUS_WARMWASSER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_modus_Warmwasser(sens))