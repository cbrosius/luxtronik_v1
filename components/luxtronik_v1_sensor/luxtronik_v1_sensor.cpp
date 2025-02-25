#include "luxtronik_v1_sensor.h"

namespace esphome {

  luxtronik_v1_sensor::luxtronik_v1_sensor(): PollingComponent(60000), uart_(nullptr), temp_VL_ptr(nullptr),
  // change all the other pointers to nullptr:
   temp_RL(nullptr),temp_RL_Soll(nullptr),temp_Heissgas(nullptr),
    temp_Aussen(nullptr), temp_BW(nullptr),
    temp_BW_Soll(nullptr), temp_WQ_Ein(nullptr),
    temp_Kaeltekreis(nullptr), temp_MK1_Vorl(nullptr),
    temp_MK1VL_Soll(nullptr), temp_Raumstat(nullptr),
    ein_Abtau_Soledruck_Durchfluss(nullptr), ein_Sperrzeit_EVU(nullptr),
    ein_Hochdruckpressostat(nullptr), ein_Motorschutz(nullptr),
    ein_Niederdruckpressostat(nullptr), ein_Fremdstromanode(nullptr),
    aus_ATV(nullptr), aus_BWP(nullptr),
    aus_FBHP(nullptr), aus_HZP(nullptr),
    aus_Mischer_1_Auf(nullptr), aus_Mischer_1_Zu(nullptr),
    aus_VentWP(nullptr), aus_VentBrunnen(nullptr),
    aus_Verdichter_1(nullptr), aus_Verdichter_2(nullptr),
    aus_ZPumpe(nullptr), aus_ZWE(nullptr),
    aus_ZWE_Stoerung(nullptr), status_Anlagentyp(nullptr),
    status_Softwareversion(nullptr), status_Bivalenzstufe(nullptr),
    status_Betriebszustand(nullptr), modus_Heizung(nullptr),
    modus_Warmwasser(nullptr) {}
  
void luxtronik_v1_sensor::loop() {

  if (!this->uart_) {
    ESP_LOGE(TAG, "UART not initialized!");
    return;
  }

  // Read message
  while (available()) {
    uint8_t byte;
    this->uart_->read_byte(&byte);

    if (this->read_pos_ == READ_BUFFER_LENGTH)
      this->read_pos_ = 0;

    ESP_LOGVV(TAG, "Buffer pos: %u %d", this->read_pos_, byte);  // NOLINT

    if (byte == ASCII_CR)
      continue;
    if (byte >= 0x7F)
      byte = '?';  // need to be valid utf8 string for log functions.
    this->read_buffer_[this->read_pos_] = byte;

    if (this->read_buffer_[this->read_pos_] == ASCII_LF) {
      this->read_buffer_[this->read_pos_] = 0;
      this->read_pos_ = 0;
      this->parse_cmd_(this->read_buffer_);
    } else {
      this->read_pos_++;
    }
  }
}

void luxtronik_v1_sensor::update() {
  // Ask for Temperatures
  send_cmd_("1100");
}

float luxtronik_v1_sensor::GetValue(const std::string &message) {
  return std::atof(message.c_str()) / 10;
}

void luxtronik_v1_sensor::send_cmd_(const std::string &message) {
  
  if (!this->uart_) {
    ESP_LOGE(TAG, "UART not initialized!");
    return;
  }

  ESP_LOGD(TAG, "S: %s - %d", message.c_str(), 0);
  this->uart_->write_str(message.c_str());
  this->uart_->write_byte(ASCII_CR);
  this->uart_->write_byte(ASCII_LF);
}

void luxtronik_v1_sensor::parse_cmd_(const std::string &message) {
  if (message.empty())
    return;

  ESP_LOGD(TAG, "R: %s - %d", message.c_str(), 0);

  if (message.find("1100") == 0 && message.length() > 4) {
    parse_temperatures_(message);
    send_cmd_("1200");
  } else if (message.find("1200") == 0 && message.length() > 4) {
    parse_inputs_(message);
    send_cmd_("1300");
  } else if (message.find("1300") == 0 && message.length() > 4) {
    parse_outputs_(message);
    send_cmd_("1700");
  } else if (message.find("1700") == 0 && message.length() > 4) {
    parse_status_(message);
    send_cmd_("3405");
  } else if (message.find("3405") == 0 && message.length() > 4) {
    parse_heating_mode_(message);
    send_cmd_("3505");
  } else if (message.find("3505") == 0 && message.length() > 4) {
    parse_water_mode_(message);
  } else if (message.find("779") == 0) {
    ESP_LOGD(TAG, "Programming Error -> Reset Programming");
    send_cmd_("3401;0");
    send_cmd_("999");
  } else if (message.find("777") == 0) {
    ESP_LOGD(TAG, "UART is ready for input");
  } else if (message.find("993") == 0) {
    ESP_LOGD(TAG, "Value saved");
  } else if (message.find("999") == 0) {
    ESP_LOGD(TAG, "Save Value");
  }
}

void luxtronik_v1_sensor::parse_temperatures_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = {
    temp_VL_ptr, temp_RL_ptr, temp_RL_Soll_ptr, temp_Heissgas_ptr, temp_Aussen_ptr,
    temp_BW_ptr, temp_BW_Soll_ptr, temp_WQ_Ein_ptr, temp_Kaeltekreis_ptr, temp_MK1_Vorl_ptr,
    temp_MK1VL_Soll_ptr, temp_Raumstat_ptr
  };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_inputs_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = {
    ein_Abtau_Soledruck_Durchfluss_ptr, ein_Sperrzeit_EVU_ptr, ein_Hochdruckpressostat_ptr,
    ein_Motorschutz_ptr, ein_Niederdruckpressostat_ptr, ein_Fremdstromanode_ptr
  };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_outputs_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = {
    aus_ATV_ptr, aus_BWP_ptr, aus_FBHP_ptr, aus_HZP_ptr, aus_Mischer_1_Auf_ptr,
    aus_Mischer_1_Zu_ptr, aus_VentWP_ptr, aus_VentBrunnen_ptr, aus_Verdichter_1_ptr,
    aus_Verdichter_2_ptr, aus_ZPumpe_ptr, aus_ZWE_ptr, aus_ZWE_Stoerung_ptr
  };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_status_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = {
    status_Anlagentyp_ptr, status_Softwareversion_ptr, status_Bivalenzstufe_ptr, status_Betriebszustand_ptr
  };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_heating_mode_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = { modus_Heizung_ptr };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_water_mode_(const std::string &message) {
  std::vector<sensor::Sensor*> sensors = { modus_Warmwasser_ptr };
  parse_message_(message, sensors, 5);
}

void luxtronik_v1_sensor::parse_message_(const std::string &message, std::vector<std::unique_ptr<Sensor>*> &sensors, size_t start) {
  size_t end = message.find(';', start);
  for (auto &sensor : sensors) {
    start = end + 1;
    end = message.find(';', start);
    std::string value = message.substr(start, end - start);
    (*sensor)->publish_state(GetValue(value));
  }
}
void luxtronik_v1_sensor::parse_message_(const std::string &message, std::vector<sensor::Sensor*> &sensors, size_t start) {
  size_t end = message.find(';', start);
  for (auto &sensor : sensors) {
    start = end + 1;
    end = message.find(';', start);
    std::string value = message.substr(start, end - start);
    if (sensor != nullptr){
        (*sensor)->publish_state(std::atof(value.c_str()) / 10);
    }
  }
}
void luxtronik_v1_sensor::set_temp_VL(sensor::Sensor* temp_VL) {
  this->temp_VL_ptr = temp_VL;
  }
void luxtronik_v1_sensor::set_temp_RL(sensor::Sensor* temp_RL) {
    this->temp_RL_ptr = temp_RL;
  }
void luxtronik_v1_sensor::set_temp_RL_Soll(sensor::Sensor* temp_RL_Soll) {
    this->temp_RL_Soll_ptr = temp_RL_Soll;
  }
void luxtronik_v1_sensor::set_temp_Heissgas(sensor::Sensor* temp_Heissgas) {
    this->temp_Heissgas_ptr = temp_Heissgas;
  }
void luxtronik_v1_sensor::set_temp_Aussen(sensor::Sensor* temp_Aussen) {
    this->temp_Aussen_ptr = temp_Aussen;
  }
void luxtronik_v1_sensor::set_temp_BW(sensor::Sensor* temp_BW) {
    this->temp_BW_ptr = temp_BW;
  }
void luxtronik_v1_sensor::set_temp_BW_Soll(sensor::Sensor* temp_BW_Soll) {
    this->temp_BW_Soll_ptr = temp_BW_Soll;
  }
void luxtronik_v1_sensor::set_temp_WQ_Ein(sensor::Sensor* temp_WQ_Ein) {
    this->temp_WQ_Ein_ptr = temp_WQ_Ein;
  }
void luxtronik_v1_sensor::set_temp_Kaeltekreis(sensor::Sensor* temp_Kaeltekreis) {
    this->temp_Kaeltekreis_ptr = temp_Kaeltekreis;
  }
void luxtronik_v1_sensor::set_temp_MK1_Vorl(sensor::Sensor* temp_MK1_Vorl) {
    this->temp_MK1_Vorl_ptr = temp_MK1_Vorl;
  }
void luxtronik_v1_sensor::set_temp_MK1VL_Soll(sensor::Sensor* temp_MK1VL_Soll) {
    this->temp_MK1VL_Soll_ptr = temp_MK1VL_Soll;
  }
void luxtronik_v1_sensor::set_temp_Raumstat(sensor::Sensor* temp_Raumstat) {
    this->temp_Raumstat_ptr = temp_Raumstat;
  }
void luxtronik_v1_sensor::set_ein_Abtau_Soledruck_Durchfluss(sensor::Sensor* ein_Abtau_Soledruck_Durchfluss) {
    this->ein_Abtau_Soledruck_Durchfluss_ptr = ein_Abtau_Soledruck_Durchfluss;
  }
void luxtronik_v1_sensor::set_ein_Sperrzeit_EVU(sensor::Sensor* ein_Sperrzeit_EVU) {
    this->ein_Sperrzeit_EVU_ptr = ein_Sperrzeit_EVU;
  }
void luxtronik_v1_sensor::set_ein_Hochdruckpressostat(sensor::Sensor* ein_Hochdruckpressostat) {
    this->ein_Hochdruckpressostat_ptr = ein_Hochdruckpressostat;
  }
void luxtronik_v1_sensor::set_ein_Motorschutz(sensor::Sensor* ein_Motorschutz) {
    this->ein_Motorschutz_ptr = ein_Motorschutz;
  }
void luxtronik_v1_sensor::set_ein_Niederdruckpressostat(sensor::Sensor* ein_Niederdruckpressostat) {
    this->ein_Niederdruckpressostat_ptr = ein_Niederdruckpressostat;
  }
void luxtronik_v1_sensor::set_ein_Fremdstromanode(sensor::Sensor* ein_Fremdstromanode) {
    this->ein_Fremdstromanode_ptr = ein_Fremdstromanode
  }
void luxtronik_v1_sensor::set_aus_ATV(sensor::Sensor* aus_ATV) {
    this->aus_ATV_ptr = aus_ATV;
  }
  void luxtronik_v1_sensor::set_aus_BWP(sensor::Sensor* aus_BWP) {
    this->aus_BWP_ptr = aus_BWP;
  }
void luxtronik_v1_sensor::set_aus_FBHP(sensor::Sensor* aus_FBHP) {
    this->aus_FBHP_ptr = aus_FBHP;
  }
void luxtronik_v1_sensor::set_aus_HZP(sensor::Sensor* aus_HZP) {
    this->aus_HZP_ptr = aus_HZP;
  }
void luxtronik_v1_sensor::set_aus_Mischer_1_Auf(sensor::Sensor* aus_Mischer_1_Auf) {
    this->aus_Mischer_1_Auf_ptr = aus_Mischer_1_Auf;
  }
void luxtronik_v1_sensor::set_aus_Mischer_1_Zu(sensor::Sensor* aus_Mischer_1_Zu) {
    this->aus_Mischer_1_Zu_ptr = aus_Mischer_1_Zu;
  }
void luxtronik_v1_sensor::set_aus_VentWP(sensor::Sensor* aus_VentWP) {
    this->aus_VentWP_ptr = aus_VentWP;
  }
void luxtronik_v1_sensor::set_aus_VentBrunnen(sensor::Sensor* aus_VentBrunnen) {
    this->aus_VentBrunnen_ptr = aus_VentBrunnen;
  }
void luxtronik_v1_sensor::set_aus_Verdichter_1(sensor::Sensor* aus_Verdichter_1) {
    this->aus_Verdichter_1_ptr = aus_Verdichter_1;
  }
void luxtronik_v1_sensor::set_aus_Verdichter_2(sensor::Sensor* aus_Verdichter_2) {
    this->aus_Verdichter_2_ptr = aus_Verdichter_2;
  }
void luxtronik_v1_sensor::set_aus_ZPumpe(sensor::Sensor* aus_ZPumpe) {
    this->aus_ZPumpe_ptr = aus_ZPumpe;
  }
void luxtronik_v1_sensor::set_aus_ZWE(sensor::Sensor* aus_ZWE) {
    this->aus_ZWE_ptr = aus_ZWE;
  }
void luxtronik_v1_sensor::set_aus_ZWE_Stoerung(sensor::Sensor* aus_ZWE_Stoerung) {
    this->aus_ZWE_Stoerung_ptr = aus_ZWE_Stoerung;
  }
void luxtronik_v1_sensor::set_status_Anlagentyp(sensor::Sensor* status_Anlagentyp) {
    this->status_Anlagentyp_ptr = status_Anlagentyp;
  }
void luxtronik_v1_sensor::set_status_Softwareversion(sensor::Sensor* status_Softwareversion) {
    this->status_Softwareversion_ptr = status_Softwareversion;
  }
void luxtronik_v1_sensor::set_status_Bivalenzstufe(sensor::Sensor* status_Bivalenzstufe) {
    this->status_Bivalenzstufe_ptr = status_Bivalenzstufe;
  }
void luxtronik_v1_sensor::set_status_Betriebszustand(sensor::Sensor* status_Betriebszustand) {
    this->status_Betriebszustand_ptr = status_Betriebszustand;
  }
void luxtronik_v1_sensor::set_modus_Heizung(sensor::Sensor* modus_Heizung) {
    this->modus_Heizung_ptr = modus_Heizung;
  }
void luxtronik_v1_sensor::set_modus_Warmwasser(sensor::Sensor* modus_Warmwasser) {
    this->modus_Warmwasser_ptr = modus_Warmwasser;
  }

}  // namespace esphome