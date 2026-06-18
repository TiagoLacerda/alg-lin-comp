A simple real-time pitch shifter based on a phase vocoder. The signal is transformed into the frequency domain, spectral components are shifted by the desired number of semitones, and the audio is reconstructed using phase-coherent overlap-add synthesis.

<video src="Assets/Videos/example.mp4" controls width="720">
  Your browser does not support embedded videos. You can view the demo at `Assets/Videos/example.mp4`.
</video>

### Dependencies

+ [JUCE](https://juce.com/)

+ [VST 3 Audio Plug-Ins SDK](https://www.steinberg.net/developers/vstsdk/)

### Building

1. Install JUCE and ensure it is configured for your preferred development environment.

2. Download the VST3 SDK from Steinberg and configure the SDK path (e.g. via `VST3_SDK_PATH` or your project settings).

3. Open the project in Projucer and export project files for your target IDE, or configure the project with CMake if supported.

4. Generate the project files and open the resulting solution/workspace.

5. Build the VST3 target.

6. Copy the resulting `.vst3` plugin to the appropriate system VST3 directory:

   * **Windows:** `C:\Program Files\Common Files\VST3`
   * **macOS:** `/Library/Audio/Plug-Ins/VST3`
   * **Linux:** `~/.vst3` or `/usr/lib/vst3`

7. Rescan plugins in your DAW if the plugin does not appear automatically.

### Notes

* A C++17-compatible compiler is recommended.
