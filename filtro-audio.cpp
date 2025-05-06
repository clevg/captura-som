#include <WiFi.h>
#include <I2S.h>
#include <FFT.h>

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 1024
#define FFT_SIZE 128
#define FILTER_SIZE 10  // Para filtro de média móvel
#define ALPHA 0.1  // Constante de filtro passa-alto

// Pinos para o I2S (INMP441)
#define I2S_DATA_IN_PIN 32   
#define I2S_LRCK_PIN   15   
#define I2S_SCK_PIN    14   

int16_t audio_buffer[BUFFER_SIZE];  // Buffer de áudio
float real[FFT_SIZE];
float imag[FFT_SIZE];
float magnitudes[FFT_SIZE / 2];

float filter_buffer[FILTER_SIZE];  // Buffer para filtro de média móvel
int filter_index = 0;  // Índice do filtro de média móvel
float previous_sample = 0;  // Para o filtro passa-alto

// Função de filtro passa-baixas (Low-Pass Filter)
float applyLowPassFilter(int16_t sample) {
  filter_buffer[filter_index] = (float)sample;
  filter_index = (filter_index + 1) % FILTER_SIZE;

  float sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    sum += filter_buffer[i];
  }
  return sum / FILTER_SIZE;
}

// Função de filtro passa-alto (High-Pass Filter)
float applyHighPassFilter(int16_t sample) {
  float filtered_sample = sample - previous_sample + ALPHA * previous_sample;
  previous_sample = sample;
  return filtered_sample;
}

// Função para processar o áudio e aplicar FFT
void processAudio(int16_t* buffer, int bufferSize) {
  for (int i = 0; i < FFT_SIZE && i < bufferSize; i++) {
    real[i] = (float) buffer[i];
    imag[i] = 0;
  }

  FFT.Windowing(real, FFT_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(real, imag, FFT_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(real, imag, magnitudes, FFT_SIZE);

  // Aqui, você pode adicionar condições para filtrar certas frequências (e.g., remover 50Hz)
  // Exemplo simples de filtro: eliminar frequências abaixo de 100 Hz
  for (int i = 0; i < FFT_SIZE / 2; i++) {
    if (i < 10) {  // Filtro simples para eliminar as frequências baixas
      magnitudes[i] = 0;
    }
  }

  // Reconstruir o sinal filtrado
  FFT.MagnitudeToComplex(real, imag, magnitudes, FFT_SIZE);
  FFT.Compute(real, imag, FFT_SIZE, FFT_INVERSE);
}

// Função principal para capturar e processar áudio
void captureAndProcessAudio() {
  int bytesRead = I2S.read(audio_buffer, sizeof(audio_buffer));
  
  if (bytesRead > 0) {
    for (int i = 0; i < bytesRead; i++) {
      // Aplique o filtro passa-baixas
      audio_buffer[i] = applyLowPassFilter(audio_buffer[i]);
      // Aplique o filtro passa-alto
      audio_buffer[i] = applyHighPassFilter(audio_buffer[i]);
    }
    
    // Processar o áudio para gerar espectrograma
    processAudio(audio_buffer, bytesRead);
    
    // Aqui você pode enviar o áudio filtrado ou o espectrograma via Wi-Fi ou LoRaWAN
    // Por exemplo, você pode enviar 'audio_buffer' ou 'magnitudes'
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("your_SSID", "your_PASSWORD");  // Conectar ao Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi");

  // Inicializar o I2S para captura de áudio
  I2S.begin(I2S_PHILIPS_MODE, SAMPLE_RATE, 16);
  pinMode(I2S_DATA_IN_PIN, INPUT);
  pinMode(I2S_LRCK_PIN, INPUT);
  pinMode(I2S_SCK_PIN, INPUT);

  Serial.println("Sistema pronto para capturar áudio.");
}

void loop() {
  captureAndProcessAudio();
  delay(50);  // Pequeno atraso para não sobrecarregar o sistema
}
