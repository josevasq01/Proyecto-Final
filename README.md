DOCUMENTO TÉCNICO – Sistema de Juego por Turnos con IA Guardian
1. Arquitectura General del Sistema

El proyecto implementa un juego de estrategia por turnos basado en dominio territorial.
La arquitectura sigue un modelo modular orientado a componentes:

1.1 Módulos Principales

Juego
Orquesta el ciclo por turnos: inicio → fase del jugador → fase del mundo → evaluación → fin.

Contexto
Contiene el estado global: mapa, recursos, turnos, bitácora, unidades y edificios.

MapaMundo
Gestiona las celdas del mapa, almacenamiento de unidades, edificios, terreno y funciones:

moveUnit()

recruitAt()

constructAt()

countOwned()

MotorEventos
Procesa eventos ambientales o neutrales del turno (opcional en versión actual).

GestorRecursos
Aplica:

Producción de edificios (comida, metal, energía).

Mantenimiento de unidades.

Validación de recursos para nuevas acciones.

Consola
Maneja impresión del mapa, menús y entrada del usuario.

IA_Guardian
Inteligencia artificial que gestiona reclutamiento, construcción y movimiento del jugador 2.

2. Arquitectura Interna del Turno

Cada turno sigue un flujo fijo:

turnoInicio()  
 → Render inicial  
 → faseJugador()  
   → acciones (mover / reclutar / construir)  
 → faseMundo()  
   → IA + eventos + producción  
 → evaluarCondiciones()  
 → turnoFin()


El diseño asegura consistencia del estado y claridad en la interacción entre sistemas.

3. Sistema de Eventos

El motor de eventos (MotorEventos) permite incorporar efectos externos al jugador:

Aparición de guardianes neutrales.

Penalidades por terreno.

Alertas contextuales para enriquecer la jugabilidad.

Su ejecución ocurre antes de la IA, permitiendo que influya en sus decisiones.

4. IA Guardian

La IA tiene un sistema de tres “slots” por turno:

4.1 Reglas de Acción

Reclutamiento

Prioriza unidades económicas (Soldado y Arquero).

Solo usa recursos disponibles.

Construcción (prioridades)

Si tiene <2 Cuarteles → construir Cuartel  
Si tiene <2 Torres y ya poseé Cuartel → construir Torre  
Si tiene <1 Forja y ya posee Torre → construir Forja  
Si comida <10 → construir Granja  


Movimiento ofensivo básico

Si detecta casilla enemiga adyacente → intenta capturar.

Movimiento simple hacia edificios sin dueño.

4.2 Registro de acciones

Cada acción ejecutada se registra como:

IA: recluto <unidad> en (x,y)
IA: construyo <edificio> en (x,y)
IA: movio unidad hacia ...


El juego imprime solo los logs generados en esta fase.

5. Balance de Juego

El balance se diseñó para que ambos jugadores escalen su poder progresivamente:

5.1 Costos y producción

Granja → genera comida

Cuartel → habilita reclutamiento avanzado

Torre → defensa y estructura de progresión

Forja → habilita Caballero y Mago

Mantenimiento evita reclutamiento infinito

Esto asegura progreso controlado y obliga a tomar decisiones económicas.

5.2 Sistema de Dominio

Cada celda dominada se contabiliza:

porcentaje = (celdas dueño / total celdas) * 100


Condiciones de fin:

Victoria del jugador: domina más territorio que la IA.

Derrota: la IA iguala o supera dominio tras el turno 20.

Victoria alternativa: ≥60% del mapa en cualquier turno.

Este sistema incentiva expansión territorial constante.

6. Casos de Prueba

A continuación, pruebas recomendadas para validar comportamiento del sistema:

6.1 Caso de Prueba: Reclutamiento

Objetivo: asegurar que unidades solo se reclutan si hay espacio y recursos.
Entradas: posición válida, posición ocupada, falta de recursos.
Resultado esperado:

Reclutamiento válido ✔

Mensaje de error en caso contrario ✔

6.2 Caso de Prueba: Construcción

Objetivo: validar construcción según terreno y requisitos.
Pruebas:

Construir en terreno válido

Construir en terreno prohibido

Construir sobre un edificio/unidad

Construcción por IA en secuencia jerárquica

Resultado: restricciones coherentes y mensaje adecuado.

6.3 Caso de Prueba: Movimiento

Casos:

Movimiento dentro del mapa

Movimiento a celda ocupada

Movimiento enemigo → captura

Resultado:

Movimiento aplicado correctamente

Captura actualiza propietario de la celda

6.4 Caso de Prueba: Producción y Mantenimiento

Validar que la cantidad de recursos aumenta o disminuye según edificios activos.

6.5 Caso de Prueba: IA

Objetivo: verificar que IA cumple prioridades.
Pasos:

Iniciar turno sin cuarteles → IA debe construir uno.

Recursos bajos → IA debe construir granja.

Unidad enemiga adyacente → IA debe mover/atacar.

6.6 Caso de Prueba: Condiciones de Victoria

Entrada: distintos valores de dominio.
Resultado:

Jugador gana si domina más.

IA gana si empata o supera pasados 20 turnos.

Victoria inmediata al 60%.

Conclusión

El sistema está compuesto por módulos cohesivos y desacoplados, lo que facilita mantenimiento y ampliaciones (más IA, nuevos terrenos, nuevos edificios).
La IA, aunque simple, respeta una jerarquía de prioridades que genera partidas consistentes.
El balance económico asegura que las decisiones del jugador y la IA tengan impacto significativo.
Los casos de prueba permiten validar la integridad del sistema y asegurar estabilidad en versiones futuras.
