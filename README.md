# 💧 Water Quality Monitoring System (ESP8266 + Firebase)

Sistem ini memonitor kualitas air menggunakan sensor suhu **DS18B20** dan sensor **TDS analog**, menampilkan data secara lokal melalui **LCD I2C**, dan mengirimkan data ke **Firebase Realtime Database** secara periodik.

---

## 🔧 Fitur

- 🌡️ Pengukuran suhu air (DS18B20)
- 🧪 Deteksi Total Dissolved Solids (TDS) dalam air
- 📺 Tampilan LCD I2C 20x4
- ☁️ Integrasi Firebase Realtime Database
- 🔁 Update data otomatis setiap 2 detik

---

## 🛠️ Komponen

| Komponen         | Jumlah |
|------------------|--------|
| ESP8266 (NodeMCU) | 1x     |
| Sensor DS18B20    | 1x     |
| Sensor TDS Analog | 1x     |
| LCD I2C 20x4      | 1x     |
| Resistor Pull-Up  | 1x (4.7kΩ pada DS18B20) |

---

## 📦 Instalasi Library

- **FirebaseESP8266** by Mobizt
- **DallasTemperature**
- **OneWire**
- **LiquidCrystal_I2C**

> Instal via **Library Manager** di Arduino IDE.

---

## ⚙️ Konfigurasi Awal

Edit bagian ini pada `main.ino`:

```cpp
#define WIFI_SSID     "masukkan_SSID_anda"
#define WIFI_PASSWORD "masukkan_PASSWORD_anda"
#define FIREBASE_HOST "masukkan_FIREBASE_HOST_anda"
#define FIREBASE_AUTH "masukkan_FIREBASE_AUTH_anda"
