# Nome do executável final
TARGET = prog

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -I $(SRC_DIR) -finput-charset=UTF-8

# Busca todos os arquivos .cpp dentro de src/
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)

# Cria os arquivos .o correspondentes em obj/
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regra principal (compila tudo)
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regra para compilar cada .cpp individualmente
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
