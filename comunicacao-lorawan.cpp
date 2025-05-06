#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

void setup() {
  Serial.begin(115200);

  // Configurações LoRaWAN
  os_init();
  LMIC_reset();

  // Defina a chave de rede, chave de aplicativo e DevEUI para comunicação LoRaWAN

  // Inicialize a transmissão LoRaWAN
  LMIC_setTxData2(1, "data", sizeof("data") - 1, 0);
}

void loop() {
  // O LoRaWAN envia pacotes de dados de áudio/espectrograma para outros dispositivos LoRaWAN
  // Aqui você pode incluir a lógica para enviar os dados de áudio ou espectrograma
}
