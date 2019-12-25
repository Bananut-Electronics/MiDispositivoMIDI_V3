# Not really a Makefile, but some simple stuff to compile the code in CI
all:
  brew install arduino-cli
  arduino-cli core update-index
  arduino-cli core install arduino:avr
  arduino-cli lib install "Adafruit NeoPixel"
  arduino-cli lib install "MIDIUSB"
  arduino-cli lib install "TimerOne"
