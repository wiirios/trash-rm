CC = gcc
CFLAGS = -Wall -Wextra
TARGET = trash-rm
SRC = src/main.c bin.c timestamp.c core.c

INSTALL_PATH = /usr/local/bin
BIN_DIR = /usr/local/share/trash-rm/bin

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

install: all
	install -d $(INSTALL_PATH)
	install -d $(BIN_DIR)
	install -m 755 $(TARGET) $(INSTALL_PATH)
	@@echo "trash-rm installed!"
	@@echo "Trash folder: $(BIN_DIR)"

uninstall:
	rm -f $(INSTALL_PATH)/$(TARGET)
	rm -rf /usr/local/share/trash-rm

clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean