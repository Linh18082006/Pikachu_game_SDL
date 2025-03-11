# Compiler
CC := g++

# Thư mục cài đặt SDL2 trong MSYS2 (ucrt64)
SDL2_FLAGS := $(shell pkg-config --cflags --libs sdl2 SDL2_ttf SDL2_image SDL2_mixer)

# Tự động tìm tất cả file .cpp trong thư mục
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
OUT := main.exe

# Lệnh mặc định: Build nếu cần rồi chạy
all: build run

# Kiểm tra file main.exe và biên dịch nếu có thay đổi
build: $(OUT)

$(OUT): $(OBJ)
	@echo "🔗 Linking..."
	$(CC) $(OBJ) -o $(OUT) $(SDL2_FLAGS)

%.o: %.cpp
	@echo "🛠️ Compiling $<..."
	$(CC) -c $< -o $@ $(SDL2_FLAGS)

# Chạy chương trình (luôn đảm bảo bản mới nhất được build)
run: build
	@echo "🚀 Running $(OUT)..."
	./$(OUT)

# Lệnh dọn dẹp file biên dịch
clean:
	@echo "🗑️ Cleaning build files..."
	rm -f $(OUT) $(OBJ)
