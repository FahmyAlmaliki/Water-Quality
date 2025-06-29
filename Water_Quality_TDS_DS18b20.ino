/*
  ============================================
  💧 Water Quality Monitoring (ESP8266 + Firebase)
  ============================================
  Author     : FahmyAlmaliki
  Hardware   : ESP8266, DS18B20, TDS Sensor, LCD I2C
  Features   :
    - Suhu air via DS18B20
    - Kadar TDS air via sensor analog
    - LCD I2C untuk tampilan lokal
    - Firebase Realtime Database untuk monitoring jarak jauh
  ============================================
*/

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ========== Konfigurasi ==========
#define WIFI_SSID     "masukkan_SSID_anda"
#define WIFI_PASSWORD "masukkan_PASSWORD_anda"

#define FIREBASE_HOST "masukkan_FIREBASE_HOST_anda"  // Contoh: your-project-id.firebaseio.com
#define FIREBASE_AUTH "masukkan_FIREBASE_AUTH_anda"

#define TDS_SENSOR_PIN A0
#define ONE_WIRE_BUS D5

#define VREF        3.3     // Tegangan referensi
#define SAMPLE_COUNT 30     // Jumlah sampel rata-rata
#define TDS_FACTOR  0.5     // Faktor kalibrasi TDS
// =================================

// Inisialisasi objek
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

float temperature = 0.0;
float tdsValue = 0.0;

void setup() {
  Serial.begin(115200);
  ds18b20.begin();
  lcd.init();
  lcd.backlight();

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  Serial.println("\n✅ WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Konfigurasi Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  lcd.setCursor(0, 1);
  if (Firebase.ready()) {
    lcd.print("Firebase: OK");
    Serial.println("✅ Firebase ready");
  } else {
    lcd.print("Firebase: FAIL");
    Serial.println("❌ Firebase connection failed");
  }

  delay(2000);
}

void loop() {
  readTemperature();
  tdsValue = readTDSValue();

  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("❌ DS18B20 tidak terdeteksi!");
    lcd.clear();
    lcd.print("DS18B20 Error!");
    delay(1000);
    return;
  }

  Serial.printf("Suhu: %.2f C | TDS: %.2f ppm\n", temperature, tdsValue);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Suhu:%.1fC", temperature);
  lcd.setCursor(0, 1);
  lcd.printf("TDS:%.1f ppm", tdsValue);

  if (Firebase.ready()) {
    if (!Firebase.setFloat(firebaseData, "/Sensor/DS18b20", temperature))
      Serial.println("Gagal kirim suhu: " + firebaseData.errorReason());

    if (!Firebase.setFloat(firebaseData, "/Sensor/TDS", tdsValue))
      Serial.println("Gagal kirim TDS: " + firebaseData.errorReason());
  } else {
    Serial.println("❌ Firebase tidak terhubung");
  }

  delay(2000);
}

void readTemperature() {
  ds18b20.requestTemperatures();
  temperature = ds18b20.getTempCByIndex(0);
}

float readTDSValue() {
  float analogSum = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    analogSum += analogRead(TDS_SENSOR_PIN);
    delay(10);
  }
  float analogValue = analogSum / SAMPLE_COUNT;
  float voltage = analogValue * VREF / 1024.0;

  float ecValue = (133.42 * voltage * voltage * voltage +
                   255.86 * voltage * voltage +
                   857.39 * voltage) * 0.5;

  float tds = ecValue * TDS_FACTOR * (1.0 + 0.02 * (temperature - 25.0));
  return tds;
}
