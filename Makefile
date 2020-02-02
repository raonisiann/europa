shell=/bin/bash

export ROOT_DIR=$(shell pwd)
export SRC_DIR=$(ROOT_DIR)/src
export BIN_DIR=$(ROOT_DIR)/bin

all: 
	$(MAKE) -C $(SRC_DIR)

debug: 
	$(MAKE) debug_enabled -C $(SRC_DIR)

clean:
	$(MAKE) clean -C $(SRC_DIR)	

install:
	/bin/bash -c $(ROOT_DIR)/install.sh
