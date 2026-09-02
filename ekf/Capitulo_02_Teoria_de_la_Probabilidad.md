# Capítulo 2 — Teoría de la probabilidad

> *Nota del traductor: traducción completa y fiel del Capítulo 2, "Probability Theory", del libro* Optimal State Estimation *de Dan J. Simon (Wiley, 2006), con el mismo criterio usado en los capítulos anteriores. Terminología de este capítulo: variable aleatoria (VA) para "random variable (RV)"; el original distingue "PDF" (función de distribución, mayúsculas) de "pdf" (función de densidad, minúsculas) — aquí se preserva esa misma distinción con FDP/fdp; "strict-sense stationary (SSS)" → estacionario en sentido estricto (ESE); "wide-sense stationary (WSS)" → estacionario en sentido amplio (ESA); "skew" → asimetría; "skewness" → coeficiente de asimetría. Se mantiene el registro formal ("usted") ya usado en los ejemplos de trato directo del Capítulo 1. Se corrigió una errata evidente de OCR ("Kolomogorov" → Kolmogórov) y una inconsistencia de subíndices ($z_1,z_1$ → $z_1,z_2$) en una integral doble, por ser claros artefactos de transcripción y no decisiones de contenido. La Figura 2.1, 2.2, 2.3 y 2.4 no se reproducen gráficamente; se conserva su leyenda traducida.*

---

> Lo más que podemos saber es en términos de probabilidades.
>
> —Richard Feynman [Fey63, p. 6-11]

> Mientras escribía mi libro [*Stochastic Processes*, publicado por primera vez en 1953] tuve una discusión con Feller. Él sostenía que todos decían "random variable" [variable aleatoria] y yo sostenía que todos decían "chance variable" [variable de azar]. Evidentemente teníamos que usar el mismo nombre en nuestros libros, así que decidimos la cuestión mediante un procedimiento estocástico. Es decir, lo decidimos a cara o cruz, y él ganó.
>
> —Joseph Doob [Sne97, p. 307]

> Las probabilidades no existen.
>
> —Bruno de Finetti [deF74]

En nuestro intento de filtrar una señal, intentaremos extraer información significativa a partir de una señal ruidosa. Para lograrlo, necesitamos saber algo acerca de qué es el ruido, algunas de sus características, y cómo funciona. Este capítulo repasa la teoría de la probabilidad. Comenzamos analizando el concepto básico de probabilidad en la Sección 2.1, y luego pasamos a las variables aleatorias (VA) en la Sección 2.2. El capítulo continúa luego con los siguientes temas:

- Una VA es un caso general de los escalares comunes con los que estamos familiarizados, y así como podemos aplicar una función a un número, también podemos aplicar

*Optimal State Estimation, primera edición.* Por Dan J. Simon — pág. 49
ISBN 0471708585 © 2006 John Wiley & Sons, Inc.

---

una función a una VA. Analizamos las funciones (transformaciones) de variables aleatorias en la Sección 2.3.

- Así como podemos tener vectores de números, también podemos tener vectores de VA, por lo que analizamos grupos de variables aleatorias y vectores aleatorios en la Sección 2.4.

- Así como podemos tener funciones escalares del tiempo, también podemos tener VA que sean funciones del tiempo, por lo que analizamos las VA que cambian con el tiempo (procesos estocásticos) en la Sección 2.5.

- Los procesos estocásticos pueden dividirse en dos categorías: ruido blanco y ruido coloreado, y analizamos estos conceptos en la Sección 2.6.

Concluimos en la Sección 2.7 con un análisis de alto nivel sobre cómo escribir una simulación por computadora de un proceso de ruido. Este capítulo es solo una breve introducción y repaso de la probabilidad y los procesos estocásticos, y puede encontrarse más detalle en muchos otros libros sobre el tema, como [Pap02, Pee01].

## 2.1 PROBABILIDAD

¿Cómo definiremos el concepto de probabilidad? Supongamos que realizamos un experimento cierta cantidad de veces. A veces ocurre el evento $A$ y a veces no. Por ejemplo, nuestro experimento puede ser tirar un dado de seis caras. El evento $A$ puede definirse como que salga el número 4 en la cara superior del dado después de tirarlo. El sentido común nos dice que la probabilidad de que ocurra el evento $A$ es $1/6$. De igual manera, esperaríamos que, si realizamos nuestro experimento muchas veces, veamos aparecer el número 1 alrededor de $1/6$ de las veces. Esta explicación intuitiva forma la base de nuestra descripción formal del concepto de probabilidad. Definimos la probabilidad del evento $A$ como

$$P(A) = \frac{\text{Número de veces que ocurre } A}{\text{Número total de resultados}} \tag{2.1}$$

A esta comprensión intuitiva de la probabilidad se la llama la definición de frecuencia relativa. Puede obtenerse una definición más formal y matemáticamente rigurosa de la probabilidad usando la teoría de conjuntos [Bil95, Nel87], que fue impulsada por Andréi Kolmogórov en la década de 1930. Pero, para nuestros propósitos, la definición de frecuencia relativa es adecuada.

En general, sabemos que hay $n$ sobre $k$ maneras distintas de elegir $k$ objetos de un total de $n$ objetos (suponiendo que el orden de los objetos no importa), donde $n$ sobre $k$ se denota y se define como

$$\binom{n}{k} = \frac{n!}{(n-k)!k!} \tag{2.2}$$

Por ejemplo, supongamos que tenemos una moneda de un centavo (P, de *penny*), una de cinco centavos (N, de *nickel*), una de diez centavos (D, de *dime*), y una de veinticinco centavos (Q, de *quarter*). ¿Cuántos subconjuntos distintos de tres monedas podemos elegir de ese conjunto? Podemos elegir PND, PNQ, PDQ, o NDQ, lo cual da un total de cuatro subconjuntos posibles. Esto es igual a 4 sobre 3.

**EJEMPLO 2.1**

¿Cuál es la probabilidad de que le sirvan póker¹ (es decir, cuatro cartas del mismo valor) jugando al póker? El número total de manos de póker posibles puede calcularse como el número total de subconjuntos de tamaño cinco que se pueden elegir de un mazo de 52 cartas. El número total de manos posibles es 52 sobre 5 $=2.598.960$. De todas esas manos, hay 48 manos posibles que contienen cuatro ases, 48 manos posibles que contienen cuatro reyes, y así sucesivamente. Entonces hay un total de $13\times 48$ manos que contienen póker. Por lo tanto, la probabilidad de que le sirvan póker es

$$
\begin{aligned}
P(A) &= \frac{(13)(48)}{2.598.960} \\
&= 1/4165 \\
&\approx 0{,}024\%
\end{aligned}
\tag{2.3}
$$

▽▽▽

La probabilidad condicional del evento $A$ dado el evento $B$ puede definirse si la probabilidad de $B$ es distinta de cero. La probabilidad condicional de $A$ dado $B$ se define como

$$P(A|B) = \frac{P(A,B)}{P(B)} \tag{2.4}$$

$P(A|B)$ es la probabilidad condicional de $A$ dado $B$, es decir, la probabilidad de que ocurra $A$ dado el hecho de que ocurrió $B$. $P(A,B)$ es la probabilidad conjunta de $A$ y $B$, es decir, la probabilidad de que ocurran tanto el evento $A$ como el evento $B$. A la probabilidad de un único evento [por ejemplo, $P(A)$ o $P(B)$] se la llama probabilidad *a priori*, porque se aplica a la probabilidad de un evento independientemente de cualquier información previamente conocida. A una probabilidad condicional [por ejemplo, $P(A|B)$] se la llama probabilidad *a posteriori*, porque se aplica a una probabilidad dado el hecho de que ya se conoce alguna información sobre un evento posiblemente relacionado.

Por ejemplo, supongamos que $A$ es la aparición de un 4 en un dado, y $B$ es la aparición de un número par en un dado. $P(A)=1/6$. Pero si sabemos que en el dado salió un número par, entonces $P(A)=1/3$ (ya que el número par podría ser 2, 4 o 6). Este ejemplo es intuitivo, pero también podemos obtener la respuesta usando la Ecuación (2.4). $P(A,B)$ es la probabilidad de que ocurran tanto $A$ (sacamos un 4) como $B$ (sacamos un número par), por lo que $P(A,B)=1/6$. Entonces la Ecuación (2.4) da

$$
\begin{aligned}
P(A|B) &= \frac{1/6}{1/2} \\
&= 1/3
\end{aligned}
\tag{2.5}
$$

La probabilidad *a priori* de $A$ es $1/6$. Pero la probabilidad *a posteriori* de $A$ dado $B$ es $1/3$.

**EJEMPLO 2.2**

Consideremos las ocho figuras de la Figura 2.1. Tenemos tres círculos y cinco cuadrados, por lo que $P(\text{círculo})=3/8$. Solo una de las figuras es un círculo gris, por lo que $P(\text{gris}, \text{círculo})$

*¹Una vez me sirvieron póker de sietes mientras jugaba con unos amigos (por desgracia, en ese momento no estaba jugando por dinero). No espero volver a verlo en mi vida.*

$=1/8$. De los tres círculos, solo uno es gris, por lo que $P(\text{gris}|\text{círculo})=1/3$. Esta última probabilidad puede calcularse usando la Ecuación (2.4) como

$$
\begin{aligned}
P(\text{gris}|\text{círculo}) &= \frac{P(\text{gris},\text{círculo})}{P(\text{círculo})} \\
&= \frac{1/8}{3/8} \\
&= 1/3
\end{aligned}
\tag{2.6}
$$

**Figura 2.1.** Algunas figuras para ilustrar la probabilidad y la regla de Bayes. *(Gráfico no reproducido; muestra ocho figuras —cuadrados y círculos, algunos grises y otros blancos.)*

▽▽▽

Nótese que podemos usar la Ecuación (2.4) para escribir $P(B|A)=P(A,B)/P(A)$. Podemos despejar $P(A,B)$ tanto de esta ecuación como de la Ecuación (2.4), e igualar las dos expresiones de $P(A,B)$ para obtener la regla de Bayes.

$$P(A|B)P(B) = P(B|A)P(A) \tag{2.7}$$

La regla de Bayes suele escribirse reordenando la ecuación anterior, para obtener

$$P(A|B) = \frac{P(B|A)P(A)}{P(B)} \tag{2.8}$$

Como ejemplo, consideremos la Figura 2.1. La probabilidad de elegir una figura gris, dado el hecho de que la figura es un círculo, puede calcularse a partir de la regla de Bayes como

$$
\begin{aligned}
P(\text{gris}|\text{círculo}) &= \frac{P(\text{círculo}|\text{gris})P(\text{gris})}{P(\text{círculo})} \\
&= \frac{(1/5)(5/8)}{3/8} \\
&= 1/3
\end{aligned}
\tag{2.9}
$$

Decimos que dos eventos son independientes si la ocurrencia de uno de ellos no tiene efecto sobre la probabilidad de ocurrencia del otro. Por ejemplo, si $A$ es la aparición de un 4 al tirar un dado, y $B$ es la aparición de un 3 al tirar *otro* dado, entonces $A$ y $B$ son independientes. Matemáticamente, la independencia de $A$ y $B$ puede expresarse de varias maneras distintas. Por ejemplo, podemos escribir

$$
\begin{aligned}
P(A,B) &= P(A)P(B) \\
P(A|B) &= P(A) \\
P(B|A) &= P(B)
\end{aligned}
\tag{2.10}
$$

si $A$ y $B$ son independientes. Como ejemplo, recordemos, a partir de la Ecuación (2.5), que si $A$ es la aparición de un 4 en un dado, y $B$ es la aparición de un número par en un dado, entonces $P(A)=1/6$ y $P(A|B)=1/3$. Dado que $P(A|B)\neq P(A)$, vemos que $A$ y $B$ son eventos dependientes.

## 2.2 VARIABLES ALEATORIAS

Definimos una variable aleatoria (VA) como una función que mapea un conjunto de resultados experimentales (el dominio) a un conjunto de números reales (el rango). Por ejemplo, la tirada de un dado puede considerarse una VA si mapeamos la aparición de un punto en el dado a la salida uno, la aparición de dos puntos en el dado a la salida dos, y así sucesivamente.

Por supuesto, después de tirar el dado, el valor del dado ya no es una variable aleatoria: se vuelve cierto. El resultado de un experimento en particular no es una VA. Si definimos $X$ como una VA que representa la tirada de un dado, entonces la probabilidad de que $X$ sea cuatro es igual a $1/6$. Si luego tiramos un cuatro, el cuatro es una realización de la VA $X$. Si volvemos a tirar el dado y obtenemos un tres, el tres es otra realización de la VA $X$. Sin embargo, la VA $X$ existe independientemente de cualquiera de sus realizaciones. Esta distinción entre una VA y sus realizaciones es importante para comprender el concepto de probabilidad. Las realizaciones de una VA no son iguales a la VA en sí. Cuando decimos que la probabilidad de $X=4$ es igual a $1/6$, eso significa que hay 1 posibilidad en 6 de que cada realización de $X$ sea igual a 4. Sin embargo, la VA $X$ siempre será aleatoria y nunca será igual a un valor específico.

Una VA puede ser continua o discreta. La tirada de un dado es una variable aleatoria discreta porque sus realizaciones pertenecen a un conjunto discreto de valores. La temperatura máxima de mañana es una variable aleatoria continua porque sus realizaciones pertenecen a un conjunto continuo de valores.

La propiedad más fundamental de una VA $X$ es su función de distribución de probabilidad (FDP) $F_X(x)$, definida como

$$F_X(x) = P(X\leq x) \tag{2.11}$$

En la ecuación anterior, $F_X(x)$ es la FDP de la VA $X$, y $x$ es una variable independiente o constante no aleatoria. Algunas propiedades de la FDP que pueden obtenerse a partir de su definición son

$$
\begin{aligned}
F_X(x) &\in [0,1] \\
F_X(-\infty) &= 0 \\
F_X(\infty) &= 1 \\
F_X(a) &\leq F_X(b) \quad \text{si } a\leq b \\
P(a<X\leq b) &= F_X(b)-F_X(a)
\end{aligned}
\tag{2.12}
$$

La función de densidad de probabilidad (fdp) $f_X(x)$ se define como la derivada de la FDP.

$$f_X(x) = \frac{dF_X(x)}{dx} \tag{2.13}$$

Algunas propiedades de la fdp que pueden obtenerse a partir de esta definición son

$$
\begin{aligned}
F_X(x) &= \int_{-\infty}^{x}f_X(z)\,dz \\
f_X(x) &\geq 0 \\
\int_{-\infty}^{\infty}f_X(x)\,dx &= 1 \\
P(a<x\leq b) &= \int_a^b f_X(x)\,dx
\end{aligned}
\tag{2.14}
$$

La función $Q$ de una VA se define como uno menos la FDP. Esto es igual a la probabilidad de que la VA sea mayor que el argumento de la función:

$$
\begin{aligned}
Q(x) &= 1-F_X(x) \\
&= P(X>x)
\end{aligned}
\tag{2.15}
$$

Así como hablamos de probabilidades condicionales en la Ecuación (2.4), también podemos hablar de la FDP condicional y la fdp condicional. La distribución y la densidad condicionales de la VA $X$, dado el hecho de que ocurrió el evento $A$, se definen como

$$
\begin{aligned}
F_X(x|A) &= P(X\leq x|A) \\
&= \frac{P(X\leq x,A)}{P(A)} \\
f_X(x|A) &= \frac{dF_X(x|A)}{dx}
\end{aligned}
\tag{2.16}
$$

La regla de Bayes, analizada en la Sección 2.1, puede generalizarse a densidades condicionales. Supongamos que tenemos las variables aleatorias $X_1$ y $X_2$. La fdp condicional de la VA $X_1$, dado el hecho de que la VA $X_2$ es igual a la realización $x_2$, se define como

$$
\begin{aligned}
f_{X_1|X_2}(x_1|x_2) &= P[(X_1\leq x_1)|(X_2=x_2)] \\
&= \frac{f_{X_1,X_2}(x_1,x_2)}{f_{X_2}(x_2)}
\end{aligned}
\tag{2.17}
$$

Aunque esto no es del todo intuitivo, puede deducirse sin demasiada dificultad [Pap02, Pee01]. Consideremos ahora el siguiente producto de dos fdp condicionales:

$$
\begin{aligned}
f[x_1|(x_2,x_3,x_4)]f[(x_2,x_3)|x_4] &= \frac{f(x_1,x_2,x_3,x_4)}{f(x_2,x_3,x_4)}\frac{f(x_2,x_3,x_4)}{f(x_4)} \\
&= \frac{f(x_1,x_2,x_3,x_4)}{f(x_4)} \\
&= f[(x_1,x_2,x_3)|x_4]
\end{aligned}
\tag{2.18}
$$

Nótese que en la ecuación anterior omitimos los subíndices de las funciones $f(\cdot)$ para simplificar la notación. Esto se hace habitualmente cuando la variable aleatoria asociada con la fdp resulta clara a partir del contexto. A esto se lo llama la ecuación de Chapman–Kolmogorov [Pap02]. Puede extenderse a cualquier número de VA, y es fundamental para el enfoque bayesiano de la estimación de estado (Capítulo 15).

El valor esperado de una VA $X$ se define como su valor promedio a lo largo de una gran cantidad de experimentos. A esto también se lo puede llamar la esperanza, la media, o el promedio de la VA. Supongamos que realizamos el experimento $N$ veces y observamos un total de $m$ resultados distintos. Observamos que el resultado $A_1$ ocurre $n_1$ veces, $A_2$ ocurre $n_2$ veces, $\cdots$, y $A_m$ ocurre $n_m$ veces. Entonces el valor esperado de $X$ se calcula como

$$E(X) = \frac{1}{N}\sum_{i=1}^{m}A_in_i \tag{2.19}$$

$E(X)$ también suele escribirse como $E(x)$, $\bar X$, o $\bar x$.

A partir de aquí, comenzaremos a usar la $x$ minúscula en lugar de la $X$ mayúscula cuando el significado sea claro. Hemos estado usando la $X$ mayúscula para referirnos a una VA, y la $x$ minúscula para referirnos a una realización de la VA, que es una constante o variable independiente. Sin embargo, debería quedar claro que, por ejemplo, $E(x)$ es el valor esperado de la VA $X$, por lo que intercambiaremos $x$ y $X$ para simplificar la notación.

Como ejemplo del valor esperado de una VA, supongamos que tiramos un dado un número infinito de veces. Esperaríamos ver cada número posible (del uno al seis) $1/6$ de las veces cada uno. Podemos calcular el valor esperado de la tirada del dado como

$$
\begin{aligned}
E(X) &= \lim_{N\to\infty}\frac{1}{N}\left[(1)(N/6)+\cdots+(6)(N/6)\right] \\
&= 3{,}5
\end{aligned}
\tag{2.20}
$$

Nótese que el valor esperado de una VA no es necesariamente lo que esperaríamos ver al realizar un experimento en particular. Por ejemplo, aunque el valor esperado de $X$ de arriba es $3{,}5$, nunca veremos un $3{,}5$ al tirar un dado.

También podemos hablar de una función de una VA, tal como podemos hablar de una función de cualquier escalar. (Analizaremos esto con más detalle en la Sección 2.3.) Si una función, digamos $g(X)$, actúa sobre una VA, entonces la salida de la función también es una VA. Por ejemplo, si $X$ es la tirada de un dado, entonces $P(X=4)=1/6$. Si $g(X)=X^2$, entonces $P[g(X)=16]=1/6$. Podemos calcular el valor esperado de cualquier función $g(X)$ como

$$E[g(X)] = \int_{-\infty}^{\infty}g(x)f_X(x)\,dx \tag{2.21}$$

donde $f_X(x)$ es la fdp de $X$. Si $g(X)=X$, entonces podemos calcular el valor esperado de $X$ como

$$E(X) = \int_{-\infty}^{\infty}xf_X(x)\,dx \tag{2.22}$$

La varianza de una VA es una medida de cuánto esperamos que la VA varíe respecto de su media. La varianza es una medida de cuánta variabilidad hay en una VA. En el caso extremo en que la VA $X$ siempre sea igual a un mismo valor (por ejemplo, el dado está cargado y siempre sacamos un 4 al tirarlo), la varianza de $X$ es igual a 0. En el otro extremo, si $X$ puede tomar cualquier valor entre $\pm\infty$ con igual probabilidad, entonces la varianza de $X$ es igual a $\infty$. La varianza de una VA se define formalmente como

$$
\begin{aligned}
\sigma_X^2 &= E[(X-\bar x)^2] \\
&= \int_{-\infty}^{\infty}(x-\bar x)^2f_X(x)\,dx
\end{aligned}
\tag{2.23}
$$

La desviación estándar de una VA es $\sigma$, que es la raíz cuadrada de la varianza. A veces denotamos la desviación estándar como $\sigma_X$ si necesitamos ser explícitos acerca de la VA cuya desviación estándar estamos analizando. Nótese que la varianza puede escribirse como

$$
\begin{aligned}
\sigma^2 &= E[X^2-2X\bar x+\bar x^2] \\
&= E(X^2)-2\bar x^2+\bar x^2 \\
&= E(X^2)-\bar x^2
\end{aligned}
\tag{2.24}
$$

Usamos la notación

$$X \sim (\bar x,\sigma^2) \tag{2.25}$$

para indicar que $X$ es una VA con media $\bar x$ y varianza $\sigma^2$.

La asimetría de una VA es una medida de cuán asimétrica es la fdp alrededor de su media. La asimetría se define como

$$\text{asimetría} = E[(X-\bar x)^3] \tag{2.26}$$

A la asimetría normalizada por el cubo de la desviación estándar se la llama coeficiente de asimetría:

$$\text{coeficiente de asimetría} = \text{asimetría}/\sigma^3 \tag{2.27}$$

En general, el momento $i$-ésimo de una variable aleatoria $X$ es el valor esperado de la $i$-ésima potencia de $X$. El momento central $i$-ésimo de una variable aleatoria $X$ es el valor esperado de la $i$-ésima potencia de $X$ menos su media:

$$
\begin{aligned}
\text{momento } i\text{-ésimo de } X &= E(X^i) \\
\text{momento central } i\text{-ésimo de } X &= E[(X-\bar x)^i]
\end{aligned}
\tag{2.28}
$$

Por ejemplo, el primer momento de una variable aleatoria es igual a su media. El primer momento central de una variable aleatoria siempre es igual a 0. El segundo momento central de una variable aleatoria es igual a su varianza.

Una VA se llama uniforme si su fdp es un valor constante entre dos límites. Esto indica que la VA tiene igual probabilidad de tomar cualquier valor entre sus límites, pero una probabilidad nula de tomar un valor fuera de esos límites:

$$
f_X(x) = \begin{cases} \dfrac{1}{b-a} & x\in[a,b] \\ 0 & \text{en otro caso} \end{cases}
\tag{2.29}
$$

La Figura 2.2 muestra la fdp de una VA uniformemente distribuida entre $\pm 1$. Nótese que el área bajo esta curva es uno (como lo es el área de todas las fdp).

**EJEMPLO 2.3**

En este ejemplo hallaremos la media y la varianza de una VA uniformemente distribuida entre 1 y 3. La fdp de la VA está dada por

$$
f_X(x) = \begin{cases} 1/2 & x\in[1,3] \\ 0 & \text{en otro caso} \end{cases}
\tag{2.30}
$$

**Figura 2.2.** Función de densidad de probabilidad de una VA uniformemente distribuida entre $\pm 1$. *(Gráfico no reproducido; muestra una función constante de altura 0,5 entre $x=-1$ y $x=1$, y cero fuera de ese intervalo.)*

La media se calcula de la siguiente manera:

$$
\begin{aligned}
\bar x &= \int_{-\infty}^{\infty}xf_X(x)\,dx \\
&= \int_1^3 \frac{1}{2}x\,dx \\
&= 2
\end{aligned}
\tag{2.31}
$$

La varianza se calcula de la siguiente manera:

$$
\begin{aligned}
\sigma_X^2 &= \int_{-\infty}^{\infty}\frac{1}{2}(x-\bar x)^2f(x)\,dx \\
&= \int_1^3 \frac{1}{2}(x-2)^2\,dx \\
&= \frac{1}{3}
\end{aligned}
\tag{2.32}
$$

▽▽▽

Una VA se llama gaussiana o normal si su fdp está dada por

$$f_X(x) = \frac{1}{\sigma\sqrt{2\pi}}\exp\left[\frac{-(x-\bar x)^2}{2\sigma^2}\right] \tag{2.33}$$

A esto se lo llama la distribución de Laplace en Francia, pero tuvo muchos otros descubridores, entre ellos Robert Adrain. Nótese que $\bar x$ y $\sigma$ en la fdp anterior son la media y la desviación estándar de la VA gaussiana. Usamos la notación

$$X \sim N(\bar x,\sigma^2) \tag{2.34}$$

para indicar que $X$ es una VA gaussiana con media $\bar x$ y varianza $\sigma^2$. La Figura 2.3 muestra la fdp de una VA gaussiana con media cero y varianza de uno. Si la media cambia, la fdp se desplazará hacia la izquierda o la derecha. Si la varianza aumenta, la fdp se ensanchará. Si la varianza disminuye, la fdp se comprimirá. La FDP de una VA gaussiana está dada por

$$F_X(x) = \frac{1}{\sigma\sqrt{2\pi}}\int_{-\infty}^{x}\exp[-(z-\bar x)^2/2\sigma^2]\,dz \tag{2.35}$$

Esta integral no tiene una solución en forma cerrada, por lo que debe evaluarse numéricamente. Sin embargo, su evaluación puede simplificarse si consideramos la FDP gaussiana normalizada de una VA con media cero y varianza unitaria:

$$F_{X0}(x) = \frac{1}{\sqrt{2\pi}}\int_{-\infty}^{x}\exp(-z^2/2)\,dz \tag{2.36}$$

Puede demostrarse que cualquier FDP gaussiana puede expresarse en términos de esta FDP normalizada como

$$F_X(x) = F_{X0}\left(\frac{x-\bar x}{\sigma}\right) \tag{2.37}$$

Además, una FDP gaussiana puede aproximarse mediante la siguiente expresión en forma cerrada [Bor79]:

$$
\begin{aligned}
F_X(x) &\approx 1-\left[\frac{1}{(1-a)x+a\sqrt{x^2+b}}\right]\frac{\exp(-x^2/2)}{\sqrt{2\pi}} \qquad x\geq 0 \\
a &= 0{,}339 \\
b &= 5{,}510
\end{aligned}
\tag{2.38}
$$

**Figura 2.3.** Función de densidad de probabilidad de una VA gaussiana con media cero y varianza uno. *(Gráfico no reproducido; muestra la clásica curva acampanada, simétrica alrededor de $x=0$, con un valor máximo de aproximadamente 0,4.)*

Supongamos que tenemos una variable aleatoria $X$ con media cero y una fdp simétrica [es decir, $f_X(x)=f_X(-x)$]. Este es el caso, por ejemplo, de las fdp mostradas en las Figuras 2.2 y 2.3. En este caso, el momento $i$-ésimo de $X$ puede escribirse como

$$
\begin{aligned}
m_i &= E(X^i) \\
&= \int_{-\infty}^{\infty}x^if_X(x)\,dx \\
&= \int_{-\infty}^{0}x^if_X(x)\,dx+\int_0^{\infty}x^if_X(x)\,dx
\end{aligned}
\tag{2.39}
$$

Si $i$ es impar, entonces $x^i=-(-x)^i$. Combinando esto con el hecho de que $f_X(x)=f_X(-x)$, vemos que

$$
\begin{aligned}
\int_{-\infty}^{0}x^if_X(x)\,dx &= \int_0^{\infty}(-x)^if_X(-x)\,dx \\
&= -\int_0^{\infty}x^if_X(x)\,dx
\end{aligned}
\tag{2.40}
$$

Entonces, para $i$ impar, el momento $i$-ésimo de la Ecuación (2.39) es cero. Vemos que todos los momentos impares de una variable aleatoria de media cero con una fdp simétrica son iguales a 0.

## 2.3 TRANSFORMACIONES DE VARIABLES ALEATORIAS

En esta sección analizaremos qué le sucede a la fdp de una VA cuando hacemos pasar la VA por alguna función. Supongamos que tenemos dos VA, $X$ e $Y$, relacionadas entre sí por las funciones monótonas² $g(\cdot)$ y $h(\cdot)$:

$$
\begin{aligned}
Y &= g(X) \\
X &= g^{-1}(Y) = h(Y)
\end{aligned}
\tag{2.41}
$$

Si conocemos la fdp de $X$ [$f_X(x)$], entonces podemos calcular la fdp de $Y$ [$f_Y(y)$] de la siguiente manera:

$$
\begin{aligned}
P(X\in[x,x+dx]) &= P(Y\in[y,y+dy]) \qquad (dx>0) \\
\int_x^{x+dx}f_X(z)\,dz &= \begin{cases} \int_y^{y+dy}f_Y(z)\,dz & \text{si } dy>0 \\ -\int_y^{y+dy}f_Y(z)\,dz & \text{si } dy<0 \end{cases} \\
f_X(x)\,dx &= f_Y(y)|dy| \\
f_Y(y) &= \left|\frac{dx}{dy}\right|f_X[h(y)] \\
&= |h'(y)|f_X[h(y)]
\end{aligned}
\tag{2.42}
$$

donde usamos, en el cálculo anterior, el supuesto de que $dx$ y $dy$ son pequeños.

*²Una función monótona es una función cuya pendiente es siempre no negativa o siempre no positiva. Si la pendiente es siempre no negativa, la función es monótonamente no decreciente. Si la pendiente es siempre positiva, la función es monótonamente creciente. Si la pendiente es siempre no positiva, la función es monótonamente no creciente. Si la pendiente es siempre negativa, la función es monótonamente decreciente.*

**EJEMPLO 2.4**

En este ejemplo, hallaremos la fdp de una función lineal de una VA gaussiana. Supongamos que $X\sim N(\bar x,\sigma_x^2)$ y $Y=g(X)=aX+b$, donde $a\neq 0$ y $b$ son constantes reales cualesquiera. Entonces

$$
\begin{aligned}
X &= h(Y) \\
&= (Y-b)/a \\
h'(y) &= 1/a \\
f_Y(y) &= |h'(y)|f_X[h(y)] \\
&= \left|\frac{1}{a}\right|\frac{1}{\sigma_X\sqrt{2\pi}}\exp\left\{\frac{-[(y-b)/a-\bar x]^2}{2\sigma_X^2}\right\} \\
&= \frac{1}{a\sigma_X\sqrt{2\pi}}\exp\left\{\frac{-[y-(a\bar x+b)]^2}{2a^2\sigma_X^2}\right\}
\end{aligned}
\tag{2.43}
$$

En otras palabras, la VA $Y$ es gaussiana, con una media y una varianza dadas por

$$
\begin{aligned}
\bar y &= a\bar x+b \\
\sigma_Y^2 &= a^2\sigma_X^2
\end{aligned}
\tag{2.44}
$$

Este importante ejemplo muestra que una transformación lineal de una VA gaussiana da como resultado una nueva VA gaussiana.

▽▽▽

**EJEMPLO 2.5**

Supongamos que hacemos pasar una VA gaussiana $X\sim N(0,\sigma_x^2)$ por la función no lineal $Y=g(X)=X^3$:

$$
\begin{aligned}
X &= h(Y) \\
&= Y^{1/3} \\
h'(y) &= \frac{y^{-2/3}}{3} \\
f_Y(y) &= |h'(y)|f_X[h(y)] \\
&= \frac{y^{-2/3}}{3}\frac{1}{\sigma_x\sqrt{2\pi}}\exp[-x^2/(2\sigma_x^2)] \\
&= \frac{y^{-2/3}}{3}\frac{1}{\sigma_x\sqrt{2\pi}}\exp[-y^{2/3}/(2\sigma_x^2)]
\end{aligned}
\tag{2.45}
$$

Vemos que la transformación no lineal $Y=X^3$ convierte una VA gaussiana en una VA no gaussiana. Puede verse que $f_Y(y)$ tiende a $\infty$ a medida que $y\to 0$. Sin embargo, el área bajo la curva de $f_Y(y)$ es igual a 1, ya que se trata de una fdp.

▽▽▽

En el caso más general de VA relacionadas por la función $Y=g(X)$, donde $g(\cdot)$ es una función no monótona, la fdp de $Y$ (evaluada en $y$) puede calcularse a partir de la fdp de $X$ como

$$f_Y(y) = \sum_i f_X(x_i)/|g'(x_i)| \tag{2.46}$$

donde los valores $x_i$ son las soluciones de la ecuación $y=g(x)$.

## 2.4 VARIABLES ALEATORIAS MÚLTIPLES

Ya definimos la función de distribución de probabilidad de una VA. Por ejemplo, si $X$ e $Y$ son VA, entonces sus funciones de distribución se definen como

$$
\begin{aligned}
F_X(x) &= P(X\leq x) \\
F_Y(y) &= P(Y\leq y)
\end{aligned}
\tag{2.47}
$$

Ahora definimos la probabilidad de que tanto $X\leq x$ como $Y\leq y$ como la función de distribución de probabilidad conjunta de $X$ e $Y$:

$$F_{XY}(x,y) = P(X\leq x,Y\leq y) \tag{2.48}$$

Si el significado resulta claro a partir del contexto, a menudo usamos la notación abreviada $F(x,y)$ para representar la función de distribución $F_{XY}(x,y)$. Algunas propiedades de la función de distribución conjunta son

$$
\begin{aligned}
F(x,y) &\in [0,1] \\
F(x,-\infty) = F(-\infty,y) &= 0 \\
F(\infty,\infty) &= 1 \\
F(a,c) &\leq F(b,d) \quad \text{si } a\leq b \text{ y } c\leq d \\
P(a<x\leq b,c<y\leq d) &= F(b,d)+F(a,c)-F(a,d)-F(b,c) \\
F(x,\infty) &= F(x) \\
F(\infty,y) &= F(y)
\end{aligned}
\tag{2.49}
$$

Nótese, a partir de las dos últimas propiedades, que la función de distribución de una VA puede obtenerse a partir de la función de distribución conjunta. Cuando la función de distribución de una única VA se obtiene de esta manera, se la llama función de distribución marginal.

La función de densidad de probabilidad conjunta se define como la siguiente derivada de la FDP conjunta:

$$f_{XY}(x,y) = \frac{\partial^2F_{XY}(x,y)}{\partial x\partial y} \tag{2.50}$$

Como antes, a menudo usamos la notación abreviada $f(x,y)$ para representar la función de densidad $f_{XY}(x,y)$. Algunas propiedades de la fdp conjunta que pueden obtenerse a partir de esta definición son

$$
\begin{aligned}
F(x,y) &= \int_{-\infty}^{x}\int_{-\infty}^{y}f(z_1,z_2)\,dz_1\,dz_2 \\
f(x,y) &\geq 0 \\
\int_{-\infty}^{\infty}\int_{-\infty}^{\infty}f(x,y)\,dx\,dy &= 1 \\
P(a<x\leq b,c<y\leq d) &= \int_c^d\int_a^bf(x,y)\,dx\,dy \\
f(x) &= \int_{-\infty}^{\infty}f(x,y)\,dy \\
f(y) &= \int_{-\infty}^{\infty}f(x,y)\,dx
\end{aligned}
\tag{2.51}
$$

Nótese, a partir de las dos últimas propiedades, que la función de densidad de una VA puede obtenerse a partir de la función de densidad conjunta. Cuando la función de densidad de una única VA se obtiene de esta manera, se la llama función de densidad marginal. Calcular el valor esperado de una función $g(\cdot,\cdot)$ de dos VA es similar a calcular el valor esperado de una función de una única VA:

$$E[g(x,y)] = \int_{-\infty}^{\infty}\int_{-\infty}^{\infty}g(x,y)f(x,y)\,dx\,dy \tag{2.52}$$

### 2.4.1 Independencia estadística

Recordemos, de la Sección 2.1, que dos eventos son independientes si la ocurrencia de uno de ellos no tiene efecto sobre la probabilidad de ocurrencia del otro. Extendemos esto para decir que las VA $X$ e $Y$ son independientes si satisfacen la siguiente relación:

$$P(X\leq x,Y\leq y) = P(X\leq x)P(Y\leq y) \qquad \text{para todo } x,y \tag{2.53}$$

A partir de nuestra definición de las funciones de distribución y densidad conjuntas, vemos que esto implica

$$
\begin{aligned}
F_{XY}(x,y) &= F_X(x)F_Y(y) \\
f_{XY}(x,y) &= f_X(x)f_Y(y)
\end{aligned}
\tag{2.54}
$$

El teorema central del límite establece que la suma de VA independientes tiende hacia una VA gaussiana, sin importar cuál sea la fdp de las VA individuales que contribuyen a la suma. Por eso tantas VA de la naturaleza parecen tener una distribución gaussiana. Muchas VA de la naturaleza son, en realidad, la suma de muchas VA individuales e independientes. Por ejemplo, la temperatura máxima de un día dado, en un lugar dado, tiende a seguir una distribución gaussiana. Esto se debe a que la temperatura máxima está afectada por las nubes, la precipitación, el viento, la presión atmosférica, la humedad, y otros factores. Cada uno de estos factores está, a su vez, determinado por otros factores aleatorios. La combinación de muchas variables aleatorias independientes determina la temperatura máxima, que tiene una fdp gaussiana.

Definimos la covarianza de dos VA escalares $X$ e $Y$ como

$$
\begin{aligned}
C_{XY} &= E[(X-\bar X)(Y-\bar Y)] \\
&= E(XY)-\bar X\bar Y
\end{aligned}
\tag{2.55}
$$

Definimos el coeficiente de correlación de dos VA escalares $X$ e $Y$ como

$$\rho = \frac{C_{XY}}{\sigma_x\sigma_y} \tag{2.56}$$

El coeficiente de correlación es una medida normalizada de la independencia entre dos VA $X$ e $Y$. Si $X$ e $Y$ son independientes, entonces $\rho=0$ (aunque la recíproca no es necesariamente cierta). Si $Y$ es una función lineal de $X$, entonces $\rho=\pm 1$ (véase el Problema 2.9).

Definimos la correlación de dos VA escalares $X$ e $Y$ como

$$R_{XY} = E(XY) \tag{2.57}$$

Se dice que dos VA son no correlacionadas si $R_{XY}=E(X)E(Y)$.

A partir de la definición de independencia, vemos que si dos VA son independientes, entonces también son no correlacionadas. La independencia implica que no estén correlacionadas, pero que no estén correlacionadas no implica necesariamente independencia. Sin embargo, en el caso especial en que dos VA son ambas gaussianas y no están correlacionadas, se sigue que también son independientes.

Se dice que dos VA son ortogonales si $R_{XY}=0$. Si dos VA son no correlacionadas, entonces son ortogonales únicamente si al menos una de ellas tiene media cero. Si dos VA son ortogonales, entonces pueden estar correlacionadas o no.

**EJEMPLO 2.6**

Dos tiradas de los dados están representadas por las VA $X$ e $Y$. Las dos VA son independientes porque una tirada del dado no tiene ningún efecto sobre una segunda tirada del dado. Cada tirada del dado tiene igual probabilidad ($1/6$) de ser un 1, 2, 3, 4, 5 o 6. Por lo tanto,

$$
\begin{aligned}
E(X) = E(Y) &= \frac{1+2+3+4+5+6}{6} \\
&= 3{,}5
\end{aligned}
\tag{2.58}
$$

Hay 36 combinaciones posibles de las dos tiradas del dado. Podríamos obtener la combinación $(1,1)$, $(1,2)$, y así sucesivamente. Cada una de estas 36 combinaciones tiene igual probabilidad ($1/36$). Por lo tanto, la correlación entre $X$ e $Y$ es

$$
\begin{aligned}
R_{XY}=E(XY) &= \frac{1}{36}\sum_{i=1}^{6}\sum_{j=1}^{6}ij \\
&= 12{,}25 \\
&= E(X)E(Y)
\end{aligned}
\tag{2.59}
$$

Dado que $E(XY)=E(X)E(Y)$, vemos que $X$ e $Y$ no están correlacionadas. Sin embargo, $R_{XY}\neq 0$, por lo que $X$ e $Y$ no son ortogonales.

▽▽▽

**EJEMPLO 2.7**

Una máquina tragamonedas está arreglada de modo que en el primer giro $X$ se obtiene 1 o $-1$ con igual probabilidad, y en el segundo giro $Y$ se obtiene el número opuesto. Tenemos igual probabilidad de obtener los resultados $(X,Y)$ de $(1,-1)$ y $(-1,1)$. Las dos VA son dependientes porque la realización de $Y$ depende de la realización de $X$. También vemos que

$$
\begin{aligned}
E(X) &= 0 \\
E(Y) &= 0 \\
E(XY) &= \frac{(1)(-1)+(-1)(1)}{2} \\
&= -1
\end{aligned}
\tag{2.60}
$$

Vemos que $X$ e $Y$ están correlacionadas porque $E(XY)\neq E(X)E(Y)$. También vemos que $X$ e $Y$ no son ortogonales porque $E(XY)\neq 0$.

▽▽▽

**EJEMPLO 2.8**

Una máquina tragamonedas está arreglada de modo que en el primer giro $X$ se obtiene $-1$, 0 o $+1$ con igual probabilidad. En el segundo giro $Y$ se obtiene 1 si $X=0$, y 0 si $X\neq 0$. Las dos VA son dependientes porque la realización de $Y$ depende de la realización de $X$. También vemos que

$$
\begin{aligned}
E(X) &= \frac{-1+0+1}{3} \\
&= 0 \\
E(Y) &= \frac{0+1+0}{3} \\
&= 1/3 \\
E(XY) &= \frac{(-1)(0)+(0)(1)+(1)(0)}{3} \\
&= 0
\end{aligned}
\tag{2.61}
$$

Vemos que $X$ e $Y$ no están correlacionadas porque $E(XY)=E(X)E(Y)$. También vemos que $X$ e $Y$ son ortogonales porque $E(XY)=0$. Este ejemplo ilustra el hecho de que la ausencia de correlación no implica necesariamente independencia.

▽▽▽

**EJEMPLO 2.9**

Supongamos que $x$ e $y$ son VA independientes, y que la VA $z$ se calcula como $z=g(x)+h(y)$. En este ejemplo calcularemos la media de $z$:

$$
\begin{aligned}
E(z) &= E[g(x)+h(y)] \\
&= \int\int[g(x)+h(y)]f(x,y)\,dx\,dy \\
&= \int\int g(x)f(x)f(y)\,dx\,dy+\int\int h(y)f(x)f(y)\,dx\,dy \\
&= \int g(x)f(x)\,dx\int f(y)\,dy+\int h(y)f(y)\,dy\int f(x)\,dx \\
&= E[g(x)](1)+E[h(y)](1) \\
&= E[g(x)]+E[h(y)]
\end{aligned}
\tag{2.62}
$$

Como caso particular de este ejemplo, vemos que la media de la suma de dos VA independientes es igual a la suma de sus medias. Es decir,

$$E(x+y) = E(x)+E(y) \qquad \text{si } x \text{ e } y \text{ son independientes} \tag{2.63}$$

▽▽▽

**EJEMPLO 2.10**

Supongamos que tiramos un dado dos veces. ¿Cuál es el valor esperado de la suma de los dos resultados? Usamos $X$ e $Y$ para referirnos a las dos tiradas del dado, y usamos $Z$ para referirnos a la suma de los dos resultados. Por lo tanto, $Z=X+Y$. Dado que $X$ e $Y$ son independientes, tenemos

$$
\begin{aligned}
E(Z) &= E(X)+E(Y) \\
&= 3{,}5+3{,}5 \\
&= 7
\end{aligned}
\tag{2.64}
$$

▽▽▽

**EJEMPLO 2.11**

Consideremos el circuito de la Figura 2.4. La tensión de entrada $V$ está uniformemente distribuida en $[-1,1]$. La tensión $V$ tiene unidades de voltios, y las dos corrientes tienen unidades de amperios.

$$
\begin{aligned}
I_1 &= \begin{cases} 0 & \text{si } V>0 \\ V & \text{si } V\leq 0 \end{cases} \\
I_2 &= \begin{cases} V & \text{si } V\geq 0 \\ 0 & \text{si } V<0 \end{cases}
\end{aligned}
\tag{2.65}
$$

Vemos que $I_1$ está uniformemente distribuida en $[-1,0]$ e $I_2$ está uniformemente distribuida en $[0,1]$. Las VA $V$, $I_1$ e $I_2$ tienen valores esperados

$$
\begin{aligned}
E(V) &= 0 \\
E(I_1) &= -1/2 \\
E(I_2) &= 1/2
\end{aligned}
\tag{2.66}
$$

Las VA $I_1$ e $I_2$ no son independientes porque están relacionadas entre sí; si $I_2\neq 0$ entonces $I_1=0$, y si $I_1\neq 0$ entonces $I_2=0$. Dado que $I_1$ o $I_2$ es igual a 0 en todo instante, $I_1I_2=0$ y $E(I_1I_2)=0$. Por lo tanto, $I_1$ e $I_2$ son ortogonales. Dado que $E(I_1)E(I_2)=-1/4$, vemos que $E(I_1I_2)\neq E(I_1)E(I_2)$, por lo que $I_1$ e $I_2$ están correlacionadas.

**Figura 2.4.** Circuito para el Ejemplo 2.11. *(Diagrama no reproducido: fuente de tensión $V$ conectada a dos ramas en paralelo, cada una con un resistor de $1\,\Omega$ en serie con un diodo, por las que circulan las corrientes $I_1$ e $I_2$ en sentidos opuestos.)*

▽▽▽

### 2.4.2 Estadística multivariante

El análisis de la subsección anterior puede generalizarse para VA que son vectores. En este caso, las cantidades definidas anteriormente se convierten en vectores y matrices. Dadas una VA $X$ de $n$ elementos y una VA $Y$ de $m$ elementos (suponiendo que tanto $X$ como $Y$ son vectores columna), su correlación se define como

$$
\begin{aligned}
R_{XY} &= E(XY^T) \\
&= \begin{bmatrix}E(X_1Y_1) & \cdots & E(X_1Y_m) \\ \vdots & & \vdots \\ E(X_nY_1) & \cdots & E(X_nY_m)\end{bmatrix}
\end{aligned}
\tag{2.67}
$$

Su covarianza se define como

$$
\begin{aligned}
C_{XY} &= E[(X-\bar X)(Y-\bar Y)^T] \\
&= E(XY^T)-\bar X\bar Y^T
\end{aligned}
\tag{2.68}
$$

La autocorrelación de la VA $X$ de $n$ elementos se define como

$$
\begin{aligned}
R_X &= E[XX^T] \\
&= \begin{bmatrix}E[X_1^2] & \cdots & E[X_1X_n] \\ \vdots & & \vdots \\ E[X_nX_1] & \cdots & E[X_n^2]\end{bmatrix}
\end{aligned}
\tag{2.69}
$$

Nótese que $E(X_iX_j)=E(X_jX_i)$, por lo que $R_X=R_X^T$. Una matriz de autocorrelación siempre es simétrica. Nótese también que, para cualquier vector columna $z$ de $n$ elementos, tenemos

$$
\begin{aligned}
z^TR_Xz &= z^TE[XX^T]z \\
&= E[z^TXX^Tz] \\
&= E[(z^TX)^2] \\
&\geq 0
\end{aligned}
\tag{2.70}
$$

Entonces, una matriz de autocorrelación siempre es semidefinida positiva.

La autocovarianza de la VA $X$ de $n$ elementos se define como

$$
\begin{aligned}
C_X &= E[(X-\bar X)(X-\bar X)^T] \\
&= \begin{bmatrix}E[(X_1-\bar X_1)^2] & \cdots & E[(X_1-\bar X_1)(X_n-\bar X_n)] \\ \vdots & & \vdots \\ E[(X_n-\bar X_n)(X_1-\bar X_1)] & \cdots & E[(X_n-\bar X_n)^2]\end{bmatrix} \\
&= \begin{bmatrix}\sigma_1^2 & \cdots & \sigma_{1n} \\ \vdots & & \vdots \\ \sigma_{n1} & \cdots & \sigma_n^2\end{bmatrix}
\end{aligned}
\tag{2.71}
$$

Nótese que $\sigma_{ij}=\sigma_{ji}$, por lo que $C_X=C_X^T$. Una matriz de autocovarianza siempre es simétrica. Nótese también que, para cualquier vector columna $z$ de $n$ elementos, tenemos

$$
\begin{aligned}
z^TC_Xz &= z^TE[(X-\bar X)(X-\bar X)^T]z \\
&= E[z^T(X-\bar X)(X-\bar X)^Tz] \\
&= E[(z^T(X-\bar X))^2] \\
&\geq 0
\end{aligned}
\tag{2.72}
$$

Entonces, una matriz de autocovarianza siempre es semidefinida positiva.

Una VA $X$ de $n$ elementos es gaussiana (normal)³ si

$$\text{fdp}(X) = \frac{1}{(2\pi)^{n/2}|C_X|^{1/2}}\exp\left[\frac{-1}{2}(X-\bar X)^TC_X^{-1}(X-\bar X)\right] \tag{2.73}$$

Consideremos ahora una VA gaussiana $X$ que sufre una transformación lineal:

$$
\begin{aligned}
Y &= g(X) \\
&= AX+b
\end{aligned}
\tag{2.74}
$$

donde $A$ es una matriz constante de $n\times n$, y $b$ es un vector constante de $n$ elementos. Si $A$ es invertible, entonces

$$
\begin{aligned}
X &= h(Y) \\
&= A^{-1}Y-A^{-1}b
\end{aligned}
\tag{2.75}
$$

A partir de la Ecuación (2.42) obtenemos

$$
\begin{aligned}
f_Y(y) &= |h'(y)|f_X[h(y)] \\
&= |A^{-1}|\frac{1}{(2\pi)^{n/2}|C_X|^{1/2}}\exp\left[\frac{-1}{2}(A^{-1}y-A^{-1}b-\bar x)^TC_X^{-1}(\cdots)\right] \\
&= |A^{-1}|\frac{1}{(2\pi)^{n/2}|C_X|^{1/2}}\times \\
&\quad \exp\left[\frac{-1}{2}(A^{-1}y-A^{-1}b-A^{-1}A\bar x)^TC_X^{-1}(\cdots)\right] \\
&= \frac{1}{(2\pi)^{n/2}|A||C_X|^{1/2}}\times \\
&\quad \exp\left[\frac{-1}{2}(A^{-1}y-A^{-1}\bar y)^TC_X^{-1}(A^{-1}y-A^{-1}\bar y)\right] \\
&= \frac{1}{(2\pi)^{n/2}|A|^{1/2}|C_X|^{1/2}|A^T|^{1/2}}\times \\
&\quad \exp\left[\frac{-1}{2}(y-\bar y)^TA^{-T}C_X^{-1}A^{-1}(y-\bar y)\right] \\
&= \frac{1}{(2\pi)^{n/2}|AC_XA^T|^{1/2}}\exp\left[\frac{-1}{2}(y-\bar y)^T(AC_XA^T)^{-1}(y-\bar y)\right] \\
y &\sim N(A\bar x+b,AC_XA^T)
\end{aligned}
\tag{2.76}
$$

Esto muestra que la normalidad se preserva en las transformaciones lineales de vectores aleatorios (así como se preserva en las transformaciones lineales de escalares aleatorios, como vimos en el Ejemplo 2.4).

*³Francis Edgeworth (1845-1926), un economista y matemático irlandés, fue el primero en ofrecer una descripción y un estudio generales de la distribución de probabilidad gaussiana multivariante, en 1892 [Sor80].*

## 2.5 PROCESOS ESTOCÁSTICOS

Un proceso estocástico, también llamado proceso aleatorio, es una generalización muy simple del concepto de VA. Un proceso estocástico $X(t)$ es una VA $X$ que cambia con el tiempo.⁴ Un proceso estocástico puede ser de uno de cuatro tipos.

Si la VA en cada instante es continua y el tiempo es continuo, entonces $X(t)$ es un proceso aleatorio continuo. Por ejemplo, la temperatura en cada momento del día es un proceso aleatorio continuo, porque tanto la temperatura como el tiempo son continuos.

Si la VA en cada instante es discreta y el tiempo es continuo, entonces $X(t)$ es un proceso aleatorio discreto. Por ejemplo, la cantidad de personas en un edificio dado en cada momento del día es un proceso aleatorio discreto, porque la cantidad de personas es una variable discreta y el tiempo es continuo.

Si la VA en cada instante es continua y el tiempo es discreto, entonces $X(t)$ es una secuencia aleatoria continua. Por ejemplo, la temperatura máxima de cada día es una secuencia aleatoria continua, porque la temperatura es continua pero el tiempo es discreto (día uno, día dos, etc.).

Si la VA en cada instante es discreta y el tiempo es discreto, entonces $X(t)$ es una secuencia aleatoria discreta. Por ejemplo, la mayor cantidad de personas en un edificio dado cada día es una secuencia aleatoria discreta, porque la cantidad de personas es una variable discreta y el tiempo también es discreto.

Dado que un proceso estocástico es una VA que cambia con el tiempo, tiene una función de distribución y una función de densidad que son funciones del tiempo. La FDP de $X(t)$ es

$$F_X(x,t) = P(X(t)\leq x) \tag{2.77}$$

Si $X(t)$ es un vector aleatorio, entonces la desigualdad anterior es una desigualdad elemento a elemento. Por ejemplo, si $X(t)$ tiene $n$ elementos, entonces

$$F_X(x,t) = P[X_1(t)\leq x_1 \text{ y } \cdots X_n(t)\leq x_n(t)] \tag{2.78}$$

La fdp de $X(t)$ es

$$f_X(x,t) = \frac{dF_X(x,t)}{dx} \tag{2.79}$$

Si $X(t)$ es un vector aleatorio, entonces la derivada anterior se toma una vez respecto de cada elemento de $x$. Por ejemplo, si $X(t)$ tiene $n$ elementos, entonces

$$f_X(x,t) = \frac{d^nF_X(x,t)}{dx_1\cdots dx_n} \tag{2.80}$$

La media y la covarianza de $X(t)$ también son funciones del tiempo:

*⁴En realidad, la variable independiente no tiene que ser necesariamente el tiempo; por ejemplo, podría ser la ubicación espacial o alguna otra cosa. Pero, típicamente, la variable independiente es el tiempo, y en este libro siempre será el tiempo.*

$$
\begin{aligned}
\bar x(t) &= \int_{-\infty}^{\infty}xf(x,t)\,dx \\
C_X(t) &= E\left\{[X(t)-\bar x(t)][X(t)-\bar x(t)]^T\right\} \\
&= \int_{-\infty}^{\infty}[x-\bar x(t)][x-\bar x(t)]^Tf(x,t)\,dx
\end{aligned}
\tag{2.81}
$$

Nótese que $X(t)$ en dos instantes distintos ($t_1$ y $t_2$) constituyen dos variables aleatorias distintas [$X(t_1)$ y $X(t_2)$]. Por lo tanto, podemos hablar de las funciones de distribución conjunta y de densidad conjunta de $X(t_1)$ y $X(t_2)$. A estas se las llama la función de distribución de segundo orden y la función de densidad de segundo orden:

$$
\begin{aligned}
F(x_1,x_2,t_1,t_2) &= P(X(t_1)\leq x_1,X(t_2)\leq x_2) \\
f(x_1,x_2,t_1,t_2) &= \frac{\partial^2F(x_1,x_2,t_1,t_2)}{\partial x_1\partial x_2}
\end{aligned}
\tag{2.82}
$$

Como se explicó antes, si $X(t)$ es un vector aleatorio de $n$ elementos, entonces la desigualdad que define $F(x_1,x_2,t_1,t_2)$ en realidad consiste en $2n$ desigualdades, y la derivada que define $f(x_1,x_2,t_1,t_2)$ en realidad consiste en $2n$ derivadas.

A la correlación entre las dos VA $X(t_1)$ y $X(t_2)$ se la llama la autocorrelación del proceso estocástico $X(t)$:

$$R_X(t_1,t_2) = E[X(t_1)X^T(t_2)] \tag{2.83}$$

La autocovarianza de un proceso estocástico se define como

$$C_X(t_1,t_2) = E\left\{[X(t_1)-\bar X(t_1)][X(t_2)-\bar X(t_2)]^T\right\} \tag{2.84}$$

Para algunos procesos estocásticos, la fdp no cambia con el tiempo. Por ejemplo, si tiramos una moneda diez veces, podemos considerar ese proceso como un proceso estocástico cuyas estadísticas son las mismas en cada uno de los diez instantes. En este caso, al proceso estocástico se lo llama estacionario en sentido estricto (ESE), o simplemente estacionario, para abreviar. En este caso, la media del proceso estocástico es constante respecto del tiempo, y la autocorrelación es una función de la diferencia de tiempo $t_2-t_1$ (no una función de los instantes absolutos):

$$
\begin{aligned}
E[X(t)] &= \bar x \\
E[X(t_1)X^T(t_2)] &= R_X(t_2-t_1)
\end{aligned}
\tag{2.85}
$$

Para algunos procesos estocásticos, estas dos condiciones son ciertas aun cuando la fdp sí cambia con el tiempo. A los procesos estocásticos para los cuales estas dos condiciones son ciertas se los llama estacionarios en sentido amplio (ESA). Un proceso estacionario es estacionario en sentido amplio, pero un proceso estacionario en sentido amplio puede ser estacionario o no. A partir de la definición de autocorrelación, puede demostrarse que, para un proceso estacionario en sentido amplio, se cumplen las siguientes propiedades:

$$
\begin{aligned}
R_X(0) &= E[X(t)X^T(t)] \\
R_X(-\tau) &= R_X(\tau)
\end{aligned}
\tag{2.86}
$$

Para procesos estocásticos escalares, puede demostrarse que

$$|R_X(\tau)| \leq R_X(0) \tag{2.87}$$

**EJEMPLO 2.12**

1. La temperatura máxima de cada día puede considerarse un proceso estocástico. Sin embargo, este proceso no es estacionario. La temperatura máxima en un día de julio podría ser una VA con una media de 100 grados Fahrenheit, pero la temperatura máxima en un día de diciembre podría tener una media de 30 grados. Este es un proceso estocástico cuyas estadísticas cambian con el tiempo, por lo que el proceso no es estacionario.

2. El ruido eléctrico de un voltímetro podría tener una media de cero y una varianza de un milivoltio. Si volvemos al día siguiente y medimos el ruido nuevamente, la media y la varianza podrían ser las mismas que antes. Si las estadísticas del ruido son las mismas todos los días, entonces el ruido eléctrico es un proceso estacionario. Nótese que, en la realidad, las estadísticas del ruido eventualmente cambiarán. Por ejemplo, después de algunas décadas el instrumento comenzará a degradarse y la media y la varianza del ruido eléctrico cambiarán. En este sentido, no existe tal cosa como un proceso aleatorio estacionario. Con el tiempo, el universo se congelará y todas las señales cambiarán. Pero, a efectos prácticos, si las estadísticas de un proceso aleatorio no cambian durante el intervalo de tiempo de interés, entonces consideramos que el proceso es estacionario.

3. El precio de cierre de mañana del índice industrial Dow Jones podría ser una VA con cierta media y varianza. Sin embargo, hace 100 años, el precio de cierre tenía una media mucho más baja. El precio de cierre del mercado de valores es una VA cuya media, en general, aumenta con el tiempo. Por lo tanto, el precio del mercado de valores es un proceso estocástico no estacionario.

▽▽▽

Supongamos que tenemos un proceso estocástico $X(t)$. Supongamos, además, que el proceso tiene una realización $x(t)$. El promedio temporal de $X(t)$ se denota como $A[X(t)]$, y la autocorrelación temporal de $X(t)$ se denota como $R[X(t)]$. Estas cantidades se definen, para procesos aleatorios en tiempo continuo, como

$$
\begin{aligned}
A[X(t)] &= \lim_{T\to\infty}\frac{1}{2T}\int_{-T}^{T}x(t)\,dt \\
R[X(t),\tau] &= A[X(t)X^T(t+\tau)]
\end{aligned}
\tag{2.88}
$$

Las definiciones para procesos aleatorios en tiempo discreto son extensiones directas de las definiciones en tiempo continuo.

Un proceso ergódico es un proceso aleatorio estacionario para el cual

$$
\begin{aligned}
A[X(t)] &= E(X) \\
R[X(t),\tau] &= R_X(\tau)
\end{aligned}
\tag{2.89}
$$

En el mundo real, a menudo estamos limitados a solo unas pocas realizaciones de un proceso estocástico. Por ejemplo, si medimos la fluctuación de la lectura de un voltímetro, en realidad solo estamos midiendo una única realización de un proceso estocástico. Podemos calcular el promedio temporal, la autocorrelación temporal, y otras estadísticas basadas en el tiempo de esa realización. Si el proceso aleatorio es ergódico, entonces podemos usar esos promedios temporales para estimar las estadísticas del proceso estocástico.

**EJEMPLO 2.13**

1. Supongamos que cada unidad de un instrumento eléctrico se fabrica con un pequeño sesgo aleatorio. ¿Es ergódico el ruido de esa instrumentación? Si medimos el ruido de un instrumento, entonces medimos su sesgo, que es igual a su media. Sin embargo, si medimos el ruido de otro instrumento, este podría tener una media diferente porque tiene un sesgo diferente. En otras palabras, no podemos obtener la media del proceso estocástico simplemente investigando un instrumento (es decir, una realización del proceso estocástico). Por lo tanto, el proceso estocástico no es ergódico.

2. Supongamos que cada unidad de un instrumento eléctrico se fabrica de manera idéntica, cada una con ruido gaussiano estacionario de media cero. ¿Es ergódico el ruido? En este caso, podríamos medir la media del proceso midiendo el ruido de muchos instrumentos distintos en un mismo instante, o midiendo el ruido de un solo instrumento durante un período prolongado de tiempo. Cualquiera de los dos experimentos nos informaría correctamente que la media del proceso estocástico es cero. Podríamos hallar las estadísticas del proceso estocástico usando todos los instrumentos en un único instante, o usando un único instrumento en muchos instantes distintos. Por lo tanto, el proceso estocástico es ergódico.

▽▽▽

Las definiciones de correlación y covarianza pueden extenderse a dos procesos estocásticos $X(t)$ e $Y(t)$. La correlación cruzada de $X(t)$ e $Y(t)$ se define como

$$R_{XY}(t_1,t_2) = E[X(t_1)Y^T(t_2)] \tag{2.90}$$

Se dice que dos procesos aleatorios $X(t)$ e $Y(t)$ son no correlacionados si $R_{XY}(t_1,t_2)=E[X(t_1)]E[Y^T(t_2)]$ para todo $t_1$ y $t_2$. La covarianza cruzada de $X(t)$ e $Y(t)$ se define como

$$C_{XY}(t_1,t_2) = E\left\{[X(t_1)-\bar X(t_1)][Y(t_2)-\bar Y(t_2)]^T\right\} \tag{2.91}$$

## 2.6 RUIDO BLANCO Y RUIDO COLOREADO

Si la VA $X(t_1)$ es independiente de la VA $X(t_2)$ para todo $t_1\neq t_2$, entonces a $X(t)$ se lo llama ruido blanco. En caso contrario, a $X(t)$ se lo llama ruido coloreado.

La blancura o el contenido de color de un proceso estocástico puede caracterizarse mediante su espectro de potencia. El espectro de potencia $S_X(\omega)$ de un proceso estocástico $X(t)$ estacionario en sentido amplio se define como la transformada de Fourier de la autocorrelación. La autocorrelación es la transformada inversa de Fourier del espectro de potencia.

$$
\begin{aligned}
S_X(\omega) &= \int_{-\infty}^{\infty}R_X(\tau)e^{-j\omega\tau}\,d\tau \\
R_X(\tau) &= \frac{1}{2\pi}\int_{-\infty}^{\infty}S_X(\omega)e^{j\omega\tau}\,d\omega
\end{aligned}
\tag{2.92}
$$

A estas ecuaciones se las llama las relaciones de Wiener–Khintchine, en honor a Norbert Wiener y Aleksandr Khinchin. Nótese que algunos autores colocan el término $1/2\pi$ del lado derecho de la definición de $S_X(\omega)$, en cuyo caso el término $1/2\pi$ del lado derecho de la definición de $R_X(\tau)$ desaparece. Al espectro de potencia a veces se lo denomina espectro de densidad de potencia, densidad espectral de potencia, o densidad de potencia. La potencia de un proceso estocástico estacionario en sentido amplio se define como

$$P_X = \frac{1}{2\pi}\int_{-\infty}^{\infty}S_X(\omega)\,d\omega \tag{2.93}$$

El espectro de potencia cruzada de dos procesos estocásticos $X(t)$ e $Y(t)$, estacionarios en sentido amplio, es la transformada de Fourier de la correlación cruzada:

$$
\begin{aligned}
S_{XY}(\omega) &= \int_{-\infty}^{\infty}R_{XY}(\tau)e^{-j\omega\tau}\,d\tau \\
R_{XY}(\tau) &= \frac{1}{2\pi}\int_{-\infty}^{\infty}S_{XY}(\omega)e^{j\omega\tau}\,d\omega
\end{aligned}
\tag{2.94}
$$

Existen definiciones similares para los procesos aleatorios en tiempo discreto. El espectro de potencia de un proceso aleatorio en tiempo discreto se define como

$$
\begin{aligned}
S_X(\omega) &= \sum_{k=-\infty}^{\infty}R_X(k)e^{-j\omega k} \qquad \omega\in[-\pi,\pi] \\
R_X(k) &= \frac{1}{2\pi}\int_{-\infty}^{\infty}S_X(\omega)e^{jk\omega}\,d\omega
\end{aligned}
\tag{2.95}
$$

A un proceso estocástico en tiempo discreto $X(t)$ se lo llama ruido blanco si

$$
\begin{aligned}
R_X(k) &= \begin{cases}\sigma^2 & \text{si } k=0 \\ 0 & \text{si } k\neq 0\end{cases} \\
&= \sigma^2\delta_k
\end{aligned}
\tag{2.96}
$$

donde $\delta_k$ es la función delta de Kronecker, definida como

$$
\delta_k = \begin{cases}1 & \text{si } k=0 \\ 0 & \text{si } k\neq 0\end{cases}
\tag{2.97}
$$

La definición de ruido blanco en tiempo discreto muestra que este no tiene ninguna correlación consigo mismo, salvo en el instante presente. Si $X(k)$ es un proceso de ruido blanco en tiempo discreto, entonces la VA $X(n)$ no está correlacionada con $X(m)$ a menos que $n=m$. Esto muestra que la potencia de un proceso de ruido blanco en tiempo discreto es igual en todas las frecuencias:

$$S_X(\omega) = R_X(0) \qquad \text{para todo } \omega\in[-\pi,\pi] \tag{2.98}$$

Para un proceso aleatorio en tiempo continuo, el ruido blanco se define de manera similar. El ruido blanco tiene igual potencia en todas las frecuencias (como la luz blanca):

$$S_X(\omega) = R_X(0) \qquad \text{para todo } \omega \tag{2.99}$$

Sustituyendo esta expresión de $S_X(\omega)$ en la Ecuación (2.92), vemos que, para el ruido blanco en tiempo continuo,

$$R_X(\tau) = R_X(0)\delta(\tau) \tag{2.100}$$

donde $\delta(\tau)$ es la función impulso en tiempo continuo. Es decir, $\delta(\tau)$ es una función que es cero en todas partes excepto en $\tau=0$; tiene un ancho de 0, una altura de $\infty$, y un área de 1. El ruido blanco en tiempo continuo no es algo que ocurra en el mundo real, porque tiene potencia infinita, como puede verse al comparar las Ecuaciones (2.93) y (2.99). Sin embargo, muchos procesos en tiempo continuo se aproximan al ruido blanco, y resultan útiles en los análisis matemáticos de señales y sistemas.

**EJEMPLO 2.14**

Supongamos que un proceso estocástico estacionario de media cero tiene la función de autocorrelación

$$R_X(\tau) = \sigma^2e^{-\beta|\tau|} \tag{2.101}$$

donde $\beta$ es un número real positivo. El espectro de potencia se calcula a partir de la Ecuación (2.92) como

$$
\begin{aligned}
S_X(\omega) &= \int_{-\infty}^{\infty}\sigma^2e^{-\beta|\tau|}e^{-j\omega\tau}\,d\tau \\
&= \int_{-\infty}^{0}\sigma^2e^{(\beta-j\omega)\tau}\,d\tau+\int_0^{\infty}\sigma^2e^{-(\beta+j\omega)\tau}\,d\tau \\
&= \frac{\sigma^2}{\beta-j\omega}+\frac{\sigma^2}{\beta+j\omega} \\
&= \frac{2\sigma^2\beta}{\omega^2+\beta^2}
\end{aligned}
\tag{2.102}
$$

La varianza del proceso estocástico se calcula como

$$
\begin{aligned}
E[X^2(t)] &= \frac{1}{2\pi}\int_{-\infty}^{\infty}\frac{2\sigma^2\beta}{\omega^2+\beta^2}\,d\omega \\
&= \frac{\sigma^2\beta}{\pi}\left[\frac{1}{\beta}\tan^{-1}\frac{\omega}{\beta}\right]_{-\infty}^{\infty} \\
&= \sigma^2 \\
&= R_X(0)
\end{aligned}
\tag{2.103}
$$

▽▽▽

## 2.7 SIMULACIÓN DE RUIDO CORRELACIONADO

En la investigación y los experimentos de filtrado óptimo, a menudo debemos simular ruido blanco correlacionado. Es decir, necesitamos crear vectores aleatorios cuyos elementos estén correlacionados entre sí según alguna matriz de covarianza predefinida. En esta sección presentaremos una manera de lograrlo.

Supongamos que queremos generar un vector aleatorio $w$ de $n$ elementos que tenga media cero y covarianza $Q$:

$$Q = \begin{bmatrix}\sigma_1^2 & \cdots & \sigma_{1n} \\ \vdots & & \vdots \\ \sigma_{1n} & \cdots & \sigma_n^2\end{bmatrix} \tag{2.104}$$

Dado que $Q$ es una matriz de covarianza, sabemos que todos sus autovalores son reales y no negativos. Por lo tanto, podemos denotar sus autovalores como $\mu_k^2$:

$$\lambda(Q) = \mu_k^2 \qquad (k=1,\ldots,n) \tag{2.105}$$

Supongamos que los autovectores de $Q$ resultan ser $d_1,\cdots,d_n$. Agrupemos los vectores $d_i$ para obtener una matriz $D$ de $n\times n$. Dado que $Q$ es simétrica, siempre podemos elegir los autovectores de modo que $D$ sea ortogonal, es decir, $D^{-1}=D^T$. Por lo tanto, obtenemos la descomposición en forma de Jordan de $Q$ como

$$Q = D\hat QD^T \tag{2.106}$$

donde $\hat Q$ es la matriz diagonal de los autovalores de $Q$. Es decir,

$$\hat Q = \text{diag}(\mu_1^2,\cdots,\mu_n^2) \tag{2.107}$$

Ahora definamos el vector aleatorio $v$ como $v=D^{-1}w$, de modo que $w=Dv$. Por lo tanto,

$$
\begin{aligned}
E(vv^T) &= E(D^Tww^TD) \\
&= D^TQD \\
&= \hat Q \\
&= \text{diag}(\mu_1^2,\cdots,\mu_n^2)
\end{aligned}
\tag{2.108}
$$

Esto muestra cómo podemos generar un vector aleatorio $w$ de $n$ elementos con una matriz de covarianza $Q$. El algoritmo está dado de la siguiente manera.

**Simulación de ruido correlacionado**

1. Halle los autovalores de $Q$, y denótelos como $\mu_1^2,\cdots,\mu_n^2$

2. Halle los autovectores de $Q$, y denótelos como $d_1,\cdots,d_n$, de modo que

$$
\begin{aligned}
D &= \begin{bmatrix}d_1 & \cdots & d_n\end{bmatrix} \\
D^{-1} &= D^T
\end{aligned}
\tag{2.109}
$$

3. Para $i=1,\cdots,n$, calcule la variable aleatoria $v_i=\mu_ir_i$, donde cada $r_i$ es un número aleatorio independiente con varianza 1 (varianza unitaria).

4. Fije $w=Dv$.

## 2.8 RESUMEN

En este capítulo repasamos los conceptos básicos de la probabilidad, las variables aleatorias, y los procesos estocásticos. La probabilidad de que ocurra algún evento se define de manera simple e intuitiva como el número de veces que ocurre el evento dividido por el número de oportunidades que tiene el evento de ocurrir. Una variable aleatoria (VA) es una variable cuyo valor no es cierto, sino que está gobernado por las leyes de la probabilidad. Por ejemplo, su puntaje en el examen de este capítulo no es determinístico, sino que es una variable aleatoria. Su puntaje real, después de que usted rinda el examen, será un número específico y determinístico. Pero antes de rendir el examen, usted no sabe qué puntaje va a obtener. Puede suponer que probablemente obtendrá entre el 80% y el 90% si tiene un buen entendimiento del material, pero su puntaje real estará determinado por eventos aleatorios tales como su salud, qué tan bien durmió la noche anterior, qué temas decide cubrir la instructora en el examen frente a qué temas estudió, cómo estuvo el tránsito camino a la escuela, el humor de la instructora cuando corrige el examen, y así sucesivamente. Un proceso estocástico es una variable aleatoria que cambia con el tiempo,
