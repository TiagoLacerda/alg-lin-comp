A simple real-time pitch shifter based on a phase vocoder. The signal is transformed into the frequency domain, spectral components are shifted by the desired number of semitones, and the audio is reconstructed using phase-coherent overlap-add synthesis.

### Dependencies

+ [JUCE](https://juce.com/)

+ [VST 3 Audio Plug-Ins SDK](https://www.steinberg.net/developers/vstsdk/)

### Building

1. Install JUCE.
2. Download and configure the VST3 SDK.
3. Open the project in the Projucer or your preferred IDE.
4. Generate project files and build the VST3 target.
