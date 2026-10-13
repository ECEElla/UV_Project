UV Classification Using Photodiode and Optical Filters

Arduino Nano 33 BLE + Edge Impulse

This project estimates relative UV intensity using an Arduino Nano 33 BLE, a photodiode, and two optical filters (red and blue). No dedicated UV sensor is used. Instead, UV levels are inferred by comparing how much light passes through each filter:
The red filter provides a relative sense of the UV.
The blue filter allows more blue and UV wavelengths to pass.
By measuring light through both filters and computing their ratio, the system obtains a UV-related signal. This ratio is then classified using a machine-learning model trained in Edge Impulse using the datasets we created.

Project Overview

The goal of this project was to classify UV exposure levels using low-cost components and embedded machine learning. A photodiode alone cannot isolate UV light from visible light, so we used optical filtering to indirectly estimate UV content. The ratio of light measured through the blue filter versus the red filter correlates with relative UV intensity.

A dataset was collected under different lighting conditions, processed, and used to train a three-class classifier in Edge Impulse:

Class 0 – Low UV

Class 1 – Medium UV

Class 2 – High UV

The trained model runs directly on the Arduino Nano 33 BLE and outputs a UV level and SPF recommendation.

Hardware:

Arduino Nano 33 BLE
Photodiode connected to analog input
Red optical filter
Blue optical filter
Adjustable ring light (for indoor testing)

Data Collection Procedure:

A ring light was used as a controlled indoor light source.
The ring light was positioned at a fixed angle relative to the photodiode for consistency.

Two lighting modes were tested:

Warm light
Cool light

For each mode, data was recorded as follows:

20 samples using the red filter
20 samples using the blue filter
20 samples using the clear filter
The samples for each configuration were averaged to reduce noise.
Additional measurements were taken in direct sunlight using the blue and clear filter only.
These measurements provided a baseline for real UV exposure.

For each condition, a ratio was computed: ratio = blue_filter_adc / clear_filter_adc

This ratio eliminates brightness differences and isolates UV-related changes.
The ratios and their corresponding class labels (0, 1, 2) were uploaded into Edge Impulse for model training.

Edge Impulse Model:

Input: Blue/Clear ratio
Output: UV class (0, 1, or 2)
Model Type: Small fully connected neural network
Deployment: C++ inference library running on the Arduino Nano 33 BLE
The device performs on-board inference and outputs UV class and recommended SPF based on the model’s prediction.

Operation Procedure:

Upload uv_classifier.ino to the Arduino Nano 33 BLE.
Open the Serial Monitor at 115200 baud.

When prompted:

Place the clear filter over the photodiode.
Then, Place the blue filter over the photodiode.

The device prints:

Clear filter ADC value
Blue filter ADC value
Computed ratio
Predicted UV class
SPF recommendation
SPF Guidelines
