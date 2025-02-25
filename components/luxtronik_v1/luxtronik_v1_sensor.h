#include "esphome.h"
#include "esphome/components/sensor/sensor.h"
#include <vector>

namespace esphome {

const char ASCII_CR = 0x0D;
const char ASCII_LF = 0x0A;

const uint8_t READ_BUFFER_LENGTH = 255;

static const char* TAG = "luxtronik_v1";

class luxtronik_v1_sensor : public PollingComponent, public uart::UARTDevice {
 public:
  // constructor
  luxtronik_v1_sensor();

  void loop() override;
  void update() override;
  void set_uart(uart::UARTComponent *uart){
      this->uart_ = uart;
  }
  void set_temp_VL(sensor::Sensor* temp_VL);
  void set_temp_RL(sensor::Sensor* temp_RL);
  void set_temp_RL_Soll(sensor::Sensor* temp_RL_Soll);
  void set_temp_Heissgas(sensor::Sensor* temp_Heissgas);
  void set_temp_Aussen(sensor::Sensor* temp_Aussen);
  void set_temp_BW(sensor::Sensor* temp_BW);
  void set_temp_BW_Soll(sensor::Sensor* temp_BW_Soll);
  void set_temp_WQ_Ein(sensor::Sensor* temp_WQ_Ein);
  void set_temp_Kaeltekreis(sensor::Sensor* temp_Kaeltekreis);
  void set_temp_MK1_Vorl(sensor::Sensor* temp_MK1_Vorl);
  void set_temp_MK1VL_Soll(sensor::Sensor* temp_MK1VL_Soll);
  void set_temp_Raumstat(sensor::Sensor* temp_Raumstat);
  void set_ein_Abtau_Soledruck_Durchfluss(sensor::Sensor* ein_Abtau_Soledruck_Durchfluss);
  void set_ein_Sperrzeit_EVU(sensor::Sensor* ein_Sperrzeit_EVU);
  void set_ein_Hochdruckpressostat(sensor::Sensor* ein_Hochdruckpressostat);
  void set_ein_Motorschutz(sensor::Sensor* ein_Motorschutz);
  void set_ein_Niederdruckpressostat(sensor::Sensor* ein_Niederdruckpressostat);
  void set_ein_Fremdstromanode(sensor::Sensor* ein_Fremdstromanode);
  void set_aus_ATV(sensor::Sensor* aus_ATV);
  void set_aus_BWP(sensor::Sensor* aus_BWP);
  void set_aus_FBHP(sensor::Sensor* aus_FBHP);
  void set_aus_HZP(sensor::Sensor* aus_HZP);
  void set_aus_Mischer_1_Auf(sensor::Sensor* aus_Mischer_1_Auf);
  void set_aus_Mischer_1_Zu(sensor::Sensor* aus_Mischer_1_Zu);
  void set_aus_VentWP(sensor::Sensor* aus_VentWP);
  void set_aus_VentBrunnen(sensor::Sensor* aus_VentBrunnen);
  void set_aus_Verdichter_1(sensor::Sensor* aus_Verdichter_1);
  void set_aus_Verdichter_2(sensor::Sensor* aus_Verdichter_2);
  void set_aus_ZPumpe(sensor::Sensor* aus_ZPumpe);
  void set_aus_ZWE(sensor::Sensor* aus_ZWE);
  void set_aus_ZWE_Stoerung(sensor::Sensor* aus_ZWE_Stoerung);
  void set_status_Anlagentyp(sensor::Sensor* status_Anlagentyp);
  void set_status_Softwareversion(sensor::Sensor* status_Softwareversion);
  void set_status_Bivalenzstufe(sensor::Sensor* status_Bivalenzstufe);
  void set_status_Betriebszustand(sensor::Sensor* status_Betriebszustand);
  void set_modus_Heizung(sensor::Sensor* modus_Heizung);
  void set_modus_Warmwasser(sensor::Sensor* modus_Warmwasser);

 protected:
  uart::UARTComponent* uart_;
  std::string sender_;
  char read_buffer_[READ_BUFFER_LENGTH];
  size_t read_pos_{0};

  float GetValue(const std::string &message);
  void send_cmd_(const std::string &message);
  void parse_cmd_(const std::string &message);
  void parse_temperatures_(const std::string &message);
  void parse_inputs_(const std::string &message);
  void parse_outputs_(const std::string &message);
  void parse_status_(const std::string &message);
  void parse_heating_mode_(const std::string &message);
  void parse_water_mode_(const std::string &message);
  void parse_message_(const std::string &message, std::vector<sensor::Sensor*> &sensors, size_t start);

  // Sensor pointers
  sensor::Sensor *temp_VL_ptr;
  sensor::Sensor *temp_RL_ptr;
  sensor::Sensor *temp_RL_Soll_ptr;
  sensor::Sensor *temp_Heissgas_ptr;
  sensor::Sensor *temp_Aussen_ptr;
  sensor::Sensor *temp_BW_ptr;
  sensor::Sensor *temp_BW_Soll_ptr;
  sensor::Sensor *temp_WQ_Ein_ptr;
  sensor::Sensor *temp_Kaeltekreis_ptr;
  sensor::Sensor *temp_MK1_Vorl_ptr;
  sensor::Sensor *temp_MK1VL_Soll_ptr;
  sensor::Sensor *temp_Raumstat_ptr;
  sensor::Sensor *ein_Abtau_Soledruck_Durchfluss_ptr;
  sensor::Sensor *ein_Sperrzeit_EVU_ptr;
  sensor::Sensor *ein_Hochdruckpressostat_ptr;
  sensor::Sensor *ein_Motorschutz_ptr;
  sensor::Sensor *ein_Niederdruckpressostat_ptr;
  sensor::Sensor *ein_Fremdstromanode_ptr;
  sensor::Sensor *aus_ATV_ptr;
  sensor::Sensor *aus_BWP_ptr;
  sensor::Sensor *aus_FBHP_ptr;
  sensor::Sensor *aus_HZP_ptr;
  sensor::Sensor *aus_Mischer_1_Auf_ptr;
  sensor::Sensor *aus_Mischer_1_Zu_ptr;
  sensor::Sensor *aus_VentWP_ptr;
  sensor::Sensor *aus_VentBrunnen_ptr;
  sensor::Sensor *aus_Verdichter_1_ptr;
  sensor::Sensor *aus_Verdichter_2_ptr;
  sensor::Sensor *aus_ZPumpe_ptr;
  sensor::Sensor *aus_ZWE_ptr;
  sensor::Sensor *aus_ZWE_Stoerung_ptr;
  sensor::Sensor *status_Anlagentyp_ptr;
  sensor::Sensor *status_Softwareversion_ptr;
  sensor::Sensor *status_Bivalenzstufe_ptr;
  sensor::Sensor *status_Betriebszustand_ptr;
  sensor::Sensor *modus_Heizung_ptr;
  sensor::Sensor *modus_Warmwasser_ptr;

  CallbackManager<void(std::string, std::string)> callback_;
};

}  // namespace esphome
