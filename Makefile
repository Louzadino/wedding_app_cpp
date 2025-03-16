# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Lista de arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Nome do executável final
EXEC = $(BIN_DIR)/prog

# Regra para compilar os arquivos .o
objs: $(OBJS)

# Regra para compilar o programa final
all: $(EXEC)

# Compila cada arquivo .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Liga os arquivos objeto e gera o executável
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Limpeza dos arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regra padrão
.PHONY: all objs clean
