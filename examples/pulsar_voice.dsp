// Pulsar synthesizer voice for VCV Rack generated via faust2vcvrack.
// The design follows Nathan Ho's pulsar-synthesis notes: short grains carved
// from an incoming waveform and shaped by a window. CV metadata exposes the
// primary modulation points (pitch, width, jitter, level) to Rack inputs.

import("stdfaust.lib");

declare name "PulsarVoice";
declare author "Arity";
declare license "MIT";

clamp(x, lo, hi) = min(max(x, lo), hi);
srq(x) = sqrt(max(x, 1e-6));

baseFreqHz = hslider("Pitch Base (Hz)", 110, 10, 8000, 0.1);
freqFromCv = hslider("Pitch CV[CV:1][unit:Hz]", 110, 10, 8000, 0.1);
coarseSemis = hslider("Tune Coarse (st)", 0, -24, 24, 0.1);
fineSemis = hslider("Tune Fine (st)", 0, -1, 1, 0.001);

widthKnob = hslider("Pulse Width", 0.25, 0.02, 0.98, 0.001);
widthCv = hslider("Pulse Width CV[CV:2]", 0.0, -0.45, 0.45, 0.001);

jitterKnob = hslider("Pulse Jitter", 0.0, 0.0, 1.0, 0.01);
jitterCv = hslider("Pulse Jitter CV[CV:3]", 0.0, -1.0, 1.0, 0.01);

levelKnob = hslider("Output Level", 0.8, 0.0, 2.0, 0.001);
levelCv = hslider("Output Level CV[CV:4]", 0.0, -1.0, 1.0, 0.001);

widthCompKnob = hslider("Width Compensation", 0.5, 0.0, 1.0, 0.01);
widthCompCv = hslider("Width Compensation CV[CV:5]", 0.0, -1.0, 1.0, 0.01);

envBlendKnob = hslider("External Env Mix", 0.0, 0.0, 1.0, 0.01);
envBlendCv = hslider("External Env Mix CV[CV:6]", 0.0, -1.0, 1.0, 0.01);

internalLevelKnob = hslider("Internal Wave Level", 0.6, 0.0, 1.0, 0.01);
internalLevelCv = hslider("Internal Wave Level CV[CV:7]", 0.0, -1.0, 1.0, 0.01);

internalMorphKnob = hslider("Internal Wave Morph", 0.5, 0.0, 1.0, 0.01);
internalMorphCv = hslider("Internal Wave Morph CV[CV:8]", 0.0, -1.0, 1.0, 0.01);

stereoSpreadKnob = hslider("Stereo Spread", 0.0, 0.0, 1.0, 0.01);
stereoSpreadCv = hslider("Stereo Spread CV[CV:9]", 0.0, -1.0, 1.0, 0.01);

// Pre-compute parameters
pitchScale = pow(2.0, (coarseSemis + fineSemis) / 12.0);
freq = max(10.0, freqFromCv) * pitchScale;
baseFreq = max(10.0, baseFreqHz);
// Crossfade between manual base and CV pitch so the oscillator keeps moving when no CV is attached.
freqTarget = (0.5 * freq) + (0.5 * baseFreq * pitchScale);

widthDepth = clamp(widthKnob + widthCv, 0.02, 0.98);
jitterDepth = clamp(jitterKnob + jitterCv, 0.0, 1.0);
level = max(0.0, levelKnob + levelCv);

widthComp = clamp(widthCompKnob + widthCompCv, 0.0, 1.0);
envBlend = clamp(envBlendKnob + envBlendCv, 0.0, 1.0);
internalLevel = clamp(internalLevelKnob + internalLevelCv, 0.0, 1.0);
internalMorph = clamp(internalMorphKnob + internalMorphCv, 0.0, 1.0);
stereoSpread = clamp(stereoSpreadKnob + stereoSpreadCv, 0.0, 1.0);

channelCount = 4;

channelPosKnob = hslider("Channel Position", 0.5, 0.0, 1.0, 0.01);
channelPosCv = hslider("Channel Position CV[CV:10]", 0.0, -1.0, 1.0, 0.01);

channelMapMixKnob = hslider("Channel Map Mix", 1.0, 0.0, 1.0, 0.01);
channelMapMixCv = hslider("Channel Map Mix CV[CV:11]", 0.0, -1.0, 1.0, 0.01);

channelSpreadKnob = hslider("Channel Spread", 0.2, 0.0, 1.0, 0.01);
channelSpreadCv = hslider("Channel Spread CV[CV:12]", 0.0, -1.0, 1.0, 0.01);

process(waveIn, envIn, mapIn) = (out0, out1, out2, out3) with {
    phase = os.phasor(freqTarget);
    phasePrev = phase';
    reset = phase < phasePrev;

    noise1 = no.noise;
    jitterHold = ba.sAndH(noise1, reset);
    widthVar = clamp(widthDepth + (0.3 * jitterDepth * jitterHold), 0.02, 0.98);

    gate = phase < widthVar;
    safeWidth = max(widthVar, 0.02);
    normPhase = min(max(phase / safeWidth, 0.0), 1.0);

    // Base Hann window and optional external envelope
    hann = gate * (0.5 * (1.0 - cos(normPhase * ma.PI)));
    externalEnv = gate * max(envIn, 0.0);
    comp = pow(safeWidth, -widthComp);
    window = comp * ((1.0 - envBlend) * hann + envBlend * externalEnv);

    // Internal fallback waveform cycles across each grain
    sineFrame = sin((2.0 * ma.PI) * normPhase);
    sawFrame = (normPhase * 2.0) - 1.0;
    internalWave = internalLevel * ((1.0 - internalMorph) * sineFrame + internalMorph * sawFrame);

    waveSource = waveIn + internalWave;
    core = fi.dcblocker(window * waveSource * level);

    mapSample = ba.sAndH(mapIn, reset);
    mapNorm = clamp(0.5 + (0.1 * mapSample), 0.0, 1.0);

    channelPosBase = clamp(channelPosKnob + channelPosCv, 0.0, 1.0);
    channelMapMix = clamp(channelMapMixKnob + channelMapMixCv, 0.0, 1.0);
    channelPos = clamp(((1.0 - channelMapMix) * channelPosBase) + (channelMapMix * mapNorm), 0.0, 1.0);

    idx = channelPos * 3.0;
    idxFloor = floor(idx);
    idxFrac = idx - idxFloor;
    idxCeil = min(idxFloor + 1.0, 3.0);

    channelSpread = clamp(channelSpreadKnob + channelSpreadCv, 0.0, 1.0);
    spreadRange = channelSpread * 3.0;

    two0 = ((idxFloor == 0.0) * (1.0 - idxFrac)) + ((idxCeil == 0.0) * idxFrac);
    two1 = ((idxFloor == 1.0) * (1.0 - idxFrac)) + ((idxCeil == 1.0) * idxFrac);
    two2 = ((idxFloor == 2.0) * (1.0 - idxFrac)) + ((idxCeil == 2.0) * idxFrac);
    two3 = ((idxFloor == 3.0) * (1.0 - idxFrac)) + ((idxCeil == 3.0) * idxFrac);

    spreadDenom = max(spreadRange, 1e-6);
    raw0 = max(0.0, 1.0 - (abs(idx - 0.0) / spreadDenom));
    raw1 = max(0.0, 1.0 - (abs(idx - 1.0) / spreadDenom));
    raw2 = max(0.0, 1.0 - (abs(idx - 2.0) / spreadDenom));
    raw3 = max(0.0, 1.0 - (abs(idx - 3.0) / spreadDenom));
    rawSum = raw0 + raw1 + raw2 + raw3;
    multi0 = raw0 / max(rawSum, 1e-6);
    multi1 = raw1 / max(rawSum, 1e-6);
    multi2 = raw2 / max(rawSum, 1e-6);
    multi3 = raw3 / max(rawSum, 1e-6);

    useMulti = channelSpread > 1e-5;
    gain0 = ((1.0 - useMulti) * two0) + (useMulti * multi0);
    gain1 = ((1.0 - useMulti) * two1) + (useMulti * multi1);
    gain2 = ((1.0 - useMulti) * two2) + (useMulti * multi2);
    gain3 = ((1.0 - useMulti) * two3) + (useMulti * multi3);

    out0 = core * srq(gain0);
    out1 = core * srq(gain1);
    out2 = core * srq(gain2);
    out3 = core * srq(gain3);
};
