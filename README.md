# EE-105-Final-Project
UV Classification Using Photodiode + Optical Filters (Arduino Nano 33 BLE + Edge Impulse)

This project estimates UV intensity without using a UV sensor.
Instead, the system uses a photodiode combined with optical filters (red and blue) and a machine-learning classifier trained with Edge Impulse.

We discovered that by comparing the light that passes through a blue filter versus a red filter, we can estimate how much short-wavelength / UV light is present in the environment.

This method allowed us to classify UV exposure levels on a simple embedded device (Arduino Nano 33 BLE).
