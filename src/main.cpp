#include <Arduino.h>
#include <driver/i2s.h>
#include <driver/dac.h>
#include <cmath>

#define I2S_PORT I2S_NUM_0
#define SAMPLE_RATE 100000
#define DAC_CHANNEL I2S_DAC_CHANNEL_RIGHT_EN  // GPIO25

// Chirp parametry
const float FREQ_START = 22000.0;
const float FREQ_END   = 25000.0;
const float CHIRP_TIME = 0.05;   // 50 ms

// Výpočet
const int CHIRP_SAMPLES = int(SAMPLE_RATE * CHIRP_TIME);

uint16_t buffer[CHIRP_SAMPLES];

void generateChirp()
{
  float phase = 0.0;
  float k = (FREQ_END - FREQ_START) / CHIRP_TIME;

  for (int i = 0; i < CHIRP_SAMPLES; i++) {
    float t = (float)i / SAMPLE_RATE;
    float freq = FREQ_START + k * t;
    phase += 2.0 * M_PI * freq / SAMPLE_RATE;

    float sample = sin(phase);

    // DAC je 8bit unsigned (0–255)
    uint8_t dac = (uint8_t)(128 + 127 * sample);

    buffer[i] = dac << 8;  // I2S očekává 16 bit
  }
}

void setupI2S()
{
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_dac_mode(DAC_CHANNEL);
}

void setup()
{
  randomSeed(analogRead(A0));
  setupI2S();
  generateChirp();
}

void loop()
{
  size_t bytes_written;
  
  i2s_write(I2S_PORT,
             buffer,
             sizeof(buffer),
             &bytes_written,
             portMAX_DELAY);

  // Náhodná pauza (habituační prevence)
  delay(random(500, 3000));
}
