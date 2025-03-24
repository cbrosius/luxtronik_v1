#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace luxtronik_v1_component {

static const char ASCII_CR = '\r';
static const char ASCII_LF = '\n';
static const uint8_t READ_BUFFER_LENGTH = 255;

class LuxtronikV1Component : public uart::UARTDevice, public PollingComponent {
 public:
  LuxtronikV1Component() : PollingComponent(60000) {}  // Default to 60 seconds

  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;

  void set_uart_parent(uart::UARTComponent *parent) { 
    this->parent_ = parent;
  }

  // Add temperature sensor setters
  void set_temperatur_vorlauf_sensor(sensor::Sensor *sens) { temperatur_vorlauf_ = sens; }
  void set_temperatur_ruecklauf_sensor(sensor::Sensor *sens) { temperatur_ruecklauf_ = sens; }
  void set_temperatur_ruecklauf_soll_sensor(sensor::Sensor *sens) { temperatur_ruecklauf_soll_ = sens; }
  void set_temperatur_heissgas_sensor(sensor::Sensor *sens) { temperatur_heissgas_ = sens; }
  void set_temperatur_aussen_sensor(sensor::Sensor *sens) { temperatur_aussen_ = sens; }
  void set_temperatur_brauchwasser_sensor(sensor::Sensor *sens) { temperatur_brauchwasser_ = sens; }
  void set_temperatur_brauchwasser_soll_sensor(sensor::Sensor *sens) { temperatur_brauchwasser_soll_ = sens; }
  void set_temperatur_waermequelle_eingang_sensor(sensor::Sensor *sens) { temperatur_waermequelle_eingang_ = sens; }
  void set_temperatur_kaeltekreis_sensor(sensor::Sensor *sens) { temperatur_kaeltekreis_ = sens; }
  void set_temperatur_mischkreis1_vorlauf_sensor(sensor::Sensor *sens) { temperatur_mischkreis1_vorlauf_ = sens; }
  void set_temperatur_mischkreis1_vorlauf_soll_sensor(sensor::Sensor *sens) { temperatur_mischkreis1_vorlauf_soll_ = sens; }
  void set_temperatur_raumstation_sensor(sensor::Sensor *sens) { temperatur_raumstation_ = sens; }
  // Add input sensor setters
  void set_eingang_abtau_soledruck_durchfluss_sensor(sensor::Sensor *sens) { eingang_abtau_soledruck_durchfluss_ = sens; }
  void set_eingang_sperrzeit_evu_sensor(sensor::Sensor *sens) { eingang_sperrzeit_evu_ = sens; }
  void set_eingang_hochdruckpressostat_sensor(sensor::Sensor *sens) { eingang_hochdruckpressostat_ = sens; }
  void set_eingang_motorschutz_sensor(sensor::Sensor *sens) { eingang_motorschutz_ = sens; }
  void set_eingang_niederdruckpressostat_sensor(sensor::Sensor *sens) { eingang_niederdruckpressostat_ = sens; }
  void set_eingang_fremdstromanode_sensor(sensor::Sensor *sens) { eingang_fremdstromanode_ = sens; }
  // Output sensor setters
  void set_ausgang_abtauventil_sensor(sensor::Sensor *sens) { ausgang_abtauventil_ = sens; }
  void set_ausgang_brauchwasserpumpe_sensor(sensor::Sensor *sens) { ausgang_brauchwasserpumpe_ = sens; }
  void set_ausgang_fussbodenheizungspumpe_sensor(sensor::Sensor *sens) { ausgang_fussbodenheizungspumpe_ = sens; }
  void set_ausgang_heizungspumpe_sensor(sensor::Sensor *sens) { ausgang_heizungspumpe_ = sens; }
  void set_ausgang_mischer_1_auf_sensor(sensor::Sensor *sens) { ausgang_mischer_1_auf_ = sens; }
  void set_ausgang_mischer_1_zu_sensor(sensor::Sensor *sens) { ausgang_mischer_1_zu_ = sens; }
  void set_ausgang_ventilator_waermepumpe_sensor(sensor::Sensor *sens) { ausgang_ventilator_waermepumpe_ = sens; }
  void set_ausgang_ventilator_brunnen_sensor(sensor::Sensor *sens) { ausgang_ventilator_brunnen_ = sens; }
  void set_ausgang_verdichter_1_sensor(sensor::Sensor *sens) { ausgang_verdichter_1_ = sens; }
  void set_ausgang_verdichter_2_sensor(sensor::Sensor *sens) { ausgang_verdichter_2_ = sens; }
  void set_ausgang_zirkulationspumpe_sensor(sensor::Sensor *sens) { ausgang_zirkulationspumpe_ = sens; }
  void set_ausgang_zweiter_waermeerzeuger_sensor(sensor::Sensor *sens) { ausgang_zweiter_waermeerzeuger_ = sens; }
  void set_ausgang_zweiter_waermeerzeuger_stoerung_sensor(sensor::Sensor *sens) { ausgang_zweiter_waermeerzeuger_stoerung_ = sens; }

  // Mode sensor setters
  void set_modus_heizung_numerisch_sensor(sensor::Sensor *sens) { modus_heizung_numerisch_ = sens; }
  void set_modus_heizung_sensor(text_sensor::TextSensor *sens) { modus_heizung_ = sens; }
  void set_modus_warmwasser_numerisch_sensor(sensor::Sensor *sens) { modus_warmwasser_numerisch_ = sens; }
  void set_modus_warmwasser_sensor(text_sensor::TextSensor *sens) { modus_warmwasser_ = sens; }

  // Status sensor setters
  void set_status_anlagentyp_sensor(sensor::Sensor *sens) { status_anlagentyp_ = sens; }
  void set_status_softwareversion_sensor(text_sensor::TextSensor *sens) { status_softwareversion_ = sens; }
  void set_status_bivalenzstufe_sensor(sensor::Sensor *sens) { status_bivalenzstufe_ = sens; }
  void set_status_betriebszustand_numerisch_sensor(sensor::Sensor *sens) { status_betriebszustand_numerisch_ = sens; }
  void set_status_betriebszustand_sensor(text_sensor::TextSensor *sens) { status_betriebszustand_ = sens; }
  void set_status_letzter_start_sensor(text_sensor::TextSensor *sens) { status_letzter_start_ = sens; }

  // Add error sensor setters
  void set_error0_fehlercode_sensor(sensor::Sensor *sens) { error0_fehlercode_ = sens; }
  void set_error0_fehlerbeschreibung_sensor(text_sensor::TextSensor *sens) { error0_fehlerbeschreibung_ = sens; }
  void set_error0_zeitpunkt_sensor(text_sensor::TextSensor *sens) { error0_zeitpunkt_ = sens; }
  void set_error1_fehlercode_sensor(sensor::Sensor *sens) { error1_fehlercode_ = sens; }
  void set_error1_fehlerbeschreibung_sensor(text_sensor::TextSensor *sens) { error1_fehlerbeschreibung_ = sens; }
  void set_error1_zeitpunkt_sensor(text_sensor::TextSensor *sens) { error1_zeitpunkt_ = sens; }
  void set_error2_fehlercode_sensor(sensor::Sensor *sens) { error2_fehlercode_ = sens; }
  void set_error2_fehlerbeschreibung_sensor(text_sensor::TextSensor *sens) { error2_fehlerbeschreibung_ = sens; }
  void set_error2_zeitpunkt_sensor(text_sensor::TextSensor *sens) { error2_zeitpunkt_ = sens; }
  void set_error3_fehlercode_sensor(sensor::Sensor *sens) { error3_fehlercode_ = sens; }
  void set_error3_fehlerbeschreibung_sensor(text_sensor::TextSensor *sens) { error3_fehlerbeschreibung_ = sens; }
  void set_error3_zeitpunkt_sensor(text_sensor::TextSensor *sens) { error3_zeitpunkt_ = sens; }
  void set_error4_fehlercode_sensor(sensor::Sensor *sens) { error4_fehlercode_ = sens; }
  void set_error4_fehlerbeschreibung_sensor(text_sensor::TextSensor *sens) { error4_fehlerbeschreibung_ = sens; }
  void set_error4_zeitpunkt_sensor(text_sensor::TextSensor *sens) { error4_zeitpunkt_ = sens; }

  // Add operatinghours sensor setters
  void set_betriebsstunden_verdichter_1_sensor(sensor::Sensor *sens) { betriebsstunden_verdichter_1_ = sens; }
  void set_impulse_verdichter_1_sensor(sensor::Sensor *sens) { impulse_verdichter_1_ = sens; }
  void set_durchschnittliche_einschaltdauer_verdichter_1_sensor(sensor::Sensor *sens) { durchschnittliche_einschaltdauer_verdichter_1_ = sens; }
  void set_betriebsstunden_verdichter_2_sensor(sensor::Sensor *sens) { betriebsstunden_verdichter_2_ = sens; }
  void set_impulse_verdichter_2_sensor(sensor::Sensor *sens) { impulse_verdichter_2_ = sens; }
  void set_durchschnittliche_einschaltdauer_verdichter_2_sensor(sensor::Sensor *sens) { durchschnittliche_einschaltdauer_verdichter_2_ = sens; }
  void set_betriebsstunden_zweiter_waermeerzeuger_1_sensor(sensor::Sensor *sens) { betriebsstunden_zweiter_waermeerzeuger_1_ = sens; }
  void set_betriebsstunden_zweiter_waermeerzeuger_2_sensor(sensor::Sensor *sens) { betriebsstunden_zweiter_waermeerzeuger_2_ = sens; }
  void set_betriebsstunden_waermepumpe_sensor(sensor::Sensor *sens) { betriebsstunden_waermepumpe_ = sens; }

  protected:
  float get_float_temp_(const std::string& value) { return std::atof(value.c_str()) / 10.0f; }
  void parse_message_(const char* message);
  void parse_temperatur_message_(const char* message);
  void parse_input_message_(const char* message);
  void parse_output_message_(const char* message);
  void parse_modus_heizung_message_(const char* message);      
  void parse_modus_warmwasser_message_(const char* message);   
  void parse_status_message_(const char* message);
  void parse_error_message_(const char* message);
  void parse_operatinghours_message_(const char* message);
  void publish_state_deferred_(sensor::Sensor* sensor, float value, const char* type, const char* name);
  std::string get_betriebszustand_text_(int state);
  std::string get_modus_text_(int state);
  std::string get_error_description_(int error_code);

  uart::UARTComponent *parent_{nullptr};
  char read_buffer_[READ_BUFFER_LENGTH];
  size_t read_pos_{0};

  // Temperature sensor pointers
  sensor::Sensor *temperatur_vorlauf_{nullptr};                 // 1100/2
  sensor::Sensor *temperatur_ruecklauf_{nullptr};               // 1100/3
  sensor::Sensor *temperatur_ruecklauf_soll_{nullptr};          // 1100/4
  sensor::Sensor *temperatur_heissgas_{nullptr};                // 1100/5
  sensor::Sensor *temperatur_aussen_{nullptr};                  // 1100/6
  sensor::Sensor *temperatur_brauchwasser_{nullptr};            // 1100/7
  sensor::Sensor *temperatur_brauchwasser_soll_{nullptr};       // 1100/8
  sensor::Sensor *temperatur_waermequelle_eingang_{nullptr};    // 1100/9
  sensor::Sensor *temperatur_kaeltekreis_{nullptr};             // 1100/10
  sensor::Sensor *temperatur_mischkreis1_vorlauf_{nullptr};     // 1100/11
  sensor::Sensor *temperatur_mischkreis1_vorlauf_soll_{nullptr};// 1100/12
  sensor::Sensor *temperatur_raumstation_{nullptr};             // 1100/13
  // Input sensor pointers
  sensor::Sensor *eingang_abtau_soledruck_durchfluss_{nullptr}; // 1200/2
  sensor::Sensor *eingang_sperrzeit_evu_{nullptr};              // 1200/3
  sensor::Sensor *eingang_hochdruckpressostat_{nullptr};        // 1200/4
  sensor::Sensor *eingang_motorschutz_{nullptr};                // 1200/5
  sensor::Sensor *eingang_niederdruckpressostat_{nullptr};      // 1200/6
  sensor::Sensor *eingang_fremdstromanode_{nullptr};            // 1200/7
  // Output sensor pointers
  sensor::Sensor *ausgang_abtauventil_{nullptr};                // 1300/2
  sensor::Sensor *ausgang_brauchwasserpumpe_{nullptr};          // 1300/3
  sensor::Sensor *ausgang_fussbodenheizungspumpe_{nullptr};     // 1300/4
  sensor::Sensor *ausgang_heizungspumpe_{nullptr};              // 1300/5
  sensor::Sensor *ausgang_mischer_1_auf_{nullptr};              // 1300/6
  sensor::Sensor *ausgang_mischer_1_zu_{nullptr};               // 1300/7
  sensor::Sensor *ausgang_ventilator_waermepumpe_{nullptr};     // 1300/8
  sensor::Sensor *ausgang_ventilator_brunnen_{nullptr};         // 1300/9
  sensor::Sensor *ausgang_verdichter_1_{nullptr};               // 1300/10
  sensor::Sensor *ausgang_verdichter_2_{nullptr};               // 1300/11
  sensor::Sensor *ausgang_zirkulationspumpe_{nullptr};          // 1300/12
  sensor::Sensor *ausgang_zweiter_waermeerzeuger_{nullptr};     // 1300/13
  sensor::Sensor *ausgang_zweiter_waermeerzeuger_stoerung_{nullptr}; // 1300/14
  
  // Modus sensor pointers
  sensor::Sensor *modus_heizung_numerisch_{nullptr};                // 3405/2
  text_sensor::TextSensor *modus_heizung_{nullptr};                // 3405/2
  sensor::Sensor *modus_warmwasser_numerisch_{nullptr};            // 3505/2
  text_sensor::TextSensor *modus_warmwasser_{nullptr};             // 3505/2

  // Status sensor pointers
  sensor::Sensor *status_anlagentyp_{nullptr};                  // 1700/2
  text_sensor::TextSensor *status_softwareversion_{nullptr};    // 1700/3
  sensor::Sensor *status_bivalenzstufe_{nullptr};               // 1700/4
  sensor::Sensor *status_betriebszustand_numerisch_{nullptr};   // 1700/5
  text_sensor::TextSensor *status_betriebszustand_{nullptr};    // 1700/5
  text_sensor::TextSensor *status_letzter_start_{nullptr};      // 1700/6-11

  // Error sensor pointers
  sensor::Sensor *error0_fehlercode_{nullptr};
  text_sensor::TextSensor *error0_fehlerbeschreibung_{nullptr};
  text_sensor::TextSensor *error0_zeitpunkt_{nullptr};
  sensor::Sensor *error1_fehlercode_{nullptr};
  text_sensor::TextSensor *error1_fehlerbeschreibung_{nullptr};
  text_sensor::TextSensor *error1_zeitpunkt_{nullptr};
  sensor::Sensor *error2_fehlercode_{nullptr};
  text_sensor::TextSensor *error2_fehlerbeschreibung_{nullptr};
  text_sensor::TextSensor *error2_zeitpunkt_{nullptr};
  sensor::Sensor *error3_fehlercode_{nullptr};
  text_sensor::TextSensor *error3_fehlerbeschreibung_{nullptr};
  text_sensor::TextSensor *error3_zeitpunkt_{nullptr};
  sensor::Sensor *error4_fehlercode_{nullptr};
  text_sensor::TextSensor *error4_fehlerbeschreibung_{nullptr};
  text_sensor::TextSensor *error4_zeitpunkt_{nullptr};

  // operatinghours sensor pointers
  sensor::Sensor *betriebsstunden_verdichter_1_{nullptr};                 // 1450/1
  sensor::Sensor *impulse_verdichter_1_{nullptr};                         // 1450/2
  sensor::Sensor *durchschnittliche_einschaltdauer_verdichter_1_{nullptr}; // 1450/3
  sensor::Sensor *betriebsstunden_verdichter_2_{nullptr};                 // 1450/4
  sensor::Sensor *impulse_verdichter_2_{nullptr};                         // 1450/5
  sensor::Sensor *durchschnittliche_einschaltdauer_verdichter_2_{nullptr}; // 1450/6
  sensor::Sensor *betriebsstunden_zweiter_waermeerzeuger_1_{nullptr};     // 1450/7
  sensor::Sensor *betriebsstunden_zweiter_waermeerzeuger_2_{nullptr};     // 1450/8
  sensor::Sensor *betriebsstunden_waermepumpe_{nullptr};                  // 1450/9
};

}  // namespace luxtronik_v1_component
}  // namespace esphome