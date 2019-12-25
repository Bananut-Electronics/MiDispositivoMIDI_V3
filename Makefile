# Not really a Makefile, but some simple stuff to compile the code in CI
all:
	for file in examples/*; do \
    		echo $file ; \
		echo "$(basename "$file")" \
		#arduino-cli -v compile --fqbn arduino:avr:leonardo examples/MDM_V3_Library4x16/MDM_V3_Library4x16.ino \
	done
