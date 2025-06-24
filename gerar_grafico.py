import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

algoritmos = [
    "fifo",
    "sjf",
    "ljf",
    "prio_static",
    "prio_dynamic",
    "prio_dynamic_quantum"
]

plt.figure(figsize=(12, 7))

for alg in algoritmos:
    caminho = f"resultados/{alg}.csv"
    if not os.path.exists(caminho):
        print(f"[AVISO] Arquivo não encontrado: {caminho}")
        continue

    df = pd.read_csv(caminho)

    # 🧼 Remove linhas com TME vazio ou inválido
    df = df[pd.to_numeric(df["TME"], errors="coerce").notna()]
    df["TME"] = df["TME"].astype(float)

    agrupado = df.groupby("NPROC")["TME"]
    medias = agrupado.mean()
    variancias = agrupado.var()

    x = medias.index
    y = medias.values
    erro = np.sqrt(variancias.values)

    plt.errorbar(x, y, yerr=erro, label=alg.upper(), capsize=5, marker='o', linestyle='--')

plt.title("Tempo Médio de Espera por Quantidade de Processos")
plt.xlabel("Número de Processos (NPROC)")
plt.ylabel("Tempo Médio de Espera (TME)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_TME.png", dpi=300)
plt.show()