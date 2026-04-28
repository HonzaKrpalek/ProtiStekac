"# ProtiStekac" 
"# ProtiStekac" 

Na I2S datovém výstupu.

Protože jsem odstranil I2S_MODE_DAC_BUILT_IN a i2s_set_dac_mode(), signál se teď vysílá přes I2S piny místo přes vestavěný DAC.

Pro ESP32-S3 jsou to standardně:

GPIO8 - DOUT (datový výstup)
GPIO9 - BCLK (bit clock)
GPIO46 - LRCK (word select)
Pokud chceš signál slyšet jako zvuk, musíš připojit externe:

Předzesilovač nebo DAC modul na GPIO8 (audio data)
Případně I2S DAC modul (např. PCM5102A, WM8960)
Pokud jsi chtěl původně používat vestavěný DAC na GPIO25, mohu kód vrátit na původní verzi s DAC_BUILT_IN (ale muselo by se to opravit pro ESP32-S3 kompatibilitu).