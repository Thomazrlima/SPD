import numpy as np

# Caminhos dos arquivos (ajuste para usar barras duplas `\\` ou `r""`)
file1_path = r"C:\PENTES\SPD\MPI\C.txt"  # Primeira versão
file2_path = r"C:\PENTES\SPD\T1\C.txt"  # Segunda versão (supostamente correta)

# Carregar os arquivos como matrizes
def load_matrix(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()
    return np.array([list(map(float, line.split())) for line in lines])

# Carregar as matrizes
matrix1 = load_matrix(file1_path)
matrix2 = load_matrix(file2_path)

# Comparação das matrizes
diff_matrix = matrix1 - matrix2
max_diff = np.max(np.abs(diff_matrix))
mean_diff = np.mean(np.abs(diff_matrix))

# Encontrar onde estão as diferenças significativas
diff_positions = np.argwhere(diff_matrix != 0)

# Printar os resultados
print(f"Máxima diferença absoluta: {max_diff}")
print(f"Média das diferenças absolutas: {mean_diff}")
print(f"Posições das primeiras diferenças:\n{diff_positions[:10]}")
