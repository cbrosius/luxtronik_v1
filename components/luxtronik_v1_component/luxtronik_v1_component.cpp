#include "esphome/core/log.h"
#include "luxtronik_v1_component.h"

namespace esphome {
namespace luxtronik_v1_component {

static const char *TAG = "luxtronik_v1_component.component";

void LuxtronikV1Component::setup() {
    ESP_LOGCONFIG(TAG, "Setting up Luxtronik V1 Component...");
    if (this->parent_ == nullptr) {
        ESP_LOGE(TAG, "UART parent not set!");
        this->mark_failed();
        return;
    }

    // Request initial values immediately after setup
    this->parent_->write_str("1100\r\n");
}

void LuxtronikV1Component::loop() {
    if (this->parent_ == nullptr) {
        ESP_LOGW(TAG, "Cannot loop - UART parent not set");
        return;
    }
    
    while (this->parent_->available()) {
        uint8_t c;
        if (!this->parent_->read_byte(&c)) {
            continue;
        }
        
        if (c == ASCII_CR || c == ASCII_LF) {
            if (this->read_pos_ > 0) {
                this->read_buffer_[this->read_pos_] = '\0';
                ESP_LOGD(TAG, "Received: %s", this->read_buffer_);
                parse_message_(this->read_buffer_);
                this->read_pos_ = 0;
            }
        } else {
            this->read_buffer_[this->read_pos_++] = c;
            if (this->read_pos_ >= READ_BUFFER_LENGTH) {
                this->read_pos_ = 0;
            }
        }
    }
}

void LuxtronikV1Component::update() {
    ESP_LOGD(TAG, "Polling Luxtronik V1 Component...");
    if (this->parent_ != nullptr) {
        this->parent_->write_str("1100\r\n");
    }
}

// Helper function for deferred publishing
void LuxtronikV1Component::publish_state_deferred_(sensor::Sensor* sensor, float value, const char* type, const char* name) {
    this->defer([this, sensor, value, type, name]() {
        sensor->publish_state(value);
        ESP_LOGV(TAG, "%s %s: %.1f", type, name, value);
    });
}

void LuxtronikV1Component::parse_message_(const char* message) {
    std::string msg(message);
    
    if (msg.empty() || msg.length() < 4) {
        return;
    }

    // Use substring comparison for better performance
    std::string prefix = msg.substr(0, 4);
    
    if (prefix == "1100") {
        this->defer([this, msg]() {
            parse_temperatur_message_(msg.c_str());
        });
    } else if (prefix == "1200") {
        this->defer([this, msg]() {
            parse_input_message_(msg.c_str());
        });
    } else if (prefix == "1300") {
        this->defer([this, msg]() {
            parse_output_message_(msg.c_str());
        });
    } else if (prefix == "3405") {
        this->defer([this, msg]() {
            parse_modus_heizung_message_(msg.c_str());
        });
    } else if (prefix == "3505") {
        this->defer([this, msg]() {
            parse_modus_warmwasser_message_(msg.c_str());
        });
    } else if (prefix == "1700") {
        this->defer([this, msg]() {
            parse_status_message_(msg.c_str());
        });
    } else if (prefix == "1500") {
        this->defer([this, msg]() {
            parse_error_message_(msg.c_str());
        });
    }
}

void LuxtronikV1Component::parse_temperatur_message_(const char* message) {
    std::string msg(message);
    std::vector<std::string> values;
    size_t start = 5;  // Skip "1100;"
    size_t end = 0;
    
    // Split message into vector for faster processing
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() < 2) return;  // At least count and one value needed
    
    size_t idx = 1;  // Skip count
    auto publish_temp = [this](sensor::Sensor* sensor, const std::string& value, const char* name) {
        if (sensor != nullptr) {
            float temp = get_float_temp_(value);
            publish_state_deferred_(sensor, temp, "Temperatur", name);
        }
    };

    // Process all temperature sensors
    if (idx < values.size()) publish_temp(temperatur_vorlauf_, values[idx++], "Vorlauf");
    if (idx < values.size()) publish_temp(temperatur_ruecklauf_, values[idx++], "Rücklauf");
    if (idx < values.size()) publish_temp(temperatur_ruecklauf_soll_, values[idx++], "Rücklauf Soll");
    if (idx < values.size()) publish_temp(temperatur_heissgas_, values[idx++], "Heissgas");
    if (idx < values.size()) publish_temp(temperatur_aussen_, values[idx++], "Aussen");
    if (idx < values.size()) publish_temp(temperatur_brauchwasser_, values[idx++], "Brauchwasser");
    if (idx < values.size()) publish_temp(temperatur_brauchwasser_soll_, values[idx++], "Brauchwasser Soll");
    if (idx < values.size()) publish_temp(temperatur_waermequelle_eingang_, values[idx++], "Wärmequelle Eingang");
    if (idx < values.size()) publish_temp(temperatur_kaeltekreis_, values[idx++], "Kältekreis");
    if (idx < values.size()) publish_temp(temperatur_mischkreis1_vorlauf_, values[idx++], "Mischkreis1 Vorlauf");
    if (idx < values.size()) publish_temp(temperatur_mischkreis1_vorlauf_soll_, values[idx++], "Mischkreis1 Vorlauf Soll");
    if (idx < values.size()) publish_temp(temperatur_raumstation_, values[idx++], "Raumstation");

    // Request input values after temperature values are parsed
    this->parent_->write_str("1200\r\n");
}

void LuxtronikV1Component::parse_input_message_(const char* message) {
    ESP_LOGD(TAG, "Input message received: %s", message);
    std::string msg(message);
    std::vector<std::string> values;
    size_t start = 5;  // Skip "1200;"
    size_t end = 0;
    
    // Split message into vector for faster processing
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() < 2) return;  // At least count and one value needed
    
    size_t idx = 1;  // Skip count
    auto publish_input = [this](sensor::Sensor* sensor, const std::string& value, const char* name) {
        if (sensor != nullptr) {
            float val = std::atof(value.c_str());
            publish_state_deferred_(sensor, val, "Input", name);
        }
    };

    // Process all input sensors
    if (idx < values.size()) publish_input(eingang_abtau_soledruck_durchfluss_, values[idx++], "Abtau Soledruck Durchfluss");
    if (idx < values.size()) publish_input(eingang_sperrzeit_evu_, values[idx++], "Sperrzeit EVU");
    if (idx < values.size()) publish_input(eingang_hochdruckpressostat_, values[idx++], "Hochdruckpressostat");
    if (idx < values.size()) publish_input(eingang_motorschutz_, values[idx++], "Motorschutz");
    if (idx < values.size()) publish_input(eingang_niederdruckpressostat_, values[idx++], "Niederdruckpressostat");
    if (idx < values.size()) publish_input(eingang_fremdstromanode_, values[idx++], "Fremdstromanode");

    // Request output values after input values are parsed
    this->parent_->write_str("1300\r\n");
}

void LuxtronikV1Component::parse_output_message_(const char* message) {
    ESP_LOGD(TAG, "Output message received: %s", message);
    std::string msg(message);
    std::vector<std::string> values;
    size_t start = 5;  // Skip "1300;"
    size_t end = 0;
    
    // Split message into vector for faster processing
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() < 2) return;  // At least count and one value needed
    
    size_t idx = 1;  // Skip count
    auto publish_output = [this](sensor::Sensor* sensor, const std::string& value, const char* name) {
        if (sensor != nullptr) {
            float val = std::atof(value.c_str());
            publish_state_deferred_(sensor, val, "Output", name);
        }
    };

    // Process all output sensors
    if (idx < values.size()) publish_output(ausgang_abtauventil_, values[idx++], "Abtauventil");
    if (idx < values.size()) publish_output(ausgang_brauchwasserpumpe_, values[idx++], "BWP");
    if (idx < values.size()) publish_output(ausgang_fussbodenheizungspumpe_, values[idx++], "FBHP");
    if (idx < values.size()) publish_output(ausgang_heizungspumpe_, values[idx++], "HZP");
    if (idx < values.size()) publish_output(ausgang_mischer_1_auf_, values[idx++], "Mischer 1 Auf");
    if (idx < values.size()) publish_output(ausgang_mischer_1_zu_, values[idx++], "Mischer 1 Zu");
    if (idx < values.size()) publish_output(ausgang_ventilator_waermepumpe_, values[idx++], "Vent WP");
    if (idx < values.size()) publish_output(ausgang_ventilator_brunnen_, values[idx++], "Vent Brunnen");
    if (idx < values.size()) publish_output(ausgang_verdichter_1_, values[idx++], "Verdichter 1");
    if (idx < values.size()) publish_output(ausgang_verdichter_2_, values[idx++], "Verdichter 2");
    if (idx < values.size()) publish_output(ausgang_zirkulationspumpe_, values[idx++], "ZPumpe");
    if (idx < values.size()) publish_output(ausgang_zweiter_waermeerzeuger_, values[idx++], "ZWE");
    if (idx < values.size()) publish_output(ausgang_zweiter_waermeerzeuger_stoerung_, values[idx++], "ZWE Störung");

    // Request heizungs-modus after input values are parsed
    this->parent_->write_str("3405\r\n");

}

// Add helper function implementation
std::string LuxtronikV1Component::get_modus_text_(int state) {
    switch (state) {
        case 0: return "Automatik";
        case 1: return "Zweiter Waermeerzeuger";
        case 2: return "Party";
        case 3: return "Ferien";
        case 4: return "Aus";
        default: return "Unbekannt";
    }
}

// Update parse_modus_heizung_message_
void LuxtronikV1Component::parse_modus_heizung_message_(const char* message) {
    std::string msg(message);
    std::vector<std::string> values;
    values.reserve(3);  // Pre-allocate for typical message size
    size_t start = 5;  // Skip "3405;"
    size_t end = 0;
    
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    // Add final value if exists
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() >= 2) {  // At least count and mode value
        float val = std::atof(values[1].c_str());
        if (modus_heizung_numerisch_ != nullptr) {
            publish_state_deferred_(modus_heizung_numerisch_, val, "Mode", "Heizung Numerisch");
        }
        if (modus_heizung_ != nullptr) {
            std::string mode_text = get_modus_text_(static_cast<int>(val));
            this->defer([this, mode_text]() {
                modus_heizung_->publish_state(mode_text);
                ESP_LOGV(TAG, "Mode Heizung: %s", mode_text.c_str());
            });
        }
    }
    
    // Request hot water mode after heating mode
    this->parent_->write_str("3505\r\n");
}

// Update parse_modus_warmwasser_message_
void LuxtronikV1Component::parse_modus_warmwasser_message_(const char* message) {
    std::string msg(message);
    std::vector<std::string> values;
    values.reserve(3);  // Pre-allocate for typical message size
    size_t start = 5;  // Skip "3505;"
    size_t end = 0;
    
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    // Add final value if exists
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() >= 2) {  // At least count and mode value
        float val = std::atof(values[1].c_str());
        if (modus_warmwasser_numerisch_ != nullptr) {
            publish_state_deferred_(modus_warmwasser_numerisch_, val, "Mode", "Warmwasser Numerisch");
        }
        if (modus_warmwasser_ != nullptr) {
            std::string mode_text = get_modus_text_(static_cast<int>(val));
            this->defer([this, mode_text]() {
                modus_warmwasser_->publish_state(mode_text);
                ESP_LOGV(TAG, "Mode Warmwasser: %s", mode_text.c_str());
            });
        }
    }
    
    // Request status values after warmwater mode
    this->parent_->write_str("1700\r\n");
}

std::string LuxtronikV1Component::get_betriebszustand_text_(int state) {
    switch (state) {
        case 0: return "Heizen";
        case 1: return "Warmwasser";
        case 3: return "EVU Sperre";
        case 5: return "Bereitschaft";
        default: return "Unbekannt";
    }
}

void LuxtronikV1Component::parse_status_message_(const char* message) {
    std::string msg(message);
    std::vector<std::string> values;
    values.reserve(13);  // Pre-allocate for all status values
    size_t start = 5;  // Skip "1700;"
    size_t end = 0;
    
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }
    
    if (values.size() < 2) return;  // At least count and one value needed
    
    size_t idx = 1;  // Skip count

    auto publish_status = [this](sensor::Sensor* sensor, const std::string& value, const char* name) {
        if (sensor != nullptr) {
            float val = std::atof(value.c_str());
            publish_state_deferred_(sensor, val, "Status", name);
        }
    };

    // Process Anlagentyp
    if (idx < values.size()) publish_status(status_anlagentyp_, values[idx++], "Anlagentyp");

    // special handling because Softwareversion is a string
    if (idx < values.size()) {
        if (status_softwareversion_ != nullptr) {
            this->defer([this, value = values[idx]]() {
                status_softwareversion_->publish_state(value);
                ESP_LOGV(TAG, "Status Softwareversion: %s", value.c_str());
            });
        }
        idx++;
    }

    // Process Bivalenzstufe
    if (idx < values.size()) publish_status(status_bivalenzstufe_, values[idx++], "Bivalenzstufe");

    // special handling because Betriebszustand has a numeric and a string value
    if (idx < values.size()) {
        float val = std::atof(values[idx].c_str());
        if (status_betriebszustand_numerisch_ != nullptr) {
            publish_state_deferred_(status_betriebszustand_numerisch_, val, "Status", "Betriebszustand Numerisch");
        }
        if (status_betriebszustand_ != nullptr) {
            std::string state_text = get_betriebszustand_text_(static_cast<int>(val));
            this->defer([this, state_text]() {
                status_betriebszustand_->publish_state(state_text);
                ESP_LOGV(TAG, "Status Betriebszustand: %s", state_text.c_str());
            });
        }
        idx++;
    }

    // Process Letzter Start
    if (idx < values.size() && status_letzter_start_ != nullptr) {
        int tag = std::atoi(values[idx++].c_str());
        int monat = std::atoi(values[idx++].c_str());
        int jahr = std::atoi(values[idx++].c_str());
        int stunde = std::atoi(values[idx++].c_str());
        int minute = std::atoi(values[idx++].c_str());
        int sekunde = std::atoi(values[idx++].c_str());
        
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d:%02d", 
                 tag, monat, jahr, stunde, minute, sekunde);
        
        this->defer([this, text = std::string(buffer)]() {
            status_letzter_start_->publish_state(text);
            ESP_LOGV(TAG, "Status Letzter Start: %s", text.c_str());
        });
    }

    // Request error values after status values are parsed
    this->parent_->write_str("1500\r\n");
}

void LuxtronikV1Component::parse_error_message_(const char* message) {
    std::string msg(message);
    std::vector<std::string> values;
    size_t start = 5;  // Skip "1500;"
    size_t end = 0;

    // Split message into vector for faster processing
    while ((end = msg.find(';', start)) != std::string::npos) {
        values.push_back(msg.substr(start, end - start));
        start = end + 1;
    }

    if (start < msg.length()) {
        values.push_back(msg.substr(start));
    }

    if (values.size() < 3) return;  // At least count and one value needed

    auto publish_output = [this](sensor::Sensor* sensor, const std::string& value, const char* name) {
        if (sensor != nullptr) {
            float val = std::atof(value.c_str());
            publish_state_deferred_(sensor, val, "Output", name);
        }
    };

    // Process Fehlerindex
    size_t idx = 0;  // get error index
    ESP_LOGD(TAG, "Error index: %s", values[idx].c_str());
    int error_index = std::atoi(values[idx].c_str());
    switch (error_index) {
        case 1500: {
            // skip Fehlerindex
            idx++;
            // skip Count
            idx++;
            // Process Fehlercode
            if (idx < values.size()) publish_output(error0_fehlercode_, values[idx], "error0_fehlercode");
            // Process Fehlerbeschreibung
            if (idx < values.size() && error0_fehlerbeschreibung_ != nullptr) {
                std::string error_text = get_error_description_(std::atoi(values[idx].c_str()));
                this->defer([this, error_text]() {
                    error0_fehlerbeschreibung_->publish_state(error_text);
                    ESP_LOGD(TAG, "Error0 Fehlerbeschreibung: %s", error_text.c_str());
                });
                idx++; // Moved here to advance only when description is processed
            }
              // Process Fehlerzeitpunkt
            if (idx < values.size() && error0_zeitpunkt_ != nullptr) {
              int tag = std::atoi(values[idx++].c_str());
              int monat = std::atoi(values[idx++].c_str());
              int jahr = std::atoi(values[idx++].c_str());
              int stunde = std::atoi(values[idx++].c_str());
              int minute = std::atoi(values[idx++].c_str());

              char buffer[32];
              snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d",
                    tag, monat, jahr, stunde, minute);

              this->defer([this, text = std::string(buffer)]() {
                  error0_zeitpunkt_->publish_state(text);
                  ESP_LOGD(TAG, "Error0 Zeitpunkt: %s", text.c_str());
              });
            }
            break;
        }
        case 1501: {
            // skip Fehlerindex
            idx++;
            // skip Count
            idx++;
            // Process Fehlercode
            if (idx < values.size()) publish_output(error1_fehlercode_, values[idx], "error1_fehlercode");
            // Process Fehlerbeschreibung
            if (idx < values.size() && error1_fehlerbeschreibung_ != nullptr) {
                std::string error_text = get_error_description_(std::atoi(values[idx].c_str()));
                this->defer([this, error_text]() {
                    error1_fehlerbeschreibung_->publish_state(error_text);
                    ESP_LOGD(TAG, "Error1 Fehlerbeschreibung: %s", error_text.c_str());
                });
                idx++; // Moved here to advance only when description is processed
            }
            // Process Fehlerzeitpunkt
            if (idx < values.size() && error1_zeitpunkt_ != nullptr) {
                int tag = std::atoi(values[idx++].c_str());
                int monat = std::atoi(values[idx++].c_str());
                int jahr = std::atoi(values[idx++].c_str());
                int stunde = std::atoi(values[idx++].c_str());
                int minute = std::atoi(values[idx++].c_str());

                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d",
                        tag, monat, jahr, stunde, minute);

                this->defer([this, text = std::string(buffer)]() {
                    error1_zeitpunkt_->publish_state(text);
                    ESP_LOGD(TAG, "Error1 Zeitpunkt: %s", text.c_str());
                });
            }
            break;
        }
        case 1502: {
            // skip Fehlerindex
            idx++;
            // skip Count
            idx++;
            // Process Fehlercode
            if (idx < values.size()) publish_output(error2_fehlercode_, values[idx], "error2_fehlercode");
            // Process Fehlerbeschreibung
            if (idx < values.size() && error2_fehlerbeschreibung_ != nullptr) {
                std::string error_text = get_error_description_(std::atoi(values[idx].c_str()));
                this->defer([this, error_text]() {
                    error2_fehlerbeschreibung_->publish_state(error_text);
                    ESP_LOGV(TAG, "Error2 Fehlerbeschreibung: %s", error_text.c_str());
                });
                idx++; // Moved here to advance only when description is processed
            }
             // Process Fehlerzeitpunkt
            if (idx < values.size() && error2_zeitpunkt_ != nullptr) {
                int tag = std::atoi(values[idx++].c_str());
                int monat = std::atoi(values[idx++].c_str());
                int jahr = std::atoi(values[idx++].c_str());
                int stunde = std::atoi(values[idx++].c_str());
                int minute = std::atoi(values[idx++].c_str());

                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d",
                        tag, monat, jahr, stunde, minute);

                this->defer([this, text = std::string(buffer)]() {
                    error2_zeitpunkt_->publish_state(text);
                    ESP_LOGV(TAG, "Error2 Zeitpunkt: %s", text.c_str());
                });
            }
            break;
        }
        case 1503: {
            // skip Fehlerindex
            idx++;
            // skip Count
            idx++;
            // Process Fehlercode
            if (idx < values.size()) publish_output(error3_fehlercode_, values[idx], "error3_fehlercode");
            // Process Fehlerbeschreibung
            if (idx < values.size() && error3_fehlerbeschreibung_ != nullptr) {
                std::string error_text = get_error_description_(std::atoi(values[idx].c_str()));
                this->defer([this, error_text]() {
                    error3_fehlerbeschreibung_->publish_state(error_text);
                    ESP_LOGD(TAG, "Error3 Fehlerbeschreibung: %s", error_text.c_str());
                });
                idx++; // Moved here to advance only when description is processed
            }
            // Process Fehlerzeitpunkt
            if (idx < values.size() && error3_zeitpunkt_ != nullptr) {
                int tag = std::atoi(values[idx++].c_str());
                int monat = std::atoi(values[idx++].c_str());
                int jahr = std::atoi(values[idx++].c_str());
                int stunde = std::atoi(values[idx++].c_str());
                int minute = std::atoi(values[idx++].c_str());
                ESP_LOGD(TAG, "Error3 tag: %d", tag);
                ESP_LOGD(TAG, "Error3 monat: %d", monat);
                ESP_LOGD(TAG, "Error3 jahr: %d", jahr);
                ESP_LOGD(TAG, "Error3 stunde: %d", stunde);
                ESP_LOGD(TAG, "Error3 minute: %d", minute);

                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d",
                        tag, monat, jahr, stunde, minute);

                this->defer([this, text = std::string(buffer)]() {
                    error3_zeitpunkt_->publish_state(text);
                    ESP_LOGD(TAG, "Error3 Zeitpunkt: %s", text.c_str());
                });
            }
            break;
        }
        case 1504: {
            // skip Fehlerindex
            idx++;
            // skip Count
            idx++;
            // Process Fehlercode
            if (idx < values.size()) publish_output(error4_fehlercode_, values[idx], "error4_fehlercode");
            // Process Fehlerbeschreibung
            if (idx < values.size() && error4_fehlerbeschreibung_ != nullptr) {
                std::string error_text = get_error_description_(std::atoi(values[idx].c_str()));
                this->defer([this, error_text]() {
                    error4_fehlerbeschreibung_->publish_state(error_text);
                    ESP_LOGV(TAG, "Error4 Fehlerbeschreibung: %s", error_text.c_str());
                });
                idx++; // Moved here to advance only when description is processed
            }
             // Process Fehlerzeitpunkt
            if (idx < values.size() && error4_zeitpunkt_ != nullptr) {
                int tag = std::atoi(values[idx++].c_str());
                int monat = std::atoi(values[idx++].c_str());
                int jahr = std::atoi(values[idx++].c_str());
                int stunde = std::atoi(values[idx++].c_str());
                int minute = std::atoi(values[idx++].c_str());
                ESP_LOGD(TAG, "Error4 tag: %d", tag);
                ESP_LOGD(TAG, "Error4 monat: %d", monat);
                ESP_LOGD(TAG, "Error4 jahr: %d", jahr);
                ESP_LOGD(TAG, "Error4 stunde: %d", stunde);
                ESP_LOGD(TAG, "Error4 minute: %d", minute);

                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d %02d:%02d",
                        tag, monat, jahr, stunde, minute);

                this->defer([this, text = std::string(buffer)]() {
                    error4_zeitpunkt_->publish_state(text);
                    ESP_LOGV(TAG, "Error4 Zeitpunkt: %s", text.c_str());
                });
            }
            break;
        }
        default: {
            break;
        }
    }
}

void LuxtronikV1Component::dump_config() {
    ESP_LOGCONFIG(TAG, "Luxtronik V1 Component:");
    ESP_LOGCONFIG(TAG, "  UART Parent: %s", this->parent_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Vorlauf: %s", this->temperatur_vorlauf_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Rücklauf: %s", this->temperatur_ruecklauf_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Rücklauf Soll: %s", this->temperatur_ruecklauf_soll_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Heissgas: %s", this->temperatur_heissgas_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Aussen: %s", this->temperatur_aussen_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Brauchwasser: %s", this->temperatur_brauchwasser_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Brauchwasser Soll: %s", this->temperatur_brauchwasser_soll_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Wärmequelle Eingang: %s", this->temperatur_waermequelle_eingang_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Kältekreis: %s", this->temperatur_kaeltekreis_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Mischkreis1 Vorlauf: %s", this->temperatur_mischkreis1_vorlauf_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Mischkreis1 Vorlauf Soll: %s", this->temperatur_mischkreis1_vorlauf_soll_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Temperatur Raumstation: %s", this->temperatur_raumstation_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Abtau Soledruck Durchfluss: %s", this->eingang_abtau_soledruck_durchfluss_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Sperrzeit EVU: %s", this->eingang_sperrzeit_evu_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Hochdruckpressostat: %s", this->eingang_hochdruckpressostat_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Motorschutz: %s", this->eingang_motorschutz_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Niederdruckpressostat: %s", this->eingang_niederdruckpressostat_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Eingang Fremdstromanode: %s", this->eingang_fremdstromanode_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Abtauventil: %s", this->ausgang_abtauventil_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Brauchwasserpumpe: %s", this->ausgang_brauchwasserpumpe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Fussbodenheizungspumpe: %s", this->ausgang_fussbodenheizungspumpe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Heizungspumpe: %s", this->ausgang_heizungspumpe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Mischer 1 Auf: %s", this->ausgang_mischer_1_auf_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Mischer 1 Zu: %s", this->ausgang_mischer_1_zu_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Ventilator Waermepumpe: %s", this->ausgang_ventilator_waermepumpe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Ventilator Brunnen: %s", this->ausgang_ventilator_brunnen_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Verdichter 1: %s", this->ausgang_verdichter_1_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Verdichter 2: %s", this->ausgang_verdichter_2_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Zirkulationspumpe: %s", this->ausgang_zirkulationspumpe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Zweiter Wärmeerzeuger: %s", this->ausgang_zweiter_waermeerzeuger_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Ausgang Zweiter Wärmeerzeuger Störung: %s", this->ausgang_zweiter_waermeerzeuger_stoerung_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Modus Heizung Numerisch: %s", this->modus_heizung_numerisch_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Modus Heizung: %s", this->modus_heizung_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Modus Warmwasser Numerisch: %s", this->modus_warmwasser_numerisch_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Modus Warmwasser: %s", this->modus_warmwasser_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Anlagentyp: %s", this->status_anlagentyp_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Softwareversion: %s", this->status_softwareversion_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Bivalenzstufe: %s", this->status_bivalenzstufe_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Betriebszustand Numerisch: %s", this->status_betriebszustand_numerisch_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Betriebszustand: %s", this->status_betriebszustand_ ? "Set" : "Not Set");
    ESP_LOGCONFIG(TAG, "  Sensor Status Letzter Start: %s", this->status_letzter_start_ ? "Set" : "Not Set");
}

std::string LuxtronikV1Component::get_error_description_(int error_code) {
    switch (error_code) {
        case 701: return "Niederdruckstörung - Niederdruckpressostat oder -sensor hat mehrfach ausgelöst.";
        case 702: return "Niederdrucksperre (Reset auto.) - Niederdruck hat angesprochen, automatischer Neustart.";
        case 703: return "Frostschutz - Vorlauftemperatur < 5°C erkannt.";
        case 704: return "Heißgasstörung - Max. Temperatur im Heißgaskreis überschritten.";
        case 705: return "Motorschutz VEN - Motorschutz des Ventilators hat ausgelöst.";
        case 706: return "Motorschutz BSUP - Motorschutz der Sole-/Brunnenwasserpumpe oder Verdichter.";
        case 707: return "Kodierungsfehler WP - Kodierungswiderstand oder Verbindung fehlerhaft.";
        case 708: return "Fühler Rücklauf - Bruch/Kurzschluss des Rücklauffühlers.";
        case 709: return "Fühler Vorlauf - Bruch/Kurzschluss des Vorlauffühlers.";
        case 710: return "Fühler Heißgas - Bruch/Kurzschluss des Heißgasfühlers.";
        case 711: return "Fühler Außentemperatur - Bruch/Kurzschluss des Außentemperaturfühlers.";
        case 712: return "Fühler Trinkwarmwasser - Bruch/Kurzschluss des Trinkwarmwasserfühlers.";
        case 713: return "Fühler WQ-Eintritt - Bruch/Kurzschluss des Wärmequellenfühlers (Eintritt).";
        case 714: return "Heißgas WW - Temperaturgrenze Trinkwarmwasser überschritten.";
        case 715: return "Hochdruck-Abschaltung (Reset) - Hochdruckpressostat hat angesprochen.";
        case 716: return "Hochdruckstörung - Hochdruckpressostat mehrfach angesprochen.";
        case 717: return "Durchfluss-WQ - Durchflussschalter hat angesprochen.";
        case 718: return "Max. Außentemp. (Reset) - Außentemperatur überschritten.";
        case 719: return "Min. Außentemp. (Reset) - Außentemperatur unterschritten.";
        case 720: return "WQ-Temperatur (Reset) - Verdampferaustrittstemp. mehrfach unter Sicherheitswert.";
        case 721: return "Niederdruckabschaltung (Reset) - Niederdruckpressostat oder -sensor hat angesprochen.";
        case 722: return "Tempdiff Heizwasser - Temperaturspreizung im Heizbetrieb ist negativ.";
        case 723: return "Tempdiff Warmw. - Temperaturspreizung im Trinkwarmwasserbetrieb ist negativ.";
        case 724: return "Tempdiff Abtauen - Temperaturspreizung im Heizkreis ist während des Abtauens > 15 K.";
        case 725: return "Anlagefehler WW - Trinkwarmwasserbetrieb gestört, gewünschte Speichertemperatur ist weit unterschritten.";
        case 726: return "Fühler Mischkreis 1 - Bruch oder Kurzschluss des Mischkreisfühlers.";
        case 727: return "Soledruck - Soledruckpressostat hat angesprochen.";
        case 728: return "Fühler WQ-Aus - Bruch oder Kurzschluss des Wärmequellenfühlers (Austritt).";
        case 729: return "Drehfeldfehler - Verdichter nach dem Einschalten ohne Leistung.";
        case 730: return "Leistung Ausheizen - Ausheizprogramm konnte eine VL-Temperaturstufe nicht erreichen.";
        case 731: return "Zeitüberschreitung TDI - Thermische Desinfektion konnte nicht durchgeführt werden.";
        case 732: return "Störung Kühlung - Heizwassertemperatur von 16°C mehrfach unterschritten.";
        case 733: return "Störung Anode - Störmeldeeingang der Fremdstromanode hat angesprochen.";
        case 734: return "Störung Anode - Fehler liegt seit mehr als zwei Wochen an, Trinkwarmwasserbereitung gesperrt.";
        case 735: return "Fühler Ext. En - Bruch oder Kurzschluss des Fühlers 'Externe Energiequelle' (TEE).";
        case 736: return "Fühler Solarkollektor - Bruch oder Kurzschluss des Solarkollektorfühlers.";
        case 737: return "Fühler Solarspeicher - Bruch oder Kurzschluss des Solarspeicherfühlers.";
        case 738: return "Fühler Mischkreis 2 - Bruch oder Kurzschluss des Mischkreisfühlers 2.";
        case 739: return "Fühler Mischkreis 3 - Bruch oder Kurzschluss des Mischkreisfühlers 3.";
        case 750: return "Fühler Rücklauf extern - Bruch oder Kurzschluss des externen Rücklauffühlers.";
        case 751: return "Phasenüberwachungsfehler - Phasenfolgerelais hat angesprochen.";
        case 752: return "Phasenüberwachungs-/Durchflussfehler - Phasenfolgerelais oder Durchflussschalter hat angesprochen.";
        case 755: return "Verbindung zu Slave verloren - Ein Slave hat für mehr als 5 Minuten nicht geantwortet.";
        case 756: return "Verbindung zu Master verloren - Master hat für mehr als 5 Minuten nicht geantwortet.";
        case 757: return "ND-Störung bei W/W-Gerät - Niederdruckpressostat hat mehrfach oder länger als 20 Sekunden angesprochen.";
        case 758: return "Störung Abtauung - Abtauung wurde 5-mal in Folge zu niedriger Vorlauftemperatur beendet.";
        case 759: return "Meldung TDI - Thermische Desinfektion konnte nicht korrekt durchgeführt werden.";
        case 760: return "Störung Abtauung - Abtauung wurde 5-mal in Folge über Maximalzeit beendet.";
        case 761: return "LIN-Verbindung unterbrochen - LIN-Timeout.";
        case 762: return "Fühler Ansaug Verdichter - Fühlerfehler Tü (Ansaug Verdichter).";
        case 763: return "Fühler Ansaug-Verdampfer - Fühlerfehler Tü1 (Ansaug Verdampfer).";
        case 764: return "Fühler Verdichterheizung - Fühlerfehler Verdichterheizung.";
        case 765: return "Überhitzung - Überhitzung länger als 5 Minuten unter 2K.";
        case 766: return "Einsatzgrenzen-VD - Betrieb 5 Minuten außerhalb des Einsatzbereichs des Verdichters.";
        case 767: return "STB E-Stab - STB des Heizstabs wurde aktiviert.";
        case 768: return "Durchflussüberwachung - 5-mal zu geringer Durchfluss vor Abtauung.";
        case 769: return "Pumpenansteuerung - Kein gültiges Durchflusssignal von der Umwälzpumpe.";
        case 770: return "Niedrige Überhitzung - Überhitzung über längere Zeit unter Grenzwert.";
        case 771: return "Hohe Überhitzung - Überhitzung über längere Zeit über Grenzwert.";
        case 776: return "Einsatzgrenzen-VD - Verdichter läuft außerhalb Einsatzgrenzen.";
        case 777: return "Expansionsventil - Expansionsventil defekt.";
        case 778: return "Fühler Niederdruck - Niederdruckfühler defekt.";
        case 779: return "Fühler Hochdruck - Hochdruckfühler defekt.";
        case 780: return "Fühler EVI - EVI-Fühler defekt.";
        case 799: return "ModBus ASB - Keine ModBus-Kommunikation mit ASB-Platine.";
        default: return "Unbekannter Fehler";
    }
}

}  // namespace luxtronik_v1_component
}  // namespace esphome