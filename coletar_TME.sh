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
      
      # Compila o código
      gcc -pthread main.c queue.c stats.c utils.c proc.c proc_init.c proc_interrupt.c thread.c time.c scheduler_${alg}.c sched.c -o "$binario" 2> /dev/null

      if [[ ! -f "$binario" ]]; then
        echo "$np,ERRO_COMPILACAO" >> resultados/$alg.csv
        echo "[$alg | $np] ❌ Binário não compilado"
        continue
      fi

      # Executa e captura saída completa
      saida=$("./$binario" scheduler_${alg} "$np")
      
      # Extrai valor numérico depois de TME:
      tme=$(echo "$saida" | grep "TME:" | sed -E 's/.*TME: *([0-9.]+).*/\1/')

      if [[ $tme =~ ^[0-9]+([.][0-9]+)?$ ]]; then
        echo "$np,$tme" >> resultados/$alg.csv
        echo "[$alg | $np] ✅ TME = $tme"
      else
        echo "$np,ERRO" >> resultados/$alg.csv
        echo "[$alg | $np] ⚠️  TME não encontrado"
        echo "$saida" > "logs/${alg}_${np}_run${i}.log"
      fi

      rm -f "$binario"
    done
  done
done