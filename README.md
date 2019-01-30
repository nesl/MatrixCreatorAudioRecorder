# Requirements
In order to use these examples:
- Make sure you have 
[MATRIX HAL](https://matrix-io.github.io/matrix-documentation/matrix-hal/overview/) installed on your MATRIX device

# Download Examples
The following commands download and compile all the examples:

```language-cpp
sudo apt-get install cmake g++ git
cd ~/MatrixCreatorAudioRecorder
mkdir build
cd build
cmake ..
make -j4
```

Also be sure to check the commands.txt under matrix_listener/microphone_array
For now, I skipped creating a makefile, but later I'll add one in for convenience.
The commands are required for utility commands - such as converting raw files into wav files.

The main file that controls recording audio is created under build/mic_record_file
Once it begins running, you have to hit ctrl+z to stop recording, otherwise it runs infinitely.
Once you stop it, you'll have 9 different *.raw files.  Each one is numbered and corresponds to 
a specific mic on the array.  The 9th *.raw file is a beamformed channel from the other 8.
Similarly, the *.wav files that are created will be named according to the channel they came from. 
