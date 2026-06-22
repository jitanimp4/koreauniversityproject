CXX      = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS  = -pthread

TARGET = odyssey_os

SRC = main.cpp \
      cryptex_boss.cpp \
      celluloid_app.cpp \
      word_pyramid.cpp \
      wordle.cpp \
      game.cpp \
      puzzle.cpp \
      forgers_table.cpp \
      art.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) save_data.txt

.PHONY: all clean
