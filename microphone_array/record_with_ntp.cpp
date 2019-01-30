/*
 * Microphone array record to file example
 * Channel 8 is beamforming output
 */

/////////////////////////
// INCLUDE STATEMENTS //
///////////////////////

// Google gflags parser
#include <gflags/gflags.h>
// Input/output stream class to operate on files
#include <fstream>
// Input/output streams and functions
#include <iostream>
// Use strings
#include <string>
// Arrays for math operations
#include <valarray>

// Default std lib
#include <stdio.h>
#include <stdlib.h>

// Communicates with MATRIX device
#include "matrix_hal/matrixio_bus.h"
// Interfaces with microphone array
#include "matrix_hal/microphone_array.h"
// Enables using FIR filter with microphone array
#include "matrix_hal/microphone_core.h"

////////////////////////
// INITIAL VARIABLES //
//////////////////////

// Defines variables from user arguments using gflags utility
// (https://gflags.github.io/gflags/)

// Grabs sampling frequency input from user
DEFINE_int32(
    sampling_frequency, 16000,
    "Sampling Frequency");  // Argument example: "--sampling_frequency 48000"
// Grabs duration input from user
DEFINE_int32(duration, 5,
             "Interrupt after N seconds");  // Argument example: "--duration 10"
// Grabs gain input from user
DEFINE_int32(gain, -1, "Microphone Gain");  // Argument example: "--gain 5"



//Check if this sound is different compared to the usual environment
bool should_activate(int16_t sound, int16_t last_sound_val) {
  
  bool is_active = false;
  
  if( abs(sound - last_sound_val) > 10000 && last_sound_val != 0) {
    last_sound_val = sound;
    is_active = true;
  }
  
  return is_active;
}


int main(int argc, char *agrv[]) {
  ////////////////////
  // INITIAL SETUP //
  //////////////////

  // Parse command line flags with gflags utility
  // (https://gflags.github.io/gflags/)
  google::ParseCommandLineFlags(&argc, &agrv, true);

  // Create MatrixIOBus object for hardware communication
  matrix_hal::MatrixIOBus bus;
  // Initialize bus and exit program if error occurs
  if (!bus.Init()) return false;

  // Set user flags from gflags as variables
  int sampling_rate = FLAGS_sampling_frequency;
  int seconds_to_record = FLAGS_duration;
  int gain = FLAGS_gain;
  
  //Set this to true if you want all 8 microphones (+1 beamformed),
  // otherwise just keep the beamformed mic.
  bool RECORD_ALL_MICS = true;

  /////////////////
  // MAIN SETUP //
  ///////////////

  // Create MicrophoneArray object
  matrix_hal::MicrophoneArray microphone_array;
  // Set microphone_array to use MatrixIOBus bus
  microphone_array.Setup(&bus);
  // Set microphone sampling rate
  microphone_array.SetSamplingRate(sampling_rate);
  // If gain is positive, set the gain
  if (gain > 0) microphone_array.SetGain(gain);

  // Log gain_ and sampling_frequency_ variables
  microphone_array.ShowConfiguration();
  // Log recording duration variable
  //std::cout << "Duration : " << seconds_to_record << "s" << std::endl;

  // Calculate and set up beamforming delays for beamforming
  microphone_array.CalculateDelays(0, 0, 1000,
                                   320 * 1000);  // These are default values

  ///////////////////////
  // FIR FILTER SETUP //
  /////////////////////

  // Create MicrophoneCore object
  matrix_hal::MicrophoneCore microphone_core(microphone_array);
  // Set microphone_core to use MatrixIOBus bus
  microphone_core.Setup(&bus);

  ///////////////////////
  // MICROPHONE INPUT //
  /////////////////////

  // Create a buffer array for microphone input
  int16_t buffer[microphone_array.Channels() + 1]
                [microphone_array.SamplingRate() +
                 microphone_array.NumberOfSamples()];

  // Create an array of streams to write microphone data to files
  std::ofstream os[microphone_array.Channels() + 1];
  

  // For each microphone channel (+1 for beamforming), make a file and open it
  for (uint16_t c = 0; c < microphone_array.Channels() + 1; c++) {
    // Set filename for microphone output
    std::string filename = "mic_" +
                           std::to_string(microphone_array.SamplingRate()) +
                           "_s16le_channel_" + std::to_string(c) + ".raw";
    if(RECORD_ALL_MICS) {
      // Create and open file
      os[c].open(filename, std::ofstream::binary);
    }                       
    else if (c == microphone_array.Channels()) {
      // Create and open file
      os[c].open(filename, std::ofstream::binary);
    }
    
  }
  

  // Counter variable for tracking recording time
  uint32_t samples = 0;
  
  //Previous sound value
  uint16_t last_sound_val = 0;
  //Is the recording active?
  bool is_recording_active = false;
  //When did this recording start?
  int recording_start_index = 0;
  
  // For recording duration
  //for (int s = 0; s < seconds_to_record; s++) {
  int time_seconds = 0;
  while (true) {
    
    //If the recording is active and some time has passed in between
    //  the activate time and the current time.
    // Essentially, if nothing has happened for a while, stop recording.
    //printf("Time Between: %d, StartIndex: %d\n", s - recording_start_index, recording_start_index);
    if(is_recording_active && (abs(time_seconds - recording_start_index) > 1) ) {
      is_recording_active = false;
      //printf("Deactivated Recording.\n");
    }
    //double current_average = 0;
    
    bool loud_sound_found = false;
    int count = 0;
    
    // Endless loop
    while (true) {

      // Read microphone stream data
      microphone_array.Read();

      // For number of samples
      for (uint32_t s = 0; s < microphone_array.NumberOfSamples(); s++) {
        // For each microphone
        for (uint16_t c = 0; c < microphone_array.Channels(); c++) {
          // Send microphone data to buffer
          buffer[c][samples] = microphone_array.At(s, c);
        }
        // Writes beamformed microphone data into buffer
        buffer[microphone_array.Channels()][samples] = microphone_array.Beam(s);
        
        //Check if recording should be activated.  Start this recording.
        if(should_activate(buffer[microphone_array.Channels()][samples], last_sound_val)) {
          
          //We only set the recording to active if it isn't active already
          if(!is_recording_active) {
            //printf("Activated Recording\n");
            is_recording_active = true;
          }
          
          //We found a loud sound - reset the last active sound.
          loud_sound_found = true;
        }
        last_sound_val = buffer[microphone_array.Channels()][samples];
        //current_average += buffer[microphone_array.Channels()][samples];
        
        //Check the current output
        /*if(samples % 160 == 0) {
          printf("%u\n", buffer[microphone_array.Channels()][samples] );
          //count += 1;
        }*/
        
        // Increment samples for buffer write
        samples++;
      }

      // Once number of samples is >= sampling rate
      if (samples >= microphone_array.SamplingRate()) {
        
        //We only write the beamformed channel to file
        //int c = microphone_array.Channels();
        //os[c].write((const char *)buffer[c], samples * sizeof(int16_t));
        
        // For each microphone channel
        for (uint16_t c = 0; c < microphone_array.Channels() + 1; c++) {
          // Write to recording file
          os[c].write((const char *)buffer[c], samples * sizeof(int16_t));
        }
        // Set samples to zero for loop to fill buffer
        
        //If a sound was noted this time(second), 
        // we note the beginning of recording
        if(loud_sound_found) {  
          recording_start_index = time_seconds;
          //printf("Setting Start Index: %d\n", s); 
        }
        //printf("Count of Output: %d\n", count); 
        
        samples = 0;
        break;
      }
    }
    
    time_seconds += 1;
    
    if(time_seconds % 5 == 0) {  //Every 5 seconds we print how long recording has been going for.
      printf("%u seconds recorded\n", time_seconds);
    }
    
  }
    //current_average = current_average/microphone_array.SamplingRate();
    //printf("%u\n", current_average);
  //}

  return 0;
}
