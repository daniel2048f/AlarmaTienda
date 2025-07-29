#include <WiFi.h>
#include <Callmebot_ESP32.h>

// ======= CONFIGURACIÓN DE RED Y WHATSAPP =======
const char* ssid = "CRISPETA";
const char* password = "ACL4712707";
String phoneNumber = "+573118639860";
String apiKey = "8623018";

// ======= PINES =======
const int trigPin = 12;
const int echoPin = 14;
const int buzzerPin = 13;

// ======= VARIABLES DE CONTROL =======
const unsigned long tiempoAlarma = 20000;         // 20 segundos de alarma
const unsigned long tiempoDesactivacion = 30000;  // 30 s sin presencia para reactivar
const unsigned long tiempoConfirmacion = 200;     // 0.2 s de presencia constante para activar
unsigned long tiempoInicioAlarma = 0;
unsigned long tiempoUltimaDeteccionCerca = 0;
unsigned long tiempoInicioConfirmacion = 0;
bool enProcesoDeConfirmacion = false;
bool alarmaActiva = true;
bool eventoEnProgreso = false;
unsigned long ultimoBip = 0;
bool buzzerActivo = false;
bool mensaje1 = false;
bool mensaje2 = false;
bool mensaje3 = false;

// ======= SETUP =======
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado. IP: " + WiFi.localIP().toString());
}

// ======= MEDIR DISTANCIA =======
long medirDistanciaCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH, 25000); // Timeout 25 ms
  return duracion * 0.034 / 2;
}

// ======= LOOP PRINCIPAL =======
void loop() {
  long distancia = medirDistanciaCM();
  unsigned long ahora = millis();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // === Verificación de activación con presencia continua ===
  if (alarmaActiva && !eventoEnProgreso) {
    if (distancia > 0 && distancia < 60) {
      if (!enProcesoDeConfirmacion) {
        tiempoInicioConfirmacion = ahora;
        enProcesoDeConfirmacion = true;
        Serial.println("⏳ Posible presencia detectada... iniciando confirmación");
      } else {
        if (ahora - tiempoInicioConfirmacion >= tiempoConfirmacion) {
          // Confirmación completada: activar la alarma
          eventoEnProgreso = true;
          tiempoInicioAlarma = ahora;
          ultimoBip = ahora;
          buzzerActivo = false;
          mensaje1 = false;
          mensaje2 = false;
          mensaje3 = false;
          enProcesoDeConfirmacion = false;
          Serial.println("✅ CLIENTE CONFIRMADO. Activando alarma.");
        }
      }
    } else {
      enProcesoDeConfirmacion = false;  // Se cancela si se pierde la detección
    }
  }

  // === Mientras la alarma está activa ===
  if (eventoEnProgreso) {
    unsigned long tiempoTranscurrido = ahora - tiempoInicioAlarma;

    if (tiempoTranscurrido <= tiempoAlarma) {
      // Buzzer intermitente cada 300 ms
      if (ahora - ultimoBip >= 300) {
        if (buzzerActivo) {
          noTone(buzzerPin);
          buzzerActivo = false;
        } else {
          tone(buzzerPin, 3000);
          buzzerActivo = true;
        }
        ultimoBip = ahora;
      }

      // Envío de los 3 mensajes dentro de los 20 segundos
      if (!mensaje1 && tiempoTranscurrido >= 1000) {
        Callmebot.whatsappMessage(phoneNumber, apiKey, "🔔 CLIENTE DETECTADO (inicio de alarma).");
        Serial.println("Mensaje 1 enviado.");
        mensaje1 = true;
      }
      if (!mensaje2 && tiempoTranscurrido >= 10000) {
        Callmebot.whatsappMessage(phoneNumber, apiKey, "🔔 CLIENTE DETECTADO (mitad de alarma).");
        Serial.println("Mensaje 2 enviado.");
        mensaje2 = true;
      }
      if (!mensaje3 && tiempoTranscurrido >= 19000) {
        Callmebot.whatsappMessage(phoneNumber, apiKey, "🔔 CLIENTE DETECTADO (fin de alarma).");
        Serial.println("Mensaje 3 enviado.");
        mensaje3 = true;
      }

    } else {
      // Fin del evento
      noTone(buzzerPin);
      eventoEnProgreso = false;
      alarmaActiva = false;
      tiempoUltimaDeteccionCerca = ahora;
      Serial.println("Alarma finalizada. Esperando zona despejada 15s...");
    }
  }

  // === Reactivación de la alarma solo si no se detecta nada cerca por 15 segundos ===
  if (!alarmaActiva) {
    if (distancia > 0 && distancia < 60) {
      tiempoUltimaDeteccionCerca = ahora;  // Reinicia el conteo si ve algo
    }

    if (ahora - tiempoUltimaDeteccionCerca >= tiempoDesactivacion) {
      alarmaActiva = true;
      Serial.println("✅ Zona despejada durante 15 segundos. Alarma reactivada.");
    }
  }

  delay(100);
}
