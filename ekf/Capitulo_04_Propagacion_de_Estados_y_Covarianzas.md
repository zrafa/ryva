# Capítulo 4 — Propagación de estados y covarianzas

> *Nota del traductor: traducción completa y fiel del Capítulo 4, "Propagation of States and Covariances", del libro* Optimal State Estimation *de Dan J. Simon (Wiley, 2006), con el mismo criterio usado en los capítulos anteriores (el Capítulo 3 no fue provisto). Terminología nueva de este capítulo: "sampled-data system" → sistema de datos muestreados; "first-order hold" → retenedor de primer orden; "sifting property" → propiedad de cribado. El original usa dos nombres distintos para ecuaciones del mismo tipo matemático en contextos distintos —"difference Lyapunov equation" (Capítulo 1) y "discrete-time Lyapunov equation" (este capítulo)—; se preserva esa misma distinción de nombres en español ("ecuación de Lyapunov en diferencias" y "ecuación de Lyapunov en tiempo discreto", respectivamente) en lugar de unificarlos. El pie de página 2 afirma que una entrada constante por tramos corresponde a un retenedor de "primer orden"; se tradujo tal cual, sin corregir a "orden cero", por no ser un error interno verificable con certeza dentro del propio texto. La Figura 4.1 no se reproduce gráficamente; se conserva su leyenda traducida.*

---

En este capítulo comenzaremos con nuestra descripción matemática de un sistema dinámico, y luego deduciremos las ecuaciones que gobiernan la propagación de la media y la covarianza del estado. El material presentado en este capítulo es fundamental para el algoritmo de estimación de estado (el filtro de Kalman) que deduciremos en el Capítulo 5.

La Sección 4.1 trata los sistemas en tiempo discreto. La Sección 4.2 trata los sistemas de datos muestreados, que son los tipos de sistemas más comunes que se encuentran en el mundo real. En este tipo de sistema, la dinámica del sistema se describe mediante ecuaciones diferenciales en tiempo continuo, pero las señales de control y de medición son en tiempo discreto (por ejemplo, control basado en una computadora digital y mediciones obtenidas en instantes discretos). La Sección 4.3 trata los sistemas en tiempo continuo.

## 4.1 SISTEMAS EN TIEMPO DISCRETO

Supongamos que tenemos el siguiente sistema lineal en tiempo discreto:

$$x_k = F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+w_{k-1} \tag{4.1}$$

donde $u_k$ es una entrada conocida y $w_k$ es ruido blanco gaussiano de media cero con covarianza $Q_k$. ¿Cómo cambia con el tiempo la media del estado $x_k$? Si tomamos el valor esperado de ambos lados de la Ecuación (4.1) obtenemos

$$
\begin{aligned}
\bar x_k &= E(x_k) \\
&= F_{k-1}\bar x_{k-1}+G_{k-1}u_{k-1}
\end{aligned}
\tag{4.2}
$$

*Optimal State Estimation, primera edición.* Por Dan J. Simon — pág. 107
ISBN 0471708585 © 2006 John Wiley & Sons, Inc.

---

¿Cómo cambia con el tiempo la covarianza de $x_k$? Podemos usar las Ecuaciones (4.1) y (4.2) para obtener

$$
\begin{aligned}
(x_k-\bar x_k)(\cdots)^T &= (F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+w_{k-1}-\bar x_k)(\cdots)^T \\
&= [F_{k-1}(x_{k-1}-\bar x_{k-1})+w_{k-1}][\cdots]^T \\
&= F_{k-1}(x_{k-1}-\bar x_{k-1})(x_{k-1}-\bar x_{k-1})^TF_{k-1}^T+w_{k-1}w_{k-1}^T+ \\
&\quad F_{k-1}(x_{k-1}-\bar x_{k-1})w_{k-1}^T+w_{k-1}(x_{k-1}-\bar x_{k-1})^TF_{k-1}^T
\end{aligned}
\tag{4.3}
$$

Por lo tanto, obtenemos la covarianza de $x_k$ como el valor esperado de la expresión anterior. Dado que $(x_{k-1}-\bar x_{k-1})$ no está correlacionada con $w_{k-1}$, obtenemos

$$
\begin{aligned}
P_k &= E\left[(x_k-\bar x_k)(\cdots)^T\right] \\
&= F_{k-1}P_{k-1}F_{k-1}^T+Q_{k-1}
\end{aligned}
\tag{4.4}
$$

A esto se lo llama una ecuación de Lyapunov en tiempo discreto, o una ecuación de Stein [Ste52]. En el próximo capítulo veremos que las Ecuaciones (4.2) y (4.4) son fundamentales en la deducción del filtro de Kalman.

Resulta interesante considerar las condiciones bajo las cuales la ecuación de Lyapunov en tiempo discreto tiene una solución en estado estacionario. Es decir, supongamos que $F_k=F$ es una constante, y $Q_k=Q$ es una constante. Entonces tenemos el siguiente teorema, cuya demostración puede encontrarse en [Kai00, Apéndice D].

**Teorema 21** *Consideremos la ecuación $P=FPF^T+Q$, donde $F$ y $Q$ son matrices reales. Denotemos con $\lambda_i(F)$ los autovalores de la matriz $F$.*

1. *Existe una solución única $P$ si y solo si $\lambda_i(F)\lambda_j(F)\neq 1$ para todo $i,j$. Esta solución única es simétrica.*

2. *Nótese que la condición anterior incluye el caso en que $F$ es estable, porque si $F$ es estable entonces todos sus autovalores tienen magnitud menor que uno, por lo que $\lambda_i(F)\lambda_j(F)\neq 1$ para todo $i,j$. Por lo tanto, vemos que, si $F$ es estable, la ecuación de Lyapunov en tiempo discreto tiene una solución $P$ que es única y simétrica. En este caso, la solución puede escribirse como*

$$P = \sum_{i=0}^{\infty}F^iQ(F^T)^i \tag{4.5}$$

3. *Si $F$ es estable y $Q$ es (semi)definida positiva, entonces la solución única $P$ es simétrica y (semi)definida positiva.*

4. *Si $F$ es estable, $Q$ es semidefinida positiva, y $(F,Q^{1/2})$ es controlable, entonces $P$ es única, simétrica y definida positiva. Nótese que $Q^{1/2}$, la raíz cuadrada de $Q$, se define aquí como cualquier matriz tal que $Q^{1/2}(Q^{1/2})^T=Q$.*

Ahora observemos la solución del sistema lineal de la Ecuación (4.1):

$$x_k = F_{k,0}x_0+\sum_{i=0}^{k-1}(F_{k,i+1}w_i+F_{k,i+1}G_iu_i) \tag{4.6}$$

La matriz $F_{k,i}$ es la matriz de transición de estado del sistema, y se define como

$$
F_{k,i} = \begin{cases}F_{k-1}F_{k-2}\cdots F_i & k>i \\ I & k=i \\ 0 & k<i\end{cases}
\tag{4.7}
$$

Nótese, a partir de la Ecuación (4.6), que $x_k$ es una combinación lineal de $x_0$, $\{w_i\}$, y $\{u_i\}$. Si la secuencia de entrada $\{u_i\}$ es conocida, entonces es una constante y puede considerarse una secuencia de variables aleatorias gaussianas con covarianza nula. Si $x_0$ y $\{w_i\}$ son desconocidas pero son variables aleatorias gaussianas, entonces $x_k$ en la Ecuación (4.6) es una combinación lineal de variables aleatorias gaussianas. Por lo tanto, $x_k$ es, en sí misma, una variable aleatoria gaussiana (véase el Ejemplo 2.4). Pero calculamos la media y la covarianza de $x_k$ en las Ecuaciones (4.2) y (4.4). Por lo tanto,

$$x_k \sim N(\bar x_k,P_k) \tag{4.8}$$

Esto caracteriza completamente a $x_k$ en un sentido estadístico, ya que una variable aleatoria gaussiana queda completamente caracterizada por su media y su covarianza.

**EJEMPLO 4.1**

Un sistema lineal que describe la población de un depredador $x(1)$ y la de su presa $x(2)$ puede escribirse como

$$
\begin{aligned}
x_{k+1}(1) &= x_k(1)-0{,}8x_k(1)+0{,}4x_k(2)+w_k(1) \\
x_{k+1}(2) &= x_k(2)-0{,}4x_k(1)+u_k+w_k(2)
\end{aligned}
\tag{4.9}
$$

En la primera ecuación, vemos que la población de depredadores provoca su propia disminución debido a la superpoblación, pero que la población de presas provoca un aumento en la población de depredadores. En la segunda ecuación, vemos que la población de presas disminuye debido a la población de depredadores, y aumenta debido a un suministro externo de alimento $u_k$. Las poblaciones también están sujetas a perturbaciones aleatorias (con varianzas respectivas 1 y 2) debidas a factores ambientales. Este sistema puede escribirse en forma de espacio de estados como

$$
\begin{aligned}
x_{k+1} &= \begin{bmatrix}0{,}2&0{,}4\\-0{,}4&1\end{bmatrix}x_k+\begin{bmatrix}0\\1\end{bmatrix}u_k+w_k \\
w_k &\sim (0,Q) \qquad Q=\text{diag}(1,2)
\end{aligned}
\tag{4.10}
$$

Las Ecuaciones (4.2) y (4.4) describen cómo cambian con el tiempo la media y la covarianza de las poblaciones. La Figura 4.1 muestra las dos medias y los dos elementos diagonales de la matriz de covarianza para los primeros pasos de tiempo, cuando $u_k=1$ y las condiciones iniciales se fijan como $\bar x_0=\begin{bmatrix}10&20\end{bmatrix}^T$ y $P_0=\text{diag}(40,40)$. Se observa que la media y la covarianza eventualmente alcanzan valores en estado estacionario, dados por

$$
\begin{aligned}
\bar x &= (I-F)^{-1}Gu \\
&= \begin{bmatrix}2{,}5&5\end{bmatrix}^T \\
P &\approx \begin{bmatrix}2{,}88&3{,}08\\3{,}08&7{,}96\end{bmatrix}
\end{aligned}
\tag{4.11}
$$

El valor en estado estacionario de $P$ también puede hallarse directamente (es decir, sin simulación) usando software de sistemas de control.¹ Nótese que, dado que $F$ es estable en este ejemplo y $Q$ es definida positiva, el Teorema 21 garantiza que $P$ tiene una solución única, definida positiva, en estado estacionario.

**Figura 4.1.** Medias y varianzas del estado para el Ejemplo 4.1. *(Gráfico no reproducido; muestra dos paneles con la evolución en el tiempo —media de la población y varianza de la población— comparando las curvas "presa" y "depredador" a lo largo de 14 pasos de tiempo.)*

*¹Por ejemplo, podemos usar la función DLYAP(F,Q) del Control System Toolbox de MATLAB.*

▽▽▽

En la Ecuación (4.1), mostramos el ruido de proceso ingresando directamente a la dinámica del sistema. Esta es la convención que usamos en este libro. Sin embargo, muchas veces el ruido de proceso primero se multiplica por alguna matriz antes de ingresar a la dinámica del sistema. Es decir,

$$x_k = F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+L_{k-1}\tilde w_{k-1}, \qquad \tilde w_k \sim (0,\tilde Q_k) \tag{4.12}$$

¿Cómo podemos llevar esto a la forma convencional de la Ecuación (4.1)? Nótese que el término más a la derecha de la Ecuación (4.12) tiene una covarianza dada por

$$
\begin{aligned}
E\left[(L_{k-1}\tilde w_{k-1})(L_{k-1}\tilde w_{k-1})^T\right] &= L_{k-1}E(\tilde w_{k-1}\tilde w_{k-1}^T)L_{k-1}^T \\
&= L_{k-1}\tilde Q_{k-1}L_{k-1}^T
\end{aligned}
\tag{4.13}
$$

Por lo tanto, la Ecuación (4.12) es equivalente a la ecuación

$$x_k = F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+w_{k-1}, \qquad w_k \sim (0,L_kQ_kL_k^T) \tag{4.14}$$

Esta idea se ilustra en las Secciones 7.3.1 y 7.3.2. El mismo tipo de transformación puede hacerse con ecuaciones de medición ruidosas. Es decir, la ecuación de medición

$$y_k = H_kx_k+L_k\tilde v_k, \qquad \tilde v_k \sim (0,\tilde R_k) \tag{4.15}$$

es equivalente a la ecuación de medición

$$y_k = H_kx_k+v_k, \qquad v_k \sim (0,L_k\tilde R_kL_k^T) \tag{4.16}$$

## 4.2 SISTEMAS DE DATOS MUESTREADOS

Pasemos ahora a los sistemas de datos muestreados, que son los sistemas que se encuentran con más frecuencia en la práctica. Un sistema de datos muestreados es un sistema cuya dinámica se describe mediante una ecuación diferencial en tiempo continuo, pero cuya entrada solo cambia en instantes discretos de tiempo, porque (por ejemplo) la entrada es generada por una computadora digital. Además, nos interesa estimar el estado únicamente en instantes discretos de tiempo. Nos interesa obtener la media y la covarianza del estado únicamente en instantes discretos de tiempo. La dinámica en tiempo continuo se describe como

$$\dot x = Ax+Bu+w \tag{4.17}$$

A partir del Capítulo 1 sabemos que la solución de $x(t)$ en algún instante arbitrario, digamos $t_k$, está dada por

$$x(t_k) = e^{A(t_k-t_{k-1})}x(t_{k-1})+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}[B(\tau)u(\tau)+w(\tau)]\,d\tau \tag{4.18}$$

Ahora supongamos que $u(t)=u_k$ para $t\in[t_k,t_{k+1}]$; es decir, el control $u(t)$ es constante por tramos.² Si hacemos las definiciones

$$
\begin{aligned}
\Delta t &= t_k-t_{k-1} \\
x_k &= x(t_k) \\
u_k &= u(t_k)
\end{aligned}
\tag{4.19}
$$

entonces la Ecuación (4.18) se convierte en

$$x_k = e^{A\Delta t}x_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}B(\tau)\,d\tau\,u_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}w(\tau)\,d\tau \tag{4.20}$$

Ahora, si definimos $F_k$ y $G_k$ como

$$
\begin{aligned}
F_k &= e^{A\Delta t} \\
G_k &= \int_{t_k}^{t_{k+1}}e^{A(t_{k+1}-\tau)}B(\tau)\,d\tau
\end{aligned}
\tag{4.21}
$$

entonces la Ecuación (4.20) se convierte en

$$x_k = F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}w(\tau)\,d\tau \tag{4.22}$$

$e^{A(t_k-\tau)}$ es la matriz de transición de estado del sistema desde el instante $\tau$ hasta el instante $t_k$. Ahora tomemos la media de la ecuación anterior, recordando que $w(t)$ tiene media cero, para obtener

$$
\begin{aligned}
\bar x_k &= E(x_k) \\
&= F_{k-1}\bar x_{k-1}+G_{k-1}u_{k-1}
\end{aligned}
\tag{4.23}
$$

*²Esto supone que se usa un retenedor de primer orden para las entradas de control. En los sistemas de datos muestreados pueden usarse otros tipos de retenedores, pero en este libro suponemos que se usan retenedores de primer orden.*

Podemos usar las ecuaciones anteriores para obtener la covarianza del estado como

$$
\begin{aligned}
P_k &= E[(x_k-\bar x_k)(x_k-\bar x_k)^T] \\
&= E\left[\left(F_{k-1}x_{k-1}+G_{k-1}u_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}w(\tau)\,d\tau-\bar x_k\right)(\cdots)^T\right] \\
&= F_{k-1}P_{k-1}F_{k-1}^T+E\left[\left(\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}w(\tau)\,d\tau\right)(\cdots)^T\right] \\
&= F_{k-1}P_{k-1}F_{k-1}^T+\int\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}E\left[w(\tau)w^T(\alpha)\right]e^{A^T(t_k-\alpha)}\,d\tau\,d\alpha
\end{aligned}
\tag{4.24}
$$

Ahora, si suponemos que $w(t)$ es ruido blanco en tiempo continuo con covarianza $Q_c(t)$, vemos que

$$E\left[w(\tau)w^T(\alpha)\right] = Q_c(\tau)\delta(\tau-\alpha) \tag{4.25}$$

Esto significa que podemos usar la propiedad de cribado de la función impulso (véase el Problema 4.10) para escribir la Ecuación (4.24) como

$$
\begin{aligned}
P_k &= F_{k-1}P_{k-1}F_{k-1}^T+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}Q_c(\tau)e^{A^T(t_k-\tau)}\,d\tau \\
&= F_{k-1}P_{k-1}F_{k-1}^T+Q_{k-1}
\end{aligned}
\tag{4.26}
$$

donde $Q_{k-1}$ queda definida por la ecuación anterior; es decir,

$$Q_{k-1} = \int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}Q_c(\tau)e^{A^T(t_k-\tau)}\,d\tau \tag{4.27}$$

En general, es difícil calcular $Q_{k-1}$, pero para valores pequeños de $(t_k-t_{k-1})$ obtenemos

$$
\begin{aligned}
e^{A(t_k-\tau)} &\approx I \quad \text{para } \tau\in[t_{k-1},t_k] \\
Q_{k-1} &\approx Q_c(t_k)\Delta t
\end{aligned}
\tag{4.28}
$$

**EJEMPLO 4.2**

Supongamos que tenemos un sistema dinámico de primer orden, en tiempo continuo, dado por la ecuación

$$
\begin{aligned}
\dot x &= fx+w \\
E[w(t)w(t+\tau)] &= q_c\delta(\tau)
\end{aligned}
\tag{4.29}
$$

Las ecuaciones de primer orden pueden usarse para describir muchos procesos físicos simples. Por ejemplo, esta ecuación describe el comportamiento de la corriente que circula por un circuito RL en serie excitado por una tensión aleatoria $w(t)$, donde $f=-R/L$. Supongamos que nos interesa obtener la media y la covarianza del estado $x(t)$ cada $\Delta t$ unidades de tiempo; es decir, $t_k-t_{k-1}=\Delta t$. Para este simple ejemplo escalar, podemos calcular explícitamente $Q_{k-1}$ en la Ecuación (4.27) como

$$
\begin{aligned}
Q_{k-1} &= \int_{t_{k-1}}^{t_k}\exp[f(t_k-\tau)]q_c\exp[f(t_k-\tau)]\,d\tau \\
&= \exp(2ft_k)q_c\int_{t_{k-1}}^{t_k}\exp(-2f\tau)\,d\tau \\
&= \exp(2ft_k)q_c\left[\frac{\exp(-2ft_{k-1})-\exp(-2ft_k)}{2f}\right] \\
&= \frac{q_c}{2f}\left[\exp(2f(t_k-t_{k-1}))-1\right] \\
&= \frac{q_c}{2f}\left[\exp(2f\Delta t)-1\right]
\end{aligned}
\tag{4.30}
$$

Para valores pequeños de $\Delta t$, podemos expandir la ecuación anterior en una serie de Taylor alrededor de $\Delta t=0$, para obtener

$$
\begin{aligned}
Q_{k-1} &= \frac{q_c}{2f}\left[\exp(2f\Delta t)-1\right] \\
&= \frac{q_c}{2f}\left[\left(1+2f\Delta t+\frac{(2f\Delta t)^2}{2!}+\cdots\right)-1\right] \\
&\approx \frac{q_c}{2f}\left[1+2f\Delta t-1\right] \\
&= q_c\Delta t
\end{aligned}
\tag{4.31}
$$

Esto coincide con la Ecuación (4.28), que dice que, para $\Delta t$ pequeño, tenemos $Q_{k-1}\approx q_c\Delta t$. La media muestreada del estado se calcula a partir de la Ecuación (4.23) [notando que la entrada de control en la Ecuación (4.29) es cero] como

$$
\begin{aligned}
\bar x_k &= F_{k-1}\bar x_{k-1}+G_{k-1}u_{k-1} \\
&= \exp[f(t_k-t_{k-1})]\bar x_{k-1}+0 \\
&= \exp(f\Delta t)\bar x_{k-1} \\
&= \exp(kf\Delta t)\bar x_0
\end{aligned}
\tag{4.32}
$$

Vemos que, si $f>0$ (es decir, si el sistema es inestable), entonces la media $\bar x_k$ crecerá sin cota (a menos que $\bar x_0=0$). Sin embargo, si $f<0$ (es decir, si el sistema es estable), entonces la media $\bar x_k$ decaerá a cero, sin importar el valor de $\bar x_0$. La covarianza muestreada del estado se calcula a partir de la Ecuación (4.26) como

$$
\begin{aligned}
P_k &= F_{k-1}P_{k-1}F_{k-1}^T+Q_{k-1} \\
&\approx (1+2f\Delta t)P_{k-1}+q_c\Delta t \\
P_k-P_{k-1} &= (2fP_{k-1}+q_c)\Delta t
\end{aligned}
\tag{4.33}
$$

A partir de la ecuación anterior, podemos ver que $P_k$ alcanza el estado estacionario (es decir, $P_k-P_{k-1}=0$) cuando $P_{k-1}=-q_c/2f$, suponiendo que $f<0$. Por otro lado, si $f\geq 0$, entonces $P_k-P_{k-1}$ siempre será mayor que 0, lo cual significa que $\lim_{k\to\infty}P_k=\infty$.

▽▽▽

## 4.3 SISTEMAS EN TIEMPO CONTINUO

En esta sección analizaremos cómo se propagan la media y la covarianza del estado de un sistema lineal en tiempo continuo. Consideremos el sistema en tiempo continuo

$$\dot x = Ax+Bu+w \tag{4.34}$$

donde $u(t)$ es una entrada de control conocida y $w(t)$ es ruido blanco de media cero con una covarianza de

$$E[w(t)w^T(\tau)] = Q_c\delta(t-\tau) \tag{4.35}$$

Tomando la media de la Ecuación (4.34), podemos obtener la siguiente ecuación para la derivada de la media del estado:

$$\dot{\bar x} = A\bar x+Bu \tag{4.36}$$

Esta ecuación muestra cómo se propaga la media del estado con el tiempo. La ecuación lineal que describe la propagación de la media se parece mucho a la ecuación de estado original, la Ecuación (4.34). También podemos obtener la Ecuación (4.36) usando la ecuación que describe la media de un sistema de datos muestreados, y tomando el límite cuando $\Delta t=t_k-t_{k-1}$ tiende a cero. Tomando la media de la Ecuación (4.18) se obtiene

$$\bar x_k = e^{A\Delta t}\bar x_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}B(\tau)u(\tau)\,d\tau \tag{4.37}$$

La matriz de transición de estado puede escribirse como

$$
\begin{aligned}
F &= e^{A\Delta t} \\
&= I+A\Delta t+\frac{(A\Delta t)^2}{2!}+\cdots
\end{aligned}
\tag{4.38}
$$

Para valores pequeños de $\Delta t$, esto puede aproximarse como

$$F \approx I+A\Delta t \tag{4.39}$$

Con esta sustitución, la Ecuación (4.37) se convierte en

$$\bar x_k = (I+A\Delta t)\bar x_{k-1}+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}B(\tau)u(\tau)\,d\tau \tag{4.40}$$

Restando $\bar x_{k-1}$ de ambos lados y dividiendo por $\Delta t$ se obtiene

$$\frac{\bar x_k-\bar x_{k-1}}{\Delta t} = A\bar x_{k-1}+\frac{1}{\Delta t}\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}B(\tau)u(\tau)\,d\tau \tag{4.41}$$

Tomando algunos límites cuando $\Delta t$ tiende a cero se obtiene lo siguiente:

$$
\begin{aligned}
\lim_{\Delta t\to 0}\frac{\bar x_k-\bar x_{k-1}}{\Delta t} &= \dot{\bar x} \\
\lim_{\Delta t\to 0}e^{A(t_k-\tau)} &= I \quad \text{para } \tau\in[t_{k-1},t_k]
\end{aligned}
\tag{4.42}
$$

Haciendo estas sustituciones en (4.41) se obtiene

$$\dot{\bar x} = A\bar x+Bu \tag{4.43}$$

que es la misma ecuación que dedujimos antes en la Ecuación (4.36), mediante un método más directo. Aunque el argumento de límite que usamos aquí no era necesario porque ya teníamos la ecuación de la media en la Ecuación (4.36), este método nos muestra cómo podemos usar argumentos de límite (en general) para obtener fórmulas en tiempo continuo.

A continuación, usaremos un argumento de límite para deducir la covarianza del estado de un sistema en tiempo continuo. Recordemos la ecuación de la covarianza de un sistema de datos muestreados, a partir de la Ecuación (4.26):

$$P_k = F_{k-1}P_{k-1}F_{k-1}^T+Q_{k-1} \tag{4.44}$$

Para $\Delta t$ pequeño, aproximamos nuevamente $F_{k-1}$ como se muestra en la Ecuación (4.39), y sustituimos en la ecuación anterior para obtener

$$
\begin{aligned}
P_k &\approx (I+A\Delta t)P_{k-1}(I+A\Delta t)^T+Q_{k-1} \\
&= P_{k-1}+AP_{k-1}\Delta t+P_{k-1}A^T\Delta t+AP_{k-1}A^T(\Delta t)^2+Q_{k-1}
\end{aligned}
\tag{4.45}
$$

Restando $P_{k-1}$ de ambos lados y dividiendo por $\Delta t$ se obtiene

$$\frac{P_k-P_{k-1}}{\Delta t} = AP_{k-1}+P_{k-1}A^T+AP_{k-1}A^T\Delta t+\frac{Q_{k-1}}{\Delta t} \tag{4.46}$$

Recordemos, a partir de la Ecuación (4.28), que, para $\Delta t$ pequeño,

$$Q_{k-1} \approx Q_c(t_k)\Delta t \tag{4.47}$$

Esto puede escribirse como

$$\frac{Q_{k-1}}{\Delta t} \approx Q_c(t_k) \tag{4.48}$$

Por lo tanto, tomando el límite de la Ecuación (4.46) cuando $\Delta t$ tiende a cero, se obtiene

$$\dot P = AP+PA^T+Q_c \tag{4.49}$$

Esta ecuación de Lyapunov en tiempo continuo, a veces también llamada una ecuación de Sylvester, nos da la ecuación de cómo se propaga con el tiempo la covarianza del estado de un sistema en tiempo continuo.

Resulta interesante considerar las condiciones bajo las cuales la ecuación de Lyapunov en tiempo continuo tiene una solución en estado estacionario. Es decir, supongamos que $A(t)=A$ es una constante, y $Q_c(t)=Q_c$ es una constante. Entonces tenemos el siguiente teorema, cuya demostración puede encontrarse en [Kai00, Apéndice D].

**Teorema 22** *Consideremos la ecuación $AP+PA^T+Q_c=0$, donde $A$ y $Q_c$ son matrices reales. Denotemos con $\lambda_i(A)$ los autovalores de la matriz $A$.*

1. *Existe una solución única $P$ si y solo si $\lambda_i(A)+\lambda_j(A)\neq 0$ para todo $i,j$. Esta solución única es simétrica.*

2. *Nótese que la condición anterior incluye el caso en que $A$ es estable, porque si $A$ es estable entonces todos sus autovalores tienen parte real menor que 0, por lo que $\lambda_i(A)+\lambda_j(A)\neq 0$ para todo $i,j$. Por lo tanto, vemos que, si $A$ es estable, la ecuación de Lyapunov en tiempo continuo tiene una solución $P$ que es única y simétrica. En este caso, la solución puede escribirse como*

$$P = \int_0^{\infty}e^{A^T\tau}Q_ce^{A\tau}\,d\tau \tag{4.50}$$

3. *Si $A$ es estable y $Q_c$ es (semi)definida positiva, entonces la solución única $P$ es simétrica y (semi)definida positiva.*

4. *Si $A$ es estable, $Q_c$ es semidefinida positiva, y $[A,(Q_c^{1/2})^T]$ es controlable, entonces $P$ es única, simétrica y definida positiva. Nótese que $Q_c^{1/2}$, la raíz cuadrada de $Q_c$, se define aquí como cualquier matriz tal que $Q_c^{1/2}(Q_c^{1/2})^T=Q_c$.*

**EJEMPLO 4.3**

Supongamos que tenemos el sistema dinámico de primer orden, en tiempo continuo, dado por la Ecuación (4.29):

$$
\begin{aligned}
\dot x &= fx+w \\
E[w(t)w(t+\tau)] &= q_c\delta(\tau)
\end{aligned}
\tag{4.51}
$$

donde $w(t)$ es ruido de media cero. La ecuación para la propagación en tiempo continuo de la media del estado se obtiene a partir de la Ecuación (4.36):

$$\dot{\bar x} = f\bar x \tag{4.52}$$

Al resolver esta ecuación para $\bar x(t)$ se obtiene

$$\bar x(t) = \exp(ft)\bar x(0) \tag{4.53}$$

Vemos que la media crecerá sin cota si $f>0$ (es decir, si el sistema es inestable), pero la media tenderá asintóticamente a cero si $f<0$ (es decir, si el sistema es estable). La ecuación para la propagación en tiempo continuo de la covarianza del estado se obtiene a partir de la Ecuación (4.49):

$$\dot P = 2fP+q_c \tag{4.54}$$

Al resolver esta ecuación para $P(t)$ se obtiene

$$P(t) = \left(P(0)+\frac{q_c}{2f}\right)\exp(2ft)-\frac{q_c}{2f} \tag{4.55}$$

Vemos que la covarianza crecerá sin cota si $f>0$ (es decir, si el sistema es inestable), pero la covarianza tenderá asintóticamente a $-q_c/2f$ si $f<0$ (es decir, si el sistema es estable). Comparemos estos resultados con el Ejemplo 4.2.

El valor en estado estacionario de $P$ también puede calcularse usando la Ecuación (4.50). Si sustituimos $A$ por $f$ y $Q_c$ por $q_c$ en la Ecuación (4.50), obtenemos

$$
\begin{aligned}
P &= \int_0^{\infty}e^{2f\tau}q_c\,d\tau \\
&= \frac{q_c}{2f}e^{2f\tau}\bigg|_0^{\infty}
\end{aligned}
\tag{4.56}
$$

La integral converge para $f<0$ (es decir, si el sistema es estable), en cuyo caso $P=-q_c/2f$.

▽▽▽

## 4.4 RESUMEN

En este capítulo dedujimos ecuaciones para la propagación de la media y la covarianza del estado de sistemas lineales. Para los sistemas en tiempo discreto, la media y la covarianza se describen mediante ecuaciones en diferencias. Los sistemas de datos muestreados son sistemas con dinámica en tiempo continuo pero con entradas de control que son constantes entre instantes de muestreo. Si la dinámica de un sistema de datos muestreados no cambia entre instantes de muestreo, entonces la media y la covarianza se describen mediante ecuaciones en diferencias, aunque los factores de esas ecuaciones en diferencias son más complicados que los correspondientes a los sistemas en tiempo discreto. Para los sistemas en tiempo continuo, la media y la covarianza se describen mediante ecuaciones diferenciales. Estos resultados formarán parte de la base para nuestra deducción del filtro de Kalman en el Capítulo 5.

Las ecuaciones de covarianza que estudiamos en este capítulo llevan el nombre de Aleksandr Lyapunov, James Sylvester, y Philip Stein. Lyapunov fue un matemático ruso que vivió entre 1857 y 1918. Hizo importantes contribuciones en las áreas de las ecuaciones diferenciales, la estabilidad de sistemas, y la probabilidad. Sylvester fue un matemático y abogado inglés que vivió entre 1814 y 1897. Trabajó durante un tiempo en los Estados Unidos como profesor en la Universidad de Virginia y en la Universidad Johns Hopkins. Mientras estuvo en Johns Hopkins, fundó el *American Journal of Mathematics*, que fue la primera revista matemática de los Estados Unidos.

## PROBLEMAS

### Ejercicios escritos

**4.1** Demuestre que

$$\frac{d}{dt}(E[x]) = E\left[\frac{dx}{dt}\right]$$

**4.2** Supongamos que un sistema dinámico escalar está dado por $x_{k+1}=fx_k+w_k$, donde $w_k$ es ruido blanco de media cero con varianza $q$. Demuestre que, si la varianza de $x_k$ es $\sigma^2$ para todo $k$, entonces necesariamente $f^2=(\sigma^2-q)/\sigma^2$.

**4.3** Consideremos el sistema

$$
\begin{aligned}
x_k &= \begin{bmatrix}1&1\\0&1/2\end{bmatrix}x_{k-1}+\begin{bmatrix}0\\1\end{bmatrix}w_{k-1} \\
w_k &\sim (0,1)
\end{aligned}
$$

donde $w_k$ es ruido blanco.

a) Halle todos los valores posibles en estado estacionario de la media de $x_k$.

b) Halle todos los valores posibles en estado estacionario de la covarianza de $x_k$.

**4.4** Consideremos el sistema del Ejemplo 1.2.

a) Discretice el sistema para hallar la matriz de transición de estado de un solo paso $F_k$, la matriz de entrada en tiempo discreto $G_k$, y la matriz de transición de estado de múltiples pasos $F_{k,i}$.
