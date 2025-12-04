/*
  UV Intensity Classification Using Photodiode + Optical Filters
  --------------------------------------------------------------
  This program runs a UV intensity classifier on the Arduino Nano 33 BLE 
  using a photodiode paired with optical filters. The system does not use a 
  dedicated UV sensor. Instead, UV level is estimated using the ratio of light 
  transmitted through a blue filter versus a red filter.

  - The blue filter allows more UV and shorter-wavelength light to pass.
  - The red filter blocks most UV and blue light, passing mostly longer wavelengths.

  By comparing the averaged ADC readings through each filter, we can compute:

      ratio = blue_filter_adc / red_filter_adc

  This ratio correlates with relative UV intensity independent of overall brightness.
  
  Collected data (warm light, cool light, and direct sunlight) was used to train 
  an Edge Impulse model with 3 classes:
      0 = Low UV
      1 = Medium UV
      2 = High UV

  The Edge Impulse model is deployed on the Nano 33 BLE and classifies UV level 
  in real time. The sketch then outputs a recommended SPF value based on the 
  predicted UV class.

  Hardware:
    - Arduino Nano 33 BLE
    - Photodiode connected to analog input
    - Blue filter (passes more UV/blue light)
    - Red filter (blocks UV/blue, passes longer wavelengths)
    
*/

#include <Arduino.h>
#include <UVSENSOR_inferencing.h>  

// ---- PINS ----
const int PD_PIN = A0;

// EI feature buffer
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// EI callback
int raw_feature_get_data(size_t offset, size_t length, float *out) {
    memcpy(out, features + offset, length * sizeof(float));
    return 0;
}

// Read average of N samples
float read_avg(int n) {
    long s = 0;
    for (int i = 0; i < n; i++) {
        s += analogRead(PD_PIN);
        delay(5);
    }
    return (float)s / (float)n;
}

void setup() {
    Serial.begin(115200);
    pinMode(PD_PIN, INPUT);

    Serial.println("UV Sensor Classifier (Clear → Blue)");
    delay(1000);

    // ---- 1. CLEAR FILTER ----
    Serial.println("Place CLEAR filter over sensor. Press ENTER when ready.");
    while (Serial.available() == 0);
    Serial.read();

    float clear_adc = read_avg(20);

    // ---- 2. BLUE FILTER ----
    Serial.println("Now place BLUE filter. Press ENTER when ready.");
    while (Serial.available() == 0);
    Serial.read();

    float blue_adc = read_avg(20);

    // ---- 3. Compute ratio ----
    float ratio = blue_adc / clear_adc;
    features[0] = ratio;

    // ---- 3.5 FIXED CLASSIFICATION BASED ON YOUR RANGES ----
    // <0.33  → Low
    // 0.33–0.75 → Medium  
    // >0.75  → High      
    String manual_label;

    if (ratio < 0.33) {
        manual_label = "low";
    }
    else if (ratio < 0.75) {
        manual_label = "medium";
    }
    else {
        manual_label = "high";
    }

    // ---- 4. Run Edge Impulse (still runs, but label will use manual thresholds) ----
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    signal.get_data = &raw_feature_get_data;

    ei_impulse_result_t result;
    EI_IMPULSE_ERROR ei_err = run_classifier(&signal, &result, false);

    if (ei_err != EI_IMPULSE_OK) {
        Serial.print("Classifier error: ");
        Serial.println(ei_err);
        return;
    }

    // ---- 6. Print result ----
    Serial.println("------------ UV RESULT ------------");
    Serial.print("Clear ADC: "); Serial.println(clear_adc);
    Serial.print("Blue ADC : "); Serial.println(blue_adc);
    Serial.print("Ratio     : "); Serial.println(ratio, 4);

    // print manual label instead of EI label
    Serial.print("Predicted (manual): ");
    Serial.println(manual_label);

    // ---- 7. SPF Recommendation ----
    if (manual_label == "low") {
        Serial.println("UV LEVEL: LOW → Wear SPF 30");
    }
    else if (manual_label == "medium") {
        Serial.println("UV LEVEL: MEDIUM → Wear SPF 50");
    }
    else {
        Serial.println("UV LEVEL: HIGH → Wear SPF 70+");
    }

    Serial.println("-----------------------------------");
    Serial.println("Done. Reset the board to measure again.");
}

void loop() {
    // one-shot
}


















