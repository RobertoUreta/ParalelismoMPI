# Ejercicio 2
- Para el correcto funcionamiento del programa se necesitan 3 procesos,
ya que fue implementando segun lo apreciado en el grafo adjunto.

- Para compilar y ejecutar el ejercicio2.c como usuario root:
(si esta utlizando un usuario sin privilegio root no debe 
escribir "--allow-run-as-root")

```
mpicc Ejercicio2.c -o ejercicio2
mpirun -np 3 --allow-run-as-root ./ejercicio2
```

# Ejercicio 3
- Para el correcto funcionamiento del programa se necesitan 2 procesos,
ya que fue implementando segun lo apreciado en el grafo adjunto.

- Para compilar y ejecutar el ejercicio3.c como usuario root:
(si esta utlizando un usuario sin privilegio root no debe 
escribir "--allow-run-as-root")

```
mpicc Ejercicio3.c -o ejercicio3
mpirun -np 2 --allow-run-as-root ./ejercicio3
```



## Autor: Roberto Ureta