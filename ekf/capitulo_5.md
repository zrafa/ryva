*Fuente: Optimal State Estimation, primera edición, por Dan J. Simon. ISBN 0471708585. © 2006 John Wiley & Sons, Inc. (Capítulo 5, páginas 123–145 del original; el ejercicio 5.4, al final, queda incompleto porque ahí termina el material subido).*

---

# CAPÍTULO 5

# El filtro de Kalman de tiempo discreto

> El filtro de Kalman, en sus diversas formas, está claramente establecido como una herramienta fundamental para analizar y resolver una amplia clase de problemas de estimación.
>
> —Leonard McGee y Stanley Schmidt [McG85]

Este capítulo constituye el corazón de este libro. Los capítulos anteriores se escribieron únicamente para sentar las bases de este capítulo, y los capítulos posteriores se escribieron únicamente para ampliar y generalizar los resultados presentados en este capítulo.

Como veremos en este capítulo, el filtro de Kalman opera propagando en el tiempo la media y la covarianza del estado. Nuestro enfoque para deducir el filtro de Kalman constará de los siguientes pasos.

1. Partimos de una descripción matemática de un sistema dinámico cuyos estados queremos estimar.

2. Implementamos ecuaciones que describen cómo se propagan en el tiempo la media del estado y la covarianza del estado. Estas ecuaciones, deducidas en el capítulo 4, constituyen en sí mismas un sistema dinámico.

3. Tomamos el sistema dinámico que describe la propagación de la media y la covarianza del estado, e implementamos las ecuaciones en una computadora. Estas ecuaciones constituyen la base para la deducción del filtro de Kalman, ya que:

   (a) La media del estado es la estimación del estado que da el filtro de Kalman.

   (b) La covarianza del estado es la covarianza de la estimación del estado que da el filtro de Kalman.

4. Cada vez que obtenemos una medición, actualizamos la media y la covarianza del estado. Esto es similar a lo que hicimos en el capítulo 3, donde usamos las mediciones para actualizar de manera recursiva nuestra estimación de una constante.

En la sección 5.1 deduciremos las ecuaciones del filtro de Kalman de tiempo discreto. Esto incluye varias formas que se ven distintas entre sí, pero que son matemáticamente equivalentes. Diversos libros y artículos que tratan sobre filtros de Kalman presentan las ecuaciones del filtro de maneras que parecen muy distintas unas de otras. No siempre resulta obvio, pero estas distintas formulaciones son, en realidad, matemáticamente equivalentes, y lo veremos en la sección 5.1. (Las secciones 9.1, 10.5.1 y 11.1 también deducen formulaciones alternativas, pero equivalentes, de las ecuaciones del filtro de Kalman.) En la sección 5.2 examinaremos algunas de las propiedades teóricas del filtro de Kalman. Un aspecto notable del filtro de Kalman es que resulta óptimo en varios sentidos distintos, como veremos en la sección 5.2. En la sección 5.3 veremos cómo las ecuaciones del filtro de Kalman pueden escribirse con una única ecuación de actualización temporal. La sección 5.4 presenta una manera de obtener una ecuación en forma cerrada para el filtro de Kalman variante en el tiempo, para un sistema escalar invariante en el tiempo, y una manera de calcular rápidamente el filtro de Kalman de estado estacionario. La sección 5.5 examina algunas situaciones en las que el filtro de Kalman es inestable, o entrega estimaciones del estado que no se acercan al estado verdadero. También veremos algunas maneras de corregir la inestabilidad y la divergencia en el filtro de Kalman.

---

## 5.1 DEDUCCIÓN DEL FILTRO DE KALMAN DE TIEMPO DISCRETO

Supongamos que tenemos un sistema lineal de tiempo discreto dado de la siguiente manera:

$$
\begin{aligned}
x_k &= F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+w_{k-1} \\
y_k &= H_kx_k+v_k
\end{aligned}
\tag{5.1}
$$

Los procesos de ruido $\{w_k\}$ y $\{v_k\}$ son blancos, de media cero, no correlacionados, y tienen matrices de covarianza conocidas $Q_k$ y $R_k$, respectivamente:

$$
\begin{aligned}
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k) \\
E[w_kw_j^T] &= Q_k\delta_{k-j} \\
E[v_kv_j^T] &= R_k\delta_{k-j} \\
E[v_kw_j^T] &= 0
\end{aligned}
\tag{5.2}
$$

donde $\delta_{k-j}$ es la función delta de Kronecker; es decir, $\delta_{k-j}=1$ si $k=j$, y $\delta_{k-j}=0$ si $k \neq j$. Nuestro objetivo es estimar el estado $x_k$ a partir de nuestro conocimiento de la dinámica del sistema y de la disponibilidad de las mediciones ruidosas $\{y_k\}$. La cantidad de información de la que disponemos para nuestra estimación del estado varía según el problema particular que estemos tratando de resolver. Si disponemos de todas las mediciones hasta el instante $k$ inclusive para usarlas en nuestra estimación de $x_k$, entonces podemos formar una estimación *a posteriori*, que denotamos como $\hat{x}_k^+$. El superíndice "+" indica que la estimación es *a posteriori*. Una manera de formar
la estimación de estado *a posteriori* es calcular el valor esperado de $x_k$ condicionado a todas las mediciones hasta el instante $k$ inclusive:

$$
\hat{x}_k^+ = E[x_k|y_1,y_2,\cdots,y_k] = \text{estimación } a\ posteriori
\tag{5.3}
$$

Si disponemos de todas las mediciones anteriores (pero sin incluir) al instante $k$ para usarlas en nuestra estimación de $x_k$, entonces podemos formar una estimación *a priori*, que denotamos como $\hat{x}_k^-$. El superíndice "−" indica que la estimación es *a priori*. Una manera de formar la estimación de estado *a priori* es calcular el valor esperado de $x_k$ condicionado a todas las mediciones anteriores (pero sin incluir) al instante $k$:

$$
\hat{x}_k^- = E[x_k|y_1,y_2,\cdots,y_{k-1}] = \text{estimación } a\ priori
\tag{5.4}
$$

Es importante notar que $\hat{x}_k^-$ y $\hat{x}_k^+$ son ambas estimaciones de la misma cantidad; ambas son estimaciones de $x_k$. Sin embargo, $\hat{x}_k^-$ es nuestra estimación de $x_k$ antes de tener en cuenta la medición $y_k$, y $\hat{x}_k^+$ es nuestra estimación de $x_k$ después de tener en cuenta la medición $y_k$. Naturalmente esperamos que $\hat{x}_k^+$ sea una mejor estimación que $\hat{x}_k^-$, porque usamos más información para calcular $\hat{x}_k^+$:

$$
\begin{aligned}
\hat{x}_k^- &= \text{estimación de } x_k \text{ antes de procesar la medición en el instante } k \\
\hat{x}_k^+ &= \text{estimación de } x_k \text{ después de procesar la medición en el instante } k
\end{aligned}
\tag{5.5}
$$

Si disponemos de mediciones posteriores al instante $k$ para usarlas en nuestra estimación de $x_k$, entonces podemos formar una **estimación suavizada**. Una manera de formar la estimación de estado suavizada es calcular el valor esperado de $x_k$ condicionado a todas las mediciones disponibles:

$$
\hat{x}_{k|k+N} = E[x_k|y_1,y_2,\cdots,y_k,\cdots,y_{k+N}] = \text{estimación suavizada}
\tag{5.6}
$$

donde $N$ es un entero positivo cuyo valor depende del problema específico que se esté resolviendo. Si queremos hallar la mejor predicción de $x_k$ más de un instante por delante de las mediciones disponibles, entonces podemos formar una **estimación predicha**. Una manera de formar la estimación de estado predicha es calcular el valor esperado de $x_k$ condicionado a todas las mediciones disponibles:

$$
\hat{x}_{k|k-M} = E[x_k|y_1,y_2,\cdots,y_{k-M}] = \text{estimación predicha}
\tag{5.7}
$$

donde $M$ es un entero positivo cuyo valor depende del problema específico que se esté resolviendo. La relación entre las estimaciones de estado *a posteriori*, *a priori*, suavizada y predicha se ilustra en la figura 5.1.

En la notación que sigue, usamos $\hat{x}_0^+$ para denotar nuestra estimación inicial de $x_0$ antes de que haya mediciones disponibles. La primera medición se toma en el instante $k=1$. Dado que no disponemos de ninguna medición para estimar $x_0$, resulta razonable formar $\hat{x}_0^+$ como el valor esperado del estado inicial $x_0$:

$$
\hat{x}_0^+ = E(x_0)
\tag{5.8}
$$

Usamos el término $P_k$ para denotar la covarianza del error de estimación. $P_k^-$ denota la covarianza del error de estimación de $\hat{x}_k^-$, y $P_k^+$ denota la covarianza del error de estimación de $\hat{x}_k^+$:

$$
\begin{aligned}
P_k^- &= E[(x_k-\hat{x}_k^-)(x_k-\hat{x}_k^-)^T] \\
P_k^+ &= E[(x_k-\hat{x}_k^+)(x_k-\hat{x}_k^+)^T]
\end{aligned}
\tag{5.9}
$$

> *[Figura 5.1 — línea de tiempo del 1 al 9, con flechas que señalan, de izquierda a derecha: "estimación suavizada $\hat{x}_{1/5}$" (hacia el instante 1), "estimación a posteriori $\hat{x}_5^+$" (hacia el instante 5), "estimación a priori $\hat{x}_6^-$" (hacia el instante 6), "predicción $\hat{x}_{9/5}$" (hacia el instante 9).]*
>
> **Figura 5.1** Línea de tiempo que muestra la relación entre las estimaciones de estado *a posteriori*, *a priori*, suavizada y predicha. En esta figura suponemos que hemos recibido mediciones en los instantes hasta $k=5$ inclusive. Una estimación del estado en $k<5$ se llama estimación suavizada. Una estimación del estado en $k=5$ se llama estimación *a posteriori*. Una estimación del estado en $k=6$ se llama estimación *a priori*. Una estimación del estado en $k>6$ se llama predicción.

Estas relaciones se ilustran en la figura 5.2. La figura muestra que, después de procesar la medición en el instante $(k-1)$, tenemos una estimación de $x_{k-1}$ (denotada $\hat{x}_{k-1}^+$) y la covarianza de esa estimación (denotada $P_{k-1}^+$). Cuando llega el instante $k$, antes de procesar la medición en el instante $k$ calculamos una estimación de $x_k$ (denotada $\hat{x}_k^-$) y la covarianza de esa estimación (denotada $P_k^-$). Luego procesamos la medición en el instante $k$ para refinar nuestra estimación de $x_k$. La estimación resultante de $x_k$ se denota $\hat{x}_k^+$, y su covarianza se denota $P_k^+$.

> *[Figura 5.2 — línea de tiempo del instante $k-1$ al instante $k$: en la fila superior, $\hat{x}_{k-1}^-,\hat{x}_{k-1}^+ \to \hat{x}_k^-,\hat{x}_k^+$; en la fila inferior, $P_{k-1}^-,P_{k-1}^+ \to P_k^-,P_k^+$.]*
>
> **Figura 5.2** Línea de tiempo que muestra las estimaciones de estado *a priori* y *a posteriori*, y las covarianzas del error de estimación.

Comenzamos el proceso de estimación con $\hat{x}_0^+$, nuestra mejor estimación del estado inicial $x_0$. Dado $\hat{x}_0^+$, ¿cómo deberíamos calcular $\hat{x}_1^-$? Queremos fijar $\hat{x}_1^- = E(x_1)$. Pero notemos que $\hat{x}_0^+ = E(x_0)$, y recordemos, a partir de la ecuación (4.2), cómo se propaga en el tiempo la media de $x$: $\bar{x}_k = F_{k-1}\bar{x}_{k-1}+G_{k-1}u_{k-1}$. Por lo tanto, obtenemos

$$
\hat{x}_1^- = F_0\hat{x}_0^++G_0u_0
\tag{5.10}
$$

Esta es una ecuación específica que muestra cómo obtener $\hat{x}_1^-$ a partir de $\hat{x}_0^+$. Sin embargo, el razonamiento puede extenderse para obtener la siguiente ecuación más general:

$$
\hat{x}_k^- = F_{k-1}\hat{x}_{k-1}^++G_{k-1}u_{k-1}
\tag{5.11}
$$

Esta se denomina la **ecuación de actualización temporal** para $\hat{x}$. Desde el instante $(k-1)^+$ hasta el instante $k^-$, la estimación del estado se propaga de la misma manera en que se propaga la media del estado. Esto tiene sentido intuitivamente. No disponemos de ninguna medición adicional que nos ayude a actualizar nuestra estimación del estado entre el instante $(k-1)^+$ y el instante $k^-$, de modo que simplemente debemos actualizar la estimación del estado a partir de nuestro conocimiento de la dinámica del sistema.

A continuación necesitamos calcular la ecuación de actualización temporal para $P$, la covarianza del error de estimación del estado. Comenzamos con $P_0^+$, que es la covarianza de nuestra estimación inicial de $x_0$. Si conocemos perfectamente el estado inicial, entonces $P_0^+=0$. Si no tenemos absolutamente ninguna idea del valor de $x_0$, entonces $P_0^+=\infty I$. En general, $P_0^+$ representa la incertidumbre de nuestra estimación inicial de $x_0$:

$$
\begin{aligned}
P_0^+ &= E[(x_0-\bar{x}_0)(x_0-\bar{x}_0)^T] \\
&= E[(x_0-\hat{x}_0^+)(x_0-\hat{x}_0^+)^T]
\end{aligned}
\tag{5.12}
$$

Dado $P_0^+$, ¿cómo podemos calcular $P_1^-$? Recordemos, a partir de la ecuación (4.4), cómo se propaga en el tiempo la covarianza del estado de un sistema lineal de tiempo discreto: $P_k = F_{k-1}P_{k-1}F_{k-1}^T+Q_{k-1}$. Por lo tanto, obtenemos

$$
P_1^- = F_0P_0^+F_0^T+Q_0
\tag{5.13}
$$

Esta es una ecuación específica que muestra cómo obtener $P_1^-$ a partir de $P_0^+$. Sin embargo, el razonamiento puede extenderse para obtener la siguiente ecuación más general:

$$
P_k^- = F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1}
\tag{5.14}
$$

Esta se denomina la **ecuación de actualización temporal** para $P$.

Hemos deducido las ecuaciones de actualización temporal para $\hat{x}$ y $P$. Ahora necesitamos deducir las **ecuaciones de actualización de la medición** para $\hat{x}$ y $P$. Dado $\hat{x}_k^-$, ¿cómo deberíamos calcular $\hat{x}_k^+$? La cantidad $\hat{x}_k^-$ es una estimación de $x_k$, y la cantidad $\hat{x}_k^+$ también es una estimación de $x_k$. La única diferencia entre $\hat{x}_k^-$ y $\hat{x}_k^+$ es que $\hat{x}_k^+$ tiene en cuenta la medición $y_k$. Recordemos, a partir del desarrollo de mínimos cuadrados recursivos de la sección 3.3, que la disponibilidad de la medición $y_k$ modifica la estimación de una constante $x$ de la siguiente manera:

$$
\begin{aligned}
K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
&= P_kH_k^TR_k^{-1} \\
\hat{x}_k &= \hat{x}_{k-1}+K_k(y_k-H_k\hat{x}_{k-1}) \\
P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T+K_kR_kK_k^T \\
&= (P_{k-1}^{-1}+H_k^TR_k^{-1}H_k)^{-1} \\
&= (I-K_kH_k)P_{k-1}
\end{aligned}
\tag{5.15}
$$

donde $\hat{x}_{k-1}$ y $P_{k-1}$ son la estimación y su covarianza antes de procesar la medición $y_k$, y $\hat{x}_k$ y $P_k$ son la estimación y su covarianza después de procesar la medición $y_k$. En este capítulo, $\hat{x}_k^-$ y $P_k^-$ son la estimación y su covarianza antes de procesar la medición $y_k$, y $\hat{x}_k^+$ y $P_k^+$ son la estimación y su covarianza después de procesar la medición $y_k$. Estas relaciones se muestran en la tabla 5.1.¹

*¹ Necesitamos usar los superíndices menos y más en $\hat{x}_k$ y $P_k$ para distinguir entre las cantidades antes de tener en cuenta $y_k$, y las cantidades después de tener en cuenta $y_k$. En el capítulo 3 no necesitábamos superíndices porque $x$ era una constante.*

**Tabla 5.1** Relaciones entre las estimaciones y las covarianzas de las secciones 3.3 y 5.1

| Sección 3.3 — Estimación por mínimos cuadrados |  | Sección 5.1 — Filtrado de Kalman |
|---|:---:|---|
| $\hat{x}_{k-1}$ = estimación antes de procesar $y_k$ | $\Longrightarrow$ | $\hat{x}_k^-$ = estimación *a priori* |
| $P_{k-1}$ = covarianza antes de procesar $y_k$       | $\Longrightarrow$ | $P_k^-$ = covarianza *a priori* |
| $\hat{x}_k$ = estimación después de procesar $y_k$   | $\Longrightarrow$ | $\hat{x}_k^+$ = estimación *a posteriori* |
| $P_k$ = covarianza después de procesar $y_k$         | $\Longrightarrow$ | $P_k^+$ = covarianza *a posteriori* |

Ahora podemos generalizar las fórmulas para la estimación de una constante de la sección 3.3, a las ecuaciones de actualización de la medición que se necesitan en esta sección. En la ecuación (3.15) —así en el original; por el desarrollo que sigue, la referencia correspondería más bien a la ecuación (3.47)— reemplazamos $\hat{x}_{k-1}$ por $\hat{x}_k^-$, reemplazamos $P_{k-1}$ por $P_k^-$, reemplazamos $\hat{x}_k$ por $\hat{x}_k^+$, y reemplazamos $P_k$ por $P_k^+$. Esto da como resultado

$$
\begin{aligned}
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1} \\
&= P_k^+H_k^TR_k^{-1} \\
\hat{x}_k^+ &= \hat{x}_k^-+K_k(y_k-H_k\hat{x}_k^-) \\
P_k^+ &= (I-K_kH_k)P_k^-(I-K_kH_k)^T+K_kR_kK_k^T \\
&= \left[(P_k^-)^{-1}+H_k^TR_k^{-1}H_k\right]^{-1} \\
&= (I-K_kH_k)P_k^-
\end{aligned}
\tag{5.16}
$$

Estas son las ecuaciones de actualización de la medición para $\hat{x}_k$ y $P_k$. La matriz $K_k$ en las ecuaciones anteriores se llama la **ganancia del filtro de Kalman**.

**El filtro de Kalman de tiempo discreto**

Aquí resumimos el filtro de Kalman de tiempo discreto combinando las ecuaciones anteriores en un único algoritmo.

1. El sistema dinámico está dado por las siguientes ecuaciones:

$$
\begin{aligned}
x_k &= F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+w_{k-1} \\
y_k &= H_kx_k+v_k \\
E(w_kw_j^T) &= Q_k\delta_{k-j} \\
E(v_kv_j^T) &= R_k\delta_{k-j} \\
E(w_kv_j^T) &= 0
\end{aligned}
\tag{5.17}
$$

2. El filtro de Kalman se inicializa de la siguiente manera:

$$
\begin{aligned}
\hat{x}_0^+ &= E(x_0) \\
P_0^+ &= E[(x_0-\hat{x}_0^+)(x_0-\hat{x}_0^+)^T]
\end{aligned}
\tag{5.18}
$$

3. El filtro de Kalman está dado por las siguientes ecuaciones, que se calculan en cada instante $k=1,2,\cdots$:

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1} \\
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1} \\
&= P_k^+H_k^TR_k^{-1} \\[4pt]
\hat{x}_k^- &= F_{k-1}\hat{x}_{k-1}^++G_{k-1}u_{k-1} = \text{estimación de estado } a\ priori \\[4pt]
\hat{x}_k^+ &= \hat{x}_k^-+K_k(y_k-H_k\hat{x}_k^-) = \text{estimación de estado } a\ posteriori \\[4pt]
P_k^+ &= (I-K_kH_k)P_k^-(I-K_kH_k)^T+K_kR_kK_k^T \\
&= \left[(P_k^-)^{-1}+H_k^TR_k^{-1}H_k\right]^{-1} \\
&= (I-K_kH_k)P_k^-
\end{aligned}
\tag{5.19}
$$

La primera expresión anterior para $P_k^+$ se llama la **versión estabilizada de Joseph** de la ecuación de actualización de la medición para la covarianza. Fue formulada por Peter Joseph en la década de 1960, y puede demostrarse que es más estable y robusta que la tercera expresión para $P_k^+$ [Buc68, Cra04] (véase el problema 5.2). La primera expresión para $P_k^+$ garantiza que $P_k^+$ siempre será simétrica y definida positiva, siempre que $P_k^-$ sea simétrica y definida positiva. La tercera expresión para $P_k^+$ es computacionalmente más simple que la primera, pero su forma no garantiza la simetría ni la definición positiva de $P_k^+$. La segunda forma para $P_k^+$ rara vez se implementa tal como se escribió arriba, pero será útil en nuestra deducción del filtro de información en la sección 6.2.

Si se usa la segunda expresión para $K_k$, entonces debe usarse la segunda expresión para $P_k^+$. Esto se debe a que la segunda expresión para $K_k$ depende de $P_k^+$, de modo que necesitamos usar una expresión para $P_k^+$ que no dependa de $K_k$.

Notemos que si $x_k$ es una constante, entonces $F_k=I$, $Q_k=0$, y $u_k=0$. En este caso, el filtro de Kalman de la ecuación (5.19) se reduce al algoritmo recursivo de mínimos cuadrados para la estimación de un vector constante, tal como se presenta en la ecuación (3.47).

Por último, mencionamos un aspecto práctico más del filtro de Kalman, que es importante. Vemos, a partir de la ecuación (5.19), que el cálculo de $P_k^-$, $K_k$ y $P_k^+$ no depende de las mediciones $y_k$, sino únicamente de los parámetros del sistema $F_k$, $H_k$, $Q_k$ y $R_k$. Esto significa que la ganancia de Kalman $K_k$ puede calcularse fuera de línea, antes de que el sistema opere, y guardarse en memoria. Luego, cuando llega el momento de operar el sistema en tiempo real, solo es necesario implementar en tiempo real las ecuaciones de $\hat{x}_k$. El esfuerzo computacional de calcular $K_k$ puede ahorrarse durante la operación en tiempo real, precalculándolo. Si el filtro de Kalman se implementa en un sistema embebido con requisitos computacionales estrictos, esto puede marcar la diferencia entre que el sistema pueda implementarse en tiempo real o no. Además, el desempeño del filtro puede investigarse y evaluarse antes de que el filtro se ejecute realmente. Esto se debe a que $P_k$ indica la exactitud de estimación del filtro, y puede calcularse fuera de línea, ya que no depende de las mediciones. En cambio, como veremos en el capítulo 13, la ganancia y la covarianza del filtro para sistemas no lineales no pueden, en general, calcularse fuera de línea, porque dependen de las mediciones.

---

## 5.2 PROPIEDADES DEL FILTRO DE KALMAN

En esta sección resumimos algunas de las propiedades interesantes e importantes del filtro de Kalman. Supongamos que se nos da el sistema lineal de la ecuación (5.17) y queremos hallar un filtro causal que produzca una estimación de estado $\hat{x}_k$. El error entre el estado verdadero y el estado estimado se denota como $\tilde{x}_k$:

$$
\tilde{x}_k = x_k - \hat{x}_k
\tag{5.20}
$$

Dado que el estado está determinado en parte por el proceso estocástico $\{w_k\}$, $x_k$ es una variable aleatoria. Dado que la estimación del estado está determinada por la secuencia de mediciones $\{y_k\}$, que a su vez está determinada en parte por el proceso estocástico $\{v_k\}$, $\hat{x}_k$ es una variable aleatoria. Por lo tanto, $\tilde{x}_k$ también es una variable aleatoria.

Supongamos que queremos hallar el estimador que minimiza (en cada instante) una norma dos ponderada del valor esperado del error de estimación $\tilde{x}_k$:

$$
\min E\left[\tilde{x}_k^TS_k\tilde{x}_k\right]
\tag{5.21}
$$

donde $S_k$ es una matriz de ponderación definida por el usuario, definida positiva. Si $S_k$ es diagonal, con elementos $S_k(1),\cdots,S_k(n)$, entonces la suma ponderada es igual a $S_k(1)E[\tilde{x}_k^2(1)]+\cdots+S_k(n)E[\tilde{x}_k^2(n)]$.

- Si $\{w_k\}$ y $\{v_k\}$ son gaussianos, de media cero, no correlacionados y blancos, entonces el filtro de Kalman es la solución al problema anterior.

- Si $\{w_k\}$ y $\{v_k\}$ son de media cero, no correlacionados y blancos, entonces el filtro de Kalman es la mejor solución **lineal** al problema anterior. Es decir, el filtro de Kalman es el mejor filtro que sea una combinación lineal de las mediciones. Podría existir un filtro no lineal que dé una mejor solución, pero el filtro de Kalman es el mejor filtro lineal. En libros y artículos suele afirmarse que el filtro de Kalman no es óptimo a menos que el ruido sea gaussiano. Sin embargo, como ha mostrado nuestra deducción en este capítulo, eso simplemente no es cierto. Tales afirmaciones surgen de interpretaciones erróneas de las deducciones del filtro de Kalman. Incluso si el ruido no es gaussiano, el filtro de Kalman sigue siendo el filtro lineal óptimo.

- Si $\{w_k\}$ y $\{v_k\}$ están correlacionados o coloreados, entonces el filtro de Kalman puede modificarse para resolver el problema anterior. Esto se mostrará en el capítulo 7.

- Para sistemas no lineales, diversas formulaciones de filtros de Kalman no lineales aproximan la solución al problema anterior. Esto se analizará más a fondo en los capítulos 13 a 15.

Recordemos la ecuación de actualización de la medición de la ecuación (5.19):

$$
\hat{x}_k^+ = \hat{x}_k^- + K_k(y_k-H_k\hat{x}_k^-)
\tag{5.22}
$$

A la cantidad $(y_k-H_k\hat{x}_k^-)$ se la llama la **innovación**. Esta es la parte de la medición que contiene información nueva sobre el estado. En la sección 10.1 demostraremos que la innovación tiene media cero y es blanca, con covarianza $(H_kP_k^-H_k^T+R_k)$. De hecho, el filtro de Kalman puede deducirse, en realidad, como un filtro que blanquea la medición y que, por lo tanto, extrae la máxima cantidad posible de información de la medición. Esto se propuso por primera vez en [Kai68]. Cuando se usa un filtro de Kalman para la estimación de estado, la innovación puede medirse, y su media y su covarianza pueden aproximarse mediante métodos estadísticos. Si la media y la covarianza de la innovación no son las esperadas, eso significa que algo anda mal con el filtro. Tal vez el modelo del sistema supuesto sea incorrecto, o las estadísticas de ruido supuestas sean incorrectas. Esto puede usarse en tiempo real para verificar el desempeño y los parámetros del filtro de Kalman, e incluso para ajustar los parámetros del filtro de Kalman con el fin de mejorar su desempeño. En la sección 10.2 se explorará una aplicación de esta idea.

---

## 5.3 ECUACIONES DE UN PASO DEL FILTRO DE KALMAN

En esta sección veremos cómo las ecuaciones *a priori* y *a posteriori* del filtro de Kalman pueden combinarse en una única ecuación. Esto puede simplificar la implementación computacional de las ecuaciones. Partimos de la expresión de la estimación de estado *a priori* de la ecuación (5.19), con el índice temporal incrementado en uno:

$$
\hat{x}_{k+1}^- = F_k\hat{x}_k^++G_ku_k
\tag{5.23}
$$

Ahora tomemos la expresión *a posteriori* para $\hat{x}_k^+$ de la ecuación (5.19), y sustituyámosla en la ecuación anterior para obtener

$$
\begin{aligned}
\hat{x}_{k+1}^- &= F_k\left[\hat{x}_k^-+K_k(y_k-H_k\hat{x}_k^-)\right]+G_ku_k \\
&= F_k(I-K_kH_k)\hat{x}_k^-+F_kK_ky_k+G_ku_k
\end{aligned}
\tag{5.24}
$$

Esto muestra que la estimación de estado *a priori* puede calcularse directamente a partir de su valor en el instante anterior, sin calcular en el medio la estimación de estado *a posteriori*. Puede seguirse un procedimiento similar para obtener una expresión de un paso para la covarianza *a priori*. Partimos de la expresión de la covarianza *a priori* de la ecuación (5.19), con el índice temporal incrementado en uno:

$$
P_{k+1}^- = F_kP_k^+F_k^T+Q_k
\tag{5.25}
$$

Ahora tomemos la expresión para $P_k^+$ de la ecuación (5.19), y sustituyámosla en la ecuación anterior para obtener

$$
\begin{aligned}
P_{k+1}^- &= F_k(P_k^--K_kH_kP_k^-)F_k^T+Q_k \\
&= F_kP_k^-F_k^T-F_kK_kH_kP_k^-F_k^T+Q_k \\
&= F_kP_k^-F_k^T-F_kP_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1}H_kP_k^-F_k^T+Q_k
\end{aligned}
\tag{5.26}
$$

Esta ecuación, llamada **ecuación de Riccati discreta**, muestra cómo puede calcularse $P_{k+1}^-$ a partir de $P_k^-$, sin un cálculo intermedio de $P_k^+$.

Pueden realizarse manipulaciones similares para obtener expresiones de un paso para la estimación de estado y la covarianza *a posteriori*. Esto da como resultado

$$
\begin{aligned}
\hat{x}_k^+ &= (I-K_kH_k)(F_{k-1}\hat{x}_{k-1}^++G_{k-1}u_{k-1})+K_ky_k \\
P_k^+ &= (I-K_kH_k)(F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1})
\end{aligned}
\tag{5.27}
$$

Podría imaginarse muchas maneras distintas de combinar las dos expresiones para $K_k$ y las tres expresiones para $P_k^+$ de la ecuación (5.19). Esto daría como resultado varias expresiones distintas para las actualizaciones de un paso de la covarianza *a priori* y *a posteriori*.

\vspace{\baselineskip}
**EJEMPLO 5.1**

Supongamos que tenemos un sistema newtoniano² sin ruido, con posición $r$, velocidad $v$, y aceleración constante $a$.

*² El sistema descrito en este ejemplo se llama newtoniano porque tiene sus raíces en el trabajo matemático de Isaac Newton. Es decir, la velocidad es la derivada de la posición, y la aceleración es la derivada de la velocidad.*

El sistema puede describirse como

$$
\begin{aligned}
\begin{bmatrix} \dot{r} \\ \dot{v} \\ \dot{a} \end{bmatrix} &= \begin{bmatrix} 0&1&0 \\ 0&0&1 \\ 0&0&0 \end{bmatrix} \begin{bmatrix} r \\ v \\ a \end{bmatrix} \\
\dot{x} &= Ax
\end{aligned}
\tag{5.28}
$$

La versión discretizada de este sistema (con un tiempo de muestreo $T$) puede escribirse como

$$
x_{k+1} = Fx_k
\tag{5.29}
$$

donde $F$ está dada por

$$
\begin{aligned}
F &= \exp(AT) \\
&= I+AT+\frac{(AT)^2}{2!}+\cdots \\
&= \begin{bmatrix} 1&T&T^2/2 \\ 0&1&T \\ 0&0&1 \end{bmatrix}
\end{aligned}
\tag{5.30}
$$

El filtro de Kalman para este sistema es

$$
\begin{aligned}
\hat{x}_k^- &= F\hat{x}_{k-1}^+ \\
P_k^- &= FP_{k-1}^+F^T + \underbrace{Q_{k-1}}_{0} \\
&= FP_{k-1}^+F^T
\end{aligned}
\tag{5.31}
$$

Vemos que la covarianza del error de estimación aumenta entre el instante $(k-1)^+$ [es decir, el instante $(k-1)$ después de procesar la medición en ese instante], y el instante $k^-$ (es decir, el instante $k$ antes de procesar la medición en ese instante). Dado que no obtenemos ninguna medición entre el instante $(k-1)^+$ y el instante $k^-$, tiene sentido que aumente nuestra incertidumbre de estimación. Supongamos ahora que medimos la posición con una varianza de $\sigma^2$:

$$
\begin{aligned}
y_k &= H_kx_k+v_k \\
&= \begin{bmatrix} 1&0&0 \end{bmatrix} x_k+v_k \\
v_k &\sim (0,R_k) \\
R_k &= \sigma^2
\end{aligned}
\tag{5.32}
$$

La ganancia de Kalman puede obtenerse a partir de la ecuación (5.19) como

$$
K_k = P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1}
\tag{5.33}
$$

Si escribimos la matriz $P_k^-$ de $3\times3$ en términos de sus elementos individuales, y sustituimos $H_k$ y $R_k$ en la ecuación anterior, obtenemos

$$
K_k = \begin{bmatrix} P_{k,11}^- \\ P_{k,12}^- \\ P_{k,13}^- \end{bmatrix} \frac{1}{P_{k,11}^-+\sigma^2}
\tag{5.34}
$$

La covarianza *a posteriori* puede obtenerse a partir de la ecuación (5.19) como

$$
P_k^+ = P_k^- - K_kH_kP_k^-
\tag{5.35}
$$

Si escribimos la matriz $P_k^-$ de $3\times3$ en términos de sus elementos individuales, y sustituimos $H_k$ y $K_k$ en la ecuación anterior, obtenemos

$$
\begin{aligned}
P_k^+ &= P_k^- - \frac{1}{P_{k,11}^-+\sigma^2}\begin{bmatrix} P_{k,11}^- & 0 & 0 \\ P_{k,12}^- & 0 & 0 \\ P_{k,13}^- & 0 & 0 \end{bmatrix}P_k^- \\[6pt]
&= P_k^- - \frac{1}{P_{k,11}^-+\sigma^2}\begin{bmatrix} (P_{k,11}^-)^2 & P_{k,11}^-P_{k,21}^- & P_{k,11}^-P_{k,31}^- \\ P_{k,12}^-P_{k,11}^- & (P_{k,12}^-)^2 & P_{k,12}^-P_{k,31}^- \\ P_{k,13}^-P_{k,11}^- & P_{k,13}^-P_{k,12}^- & (P_{k,13}^-)^2 \end{bmatrix}
\end{aligned}
\tag{5.36}
$$

Usaremos esta expresión para mostrar que, del instante $k^-$ al instante $k^+$, la traza de la covarianza del error de estimación disminuye. Para verlo, notemos primero que la traza de $P_k^-$ está dada por

$$
\mathrm{Tr}(P_k^-) = P_{k,11}^-+P_{k,22}^-+P_{k,33}^-
\tag{5.37}
$$

A partir de la ecuación (5.36), vemos que la traza de $P_k^+$ está dada por

$$
\begin{aligned}
\mathrm{Tr}(P_k^+) &= P_{k,11}^++P_{k,22}^++P_{k,33}^+ \\[4pt]
&= \left(P_{k,11}^- - \frac{(P_{k,11}^-)^2}{P_{k,11}^-+\sigma^2}\right) + \left(P_{k,22}^- - \frac{(P_{k,12}^-)^2}{P_{k,11}^-+\sigma^2}\right) + \\
&\quad \left(P_{k,33}^- - \frac{(P_{k,13}^-)^2}{P_{k,11}^-+\sigma^2}\right) \\[4pt]
&= \mathrm{Tr}(P_k^-) - \frac{(P_{k,11}^-)^2+(P_{k,12}^-)^2+(P_{k,13}^-)^2}{P_{k,11}^-+\sigma^2}
\end{aligned}
\tag{5.38}
$$

Cuando obtenemos una medición nueva, esperamos que nuestra estimación del estado mejore. Es decir, esperamos que la covarianza disminuya, y la ecuación anterior muestra que, en efecto, disminuye. Es decir, la traza de $P_k^+$ es menor que la traza de $P_k^-$.

Este sistema se simuló con cinco unidades de tiempo entre pasos de discretización ($T=5$), y una desviación estándar de la medición de posición de 30 unidades. La figura 5.3 muestra la varianza de la estimación de posición ($P_{k,11}^-$ y $P_{k,11}^+$) para los primeros cinco instantes del filtro de Kalman. Puede observarse que la varianza (incertidumbre) aumenta de un instante al siguiente, pero luego disminuye en cada instante a medida que se procesa la medición.

La figura 5.4 muestra la varianza de la estimación de posición ($P_{k,11}^-$ y $P_{k,11}^+$) para los primeros 60 instantes del filtro de Kalman. Esto muestra que la varianza aumenta entre instantes, y luego disminuye en cada instante. Pero en esta figura también puede verse que la varianza converge a un valor de estado estacionario.

> *[Figura 5.3 — gráfico en forma de dientes de sierra: eje vertical "varianza del error de estimación de posición" (0 a 3500), eje horizontal "instante" (0 a 5); se ven los puntos $P_0^+, P_1^-, P_1^+, P_2^-, P_2^+, P_3^-, P_3^+, P_4^-, P_4^+, P_5^-$, con la varianza subiendo entre mediciones y cayendo al procesar cada medición.]*
>
> **Figura 5.3** Los primeros cinco instantes de las varianzas del error de estimación de posición, *a priori* y *a posteriori*, para el ejemplo 5.1.

> *[Figura 5.4 — el mismo patrón de dientes de sierra que la figura 5.3, pero para los primeros 60 instantes, mostrando la convergencia de la varianza a un valor de estado estacionario.]*
>
> **Figura 5.4** Los primeros 60 instantes de las varianzas del error de estimación de posición, *a priori* y *a posteriori*, para el ejemplo 5.1.

La figura 5.5 muestra el error de la medición de posición (con una desviación estándar de 30) y el error de la estimación de posición *a posteriori*. El error de estimación comienza con una desviación estándar cercana a 30, pero al final de la simulación la desviación estándar es de aproximadamente 11.

> *[Figura 5.5 — dos curvas frente al "instante" (0 a 60): "error de medición" (punteada) y "error de estimación" (continua), eje vertical "posición" (−80 a 80); la curva de error de estimación tiene una amplitud claramente menor que la del error de medición hacia el final de la simulación.]*
>
> **Figura 5.5** El error de la medición de posición y el error de estimación de posición para el ejemplo 5.1.

---

## 5.4 PROPAGACIÓN ALTERNATIVA DE LA COVARIANZA

En esta sección deducimos una ecuación alternativa para la propagación de la covarianza del error de estimación $P$. Esta ecuación alternativa, basada en [Gre01], puede usarse para hallar una ecuación en forma cerrada para un filtro de Kalman escalar.³ También puede usarse para hallar una solución rápida para la covarianza del error de estimación en estado estacionario.

### 5.4.1 Sistemas de múltiples estados

Recordemos, de la ecuación (5.19), las ecuaciones de actualización de la covarianza del error de estimación:

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1} \\
P_k^+ &= P_k^--P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1}H_kP_k^-
\end{aligned}
\tag{5.39}
$$

Si la matriz $P_k^-$ de $n\times n$ puede factorizarse como

$$
P_k^- = A_kB_k^{-1}
\tag{5.40}
$$

donde $A_k$ y $B_k$ son matrices de $n\times n$ que deben determinarse, entonces $P_{k+1}^-$ satisface

$$
P_{k+1}^- = A_{k+1}B_{k+1}^{-1}
\tag{5.41}
$$

donde $A$ y $B$ se propagan de la siguiente manera:

$$
\begin{bmatrix} A_{k+1} \\ B_{k+1} \end{bmatrix} = \begin{bmatrix} (F_k+Q_kF_k^{-T}H_k^TR_k^{-1}H_k) & Q_kF_k^{-T} \\ F_k^{-T}H_k^TR_k^{-1}H_k & F_k^{-T} \end{bmatrix} \begin{bmatrix} A_k \\ B_k \end{bmatrix}
\tag{5.42}
$$

*³ Las ecuaciones dadas en [Gre01] presentan algunos errores tipográficos que se han corregido en esta sección.*

Esto puede verse notando, a partir de la ecuación (5.42), que

$$
\begin{aligned}
B_{k+1}^{-1} &= \left[F_k^{-T}H_k^TR_k^{-1}H_kA_k+F_k^{-T}B_k\right]^{-1} \\
&= \left[F_k^{-T}(H_k^TR_k^{-1}H_kA_kB_k^{-1}+I)B_k\right]^{-1} \\
&= B_k^{-1}\left[H_k^TR_k^{-1}H_kA_kB_k^{-1}+I\right]^{-1}F_k^T
\end{aligned}
\tag{5.43}
$$

A partir de la ecuación (5.42), vemos que

$$
A_{k+1}B_{k+1}^{-1} = \left[(F_k+Q_kF_k^{-T}H_k^TR_k^{-1}H_k)A_k+Q_kF_k^{-T}B_k\right]B_{k+1}^{-1}
\tag{5.44}
$$

Sustituyendo la expresión para $B_{k+1}^{-1}$ en esta ecuación se obtiene

$$
\begin{aligned}
A_{k+1}B_{k+1}^{-1} &= \left[(F_k+Q_kF_k^{-T}H_k^TR_k^{-1}H_k)A_k+Q_kF_k^{-T}B_k\right] \times \\
&\quad B_k^{-1}\left[H_k^TR_k^{-1}H_kA_kB_k^{-1}+I\right]^{-1}F_k^T \\[4pt]
&= \left[(F_k+Q_kF_k^{-T}H_k^TR_k^{-1}H_k)A_kB_k^{-1}+Q_kF_k^{-T}\right] \times \\
&\quad \left[H_k^TR_k^{-1}H_kA_kB_k^{-1}+I\right]^{-1}F_k^T
\end{aligned}
\tag{5.45}
$$

Sustituyendo $P_k^-$ por $A_kB_k^{-1}$ en la ecuación anterior se obtiene

$$
\begin{aligned}
A_{k+1}B_{k+1}^{-1} &= \left[(F_k+Q_kF_k^{-T}H_k^TR_k^{-1}H_k)P_k^-+Q_kF_k^{-T}\right] \times \\
&\quad \left[H_k^TR_k^{-1}H_kP_k^-+I\right]^{-1}F_k^T \\[4pt]
&= \left[F_kP_k^-+Q_kF_k^{-T}(H_k^TR_k^{-1}H_kP_k^-+I)\right] \times \\
&\quad \left[H_k^TR_k^{-1}H_kP_k^-+I\right]^{-1}F_k^T \\[4pt]
&= F_kP_k^-\left[H_k^TR_k^{-1}H_kP_k^-+I\right]^{-1}F_k^T+Q_kF_k^{-T}F_k^T
\end{aligned}
\tag{5.46}
$$

Aplicando el lema de inversión de matrices al término entre corchetes se obtiene

$$
\begin{aligned}
A_{k+1}B_{k+1}^{-1} &= F_kP_k^-\left[I-H_k^T(H_kP_k^-H_k^T+R_k)^{-1}H_kP_k^-\right]F_k^T+Q_k \\
&= F_k\left[P_k^--P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1}H_kP_k^-\right]F_k^T+Q_k \\
&= F_kP_k^+F_k^T+Q_k \\
&= P_{k+1}^-
\end{aligned}
\tag{5.47}
$$

Entonces vemos que $A_{k+1}B_{k+1}^{-1} = P_{k+1}^-$.

La ecuación (5.42) puede usarse para obtener una solución rápida para la covarianza de estado estacionario en sistemas multidimensionales (aunque no una solución en forma cerrada). Supongamos que $F$, $Q$, $H$ y $R$ son matrices constantes. A partir de la ecuación (5.42) obtenemos

$$
\begin{aligned}
\begin{bmatrix} A_{k+1} \\ B_{k+1} \end{bmatrix} &= \begin{bmatrix} (F+QF^{-T}H^TR^{-1}H) & QF^{-T} \\ F^{-T}H^TR^{-1}H & F^{-T} \end{bmatrix} \begin{bmatrix} A_k \\ B_k \end{bmatrix} \\
&= \Psi\begin{bmatrix} A_k \\ B_k \end{bmatrix} \\[6pt]
\begin{bmatrix} A_k \\ B_k \end{bmatrix} &= \Psi^{k-1}\begin{bmatrix} P_1^- \\ I \end{bmatrix}
\end{aligned}
\tag{5.48}
$$

donde usamos el hecho de que $A_1=P_1^-$ y $B_1=I$ satisface la factorización original de la ecuación (5.40). Ahora podemos elevar $\Psi$ al cuadrado sucesivamente, un total de $p$ veces, para obtener $\Psi^2$, $\Psi^4$, $\Psi^8$, y así sucesivamente, hasta que $\Psi^{2^p}$ converja a un valor de estado estacionario:

$$
\begin{bmatrix} A_\infty \\ B_\infty \end{bmatrix} \approx \Psi^{2^p}\begin{bmatrix} P_1^- \\ I \end{bmatrix} \quad \text{para } p \text{ grande}
\tag{5.49}
$$

La covarianza de estado estacionario es $P_\infty^- = A_\infty B_\infty^{-1}$. También podemos hallar la ganancia de Kalman de estado estacionario simplemente iterando las ecuaciones del filtro de la ecuación (5.19), pero el método de esta sección podría ser una manera mucho más rápida de hallar la ganancia de estado estacionario. Una vez que hallamos $P_\infty^-$ como se mostró arriba, calculamos $K_\infty = P_\infty^-H^T(HP_\infty^-H^T+R)^{-1}$ como la ganancia de estado estacionario del filtro de Kalman. En la sección 7.3 se ofrece un análisis más detallado del filtrado de Kalman de estado estacionario.

### 5.4.2 Sistemas escalares

La ecuación (5.42) puede usarse para obtener una solución en forma cerrada para el filtro de Kalman escalar, para sistemas invariantes en el tiempo. Supongamos que $F$, $Q$, $H$ y $R$ son escalares constantes. Entonces, a partir de la ecuación (5.42), obtenemos

$$
\begin{aligned}
\begin{bmatrix} A_{k+1} \\ B_{k+1} \end{bmatrix} &= \begin{bmatrix} F+\dfrac{H^2Q}{FR} & \dfrac{Q}{F} \\[8pt] \dfrac{H^2}{FR} & \dfrac{1}{F} \end{bmatrix} \begin{bmatrix} A_k \\ B_k \end{bmatrix} \\[6pt]
&= \Psi\begin{bmatrix} A_k \\ B_k \end{bmatrix}
\end{aligned}
\tag{5.50}
$$

donde $\Psi$ está definida por la ecuación anterior. Ahora hallemos los valores propios y los vectores propios de $\Psi$. Supongamos que los valores propios de $\Psi$ son $\lambda_1$ y $\lambda_2$, y que los vectores propios de $\Psi$ se combinan para formar la matriz $M$ de $2\times2$. Entonces

$$
\Psi = M\begin{bmatrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{bmatrix}M^{-1}
\tag{5.51}
$$

y obtenemos

$$
\begin{aligned}
\begin{bmatrix} A_k \\ B_k \end{bmatrix} &= \Psi^{k-1}\begin{bmatrix} A_1 \\ B_1 \end{bmatrix} \\[4pt]
&= M\begin{bmatrix} \lambda_1^{k-1} & 0 \\ 0 & \lambda_2^{k-1} \end{bmatrix}M^{-1}\begin{bmatrix} P_1^- \\ 1 \end{bmatrix}
\end{aligned}
\tag{5.52}
$$

donde usamos el hecho de que $A_1=P_1^-$ y $B_1=1$ satisface la factorización original de la ecuación (5.40). Desarrollando la matemática para obtener $\lambda_1$, $\lambda_2$ y $M$ se obtiene lo siguiente.

$$
\begin{aligned}
P_k^- &= \frac{\tau_1\mu_1^{k-1}(2RH^2P_1^--\tau_2)-\tau_2\mu_2^{k-1}(2H^2P_1^--\tau_1)}{2H^2\mu_1^{k-1}(2RH^2P_1^--\tau_2)-2H^2\mu_2^{k-1}(2H^2P_1^--\tau_1)} \\[6pt]
\lambda_1 &= \frac{H^2Q+R(F^2+1)+\sigma}{2FR} \\[4pt]
\lambda_2 &= \frac{H^2Q+R(F^2+1)-\sigma}{2FR} \\[4pt]
\sigma &= \sqrt{H^2Q+R(F+1)^2}\sqrt{H^2Q+R(F-1)^2} \\[4pt]
\tau_1 &= H^2Q+R(F^2-1)+\sigma \\
\tau_2 &= H^2Q+R(F^2-1)-\sigma \\
\mu_1 &= H^2Q+R(F^2+1)+\sigma \\
\mu_2 &= H^2Q+R(F^2+1)-\sigma
\end{aligned}
\tag{5.53}
$$

$$
\begin{aligned}
M &= \begin{bmatrix} \dfrac{\tau_1}{2H^2} & \dfrac{\tau_2}{2H^2} \\[8pt] 1 & 1 \end{bmatrix} \\[10pt]
M^{-1} &= \frac{1}{\tau_1(R-1)+2\sigma}\begin{bmatrix} 2RH^2 & -\tau_1 \\ -2RH^2 & R\tau_1 \end{bmatrix}
\end{aligned}
$$
*(5.53, cont.)*

Esta es una ecuación en forma cerrada para el filtro de Kalman variante en el tiempo, para un sistema escalar invariante en el tiempo. Esto puede usarse fácilmente para obtener el valor de estado estacionario de $P_k^-$. Notemos que $\mu_2<\mu_1$. A medida que $k$ aumenta, $\mu_2^k$ se vuelve cada vez más pequeño en relación con $\mu_1^k$. Por lo tanto,

$$
\begin{aligned}
\lim_{k\to\infty}P_k^- &= \lim_{k\to\infty}\frac{\tau_1\mu_1^{k-1}(2RH^2P_1^--\tau_2)-\tau_2\mu_2^{k-1}(2H^2P_1^--\tau_1)}{2H^2\mu_1^{k-1}(2RH^2P_1^--\tau_2)-2H^2\mu_2^{k-1}(2H^2P_1^--\tau_1)} \\[6pt]
&= \lim_{k\to\infty}\frac{\tau_1\mu_1^{k-1}(2RH^2P_1^--\tau_2)}{2H^2\mu_1^{k-1}(2RH^2P_1^--\tau_2)} \\[6pt]
&= \frac{\tau_1}{2H^2}
\end{aligned}
\tag{5.54}
$$

Esto da la covarianza de estado estacionario para un sistema escalar.

\vspace{\baselineskip}
**EJEMPLO 5.2**

En este ejemplo mostraremos cómo puede propagarse una covarianza escalar. Consideremos el siguiente sistema escalar:

$$
\begin{aligned}
x_{k+1} &= x_k+w_k \\
y_k &= x_k+v_k \\
w_k &\sim (0,1) \\
v_k &\sim (0,1)
\end{aligned}
\tag{5.55}
$$

Este es un sistema muy simple, pero que surge en muchas aplicaciones. Por ejemplo, puede representar algún parámetro $x_k$ que varía lentamente y que medimos directamente. El término de ruido de proceso $w_k$ da cuenta de las variaciones en $x_k$, y el término de ruido de medición $v_k$ da cuenta de los errores de medición. En este sistema tenemos $F=H=Q=R=1$. Sustituyendo estos valores en la ecuación (5.53) se obtiene

$$
\begin{aligned}
\tau_1 &= 1+\sqrt{5} \\
\tau_2 &= 1-\sqrt{5} \\
\mu_1 &= 3+\sqrt{5} \\
\mu_2 &= 3-\sqrt{5} \\[4pt]
P_k^- &= \frac{\tau_1\mu_1^{k-1}(2P_1^--\tau_2)-\tau_2\mu_2^{k-1}(2P_1^--\tau_1)}{2\mu_1^{k-1}(2P_1^--\tau_2)-2\mu_2^{k-1}(2P_1^--\tau_1)}
\end{aligned}
\tag{5.56}
$$

Tomando el límite cuando $k\to\infty$ se obtiene el valor de estado estacionario de $P_k^-$:

$$
\begin{aligned}
P_\infty^- &= \frac{\tau_1}{2} \\
&= \frac{1+\sqrt{5}}{2} \\
&\approx 1.62
\end{aligned}
\tag{5.57}
$$

Ahora podemos usar la ecuación (5.19) para hallar el valor de estado estacionario de $K_k$:

$$
\begin{aligned}
K_k &= \frac{P_k^-}{P_k^-+1} \\[4pt]
K_\infty &= \frac{1+\sqrt{5}}{3+\sqrt{5}} \\
&\approx 0.62
\end{aligned}
\tag{5.58}
$$

La figura 5.6 muestra la covarianza de estimación *a priori* y la ganancia de Kalman en función del tiempo, e ilustra su convergencia a valores de estado estacionario. A partir de la ecuación para la covarianza de estimación *a posteriori*, sabemos que $P_k^+=(I-K_kH_k)P_k^-$. Por lo tanto, para este ejemplo vemos que el valor de estado estacionario de $P_k^+$ está dado por

$$
\begin{aligned}
P_\infty^+ &= \left(1-\frac{1+\sqrt{5}}{3+\sqrt{5}}\right)\frac{1+\sqrt{5}}{2} \\[4pt]
&= \frac{1+\sqrt{5}}{3+\sqrt{5}}
\end{aligned}
\tag{5.59}
$$

> *[Figura 5.6 — dos curvas frente al "tiempo" (0 a 10): "ganancia de Kalman" (continua, comienza cerca de 0.5 y decrece rápidamente hacia $\approx 0.62$... nota: en la figura decrece hacia un valor bajo y estable) y "covarianza de estimación" (discontinua, comienza cerca de 2 y decrece hacia $\approx 1.62$); ambas curvas convergen a valores de estado estacionario.]*
>
> **Figura 5.6** Covarianza de estimación y ganancia de Kalman en función del tiempo, para el ejemplo 5.2. La covarianza y la ganancia convergen a valores de estado estacionario.

---

## 5.5 PROBLEMAS DE DIVERGENCIA

La teoría presentada en este capítulo hace del filtro de Kalman una opción atractiva para la estimación de estado. Pero cuando un filtro de Kalman se implementa en un sistema real, puede no funcionar, aunque la teoría sea correcta. Dos de las causas principales de las fallas del filtrado de Kalman son la aritmética de precisión finita y los errores de modelado [Fit71].

La teoría presentada en este capítulo supone que la aritmética del filtro de Kalman tiene precisión infinita. En los microprocesadores digitales, la aritmética es de precisión finita: solo se usa una determinada cantidad de bits para representar los números en las ecuaciones del filtro de Kalman. Esto puede causar divergencia, o incluso inestabilidad, en la implementación del filtro de Kalman.

La teoría presentada también supone que el modelo del sistema se conoce con precisión. Se supone que las matrices $F$, $Q$, $H$ y $R$ se conocen exactamente, y se supone que las secuencias de ruido $\{w_k\}$ y $\{v_k\}$ son puramente blancas, de media cero, y completamente no correlacionadas. Si se viola alguno de estos supuestos, como siempre ocurre en las implementaciones reales, entonces se violan los supuestos del filtro de Kalman y la teoría puede no funcionar.

Para mejorar el desempeño del filtro frente a estas realidades, quien lo diseña puede usar varias estrategias:

1. Aumentar la precisión aritmética.

2. Usar alguna forma de filtrado de raíz cuadrada.

3. Simetrizar $P$ en cada instante: $P = (P+P^T)/2$.

4. Inicializar $P$ de manera adecuada, para evitar grandes cambios en $P$.

5. Usar un filtro de memoria decreciente.

6. Usar ruido de proceso ficticio (especialmente para estimar "constantes").

Estas estrategias suelen depender del problema, y es necesario explorarlas mediante simulación o experimentación para obtener buenos resultados. Algunas de estas estrategias pueden ser más atractivas que otras, según el problema específico.

El punto 1 anterior, aumentar la precisión aritmética, simplemente obliga a que la implementación digital del filtro se ajuste más de cerca a la teoría analógica. En una implementación basada en una PC, puede requerir solo un esfuerzo trivial aumentar la precisión aritmética: cambiar todas las variables a doble precisión. Este cambio trivial puede marcar la diferencia entre la divergencia y la convergencia. Sin embargo, en una implementación con microcontrolador, puede no ser viable aumentar la precisión aritmética.

El punto 2 anterior, el filtrado de raíz cuadrada, es una manera de reformular las ecuaciones del filtro. Aunque la precisión física de la implementación no cambia, el filtrado de raíz cuadrada aumenta efectivamente la precisión aritmética. Esto se analizará más a fondo en las secciones 6.3, 6.4 y 8.3. Pero el filtrado de raíz cuadrada requiere más esfuerzo computacional, lo cual puede ser o no una consideración importante para una aplicación dada. El filtrado de raíz cuadrada también agrega mucha complicación a las ecuaciones del filtro, lo cual favorece la aparición de errores de software.

Los puntos 3 y 4 anteriores implican forzar la simetría de $P$ e inicializar $P$ de manera adecuada. Estas son soluciones sencillas, pero por lo general no producen mejoras importantes en las propiedades de convergencia del filtro. Sin embargo, estos pasos siempre deberían implementarse, ya que son directos y sencillos, y ya que pueden prevenir problemas numéricos. Notemos, a partir de la ecuación (5.19), que la expresión para $P_k^-$ ya es simétrica, de modo que no tiene sentido forzar la simetría de $P_k^-$. Sin embargo, según qué ecuación se use, $P_k^+$ puede ser simétrica o no. Las
expresiones para $P_k^+$ de la ecuación (5.19) son matemáticamente equivalentes, pero no son numéricamente equivalentes. Una de ellas tiene la simetría incorporada, pero las otras no. Si se usa una ecuación para $P_k^+$ que no tiene la simetría incorporada, entonces resulta muy fácil, y puede rendir grandes beneficios, forzar la simetría. Esto se ha hecho de varias maneras distintas en la bibliografía. Una manera es la descrita en el punto 3 anterior; es decir, una vez calculada $P$, fijar $P=(P+P^T)/2$. Otras maneras implican forzar que los términos debajo de la diagonal sean iguales a los términos arriba de la diagonal, o forzar que los valores propios de $P$ sean positivos.

El punto 5 anterior es una manera sencilla de forzar al filtro a "olvidar" las mediciones del pasado distante, y a poner más énfasis en las mediciones recientes. Esto hace que el filtro sea más sensible a las mediciones. En teoría, esto produce una pérdida de optimalidad del filtro de Kalman, pero puede restablecer la convergencia y la estabilidad. Es mejor tener un filtro teóricamente subóptimo que funcione, que un filtro teóricamente óptimo que no funcione debido a errores de modelado. La mayor sensibilidad del filtro de memoria decreciente a las mediciones recientes hace que el filtro sea menos sensible a los errores de modelado, y por lo tanto más robusto. Este enfoque se analizará más a fondo en la sección 7.4.

El punto 6 anterior, el uso de ruido de proceso ficticio, también es fácil de implementar. De hecho, puede implementarse de una manera que es matemáticamente equivalente al filtro de memoria decreciente del punto 5. Agregar ruido de proceso ficticio es una manera de indicarle al filtro que se tiene menos confianza en el modelo del sistema. Esto hace que el filtro ponga más énfasis en las mediciones, y menos énfasis en el modelo del proceso (que puede ser incorrecto) [Jaz69].

\vspace{\baselineskip}
**EJEMPLO 5.3**

Ilustremos el uso de ruido de proceso ficticio con un ejemplo. Supongamos que estamos tratando de estimar un estado que creemos que es una constante, pero que en realidad es una rampa. En otras palabras, tenemos un error de modelado. Nuestro modelo supuesto (pero incorrecto), sobre el cual basamos el filtro de Kalman, está dado de la siguiente manera:

$$
\begin{aligned}
x_{k+1} &= x_k+w_k \\
y_k &= x_k+v_k \\
w_k &\sim (0,0) \\
v_k &\sim (0,1)
\end{aligned}
\tag{5.60}
$$

Se supone que el ruido de proceso es cero, lo cual significa que estamos modelando $x_k$ como una constante. A partir de la ecuación (5.19), deducimos las ecuaciones del filtro de Kalman para este sistema como

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1} \\
&= P_{k-1}^+ \\[6pt]
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1} \\
&= \frac{P_k^-}{P_k^-+1} \\[6pt]
\hat{x}_k^- &= F_{k-1}\hat{x}_{k-1}^+ \\
&= \hat{x}_{k-1}^+ \\[6pt]
\hat{x}_k^+ &= \hat{x}_k^-+K_k(y_k-H_k\hat{x}_k^-) \\
&= \hat{x}_k^-+K_k(y_k-\hat{x}_k^-) \\[6pt]
P_k^+ &= (I-K_kH_k)P_k^-(I-K_kH_k)^T+K_kR_kK_k^T \\
&= (1-K_k)^2P_k^-+K_k^2
\end{aligned}
\tag{5.61}
$$

Supongamos que el sistema verdadero, aunque desconocido para quien diseña el filtro de Kalman, está dado por el siguiente modelo de dos estados:

$$
\begin{aligned}
x_{1,k+1} &= x_{1,k}+x_{2,k} \\
x_{2,k+1} &= x_{2,k} \\
y_k &= x_{1,k}+v_k \\
v_k &\sim (0,1)
\end{aligned}
\tag{5.62}
$$

El primer estado es una rampa, que en nuestro modelo del sistema supusimos incorrectamente que era una constante. La figura 5.7 muestra el estado verdadero $x_{1,k}$ y el estado estimado $\hat{x}_{1,k}$. Puede verse que la estimación se aleja (diverge) del estado verdadero, y que el error de estimación crece sin límite.

> *[Figura 5.7 — dos curvas frente al "tiempo" (0 a 50): "estado verdadero" (continua, línea recta ascendente hasta cerca de 500) y "estado estimado" (discontinua, asciende mucho más lentamente, hasta cerca de 240), mostrando cómo la estimación se atrasa cada vez más respecto del estado verdadero.]*
>
> **Figura 5.7** Divergencia del filtro de Kalman debida a un modelado incorrecto.

Sin embargo, si agregamos ruido de proceso ficticio al filtro de Kalman, entonces el filtro pondrá más énfasis en las mediciones, lo cual mejorará el desempeño del filtro. La figura 5.8 muestra el estado verdadero y el estado estimado cuando se usan distintos valores de $Q$ en el filtro de Kalman. A medida que el ruido de proceso ficticio se vuelve mayor, el error de estimación se vuelve menor. Por supuesto, esto tiene el costo de un desempeño más pobre en caso de que el modelo del sistema supuesto sea, en realidad, correcto. Quien diseña el filtro necesita agregar una cantidad adecuada de ruido de proceso ficticio para equilibrar el desempeño en condiciones nominales con el desempeño en condiciones de modelado incorrecto.

La figura 5.9 muestra la evolución temporal de la ganancia de Kalman $K_k$ para este ejemplo, para distintos valores de $Q$. Como es de esperar, la ganancia $K_k$ converge a un valor de estado estacionario mayor cuando $Q$ es mayor, lo cual hace que el filtro sea más sensible a las mediciones [véase la expresión de $\hat{x}_k^+$ en la ecuación (5.61)]. Esto compensa los errores de modelado. Como se muestra más adelante en la sección 7.4, el filtro de memoria decreciente logra lo mismo de una manera distinta. Notemos también, a partir de la figura 5.9, que la ganancia de Kalman de estado estacionario es de aproximadamente 0.62 cuando $Q=1$. Esto coincide con los resultados del ejemplo 5.2.

> *[Figura 5.8 — "estado verdadero" (continua, recta ascendente hasta 500) junto con cuatro curvas de "$\hat{x}$" estimado para $Q=1$, $Q=0.1$, $Q=0.01$ y $Q=0$, en función del "tiempo" (0 a 50); cuanto mayor es $Q$, más se acerca la curva estimada a la del estado verdadero.]*
>
> **Figura 5.8** Mejora del filtro de Kalman debida al ruido de proceso ficticio.

> *[Figura 5.9 — cuatro curvas de la "ganancia de Kalman" (eje vertical, 0 a 0.7) en función del "tiempo" (0 a 50), para $Q=1$, $Q=0.1$, $Q=0.01$ y $Q=0$; cada curva converge a un valor de estado estacionario distinto, mayor cuanto mayor es $Q$.]*
>
> **Figura 5.9** Ganancia de Kalman para distintos valores de ruido de proceso.

Este ejemplo ilustra el principio general de que el ruido del modelo es bueno, pero solo hasta cierto punto. Si el modelo de un sistema tiene demasiado ruido, entonces resulta difícil estimar su estado. Pero si el modelo de un sistema tiene muy poco ruido, entonces nuestro estimador de estado podría volverse excesivamente susceptible a los errores de modelado.⁴ Al diseñar un modelo para un filtro de Kalman, necesitamos equilibrar nuestra confianza en el modelo (poco ruido produce un seguimiento cercano del modelo; es decir, ancho de banda bajo) con una sana autocrítica (mucho ruido produce sensibilidad del filtro; es decir, ancho de banda alto).



*⁴ El ruido, como la mayoría de las cosas en la vida, es beneficioso en cantidades moderadas. Esto también lo vemos en las respuestas psicológicas humanas al ruido. Demasiado ruido puede volver loco a un ser humano, pero muy poco ruido también podría producir una pérdida de la cordura. El ruido es especialmente beneficioso para los ingenieros de control, quienes no solo perderían la cordura, sino que también perderían el financiamiento de sus investigaciones si no fuera por el ruido [Bar01, pág. 179].*

El examen de las ecuaciones del filtro muestra por qué agregar ruido de proceso ficticio compensa los errores de modelado. Recordemos las ecuaciones del filtro de Kalman de la ecuación (5.19), algunas de las cuales repetimos aquí:

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T+Q_{k-1} \\
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1} \\
\hat{x}_k^+ &= \hat{x}_k^-+K_k(y_k-H_k\hat{x}_k^-)
\end{aligned}
\tag{5.63}
$$

Si $Q_k$ es pequeño, entonces la covarianza puede no aumentar mucho entre instantes de muestreo. En el ejemplo 5.3 teníamos $F_k=1$, de modo que $P_k^-=P_{k-1}^+$ cuando $Q_k=0$. Pero la covarianza disminuirá de $P_k^-$ a $P_k^+$ cada vez que se obtenga una medición, debido a la ecuación de actualización de la medición para la covarianza. Eventualmente, $P_k^-$ convergerá a cero. Esto puede verse observando la ecuación (5.26), que muestra la ecuación de un paso para $P_k^-$:

$$
P_{k+1}^- = F_kP_k^-F_k^T-F_kK_kH_kP_k^-F_k^T+Q_k
\tag{5.64}
$$

Si $Q_k=0$, entonces esta ecuación tiene una solución estacionaria de cero. Un valor de cero para $P_k^-$ dará como resultado $K_k=0$, como se ve en la ecuación (5.63). Un valor de cero para $K_k$ significa que la ecuación de actualización de la medición (5.63) para $\hat{x}$ no tomará en cuenta la medición en absoluto; es decir, la medición $y_k$ se ignorará por completo en el cálculo de $\hat{x}^+$. Esto se debe a que la covarianza del ruido de medición $R_k$ (suponiendo que sea mayor que cero) será infinitamente más grande que el ruido de proceso $Q_k=0$. El filtro se volverá lento (perezoso), en el sentido de que no responderá a las mediciones.

Por otro lado, si $Q_k$ es mayor, entonces la covarianza siempre aumentará entre instantes de muestreo; es decir, $P_k^-$ siempre será mayor que $P_{k-1}^+$. Cuando $P_k$ converja, convergerá a un valor mayor. Esto hará que $K_k$ converja a un valor mayor. Una $K_k$ mayor significa que la actualización de la medición para $\hat{x}$ en la ecuación (5.63) incluirá un mayor énfasis en la medición; es decir, el filtro prestará más atención a las mediciones.

---

## 5.6 RESUMEN

En este capítulo hemos presentado la esencia del filtro de Kalman de tiempo discreto. En las últimas décadas, este algoritmo de estimación ha encontrado aplicaciones en prácticamente todas las áreas de la ingeniería. Hemos visto que las ecuaciones del filtro de Kalman pueden escribirse de varias maneras distintas, cada una de las cuales puede parecer bastante diferente de las demás, aunque todas son matemáticamente equivalentes. Hemos visto que el filtro de Kalman es óptimo incluso cuando el ruido no es gaussiano. El filtro de Kalman es el estimador óptimo cuando el ruido es gaussiano, y es el estimador lineal óptimo cuando el ruido no es gaussiano. Hemos visto que el filtro de Kalman puede no desempeñarse bien si no se cumplen los supuestos subyacentes, y mencionamos brevemente algunas maneras de compensar el incumplimiento de esos supuestos. Los capítulos posteriores de este libro ampliarán y generalizarán los resultados presentados en este capítulo.

---

## PROBLEMAS

### Ejercicios escritos

**5.1** Una masa radiactiva tiene una vida media de $\tau$ segundos. En cada instante, la cantidad de partículas emitidas $x$ es la mitad de lo que era en el instante anterior, pero hay cierto error $w_k$ (de media cero, con varianza $Q$) en la cantidad de partículas emitidas, debido a la radiación de fondo. En cada instante se cuenta la cantidad de partículas emitidas. El instrumento usado para contar la cantidad de partículas emitidas tiene, en el instante $k$, un error aleatorio $v_k$, que es de media cero, con varianza $R$. Suponga que $w_k$ y $v_k$ no están correlacionados.

a) Escriba las ecuaciones del sistema lineal para este sistema.

b) Suponga que queremos usar un filtro de Kalman para hallar la estimación óptima de la cantidad de partículas emitidas en cada instante. Escriba las ecuaciones de un paso *a posteriori* del filtro de Kalman para este sistema.

c) Halle la varianza del error de estimación *a posteriori*, de estado estacionario, para el filtro de Kalman.

d) ¿Cuál es la ganancia de Kalman de estado estacionario cuando $Q=R$? ¿Cuál es la ganancia de Kalman de estado estacionario cuando $Q=2R$? Dé una explicación intuitiva de por qué la ganancia de estado estacionario cambia de la manera en que lo hace cuando cambia la razón entre $Q$ y $R$.

**5.2** Este problema ilustra la robustez que se logra mediante el uso de la forma de Joseph de la ecuación de actualización de la medición para la covarianza. Suponga que tiene un filtro de Kalman de tiempo discreto para un sistema escalar.

a) Halle $\partial P_k^+/\partial K_k$ para la tercera forma de la actualización de la medición para la covarianza en la ecuación (5.19).

b) Halle $\partial P_k^+/\partial K_k$ para la forma de Joseph (la primera forma) de la actualización de la medición para la covarianza en la ecuación (5.19). Después de obtener su respuesta, sustituya $K_k$ por la expresión de la ganancia de Kalman.

c) Use los resultados anteriores para explicar por qué la forma de Joseph de la ecuación de actualización de la medición para la covarianza es estable y robusta.

**5.3** Demuestre que $E[\hat{x}_k^+(\tilde{x}_k^+)^T]=0$. Sugerencia: dado que $\hat{x}_0^+=E[x_0]$ es una constante, y $\tilde{x}_0^+=x_0-\hat{x}_0^+$ es de media cero, sabemos que $E[\hat{x}_0^+(\tilde{x}_0^+)^T]=0$. Con esta información, demuestre que $E[\hat{x}_1^+(\tilde{x}_1^+)^T]=0$. A partir de este punto, use inducción para completar la demostración.

**5.4** Suponga que tiene un acuario con $x_p$ pirañas y $x_g$ guppies [Bay99]. Una vez por semana, coloca alimento para guppies en el acuario (que las pirañas no comen). Cada semana, las pirañas se comen algunos de los guppies. La tasa de natalidad de las pirañas es proporcional a la población de guppies, y la tasa de mortalidad de las pirañas es

*[el material fuente proporcionado termina aquí, a mitad de oración, en la página 145]*
