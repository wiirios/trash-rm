CC = gcc
CFLAGS = -Wall -Wextra
TARGET = /tmp/trash-rm
SRC = src/main.c src/bin.c src/timestamp.c src/core.c

INSTALL_PATH = /usr/local/bin

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

install: all
	install -d $(INSTALL_PATH)
	install -m 755 $(TARGET) $(INSTALL_PATH)/trash-rm

	@echo "trash-rm installed!"

uninstall:
	rm -f $(INSTALL_PATH)/trash-rm

clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean