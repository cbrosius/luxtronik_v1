import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart, sensor
from esphome import core

from .luxtronik_v1_sensor import luxtronik_v1_sensor

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

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        cv.GenerateID(): cv.declare_id(luxtronik_v1_sensor),
        cv.Optional(CONF_TEMP_VL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_RL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_RL_SOLL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_HEISSGAS): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_AUSSEN): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_BW): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_BW_SOLL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_WQ_EIN): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_KAELTEKREIS): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_MK1_VORL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_MK1VL_SOLL): sensor.sensor_schema(),
        cv.Optional(CONF_TEMP_RAUMSTAT): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_SPERRZEIT_EVU): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_HOCHDRUCKPRESSOSTAT): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_MOTORSCHUTZ): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_NIEDERDRUCKPRESSOSTAT): sensor.sensor_schema(),
        cv.Optional(CONF_EIN_FREMDSTROANODE): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_ATV): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_BWP): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_FBHP): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_HZP): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_MISCHER_1_AUF): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_MISCHER_1_ZU): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_VENTWP): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_VENTBRUNNEN): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_VERDICHTER_1): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_VERDICHTER_2): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_ZPUMPE): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_ZWE): sensor.sensor_schema(),
        cv.Optional(CONF_AUS_ZWE_STOERUNG): sensor.sensor_schema(),
        cv.Optional(CONF_STATUS_ANLAGENTYP): sensor.sensor_schema(),
        cv.Optional(CONF_STATUS_SOFTWAREVERSION): sensor.sensor_schema(),
        cv.Optional(CONF_STATUS_BIVALENZSTUFE): sensor.sensor_schema(),
        cv.Optional(CONF_STATUS_BETRIEBSZUSTAND): sensor.sensor_schema(),
        cv.Optional(CONF_MODUS_HEIZUNG): sensor.sensor_schema(),
        cv.Optional(CONF_MODUS_WARMWASSER): sensor.sensor_schema(),
    }
)

async def setup(config):
    var = cg.new_Pvariable(config[CONF_LUXTRONIK_V1_ID])
    await cg.register_component(var, config)
    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))

    # add the sensors:
    if CONF_TEMP_VL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_VL])
        cg.add(var.set_temp_VL(sens))
    if CONF_TEMP_RL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_RL])
        cg.add(var.set_temp_RL(sens))
    if CONF_TEMP_RL_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_RL_SOLL])
        cg.add(var.set_temp_RL_Soll(sens))
    if CONF_TEMP_HEISSGAS in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_HEISSGAS])
        cg.add(var.set_temp_Heissgas(sens))
    if CONF_TEMP_AUSSEN in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_AUSSEN])
        cg.add(var.set_temp_Aussen(sens))
    if CONF_TEMP_BW in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_BW])
        cg.add(var.set_temp_BW(sens))
    if CONF_TEMP_BW_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_BW_SOLL])
        cg.add(var.set_temp_BW_Soll(sens))
    if CONF_TEMP_WQ_EIN in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_WQ_EIN])
        cg.add(var.set_temp_WQ_Ein(sens))
    if CONF_TEMP_KAELTEKREIS in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_KAELTEKREIS])
        cg.add(var.set_temp_Kaeltekreis(sens))
    if CONF_TEMP_MK1_VORL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_MK1_VORL])
        cg.add(var.set_temp_MK1_Vorl(sens))
    if CONF_TEMP_MK1VL_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_MK1VL_SOLL])
        cg.add(var.set_temp_MK1VL_Soll(sens))
    if CONF_TEMP_RAUMSTAT in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_RAUMSTAT])
        cg.add(var.set_temp_Raumstat(sens))
    if CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS in config:
        sens = await sensor.new_sensor(config[CONF_EIN_ABTAU_SOLEDRUCK_DURCHFLUSS])
        cg.add(var.set_ein_Abtau_Soledruck_Durchfluss(sens))
    if CONF_EIN_SPERRZEIT_EVU in config:
        sens = await sensor.new_sensor(config[CONF_EIN_SPERRZEIT_EVU])
        cg.add(var.set_ein_Sperrzeit_EVU(sens))
    if CONF_EIN_HOCHDRUCKPRESSOSTAT in config:
        sens = await sensor.new_sensor(config[CONF_EIN_HOCHDRUCKPRESSOSTAT])
        cg.add(var.set_ein_Hochdruckpressostat(sens))
    if CONF_EIN_MOTORSCHUTZ in config:
        sens = await sensor.new_sensor(config[CONF_EIN_MOTORSCHUTZ])
        cg.add(var.set_ein_Motorschutz(sens))
    if CONF_EIN_NIEDERDRUCKPRESSOSTAT in config:
        sens = await sensor.new_sensor(config[CONF_EIN_NIEDERDRUCKPRESSOSTAT])
        cg.add(var.set_ein_Niederdruckpressostat(sens))
    if CONF_EIN_FREMDSTROANODE in config:
        sens = await sensor.new_sensor(config[CONF_EIN_FREMDSTROANODE])
        cg.add(var.set_ein_Fremdstromanode(sens))
    if CONF_AUS_ATV in config:
        sens = await sensor.new_sensor(config[CONF_AUS_ATV])
        cg.add(var.set_aus_ATV(sens))
    if CONF_AUS_BWP in config:
        sens = await sensor.new_sensor(config[CONF_AUS_BWP])
        cg.add(var.set_aus_BWP(sens))
    if CONF_AUS_FBHP in config:
        sens = await sensor.new_sensor(config[CONF_AUS_FBHP])
        cg.add(var.set_aus_FBHP(sens))
    if CONF_AUS_HZP in config:
        sens = await sensor.new_sensor(config[CONF_AUS_HZP])
        cg.add(var.set_aus_HZP(sens))
    if CONF_AUS_MISCHER_1_AUF in config:
        sens = await sensor.new_sensor(config[CONF_AUS_MISCHER_1_AUF])
        cg.add(var.set_aus_Mischer_1_Auf(sens))
    if CONF_AUS_MISCHER_1_ZU in config:
        sens = await sensor.new_sensor(config[CONF_AUS_MISCHER_1_ZU])
        cg.add(var.set_aus_Mischer_1_Zu(sens))
    if CONF_AUS_VENTWP in config:
        sens = await sensor.new_sensor(config[CONF_AUS_VENTWP])
        cg.add(var.set_aus_VentWP(sens))
    if CONF_AUS_VENTBRUNNEN in config:
        sens = await sensor.new_sensor(config[CONF_AUS_VENTBRUNNEN])
        cg.add(var.set_aus_VentBrunnen(sens))
    if CONF_AUS_VERDICHTER_1 in config:
        sens = await sensor.new_sensor(config[CONF_AUS_VERDICHTER_1])
        cg.add(var.set_aus_Verdichter_1(sens))
    if CONF_AUS_VERDICHTER_2 in config:
        sens = await sensor.new_sensor(config[CONF_AUS_VERDICHTER_2])
        cg.add(var.set_aus_Verdichter_2(sens))
    if CONF_AUS_ZPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUS_ZPUMPE])
        cg.add(var.set_aus_ZPumpe(sens))
    if CONF_AUS_ZWE in config:
        sens = await sensor.new_sensor(config[CONF_AUS_ZWE])
        cg.add(var.set_aus_ZWE(sens))
    if CONF_AUS_ZWE_STOERUNG in config:
        sens = await sensor.new_sensor(config[CONF_AUS_ZWE_STOERUNG])
        cg.add(var.set_aus_ZWE_Stoerung(sens))
    if CONF_STATUS_ANLAGENTYP in config:
        sens = await sensor.new_sensor(config[CONF_STATUS_ANLAGENTYP])
        cg.add(var.set_status_Anlagentyp(sens))
    if CONF_STATUS_SOFTWAREVERSION in config:
        sens = await sensor.new_sensor(config[CONF_STATUS_SOFTWAREVERSION])
        cg.add(var.set_status_Softwareversion(sens))
    if CONF_STATUS_BIVALENZSTUFE in config:
        sens = await sensor.new_sensor(config[CONF_STATUS_BIVALENZSTUFE])
        cg.add(var.set_status_Bivalenzstufe(sens))
    if CONF_STATUS_BETRIEBSZUSTAND in config:
        sens = await sensor.new_sensor(config[CONF_STATUS_BETRIEBSZUSTAND])
        cg.add(var.set_status_Betriebszustand(sens))
    if CONF_MODUS_HEIZUNG in config:
        sens = await sensor.new_sensor(config[CONF_MODUS_HEIZUNG])
        cg.add(var.set_modus_Heizung(sens))
    if CONF_MODUS_WARMWASSER in config:
        sens = await sensor.new_sensor(config[CONF_MODUS_WARMWASSER])
        cg.add(var.set_modus_Warmwasser(sens))

