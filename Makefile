# Not really a Makefile, but some simple stuff to compile the code in CI
all:
	for file in examples/*; do \
    	echo $file ; \
		echo "$(basename "$file")" \
	done
