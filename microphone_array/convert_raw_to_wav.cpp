
// Default std lib
#include <stdio.h>
#include <stdlib.h>
#include <string>

//Converts a raw audio file into a wav
//IMPORTANT - ASSUMES YOU HAVE sox and alsa-utils installed!
// Run 'sudo apt-get install sox alsa-utils' if you get an error here!
void convert_raw_to_wav() {
  
  
  
  //This is the beamformed output!  It is a combination of the other 8 microphone channels!
  //system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 " + parent_folder + "/mic_16000_s16le_channel_2.raw output2.wav\"");
  
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_8.raw output8.wav\"");

  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_0.raw output0.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_1.raw output1.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_2.raw output2.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_3.raw output3.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_4.raw output4.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_5.raw output5.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_6.raw output6.wav\"");
  system("/bin/sh -c \"sox -r 16000 -c 1 -e signed -b 16 mic_16000_s16le_channel_7.raw output7.wav\"");
  
  //In case you want to play the sound after.
  //system("/bin/sh -c \" aplay output8.wav\"");
}

int main(int argc, char *agrv[]) {

	convert_raw_to_wav();

}
