import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)

DEPENDENCIES = ["uart"]
CONF_TEMPERATUR_VORLAUF = "temperatur_vorlauf"
CONF_TEMPERATUR_RUECKLAUF = "temperatur_ruecklauf"
CONF_TEMPERATUR_RUECKLAUF_SOLL = "temperatur_ruecklauf_soll"
CONF_TEMPERATUR_HEISSGAS = "temperatur_heissgas"
CONF_TEMPERATUR_AUSSEN = "temperatur_aussen"
CONF_TEMPERATUR_BRAUCHWASSER = "temperatur_brauchwasser"
CONF_TEMPERATUR_BRAUCHWASSER_SOLL = "temperatur_brauchwasser_soll"
CONF_TEMPERATUR_WAERMEQUELLE_EINGANG = "temperatur_waermequelle_eingang"
CONF_TEMPERATUR_KAELTEKREIS = "temperatur_kaeltekreis"
CONF_TEMPERATUR_MISCHKREIS1_VORLAUF = "temperatur_mischkreis1_vorlauf"
CONF_TEMPERATUR_MISCHKREIS1_VORLAUF_SOLL = "temperatur_mischkreis1_vorlauf_soll"
CONF_TEMPERATUR_RAUMSTATION = "temperatur_raumstation"
CONF_EINGANG_ABTAU_SOLEDRUCK_DURCHFLUSS = "eingang_abtau_soledruck_durchfluss"
CONF_EINGANG_SPERRZEIT_EVU = "eingang_sperrzeit_evu"
CONF_EINGANG_HOCHDRUCKPRESSOSTAT = "eingang_hochdruckpressostat"
CONF_EINGANG_MOTORSCHUTZ = "eingang_motorschutz"
CONF_EINGANG_NIEDERDRUCKPRESSOSTAT = "eingang_niederdruckpressostat"
CONF_EINGANG_FREMDSTROMANODE = "eingang_fremdstromanode"
CONF_AUSGANG_ABTAUVENTIL = "ausgang_abtauventil"
CONF_AUSGANG_BRAUCHWASSERPUMPE = "ausgang_brauchwasserpumpe"
CONF_AUSGANG_FUSSBODENHEIZUNGSPUMPE = "ausgang_fussbodenheizungspumpe"
CONF_AUSGANG_HEIZUNGSPUMPE = "ausgang_heizungspumpe"
CONF_AUSGANG_MISCHER_1_AUF = "ausgang_mischer_1_auf"
CONF_AUSGANG_MISCHER_1_ZU = "ausgang_mischer_1_zu"
CONF_AUSGANG_VENTILATOR_WAERMEPUMPE = "ausgang_ventilator_waermepumpe"
CONF_AUSGANG_VENTILATOR_BRUNNEN = "ausgang_ventilator_brunnen"
CONF_AUSGANG_VERDICHTER_1 = "ausgang_verdichter_1"
CONF_AUSGANG_VERDICHTER_2 = "ausgang_verdichter_2"
CONF_AUSGANG_ZIRKULATIONSPUMPE = "ausgang_zirkulationspumpe"
CONF_AUSGANG_ZWEITER_WAERMEERZEUGER = "ausgang_zweiter_waermeerzeuger"
CONF_AUSGANG_ZWEITER_WAERMEERZEUGER_STOERUNG = "ausgang_zweiter_waermeerzeuger_stoerung"

luxtronik_v1_component_ns = cg.esphome_ns.namespace("luxtronik_v1_component")
LuxtronikV1Component = luxtronik_v1_component_ns.class_(
    "LuxtronikV1Component", cg.Component, uart.UARTDevice
)

TEMPERATURE_SCHEMA = sensor.sensor_schema(
    device_class=DEVICE_CLASS_TEMPERATURE,
    state_class=STATE_CLASS_MEASUREMENT,
    unit_of_measurement=UNIT_CELSIUS,
    accuracy_decimals=1,
)

INPUT_OUTPUT_SCHEMA = sensor.sensor_schema(
    state_class=STATE_CLASS_MEASUREMENT,
    unit_of_measurement="", # no unit
    accuracy_decimals=0,    # no decimals
)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(LuxtronikV1Component),
        cv.Optional(CONF_TEMPERATUR_VORLAUF): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_RUECKLAUF): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_RUECKLAUF_SOLL): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_HEISSGAS): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_AUSSEN): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_BRAUCHWASSER): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_BRAUCHWASSER_SOLL): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_WAERMEQUELLE_EINGANG): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_KAELTEKREIS): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_MISCHKREIS1_VORLAUF): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_MISCHKREIS1_VORLAUF_SOLL): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_TEMPERATUR_RAUMSTATION): TEMPERATURE_SCHEMA,
        cv.Optional(CONF_EINGANG_ABTAU_SOLEDRUCK_DURCHFLUSS): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_EINGANG_SPERRZEIT_EVU): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_EINGANG_HOCHDRUCKPRESSOSTAT): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_EINGANG_MOTORSCHUTZ): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_EINGANG_NIEDERDRUCKPRESSOSTAT): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_EINGANG_FREMDSTROMANODE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_ABTAUVENTIL): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_BRAUCHWASSERPUMPE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_FUSSBODENHEIZUNGSPUMPE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_HEIZUNGSPUMPE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_MISCHER_1_AUF): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_MISCHER_1_ZU): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_VENTILATOR_WAERMEPUMPE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_VENTILATOR_BRUNNEN): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_VERDICHTER_1): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_VERDICHTER_2): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_ZIRKULATIONSPUMPE): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_ZWEITER_WAERMEERZEUGER): INPUT_OUTPUT_SCHEMA,
        cv.Optional(CONF_AUSGANG_ZWEITER_WAERMEERZEUGER_STOERUNG): INPUT_OUTPUT_SCHEMA,
    })
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    
    if CONF_TEMPERATUR_VORLAUF in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_VORLAUF])
        cg.add(var.set_temperatur_vorlauf_sensor(sens))
    
    if CONF_TEMPERATUR_RUECKLAUF in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_RUECKLAUF])
        cg.add(var.set_temperatur_ruecklauf_sensor(sens))

    if CONF_TEMPERATUR_RUECKLAUF_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_RUECKLAUF_SOLL])
        cg.add(var.set_temperatur_ruecklauf_soll_sensor(sens))

    if CONF_TEMPERATUR_HEISSGAS in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_HEISSGAS])
        cg.add(var.set_temperatur_heissgas_sensor(sens))

    if CONF_TEMPERATUR_AUSSEN in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_AUSSEN])
        cg.add(var.set_temperatur_aussen_sensor(sens))

    if CONF_TEMPERATUR_BRAUCHWASSER in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_BRAUCHWASSER])
        cg.add(var.set_temperatur_brauchwasser_sensor(sens))

    if CONF_TEMPERATUR_BRAUCHWASSER_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_BRAUCHWASSER_SOLL])
        cg.add(var.set_temperatur_brauchwasser_soll_sensor(sens))

    if CONF_TEMPERATUR_WAERMEQUELLE_EINGANG in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_WAERMEQUELLE_EINGANG])
        cg.add(var.set_temperatur_waermequelle_eingang_sensor(sens))

    if CONF_TEMPERATUR_KAELTEKREIS in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_KAELTEKREIS])
        cg.add(var.set_temperatur_kaeltekreis_sensor(sens))

    if CONF_TEMPERATUR_MISCHKREIS1_VORLAUF in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_MISCHKREIS1_VORLAUF])
        cg.add(var.set_temperatur_mischkreis1_vorlauf_sensor(sens))

    if CONF_TEMPERATUR_MISCHKREIS1_VORLAUF_SOLL in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_MISCHKREIS1_VORLAUF_SOLL])
        cg.add(var.set_temperatur_mischkreis1_vorlauf_soll_sensor(sens))

    if CONF_TEMPERATUR_RAUMSTATION in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATUR_RAUMSTATION])
        cg.add(var.set_temperatur_raumstation_sensor(sens))

    if CONF_EINGANG_ABTAU_SOLEDRUCK_DURCHFLUSS in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_ABTAU_SOLEDRUCK_DURCHFLUSS])
        cg.add(var.set_eingang_abtau_soledruck_durchfluss_sensor(sens))

    if CONF_EINGANG_SPERRZEIT_EVU in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_SPERRZEIT_EVU])
        cg.add(var.set_eingang_sperrzeit_evu_sensor(sens))

    if CONF_EINGANG_HOCHDRUCKPRESSOSTAT in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_HOCHDRUCKPRESSOSTAT])
        cg.add(var.set_eingang_hochdruckpressostat_sensor(sens))

    if CONF_EINGANG_MOTORSCHUTZ in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_MOTORSCHUTZ])
        cg.add(var.set_eingang_motorschutz_sensor(sens))

    if CONF_EINGANG_NIEDERDRUCKPRESSOSTAT in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_NIEDERDRUCKPRESSOSTAT])
        cg.add(var.set_eingang_niederdruckpressostat_sensor(sens))

    if CONF_EINGANG_FREMDSTROMANODE in config:
        sens = await sensor.new_sensor(config[CONF_EINGANG_FREMDSTROMANODE])
        cg.add(var.set_eingang_fremdstromanode_sensor(sens))
    if CONF_AUSGANG_ABTAUVENTIL in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_ABTAUVENTIL])
        cg.add(var.set_ausgang_abtauventil_sensor(sens))
    
    if CONF_AUSGANG_BRAUCHWASSERPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_BRAUCHWASSERPUMPE])
        cg.add(var.set_ausgang_brauchwasserpumpe_sensor(sens))
        
    if CONF_AUSGANG_FUSSBODENHEIZUNGSPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_FUSSBODENHEIZUNGSPUMPE])
        cg.add(var.set_ausgang_fussbodenheizungspumpe_sensor(sens))
        
    if CONF_AUSGANG_HEIZUNGSPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_HEIZUNGSPUMPE])
        cg.add(var.set_ausgang_heizungspumpe_sensor(sens))
        
    if CONF_AUSGANG_MISCHER_1_AUF in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_MISCHER_1_AUF])
        cg.add(var.set_ausgang_mischer_1_auf_sensor(sens))
        
    if CONF_AUSGANG_MISCHER_1_ZU in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_MISCHER_1_ZU])
        cg.add(var.set_ausgang_mischer_1_zu_sensor(sens))
        
    if CONF_AUSGANG_VENTILATOR_WAERMEPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_VENTILATOR_WAERMEPUMPE])
        cg.add(var.set_ausgang_ventilator_waermepumpe_sensor(sens))
        
    if CONF_AUSGANG_VENTILATOR_BRUNNEN in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_VENTILATOR_BRUNNEN])
        cg.add(var.set_ausgang_ventilator_brunnen_sensor(sens))
        
    if CONF_AUSGANG_VERDICHTER_1 in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_VERDICHTER_1])
        cg.add(var.set_ausgang_verdichter_1_sensor(sens))
        
    if CONF_AUSGANG_VERDICHTER_2 in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_VERDICHTER_2])
        cg.add(var.set_ausgang_verdichter_2_sensor(sens))
        
    if CONF_AUSGANG_ZIRKULATIONSPUMPE in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_ZIRKULATIONSPUMPE])
        cg.add(var.set_ausgang_zirkulationspumpe_sensor(sens))
        
    if CONF_AUSGANG_ZWEITER_WAERMEERZEUGER in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_ZWEITER_WAERMEERZEUGER])
        cg.add(var.set_ausgang_zweiter_waermeerzeuger_sensor(sens))
        
    if CONF_AUSGANG_ZWEITER_WAERMEERZEUGER_STOERUNG in config:
        sens = await sensor.new_sensor(config[CONF_AUSGANG_ZWEITER_WAERMEERZEUGER_STOERUNG])
        cg.add(var.set_ausgang_zweiter_waermeerzeuger_stoerung_sensor(sens))