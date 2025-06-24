#!/bin/bash

algos=("fifo" "sjf" "ljf" "prio_static" "prio_dynamic" "prio_dynamic_quantum")
np_list=(10 20 30 40 50 60 70 80 90 100)
runs=10

mkdir -p resultados logs

for alg in "${algos[@]}"; do
  echo "🔁 Rodando algoritmo: $alg"
  echo "NPROC,TME" > resultados/$alg.csv

  for np in "${np_list[@]}"; do
    for ((i=1; i<=runs; i++)); do
      binario="principal_${alg}"

      gcc -pthread main.c queue.c stats.c utils.c proc.c proc_init.c proc_interrupt.c thread.c time.c scheduler_${alg}.c sched.c -o "$binario" 2> /dev/null

      if [[ ! -f "$binario" ]]; then
        echo "$np,ERRO_COMPILACAO" >> resultados/$alg.csv
        continue
      fi

      saida=$("./$binario" scheduler_${alg} "$np")
      tme=$(echo "$saida" | grep "TME:" | sed -E 's/.*TME: *([0-9.]+).*/\1/')

      if [[ $tme =~ ^[0-9]+([.][0-9]+)?$ ]]; then
        echo "$np,$tme" >> resultados/$alg.csv
      else
        echo "$np,ERRO" >> resultados/$alg.csv
        echo "$saida" > "logs/${alg}_${np}_run${i}.log"
      fi

      rm -f "$binario"
    done
  done
done

# ✅ Gera o gráfico ao final
python3 - <<EOF
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

algoritmos = ["fifo", "sjf", "ljf", "prio_static", "prio_dynamic", "prio_dynamic_quantum"]
plt.figure(figsize=(12, 7))

for alg in algoritmos:
    caminho = f"resultados/{alg}.csv"
    if not os.path.exists(caminho):
        continue

    df = pd.read_csv(caminho)
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
EOF