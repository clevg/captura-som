#include <FFT.h>  // Biblioteca FFT

#define FFT_SIZE 128  // Tamanho da FFT

float real[FFT_SIZE];
float imag[FFT_SIZE];
float magnitudes[FFT_SIZE / 2];  // Armazena as magnitudes de frequências

void processAudio(int16_t* buffer, int bufferSize) {
  // Preenche os arrays de dados para FFT
  for (int i = 0; i < FFT_SIZE && i < bufferSize; i++) {
    real[i] = (float) buffer[i];  // Atribui os dados do buffer à parte real da FFT
    imag[i] = 0;  // Inicializa a parte imaginária como 0
  }

  // Aplica a FFT
  FFT.Windowing(real, FFT_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
  FFT.Compute(real, imag, FFT_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(real, imag, magnitudes, FFT_SIZE);

  // Aqui, as magnitudes armazenadas em 'magnitudes' podem ser usadas para gerar o espectrograma
}
