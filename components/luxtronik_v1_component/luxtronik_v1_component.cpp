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
}

}  // namespace luxtronik_v1_component
}  // namespace esphome