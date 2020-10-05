CC = g++
CFLAGS = -Wall -Wextra
TARGET = line
OUTPUT = result.txt

.PHONY: run check clean
default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $? $(CFLGAS) -o $@

run:
	./$(TARGET)

check:
	./$(TARGET) > $(OUTPUT)

clean:
	rm -f $(TARGET) $(TARGET).o $(OUTPUT)
