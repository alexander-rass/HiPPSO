EXECUTABLE=high_precision_pso
SOURCE_DIRECTORY=src
BINARY_DIRECTORY=bin

.PHONY: all
all:
	$(MAKE) -C $(SOURCE_DIRECTORY)/ && \
    mkdir -p $(BINARY_DIRECTORY) && \
    cp $(SOURCE_DIRECTORY)/$(EXECUTABLE) $(BINARY_DIRECTORY)/$(EXECUTABLE)

.PHONY: test
test: all
	$(MAKE) test -C $(SOURCE_DIRECTORY)


clean:
	rm -rf $(BINARY_DIRECTORY)
	$(MAKE) clean -C $(SOURCE_DIRECTORY)

debug:
	$(MAKE) debug -C $(SOURCE_DIRECTORY)/ && \
    mkdir -p $(BINARY_DIRECTORY) && \
    cp $(SOURCE_DIRECTORY)/$(EXECUTABLE) $(BINARY_DIRECTORY)/$(EXECUTABLE)

debugtest: debug
	$(MAKE) debugtest -C $(SOURCE_DIRECTORY)

.PHONY: doc

doc:
	$(MAKE) cleandoc && \
	doxygen doc/Doxyfile > /dev/null

cleandoc:
	rm -rf doc/html/*

coverage:
	$(MAKE) coverage -C $(SOURCE_DIRECTORY)/ && \
    mkdir -p $(BINARY_DIRECTORY) && \
    cp $(SOURCE_DIRECTORY)/$(EXECUTABLE) $(BINARY_DIRECTORY)/$(EXECUTABLE)

coveragetest: coverage
	$(MAKE) coveragetest -C $(SOURCE_DIRECTORY)
