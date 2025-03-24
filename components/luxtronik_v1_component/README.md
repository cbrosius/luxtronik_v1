```yaml
# example configuration:

esphome:
  name: luxtronik-test
  friendly_name: luxtronik-test

esp32:
  board: esp32dev
  framework:
    type: esp-idf

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  fast_connect: True

# Enable logging
logger:
  # disable serial loggin
  baud_rate: 0

web_server:

api:

ota:
  platform: esphome

uart:
  id: uart_bus
  rx_pin: GPIO2
  tx_pin: GPIO4
  baud_rate: 57600
  data_bits: 8
  parity: NONE
  stop_bits: 1
#  debug:
#    direction: BOTH
#    dummy_receiver: false

external_components:
  - source:
      type: git
      url: https://github.com/cbrosius/luxtronik-v1-component
      ref: Input-sensors
    components: [ luxtronik_v1_component ]
    refresh: 0s

luxtronik_v1_component:
  id: luxtronik_v1_component_1
  temperatur_vorlauf:
    name: "Temperatur Vorlauf"
    id: temperatur_vorlauf
  temperatur_ruecklauf:
    name: "Temperatur Rücklauf"
    id: temperatur_ruecklauf
  temperatur_ruecklauf_soll:
    name: "Temperatur Rücklauf Soll"
    id: temperatur_ruecklauf_soll
  temperatur_heissgas:
    name: "Temperatur Heissgas"
    id: temperatur_heissgas
  temperatur_aussen:
    name: "Temperatur Aussen"
    id: temperatur_aussen
  temperatur_brauchwasser:
    name: "Temperatur Brauchwasser"
    id: temperatur_brauchwasser
  temperatur_brauchwasser_soll:
    name: "Temperatur Brauchwasser Soll"
    id: temperatur_brauchwasser_soll
  temperatur_waermequelle_eingang:
    name: "Temperatur Wärmequelle Eingang"
    id: temperatur_waermequelle_eingang
  temperatur_kaeltekreis:
    name: "Temperatur Kältekreis"
    id: temperatur_kaeltekreis
  temperatur_mischkreis1_vorlauf:
    name: "Temperatur Mischkreis1 Vorlauf"
    id: temperatur_mischkreis1_vorlauf
  temperatur_mischkreis1_vorlauf_soll:
    name: "Temperatur Mischkreis1 Vorlauf Soll"
    id: temperatur_mischkreis1_vorlauf_soll
  temperatur_raumstation:
    name: "Temperatur Raumstation"
    id: temperatur_raumstation
  eingang_abtau_soledruck_durchfluss:
    name: "Eingang Abtau Soledruck Durchfluss"
    id: eingang_abtau_soledruck_durchfluss
  eingang_sperrzeit_evu:
    name: "Eingang Sperrzeit EVU"
    id: eingang_sperrzeit_evu
  eingang_hochdruckpressostat:
    name: "Eingang Hochdruckpressostat"
    id: eingang_hochdruckpressostat
  eingang_motorschutz:
    name: "Eingang Motorschutz"
    id: eingang_motorschutz
  eingang_niederdruckpressostat:
    name: "Eingang Niederdruckpressostat"
    id: eingang_niederdruckpressostat
  eingang_fremdstromanode:
    name: "Eingang Fremdstromanode"
    id: eingang_fremdstromanode
  # Output sensors
  ausgang_abtauventil:
    name: "Ausgang Abtauventil"
    id: ausgang_abtauventil
  ausgang_brauchwasserpumpe:
    name: "Ausgang Brauchwasserpumpe"
    id: ausgang_brauchwasserpumpe
  ausgang_fussbodenheizungspumpe:
    name: "Ausgang Fußbodenheizungspumpe"
    id: ausgang_fussbodenheizungspumpe
  ausgang_heizungspumpe:
    name: "Ausgang Heizungspumpe"
    id: ausgang_heizungspumpe
  ausgang_mischer_1_auf:
    name: "Ausgang Mischer 1 Auf"
    id: ausgang_mischer_1_auf
  ausgang_mischer_1_zu:
    name: "Ausgang Mischer 1 Zu"
    id: ausgang_mischer_1_zu
  ausgang_ventilator_waermepumpe:
    name: "Ausgang Ventilator Wärmepumpe"
    id: ausgang_ventilator_waermepumpe
  ausgang_ventilator_brunnen:
    name: "Ausgang Ventilator Brunnen"
    id: ausgang_ventilator_brunnen
  ausgang_verdichter_1:
    name: "Ausgang Verdichter 1"
    id: ausgang_verdichter_1
  ausgang_verdichter_2:
    name: "Ausgang Verdichter 2"
    id: ausgang_verdichter_2
  ausgang_zirkulationspumpe:
    name: "Ausgang Zirkulationspumpe"
    id: ausgang_zirkulationspumpe
  ausgang_zweiter_waermeerzeuger:
    name: "Ausgang Zweiter Wärmeerzeuger"
    id: ausgang_zweiter_waermeerzeuger
  ausgang_zweiter_waermeerzeuger_stoerung:
    name: "Ausgang Zweiter Wärmeerzeuger Störung"
    id: ausgang_zweiter_waermeerzeuger_stoerung
  modus_heizung:
    name: "Modus Heizung"
    id: modus_heizung
  modus_heizung_numerisch:
    name: "Modus Heizung Numerisch"
    id: modus_heizung_numerisch
  modus_warmwasser:
    name: "Modus Warmwasser"
    id: modus_warmwasser
  modus_warmwasser_numerisch:
    name: "Modus Warmwasser Numerisch"
    id: modus_warmwasser_numerisch
  # Status sensors
  status_anlagentyp:
    name: "Status Anlagentyp"
    id: status_anlagentyp
  status_softwareversion:
    name: "Status Softwareversion"
    id: status_softwareversion
  status_bivalenzstufe:
    name: "Status Bivalenzstufe"
    id: status_bivalenzstufe
  status_betriebszustand:
    name: "Status Betriebszustand"
    id: status_betriebszustand
  status_betriebszustand_numerisch:
    name: "Status Betriebszustand Numerisch"
    id: status_betriebszustand_numerisch
  status_letzter_start:
    name: "Status Letzter Start"
    id: status_letzter_start
  # Fehler sensors
  error0_zeitpunkt:
    name: "Error0 Zeitpunkt"
    id: error0_zeitpunkt
  error0_fehlercode:
    name: "Error0 Fehlercode"
    id: error0_fehlercode
  error0_fehlerbeschreibung:
    name: "Error0 Fehlerbeschreibung"
    id: error0_fehlerbeschreibung
  error1_zeitpunkt:
    name: "Error1 Zeitpunkt"
    id: error1_zeitpunkt
  error1_fehlercode:
    name: "Error1 Fehlercode"
    id: error1_fehlercode
  error1_fehlerbeschreibung:
    name: "Error1 Fehlerbeschreibung"
    id: error1_fehlerbeschreibung
  error2_zeitpunkt:
    name: "Error2 Zeitpunkt"
    id: error2_zeitpunkt
  error2_fehlercode:
    name: "Error2 Fehlercode"
    id: error2_fehlercode
  error2_fehlerbeschreibung:
    name: "Error2 Fehlerbeschreibung"
    id: error2_fehlerbeschreibung
  error3_zeitpunkt:
    name: "Error3 Zeitpunkt"
    id: error3_zeitpunkt
  error3_fehlercode:
    name: "Error3 Fehlercode"
    id: error3_fehlercode
  error3_fehlerbeschreibung:
    name: "Error3 Fehlerbeschreibung"
    id: error3_fehlerbeschreibung
  error4_zeitpunkt:
    name: "Error4 Zeitpunkt"
    id: error4_zeitpunkt
  error4_fehlercode:
    name: "Error4 Fehlercode"
    id: error4_fehlercode
  error4_fehlerbeschreibung:
    name: "Error4 Fehlerbeschreibung"
    id: error4_fehlerbeschreibung
  # Betriebsstunden sensors
  betriebsstunden_verdichter_1:
    name: "Betriebsstunden Verdichter 1"
    id: betriebsstunden_verdichter_1
  impulse_verdichter_1:
    name: "Impulse Verdichter 1"
    id: impulse_verdichter_1
  durchschnittliche_einschaltdauer_verdichter_1:
    name: "Durchschnittliche Einschaltdauer Verdichter 1"
    id: durchschnittliche_einschaltdauer_verdichter_1
  betriebsstunden_verdichter_2:
    name: "Betriebsstunden Verdichter 2"  
    id: betriebsstunden_verdichter_2
  impulse_verdichter_2:
    name: "Impulse Verdichter 2"
    id: impulse_verdichter_2
  durchschnittliche_einschaltdauer_verdichter_2:
    name: "Durchschnittliche Einschaltdauer Verdichter 2"
    id: durchschnittliche_einschaltdauer_verdichter_2
  betriebsstunden_zweiter_waermeerzeuger_1:
    name: "Betriebsstunden Zweiter Wärmeerzeuger 1"
    id: betriebsstunden_zweiter_waermeerzeuger_1
  betriebsstunden_zweiter_waermeerzeuger_2:
    name: "Betriebsstunden Zweiter Wärmeerzeuger 2"
    id: betriebsstunden_zweiter_waermeerzeuger_2
  betriebsstunden_waermepumpe:
    name: "Betriebsstunden Wärmepumpe"
    id: betriebsstunden_waermepumpe

button:
  - platform: template
    name: " UART prüfen"
    id: check_uart
    on_press:
      then:
        - uart.write:
            data: "\r\n"
  - platform: template
    name: " Werte manuell abfragen"
    id: ask_for_values
    on_press:
      then:
        - uart.write:
            data: "1100\r\n"
```