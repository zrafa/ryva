# EJEMPLO: FILTRO DE KALMAN EXTENDIDO PARA UN MÓVIL DE BAJA VELOCIDAD

Este ejemplo desarrolla, de principio a fin, un **EKF discreto** para un móvil terrestre que se desplaza lentamente.

El objetivo es estimar su posición, rumbo y velocidad combinando:

- GPS a **1 Hz**, que entrega posición `(x,y)` y velocidad `v`.
- odometría a **5 Hz**, que entrega la velocidad longitudinal.
- ángulo de dirección de las ruedas a **5 Hz**.

El modelo cinemático elegido es el **modelo de bicicleta**. La estructura sigue el desarrollo del capítulo sobre EKF discreto: modelo no lineal, linealización mediante serie de Taylor, matrices Jacobianas, propagación de la covarianza y actualización de medición. En particular, el procedimiento corresponde al esquema de las ecuaciones (13.44)-(13.49) del capítulo.

---

## 1. Planteamiento del problema

Tomaremos un sistema de coordenadas planas `(x,y)`. El rumbo del móvil es $ψ$ y la velocidad longitudinal es `v`.

Definimos el estado como

$$
x_k =
\begin{bmatrix}
x_k \\
y_k \\
\psi_k \\
v_k
\end{bmatrix}.
$$

El parámetro geométrico del vehículo es la distancia entre ejes

$$
L = 1.50\;\mathrm{m}.
$$

El sensor de dirección entrega el ángulo de las ruedas delanteras $δ$.

---

## 2. Modelo no lineal de movimiento

Para el modelo de bicicleta cinemático:

$$
\dot{x} = v\cos\psi,
$$

$$
\dot{y} = v\sin\psi,
$$

$$
\dot{\psi} = \frac{v}{L}\tan\delta,
$$

$$
\dot{v} = 0.
$$

El modelo ideal supone que la velocidad permanece constante durante un intervalo corto. Las discrepancias producidas por terreno irregular, pequeños deslizamientos, errores geométricos y otras perturbaciones se representan mediante ruido de proceso.

Escribimos entonces

$$
\dot{x} = f(x,u,w),
$$

con

$$
u = \delta
$$

y, conceptualmente,

$$
w =
\begin{bmatrix}
w_x & w_y & w_\psi & w_v
\end{bmatrix}^{T}.
$$

Esto sigue la formulación general del capítulo, donde el sistema no lineal se expresa como una función `f` del estado, las entradas y el ruido. El capítulo enfatiza que el sistema se linealiza localmente mediante una expansión de Taylor y que el EKF utiliza la propia estimación como punto de linealización.

---

## 3. Qué hacemos con cada sensor

Hay aquí una distinción importante.

### 3.1 Ángulo de dirección

El ángulo de dirección no es, en este ejemplo, un estado que queramos estimar. Por ello se utiliza como **entrada del modelo**:

$$
u_k = \delta_k^{\mathrm{medida}}.
$$

Su error de medición no se ignora: se introduce en la covarianza del ruido de proceso. Así el EKF sabe que la predicción de $ψ$ es incierta porque $δ$ no es exacto.

### 3.2 Odometría

La odometría mide directamente la velocidad longitudinal. La utilizaremos como una **medición del estado `v`**:

$$
z_k^{odo} = v_k + n_k^{odo},
$$

con

$$
n_k^{odo}\sim\mathcal N(0,R_{odo}).
$$

### 3.3 GPS

Cada 1 segundo el GPS entrega posición y velocidad:

$$
z_k^{GPS} =
\begin{bmatrix}
x_k \\
y_k \\
v_k
\end{bmatrix}
+
n_k^{GPS},
$$

con

$$
n_k^{GPS}\sim\mathcal N(0,R_{GPS}).
$$

Por lo tanto, en este ejemplo el ciclo de cálculo es:

**predicción con $δ$ → actualización con odometría → cada 1 s, actualización adicional con GPS.**

---

## 4. Discretización mediante serie de Taylor

El capítulo y el trabajo de referencia parten de sistemas que pueden ser continuos pero señalan que, para implementarlos en una computadora, resulta habitual discretizar la dinámica.

Sea

$$
t_{k+1}=t_k+\Delta t,
$$

con

$$
\Delta t = 0.2\;\mathrm{s}
$$

porque los sensores de 5 Hz producen una muestra cada `0.2 s`.

Para cualquier componente del estado usamos la expansión de Taylor en el tiempo:

$$
x(t+\Delta t)
=
x(t)+\dot{x}(t)\Delta t
+\frac{1}{2}\ddot{x}(t)\Delta t^2+\cdots.
$$

Para un intervalo pequeño conservamos el primer orden:

$$
x(t+\Delta t)\approx x(t)+\dot{x}(t)\Delta t.
$$

Aplicando esto a las cuatro ecuaciones del modelo:

$$
x_{k+1}=x_k+v_k\cos\psi_k\,\Delta t,
$$

$$
y_{k+1}=y_k+v_k\sin\psi_k\,\Delta t,
$$

$$
\psi_{k+1}=\psi_k+\frac{v_k}{L}\tan\delta_k\,\Delta t,
$$

$$
v_{k+1}=v_k.
$$

Ésta es la función no lineal discreta que utilizaremos:

$$
x_{k+1}=f_k(x_k,\delta_k,w_k).
$$

---

## 5. Linealización mediante serie de Taylor

Ahora aparece la segunda utilización de Taylor: no para discretizar en el tiempo, sino para **linealizar la función no lineal respecto del estado**.

Alrededor de la estimación actual $\hat{x}_k^+$:

$$
f(x_k)\approx
f(\hat{x}_k^+)
+
\left.
\frac{\partial f}{\partial x}
\right|_{\hat{x}_k^+}
(x_k-\hat{x}_k^+).
$$

Definimos entonces el Jacobiano:

$$
F_k=
\left.
\frac{\partial f_k}{\partial x}
\right|_{\hat{x}_k^+}.
$$

El punto de linealización cambia en cada iteración: es la **estimación actual del EKF**. Esto es justamente la idea del EKF descrita en el capítulo: utilizar la estimación como trayectoria/punto nominal y volver a linealizar en cada paso.

---

## 6. Cálculo explícito del Jacobiano `F`

Partimos de

$$
f(x,u)=
\begin{bmatrix}
x+v\cos\psi\,\Delta t \\
y+v\sin\psi\,\Delta t \\
\psi+\frac{v}{L}\tan\delta\,\Delta t \\
v
\end{bmatrix}.
$$

Las derivadas parciales son

$$
\frac{\partial f_1}{\partial x}=1,\quad
\frac{\partial f_1}{\partial y}=0,\quad
\frac{\partial f_1}{\partial\psi}=-v\sin\psi\,\Delta t,\quad
\frac{\partial f_1}{\partial v}=\cos\psi\,\Delta t.
$$

Para `y`:

$$
\frac{\partial f_2}{\partial x}=0,\quad
\frac{\partial f_2}{\partial y}=1,\quad
\frac{\partial f_2}{\partial\psi}=v\cos\psi\,\Delta t,\quad
\frac{\partial f_2}{\partial v}=\sin\psi\,\Delta t.
$$

Para el rumbo:

$$
\frac{\partial f_3}{\partial x}=0,\quad
\frac{\partial f_3}{\partial y}=0,\quad
\frac{\partial f_3}{\partial\psi}=1,\quad
\frac{\partial f_3}{\partial v}=\frac{\tan\delta}{L}\Delta t.
$$

Y para la velocidad:

$$
\frac{\partial f_4}{\partial x}=0,\quad
\frac{\partial f_4}{\partial y}=0,\quad
\frac{\partial f_4}{\partial\psi}=0,\quad
\frac{\partial f_4}{\partial v}=1.
$$

Por lo tanto:

$$
F_k=
\begin{bmatrix}
1 & 0 & -\hat v_k^+\sin\hat\psi_k^+\,\Delta t & \cos\hat\psi_k^+\,\Delta t \\
0 & 1 & \hat v_k^+\cos\hat\psi_k^+\,\Delta t & \sin\hat\psi_k^+\,\Delta t \\
0 & 0 & 1 & \dfrac{\tan\delta_k}{L}\Delta t \\
0 & 0 & 0 & 1
\end{bmatrix}.
$$

Este paso es el equivalente directo al cálculo del Jacobiano `F` que aparece en el Ejemplo 13.3 del capítulo, donde las posiciones dependen no linealmente del rumbo y la velocidad.

---

## 7. Ruido de proceso

Usaremos cinco fuentes de incertidumbre por intervalo:

1. error de modelado en `x`;
2. error de modelado en `y`;
3. error de modelado del rumbo;
4. error de la dinámica de velocidad;
5. error del ángulo de dirección.

El vector de ruido es

$$
\eta_k=
\begin{bmatrix}
w_x & w_y & w_\psi & w_v & n_\delta
\end{bmatrix}^{T}.
$$

Supongamos:

$$
\sigma_x=\sigma_y=0.008\;m,
$$

$$
\sigma_\psi=0.15^\circ,
$$

$$
\sigma_v=0.008\;m/s,
$$

$$
\sigma_\delta=0.7^\circ.
$$

Su matriz de covarianza es

$$
Q_\eta=
\operatorname{diag}
(\sigma_x^2,\sigma_y^2,\sigma_\psi^2,\sigma_v^2,\sigma_\delta^2).
$$

El error del ángulo de dirección modifica el incremento del rumbo. A primer orden:

$$
\Delta\psi_\delta
\approx
\frac{\partial}{\partial\delta}
\left(
\frac{v}{L}\tan\delta\,\Delta t
\right)
n_\delta.
$$

Por lo tanto:

$$
a_\delta=
\frac{v}{L}\sec^2\delta\,\Delta t.
$$

Podemos escribir la matriz `L` de entrada del ruido como

$$
L_k=
\begin{bmatrix}
1&0&0&0&0\\
0&1&0&0&0\\
0&0&1&0&a_\delta\\
0&0&0&1&0
\end{bmatrix}.
$$

y la covarianza de proceso efectiva utilizada en la predicción:

$$
Q_k^{eff}=L_kQ_\eta L_k^T.
$$

Así, el EKF aumenta la incertidumbre del rumbo cuando el ángulo de dirección tiene incertidumbre.

---

## 8. Medición de odometría

La odometría mide la velocidad:

$$
z_k^{odo}=h_{odo}(x_k)+n_k^{odo}=v_k+n_k^{odo}.
$$

Su Jacobiano es

$$
H_k^{odo}=
\begin{bmatrix}
0&0&0&1
\end{bmatrix}.
$$

Usamos

$$
\sigma_{odo}=0.015\;m/s,
$$

por lo que

$$
R_{odo}=\sigma_{odo}^2.
$$

---

## 9. Medición GPS

El GPS proporciona tres cantidades:

$$
z_k^{GPS}=
\begin{bmatrix}
x_k\\
y_k\\
v_k
\end{bmatrix}
+
n_k^{GPS}.
$$

El Jacobiano es directamente

$$
H_k^{GPS}=
\begin{bmatrix}
1&0&0&0\\
0&1&0&0\\
0&0&0&1
\end{bmatrix}.
$$

Su matriz de ruido será

$$
R_{GPS}=
\operatorname{diag}
(\sigma_{GPS,x}^2,\sigma_{GPS,y}^2,\sigma_{GPS,v}^2),
$$

con

$$
\sigma_{GPS,x}=\sigma_{GPS,y}=0.70\;m
$$

y

$$
\sigma_{GPS,v}=0.05\;m/s.
$$

El modelo de medición GPS es lineal, por lo que aquí no hace falta una aproximación no lineal adicional. La no linealidad importante del problema está en la dinámica del vehículo.

---

## 10. Ecuaciones completas del EKF

El capítulo presenta el EKF discreto como un ciclo de **predicción + actualización**. Para cada `k`:

### Paso 1: calcular `F_k` y `L_k`

Se calculan los Jacobianos usando la estimación anterior $\hat{x}_{k-1}^+$.

### Paso 2: predecir el estado

$$
\hat{x}_k^- = f(\hat{x}_{k-1}^+,\delta_k,0).
$$

### Paso 3: predecir la covarianza

$$
P_k^- = F_{k-1}P_{k-1}^+F_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T.
$$

### Paso 4: actualizar con odometría

$$
S_k^{odo}=H_k^{odo}P_k^-{H_k^{odo}}^T+R_{odo},
$$

$$
K_k^{odo}=P_k^-{H_k^{odo}}^T(S_k^{odo})^{-1},
$$

$$
\hat{x}_k=\hat{x}_k^-
+K_k^{odo}
[z_k^{odo}-h_{odo}(\hat{x}_k^-)].
$$

$$
P_k=(I-K_k^{odo}H_k^{odo})P_k^-.
$$

### Paso 5: si hay GPS, actualizar nuevamente

Cuando `t_k` coincide con un instante de GPS:

$$
S_k^{GPS}=H_k^{GPS}P_k{H_k^{GPS}}^T+R_{GPS},
$$

$$
K_k^{GPS}=P_k{H_k^{GPS}}^T(S_k^{GPS})^{-1},
$$

$$
\hat{x}_k^+
=
\hat{x}_k
+K_k^{GPS}
[z_k^{GPS}-h_{GPS}(\hat{x}_k)].
$$

$$
P_k^+
=(I-K_k^{GPS}H_k^{GPS})P_k.
$$

Al finalizar el instante `k`, $\hat{x}_k^+$ y `P_k^+` son la estimación y la covarianza que pasan al siguiente intervalo.

---

## 11. Qué ocurre en una iteración concreta

Supongamos que estamos en `t=2.4 s`.

En ese instante se dispone de:

- el estado estimado en `2.2 s`;
- el ángulo de dirección medido a `5 Hz`;
- la odometría de `2.4 s`;
- y, como `2.4 s` no es múltiplo entero de `1 s`, no hay GPS.

Entonces:

$$
(\hat{x}_{2.2}^+,P_{2.2}^+)
\xrightarrow[\delta_{2.4}]{\mathrm{predicción}}
(\hat{x}_{2.4}^-,P_{2.4}^-)
$$

y luego:

$$
(\hat{x}_{2.4}^-,P_{2.4}^-)
\xrightarrow[z_{2.4}^{odo}]{\mathrm{actualización}}
(\hat{x}_{2.4}^+,P_{2.4}^+).
$$

En `t=3.0 s` sucede lo mismo, pero después de la odometría llega también el GPS:

$$
\mathrm{predicción}
\rightarrow
\mathrm{odometría}
\rightarrow
\mathrm{GPS}.
$$

Éste es todo el ciclo de ejecución.

---


## 11.1. Una iteración numérica completa

Para que las ecuaciones anteriores no queden solamente en forma simbólica, hagamos explícitamente la **primera iteración** del filtro, en `t = 0.2 s`.

El filtro comienza en

$$
\hat{x}_0^+=
\begin{bmatrix}
0.40\\
-0.30\\
5^\circ\\
0.45
\end{bmatrix}
$$

y

$$
P_0^+=
\operatorname{diag}
(0.35^2,0.35^2,(8^\circ)^2,0.08^2).
$$

La medición ficticia de esta primera muestra es:

$$
z_1^{odo}=0.59846\;m/s
$$

y el ángulo de dirección medido es

$$
\delta_1=2.235^\circ.
$$

### Predicción del estado

Aplicamos

$$
\hat{x}_1^-=
f(\hat{x}_0^+,\delta_1,0),
$$

obteniendo

$$
\hat{x}_1^- \approx
\begin{bmatrix}
0.48966\\
-0.29216\\
0.08866\\
0.45000
\end{bmatrix}.
$$

El tercer componente está expresado en radianes, de modo que `0.08866 rad ≈ 5.080°`.

### Jacobiano `F`

Evaluado en el estado inicial:

$$
F_0\approx
\begin{bmatrix}
1&0&-0.007844&0.199239\\
0&1&0.089658&0.017431\\
0&0&1&0.003104\\
0&0&0&1
\end{bmatrix}.
$$

### Entrada del ruido producido por el ángulo de dirección

Para esta muestra,

$$
a_\delta=
\frac{v}{L}\sec^2\delta\,\Delta t
\approx 0.06003.
$$

Por lo tanto,

$$
L_0=
\begin{bmatrix}
1&0&0&0&0\\
0&1&0&0&0\\
0&0&1&0&0.06003\\
0&0&0&1&0
\end{bmatrix}.
$$

Con los valores de ruido definidos anteriormente:

$$
Q_1^{eff}=L_0Q_\eta L_0^T
\approx
\begin{bmatrix}
0.000064&0&0&0\\
0&0.000064&0&0\\
0&0&0.00000739&0\\
0&0&0&0.000064
\end{bmatrix}.
$$

### Propagación de la covarianza

Entonces,

$$
P_1^- = F_0P_0^+F_0^T+Q_1^{eff},
$$

y numéricamente:

$$
P_1^-\approx
\begin{bmatrix}
0.122819&0.000009&-0.000149&0.001275\\
0.000009&0.122723&0.001748&0.000112\\
-0.000149&0.001748&0.019503&0.000020\\
0.001275&0.000112&0.000020&0.006464
\end{bmatrix}.
$$

### Actualización con odometría

El Jacobiano de la medición es

$$
H^{odo}=
\begin{bmatrix}
0&0&0&1
\end{bmatrix}.
$$

La innovación es

$$
\nu_1=
z_1^{odo}-H^{odo}\hat{x}_1^-
=
0.59846-0.45
=
0.14846\;m/s.
$$

La covarianza de la innovación es

$$
S_1=
H^{odo}P_1^-{H^{odo}}^T+R_{odo}
\approx0.006689.
$$

La ganancia de Kalman resulta

$$
K_1^{odo}\approx
\begin{bmatrix}
0.19063\\
0.01668\\
0.00297\\
0.96636
\end{bmatrix}.
$$

Finalmente:

$$
\hat{x}_1^+
=
\hat{x}_1^-
+
K_1^{odo}\nu_1,
$$

por lo que

$$
\hat{x}_1^+\approx
\begin{bmatrix}
0.51796\\
-0.28968\\
0.08910\\
0.59347
\end{bmatrix}.
$$

La velocidad pasó de `0.45 m/s`, que era la estimación previa, a `0.593 m/s`, porque la odometría indicó `0.598 m/s` y su incertidumbre es pequeña.

La nueva covarianza queda aproximadamente:

$$
P_1^+\approx
\begin{bmatrix}
0.122576&-0.000013&-0.000153&0.000043\\
-0.000013&0.122721&0.001748&0.000004\\
-0.000153&0.001748&0.019503&0.000001\\
0.000043&0.000004&0.000001&0.000217
\end{bmatrix}.
$$

Este último resultado muestra claramente qué hace la medición de odometría: reduce mucho la incertidumbre de la velocidad, mientras que las demás variables se modifican indirectamente mediante las correlaciones de la matriz `P`.

A partir de aquí, la siguiente iteración vuelve a comenzar exactamente igual, pero usando $\hat{x}_1^+$ y `P_1^+` como entrada. En `t=0.4, 0.6, ..., 4.8 s` sólo habrá predicción + odometría. En `t=1,2,3,4,5 s` habrá además una segunda actualización con GPS.

---

## 12. Inicialización

Para que el ejemplo empiece con un error deliberado, usamos:

$$
\hat{x}_0^+=
\begin{bmatrix}
0.40\\
-0.30\\
5^\circ\\
0.45
\end{bmatrix}.
$$

Es decir, el filtro comienza desplazado respecto del ground truth y con una velocidad inferior.

La covarianza inicial es

$$
P_0^+=
\operatorname{diag}
(0.35^2,0.35^2,(8^\circ)^2,0.08^2).
$$

---

## 13. Datos ficticios utilizados

Se generaron 25 muestras de prueba, equivalentes a 5 segundos a 5 Hz. Los datos son deliberadamente ficticios; sirven para mostrar el mecanismo de cálculo y no para representar un sensor concreto.

### GPS: 5 muestras, 1 Hz

| t [s] | x GPS [m] | y GPS [m] | v GPS [m/s] |
|---:|---:|---:|---:|
| 1.0 | 0.431 | 0.006 | 0.521 |
| 2.0 | 0.553 | -0.754 | 0.555 |
| 3.0 | 2.519 | -0.557 | 0.557 |
| 4.0 | 2.005 | -0.033 | 0.525 |
| 5.0 | 2.660 | 0.361 | 0.457 |

### Odometría y dirección: 25 muestras, 5 Hz

| k | t [s] | x GT [m] | y GT [m] | ψ GT [°] | v GT [m/s] | odometría [m/s] | δ medido [°] | x EKF [m] | y EKF [m] | ψ EKF [°] | v EKF [m/s] | GPS |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---:|
| 1 | 0.2 | 0.115 | 0.000 | 0.082 | 0.576 | 0.598 | 1.334 | 0.518 | -0.290 | 5.105 | 0.593 | no |
| 2 | 0.4 | 0.232 | 0.000 | 0.207 | 0.584 | 0.565 | 0.974 | 0.631 | -0.280 | 5.179 | 0.578 | no |
| 3 | 0.6 | 0.350 | 0.001 | 0.369 | 0.591 | 0.603 | 1.485 | 0.750 | -0.269 | 5.296 | 0.589 | no |
| 4 | 0.8 | 0.469 | 0.001 | 0.560 | 0.596 | 0.593 | 2.499 | 0.868 | -0.258 | 5.493 | 0.591 | no |
| 5 | 1.0 | 0.589 | 0.003 | 0.773 | 0.598 | 0.604 | 2.543 | 0.875 | -0.193 | 6.001 | 0.594 | sí |
| 6 | 1.2 | 0.709 | 0.004 | 0.998 | 0.598 | 0.599 | 3.055 | 0.994 | -0.181 | 6.244 | 0.596 | no |
| 7 | 1.4 | 0.828 | 0.006 | 1.225 | 0.597 | 0.601 | 2.615 | 1.113 | -0.168 | 6.453 | 0.598 | no |
| 8 | 1.6 | 0.947 | 0.009 | 1.446 | 0.594 | 0.567 | 3.583 | 1.228 | -0.155 | 6.732 | 0.585 | no |
| 9 | 1.8 | 1.065 | 0.012 | 1.655 | 0.590 | 0.572 | 2.867 | 1.343 | -0.141 | 6.952 | 0.580 | no |
| 10 | 2.0 | 1.182 | 0.015 | 1.845 | 0.586 | 0.620 | 1.640 | 1.311 | -0.247 | 6.086 | 0.595 | sí |
| 11 | 2.2 | 1.298 | 0.019 | 2.013 | 0.582 | 0.594 | 3.242 | 1.430 | -0.234 | 6.344 | 0.594 | no |
| 12 | 2.4 | 1.414 | 0.023 | 2.158 | 0.578 | 0.549 | 3.233 | 1.543 | -0.222 | 6.590 | 0.576 | no |
| 13 | 2.6 | 1.529 | 0.027 | 2.277 | 0.576 | 0.588 | 1.294 | 1.659 | -0.208 | 6.691 | 0.581 | no |
| 14 | 2.8 | 1.643 | 0.032 | 2.373 | 0.574 | 0.574 | 0.579 | 1.773 | -0.195 | 6.735 | 0.578 | no |
| 15 | 3.0 | 1.758 | 0.037 | 2.446 | 0.573 | 0.563 | 2.295 | 1.980 | -0.259 | 5.790 | 0.571 | sí |
| 16 | 3.2 | 1.872 | 0.041 | 2.499 | 0.573 | 0.569 | 1.394 | 2.093 | -0.248 | 5.896 | 0.570 | no |
| 17 | 3.4 | 1.987 | 0.046 | 2.535 | 0.573 | 0.564 | -1.085 | 2.206 | -0.236 | 5.813 | 0.568 | no |
| 18 | 3.6 | 2.101 | 0.052 | 2.555 | 0.573 | 0.588 | -0.548 | 2.321 | -0.225 | 5.772 | 0.576 | no |
| 19 | 3.8 | 2.215 | 0.057 | 2.564 | 0.572 | 0.604 | -0.404 | 2.439 | -0.213 | 5.741 | 0.587 | no |
| 20 | 4.0 | 2.329 | 0.062 | 2.562 | 0.570 | 0.568 | -0.379 | 2.482 | -0.162 | 6.373 | 0.578 | sí |
| 21 | 4.2 | 2.442 | 0.067 | 2.552 | 0.567 | 0.542 | -0.020 | 2.593 | -0.149 | 6.371 | 0.563 | no |
| 22 | 4.4 | 2.555 | 0.072 | 2.534 | 0.562 | 0.564 | -0.315 | 2.705 | -0.137 | 6.348 | 0.564 | no |
| 23 | 4.6 | 2.666 | 0.077 | 2.509 | 0.556 | 0.532 | -0.965 | 2.814 | -0.125 | 6.277 | 0.551 | no |
| 24 | 4.8 | 2.776 | 0.082 | 2.475 | 0.549 | 0.565 | -0.429 | 2.925 | -0.113 | 6.244 | 0.557 | no |
| 25 | 5.0 | 2.884 | 0.086 | 2.431 | 0.541 | 0.512 | -0.784 | 2.982 | 0.002 | 7.750 | 0.535 | sí |

---

## 14. Resultados

Las figuras se generaron con exactamente los datos de las tablas anteriores.

![Trayectoria](figuras/trayectoria.png)

**Figura 1.** Trayectoria ficticia. La línea `ground truth` es la trayectoria usada para generar los sensores; los cuadrados corresponden al GPS; la línea marcada con `EKF` es la estimación fusionada.

![Velocidad](figuras/velocidad.png)

**Figura 2.** Velocidad verdadera, odometría, GPS y velocidad estimada por el EKF.

![Rumbo](figuras/rumbo.png)

**Figura 3.** Rumbo verdadero y rumbo estimado por el EKF.

Para esta realización ficticia, los errores RMS obtenidos son aproximadamente:

- posición: **0.339 m**;

- velocidad: **0.007 m/s**;

- rumbo: **0.077 rad** (4.409°).

Estos números dependen de la realización aleatoria de los ruidos y sirven solamente para ilustrar el comportamiento del filtro.

---

## 15. Resumen operativo del algoritmo

En una implementación real, el pseudocódigo queda reducido a:

```text
inicializar x_hat y P

para cada muestra de 5 Hz:

    leer delta_medida
    leer odometria

    calcular F y L
    x_hat_minus = f(x_hat, delta_medida, 0)
    P_minus = F P F^T + L Q L^T

    # actualización de odometría
    H = [0 0 0 1]
    S = H P_minus H^T + R_odo
    K = P_minus H^T inv(S)
    x_hat = x_hat_minus + K (odo - h(x_hat_minus))
    P = (I - K H) P_minus

    si llegó un GPS:
        H = [[1 0 0 0],
             [0 1 0 0],
             [0 0 0 1]]
        S = H P H^T + R_gps
        K = P H^T inv(S)
        x_hat = x_hat + K (gps - h(x_hat))
        P = (I - K H) P

    normalizar psi a [-pi, pi]
```

---

## 16. Qué aporta cada sensor

El **modelo de bicicleta** proporciona la relación geométrica entre velocidad, rumbo y ángulo de dirección.

La **odometría** mantiene muy bien estimada la velocidad entre las mediciones de GPS.

El **ángulo de dirección** permite predecir la evolución del rumbo y, por lo tanto, la dirección de desplazamiento.

El **GPS** corrige principalmente la deriva acumulada de la posición y aporta además una medición independiente de la velocidad.

El EKF combina esas fuentes ponderándolas mediante sus covarianzas `P`, `Q` y `R`. Esta es la misma idea que aparece en el ejemplo de robot móvil del capítulo: incluso cuando una variable no se mide directamente, el acoplamiento del modelo permite que otras mediciones contribuyan a estimarla.

---

## 17. Observación final sobre el modelo

Este ejemplo usa deliberadamente el modelo de bicicleta más simple. No incluye lateral slip, bias de odometría, bias del ángulo de dirección, pendiente del terreno, ni aceleración longitudinal explícita.

Para una implementación sobre un vehículo real, el paso siguiente natural sería agregar al estado alguno de esos términos —por ejemplo, un **bias de odometría** o un **bias del ángulo de dirección**— y volver a obtener los Jacobianos. La mecánica del EKF no cambia: se amplía el vector de estado y se recalculan `F`, `H` y las covarianzas.

---

## Referencias utilizadas para estructurar el ejemplo

1. Capítulo 13, *Filtrado de Kalman no lineal*: desarrollo del EKF discreto, serie de Taylor, Jacobianos `F`, `L`, `H`, `M`, propagación de `P` y actualización con mediciones.
2. Malik, A. M.; Riaz, S., *Single Seven State Discrete Time Extended Kalman Filter for Micro Air Vehicle*, WCE 2010: ejemplo de implementación discreta, linealización por Jacobianos y tratamiento de las covarianzas de proceso y medición.
