# Not really a Makefile, but some simple stuff to compile the code in CI
all:
	for folder in examples/*/; do \
		echo "" ; \
		echo "-------------------------" ; \
    	echo $$folder ; \
    	arduino-cli -v compile --fqbn arduino:avr:leonardo $$folder ; \
	done
