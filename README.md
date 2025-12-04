# Proyecto Final – Juego de Estrategia por Turnos con IA Guardian

Este proyecto implementa un juego de estrategia por turnos donde el jugador compite contra una IA por el dominio del mapa. Incluye sistemas de economía, reclutamiento, construcción, movimiento, control territorial y un motor de IA modular.

---

## ⚙️ Arquitectura del Sistema

La arquitectura se divide en módulos independientes para permitir mantenimiento y ampliación:

### **Módulos principales**
- **`Juego`**  
  Ciclo completo del turno: inicio → fase jugador → IA → evaluación → fin.

- **`Contexto`**  
  Estado global: mapa, recursos, unidades, edificios, dominio, bitácora.

- **`MapaMundo`**  
  Manejo de celdas y reglas: movimiento, reclutamiento, construcción y conteo territorial.

- **`MotorEventos`**  
  Ejecuta efectos o alteraciones ambientales del mundo.

- **`GestorRecursos`**  
  Controla producción económica y mantenimiento de unidades.

- **`IA_Guardian`**  
  Lógica del jugador 2: recluta, construye y captura territorio.

---

## Flujo del Turno

turnoInicio()
→ impresión de mapa y recursos
→ faseJugador()
→ mover / reclutar / construir / pasar
→ faseMundo()
→ IA + eventos + producción
→ evaluarCondiciones()
→ turnoFin()

markdown
Copiar código

Después de **cada acción del jugador**, el mapa se actualiza visualmente.

---

## 🤖 IA Guardian (Jugador 2)

La IA ejecuta hasta **3 acciones por turno**, en este orden:

### **1. Reclutamiento**
- Prioriza Soldado y Arquero.
- Caballero y Mago requieren una **Forja**.

### **2. Construcción**
Orden de prioridad:
1. Construir **Cuartel** si no tiene suficientes.
2. Construir **Torre** si ya tiene cuarteles.
3. Construir **Forja** si ya tiene torres.
4. Construir **Granja** si la comida está baja.

### **3. Movimiento ofensivo**
- Captura edificios enemigos adyacentes.
- Se expande hacia celdas neutrales estratégicas.

### **Registro de acciones**
Solo se muestran logs del turno actual:

Recluto Soldado en (x,y)
Construyo Torre en (x,y)
Unidad movida y capturó (x,y)

---

## 🏗️ Edificios y Unidades

### **Edificios**
| Edificio | Función | Habilita |
|----------|---------|-----------|
| **Granja** | Produce Comida | — |
| **Cuartel** | Permite reclutar unidades básicas | Soldado, Arquero |
| **Torre** | Defensa territorial | — |
| **Forja** | Tecnología avanzada | Caballero, Mago |

### **Unidades**
| Unidad | Requisito | Rol |
|--------|-----------|------|
| Soldado | Cuartel | Básico, económico |
| Arquero | Cuartel | Ataques a distancia (si aplica) |
| Ingeniero | Ninguno | Soporte |
| Caballero | Forja | Alta resistencia |
| Mago | Forja | Avanzado |

---

## 📊 Balance del Juego

### **Cálculo de dominio**
Dominio = (celdas ocupadas / total celdas) * 100

### **Condiciones de victoria**
- **Victoria inmediata** → si el jugador domina más territorio que la IA.  
- **Derrota** → si al turno 20 la IA tiene igual o mayor dominio.  
- **Victoria alternativa** → si el jugador posee **≥60% del mapa**.

### **Economía**
- Edificios producen recursos cada turno.
- Unidades consumen mantenimiento.
- La construcción o reclutamiento falla si no hay recursos suficientes.

---

## Casos de Prueba

### **Reclutamiento**
- Reclutar en celda vacía → éxito.
- Reclutar sobre unidad/edificio → error correcto.
- Reclutar sin recursos → rechazo esperado.

### **Construcción**
- Terreno válido → éxito.
- Intentar construir sobre ocupantes → error.
- Construir Forja sin Torre → prohibido.

### **Movimiento**
- Movimientos válidos funcionan.
- Movimiento hacia casilla ocupada → rechazado.
- Movimiento hacia edificio enemigo → captura.

### **IA**
- IA sin cuartel → construye cuartel.
- IA sin comida → construye granja.
- IA cerca de enemigo → ataca.

### **Condiciones de victoria**
- Jugador supera dominio IA → victoria.
- Al turno 20 IA ≥ jugador → derrota.

---

## 📝 Conclusión

Este proyecto implementa un sistema modular de estrategia por turnos con IA funcional,
economía, construcción, control territorial y reportes claros de acciones.

El diseño permite agregar fácilmente nuevas unidades, edificios o comportamientos.
