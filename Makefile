# Not really a Makefile, but some simple stuff to compile the code in CI
all:
  arduino-cli -v compile --fqbn arduino:avr:leonardo MDM_V3_Library4x16/MDM_V3_Library4x16.ino
