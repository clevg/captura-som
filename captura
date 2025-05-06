#include <I2S.h>

#define I2S_DATA_IN_PIN 32   // Pino de dados do microfone (SD)
#define I2S_LRCK_PIN   15   // Pino de clock de esquerda/direita (LRCL)
#define I2S_SCK_PIN    14   // Pino de clock (SCK)

#define SAMPLE_RATE    16000  // Taxa de amostragem em Hz
#define BUFFER_SIZE    1024   // Tamanho do buffer de áudio

int16_t audio_buffer[BUFFER_SIZE];  // Buffer para armazenar áudio

void setup() {
  Serial.begin(115200);

  // Inicializa o I2S
  I2S.begin(I2S_PHILIPS_MODE, SAMPLE_RATE, 16);  // Modo Philips, 16 bits por amostra

  pinMode(I2S_DATA_IN_PIN, INPUT);
  pinMode(I2S_LRCK_PIN, INPUT);
  pinMode(I2S_SCK_PIN, INPUT);
}

void loop() {
  int bytesRead = I2S.read(audio_buffer, sizeof(audio_buffer));  // Lê os dados de áudio no buffer

  if (bytesRead > 0) {
    // Aqui o áudio foi capturado e agora pode ser processado ou enviado
    // Processar o áudio ou gerar espectrograma
  }

  delay(100);  // Pequeno atraso para não sobrecarregar o loop
}
