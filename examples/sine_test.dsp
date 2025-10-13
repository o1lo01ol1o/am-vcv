import("stdfaust.lib");

freq = hslider("Freq", 440, 50, 2000, 1);
gain = hslider("Gain", 0.2, 0, 1, 0.01);

process = os.osc(freq) * gain;
