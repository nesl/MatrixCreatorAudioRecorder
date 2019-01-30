CC = g++
CPPFLAGS = -g -Wall
OBJS = convert_raw_to_wav.o

../build/microphone_array/convert_raw_to_wav : convert_raw_to_wav.o
	$(CC) -o ../build/microphone_array/convert_raw_to_wav convert_raw_to_wav.o

convert_raw_to_wav.o :
	$(CC) $(CPPFLAGS) -c convert_raw_to_wav.c

clean:
	rm -f ../build/microphone_array/convert_raw_to_wav $(OBJS)




