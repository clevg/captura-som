#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "your_SSID";        // Substitua com sua rede Wi-Fi
const char* password = "your_PASSWORD";
WiFiUDP udp;
const char* host = "192.168.1.100";  // IP de destino
const int port = 12345;              // Porta para envio de dados

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }

  Serial.println("Conectado ao Wi-Fi");
}

void loop() {
  // Envia o áudio capturado via UDP (exemplo simples)
  udp.beginPacket(host, port);
  udp.write((byte*)audio_buffer, sizeof(audio_buffer));  // Envia o buffer de áudio
  udp.endPacket();

  // Envia o espectrograma (de forma simplificada, usando magnitudes)
  udp.beginPacket(host, port);
  udp.write((byte*)magnitudes, sizeof(magnitudes));  // Envia o espectrograma
  udp.endPacket();

  delay(100);  // Intervalo para não sobrecarregar a rede
}
