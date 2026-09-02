*Fuente: Optimal State Estimation, primera edición, por Dan J. Simon. ISBN 0471708585. © 2006 John Wiley & Sons, Inc. (Capítulo 3, páginas 79–94 y 102 del original; ver nota sobre páginas faltantes más abajo, en la sección 3.4).*

---

# CAPÍTULO 3

# Estimación por mínimos cuadrados

> El valor más probable de las cantidades desconocidas será aquel para el cual la suma de los cuadrados de las diferencias entre los valores realmente observados y los valores calculados, multiplicados por números que midan el grado de precisión, sea mínima.
>
> —Karl Friedrich Gauss [Gau04]

En este capítulo trataremos la estimación por mínimos cuadrados, que es la idea básica de la cita de Karl Gauss mencionada arriba.¹ El material de este capítulo se apoya en la teoría de los dos capítulos anteriores y nos permitirá deducir, más adelante en este libro, estimadores de estado óptimos.

La sección 3.1 aborda la estimación de un vector constante a partir de varias mediciones lineales, pero ruidosas, de dicho vector. La sección 3.2 extiende los resultados de la sección 3.1 al caso en que algunas mediciones son más ruidosas que otras; es decir, tenemos menos confianza en algunas mediciones que en otras. Las secciones 3.1 y 3.2 emplean matrices y vectores cuyas dimensiones crecen a medida que se obtienen más mediciones. Esto vuelve engorroso el problema cuando se dispone de muchas mediciones. Esto nos lleva a la sección 3.3, que presenta una forma recursiva de estimar una constante a partir de mediciones ruidosas. La estimación recursiva, en este capítulo, es un método para estimar una constante sin que aumente el esfuerzo computacional del algoritmo, sin importar cuántas mediciones estén disponibles. Por último, la sección 3.4 presenta el filtro de Wiener, un método para estimar una señal variante en el tiempo que está corrompida por ruido, a partir de mediciones ruidosas. Hasta 1960, el filtrado de Wiener representaba el estado del arte en la estimación de señales. El paradigma de la estimación de señales se vio trastocado con la publicación del trabajo de Rudolph Kalman y artículos relacionados a comienzos de la década de 1960, pero aun así vale la pena comprender el filtrado de Wiener por su lugar histórico en la historia de la estimación de señales. Además, el filtrado de Wiener sigue siendo muy útil en el procesamiento de señales y en la teoría de la comunicación.

*¹ Gauss publicó su libro en 1809, aunque afirmó haber desarrollado su teoría ya en 1795 (cuando tenía 18 años).*

---

## 3.1 ESTIMACIÓN DE UNA CONSTANTE

En esta sección determinaremos cómo estimar una constante a partir de varias mediciones ruidosas de dicha constante. Por ejemplo, supongamos que tenemos un resistor pero no conocemos su resistencia. Tomamos varias mediciones de su resistencia con un multímetro, pero las mediciones son ruidosas porque tenemos un multímetro barato. Queremos estimar la resistencia a partir de nuestras mediciones ruidosas. En este caso, queremos estimar una constante escalar, pero, en general, podríamos querer estimar un vector constante.

Para plantear el problema en términos matemáticos, supongamos que $x$ es un vector de $n$ elementos, constante pero desconocido, y que $y$ es un vector de medición de $k$ elementos, ruidoso. Supongamos que cada elemento del vector de medición $y$ es una combinación lineal de los elementos de $x$, con el agregado de cierto ruido de medición:

$$
\begin{aligned}
y_1 &= H_{11}x_1 + \cdots + H_{1n}x_n + v_1 \\[2pt]
&\ \ \vdots \\[2pt]
y_k &= H_{k1}x_1 + \cdots + H_{kn}x_n + v_k
\end{aligned}
$$
*(3.1)*

Este conjunto de ecuaciones puede expresarse en forma matricial como

$$
y = Hx + v
$$
*(3.2)*

Ahora definamos $\epsilon_y$ como la diferencia entre las mediciones ruidosas y el vector $H\hat{x}$:

$$
\epsilon_y = y - H\hat{x}
$$
*(3.3)*

A $\epsilon_y$ se lo llama el **residuo de medición**. Como escribió Karl Gauss [Gau04], el valor más probable del vector $x$ es el vector $\hat{x}$ que minimiza la suma de los cuadrados entre los valores observados $y$ y el vector $H\hat{x}$. Por lo tanto, intentaremos calcular el $\hat{x}$ que minimiza la función de costo $J$, dada por

$$
\begin{aligned}
J &= \epsilon_{y1}^2 + \cdots + \epsilon_{yk}^2 \\
&= \epsilon_y^T\epsilon_y
\end{aligned}
$$
*(3.4)*

A $J$ se la suele llamar, en libros y artículos de control y estimación, **función de costo**, **función objetivo** o **función de retorno**. Podemos sustituir $\epsilon_y$ en la ecuación anterior para reescribir $J$ como

$$
\begin{aligned}
J &= (y - H\hat{x})^T(y - H\hat{x}) \\
&= y^Ty - \hat{x}^TH^Ty - y^TH\hat{x} + \hat{x}^TH^TH\hat{x}
\end{aligned}
$$
*(3.5)*

Para minimizar $J$ con respecto a $\hat{x}$, calculamos su derivada parcial y la igualamos a cero:

$$
\begin{aligned}
\frac{\partial J}{\partial \hat{x}} &= -y^TH - y^TH + 2\hat{x}^TH^TH \\
&= 0
\end{aligned}
$$
*(3.6)*

Al resolver esta ecuación para $\hat{x}$ se obtiene

$$
\begin{aligned}
H^Ty &= H^TH\hat{x} \\
\hat{x} &= (H^TH)^{-1}H^Ty \\
&= H^Ly
\end{aligned}
$$
*(3.7)*

donde $H^L$, la **pseudoinversa izquierda** de $H$, existe si $k \geq n$ y $H$ tiene rango completo. Esto significa que el número de mediciones $k$ es mayor que el número de variables $n$ que estamos tratando de estimar, y que las mediciones son linealmente independientes. Para demostrar que hemos hallado un mínimo y no algún otro tipo de punto estacionario² de $J$, es necesario demostrar que la segunda derivada de $J$ es semidefinida positiva (véase el problema 3.1).

**■ EJEMPLO 3.1**

Volvamos a nuestro problema original de estimar la resistencia $x$ de un resistor sin marcar, a partir de $k$ mediciones ruidosas tomadas con un multímetro. En este caso, $x$ es un escalar, de modo que nuestras $k$ mediciones ruidosas están dadas por

$$
\begin{aligned}
y_1 &= x + v_1 \\
&\ \ \vdots \\
y_k &= x + v_k
\end{aligned}
$$
*(3.8)*

Estas $k$ ecuaciones pueden combinarse en una única ecuación matricial:

$$
\begin{bmatrix} y_1 \\ \vdots \\ y_k \end{bmatrix} = \begin{bmatrix} 1 \\ \vdots \\ 1 \end{bmatrix} x + \begin{bmatrix} v_1 \\ \vdots \\ v_k \end{bmatrix}
$$
*(3.9)*

La ecuación (3.7) muestra que la estimación óptima de la resistencia $x$ está dada por

$$
\begin{aligned}
\hat{x} &= (H^TH)^{-1}H^Ty \\[4pt]
&= \left( \begin{bmatrix} 1 & \cdots & 1 \end{bmatrix} \begin{bmatrix} 1 \\ \vdots \\ 1 \end{bmatrix} \right)^{-1} \begin{bmatrix} 1 & \cdots & 1 \end{bmatrix} \begin{bmatrix} y_1 \\ \vdots \\ y_k \end{bmatrix} \\[4pt]
&= \frac{1}{k}(y_1 + \cdots + y_k)
\end{aligned}
$$
*(3.10)*

En este ejemplo sencillo, vemos que la estimación por mínimos cuadrados concuerda con nuestra intuición de simplemente calcular el promedio de las mediciones.

▽▽▽

*² Un punto estacionario de una función es cualquier punto en el que su derivada es igual a cero. Un punto estacionario de una función escalar puede ser un máximo, un mínimo o un punto de inflexión. Un punto estacionario de una función vectorial puede ser un máximo, un mínimo o un punto de silla.*

---

## 3.2 ESTIMACIÓN POR MÍNIMOS CUADRADOS PONDERADOS

En la sección anterior, supusimos que teníamos el mismo grado de confianza en todas nuestras mediciones. Supongamos ahora que tenemos más confianza en algunas mediciones que en otras. En este caso, necesitamos generalizar los resultados de la sección anterior para obtener la estimación por mínimos cuadrados ponderados. Por ejemplo, supongamos que tenemos varias mediciones de la resistencia de un resistor sin marcar. Algunas de las mediciones se tomaron con un multímetro caro y de bajo ruido, pero otras mediciones se tomaron con un multímetro barato, de noche, por un estudiante cansado. Tenemos más confianza en el primer conjunto de mediciones, de modo que deberíamos, de alguna manera, darle más énfasis a esas mediciones que a las otras. Sin embargo, aunque el segundo conjunto de mediciones es menos confiable, parece que igual podríamos obtener al menos alguna información a partir de ellas. Esta sección muestra que, en efecto, podemos obtener algo de información incluso de mediciones menos confiables. Nunca deberíamos descartar mediciones, sin importar cuán poco confiables puedan ser.

Para plantear el problema en términos matemáticos, supongamos que $x$ es un vector de $n$ elementos, constante pero desconocido, y que $y$ es un vector de medición de $k$ elementos, ruidoso. Suponemos que cada elemento de $y$ es una combinación lineal de los elementos de $x$, con el agregado de cierto ruido de medición, y que la varianza del ruido de medición puede ser diferente para cada elemento de $y$:

$$
\begin{bmatrix} y_1 \\ \vdots \\ y_k \end{bmatrix} = \begin{bmatrix} H_{11} & \cdots & H_{1n} \\ \vdots & \ddots & \vdots \\ H_{k1} & \cdots & H_{kn} \end{bmatrix} \begin{bmatrix} x_1 \\ \vdots \\ x_n \end{bmatrix} + \begin{bmatrix} v_1 \\ \vdots \\ v_k \end{bmatrix}
$$
$$
E(v_i^2) = \sigma_i^2 \quad (i = 1,\ldots,k)
$$
*(3.11)*

Suponemos que el ruido de cada medición tiene media cero y es independiente. La matriz de covarianza de la medición es

$$
\begin{aligned}
R &= E(vv^T) \\[4pt]
&= \begin{bmatrix} \sigma_1^2 & \cdots & 0 \\ \vdots & & \vdots \\ 0 & \cdots & \sigma_k^2 \end{bmatrix}
\end{aligned}
$$
*(3.12)*

Ahora minimizaremos la siguiente cantidad con respecto a $\hat{x}$:

$$
J = \epsilon_{y1}^2/\sigma_1^2 + \cdots + \epsilon_{yk}^2/\sigma_k^2
$$
*(3.13)*

Notemos que, en lugar de minimizar la suma de los cuadrados de los elementos de $\epsilon_y$ como hicimos en la ecuación (3.4), minimizaremos la suma de cuadrados **ponderada**. Si $y_1$ es una medición relativamente ruidosa, por ejemplo, entonces no nos preocupa tanto minimizar la diferencia entre $y_1$ y el primer elemento de $H\hat{x}$, porque no tenemos mucha confianza en $y_1$ en primer lugar. La función de costo $J$ puede escribirse como

$$
\begin{aligned}
J &= \epsilon_y^TR^{-1}\epsilon_y \\
&= (y-H\hat{x})^TR^{-1}(y-H\hat{x}) \\
&= y^TR^{-1}y - \hat{x}^TH^TR^{-1}y - y^TR^{-1}H\hat{x} + \hat{x}^TH^TR^{-1}H\hat{x}
\end{aligned}
$$
*(3.14)*

Ahora tomamos la derivada parcial de $J$ con respecto a $\hat{x}$ y la igualamos a cero para calcular la mejor estimación $\hat{x}$:

$$
\begin{aligned}
\frac{\partial J}{\partial \hat{x}} &= -y^TR^{-1}H + \hat{x}^TH^TR^{-1}H \\
&= 0 \\[4pt]
H^TR^{-1}y &= H^TR^{-1}H\hat{x} \\
\hat{x} &= (H^TR^{-1}H)^{-1}H^TR^{-1}y
\end{aligned}
$$
*(3.15)*

Notemos que este método requiere que la matriz de ruido de medición $R$ sea no singular. Es decir, cada una de las mediciones $y_i$ debe estar corrompida por al menos algo de ruido para que este método funcione.

**■ EJEMPLO 3.2**

Volvemos a nuestro problema original de tratar de estimar la resistencia $x$ de un resistor sin marcar, a partir de $k$ mediciones ruidosas tomadas con un multímetro. En este caso, $x$ es un escalar, de modo que nuestras $k$ mediciones ruidosas están dadas por

$$
\begin{aligned}
y_i &= x + v_i \\
E(v_i^2) &= \sigma_i^2 \quad (i=1,\ldots,k)
\end{aligned}
$$
*(3.16)*

Las $k$ ecuaciones de medición pueden combinarse en una única ecuación matricial:

$$
\begin{bmatrix} y_1 \\ \vdots \\ y_k \end{bmatrix} = \begin{bmatrix} 1 \\ \vdots \\ 1 \end{bmatrix} x + \begin{bmatrix} v_1 \\ \vdots \\ v_k \end{bmatrix}
$$
*(3.17)*

y la covarianza del ruido de medición está dada por

$$
R = \mathrm{diag}(\sigma_1^2,\ldots,\sigma_k^2)
$$
*(3.18)*

La ecuación (3.15) muestra que la estimación óptima de la resistencia $x$ está dada por

$$
\begin{aligned}
\hat{x} &= (H^TR^{-1}H)^{-1}H^TR^{-1}y \\[4pt]
&= \left( \begin{bmatrix} 1 & \cdots & 1 \end{bmatrix} \begin{bmatrix} \sigma_1^2 & \cdots & 0 \\ \vdots & \ddots & \vdots \\ 0 & \cdots & \sigma_k^2 \end{bmatrix}^{-1} \begin{bmatrix} 1 \\ \vdots \\ 1 \end{bmatrix} \right)^{-1} \times \\[4pt]
&\quad \begin{bmatrix} 1 & \cdots & 1 \end{bmatrix} \begin{bmatrix} \sigma_1^2 & \cdots & 0 \\ \vdots & \ddots & \vdots \\ 0 & \cdots & \sigma_k^2 \end{bmatrix}^{-1} \begin{bmatrix} y_1 \\ \vdots \\ y_k \end{bmatrix} \\[4pt]
&= \left(\sum 1/\sigma_i^2\right)^{-1} \left(y_1/\sigma_1^2 + \cdots + y_k/\sigma_k^2\right)
\end{aligned}
$$
*(3.19)*

Vemos que la estimación óptima $\hat{x}$ es una suma ponderada de las mediciones, en la que cada medición se pondera por el inverso de su incertidumbre. En otras palabras, ponemos más énfasis en ciertas mediciones, de acuerdo con nuestra intuición. Notemos que, si todas las constantes $\sigma_i$ son iguales, esta estimación se reduce a la forma más simple dada en la ecuación (3.10).

▽▽▽

---

## 3.3 ESTIMACIÓN RECURSIVA POR MÍNIMOS CUADRADOS

La ecuación (3.15) nos da una manera de calcular la estimación óptima de una constante, pero hay un problema. Notemos que la matriz $H$ en (3.15) es una matriz de $k \times n$. Si obtenemos las mediciones de forma secuencial y queremos actualizar nuestra estimación de $x$ con cada nueva medición, necesitamos aumentar la matriz $H$ y recalcular por completo la estimación $\hat{x}$. Si el número de mediciones se vuelve grande, el esfuerzo computacional podría volverse prohibitivo. Por ejemplo, supongamos que obtenemos una medición de la altitud de un satélite una vez por segundo. Después de que transcurre una hora, el número de mediciones es 3600 y sigue creciendo. El esfuerzo computacional de la estimación por mínimos cuadrados puede superar rápidamente nuestros recursos.

En esta sección mostraremos cómo calcular, de manera recursiva, la estimación por mínimos cuadrados ponderados de una constante. Es decir, supongamos que tenemos $\hat{x}$ después de $(k-1)$ mediciones, y obtenemos una nueva medición $y_k$. ¿Cómo podemos actualizar nuestra estimación sin reformular por completo la ecuación (3.15)?

Un estimador recursivo lineal puede escribirse de la forma

$$
\begin{aligned}
y_k &= H_kx + v_k \\
\hat{x}_k &= \hat{x}_{k-1} + K_k(y_k - H_k\hat{x}_{k-1})
\end{aligned}
$$
*(3.20)*

Es decir, calculamos $\hat{x}_k$ a partir de la estimación anterior $\hat{x}_{k-1}$ y la nueva medición $y_k$. $K_k$ es una matriz que debe determinarse, llamada **matriz de ganancia del estimador**. A la cantidad $(y_k - H_k\hat{x}_{k-1})$ se la llama **término de corrección**. Notemos que si el término de corrección es cero, o si la matriz de ganancia es cero, entonces la estimación no cambia del instante $(k-1)$ al instante $k$.

Antes de calcular la matriz de ganancia óptima $K_k$, pensemos en la media del error de estimación del estimador recursivo lineal. La media del error de estimación puede calcularse como

$$
\begin{aligned}
E(\epsilon_{x,k}) &= E(x - \hat{x}_k) \\
&= E[x - \hat{x}_{k-1} - K_k(y_k - H_k\hat{x}_{k-1})] \\
&= E[x - \hat{x}_{k-1} - K_k(H_kx + v_k - H_k\hat{x}_{k-1})] \\
&= E[\epsilon_{x,k-1} - K_kH_k(x - \hat{x}_{k-1}) - K_kv_k] \\
&= (I - K_kH_k)E(\epsilon_{x,k-1}) - K_kE(v_k)
\end{aligned}
$$
*(3.21)*

Entonces, si $E(v_k) = 0$ y $E(\epsilon_{x,k-1}) = 0$, entonces $E(\epsilon_{x,k}) = 0$. En otras palabras, si el ruido de medición $v_k$ tiene media cero para todo $k$, y la estimación inicial de $x$ se fija igual al valor esperado de $x$ [es decir, $\hat{x}_0 = E(x)$], entonces el valor esperado de $\hat{x}_k$ será igual a $x_k$ para todo $k$. Debido a esto, al estimador de la ecuación (3.20) se lo llama **estimador insesgado**. Notemos que esta propiedad se cumple sin importar el valor de la matriz de ganancia $K_k$. Esta es una propiedad deseable de un estimador, porque indica que, en promedio, la estimación $\hat{x}$ será igual al valor verdadero $x$.

A continuación centramos nuestra atención en la determinación del valor óptimo de $K_k$. Dado que el estimador es insesgado sin importar qué valor de $K_k$ usemos, debemos elegir algún otro criterio de optimalidad para determinar $K_k$. El criterio de optimalidad que elegimos minimizar es la suma de las varianzas de los errores de estimación en el instante $k$:

$$
\begin{aligned}
J_k &= E[(x_1-\hat{x}_1)^2] + \cdots + E[(x_n-\hat{x}_n)^2] \\
&= E\left(\epsilon_{x1,k}^2 + \cdots + \epsilon_{xn,k}^2\right) \\
&= E\left(\epsilon_{x,k}^T\epsilon_{x,k}\right) \\
&= E\left[\mathrm{Tr}(\epsilon_{x,k}\epsilon_{x,k}^T)\right] \\
&= \mathrm{Tr}\,P_k
\end{aligned}
$$
*(3.22)*

donde $P_k$, la **covarianza del error de estimación**, se define mediante la ecuación anterior. Podemos usar un proceso similar al que seguimos en la ecuación (3.21) para obtener una fórmula recursiva para el cálculo de $P_k$:

$$
\begin{aligned}
P_k &= E(\epsilon_{x,k}\epsilon_{x,k}^T) \\
&= E\left\{[(I-K_kH_k)\epsilon_{x,k-1} - K_kv_k][\cdots]^T\right\} \\
&= (I-K_kH_k)E(\epsilon_{x,k-1}\epsilon_{x,k-1}^T)(I-K_kH_k)^T - \\
&\quad K_kE(v_k\epsilon_{x,k-1}^T)(I-K_kH_k)^T - (I-K_kH_k)E(\epsilon_{x,k-1}v_k^T)K_k^T + \\
&\quad K_kE(v_kv_k^T)K_k^T
\end{aligned}
$$
*(3.23)*

Ahora notemos que $\epsilon_{x,k-1}$ [el error de estimación en el instante $(k-1)$] es independiente de $v_k$ (el ruido de medición en el instante $k$). Por lo tanto,

$$
\begin{aligned}
E(v_k\epsilon_{x,k-1}^T) &= E(v_k)E(\epsilon_{x,k-1}) \\
&= 0
\end{aligned}
$$
*(3.24)*

ya que ambos valores esperados son cero. Por lo tanto, la ecuación (3.23) se convierte en

$$
P_k = (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T
$$
*(3.25)*

donde $R_k$ es la covarianza de $v_k$. Esta es la fórmula recursiva para la covarianza del error de la estimación por mínimos cuadrados. Esto es coherente con la intuición, en el sentido de que, a medida que aumenta el ruido de medición (es decir, $R_k$ aumenta), también aumenta la incertidumbre en nuestra estimación (es decir, $P_k$ aumenta). Notemos que $P_k$ debería ser definida positiva, dado que es una matriz de covarianza, y la forma de la ecuación (3.25) garantiza que $P_k$ será definida positiva, suponiendo que $P_{k-1}$ y $R_k$ son definidas positivas.

Ahora necesitamos hallar el valor de $K_k$ que hace que la función de costo en la ecuación (3.22) sea lo más pequeña posible. La media del error de estimación es cero para cualquier valor de $K_k$. Entonces, si elegimos $K_k$ de manera que la función de costo (es decir, la traza de $P_k$) sea pequeña, el error de estimación no solo tendrá media cero, sino que además será consistentemente cercano a cero. Para hallar el mejor valor de $K_k$, primero necesitamos recordar, a partir de la ecuación (1.66), que $\partial\mathrm{Tr}(ABA^T)/\partial A = 2AB$ si $B$ es simétrica. Teniendo esto en cuenta, podemos usar las ecuaciones (3.22), (3.25) y la regla de la cadena para obtener

$$
\frac{\partial J_k}{\partial K_k} = 2(I-K_kH_k)P_{k-1}(-H_k^T) + 2K_kR_k
$$
*(3.26)*

Para hallar el valor de $K_k$ que minimiza $J_k$, igualamos a cero la derivada anterior y luego resolvemos para $K_k$ de la siguiente manera:

$$
\begin{aligned}
K_kR_k &= (I-K_kH_k)P_{k-1}H_k^T \\
K_k(R_k+H_kP_{k-1}H_k^T) &= P_{k-1}H_k^T \\
K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}
\end{aligned}
$$
*(3.27)*

Las ecuaciones (3.20), (3.25) y (3.27) forman el estimador recursivo de mínimos cuadrados. El estimador recursivo de mínimos cuadrados puede resumirse de la siguiente manera.

> **Estimación recursiva por mínimos cuadrados**
>
> 1. Inicializar el estimador de la siguiente manera:
>
> $$
> \begin{aligned}
> \hat{x}_0 &= E(x) \\
> P_0 &= E[(x-\hat{x}_0)(x-\hat{x}_0)^T]
> \end{aligned}
> $$
> *(3.28)*
>
> Si no se dispone de ningún conocimiento sobre $x$ antes de tomar las mediciones, entonces $P_0 = \infty I$. Si se dispone de un conocimiento perfecto sobre $x$ antes de tomar las mediciones, entonces $P_0 = 0$.
>
> 2. Para $k = 1, 2, \cdots$, realizar lo siguiente.
>
> (a) Obtener la medición $y_k$, suponiendo que $y_k$ está dada por la ecuación
>
> $$
> y_k = H_kx + v_k
> $$
> *(3.29)*
>
> donde $v_k$ es un vector aleatorio de media cero con covarianza $R_k$. Supongamos, además, que el ruido de medición en cada instante $k$ es independiente, es decir, $E(v_iv_k) = R_k\delta_{k-i}$. Esto implica que el ruido de medición es blanco.
>
> (b) Actualizar la estimación de $x$ y la covarianza del error de estimación $P$ de la siguiente manera:
>
> $$
> \begin{aligned}
> K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
> \hat{x}_k &= \hat{x}_{k-1} + K_k(y_k - H_k\hat{x}_{k-1}) \\
> P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T
> \end{aligned}
> $$
> *(3.30)*

### 3.3.1 Formas alternativas del estimador

A veces resulta útil escribir las ecuaciones de $P_k$ y $K_k$ en formas alternativas. Aunque estas formas alternativas son matemáticamente idénticas, pueden resultar beneficiosas desde un punto de vista computacional. También pueden dar lugar a nuevos resultados, que descubriremos en capítulos posteriores.

Primero hallaremos una forma alternativa para la expresión de la covarianza del error de estimación. Sustituyendo $K_k$ de la ecuación (3.27) en la ecuación (3.25) obtenemos

$$
P_k = \left[I - P_{k-1}H_k^TS_k^{-1}H_k\right]P_{k-1}[\cdots]^T + K_kR_kK_k^T
$$
*(3.31)*

donde hemos introducido la variable auxiliar $S_k = (H_kP_{k-1}H_k^T + R_k)$. Nuevamente sustituimos $K_k$ al final de esta ecuación y expandimos los términos para obtener

$$
\begin{aligned}
P_k &= P_{k-1} - P_{k-1}H_k^TS_k^{-1}H_kP_{k-1} - P_{k-1}H_k^TS_k^{-1}H_kP_{k-1} + \\
&\quad P_{k-1}H_k^TS_k^{-1}H_kP_{k-1}H_k^TS_k^{-1}H_kP_{k-1} + P_{k-1}H_k^TS_k^{-1}R_kS_k^{-1}H_kP_{k-1}
\end{aligned}
$$
*(3.32)*

Combinando los últimos dos términos de esta ecuación se obtiene

$$
\begin{aligned}
P_k &= P_{k-1} - 2P_{k-1}H_k^TS_k^{-1}H_kP_{k-1} + P_{k-1}H_k^TS_k^{-1}S_kS_k^{-1}H_kP_{k-1} \\
&= P_{k-1} - 2P_{k-1}H_k^TS_k^{-1}H_kP_{k-1} + P_{k-1}H_k^TS_k^{-1}H_kP_{k-1} \\
&= P_{k-1} - P_{k-1}H_k^TS_k^{-1}H_kP_{k-1}
\end{aligned}
$$
*(3.33)*

Ahora notemos, a partir de la expresión de $K_k$ en la ecuación (3.27), que $K_k$ aparece de manera implícita en la ecuación anterior. Por lo tanto, podemos reescribir esta ecuación como

$$
\begin{aligned}
P_k &= P_{k-1} - K_kH_kP_{k-1} \\
&= (I-K_kH_k)P_{k-1}
\end{aligned}
$$
*(3.34)*

Esta es una ecuación más simple para $P_k$ [en comparación con la ecuación (3.25)], pero los problemas numéricos de cómputo (es decir, cuestiones de escala) pueden hacer que esta expresión para $P_k$ no resulte definida positiva, incluso cuando $P_{k-1}$ y $R_k$ son definidas positivas.

También podemos usar el lema de inversión de matrices de la sección 1.1.2 para reescribir la ecuación de actualización de la medición para $P_k$. Partiendo de la ecuación (3.33) obtenemos

$$
P_k = P_{k-1} - P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}H_kP_{k-1}
$$
*(3.35)*

Tomando la inversa de ambos lados de esta ecuación se obtiene

$$
P_k^{-1} = \left[P_{k-1} - P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}H_kP_{k-1}\right]^{-1}
$$
*(3.36)*

Aplicando el lema de inversión de matrices a esta ecuación se obtiene

$$
\begin{aligned}
P_k^{-1} &= P_{k-1}^{-1} + P_{k-1}^{-1}P_{k-1}H_k^T\left[(H_kP_{k-1}H_k^T+R_k) - \right. \\
&\qquad \left. H_kP_{k-1}P_{k-1}^{-1}(P_{k-1}H_k^T)\right]^{-1}H_kP_{k-1}P_{k-1}^{-1} \\
&= P_{k-1}^{-1} + H_k^TR_k^{-1}H_k
\end{aligned}
$$
*(3.37)*

Invirtiendo ambos lados de esta ecuación se obtiene

$$
P_k = \left[P_{k-1}^{-1} + H_k^TR_k^{-1}H_k\right]^{-1}
$$
*(3.38)*

Esta ecuación para $P_k$ es más complicada, ya que requiere tres inversiones de matrices, pero puede resultar computacionalmente ventajosa en algunas situaciones, como se explicará en la sección 6.2.

Podemos usar la ecuación (3.38) para deducir una ecuación equivalente para la ganancia del estimador $K_k$. Partiendo de la ecuación (3.27) tenemos

$$
K_k = P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}
$$
*(3.39)*

Premultiplicando el lado derecho por $P_kP_k^{-1}$, que es igual a la matriz identidad, se obtiene

$$
K_k = P_kP_k^{-1}P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}
$$
*(3.40)*

Sustituyendo $P_k^{-1}$ a partir de la ecuación (3.38) se obtiene

$$
K_k = P_k(P_{k-1}^{-1}+H_k^TR_k^{-1}H_k)P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1}
$$
*(3.41)*

Notemos el factor $P_{k-1}H_k^T$ que aparece a la derecha del primer término entre paréntesis. Podemos multiplicar este factor dentro del primer término entre paréntesis para obtener

$$
K_k = P_k(H_k^T + H_k^TR_k^{-1}H_kP_{k-1}H_k^T)(H_kP_{k-1}H_k^T+R_k)^{-1}
$$
*(3.42)*

Ahora llevamos $H_k^T$ hacia afuera, al lado izquierdo de los paréntesis, para obtener

$$
K_k = P_kH_k^T(I + R_k^{-1}H_kP_{k-1}H_k^T)(H_kP_{k-1}H_k^T+R_k)^{-1}
$$
*(3.43)*

Ahora premultiplicamos la primera expresión entre paréntesis por $R_k^{-1}$, y multiplicamos por dentro de la expresión entre paréntesis por $R_k$, para obtener

$$
\begin{aligned}
K_k &= P_kH_k^TR_k^{-1}(R_k + H_kP_{k-1}H_k^T)(H_kP_{k-1}H_k^T+R_k)^{-1} \\
&= P_kH_k^TR_k^{-1}
\end{aligned}
$$
*(3.44)*

> **Estimación recursiva general por mínimos cuadrados**
>
> El algoritmo recursivo de mínimos cuadrados puede resumirse mediante las siguientes ecuaciones. Las ecuaciones de medición están dadas por
>
> $$
> \begin{aligned}
> y_k &= H_kx + v_k \\
> x &= \text{constante} \\
> E(v_k) &= 0 \\
> E(v_kv_i^T) &= R_k\delta_{k-i}
> \end{aligned}
> $$
> *(3.45)*
>
> La estimación inicial del vector constante $x$, junto con la incertidumbre de dicha estimación, está dada por
>
> $$
> \begin{aligned}
> \hat{x}_0 &= E(x) \\
> P_0 &= E[(x-\hat{x}_0)(x-\hat{x}_0)^T]
> \end{aligned}
> $$
> *(3.46)*
>
> El algoritmo recursivo de mínimos cuadrados está dado de la siguiente manera.
> Para $k = 1, 2, \cdots$,
>
> $$
> \begin{aligned}
> K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
> &= P_kH_k^TR_k^{-1} \\[4pt]
> \hat{x}_k &= \hat{x}_{k-1} + K_k(y_k - H_k\hat{x}_{k-1}) \\[4pt]
> P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T \\
> &= (P_{k-1}^{-1}+H_k^TR_k^{-1}H_k)^{-1} \\
> &= (I-K_kH_k)P_{k-1}
> \end{aligned}
> $$
> *(3.47)*

**■ EJEMPLO 3.3**

Una vez más, retomamos el problema de tratar de estimar la resistencia $x$ de un resistor sin marcar, a partir de mediciones ruidosas tomadas con un multímetro. Sin embargo, no queremos esperar a tener todas las mediciones para obtener una estimación. Queremos modificar de manera recursiva nuestra estimación de $x$ cada vez que obtenemos una nueva medición. En el instante de muestreo $k$, nuestra medición es

$$
\begin{aligned}
y_k &= H_kx + v_k \\
H_k &= 1 \\
R_k &= E(v_k^2)
\end{aligned}
$$
*(3.48)*

Para este problema escalar, la matriz de medición $H_k$ es un escalar, y la covarianza del ruido de medición $R_k$ también es un escalar. Supondremos que cada medición tiene la misma covarianza, de modo que la covarianza de medición $R_k$ no depende de $k$ y puede escribirse como $R$. Inicialmente, antes de tener alguna medición, tenemos alguna idea sobre el valor de la resistencia $x$, y esto constituye nuestra estimación inicial. También tenemos cierta incertidumbre acerca de nuestra estimación inicial, y esto constituye nuestra covarianza inicial:

$$
\begin{aligned}
\hat{x}_0 &= E(x) \\
P_0 &= E[(x-\hat{x}_0)(x-\hat{x}_0)^T] \\
&= E[(x-\hat{x}_0)^2]
\end{aligned}
$$
*(3.49)*

Si no tenemos absolutamente ninguna idea sobre el valor de la resistencia, entonces $P(0) = \infty$. Si estamos 100% seguros del valor de la resistencia antes de tomar cualquier medición, entonces $P(0) = 0$ (pero, en ese caso, por supuesto, no habría ninguna necesidad de tomar mediciones). La ecuación (3.47) nos indica cómo obtener la ganancia del estimador, la estimación de $x$ y la covarianza de estimación, después de la primera medición ($k=1$):

$$
\begin{aligned}
K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
K_1 &= P_0(P_0+R)^{-1} \\[4pt]
\hat{x}_k &= \hat{x}_{k-1} + K_k(y_k-H_k\hat{x}_{k-1}) \\
\hat{x}_1 &= \hat{x}_0 + \frac{P_0}{P_0+R}(y_1-\hat{x}_0) \\[4pt]
P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T \\
P_1 &= \frac{P_0R}{P_0+R}
\end{aligned}
$$
*(3.50)*

Repitiendo estos cálculos para hallar estas cantidades después de la segunda medición ($k=2$) se obtiene

$$
\begin{aligned}
K_2 &= \frac{P_1}{P_1+R} = \frac{P_0}{2P_0+R} \\[4pt]
P_2 &= \frac{P_1R}{P_1+R} = \frac{P_0R}{2P_0+R} \\[4pt]
\hat{x}_2 &= \hat{x}_1 + \frac{P_1}{P_1+R}(y_2-\hat{x}_1) \\
&= \frac{P_0+R}{2P_0+R}\hat{x}_1 + \frac{P_0}{2P_0+R}y_2
\end{aligned}
$$
*(3.51)*

Por inducción, podemos hallar expresiones generales para $P_{k-1}$, $K_k$ y $\hat{x}_k$ de la siguiente manera:

$$
\begin{aligned}
P_{k-1} &= \frac{P_0R}{(k-1)P_0+R} \\[4pt]
K_k &= \frac{P_0}{kP_0+R} \\[4pt]
\hat{x}_k &= \hat{x}_{k-1} + K_k(y_k-\hat{x}_{k-1}) \\
&= (1-K_k)\hat{x}_{k-1} + K_ky_k \\
&= \frac{(k-1)P_0+R}{kP_0+R}\hat{x}_{k-1} + \frac{P_0}{kP_0+R}y_k
\end{aligned}
$$
*(3.52)*

Notemos que, si $x$ se conoce perfectamente a priori (es decir, antes de obtener cualquier medición), entonces $P_0 = 0$, y las ecuaciones anteriores muestran que $K_k = 0$ y $\hat{x}_k = \hat{x}_0$. Es decir, la estimación óptima de $x$ es independiente de cualquier medición que se obtenga. Por otro lado, si $x$ es completamente desconocida a priori, entonces $P_0 \to \infty$, y las ecuaciones anteriores muestran que

$$
\begin{aligned}
\hat{x}_k &= \frac{(k-1)P_0}{kP_0}\hat{x}_{k-1} + \frac{P_0}{kP_0}y_k \\
&= \frac{k-1}{k}\hat{x}_{k-1} + \frac{1}{k}y_k \\
&= \frac{1}{k}\left[(k-1)\hat{x}_{k-1} + y_k\right]
\end{aligned}
$$
*(3.53)*

En otras palabras, la estimación óptima de $x$ es igual al promedio móvil de las mediciones $y_k$, que puede escribirse como

$$
\begin{aligned}
\bar{y}_k &= \frac{1}{k}\sum_{j=1}^{k}y_j \\
&= \frac{1}{k}\left(\sum_{j=1}^{k-1}y_j + y_k\right) \\
&= \frac{1}{k}\left[(k-1)\left(\frac{1}{k-1}\sum_{j=1}^{k-1}y_j\right) + y_k\right] \\
&= \frac{1}{k}\left[(k-1)\bar{y}_{k-1} + y_k\right]
\end{aligned}
$$
*(3.54)*

▽▽▽

**■ EJEMPLO 3.4**

En este ejemplo, ilustramos las ventajas computacionales de la primera forma de la actualización de la covarianza en la ecuación (3.47), en comparación con la tercera forma. Supongamos que tenemos un parámetro escalar $x$ y una medición perfecta de dicho parámetro. Es decir, $H_1 = 1$ y $R_1 = 0$. Supongamos además que nuestra covarianza de estimación inicial es $P_0 = 6$, y que nuestra computadora ofrece una precisión de tres dígitos a la derecha del punto decimal para cada cantidad que calcula. La ganancia del estimador $K_1$ se calcula como

$$
\begin{aligned}
K_1 &= P_0(P_0+R_1)^{-1} \\
&= (6)\left(\frac{1}{6}\right) \\
&= (6)(0.167) \\
&= 1.002
\end{aligned}
$$
*(3.55)*

Si usamos la tercera forma de la actualización de la covarianza en la ecuación (3.47), obtenemos

$$
\begin{aligned}
P_1 &= (1-K_1)P_0 \\
&= (-0.002)(6) \\
&= -0.012
\end{aligned}
$$
*(3.56)*

La covarianza después de la primera medición es negativa, lo cual es físicamente imposible. Sin embargo, si usamos la primera forma de la actualización de la covarianza en la ecuación (3.47), obtenemos

$$
\begin{aligned}
P_1 &= (1-K_1)P_0(1-K_1) + K_1^2R_1 \\
&= (1-K_1)^2P_0 + K_1^2R_1 \\
&= 0
\end{aligned}
$$
*(3.57)*

La razón por la que obtenemos cero es que $(1-K_1)^2 = 0.000004$, pero nuestra computadora conserva solamente tres dígitos a la derecha del punto decimal. Cero es el valor teóricamente correcto de $P_1$. La forma de la expresión anterior para $P_1$ garantiza que esta nunca será negativa, sin importar los errores numéricos que pueda haber en $P_0$, $R_1$ y $K_1$.

▽▽▽

**■ EJEMPLO 3.5**

Supongamos que un tanque contiene una concentración $x_1$ de la sustancia química 1, y una concentración $x_2$ de la sustancia química 2. Se dispone de instrumentación capaz de detectar la concentración combinada $(x_1+x_2)$ de las dos sustancias, pero dicha instrumentación no permite distinguir entre ambas. La sustancia química 2 se elimina del tanque mediante un proceso de lixiviación, de modo que su concentración disminuye en un 1% de un instante de medición al siguiente. Por lo tanto, la ecuación de medición está dada por

$$
\begin{aligned}
y_k &= x_1 + 0.99^{k-1}x_2 + v_k \\
&= \begin{bmatrix} 1 & 0.99^{k-1} \end{bmatrix} x + v_k
\end{aligned}
$$
*(3.58)*

donde $v_k$ es el ruido de medición, que es una variable aleatoria de media cero con varianza $R = 0.01$. Supongamos que $x_1 = 10$ y $x_2 = 5$. Supongamos, además, que nuestras estimaciones iniciales son $\hat{x}_1 = 8$ y $\hat{x}_2 = 7$, con una varianza inicial del error de estimación $P_0$ igual a la matriz identidad. Puede implementarse un algoritmo recursivo de mínimos cuadrados, tal como se muestra en la ecuación (3.47), para estimar las dos concentraciones. La figura 3.1 muestra la estimación de $x_1$ y $x_2$ conforme se obtienen mediciones, junto con la varianza de los errores de estimación. Puede observarse que, después de un par de docenas de mediciones, las estimaciones se acercan bastante a sus valores verdaderos de 10 y 5. Las varianzas de los errores de estimación se aproximan asintóticamente a cero, lo cual significa que tenemos cada vez más confianza en nuestras estimaciones a medida que obtenemos más mediciones.

> *[Figura 3.1 — dos paneles frente al instante de tiempo $k$ (0 a 50): panel superior, "estimaciones", con $\hat{x}_1$ (línea continua) ascendiendo desde 8 hacia 10 y $\hat{x}_2$ (línea punteada) descendiendo desde 7 hacia 5; panel inferior, "varianzas", con $P(1,1)$ (línea continua) y $P(2,2)$ (línea punteada) decreciendo desde aproximadamente 1 hacia 0.]*
>
> **Figura 3.1** Estimaciones de los parámetros y varianzas de estimación para el ejemplo 3.5.

▽▽▽

### 3.3.2 Ajuste de curvas

En esta sección aplicaremos la teoría de mínimos cuadrados recursivos al problema de ajuste de curvas. En el problema de ajuste recursivo de curvas, medimos datos una muestra a la vez ($y_1, y_2, \cdots$) y queremos hallar el mejor ajuste de una curva a los datos. La curva que queremos ajustar a los datos podría estar restringida a ser lineal, cuadrática, sinusoidal, o de alguna otra forma, según el problema subyacente.

**■ EJEMPLO 3.6**

Supongamos que queremos ajustar una línea recta a un conjunto de puntos de datos. El problema de ajuste lineal de datos puede escribirse como

$$
\begin{aligned}
y_k &= x_1 + x_2t_k + v_k \\
E(v_k^2) &= R_k
\end{aligned}
$$
*(3.59)*

$t_k$ es la variable independiente (tal vez el tiempo), $y_k$ son los datos ruidosos, y queremos hallar la relación lineal entre $y_k$ y $t_k$. En otras palabras, queremos estimar las constantes $x_1$ y $x_2$. La matriz de medición puede escribirse como

$$
H_k = \begin{bmatrix} 1 & t_k \end{bmatrix}
$$
*(3.60)*

de modo que la ecuación (3.59) puede escribirse como

$$
y_k = H_kx + v_k
$$
*(3.61)*

Nuestro estimador recursivo se inicializa como

$$
\begin{aligned}
\hat{x}_0 &= E(x) \\[2pt]
\begin{bmatrix} \hat{x}_{1,0} \\ \hat{x}_{2,0} \end{bmatrix} &= \begin{bmatrix} E(x_1) \\ E(x_2) \end{bmatrix} \\[6pt]
P_0 &= E[(x-\hat{x}_0)(x-\hat{x}_0)^T] \\
&= \begin{bmatrix} E[(x_1-\hat{x}_{1,0})^2] & E[(x_1-\hat{x}_{1,0})(x_2-\hat{x}_{2,0})] \\ E[(x_1-\hat{x}_{1,0})(x_2-\hat{x}_{2,0})] & E[(x_2-\hat{x}_{2,0})^2] \end{bmatrix}
\end{aligned}
$$
*(3.62)*

La estimación recursiva del vector $x$ de dos elementos se obtiene entonces a partir de la ecuación (3.47) de la siguiente manera:
Para $k = 1, 2, \cdots$,

$$
\begin{aligned}
K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
\hat{x}_k &= \hat{x}_{k-1} + K_k(y_k-H_k\hat{x}_{k-1}) \\
P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T
\end{aligned}
$$
*(3.63)*

▽▽▽

**■ EJEMPLO 3.7**

Supongamos que sabemos a priori que los datos subyacentes son una función cuadrática del tiempo. En este caso, tenemos un problema de ajuste cuadrático de datos. Por ejemplo, supongamos que estamos midiendo la altitud de un objeto en caída libre. Sabemos, a partir de nuestra comprensión de la física, que la altitud $r$ es una función de la aceleración de la gravedad, la altitud y velocidad iniciales del objeto $r_0$ y $v_0$, y el tiempo $t$, según la ecuación $r = r_0 + v_0t + (a/2)t^2$. Entonces, si medimos $r$ en distintos instantes y ajustamos una curva cuadrática a la curva resultante de $r$ en función de $t$, obtenemos una estimación de los parámetros $r_0$, $v_0$ y $a/2$. En general, el problema de ajuste cuadrático de datos puede escribirse como

$$
\begin{aligned}
y_k &= x_1 + x_2t_k + x_3t_k^2 + v_k \\
E(v_k^2) &= R_k
\end{aligned}
$$
*(3.64)*

$t_k$ es la variable independiente, $y_k$ es la medición ruidosa, y queremos hallar la relación cuadrática entre $y_k$ y $t_k$. En otras palabras, queremos estimar las constantes $x_1$, $x_2$ y $x_3$. La matriz de medición puede escribirse como

$$
H_k = \begin{bmatrix} 1 & t_k & t_k^2 \end{bmatrix}
$$
*(3.65)*

de modo que la ecuación (3.64) puede escribirse como

$$
y_k = H_kx + v_k
$$
*(3.66)*

Nuestro estimador recursivo se inicializa como

$$
\begin{aligned}
\hat{x}_0 &= E(x) \\
P_0 &= E[(x-\hat{x}_0)(x-\hat{x}_0)^T]
\end{aligned}
$$
*(3.67)*

donde $P_0$ es una matriz de $3\times3$. La estimación recursiva del vector $x$ de tres elementos se obtiene entonces a partir de la ecuación (3.47) de la siguiente manera:
Para $k = 1, 2, \cdots$,

$$
\begin{aligned}
K_k &= P_{k-1}H_k^T(H_kP_{k-1}H_k^T+R_k)^{-1} \\
\hat{x}_k &= \hat{x}_{k-1} + K_k(y_k-H_k\hat{x}_{k-1}) \\
P_k &= (I-K_kH_k)P_{k-1}(I-K_kH_k)^T + K_kR_kK_k^T
\end{aligned}
$$
*(3.68)*

▽▽▽

---

## 3.4 FILTRADO DE WIENER

En esta sección, ofreceremos una breve reseña del filtrado de Wiener. El resto del libro no supone ningún conocimiento previo del lector sobre el filtrado de Wiener. Sin embargo, el filtrado de Wiener es importante desde una perspectiva histórica, y todavía tiene numerosas aplicaciones en el procesamiento de señales y en la teoría de la comunicación. Pero, dado que ya no se usa demasiado para la estimación de estado, el lector puede omitir esta sección sin problema, si así lo desea.

El filtrado de Wiener aborda el problema de diseñar un filtro lineal e invariante en el tiempo para extraer una señal a partir del ruido, abordando el problema desde la perspectiva del dominio de la frecuencia. Norbert Wiener inventó su filtro como parte del esfuerzo de la Segunda Guerra Mundial para los Estados Unidos. Publicó su trabajo sobre el problema en 1942, pero no estuvo disponible para el público hasta 1949 [Wie64]. Su libro fue conocido como el "peligro amarillo" debido a su dificultad matemática y su portada amarilla [Deu65, pág. 176]. Andrey Kolmogorov, de hecho, había resuelto antes un problema más general (1941), y Mark Krein también trabajó en el mismo problema (1945). El trabajo de Kolmogorov y el de Krein fueron independientes del de Wiener, y Wiener reconoce que el trabajo de Kolmogorov se adelantó al suyo [Wie56]. Sin embargo, el trabajo de Kolmogorov y de Krein no se hizo bien conocido en el mundo occidental hasta más tarde, ya que había sido publicado en ruso [Kol41]. En su autobiografía [Wie56] se ofrece una descripción no técnica del trabajo de Wiener.

Para preparar la presentación del filtro de Wiener, primero necesitamos plantear la siguiente pregunta: ¿cómo cambia el espectro de potencia de un proceso estocástico $x(t)$ cuando este atraviesa un sistema LTI con respuesta al impulso $g(t)$? La salida $y(t)$ del sistema está dada por la convolución de la respuesta al impulso con la entrada:

$$
y(t) = g(t) * x(t)
$$
*(3.69)*

Dado que el sistema es invariante en el tiempo, un desplazamiento temporal en la entrada produce un desplazamiento temporal igual en la salida:

$$
y(t+\alpha) = g(t) * x(t+\alpha)
$$
*(3.70)*

Multiplicando las dos ecuaciones anteriores y escribiendo las convoluciones como integrales se obtiene

$$
y(t)y(t+\alpha) = \int g(\tau)x(t-\tau)\,d\tau \int g(\gamma)x(t+\alpha-\gamma)\,d\gamma
$$
*(3.71)*

Tomando el valor esperado de ambos lados de la ecuación anterior se obtiene la autocorrelación de $y(t)$ en función de la autocorrelación de $x(t)$:

$$
E[y(t)y(t+\alpha)] = \int\int g(\tau)g(\gamma)E[x(t-\tau)x(t+\alpha-\gamma)]\,d\tau\,d\gamma
$$
*(3.72)*

> **[Nota del traductor: el PDF proporcionado salta de la página 94 a la página 102. Las páginas 95 a 101 —que contienen el resto del desarrollo del filtro de Wiener en la sección 3.4— no fueron incluidas en el archivo subido y, por lo tanto, no forman parte de esta traducción.]**

---

## 3.5 RESUMEN

En este capítulo tratamos la estimación por mínimos cuadrados en un par de contextos diferentes. Primero dedujimos un método para estimar un vector constante a partir de varias mediciones ruidosas de dicho vector. De hecho, las mediciones no tienen que ser mediciones directas del vector constante, sino que pueden ser mediciones de alguna combinación lineal de los elementos del vector constante. Además, el ruido asociado con cada medición no tiene que ser el mismo. La técnica de estimación por mínimos cuadrados que dedujimos supuso que el ruido de medición tiene media cero y es blanco (no correlacionado consigo mismo de un instante al siguiente), y que conocemos la varianza del ruido de medición. Luego extendimos nuestro estimador de mínimos cuadrados a una formulación recursiva, en la que el esfuerzo computacional se mantiene igual en cada instante, sin importar el número total de mediciones que hayamos procesado. La estimación por mínimos cuadrados de un vector constante constituye gran parte del fundamento del filtro de Kalman, que deduciremos más adelante en este libro.

En la sección 3.4, hicimos una breve incursión en el filtrado de Wiener, que es un método para estimar una señal variante en el tiempo que está corrompida por ruido. El filtro de Wiener se basa en análisis en el dominio de la frecuencia, mientras que el filtro de Kalman que deduciremos más adelante se basa en análisis en el dominio del tiempo. No obstante, ambos filtros son óptimos bajo sus propios supuestos. Algunos problemas pueden resolverse tanto con los métodos de Wiener como con los de Kalman, en cuyo caso ambos métodos dan el mismo resultado.

---

## PROBLEMAS

### Ejercicios escritos

**3.1** En la ecuación (3.6) calculamos la derivada parcial de nuestra función de costo con respecto a nuestra estimación, y la igualamos a cero para resolver la estimación óptima. Sin embargo, la solución minimiza la función de costo solo si la segunda derivada de la función de costo con respecto a la estimación es semidefinida positiva. Halle la segunda derivada de la función de costo y demuestre que es semidefinida positiva.

**3.2** Demuestre que la matriz $P_k$ calculada a partir de la ecuación (3.25) siempre será definida positiva si $P_{k-1}$ y $R_k$ son definidas positivas.

**3.3** Considere el estimador recursivo de mínimos cuadrados de las ecuaciones (3.28)-(3.30). Si no se dispone de ninguna información sobre el estado inicial, entonces $P_0 = \infty I$. Suponga que tiene un sistema como este con $H_k = 1$. ¿Cuáles serán los valores de $K_1$ y $P_1$?

**3.4** Considere una batería con un voltaje completamente desconocido ($P_0 = \infty$). Se toman dos mediciones independientes del voltaje para estimar el voltaje, la primera con una varianza de 1, y la segunda con una varianza de 4.

a) Escriba la estimación de voltaje por mínimos cuadrados ponderados en términos de las dos mediciones $y_1$ y $y_2$.

b) Si se utiliza el método de mínimos cuadrados ponderados para estimar el voltaje, ¿cuál es la varianza de la estimación de voltaje después de la primera medición? ¿Cuál es la varianza de la estimación de voltaje después de la segunda medición?
