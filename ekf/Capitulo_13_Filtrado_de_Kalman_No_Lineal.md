# Capítulo 13 — Filtrado de Kalman no lineal

> *Nota del traductor: esta es una traducción completa y fiel del Capítulo 13, "Nonlinear Kalman Filtering", del libro* Optimal State Estimation *de Dan J. Simon (Wiley, 2006). Se conserva la numeración original de ecuaciones, figuras y tablas para facilitar la referencia cruzada. La sigla EKF (filtro de Kalman extendido) se mantiene en inglés por ser el uso estándar en la literatura técnica en español. Los decimales se expresan con coma y los miles con punto, según la convención académica habitual en español. Las Figuras 13.1, 13.2 y 13.3 no se reproducen gráficamente; se conserva su leyenda traducida en el lugar correspondiente.*

---

> Pareciera que ningún filtro [no lineal] aproximado en particular es consistentemente mejor que los demás, aunque… cualquier filtro no lineal es mejor que uno estrictamente lineal.
>
> —Lawrence Schwartz y Edwin Stear [Sch68]

Todo lo que hemos analizado hasta aquí se refirió a filtros lineales para sistemas lineales. Lamentablemente, los sistemas lineales no existen. Todos los sistemas son, en última instancia, no lineales. Incluso la simple relación $I = V/R$ de la ley de Ohm es solo una aproximación válida dentro de un rango limitado. Si la tensión aplicada a un resistor supera cierto umbral, la aproximación lineal deja de ser válida. La Figura 13.1 muestra una relación típica entre la corriente que circula por un resistor y la tensión aplicada a sus bornes. Para tensiones de entrada pequeñas, la relación es aproximadamente lineal, pero si la potencia disipada por el resistor supera cierto umbral, la relación se vuelve altamente no lineal. Incluso un dispositivo tan simple como un resistor es solo aproximadamente lineal, y esto únicamente dentro de un rango limitado de operación.

Vemos entonces que los sistemas lineales no existen realmente. Sin embargo, muchos sistemas son lo bastante cercanos a lineales como para que los enfoques de estimación lineal den resultados satisfactorios. Pero esa "cercanía suficiente" solo alcanza hasta cierto punto. Tarde o temprano nos encontramos con un sistema que no se comporta linealmente ni siquiera en un rango pequeño de operación, y nuestros enfoques lineales de estimación dejan de dar buenos resultados. En ese caso, es necesario explorar estimadores no lineales.

*Optimal State Estimation, primera edición.* Por Dan J. Simon — pág. 395
ISBN 0471708585 © 2006 John Wiley & Sons, Inc.

---

**Figura 13.1.** Relación típica entre corriente y tensión para un resistor. La relación es lineal dentro de un rango limitado de operación, pero se vuelve altamente no lineal más allá de ese rango. *(Gráfico no reproducido; ejes originales: "Current" [corriente] y "Voltage" [tensión].)*

El filtrado no lineal puede ser un tema difícil y complejo. Ciertamente no es tan maduro, cohesivo ni bien comprendido como el filtrado lineal. Todavía queda mucho margen para avances y mejoras en las técnicas de estimación no lineal. Sin embargo, algunos métodos de estimación no lineal se han vuelto (o se están volviendo) de uso generalizado. Estas técnicas incluyen extensiones no lineales del filtro de Kalman, el filtrado *unscented* y el filtrado de partículas.

En este capítulo analizaremos algunas extensiones no lineales del filtro de Kalman. El filtro de Kalman que estudiamos antes en este libro se aplica directamente solo a sistemas lineales. Sin embargo, un sistema no lineal puede linealizarse, tal como se explicó en la Sección 1.3, y luego se pueden aplicar técnicas de estimación lineal (como el filtro de Kalman o el filtro $H_\infty$). Este capítulo aborda ese tipo de enfoques para el filtrado de Kalman no lineal.

En la Sección 13.1 analizaremos el filtro de Kalman linealizado. Esto implicará hallar un sistema lineal cuyos estados representen las desviaciones respecto de una trayectoria nominal de un sistema no lineal. Luego podremos usar el filtro de Kalman para estimar esas desviaciones respecto de la trayectoria nominal, y así obtener una estimación de los estados del sistema no lineal. En la Sección 13.2 extenderemos el filtro de Kalman linealizado para estimar directamente los estados de un sistema no lineal. Este filtro, denominado filtro de Kalman extendido (EKF), es sin dudas la técnica de estimación de estado no lineal más utilizada en las últimas décadas. En la Sección 13.3 analizaremos enfoques "de orden superior" para el filtrado de Kalman no lineal. Estos enfoques implican algo más que una linealización directa del sistema no lineal, de ahí la expresión "de orden superior". Entre estos métodos se incluyen el filtrado de Kalman de segundo orden, el filtrado de Kalman iterado, el filtrado de Kalman basado en sumas y el filtrado de Kalman basado en cuadrícula. Estos filtros ofrecen formas de reducir los errores de linealización inherentes al EKF. Por lo general, brindan un desempeño de estimación superior al del EKF, pero a costa de una mayor complejidad y un mayor costo computacional.

*(p. 397 del original)*

La Sección 13.4 trata la estimación de parámetros mediante el filtrado de Kalman. En ocasiones, un ingeniero desea estimar los parámetros de un sistema sin que le interese estimar los estados. Esto se convierte en un problema de identificación de sistemas. Las ecuaciones del sistema son, en general, funciones no lineales de los parámetros del sistema. Por lo general, los parámetros del sistema se consideran constantes, o de variación lenta en el tiempo, y un filtro de Kalman no lineal (o cualquier otro estimador de estado no lineal) puede adaptarse para estimar los parámetros del sistema.

---

## 13.1 El filtro de Kalman linealizado

En esta sección mostraremos cómo linealizar un sistema no lineal y luego usar la teoría del filtrado de Kalman para estimar las desviaciones del estado respecto de un valor de estado nominal. Esto nos permitirá obtener una estimación del estado del sistema no lineal. Derivaremos el filtro de Kalman linealizado desde el punto de vista de tiempo continuo, pero la derivación análoga para sistemas en tiempo discreto o híbridos es directa.

Consideremos el siguiente modelo general de sistema no lineal:

$$
\begin{aligned}
\dot x &= f(x,u,w,t) \\
y &= h(x,v,t) \\
w &\sim (0,Q) \\
v &\sim (0,R)
\end{aligned}
\tag{13.1}
$$

La ecuación del sistema $f(\cdot)$ y la ecuación de medición $h(\cdot)$ son funciones no lineales. Usaremos series de Taylor para expandir estas ecuaciones alrededor de un control nominal $u_0$, un estado nominal $x_0$, una salida nominal $y_0$, y valores nominales de ruido $w_0$ y $v_0$. Estos valores nominales (todos ellos funciones del tiempo) se basan en estimaciones *a priori* de cuál podría ser la trayectoria del sistema. Por ejemplo, si las ecuaciones del sistema representan la dinámica de un avión, el control, el estado y la salida nominales podrían corresponder a la trayectoria de vuelo planificada. La trayectoria de vuelo real diferirá de esta trayectoria nominal debido a errores de modelado, perturbaciones y otros efectos imprevistos. Pero la trayectoria real debería estar cerca de la trayectoria nominal, en cuyo caso la linealización mediante series de Taylor debería ser aproximadamente correcta. La linealización mediante series de Taylor de la Ecuación (13.1) da como resultado

$$
\begin{aligned}
\dot x &\approx f(x_0,u_0,w_0,t) + \left.\frac{\partial f}{\partial x}\right|_0(x-x_0) + \left.\frac{\partial f}{\partial u}\right|_0(u-u_0) + \left.\frac{\partial f}{\partial w}\right|_0(w-w_0) \\
&= f(x_0,u_0,w_0,t) + A\Delta x + B\Delta u + L\Delta w \\
y &\approx h(x_0,v_0,t) + \left.\frac{\partial h}{\partial x}\right|_0(x-x_0) + \left.\frac{\partial h}{\partial v}\right|_0(v-v_0) \\
&= h(x_0,v_0,t) + C\Delta x + M\Delta v
\end{aligned}
\tag{13.2}
$$

Las definiciones de las matrices de derivadas parciales $A$, $B$, $C$, $L$ y $M$ resultan evidentes a partir de las ecuaciones anteriores. El subíndice 0 en las derivadas parciales indica que estas se evalúan en los valores nominales de control, estado, salida y ruido. Las definiciones de las desviaciones $\Delta x$, $\Delta u$, $\Delta w$ y $\Delta v$ también resultan evidentes a partir de las ecuaciones anteriores.

*(p. 398 del original)*

Supongamos que los valores nominales de ruido $w_0(t)$ y $v_0(t)$ son ambos iguales a 0 para todo instante de tiempo. [Si no fueran iguales a 0, deberíamos poder escribirlos como la suma de una parte determinística conocida y una parte de media cero, redefinir las cantidades de ruido, y reescribir la Ecuación (13.1) de modo que los valores nominales de ruido sean iguales a 0. Véase el Problema 13.1]. Dado que $w_0(t)$ y $v_0(t)$ son ambos iguales a 0, vemos que $\Delta w(t) = w(t)$ y $\Delta v(t) = v(t)$. Supongamos además que el control $u(t)$ es perfectamente conocido. En general, esta es una suposición razonable. Después de todo, la entrada de control $u(t)$ está determinada por nuestro sistema de control, por lo que no debería haber incertidumbre en su valor. Esto significa que $u_0(t) = u(t)$ y $\Delta u(t) = 0$. Sin embargo, en la práctica puede haber incertidumbres en las salidas de nuestro sistema de control, ya que estas están conectadas a actuadores que presentan sesgos y ruido. Si este es el caso, podemos expresar el control como $u_0(t) + \Delta u(t)$, donde $u_0(t)$ es conocido y $\Delta u(t)$ es una variable aleatoria de media cero, reescribir las ecuaciones del sistema con una señal de control perfectamente conocida, e incluir $\Delta u(t)$ como parte del ruido de proceso (véase el Problema 13.2). Ahora definimos la trayectoria nominal del sistema como

$$
\begin{aligned}
\dot x_0 &= f(x_0,u_0,w_0,t) \\
y_0 &= h(x_0,v_0,t)
\end{aligned}
\tag{13.3}
$$

Definimos la desviación de la derivada del estado real respecto de la derivada del estado nominal, y la desviación de la medición real respecto de la medición nominal, de la siguiente manera:

$$
\begin{aligned}
\Delta \dot x &= \dot x - \dot x_0 \\
\Delta y &= y - y_0
\end{aligned}
\tag{13.4}
$$

Con estas definiciones, la Ecuación (13.2) se convierte en

$$
\begin{aligned}
\Delta \dot x &= A\Delta x + Lw \\
&= A\Delta x + \tilde w \\
\tilde w &\sim (0,\tilde Q), \quad \tilde Q = LQL^T \\
\Delta y &= C\Delta x + Mv \\
&= C\Delta x + \tilde v \\
\tilde v &\sim (0,\tilde R), \quad \tilde R = MRM^T
\end{aligned}
\tag{13.5}
$$

La ecuación anterior es un sistema lineal con estado $\Delta x$ y medición $\Delta y$, por lo que podemos usar un filtro de Kalman para estimar $\Delta x$. Las entradas del filtro consisten en $\Delta y$, que es la diferencia entre la medición real $y$ y la medición nominal $y_0$. El $\Delta x$ que entrega el filtro de Kalman como salida es una estimación de la diferencia entre el estado real $x$ y el estado nominal $x_0$. Las ecuaciones del filtro de Kalman para el filtro de Kalman linealizado son

$$
\begin{aligned}
\Delta \hat x(0) &= 0 \\
P(0) &= E\left[(\Delta x(0)-\Delta \hat x(0))(\Delta x(0)-\Delta \hat x(0))^T\right] \\
\Delta \dot{\hat x} &= A\Delta \hat x + K(\Delta y - C\Delta \hat x) \\
K &= PC^T\tilde R^{-1} \\
\dot P &= AP+PA^T+\tilde Q-PC^T\tilde R^{-1}CP \\
\hat x &= x_0 + \Delta \hat x
\end{aligned}
\tag{13.6}
$$

*(p. 399 del original)*

En el filtro de Kalman, $P$ es igual a la covarianza del error de estimación. En el filtro de Kalman linealizado, esto ya no es exactamente cierto debido a los errores que se introducen en la linealización de la Ecuación (13.2). Sin embargo, si los errores de linealización son pequeños, $P$ debería ser aproximadamente igual a la covarianza del error de estimación. El filtro de Kalman linealizado puede resumirse de la siguiente manera.

**El filtro de Kalman linealizado en tiempo continuo**

1. Las ecuaciones del sistema están dadas por

$$
\begin{aligned}
\dot x &= f(x,u,w,t) \\
y &= h(x,v,t) \\
w &\sim (0,Q) \\
v &\sim (0,R)
\end{aligned}
\tag{13.7}
$$

La trayectoria nominal se conoce de antemano:

$$
\begin{aligned}
\dot x_0 &= f(x_0,u_0,0,t) \\
y_0 &= h(x_0,0,t)
\end{aligned}
\tag{13.8}
$$

2. Calcule las siguientes matrices de derivadas parciales, evaluadas en los valores de la trayectoria nominal:

$$
\begin{aligned}
A &= \left.\frac{\partial f}{\partial x}\right|_0 \\
L &= \left.\frac{\partial f}{\partial w}\right|_0 \\
C &= \left.\frac{\partial h}{\partial x}\right|_0 \\
M &= \left.\frac{\partial h}{\partial v}\right|_0
\end{aligned}
\tag{13.9}
$$

3. Calcule las siguientes matrices:

$$
\begin{aligned}
\tilde Q &= LQL^T \\
\tilde R &= MRM^T
\end{aligned}
\tag{13.10}
$$

4. Defina $\Delta y$ como la diferencia entre la medición real $y$ y la medición nominal $y_0$:

$$
\Delta y = y - y_0
\tag{13.11}
$$

5. Ejecute las siguientes ecuaciones del filtro de Kalman:

$$
\begin{aligned}
\Delta \hat x(0) &= 0 \\
P(0) &= E\left[(\Delta x(0)-\Delta \hat x(0))(\Delta x(0)-\Delta \hat x(0))^T\right] \\
\Delta \dot{\hat x} &= A\Delta \hat x + K(\Delta y - C\Delta \hat x) \\
K &= PC^T\tilde R^{-1} \\
\dot P &= AP+PA^T+\tilde Q-PC^T\tilde R^{-1}CP
\end{aligned}
\tag{13.12}
$$

*(p. 400 del original)*

6. Estime el estado de la siguiente manera:

$$
\hat x = x_0 + \Delta \hat x
\tag{13.13}
$$

El filtro de Kalman linealizado híbrido y el filtro de Kalman linealizado en tiempo discreto no se presentan aquí, pero si se comprendió el desarrollo anterior, sus derivaciones deberían resultar directas.

---

## 13.2 El filtro de Kalman extendido

La sección anterior obtuvo un filtro de Kalman linealizado para estimar los estados de un sistema no lineal. La derivación se basó en linealizar el sistema no lineal alrededor de una trayectoria de estado nominal. La pregunta que surge es: ¿cómo conocemos la trayectoria de estado nominal? En algunos casos, puede no ser sencillo hallar la trayectoria nominal. Sin embargo, dado que el filtro de Kalman estima el estado del sistema, podemos usar la estimación del filtro de Kalman como trayectoria de estado nominal. Se trata de una especie de método *bootstrap*. Linealizamos el sistema no lineal alrededor de la estimación del filtro de Kalman, y la estimación del filtro de Kalman se basa en el sistema linealizado. Esta es la idea del filtro de Kalman extendido (EKF), que fue propuesto originalmente por Stanley Schmidt para que el filtro de Kalman pudiera aplicarse a problemas no lineales de navegación de naves espaciales [Bel67].

En la Sección 13.2.1 presentaremos el EKF para sistemas en tiempo continuo con mediciones en tiempo continuo. En la Sección 13.2.2 presentaremos el EKF híbrido, que es el EKF para sistemas en tiempo continuo con mediciones en tiempo discreto. En la Sección 13.2.3 presentaremos el EKF para sistemas en tiempo discreto con mediciones en tiempo discreto.

### 13.2.1 El filtro de Kalman extendido en tiempo continuo

Combinando la expresión de $\dot x_0$ de la Ecuación (13.3) con la expresión de $\Delta \dot{\hat x}$ de la Ecuación (13.6), se obtiene

$$
\dot x_0 + \Delta \dot{\hat x} = f(x_0,u_0,w_0,t) + A\Delta \hat x + K[y-y_0-C(\hat x-x_0)]
\tag{13.14}
$$

Ahora elijamos $x_0(t) = \hat x(t)$, de modo que $\Delta \hat x(t) = 0$ y $\Delta \dot{\hat x}(t) = 0$. En otras palabras, nuestra trayectoria de linealización $x_0(t)$ es igual a nuestra estimación del filtro de Kalman linealizado $\hat x(t)$. Entonces, la expresión de la medición nominal en la Ecuación (13.3) se convierte en

$$
\begin{aligned}
y_0 &= h(x_0,v_0,t) \\
&= h(\hat x,v_0,t)
\end{aligned}
\tag{13.15}
$$

y la Ecuación (13.14) se convierte en

$$
\dot{\hat x} = f(\hat x,u,w_0,t) + K[y-h(\hat x,v_0,t)]
\tag{13.16}
$$

Esto es equivalente al filtro de Kalman linealizado, salvo que hemos elegido $x_0 = \hat x$, y hemos reordenado las ecuaciones para obtener $\hat x$ directamente. La ganancia de Kalman $K$ es la misma que la presentada en la Ecuación (13.6). Pero esta formulación toma la medición $y$ directamente como entrada, y entrega la estimación del estado $\hat x$ directamente como salida. A esto suele llamárselo filtro de Kalman-Bucy extendido, porque Richard Bucy colaboró con Rudolph Kalman en la primera publicación del filtro de Kalman en tiempo continuo [Kal61]. El EKF en tiempo continuo puede resumirse de la siguiente manera.

*(p. 401 del original)*

**El filtro de Kalman extendido en tiempo continuo**

1. Las ecuaciones del sistema están dadas por

$$
\begin{aligned}
\dot x &= f(x,u,w,t) \\
y &= h(x,v,t) \\
w &\sim (0,Q) \\
v &\sim (0,R)
\end{aligned}
\tag{13.17}
$$

2. Calcule las siguientes matrices de derivadas parciales, evaluadas en la estimación de estado actual:

$$
\begin{aligned}
A &= \left.\frac{\partial f}{\partial x}\right|_{\hat x} \\
L &= \left.\frac{\partial f}{\partial w}\right|_{\hat x} \\
C &= \left.\frac{\partial h}{\partial x}\right|_{\hat x} \\
M &= \left.\frac{\partial h}{\partial v}\right|_{\hat x}
\end{aligned}
\tag{13.18}
$$

3. Calcule las siguientes matrices:

$$
\begin{aligned}
\tilde Q &= LQL^T \\
\tilde R &= MRM^T
\end{aligned}
\tag{13.19}
$$

4. Ejecute las siguientes ecuaciones del filtro de Kalman:

$$
\begin{aligned}
\hat x(0) &= E[x(0)] \\
P(0) &= E\left[(x(0)-\hat x(0))(x(0)-\hat x(0))^T\right] \\
\dot{\hat x} &= f(\hat x,u,w_0,t) + K[y-h(\hat x,v_0,t)] \\
K &= PC^T\tilde R^{-1} \\
\dot P &= AP+PA^T+\tilde Q-PC^T\tilde R^{-1}CP
\end{aligned}
\tag{13.20}
$$

donde los valores nominales de ruido están dados por $w_0=0$ y $v_0=0$.

\vspace{\baselineskip}
**EJEMPLO 13.1**

En este ejemplo usaremos el EKF en tiempo continuo para estimar el estado de un motor síncrono de imán permanente bifásico. Las ecuaciones del sistema están dadas en el Ejemplo 1.4 y se repiten aquí:

$$
\begin{aligned}
\dot i_a &= \frac{-R}{L}i_a + \frac{\omega\lambda}{L}\sin\theta + \frac{u_a+q_1}{L} \\
\dot i_b &= \frac{-R}{L}i_b - \frac{\omega\lambda}{L}\cos\theta + \frac{u_b+q_2}{L} \\
\dot \omega &= \frac{-3\lambda}{2J}i_a\sin\theta + \frac{3\lambda}{2J}i_b\cos\theta - \frac{F\omega}{J} + q_3 \\
\dot \theta &= \omega
\end{aligned}
\tag{13.21}
$$

*(p. 402 del original)*

donde $i_a$ e $i_b$ son las corrientes en los dos devanados, $\theta$ y $\omega$ son la posición angular y la velocidad angular del rotor, $R$ y $L$ son la resistencia y la inductancia del devanado, $\lambda$ es la constante de flujo, y $F$ es el coeficiente de fricción viscosa. Las entradas de control $u_a$ y $u_b$ consisten en las tensiones aplicadas a los dos devanados, y $J$ es el momento de inercia del eje del motor y la carga. El estado se define como

$$
x = \begin{bmatrix} i_a & i_b & \omega & \theta \end{bmatrix}^T
\tag{13.22}
$$

Los términos $q_i$ son ruido de proceso debido a la incertidumbre en las entradas de control ($q_1$ y $q_2$) y en el torque de carga ($q_3$). La matriz de derivadas parciales $A$ se obtiene como

$$
A = \frac{\partial f}{\partial x} =
\begin{bmatrix}
-R/L & 0 & \lambda s/L & x_3\lambda c/L \\
0 & -R/L & -\lambda c/L & x_3\lambda s/L \\
-3\lambda s/2J & 3\lambda c/2J & -F/J & -3\lambda(x_1c+x_2s)/2J \\
0 & 0 & 1 & 0
\end{bmatrix}
\tag{13.23}
$$

donde hemos usado la notación $s=\sin x_4$ y $c=\cos x_4$. Supongamos que podemos medir las corrientes de los devanados mediante resistores sensores de corriente, de modo que nuestras ecuaciones de medición son

$$
\begin{aligned}
y(1) &= i_a + v(1) \\
y(2) &= i_b + v(2)
\end{aligned}
\tag{13.24}
$$

donde $v(1)$ y $v(2)$ son procesos de ruido blanco independientes, de media cero, con desviaciones estándar iguales a 0,1 amperios. Las entradas de control nominales se fijan en

$$
\begin{aligned}
u_a(t) &= \sin(2\pi t) \\
u_b(t) &= \cos(2\pi t)
\end{aligned}
\tag{13.25}
$$

Las entradas de control reales son iguales a los valores nominales más $q_1$ y $q_2$ (términos de ruido eléctrico), que son procesos de ruido blanco independientes, de media cero, con desviaciones estándar iguales a 0,01 amperios. El ruido debido a las perturbaciones del torque de carga ($q_3$) tiene una desviación estándar de 0,5 rad/s². Las mediciones se obtienen de manera continua. Aunque nuestras mediciones consisten únicamente en las corrientes de los devanados y el sistema es no lineal, podemos usar un EKF en tiempo continuo (implementado en circuitería analógica o en lógica digital muy rápida) para estimar la posición y la velocidad del rotor. Los resultados de la simulación se muestran en la Figura 13.2. Los cuatro estados se estiman bastante bien. En particular, la estimación de la posición del rotor es tan buena que las curvas de posición real y estimada del rotor resultan indistinguibles en la Figura 13.2.

La matriz $P$ cuantifica la incertidumbre en las estimaciones de estado. Si las no linealidades del sistema y de la medición no son demasiado severas, la matriz $P$ debería darnos una idea de cuán precisas son nuestras estimaciones. En este ejemplo, las desviaciones estándar de los errores de estimación de estado se obtuvieron a partir de la simulación y luego se compararon con los elementos diagonales de la matriz $P$ en estado estacionario que resultó del filtro de Kalman. La Tabla 13.1 muestra una comparación de los errores de estimación determinados mediante simulación y

*(p. 403 del original)*

**Figura 13.2.** Resultados de la simulación del filtro de Kalman extendido continuo para el motor síncrono de imán permanente bifásico del Ejemplo 13.1. *(Gráfico no reproducido; muestra cuatro paneles — Corriente A, Corriente B, Velocidad y Posición, todos en función del tiempo — con las curvas "Real" y "Estimada" superpuestas.)*

**Tabla 13.1.** Resultados del Ejemplo 13.1, que muestran los errores de estimación de estado de una desviación estándar, determinados a partir de los resultados de la simulación y a partir de la matriz $P$ del EKF. Estos resultados corresponden a la simulación del motor de imán permanente bifásico. Esta tabla muestra que la matriz $P$ brinda una buena indicación de la magnitud de los errores de estimación de estado del EKF.

| | Simulación | Matriz $P$ |
|---|---|---|
| Corriente del devanado A | 0,054 A | 0,094 A |
| Corriente del devanado B | 0,052 A | 0,094 A |
| Velocidad | 0,26 rad/s | 0,44 rad/s |
| Posición | 0,013 rad | 0,025 rad |

los errores de estimación teóricos basados en la matriz $P$. Vemos que la matriz $P$ da una buena indicación de la magnitud de los errores de estimación.

▽▽▽

### 13.2.2 El filtro de Kalman extendido híbrido

Muchos sistemas de ingeniería reales están gobernados por una dinámica en tiempo continuo, mientras que las mediciones se obtienen en instantes discretos de tiempo. En esta sección derivaremos el EKF híbrido, que considera sistemas con dinámica en tiempo continuo y mediciones en tiempo discreto. Esta es la situación más común que se encuentra en la práctica.

Supongamos que tenemos un sistema en tiempo continuo con mediciones en tiempo discreto, de la siguiente manera:

*(p. 404 del original)*

$$
\begin{aligned}
\dot x &= f(x,u,w,t) \\
y_k &= h_k(x_k,v_k) \\
w(t) &\sim (0,Q) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.26}
$$

El ruido de proceso $w(t)$ es ruido blanco en tiempo continuo con covarianza $Q$, y el ruido de medición $v_k$ es ruido blanco en tiempo discreto con covarianza $R_k$. Entre mediciones, propagamos la estimación del estado de acuerdo con la dinámica no lineal conocida, y propagamos la covarianza tal como se derivó en el EKF en tiempo continuo de la Sección 13.2.1, usando la Ecuación (13.20). Recordemos que la expresión de $\dot P$ de la Ecuación (13.20) está dada por

$$
\dot P = AP+PA^T+LQL^T-PC^T(MRM^T)^{-1}CP
\tag{13.27}
$$

En el EKF híbrido, no deberíamos incluir el término $R$ en la ecuación de $\dot P$, porque estamos integrando $P$ entre instantes de medición, período durante el cual no disponemos de ninguna medición. Otra manera de verlo es que, entre instantes de medición, tenemos mediciones con covarianza infinita ($R=\infty$), por lo que el último término del lado derecho de la ecuación de $\dot P$ tiende a cero. Esto nos da las siguientes ecuaciones de actualización temporal para el EKF híbrido:

$$
\begin{aligned}
\dot{\hat x} &= f(\hat x,u,w_0,t) \\
\dot P &= AP+PA^T+LQL^T
\end{aligned}
\tag{13.28}
$$

donde $A$ y $L$ están dadas en la Ecuación (13.18). Las ecuaciones anteriores propagan $\hat x$ desde $\hat x_{k-1}^+$ hasta $\hat x_k^-$, y $P$ desde $P_{k-1}^+$ hasta $P_k^-$. Nótese que $w_0$ es el ruido de proceso nominal en la ecuación anterior; es decir, $w_0(t)=0$.

En cada instante de medición, actualizamos la estimación del estado y la covarianza tal como se dedujo en el filtro de Kalman en tiempo discreto (Capítulo 5):

$$
\begin{aligned}
K_k &= P_k^- H_k^T\left(H_kP_k^-H_k^T+M_kR_kM_k^T\right)^{-1} \\
\hat x_k^+ &= \hat x_k^- + K_k\left[y_k-h_k(\hat x_k^-,v_0,t_k)\right] \\
P_k^+ &= (I-K_kH_k)P_k^-(I-K_kH_k)^T + K_kM_kR_kM_k^TK_k^T
\end{aligned}
\tag{13.29}
$$

donde $v_0$ es el ruido de medición nominal; es decir, $v_0=0$. $H_k$ es la derivada parcial de $h_k(x_k,v_k)$ respecto de $x_k$, y $M_k$ es la derivada parcial de $h_k(x_k,v_k)$ respecto de $v_k$. $H_k$ y $M_k$ se evalúan en $\hat x_k^-$.

Nótese que $P_k$ y $K_k$ no pueden calcularse fuera de línea (*offline*), porque dependen de $H_k$ y $M_k$, que a su vez dependen de $\hat x_k^-$, el cual depende de las mediciones ruidosas. Por lo tanto, en general, no existe una solución en estado estacionario para el filtro de Kalman extendido. Sin embargo, se han reportado algunos esfuerzos para obtener aproximaciones en estado estacionario del filtro de Kalman extendido en [Saf78].

El EKF híbrido puede resumirse de la siguiente manera.

*(p. 405 del original)*

**El filtro de Kalman extendido híbrido**

1. Las ecuaciones del sistema, con dinámica en tiempo continuo y mediciones en tiempo discreto, están dadas de la siguiente manera:

$$
\begin{aligned}
\dot x &= f(x,u,w,t) \\
y_k &= h_k(x_k,v_k) \\
w(t) &\sim (0,Q) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.30}
$$

2. Inicialice el filtro de la siguiente manera:

$$
\begin{aligned}
\hat x_0^+ &= E[x_0] \\
P_0^+ &= E\left[(x_0-\hat x_0^+)(x_0-\hat x_0^+)^T\right]
\end{aligned}
\tag{13.31}
$$

3. Para $k=1,2,\cdots$, realice lo siguiente.

(a) Integre la estimación del estado y su covarianza desde el instante $(k-1)^+$ hasta el instante $k^-$, de la siguiente manera:

$$
\begin{aligned}
\dot{\hat x} &= f(\hat x,u,0,t) \\
\dot P &= AP+PA^T+LQL^T
\end{aligned}
\tag{13.32}
$$

donde $F$ y $L$ están dadas en la Ecuación (13.18). Comenzamos este proceso de integración con $\hat x = \hat x_{k-1}^+$ y $P = P_{k-1}^+$. Al final de esta integración obtenemos $\hat x = \hat x_k^-$ y $P = P_k^-$.

(b) En el instante $k$, incorpore la medición $y_k$ a la estimación del estado y a la covarianza de estimación, de la siguiente manera:

$$
\begin{aligned}
K_k &= P_k^- H_k^T\left(H_kP_k^-H_k^T+M_kR_kM_k^T\right)^{-1} \\
\hat x_k^+ &= \hat x_k^- + K_k\left(y_k-h_k(\hat x_k^-,0,t_k)\right) \\
P_k^+ &= (I-K_kH_k)P_k^-(I-K_kH_k)^T + K_kM_kR_kM_k^TK_k^T
\end{aligned}
\tag{13.33}
$$

$H_k$ y $M_k$ son las derivadas parciales de $h_k(x_k,v_k)$ respecto de $x_k$ y de $v_k$, respectivamente, y ambas se evalúan en $\hat x_k^-$. Nótese que pueden usarse otras expresiones equivalentes para $K_k$ y $P_k^+$, como resulta evidente a partir de la Ecuación (5.19).

\vspace{\baselineskip}
**EJEMPLO 13.2**

En este ejemplo usaremos el EKF en tiempo continuo y el EKF híbrido para estimar la altitud $x_1$, la velocidad $x_2$, y el coeficiente balístico constante $1/x_3$, de un cuerpo mientras cae hacia la Tierra. Un dispositivo de medición de distancia (telémetro) mide la altitud del cuerpo en caída. Este ejemplo (o alguna variante de él) se presenta en varias fuentes, por ejemplo, en [Ath68, Ste94, Jul00]. Las ecuaciones de este sistema son

$$
\begin{aligned}
\dot x_1 &= x_2 + w_1 \\
\dot x_2 &= \rho_0\exp(-x_1/k)x_2^2/2x_3 - g + w_2 \\
\dot x_3 &= w_3 \\
y &= x_1 + v
\end{aligned}
\tag{13.34}
$$

*(p. 406 del original)*

Como de costumbre, $w_i$ es el ruido que afecta a la $i$-ésima ecuación de proceso, y $v$ es el ruido de medición. $\rho_0$ es la densidad del aire al nivel del mar, $k$ es una constante que define la relación entre la densidad del aire y la altitud, y $g$ es la aceleración debida a la gravedad. Las matrices de derivadas parciales de este sistema están dadas de la siguiente manera:

$$
\begin{aligned}
A &= \frac{\partial f}{\partial x} =
\begin{bmatrix} 0 & 1 & 0 \\ A_{21} & A_{22} & A_{23} \\ 0 & 0 & 0 \end{bmatrix} \\[4pt]
A_{21} &= -\rho_0\exp(-x_1/k)\,x_2^2/2kx_3 \\
A_{22} &= \rho_0\exp(-x_1/k)\,x_2/x_3 \\
A_{23} &= -\rho_0\exp(-x_1/k)\,x_2^2/2x_3^2 \\
C = H &= \frac{\partial h}{\partial x} = \begin{bmatrix} 1 & 0 & 0 \end{bmatrix}
\end{aligned}
\tag{13.35}
$$

Usaremos las ecuaciones del sistema en tiempo continuo para simular el sistema. Para el sistema híbrido, supondremos que obtenemos mediciones de distancia cada 0,5 segundos. Las constantes que usaremos están dadas por

$$
\begin{aligned}
\rho_0 &= 0{,}0034 \text{ lb·s}^2/\text{ft}^4 \\
g &= 32{,}2 \text{ ft/s}^2 \\
k &= 22.000 \text{ ft} \\
E[v^2(t)] &= 100 \text{ ft}^2 \\
E[w_i^2(t)] &= 0 \quad (i=1,2,3)
\end{aligned}
\tag{13.36}
$$

Las condiciones iniciales del sistema y del estimador están dadas por

$$
\begin{aligned}
x_0 &= \begin{bmatrix} 100.000 & -6.000 & 1/2.000 \end{bmatrix}^T \\
\hat x_0^+ &= \begin{bmatrix} 100.010 & -6.100 & 1/2.500 \end{bmatrix}^T \\
P_0^+ &= \begin{bmatrix} 500 & 0 & 0 \\ 0 & 20.000 & 0 \\ 0 & 0 & 1/250.000 \end{bmatrix}
\end{aligned}
\tag{13.37}
$$

Utilizamos integración rectangular con un paso de 0,4 ms para simular el sistema, el EKF en tiempo continuo y el EKF híbrido (con un intervalo de medición de 0,5 s). La Figura 13.3 muestra las magnitudes del error de estimación, promediadas sobre 100 simulaciones, para la altitud, la velocidad y el recíproco del coeficiente balístico del cuerpo en caída. Vemos que el EKF en tiempo continuo, en general, parece tener un mejor desempeño que el EKF híbrido. Esto es de esperar, ya que el EKF en tiempo continuo incorpora más mediciones. Los errores de estimación RMS promediados sobre 100 simulaciones fueron de 2,8 pies para el EKF en tiempo continuo y 5,1 pies para el EKF híbrido en la estimación de la altitud; 1,2 pies/s para el EKF en tiempo continuo y 2,0 pies/s para el EKF híbrido en la estimación de la velocidad; y 213 para el EKF en tiempo continuo y 246 para el EKF híbrido

*(p. 407 del original)*

en la estimación del recíproco del coeficiente balístico. Por supuesto, un EKF en tiempo continuo (en hardware analógico) sería más difícil de implementar, ajustar y modificar que un EKF híbrido (en hardware digital).

**Figura 13.3.** Magnitudes del error de estimación de la altitud, la velocidad y el recíproco del coeficiente balístico de un cuerpo en caída, correspondientes al Ejemplo 13.2, promediadas sobre 100 simulaciones. El EKF en tiempo continuo generalmente tiene un mejor desempeño que el EKF híbrido. *(Gráfico no reproducido; muestra tres paneles — Altitud, Velocidad y 1/Coef. Balístico — en función del tiempo, comparando las curvas "Continuo" e "Híbrido".)*

▽▽▽

### 13.2.3 El filtro de Kalman extendido en tiempo discreto

En esta sección deduciremos el EKF en tiempo discreto, que considera dinámica en tiempo discreto y mediciones en tiempo discreto. Esta situación se encuentra a menudo en la práctica. Incluso si la dinámica subyacente del sistema es en tiempo continuo, el EKF por lo general debe implementarse en una computadora digital. Esto significa que podría no haber suficiente capacidad de cómputo para integrar la dinámica del sistema como lo requiere un EKF en tiempo continuo o un EKF híbrido. Por eso, la dinámica suele discretizarse (véase la Sección 1.4), y luego puede usarse un EKF en tiempo discreto.

Supongamos que tenemos el siguiente modelo de sistema

$$
\begin{aligned}
x_k &= f_{k-1}(x_{k-1},u_{k-1},w_{k-1}) \\
y_k &= h_k(x_k,v_k) \\
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.38}
$$

Realizamos una expansión en serie de Taylor de la ecuación de estado alrededor de $x_{k-1}=\hat x_{k-1}^+$ y $w_{k-1}=0$, para obtener lo siguiente:

*(p. 408 del original)*

$$
\begin{aligned}
x_k &= f_{k-1}(\hat x_{k-1}^+,u_{k-1},0) + \left.\frac{\partial f_{k-1}}{\partial x}\right|_{\hat x_{k-1}^+}(x_{k-1}-\hat x_{k-1}^+) + \left.\frac{\partial f_{k-1}}{\partial w}\right|_{\hat x_{k-1}^+}w_{k-1} \\
&= f_{k-1}(\hat x_{k-1}^+,u_{k-1},0) + F_{k-1}(x_{k-1}-\hat x_{k-1}^+) + L_{k-1}w_{k-1} \\
&= F_{k-1}x_{k-1} + \left[f_{k-1}(\hat x_{k-1}^+,u_{k-1},0)-F_{k-1}\hat x_{k-1}^+\right] + L_{k-1}w_{k-1} \\
&= F_{k-1}x_{k-1} + \tilde u_{k-1} + \tilde w_{k-1}
\end{aligned}
\tag{13.39}
$$

$F_{k-1}$ y $L_{k-1}$ quedan definidas por la ecuación anterior. La señal conocida $\tilde u_k$ y la señal de ruido $\tilde w_k$ se definen de la siguiente manera:

$$
\begin{aligned}
\tilde u_k &= f_k(\hat x_k^+,u_k,0) - F_k\hat x_k^+ \\
\tilde w_k &\sim (0,L_kQ_kL_k^T)
\end{aligned}
\tag{13.40}
$$

Linealizamos la ecuación de medición alrededor de $x_k=\hat x_k^-$ y $v_k=0$, para obtener

$$
\begin{aligned}
y_k &= h_k(\hat x_k^-,0) + \left.\frac{\partial h_k}{\partial x}\right|_{\hat x_k^-}(x_k-\hat x_k^-) + \left.\frac{\partial h_k}{\partial v}\right|_{\hat x_k^-}v_k \\
&= h_k(\hat x_k^-,0) + H_k(x_k-\hat x_k^-) + M_kv_k \\
&= H_kx_k + \left[h_k(\hat x_k^-,0)-H_k\hat x_k^-\right] + M_kv_k \\
&= H_kx_k + z_k + \tilde v_k
\end{aligned}
\tag{13.41}
$$

$H_k$ y $M_k$ quedan definidas por la ecuación anterior. La señal conocida $z_k$ y la señal de ruido $\tilde v_k$ se definen como

$$
\begin{aligned}
z_k &= h_k(\hat x_k^-,0) - H_k\hat x_k^- \\
\tilde v_k &\sim (0,M_kR_kM_k^T)
\end{aligned}
\tag{13.42}
$$

Tenemos un sistema lineal en el espacio de estados en la Ecuación (13.39) y una medición lineal en la Ecuación (13.41). Esto significa que podemos usar las ecuaciones estándar del filtro de Kalman para estimar el estado. Esto da como resultado las siguientes ecuaciones para el filtro de Kalman extendido en tiempo discreto.

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T \\
K_k &= P_k^-H_k^T\left(H_kP_k^-H_k^T+M_kR_kM_k^T\right)^{-1} \\
\hat x_k^- &= f_{k-1}(\hat x_{k-1}^+,u_{k-1},0) \\
z_k &= h_k(\hat x_k^-,0) - H_k\hat x_k^- \\
\hat x_k^+ &= \hat x_k^- + K_k(y_k-H_k\hat x_k^- - z_k) \\
&= \hat x_k^- + K_k\left[y_k-h_k(\hat x_k^-,0)\right] \\
P_k^+ &= (I-K_kH_k)P_k^-
\end{aligned}
\tag{13.43}
$$

El EKF en tiempo discreto puede resumirse de la siguiente manera.

*(p. 409 del original)*

**El filtro de Kalman extendido en tiempo discreto**

1. Las ecuaciones del sistema y de medición están dadas de la siguiente manera:

$$
\begin{aligned}
x_k &= f_{k-1}(x_{k-1},u_{k-1},w_{k-1}) \\
y_k &= h_k(x_k,v_k) \\
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.44}
$$

2. Inicialice el filtro de la siguiente manera:

$$
\begin{aligned}
\hat x_0^+ &= E(x_0) \\
P_0^+ &= E\left[(x_0-\hat x_0^+)(x_0-\hat x_0^+)^T\right]
\end{aligned}
\tag{13.45}
$$

3. Para $k=1,2,\cdots$, realice lo siguiente.

(a) Calcule las siguientes matrices de derivadas parciales:

$$
\begin{aligned}
F_{k-1} &= \left.\frac{\partial f_{k-1}}{\partial x}\right|_{\hat x_{k-1}^+} \\
L_{k-1} &= \left.\frac{\partial f_{k-1}}{\partial w}\right|_{\hat x_{k-1}^+}
\end{aligned}
\tag{13.46}
$$

(b) Realice la actualización temporal de la estimación del estado y de la covarianza del error de estimación, de la siguiente manera:

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T \\
\hat x_k^- &= f_{k-1}(\hat x_{k-1}^+,u_{k-1},0)
\end{aligned}
\tag{13.47}
$$

(c) Calcule las siguientes matrices de derivadas parciales:

$$
\begin{aligned}
H_k &= \left.\frac{\partial h_k}{\partial x}\right|_{\hat x_k^-} \\
M_k &= \left.\frac{\partial h_k}{\partial v}\right|_{\hat x_k^-}
\end{aligned}
\tag{13.48}
$$

(d) Realice la actualización de medición de la estimación del estado y de la covarianza del error de estimación, de la siguiente manera:

$$
\begin{aligned}
K_k &= P_k^-H_k^T\left(H_kP_k^-H_k^T+M_kR_kM_k^T\right)^{-1} \\
\hat x_k^+ &= \hat x_k^- + K_k\left[y_k-h_k(\hat x_k^-,0)\right] \\
P_k^+ &= (I-K_kH_k)P_k^-
\end{aligned}
\tag{13.49}
$$

Nótese que pueden usarse otras expresiones equivalentes para $K_k$ y $P_k^+$, como resulta evidente a partir de la Ecuación (5.19).
