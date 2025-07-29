# 🔔 ESP32 Presence Detection and WhatsApp Alert System

This project implements a real-time presence detection system using an **ESP32**, an **ultrasonic sensor**, and **CallMeBot** to send WhatsApp alerts. The system is ideal for applications such as client arrival notifications, motion-triggered alarms, or any situation where timely presence awareness is crucial.

## 📌 Features

- 📶 Connects to WiFi and monitors proximity with an ultrasonic sensor (HC-SR04).
- 🔊 Activates a buzzer alarm for 20 seconds when presence is confirmed.
- 🕐 Ensures continuous presence for 200 milliseconds before triggering.
- 📱 Sends **three WhatsApp alerts** during the alarm period:
  - At start of detection.
  - At 10 seconds.
  - At the end of the alarm (20 seconds).
- 🔁 Automatically reactivates the system after 30 seconds of no presence.

## 🛠️ Hardware Requirements

- ✅ ESP32 Dev Board
- ✅ HC-SR04 Ultrasonic Distance Sensor
- ✅ Passive Buzzer
- ✅ Breadboard and jumper wires
- ✅ Internet connection (2.4 GHz WiFi)

## 📷 Circuit Diagram

| Component        | ESP32 Pin |
|------------------|-----------|
| HC-SR04 Trig     | GPIO 12   |
| HC-SR04 Echo     | GPIO 14   |
| Buzzer           | GPIO 13   |

> ⚠️ Use a resistor divider or voltage clamp on the **Echo pin** if your sensor outputs 5V.

## 🔧 Configuration

Before uploading the code, update the following values in the sketch:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
String phoneNumber = "+YOUR_PHONE_NUMBER";
String apiKey = "YOUR_CALLMEBOT_APIKEY";
```

You can get your API key for CallMeBot here:  
👉 [https://www.callmebot.com/blog/free-api-whatsapp-messages/](https://www.callmebot.com/blog/free-api-whatsapp-messages/)

## ⏱️ System Timing Overview

| Event                     | Time (ms) | Description                                      |
|---------------------------|-----------|--------------------------------------------------|
| Presence confirmation     | 200       | Minimum duration of continuous detection         |
| Buzzer duration           | 20000     | Alarm duration (20 seconds)                      |
| WhatsApp message #1       | 1000      | Sent at the beginning of the alarm               |
| WhatsApp message #2       | 10000     | Sent halfway through the alarm                   |
| WhatsApp message #3       | 19000     | Sent at the end of the alarm                     |
| Reactivation delay        | 30000     | System re-enables after 30s of clear detection   |

## 📤 Example Output (Serial Monitor)

```
Conectando a WiFi...
Conectado. IP: 192.168.0.20
Distancia: 42 cm
⏳ Posible presencia detectada... iniciando confirmación
✅ CLIENTE CONFIRMADO. Activando alarma.
Mensaje 1 enviado.
Mensaje 2 enviado.
Mensaje 3 enviado.
Alarma finalizada. Esperando zona despejada 15s...
✅ Zona despejada durante 15 segundos. Alarma reactivada.
```

## 📦 Libraries Used

- [WiFi.h](https://www.arduino.cc/en/Reference/WiFi)
- [Callmebot_ESP32.h](https://github.com/yours/Callmebot_ESP32) *(custom or local library for CallMeBot support)*

> ⚠️ If `Callmebot_ESP32.h` is not available in the Arduino Library Manager, be sure to include or create it manually as needed.

## 🚀 How to Upload

1. Open the code in the **Arduino IDE**.
2. Select your ESP32 board from **Tools > Board > ESP32 Dev Module**.
3. Choose the correct port.
4. Click **Upload**.

## ✅ Testing Tips

- Verify the ultrasonic sensor works correctly by observing values in the Serial Monitor.
- If WhatsApp messages do not arrive:
  - Check your API key and phone number format.
  - Ensure the device is online.
  - Add a small delay between messages if needed.

## 📄 License

This project is released under the MIT License.

---

## 👨‍💻 Author

**Daniel Alejandro Cangrejo López**  
Project developed for educational and practical IoT purposes.

---

## 🌐 Demo Ideas

- Visitor detection at store entrances.
- Pet or intruder monitoring.
- Smart doorbell alternatives.