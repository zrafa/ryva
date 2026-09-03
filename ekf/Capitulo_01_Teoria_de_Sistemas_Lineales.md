# Capítulo 1 — Teoría de sistemas lineales

> *Nota del traductor: traducción completa y fiel del Capítulo 1, "Linear Systems Theory", del libro* Optimal State Estimation *de Dan J. Simon (Wiley, 2006), con el mismo criterio usado en la traducción del Capítulo 13. Se conserva la numeración original de ecuaciones, definiciones, teoremas, ejemplos, figuras y tablas. Terminología nueva de este capítulo: autovalor/autovector (eigenvalue/eigenvector), rango (rank), traza (trace), gramiano (grammian), hermítica (hermitian), definida positiva/negativa. Los decimales se expresan con coma y los miles con punto. Las figuras no se reproducen gráficamente; se conserva su leyenda traducida. En un par de puntos donde el original en inglés presenta una inconsistencia matemática evidente y aislada (p. ej. una matriz sin el superíndice de transpuesta hermítica que exige la propia frase que la introduce), se tradujo la versión matemáticamente coherente; el resto del texto, incluida alguna referencia cruzada con nombre de variable inconsistente, se tradujo tal como aparece en el original.*

---

> Finalmente, hacemos algunos comentarios sobre por qué los sistemas *lineales* son tan importantes. La respuesta es simple: ¡porque podemos resolverlos!
>
> —Richard Feynman [Fey63, p. 25-4]

Este capítulo repasa algunos aspectos esenciales de la teoría de sistemas lineales. Este material suele cubrirse en un curso de sistemas lineales, que es una asignatura de posgrado de primer semestre en ingeniería eléctrica. La teoría de la estimación óptima de estado se apoya fuertemente en la teoría de matrices, incluido el cálculo matricial, por lo que la teoría de matrices se repasa en la Sección 1.1. La estimación óptima de estado puede aplicarse tanto a sistemas lineales como no lineales, aunque la estimación de estado es mucho más directa para los sistemas lineales. Los sistemas lineales se repasan brevemente en la Sección 1.2, y los sistemas no lineales se analizan en la Sección 1.3. Los sistemas en el espacio de estados pueden representarse en el dominio del tiempo continuo o en el del tiempo discreto. Los sistemas físicos suelen describirse en tiempo continuo, pero los algoritmos de control y de estimación de estado suelen implementarse en computadoras digitales. La Sección 1.4 analiza algunos métodos estándar para obtener una representación en tiempo discreto de un sistema en tiempo continuo. La Sección 1.5 explica cómo simular sistemas en tiempo continuo en una computadora digital. Las Secciones 1.6 y 1.7 tratan los conceptos estándar de estabilidad, controlabilidad y observabilidad de los sistemas lineales. Estos conceptos son necesarios para comprender parte del material sobre estimación óptima de estado que se presenta más adelante en el libro. Los estudiantes con una sólida

*Optimal State Estimation, primera edición.* Por Dan J. Simon — pág. 3
ISBN 0471708585 © 2006 John Wiley & Sons, Inc.

---

formación en teoría de sistemas lineales pueden omitir el material de este capítulo. Sin embargo, de todos modos sería útil al menos repasar este capítulo para consolidar los conceptos fundamentales de la estimación de estado antes de avanzar hacia los capítulos posteriores de este libro.

## 1.1 ÁLGEBRA MATRICIAL Y CÁLCULO MATRICIAL

En esta sección repasamos las matrices, el álgebra matricial y el cálculo matricial. Esto es necesario para comprender el resto del libro, ya que los algoritmos de estimación óptima de estado suelen formularse con matrices.

Un escalar es una cantidad individual. Por ejemplo, el número 2 es un escalar. El número $1+3j$ es un escalar (en este libro usamos $j$ para denotar la raíz cuadrada de $-1$). El número $\pi$ es un escalar.

Un vector está formado por escalares dispuestos en una fila o una columna. Por ejemplo, el vector

$$\begin{bmatrix} 1 & 3 & \pi \end{bmatrix} \tag{1.1}$$

es un vector de 3 elementos. A este vector se lo llama vector de $1 \times 3$ porque tiene 1 fila y 3 columnas. A este vector también se lo llama vector fila porque está dispuesto como una única fila. El vector

$$\begin{bmatrix} -2 \\ \pi^2 \\ j \\ 0 \end{bmatrix} \tag{1.2}$$

es un vector de 4 elementos. A este vector también se lo llama vector de $4\times 1$ porque tiene 4 filas y 1 columna. A este vector también se lo llama vector columna porque está dispuesto como una única columna. Nótese que un escalar puede considerarse un vector de 1 elemento; un escalar es un vector degenerado. (Esto es análogo a cómo un plano puede considerarse una figura tridimensional; un plano es una figura tridimensional degenerada.)

Una matriz está formada por escalares dispuestos en un rectángulo. Por ejemplo, la matriz

$$\begin{bmatrix} -2 & 3 \\ 0 & \pi^2 \\ j & 0 \end{bmatrix} \tag{1.3}$$

es una matriz de $3\times 2$ porque tiene 3 filas y 2 columnas. El número de filas y columnas de una matriz puede denominarse, en conjunto, la dimensión de la matriz. Por ejemplo, la dimensión de la matriz de la ecuación anterior es $3\times 2$. Nótese que un vector puede considerarse una matriz degenerada. Por ejemplo, la Ecuación (1.1) es una matriz de $1\times 3$. Un escalar también puede considerarse una matriz degenerada. Por ejemplo, el escalar 6 es una matriz de $1\times 1$.

El rango de una matriz se define como el número de filas linealmente independientes. Esto también es igual al número de columnas linealmente independientes. El rango de una matriz $A$ suele indicarse con la notación $\rho(A)$. El rango de una matriz siempre es menor o igual que el número de filas, y también es menor o igual que el número de columnas. Por ejemplo, la matriz

$$A = \begin{bmatrix} 1 & 2 \\ 2 & 4 \end{bmatrix} \tag{1.4}$$

tiene rango uno porque tiene solo una fila linealmente independiente; las dos filas son múltiplos entre sí. También tiene solo una columna linealmente independiente; las dos columnas son múltiplos entre sí. Por otro lado, la matriz

$$A = \begin{bmatrix} 1 & 3 \\ 2 & 4 \end{bmatrix} \tag{1.5}$$

tiene rango dos porque tiene dos filas linealmente independientes. Es decir, no existen escalares no nulos $c_1$ y $c_2$ tales que

$$c_1\begin{bmatrix}1 & 3\end{bmatrix} + c_2\begin{bmatrix}2 & 4\end{bmatrix} = \begin{bmatrix}0 & 0\end{bmatrix} \tag{1.6}$$

por lo que las dos filas son linealmente independientes. También tiene dos columnas linealmente independientes. Es decir, no existen escalares no nulos $c_1$ y $c_2$ tales que

$$c_1\begin{bmatrix}1\\2\end{bmatrix} + c_2\begin{bmatrix}3\\4\end{bmatrix} = \begin{bmatrix}0\\0\end{bmatrix} \tag{1.7}$$

por lo que las dos columnas son linealmente independientes. Una matriz cuyos elementos son todos ceros tiene rango cero. Una matriz de $n\times m$ cuyo rango es igual a $\min(n,m)$ se denomina de rango completo. La nulidad de una matriz $A$ de $n\times m$ es igual a $[m-\rho(A)]$.

La transpuesta de una matriz (o vector) puede obtenerse cambiando todas las filas por columnas, y todas las columnas por filas. La transpuesta de una matriz se indica con un superíndice $T$, como en $A^T$.¹ Por ejemplo, si $A$ es la matriz de $r\times n$

$$A = \begin{bmatrix} A_{11} & \cdots & A_{1n} \\ \vdots & & \vdots \\ A_{r1} & \cdots & A_{rn} \end{bmatrix} \tag{1.8}$$

entonces $A^T$ es la matriz de $n\times r$

$$A^T = \begin{bmatrix} A_{11} & \cdots & A_{r1} \\ \vdots & & \vdots \\ A_{1n} & \cdots & A_{rn} \end{bmatrix} \tag{1.9}$$

Nótese que usamos la notación $A_{ij}$ para indicar el escalar en la fila $i$ y la columna $j$ de la matriz $A$. Una matriz simétrica es aquella para la cual $A=A^T$.

La transpuesta hermítica de una matriz (o vector) es el conjugado complejo de la transpuesta, y se indica con un superíndice $H$, como en $A^H$. Por ejemplo, si

$$A = \begin{bmatrix} 1 & 2j & 3-j \\ 4j & 5+j & 1-3j \end{bmatrix} \tag{1.10}$$

entonces

$$A^H = \begin{bmatrix} 1 & -4j \\ -2j & 5-j \\ 3+j & 1+3j \end{bmatrix} \tag{1.11}$$

Una matriz hermítica es aquella para la cual $A=A^H$.

*¹Muchos artículos o libros indican la transpuesta con una prima, como en $A'$, o con una $t$ minúscula, como en $A^t$.*

### 1.1.1 Álgebra matricial

La suma y la resta de matrices se definen simplemente como la suma y la resta elemento a elemento. Por ejemplo,

$$\begin{bmatrix}1&2&3\\3&2&1\end{bmatrix} + \begin{bmatrix}0&4&1\\1&-1&-2\end{bmatrix} = \begin{bmatrix}1&6&4\\4&1&-1\end{bmatrix} \tag{1.12}$$

La suma $(A+B)$ y la diferencia $(A-B)$ están definidas únicamente si la dimensión de $A$ es igual a la dimensión de $B$.

Supongamos que $A$ es una matriz de $n\times r$ y $B$ es una matriz de $r\times p$. Entonces el producto de $A$ y $B$ se escribe como $C=AB$. Cada elemento de la matriz producto $C$ se calcula como

$$C_{ij} = \sum_{k=1}^{r} A_{ik}B_{kj} \qquad i=1,\ldots,n \quad j=1,\ldots,p \tag{1.13}$$

El producto matricial $AB$ está definido únicamente si el número de columnas de $A$ es igual al número de filas de $B$. Es importante notar que la multiplicación de matrices no es conmutativa. En general, $AB\neq BA$.

Supongamos que tenemos un vector $x$ de $n\times 1$. Podemos calcular el producto $x^Tx$, de $1\times 1$, y el producto $xx^T$, de $n\times n$, de la siguiente manera:

$$
\begin{aligned}
x^Tx &= \begin{bmatrix}x_1 & \cdots & x_n\end{bmatrix}\begin{bmatrix}x_1\\ \vdots \\ x_n\end{bmatrix} \\
&= x_1^2+\cdots+x_n^2 \\
xx^T &= \begin{bmatrix}x_1\\ \vdots\\ x_n\end{bmatrix}\begin{bmatrix}x_1 & \cdots & x_n\end{bmatrix} \\
&= \begin{bmatrix} x_1^2 & \cdots & x_1x_n \\ \vdots & \ddots & \vdots \\ x_nx_1 & \cdots & x_n^2 \end{bmatrix}
\end{aligned}
\tag{1.14}
$$

Supongamos que tenemos una matriz $H$ de $p\times n$ y una matriz $P$ de $n\times n$. Entonces $H^T$ es una matriz de $n\times p$, y podemos calcular el producto matricial $HPH^T$, de $p\times p$.

$$
\begin{aligned}
HPH^T &= \begin{bmatrix}H_{11}&\cdots&H_{1n}\\ \vdots&\ddots&\vdots\\ H_{p1}&\cdots&H_{pn}\end{bmatrix}\begin{bmatrix}P_{11}&\cdots&P_{1n}\\ \vdots&\ddots&\vdots\\ P_{n1}&\cdots&P_{nn}\end{bmatrix}\begin{bmatrix}H_{11}&\cdots&H_{p1}\\ \vdots&\ddots&\vdots\\ H_{1n}&\cdots&H_{pn}\end{bmatrix} \\
&= \begin{bmatrix} \sum_{j,k}H_{1j}P_{jk}H_{1k} & \cdots & \sum_{j,k}H_{1j}P_{jk}H_{pk} \\ \vdots & \ddots & \vdots \\ \sum_{j,k}H_{pj}P_{jk}H_{1k} & \cdots & \sum_{j,k}H_{pj}P_{jk}H_{pk} \end{bmatrix}
\end{aligned}
\tag{1.15}
$$

Esta matriz de sumas puede escribirse como la siguiente suma de matrices:

$$
\begin{aligned}
HPH^T &= \begin{bmatrix} H_{11}P_{11}H_{11} & \cdots & H_{11}P_{11}H_{p1} \\ \vdots & \ddots & \vdots \\ H_{p1}P_{11}H_{11} & \cdots & H_{p1}P_{11}H_{p1} \end{bmatrix} + \cdots + \\
&\quad \begin{bmatrix} H_{1n}P_{nn}H_{1n} & \cdots & H_{1n}P_{nn}H_{pn} \\ \vdots & \ddots & \vdots \\ H_{pn}P_{nn}H_{1n} & \cdots & H_{pn}P_{nn}H_{pn} \end{bmatrix} \\
&= H_1P_{11}H_1^T + \cdots + H_nP_{nn}H_n^T \\
&= \sum_{j,k}H_jP_{jk}H_k^T
\end{aligned}
\tag{1.16}
$$

donde hemos usado la notación de que $H_k$ es la $k$-ésima columna de $H$.

La división de matrices no está definida; no podemos dividir una matriz por otra matriz (a menos, por supuesto, que la matriz del denominador sea un escalar).

Una matriz identidad $I$ se define como una matriz cuadrada con unos en la diagonal y ceros en el resto de sus elementos. Por ejemplo, la matriz identidad de $3\times 3$ es igual a

$$I = \begin{bmatrix}1&0&0\\0&1&0\\0&0&1\end{bmatrix} \tag{1.17}$$

La matriz identidad tiene la propiedad de que $AI=A$ para cualquier matriz $A$, y $IA=A$ (siempre que las dimensiones de las matrices identidad sean compatibles con las de $A$). La matriz identidad de $1\times 1$ es igual al escalar 1.

El determinante de una matriz se define de manera inductiva para matrices cuadradas. El determinante de un escalar (es decir, de una matriz de $1\times 1$) es igual al escalar. Consideremos ahora una matriz $A$ de $n\times n$. Usemos la notación $A^{(i,j)}$ para denotar la matriz que se forma al eliminar la fila $i$ y la columna $j$ de $A$. El determinante de $A$ se define como

$$|A| = \sum_{j=1}^{n}(-1)^{i+j}A_{ij}|A^{(i,j)}| \tag{1.18}$$

para cualquier valor de $i\in[1,n]$. A esto se lo denomina el desarrollo de Laplace de $A$ a lo largo de su fila $i$. Vemos que el determinante de la matriz $A$ de $n\times n$ se define en términos de los determinantes de matrices de $(n-1)\times(n-1)$. De manera similar, los determinantes de las matrices de $(n-1)\times(n-1)$ se definen en términos de los determinantes de matrices de $(n-2)\times(n-2)$. Esto continúa hasta que los determinantes de las matrices de $2\times 2$ quedan definidos en términos de los determinantes de matrices de $1\times 1$, que son escalares. El determinante de $A$ también puede definirse como

$$|A| = \sum_{i=1}^{n}(-1)^{i+j}A_{ij}|A^{(i,j)}| \tag{1.19}$$

para cualquier valor de $j\in[1,n]$. A esto se lo denomina el desarrollo de Laplace de $A$ a lo largo de su columna $j$. Curiosamente, la Ecuación (1.18) (para cualquier valor de $i$) y la Ecuación (1.19) (para cualquier valor de $j$) dan resultados idénticos. A partir de la definición del determinante vemos que

$$
\begin{aligned}
\det[A_{11}] &= A_{11} \\
\det\begin{bmatrix}A_{11}&A_{12}\\A_{21}&A_{22}\end{bmatrix} &= A_{11}A_{22}-A_{12}A_{21} \\
\det\begin{bmatrix}A_{11}&A_{12}&A_{13}\\A_{21}&A_{22}&A_{23}\\A_{31}&A_{32}&A_{33}\end{bmatrix} &= A_{11}(A_{22}A_{33}-A_{23}A_{32}) - \\
&\quad A_{12}(A_{21}A_{33}-A_{23}A_{31}) + \\
&\quad A_{13}(A_{21}A_{32}-A_{22}A_{31})
\end{aligned}
\tag{1.20}
$$

Algunas propiedades interesantes de los determinantes son

$$|AB| = |A||B| \tag{1.21}$$

suponiendo que $A$ y $B$ son cuadradas y tienen la misma dimensión. Además,

$$|A| = \prod_{i=1}^{n}\lambda_i \tag{1.22}$$

donde $\lambda_i$ (los autovalores de $A$) se definen más adelante.

La inversa de una matriz $A$ se define como la matriz $A^{-1}$ tal que $AA^{-1}=A^{-1}A=I$. Una matriz no puede tener inversa a menos que sea cuadrada. Algunas matrices cuadradas no tienen inversa. Una matriz cuadrada que no tiene inversa se denomina singular o no invertible. En el caso escalar, el único número que no tiene inverso es el número 0. Pero en el caso matricial, hay muchas matrices que son singulares. Una matriz que sí tiene inversa se denomina no singular o invertible. Por ejemplo, nótese que

$$\begin{bmatrix}1&0\\2&3\end{bmatrix}\begin{bmatrix}1&0\\-2/3&1/3\end{bmatrix} = \begin{bmatrix}1&0\\0&1\end{bmatrix} \tag{1.23}$$

Por lo tanto, las dos matrices del lado izquierdo de la ecuación son inversas entre sí. La no singularidad de una matriz $A$ de $n\times n$ puede enunciarse de muchas maneras equivalentes, algunas de las cuales son las siguientes [Hor85]:

- $A$ es no singular.
- $A^{-1}$ existe.
- El rango de $A$ es igual a $n$.
- Las filas de $A$ son linealmente independientes.
- Las columnas de $A$ son linealmente independientes.
- $|A|\neq 0$.
- $Ax=b$ tiene una solución única $x$ para todo $b$.
- 0 no es un autovalor de $A$.

La traza de una matriz cuadrada se define como la suma de sus elementos diagonales:

$$\text{Tr}(A) = \sum_i A_{ii} \tag{1.24}$$

La traza de una matriz está definida únicamente si la matriz es cuadrada. La traza de una matriz de $1\times 1$ es igual a la traza de un escalar, que es igual al valor de ese escalar. Una propiedad interesante de la traza de una matriz cuadrada es

$$\text{Tr}(A) = \sum_i \lambda_i \tag{1.25}$$

Es decir, la traza de una matriz cuadrada es igual a la suma de sus autovalores. Algunas características interesantes y útiles de los productos de matrices son las siguientes:

$$
\begin{aligned}
(AB)^T &= B^TA^T \\
(AB)^{-1} &= B^{-1}A^{-1} \\
\text{Tr}(AB) &= \text{Tr}(BA)
\end{aligned}
\tag{1.26}
$$

Esto supone que las inversas existen, para la ecuación de la inversa, y que las dimensiones de las matrices son compatibles de modo que la multiplicación matricial esté definida. La transpuesta de un producto de matrices es igual al producto de las transpuestas en orden inverso. La inversa de un producto de matrices es igual al producto de las inversas en orden inverso. La traza de un producto de matrices es independiente del orden en que se multiplican las matrices.

La norma dos de un vector columna de números reales, también llamada norma euclídea, se define de la siguiente manera:

$$
\begin{aligned}
||x||_2 &= \sqrt{x^Tx} \\
&= \sqrt{x_1^2+\cdots+x_n^2}
\end{aligned}
\tag{1.27}
$$

A partir de (1.14) vemos que

$$xx^T = \begin{bmatrix}x_1^2 & \cdots & x_1x_n \\ \vdots & \ddots & \vdots \\ x_nx_1 & \cdots & x_n^2\end{bmatrix} \tag{1.28}$$

Al tomar la traza de esta matriz obtenemos

$$
\begin{aligned}
\text{Tr}(xx^T) &= x_1^2+\cdots+x_n^2 \\
&= ||x||_2^2
\end{aligned}
\tag{1.29}
$$

Una matriz $A$ de $n\times n$ tiene $n$ autovalores y $n$ autovectores. El escalar $\lambda$ es un autovalor de $A$, y el vector $x$ de $n\times 1$ es un autovector de $A$, si se cumple la siguiente ecuación:

$$Ax = \lambda x \tag{1.30}$$

A los autovalores y autovectores de una matriz, en conjunto, se los denomina los datos propios de la matriz.² Una matriz de $n\times n$ tiene exactamente $n$ autovalores, aunque algunos pueden estar repetidos. Esto es análogo a decir que una ecuación polinómica de grado $n$ tiene exactamente $n$ raíces, aunque algunas puedan estar repetidas. A partir de las definiciones anteriores de autovalores y autovectores podemos ver que

$$
\begin{aligned}
Ax &= \lambda x \\
A^2x &= A\lambda x \\
&= \lambda(Ax) \\
&= \lambda(\lambda x) \\
&= \lambda^2 x
\end{aligned}
\tag{1.31}
$$

Entonces, si $A$ tiene datos propios $(\lambda,x)$, $A^2$ tiene datos propios $(\lambda^2,x)$. Puede demostrarse que $A^{-1}$ existe si y solo si ninguno de los autovalores de $A$ es igual a 0. Si $A$ es simétrica, entonces todos sus autovalores son números reales.

*²A los datos propios también se los ha denominado de muchas otras maneras a lo largo de los años, entre ellas raíces características, raíces y vectores latentes, y números y vectores propios [Fad59].*

Una matriz simétrica $A$ de $n\times n$ puede caracterizarse como definida positiva, semidefinida positiva, definida negativa, semidefinida negativa, o indefinida. La matriz $A$ es:

- *Definida positiva* si $x^TAx>0$ para todo vector $x$ no nulo de $n\times 1$. Esto es equivalente a decir que todos los autovalores de $A$ son números reales positivos. Si $A$ es definida positiva, entonces $A^{-1}$ también es definida positiva.
- *Semidefinida positiva* si $x^TAx\geq 0$ para todo vector $x$ de $n\times 1$. Esto es equivalente a decir que todos los autovalores de $A$ son números reales no negativos. A las matrices semidefinidas positivas a veces se las llama definidas no negativas.
- *Definida negativa* si $x^TAx<0$ para todo vector $x$ no nulo de $n\times 1$. Esto es equivalente a decir que todos los autovalores de $A$ son números reales negativos. Si $A$ es definida negativa, entonces $A^{-1}$ también es definida negativa.
- *Semidefinida negativa* si $x^TAx\leq 0$ para todo vector $x$ de $n\times 1$. Esto es equivalente a decir que todos los autovalores de $A$ son números reales no positivos. A las matrices semidefinidas negativas a veces se las llama definidas no positivas.
- *Indefinida* si no encaja en ninguna de las cuatro categorías anteriores. Esto es equivalente a decir que algunos de sus autovalores son positivos y otros son negativos.

Algunos libros generalizan la idea de definición positiva y definición negativa para incluir matrices no simétricas.

La norma dos ponderada de un vector $x$ de $n\times 1$ se define como

$$||x||_Q^2 = \sqrt{x^TQx} \tag{1.32}$$

donde $Q$ debe ser una matriz definida positiva de $n\times n$. A la norma anterior también se la llama norma dos ponderada por $Q$ de $x$. Una cantidad de la forma $x^TQx$ se denomina cuadrática, en analogía con un término cuadrático en una ecuación escalar.

Los valores singulares $\sigma$ de una matriz $A$ se definen como

$$
\begin{aligned}
\sigma^2(A) &= \lambda(A^TA) \\
&= \lambda(AA^T)
\end{aligned}
\tag{1.33}
$$

Si $A$ es una matriz de $n\times m$, entonces tiene $\min(n,m)$ valores singulares. $AA^T$ tendrá $n$ autovalores, y $A^TA$ tendrá $m$ autovalores. Si $n>m$, entonces $AA^T$ tendrá los mismos autovalores que $A^TA$ más $(n-m)$ ceros adicionales. Estos ceros adicionales no se consideran valores singulares de $A$, porque $A$ siempre tiene $\min(n,m)$ valores singulares. Este hecho puede ayudar a reducir el esfuerzo al calcular los valores singulares. Por ejemplo, si $A$ es una matriz de $13\times 3$, entonces resulta mucho más fácil calcular los autovalores de la matriz $A^TA$, de $3\times 3$, que los de la matriz $AA^T$, de $13\times 13$. Cualquiera de los dos cálculos dará como resultado los mismos tres valores singulares.

### 1.1.2 El lema de inversión de matrices

En esta sección deduciremos el lema de inversión de matrices, que es una herramienta que usaremos muchas veces en este libro. También es una herramienta que resulta frecuentemente útil en otras áreas del control, la teoría de la estimación y el procesamiento de señales.

Supongamos que tenemos la matriz particionada $\begin{bmatrix}A&B\\C&D\end{bmatrix}$, donde $A$ y $D$ son matrices cuadradas invertibles, y las matrices $B$ y $C$ pueden ser cuadradas o no. Definimos las matrices $E$ y $F$ de la siguiente manera:

$$
\begin{aligned}
E &= D-CA^{-1}B \\
F &= A-BD^{-1}C
\end{aligned}
\tag{1.34}
$$

Supongamos que $E$ es invertible. Entonces podemos demostrar que

$$
\begin{aligned}
&\begin{bmatrix}A&B\\C&D\end{bmatrix}\begin{bmatrix}A^{-1}+A^{-1}BE^{-1}CA^{-1} & -A^{-1}BE^{-1} \\ -E^{-1}CA^{-1} & E^{-1}\end{bmatrix} \\
&= \begin{bmatrix} I+BE^{-1}CA^{-1}-BE^{-1}CA^{-1} & -BE^{-1}+BE^{-1} \\ CA^{-1}+CA^{-1}BE^{-1}CA^{-1}-DE^{-1}CA^{-1} & -CA^{-1}BE^{-1}+DE^{-1} \end{bmatrix} \\
&= \begin{bmatrix} I & 0 \\ CA^{-1}-(D-CA^{-1}B)E^{-1}CA^{-1} & (D-CA^{-1}B)E^{-1} \end{bmatrix} \\
&= \begin{bmatrix}I&0\\0&I\end{bmatrix}
\end{aligned}
\tag{1.35}
$$

Ahora supongamos que $F$ es invertible. Entonces podemos demostrar que

$$
\begin{aligned}
&\begin{bmatrix}A&B\\C&D\end{bmatrix}\begin{bmatrix}F^{-1} & -A^{-1}BE^{-1} \\ -D^{-1}CF^{-1} & E^{-1}\end{bmatrix} \\
&= \begin{bmatrix} AF^{-1}-BD^{-1}CF^{-1} & -BE^{-1}+BE^{-1} \\ CF^{-1}-CF^{-1} & -CA^{-1}BE^{-1}+DE^{-1} \end{bmatrix} \\
&= \begin{bmatrix} (A-BD^{-1}C)F^{-1} & 0 \\ 0 & (D-CA^{-1}B)E^{-1} \end{bmatrix} \\
&= \begin{bmatrix}I&0\\0&I\end{bmatrix}
\end{aligned}
\tag{1.36}
$$

Las Ecuaciones (1.35) y (1.36) son dos expresiones para la inversa de $\begin{bmatrix}A&B\\C&D\end{bmatrix}$. Dado que ambas expresiones son inversas de la misma matriz, deben ser iguales. Concluimos entonces que las particiones superiores izquierdas de las matrices son iguales, lo cual da

$$F^{-1} = A^{-1}+A^{-1}BE^{-1}CA^{-1} \tag{1.37}$$

Ahora podemos usar la definición de $F$ para obtener

$$(A-BD^{-1}C)^{-1} = A^{-1}+A^{-1}B(D-CA^{-1}B)^{-1}CA^{-1} \tag{1.38}$$

Esto se denomina el lema de inversión de matrices. También se lo conoce por otros nombres, como la fórmula de Sherman–Morrison, la identidad de Woodbury, y la fórmula de matrices modificadas. Una de sus primeras presentaciones fue en 1944, por William Duncan [Dun44], y Alston Householder desarrolló identidades similares [Hou53]. En [Hen81] se ofrece un relato de sus orígenes y variantes (por ejemplo, para $A$ singular). El lema de inversión de matrices suele enunciarse de maneras ligeramente distintas pero equivalentes. Por ejemplo,

$$(A+BD^{-1}C)^{-1} = A^{-1}-A^{-1}B(D+CA^{-1}B)^{-1}CA^{-1} \tag{1.39}$$

El lema de inversión de matrices a veces puede usarse para reducir el esfuerzo computacional de la inversión de matrices. Por ejemplo, supongamos que $A$ es de $n\times n$, $B$ es de $n\times p$, $C$ es de $p\times n$, $D$ es de $p\times p$, y $p<n$. Supongamos además que ya conocemos $A^{-1}$, y queremos sumarle alguna cantidad a $A$ y luego calcular la nueva inversa. Un cálculo directo de la nueva inversa requeriría una inversión de $n\times n$. Pero si la nueva matriz por invertir puede escribirse en la forma del lado izquierdo de la Ecuación (1.39), entonces podemos usar el lado derecho de la Ecuación (1.39) para calcular la nueva inversa, y el lado derecho de la Ecuación (1.39) requiere una inversión de $p\times p$ en lugar de una de $n\times n$ (ya que conocemos de antemano la inversa de la matriz $A$ original).

\vspace{\baselineskip}
**EJEMPLO 1.1**

En su firma de inversión, usted nota que en enero el índice de la Bolsa de Nueva York disminuyó un 2%, el índice de la Bolsa Americana de Valores aumentó un 1%, y el índice de la bolsa NASDAQ aumentó un 2%. Como resultado, los inversores aumentaron sus depósitos un 1%. El mes siguiente, los índices bursátiles cambiaron un −4%, 3% y 2%, respectivamente, y los depósitos de los inversores aumentaron un 2%. El mes posterior, los índices bursátiles cambiaron un −5%, 1% y 5%, respectivamente, y los depósitos de los inversores aumentaron un 2%. Usted sospecha que los cambios en la inversión $y$ pueden modelarse como $y=g_1x_1+g_2x_2+g_3x_3$, donde las variables $x_i$ son los cambios en los índices bursátiles, y las $g_i$ son constantes desconocidas. Para determinar las constantes $g_i$, necesita invertir la matriz

$$A = \begin{bmatrix}-2&1&2\\-4&3&2\\-5&1&5\end{bmatrix} \tag{1.40}$$

El resultado es

$$
\begin{aligned}
A^{-1} &= \frac{1}{6}\begin{bmatrix}13&-3&-4\\10&0&-4\\11&-3&-2\end{bmatrix} \\
g &= A^{-1}\begin{bmatrix}1\\2\\2\end{bmatrix} \\
&= \frac{1}{6}\begin{bmatrix}-1\\2\\1\end{bmatrix}
\end{aligned}
\tag{1.41}
$$

Esto le permite usar los cambios de los índices bursátiles para predecir los cambios de inversión del mes siguiente, lo cual le permite programar mejor los recursos de personal y de cómputo. Sin embargo, poco después descubre que el cambio del NASDAQ en el tercer mes fue en realidad del 6% y no del 5%. Esto significa que, para hallar las constantes $g_i$, necesita invertir la matriz

$$A' = \begin{bmatrix}-2&1&2\\-4&3&2\\-5&1&6\end{bmatrix} \tag{1.42}$$

Usted está cansado de invertir matrices, así que se pregunta si de alguna manera puede usar la inversa de $A$ (que ya calculó) para hallar la inversa de $A'$. Recordando el lema de inversión de matrices, se da cuenta de que $A'=A+BD^{-1}C$, donde

$$
\begin{aligned}
B &= \begin{bmatrix}0&0&1\end{bmatrix}^T \\
C &= \begin{bmatrix}0&0&1\end{bmatrix} \\
D &= 1
\end{aligned}
\tag{1.43}
$$

Por lo tanto, usted usa el lema de inversión de matrices para calcular

$$
\begin{aligned}
(A')^{-1} &= (A+BD^{-1}C)^{-1} \\
&= A^{-1}-A^{-1}B(D+CA^{-1}B)^{-1}CA^{-1}
\end{aligned}
\tag{1.44}
$$

El término $(D+CA^{-1}B)$ que hay que invertir en la ecuación anterior es un escalar, por lo que su inversión es simple. Esto da

$$
\begin{aligned}
(A')^{-1} &= \begin{bmatrix}4{,}00&1{,}00&-1{,}00\\3{,}50&-0{,}50&-1{,}00\\2{,}75&-0{,}75&-0{,}50\end{bmatrix} \\
g &= (A')^{-1}\begin{bmatrix}1\\2\\2\end{bmatrix} \\
&= \begin{bmatrix}0\\0{,}5\\0{,}25\end{bmatrix}
\end{aligned}
\tag{1.45}
$$

En este ejemplo, el uso del lema de inversión de matrices no es realmente necesario, porque $A'$ (la nueva matriz por invertir) es de apenas $3\times 3$. Sin embargo, con matrices más grandes, como matrices de $1.000\times 1.000$, el ahorro computacional que se logra al usar el lema de inversión de matrices podría ser significativo.

▽▽▽

Supongamos ahora que $A$, $B$, $C$ y $D$ son matrices, con $A$ y $D$ cuadradas. Entonces puede verse que

$$\begin{bmatrix}I&0\\-CA^{-1}&I\end{bmatrix}\begin{bmatrix}A&B\\C&D\end{bmatrix}\begin{bmatrix}I&-A^{-1}B\\0&I\end{bmatrix} = \begin{bmatrix}A&0\\0&D-CA^{-1}B\end{bmatrix} \tag{1.46}$$

Esto significa que

$$\begin{vmatrix}A&B\\C&D\end{vmatrix} = |A||D-CA^{-1}B| \tag{1.47}$$

De manera similar, puede demostrarse que

$$\begin{vmatrix}A&B\\C&D\end{vmatrix} = |D||A-BD^{-1}C| \tag{1.48}$$

A estas fórmulas se las llama reglas del producto para determinantes. Fueron presentadas por primera vez por el matemático de origen ruso Issai Schur, en un artículo en alemán [Sch17] que fue reeditado en inglés en [Sch86].

### 1.1.3 Cálculo matricial

En nuestro primer curso de cálculo, aprendimos la matemática de las derivadas y las integrales, y cómo aplicar esos conceptos a escalares. También podemos aplicar la matemática del cálculo a vectores y matrices. Algunos aspectos del cálculo matricial son idénticos a los del cálculo escalar, pero algunos conceptos del cálculo escalar deben extenderse para poder deducir fórmulas de cálculo matricial.

Como la intuición nos haría suponer, la derivada temporal de una matriz es simplemente igual a la matriz de las derivadas temporales de los elementos individuales de la matriz. Asimismo, la integral de una matriz es igual a la matriz de las integrales de los elementos individuales de la matriz. En otras palabras, suponiendo que $A$ es una matriz de $m\times n$, tenemos

$$
\begin{aligned}
\dot A(t) &= \begin{bmatrix}\dot A_{11}(t) & \cdots & \dot A_{1n}(t) \\ \vdots & \ddots & \vdots \\ \dot A_{n1}(t) & \cdots & \dot A_{nn}(t)\end{bmatrix} \\
\int A(t)\,dt &= \begin{bmatrix}\int A_{11}(t)\,dt & \cdots & \int A_{1n}(t)\,dt \\ \vdots & \ddots & \vdots \\ \int A_{n1}(t)\,dt & \cdots & \int A_{nn}(t)\,dt\end{bmatrix}
\end{aligned}
\tag{1.49}
$$

A continuación calcularemos la derivada temporal de la inversa de una matriz. Supongamos que la matriz $A(t)$, que denotaremos como $A$, tiene elementos que son funciones del tiempo. Sabemos que $AA^{-1}=I$; es decir, $AA^{-1}$ es una matriz constante y, por lo tanto, tiene una derivada temporal igual a cero. Pero la derivada temporal de $AA^{-1}$ puede calcularse como

$$\frac{d}{dt}(AA^{-1}) = \dot AA^{-1}+A\frac{d}{dt}(A^{-1}) \tag{1.50}$$

Dado que esto es cero, podemos despejar $d(A^{-1})/dt$ como

$$\frac{d}{dt}(A^{-1}) = -A^{-1}\dot AA^{-1} \tag{1.51}$$

Nótese que, para el caso especial de un $A$ escalar, esto se reduce a la conocida ecuación

$$
\begin{aligned}
\frac{d}{dt}(1/A) &= \frac{\partial(1/A)}{\partial A}\frac{dA}{dt} \\
&= -\dot A/A^2
\end{aligned}
\tag{1.52}
$$

Supongamos ahora que $x$ es un vector de $n\times 1$ y que $f(x)$ es una función escalar de los elementos de $x$. Entonces

$$\frac{\partial f}{\partial x} = \begin{bmatrix}\partial f/\partial x_1 & \cdots & \partial f/\partial x_n\end{bmatrix} \tag{1.53}$$

Aunque $x$ es un vector columna, $\partial f/\partial x$ es un vector fila. La recíproca también es cierta: si $x$ es un vector fila, entonces $\partial f/\partial x$ es un vector columna. Nótese que algunos autores definen esto al revés. Es decir, sostienen que si $x$ es un vector columna, entonces $\partial f/\partial x$ también es un vector columna. No existe una convención aceptada para la definición de la derivada parcial de un escalar respecto de un vector. En realidad no importa qué definición usemos, siempre que seamos consistentes. En este libro usaremos la convención descrita por la Ecuación (1.53).

Supongamos ahora que $A$ es una matriz de $m\times n$ y que $f(A)$ es un escalar. Entonces la derivada parcial de un escalar respecto de una matriz puede calcularse de la siguiente manera:

$$\frac{\partial f}{\partial A} = \begin{bmatrix}\partial f/\partial A_{11} & \cdots & \partial f/\partial A_{1n} \\ \vdots & \ddots & \vdots \\ \partial f/\partial A_{m1} & \cdots & \partial f/\partial A_{mn}\end{bmatrix} \tag{1.54}$$

Con estas definiciones podemos calcular la derivada parcial del producto punto de dos vectores. Supongamos que $x$ e $y$ son vectores columna de $n$ elementos. Entonces

$$
\begin{aligned}
x^Ty &= x_1y_1+\cdots+x_ny_n \\
\frac{\partial(x^Ty)}{\partial x} &= \begin{bmatrix}\partial(x^Ty)/\partial x_1 & \cdots & \partial(x^Ty)/\partial x_n\end{bmatrix} \\
&= \begin{bmatrix}y_1 & \cdots & y_n\end{bmatrix} \\
&= y^T
\end{aligned}
\tag{1.55}
$$

Del mismo modo, podemos obtener

$$\frac{\partial(x^Ty)}{\partial y} = x^T \tag{1.56}$$

Ahora calcularemos la derivada parcial de una cuadrática respecto de un vector. Primero escribamos la cuadrática de la siguiente manera:

$$
\begin{aligned}
x^TAx &= \begin{bmatrix}x_1 & \cdots & x_n\end{bmatrix}\begin{bmatrix}A_{11}&\cdots&A_{1n}\\ \vdots&\ddots&\vdots\\ A_{n1}&\cdots&A_{nn}\end{bmatrix}\begin{bmatrix}x_1\\ \vdots\\ x_n\end{bmatrix} \\
&= \begin{bmatrix}\sum_ix_iA_{i1} & \cdots & \sum_ix_iA_{in}\end{bmatrix}\begin{bmatrix}x_1\\ \vdots\\ x_n\end{bmatrix} \\
&= \sum_{i,j}x_ix_jA_{ij}
\end{aligned}
\tag{1.57}
$$

Ahora tomemos la derivada parcial de la cuadrática de la siguiente manera:

$$
\begin{aligned}
\frac{\partial(x^TAx)}{\partial x} &= \begin{bmatrix}\partial(x^TAx)/\partial x_1 & \cdots & \partial(x^TAx)/\partial x_n\end{bmatrix} \\
&= \begin{bmatrix}\sum_jx_jA_{1j}+\sum_ix_iA_{i1} & \cdots & \sum_jx_jA_{nj}+\sum_ix_iA_{in}\end{bmatrix} \\
&= \begin{bmatrix}\sum_jx_jA_{1j} & \cdots & \sum_jx_jA_{nj}\end{bmatrix} + \begin{bmatrix}\sum_ix_iA_{i1} & \cdots & \sum_ix_iA_{in}\end{bmatrix} \\
&= x^TA^T+x^TA
\end{aligned}
\tag{1.58}
$$

Si $A$ es simétrica, como suele ocurrir en las expresiones cuadráticas, entonces $A=A^T$ y la expresión anterior se simplifica a

$$\frac{\partial(x^TAx)}{\partial x} = 2x^TA \qquad \text{si } A=A^T \tag{1.59}$$

A continuación, definimos la derivada parcial de un vector respecto de otro vector. Supongamos que $g(x)=\begin{bmatrix}g_1(x)\\ \vdots \\ g_m(x)\end{bmatrix}$ y $x=\begin{bmatrix}x_1\\ \vdots \\ x_n\end{bmatrix}$. Entonces

$$\frac{\partial g}{\partial x} = \begin{bmatrix}\partial g_1/\partial x_1 & \cdots & \partial g_1/\partial x_n \\ \vdots & & \vdots \\ \partial g_m/\partial x_1 & \cdots & \partial g_m/\partial x_n\end{bmatrix} \tag{1.60}$$

Si $g(x)$ o $x$ están transpuestos, entonces la derivada parcial también queda transpuesta.

$$
\begin{aligned}
\frac{\partial g^T}{\partial x} &= \left(\frac{\partial g}{\partial x}\right)^T \\
\frac{\partial g}{\partial x^T} &= \left(\frac{\partial g}{\partial x}\right)^T \\
\frac{\partial g^T}{\partial x^T} &= \frac{\partial g}{\partial x}
\end{aligned}
\tag{1.61}
$$

Con estas definiciones, pueden deducirse las siguientes igualdades importantes. Supongamos que $A$ es una matriz de $m\times n$ y $x$ es un vector de $n\times 1$. Entonces

$$
\begin{aligned}
\frac{\partial(Ax)}{\partial x} &= A \\
\frac{\partial(x^TA)}{\partial x} &= A
\end{aligned}
\tag{1.62}
$$

Supongamos ahora que $A$ es una matriz de $m\times n$, $B$ es una matriz de $n\times n$, y queremos calcular la derivada parcial de $\text{Tr}(ABA^T)$ respecto de $A$. Primero calculemos $ABA^T$ de la siguiente manera:

$$
\begin{aligned}
ABA^T &= \begin{bmatrix}A_{11}&\cdots&A_{1n}\\ \vdots& &\vdots\\ A_{m1}&\cdots&A_{mn}\end{bmatrix}\begin{bmatrix}B_{11}&\cdots&B_{1n}\\ \vdots&\ddots&\vdots\\ B_{n1}&\cdots&B_{nn}\end{bmatrix}\begin{bmatrix}A_{11}&\cdots&A_{m1}\\ \vdots&\ddots&\vdots\\ A_{1n}&\cdots&A_{nn}\end{bmatrix} \\
&= \begin{bmatrix}\sum_{j,k}A_{1k}B_{kj}A_{1j} & \cdots & \sum_{j,k}A_{1k}B_{kj}A_{mj} \\ \vdots & & \vdots \\ \sum_{j,k}A_{mk}B_{kj}A_{1j} & \cdots & \sum_{j,k}A_{mk}B_{kj}A_{mj}\end{bmatrix}
\end{aligned}
\tag{1.63}
$$

A partir de esto vemos que la traza de $ABA^T$ está dada por

$$\text{Tr}(ABA^T) = \sum_{i,j,k}A_{ik}B_{kj}A_{ij} \tag{1.64}$$

Su derivada parcial respecto de $A$ puede calcularse como

$$
\begin{aligned}
\frac{\partial\text{Tr}(ABA^T)}{\partial A} &= \begin{bmatrix}\partial\text{Tr}(ABA^T)/\partial A_{11} & \cdots & \partial\text{Tr}(ABA^T)/\partial A_{1n} \\ \vdots & & \vdots \\ \partial\text{Tr}(ABA^T)/\partial A_{m1} & \cdots & \partial\text{Tr}(ABA^T)/\partial A_{mn}\end{bmatrix} \\
&= \begin{bmatrix}\sum_jA_{1j}B_{1j}+\sum_kA_{1k}B_{k1} & \cdots & \sum_jA_{1j}B_{nj}+\sum_kA_{1k}B_{kn} \\ \vdots & \ddots & \vdots \\ \sum_jA_{mj}B_{1j}+\sum_kA_{mk}B_{k1} & \cdots & \sum_jA_{mj}B_{nj}+\sum_kA_{mk}B_{kn}\end{bmatrix} \\
&= \begin{bmatrix}\sum_jA_{1j}B_{1j} & \cdots & \sum_jA_{1j}B_{nj} \\ \vdots & & \vdots \\ \sum_jA_{mj}B_{1j} & \cdots & \sum_jA_{mj}B_{nj}\end{bmatrix} + \\
&\quad\ \begin{bmatrix}\sum_kA_{1k}B_{k1} & \cdots & \sum_kA_{1k}B_{kn} \\ \vdots & & \vdots \\ \sum_kA_{mk}B_{k1} & \cdots & \sum_kA_{mk}B_{kn}\end{bmatrix} \\
&= AB^T+AB
\end{aligned}
\tag{1.65}
$$

Si $B$ es simétrica, como suele ocurrir en las derivadas parciales de la forma anterior, esto puede simplificarse a

$$\frac{\partial\text{Tr}(ABA^T)}{\partial A} = 2AB \qquad \text{si } B=B^T \tag{1.66}$$

Pueden encontrarse varios resultados adicionales interesantes relacionados con el cálculo matricial en [Ske98, Apéndice B].

### 1.1.4 La historia de las matrices

Esta sección es una breve digresión para presentar parte de la historia de la teoría de matrices. Gran parte de la información de esta sección se toma de [OCo96].

El uso de matrices puede rastrearse hasta el siglo IV a. C. Vemos en tablillas de arcilla antiguas que los babilonios estudiaban problemas que conducían a sistemas de ecuaciones lineales. Por ejemplo, una tablilla que data de alrededor del año 300 a. C. contiene el siguiente problema: "Hay dos campos cuya área total es de 1.800 unidades. Uno produce grano a razón de 2/3 de bushel por unidad, mientras que el otro produce grano a razón de 1/2 bushel por unidad. Si el rendimiento total es de 1.100 bushels, ¿cuál es el tamaño de cada campo?".

Más adelante, los chinos se acercaron aún más al uso de matrices. En [She99] (publicado originalmente entre los años 200 a. C. y 100 d. C.) encontramos el siguiente problema: "Hay tres tipos de grano, de los cuales tres manojos del primero, dos del segundo y uno del tercero hacen 39 medidas. Dos del primero, tres del segundo y uno del tercero hacen 34 medidas. Y uno del primero, dos del segundo y tres del tercero hacen 26 medidas. ¿Cuántas medidas de grano contiene un manojo de cada tipo?". En ese punto, los antiguos chinos usan, en esencia, la eliminación gaussiana (que no fue bien conocida hasta el siglo XIX) para resolver el problema.

A pesar de este comienzo tan temprano, la investigación seria del álgebra matricial no comenzó hasta fines del siglo XVII. En 1683, el matemático japonés Takakazu Seki Kowa escribió un libro titulado "Método para resolver los problemas disimulados". Este libro ofrece métodos generales para calcular determinantes y presenta ejemplos con matrices de hasta $5\times 5$. Coincidentemente, ese mismo año (1683), Gottfried Leibniz, en Europa, también usó por primera vez determinantes para resolver sistemas de ecuaciones lineales. Leibniz también descubrió que un determinante podía desarrollarse usando cualquiera de las columnas de la matriz.

A mediados del siglo XVIII, Colin Maclaurin y Gabriel Cramer publicaron importantes contribuciones a la teoría de matrices. A partir de entonces, el trabajo sobre matrices se volvió bastante regular, con contribuciones significativas de Etienne Bezout, Alexandre Vandermonde, Pierre Laplace, Joseph Lagrange y Carl Gauss. El término "determinante" fue usado por primera vez en el sentido moderno por Augustin Cauchy en 1812 (aunque la palabra ya había sido usada antes por Gauss con un sentido diferente). Cauchy también descubrió los autovalores de una matriz y la diagonalización, e introdujo la idea de matrices semejantes. Fue el primero en demostrar que toda matriz simétrica real es diagonalizable.

James Sylvester (en 1850) fue el primero en usar el término "matriz". Sylvester se mudó a Inglaterra en 1851 para ejercer como abogado, y allí conoció a Arthur Cayley, otro abogado que también estaba interesado en las matemáticas. Cayley advirtió la importancia de la idea de las matrices, y en 1853 inventó la inversión de matrices. Cayley también demostró que las matrices de $2\times 2$ y $3\times 3$ satisfacen sus propias ecuaciones características. Al hecho de que una matriz satisfaga su propia ecuación característica hoy se lo llama el teorema de Cayley–Hamilton (véase el Problema 1.5). El teorema lleva asociado el nombre de William Hamilton porque este lo demostró para matrices de $4\times 4$ en el transcurso de su trabajo sobre cuaterniones.

Camille Jordan inventó la forma canónica de Jordan de una matriz en 1870. Georg Frobenius demostró en 1878 que todas las matrices satisfacen su propia ecuación característica (el teorema de Cayley-Hamilton). También introdujo la definición del rango de una matriz. La nulidad de una matriz cuadrada fue definida por Sylvester en 1884. Las publicaciones de Karl Weierstrass y Leopold Kronecker en 1903 fueron decisivas para establecer la teoría de matrices como una rama importante de las matemáticas. El libro de Leon Mirsky de 1955 [Mir90] ayudó a consolidar la teoría de matrices como un tema de importancia fundamental en las matemáticas universitarias.

## 1.2 SISTEMAS LINEALES

Muchos procesos de nuestro mundo pueden describirse mediante sistemas en el espacio de estados. Esto incluye procesos de la ingeniería, la economía, la física, la química, la biología, y muchas otras áreas. Si podemos deducir un modelo matemático para un proceso, entonces podemos usar las herramientas de la matemática para controlar el proceso y obtener información sobre él. Por eso los sistemas en el espacio de estados son tan importantes para los ingenieros. Si conocemos el estado de un sistema en el instante presente, y conocemos todas las entradas presentes y futuras, entonces podemos deducir los valores de todas las salidas futuras del sistema.

Los modelos en el espacio de estados pueden dividirse, en general, en modelos lineales y modelos no lineales. Aunque la mayoría de los procesos reales son no lineales, las herramientas matemáticas disponibles para la estimación y el control son mucho más accesibles y están mucho mejor comprendidas para los sistemas lineales. Por eso los sistemas no lineales suelen aproximarse como sistemas lineales. De esa manera podemos usar las herramientas que se han desarrollado para sistemas lineales a fin de deducir algoritmos de estimación o de control.

Un sistema lineal, determinístico y en tiempo continuo puede describirse mediante las ecuaciones

$$
\begin{aligned}
\dot x &= Ax+Bu \\
y &= Cx
\end{aligned}
\tag{1.67}
$$

donde $x$ es el vector de estado, $u$ es el vector de control, e $y$ es el vector de salida. Las matrices $A$, $B$ y $C$ son matrices de dimensiones apropiadas. A la matriz $A$ se la suele llamar matriz del sistema, a $B$ se la suele llamar matriz de entrada, y a $C$ se la suele llamar matriz de salida. En general, $A$, $B$ y $C$ pueden ser matrices variantes en el tiempo y el sistema seguirá siendo lineal. Si $A$, $B$ y $C$ son constantes, entonces la solución de la Ecuación (1.67) está dada por

$$
\begin{aligned}
x(t) &= e^{A(t-t_0)}x(t_0) + \int_{t_0}^{t}e^{A(t-\tau)}Bu(\tau)\,d\tau \\
y(t) &= Cx(t)
\end{aligned}
\tag{1.68}
$$

donde $t_0$ es el instante inicial del sistema y suele tomarse igual a 0. Esto es fácil de verificar cuando todas las cantidades de la Ecuación (1.67) son escalares, pero resulta ser cierto también en el caso vectorial. Nótese que, en el caso de entrada nula, $x(t)$ está dado por

$$x(t) = e^{A(t-t_0)}x(t_0), \qquad \text{caso de entrada nula} \tag{1.69}$$

Por esta razón, a $e^{At}$ se lo llama la matriz de transición de estado del sistema.³ Es la matriz que describe cómo cambia el estado a partir de su condición inicial en ausencia de entradas externas. Podemos evaluar la ecuación anterior en $t=t_0$ para ver que

$$e^{A0} = I \tag{1.70}$$

en analogía con la exponencial escalar de cero.

Como se dijo antes, incluso si $x$ es un vector de $n$ elementos, la Ecuación (1.68) sigue describiendo la solución de la Ecuación (1.67). Sin embargo, en este caso surge una pregunta fundamental: ¿cómo podemos tomar la exponencial de la matriz $A$ en la Ecuación (1.68)? ¿Qué significa elevar el escalar $e$ a la potencia de una matriz? Existen muchas maneras distintas de calcular esta cantidad [Mol03]. Tres de las más útiles son las siguientes:

$$
\begin{aligned}
e^{At} &= \sum_{j=0}^{\infty}\frac{(At)^j}{j!} \\
&= \mathcal{L}^{-1}[(sI-A)^{-1}] \\
&= Qe^{\hat At}Q^{-1}
\end{aligned}
\tag{1.71}
$$

La primera expresión de las anteriores es la definición de $e^{At}$, y es análoga a la definición de la exponencial de un escalar. Esta definición muestra que $A$ debe ser cuadrada para que $e^{At}$ exista. A partir de la Ecuación (1.67), vemos que una matriz de sistema siempre es cuadrada. La definición de $e^{At}$ también puede usarse para deducir las siguientes propiedades.

$$
\begin{aligned}
\frac{d}{dt}e^{At} &= Ae^{At} \\
&= e^{At}A
\end{aligned}
\tag{1.72}
$$

*³La función EXPM de MATLAB calcula la exponencial matricial. Nótese que la función EXP de MATLAB calcula la exponencial elemento a elemento de una matriz, que en general no es lo mismo que la exponencial matricial.*

En general, las matrices no conmutan bajo la multiplicación pero, curiosamente, una matriz siempre conmuta con su exponencial.

La primera expresión de la Ecuación (1.71) por lo general no resulta práctica para fines computacionales, ya que es una suma infinita (aunque los últimos términos de la suma a menudo disminuyen rápidamente en magnitud, e incluso pueden llegar a ser cero). La segunda expresión de la Ecuación (1.71) usa la transformada inversa de Laplace para calcular $e^{At}$. En la tercera expresión de la Ecuación (1.71), $Q$ es una matriz cuyas columnas están formadas por los autovectores de $A$, y $\hat A$ es la forma de Jordan⁴ de $A$. Nótese que $Q$ y $\hat A$ están bien definidas para cualquier matriz cuadrada $A$, por lo que la exponencial matricial $e^{At}$ existe para todas las matrices cuadradas $A$ y todo $t$ finito. La matriz $\hat A$ suele ser diagonal, en cuyo caso $e^{\hat At}$ es fácil de calcular:

$$
\begin{aligned}
\hat A &= \begin{bmatrix}\hat A_{11}&0&\cdots&0\\0&\hat A_{22}&\cdots&0\\ \vdots&\ddots&\ddots&\vdots\\0&\cdots&\cdots&\hat A_{nn}\end{bmatrix} \\
e^{\hat At} &= \begin{bmatrix}e^{\hat A_{11}}&0&\cdots&0\\0&e^{\hat A_{22}}&\cdots&0\\ \vdots&\ddots&\ddots&\vdots\\0&\cdots&\cdots&e^{\hat A_{nn}}\end{bmatrix}
\end{aligned}
\tag{1.73}
$$

Esto puede calcularse a partir de la definición de $e^{\hat At}$ en la Ecuación (1.71). Incluso si la matriz en forma de Jordan $\hat A$ no es diagonal, $e^{\hat At}$ es fácil de calcular [Bay99, Che99, Kai80]. También podemos notar, a partir de la tercera expresión de la Ecuación (1.71), que

$$
\begin{aligned}
\left[e^{At}\right]^{-1} &= e^{-At} \\
&= Qe^{-\hat At}Q^{-1}
\end{aligned}
\tag{1.74}
$$

(Recordemos que $A$ y $-A$ tienen los mismos autovectores, y que sus autovalores son opuestos entre sí. Véase el Problema 1.10.) Vemos, a partir de esto, que $e^{At}$ siempre es invertible. Esto es análogo a la situación escalar, en la cual la exponencial de un escalar siempre es distinta de cero.

*⁴De hecho, la Ecuación (1.71) puede usarse para definir la forma de Jordan de una matriz. Es decir, si $e^{At}$ puede escribirse como se muestra en la Ecuación (1.71), donde $Q$ es una matriz cuyas columnas están formadas por los autovectores de $A$, entonces $\hat A$ es la forma de Jordan de $A$. Puede encontrarse un análisis más detallado sobre las formas de Jordan y su cálculo en la mayoría de los libros de sistemas lineales [Kai80, Bay99, Che99].*

Otro hecho interesante acerca de la exponencial matricial es que todos los elementos individuales de la exponencial matricial $e^A$ son no negativos si y solo si todos los elementos individuales de $A$ son no negativos [Bel60, Bel80].

\vspace{\baselineskip}
**EJEMPLO 1.2**

Como ejemplo de un sistema lineal, supongamos que estamos controlando la aceleración angular de un motor (por ejemplo, mediante alguna tensión aplicada a los devanados del motor). La derivada de la posición es la velocidad. Un modelo simplificado del motor puede entonces escribirse como

$$
\begin{aligned}
\dot\theta &= \omega \\
\dot\omega &= u+w_1
\end{aligned}
\tag{1.75}
$$

El escalar $w_1$ es el ruido de aceleración, y podría estar compuesto por factores tales como la incertidumbre en la aceleración aplicada, la excentricidad del eje del motor, y perturbaciones de la carga. Si nuestra medición consiste en la posición angular del motor, entonces una descripción en el espacio de estados de este sistema puede escribirse como

$$
\begin{aligned}
\begin{bmatrix}\dot\theta\\ \dot\omega\end{bmatrix} &= \begin{bmatrix}0&1\\0&0\end{bmatrix}\begin{bmatrix}\theta\\ \omega\end{bmatrix} + \begin{bmatrix}0\\1\end{bmatrix}u + \begin{bmatrix}0\\w_1\end{bmatrix} \\
y &= \begin{bmatrix}1&0\end{bmatrix}x+v
\end{aligned}
\tag{1.76}
$$

El escalar $v$ consiste en ruido de medición. Comparando con la Ecuación (1.67), vemos que el vector de estado $x$ es un vector de $2\times 1$ que contiene los escalares $\theta$ y $\omega$.

▽▽▽

\vspace{\baselineskip}
**EJEMPLO 1.3**

En este ejemplo usaremos las tres expresiones de la Ecuación (1.71) para calcular la matriz de transición de estado del sistema descrito en el Ejemplo 1.2. A partir de la primera expresión de la Ecuación (1.71) obtenemos

$$
\begin{aligned}
e^{At} &= \sum_{j=0}^{\infty}\frac{(At)^j}{j!} \\
&= (At)^0+(At)^1+\frac{(At)^2}{2!}+\frac{(At)^3}{3!}+\cdots \\
&= I+At
\end{aligned}
\tag{1.77}
$$

donde la última igualdad proviene del hecho de que $A^k=0$ cuando $k>1$ para la matriz $A$ dada en el Ejemplo 1.2. Por lo tanto, obtenemos

$$
\begin{aligned}
e^{At} &= \begin{bmatrix}1&0\\0&1\end{bmatrix}+\begin{bmatrix}0&t\\0&0\end{bmatrix} \\
&= \begin{bmatrix}1&t\\0&1\end{bmatrix}
\end{aligned}
\tag{1.78}
$$

A partir de la segunda expresión de la Ecuación (1.71) obtenemos

$$
\begin{aligned}
e^{At} &= \mathcal{L}^{-1}[(sI-A)^{-1}] \\
&= \mathcal{L}^{-1}\left(\begin{bmatrix}s&-1\\0&s\end{bmatrix}^{-1}\right) \\
&= \mathcal{L}^{-1}\begin{bmatrix}1/s&1/s^2\\0&1/s\end{bmatrix} \\
&= \begin{bmatrix}1&t\\0&1\end{bmatrix}
\end{aligned}
\tag{1.79}
$$

Para usar la tercera expresión de la Ecuación (1.71), primero necesitamos obtener los datos propios (es decir, los autovalores y autovectores) de la matriz $A$. Estos se hallan como

$$
\begin{aligned}
\lambda(A) &= \{0,0\} \\
v(A) &= \left\{\begin{bmatrix}1\\0\end{bmatrix},\begin{bmatrix}0\\1\end{bmatrix}\right\}
\end{aligned}
\tag{1.80}
$$

Esto muestra que

$$
\begin{aligned}
\hat A &= \begin{bmatrix}0&1\\0&0\end{bmatrix} \\
Q &= \begin{bmatrix}1&0\\0&1\end{bmatrix}
\end{aligned}
\tag{1.81}
$$

Nótese que, en este ejemplo simple, $A$ ya está en forma de Jordan, por lo que $\hat A=A$ y $Q=I$. Por lo tanto, la tercera expresión de la Ecuación (1.71) da

$$
\begin{aligned}
e^{At} &= Qe^{\hat At}Q^{-1} \\
&= \begin{bmatrix}1&0\\0&1\end{bmatrix}\begin{bmatrix}1&t\\0&1\end{bmatrix}\begin{bmatrix}1&0\\0&1\end{bmatrix}^{-1} \\
&= \begin{bmatrix}1&t\\0&1\end{bmatrix}
\end{aligned}
\tag{1.82}
$$

▽▽▽

## 1.3 SISTEMAS NO LINEALES

El análisis de los sistemas lineales de la sección anterior es un poco optimista, porque en la realidad los sistemas lineales no existen. Los sistemas reales siempre presentan algunas no linealidades. Incluso un simple resistor es, en última instancia, no lineal si le aplicamos una tensión suficientemente grande. Sin embargo, a menudo modelamos un resistor con la simple ecuación lineal $V=IR$, porque esta ecuación describe con precisión el funcionamiento del resistor en un amplio rango de operación. Entonces, aunque los sistemas lineales no existen en el mundo real, la teoría de sistemas lineales sigue siendo una herramienta valiosa para abordar los sistemas no lineales.

La forma general de un sistema no lineal en tiempo continuo puede escribirse como

$$
\begin{aligned}
\dot x &= f(x,u,w) \\
y &= h(x,v)
\end{aligned}
\tag{1.83}
$$

donde $f(\cdot)$ y $h(\cdot)$ son funciones vectoriales arbitrarias. Usamos $w$ para indicar el ruido de proceso, y $v$ para indicar el ruido de medición. Si $f(\cdot)$ y $h(\cdot)$ son funciones explícitas de $t$, entonces el sistema es variante en el tiempo. En caso contrario, el sistema es invariante en el tiempo. Si $f(x,u,w)=Ax+Bu+w$, y $h(x,v)=Hx+v$, entonces el sistema es lineal [compárese con la Ecuación (1.67)]. En caso contrario, el sistema es no lineal.

Para aplicar herramientas de la teoría de sistemas lineales a sistemas no lineales, necesitamos linealizar el sistema no lineal. En otras palabras, necesitamos hallar un sistema lineal que sea aproximadamente igual al sistema no lineal. Para ver cómo se hace esto, comencemos con una función vectorial no lineal $f(\cdot)$ de un escalar $x$. Expandimos $f(x)$ en una serie de Taylor alrededor de algún punto de operación nominal (también llamado punto de linealización) $x=\bar x$, definiendo $\tilde x=x-\bar x$:

$$f(x) = f(\bar x)+\left.\frac{\partial f}{\partial x}\right|_{\bar x}\tilde x+\frac{1}{2!}\left.\frac{\partial^2 f}{\partial x^2}\right|_{\bar x}\tilde x^2+\frac{1}{3!}\left.\frac{\partial^3 f}{\partial x^3}\right|_{\bar x}\tilde x^3+\cdots \tag{1.84}$$

Supongamos ahora que $x$ es un vector de $2\times 1$. Esto implica que $f(x)$ es una función no lineal de dos variables independientes, $x_1$ y $x_2$. La expansión en serie de Taylor de $f(x)$ se convierte en

$$
\begin{aligned}
f(x) &= f(\bar x)+\left.\frac{\partial f}{\partial x_1}\right|_{\bar x}\tilde x_1+\left.\frac{\partial f}{\partial x_2}\right|_{\bar x}\tilde x_2+ \\
&\quad \frac{1}{2!}\left(\left.\frac{\partial^2f}{\partial x_1^2}\right|_{\bar x}\tilde x_1^2+\left.\frac{\partial^2f}{\partial x_2^2}\right|_{\bar x}\tilde x_2^2+2\left.\frac{\partial^2f}{\partial x_1x_2}\right|_{\bar x}\tilde x_1\tilde x_2\right)+ \\
&\quad \frac{1}{3!}\left(\left.\frac{\partial^3f}{\partial x_1^3}\right|_{\bar x}\tilde x_1^3+\left.\frac{\partial^3f}{\partial x_2^3}\right|_{\bar x}\tilde x_2^3+3\left.\frac{\partial^3f}{\partial x_1^2x_2}\right|_{\bar x}\tilde x_1^2\tilde x_2+3\left.\frac{\partial^3f}{\partial x_1x_2^2}\right|_{\bar x}\tilde x_1\tilde x_2^2\right)+\cdots
\end{aligned}
\tag{1.85}
$$

Esto puede escribirse de manera más compacta como

$$
\begin{aligned}
f(x) &= f(\bar x)+\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\tilde x_2\frac{\partial}{\partial x_2}\right)f\right|_{\bar x}+\frac{1}{2!}\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\tilde x_2\frac{\partial}{\partial x_2}\right)^2f\right|_{\bar x}+ \\
&\quad \frac{1}{3!}\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\tilde x_2\frac{\partial}{\partial x_2}\right)^3f\right|_{\bar x}+\cdots
\end{aligned}
\tag{1.86}
$$

Extendiendo esto al caso general en que $x$ es un vector de $n\times 1$, vemos que cualquier función vectorial continua $f(x)$ puede expandirse en una serie de Taylor como

$$
\begin{aligned}
f(x) &= f(\bar x)+\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\cdots+\tilde x_n\frac{\partial}{\partial x_n}\right)f\right|_{\bar x}+ \\
&\quad \frac{1}{2!}\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\cdots+\tilde x_n\frac{\partial}{\partial x_n}\right)^2f\right|_{\bar x}+ \\
&\quad \frac{1}{3!}\left.\left(\tilde x_1\frac{\partial}{\partial x_1}+\cdots+\tilde x_n\frac{\partial}{\partial x_n}\right)^3f\right|_{\bar x}+\cdots
\end{aligned}
\tag{1.87}
$$

Ahora definimos la operación $D_{\bar x}^kf$ como

$$D_{\bar x}^kf = \left(\sum_{i=1}^{n}\tilde x_i\frac{\partial}{\partial x_i}\right)^kf(x)\bigg|_{\bar x} \tag{1.88}$$

Usando esta definición, escribimos la expansión en serie de Taylor de $f(x)$ como

$$f(x) = f(\bar x)+D_{\bar x}f+\frac{1}{2!}D_{\bar x}^2f+\frac{1}{3!}D_{\bar x}^3f+\cdots \tag{1.89}$$

Si la función no lineal $f(x)$ es "suficientemente suave", entonces las derivadas de orden superior de $f(x)$ deberían ser "relativamente pequeñas". Además, si $f(x)$ se expande alrededor de un punto tal que $x$ esté "cerca" de $\bar x$, entonces $\tilde x$ será "pequeño" y las potencias superiores de $\tilde x$ en la Ecuación (1.89) serán "pequeñas". Por último, las derivadas de orden superior en la expansión en serie de Taylor de la Ecuación (1.89) se dividen por factoriales cada vez más grandes, lo cual disminuye aún más la magnitud de los términos de orden superior en la Ecuación (1.89). Esto justifica la aproximación

$$
\begin{aligned}
f(x) &\approx f(\bar x)+D_{\bar x}f \\
&\approx f(\bar x)+\left.\frac{\partial f}{\partial x}\right|_{\bar x}\tilde x \\
&\approx f(\bar x)+A\tilde x
\end{aligned}
\tag{1.90}
$$

donde $A$ es la matriz definida por la ecuación anterior.

Volviendo a nuestras ecuaciones de sistema no lineal de la Ecuación (1.83), podemos expandir la ecuación del sistema no lineal $f(x,u,w)$ alrededor del punto de operación nominal $(\bar x,\bar u,\bar w)$. Obtenemos entonces una aproximación lineal del sistema de la siguiente manera.

$$
\begin{aligned}
\dot x &= f(x,u,w) \\
&\approx f(\bar x,\bar u,\bar w)+\left.\frac{\partial f}{\partial x}\right|_0(x-\bar x)+\left.\frac{\partial f}{\partial u}\right|_0(u-\bar u)+\left.\frac{\partial f}{\partial w}\right|_0(w-\bar w) \\
&= \dot{\bar x}+A\tilde x+B\tilde u+L\tilde w
\end{aligned}
\tag{1.91}
$$

donde el subíndice 0 significa que la función se evalúa en el punto nominal $(\bar x,\bar u,\bar w)$, y $A$, $B$ y $L$ quedan definidas por las ecuaciones anteriores. Restando $\dot{\bar x}$ de ambos lados de la Ecuación (1.91) se obtiene

$$\dot{\tilde x} = A\tilde x+B\tilde u+L\tilde w \tag{1.92}$$

Dado que $w$ es ruido, fijaremos $\bar w=0$, de modo que $\tilde w=w$, y obtenemos

$$\dot{\tilde x} = A\tilde x+B\tilde u+Lw \tag{1.93}$$

Vemos que tenemos una ecuación lineal para $\dot{\tilde x}$ en términos de $\tilde x$, $\tilde u$ y $w$. Tenemos una ecuación lineal para las desviaciones del estado y del control respecto de sus valores nominales. Mientras las desviaciones sigan siendo pequeñas, la linealización será precisa, y la ecuación lineal describirá con precisión las desviaciones de $x$ respecto de su valor nominal $\bar x$.

De manera similar, podemos expandir la ecuación de medición no lineal dada por la Ecuación (1.83) alrededor de un punto de operación nominal $x=\bar x$ y $v=\bar v=0$. Esto da como resultado la ecuación de medición linealizada

$$
\begin{aligned}
\tilde y &= \left.\frac{\partial h}{\partial x}\right|_0\tilde x+\left.\frac{\partial h}{\partial v}\right|_0\tilde v \\
&= C\tilde x+Dv
\end{aligned}
\tag{1.94}
$$

donde $C$ y $D$ quedan definidas por la ecuación anterior. Las Ecuaciones (1.93) y (1.94) constituyen un sistema lineal que describe las desviaciones del estado y de la salida respecto de sus valores nominales. Recordemos que las cantidades con tilde en las Ecuaciones (1.93) y (1.94) se definen como

$$
\begin{aligned}
\tilde x &= x-\bar x \\
\tilde u &= u-\bar u \\
\tilde y &= y-\bar y
\end{aligned}
\tag{1.95}
$$

\vspace{\baselineskip}
**EJEMPLO 1.4**

Consideremos el siguiente modelo de un motor síncrono de imán permanente bifásico:

$$
\begin{aligned}
\dot i_a &= \frac{-R}{L}i_a+\frac{\omega\lambda}{L}\sin\theta+\frac{u_a}{L} \\
\dot i_b &= \frac{-R}{L}i_b-\frac{\omega\lambda}{L}\cos\theta+\frac{u_b}{L} \\
\dot\omega &= \frac{-3\lambda}{2J}i_a\sin\theta+\frac{3\lambda}{2J}i_b\cos\theta-\frac{F\omega}{J}-\frac{T_l}{J} \\
\dot\theta &= \omega
\end{aligned}
\tag{1.96}
$$

donde $i_a$ e $i_b$ son las corrientes que circulan por los dos devanados, $R$ y $L$ son la resistencia y la inductancia de los devanados, $\theta$ y $\omega$ son la posición angular y la velocidad angular del rotor, $\lambda$ es la constante de flujo del motor, $u_a$ y $u_b$ son las tensiones aplicadas a los dos devanados, $J$ es el momento de inercia del rotor y su carga, $F$ es la fricción viscosa del rotor, y $T_l$ es el torque de carga. La variable temporal no aparece explícitamente en el lado derecho de la ecuación anterior, por lo que este es un sistema invariante en el tiempo. Sin embargo, el sistema es altamente no lineal, por lo que no podemos usar directamente ninguna herramienta de sistemas lineales para el control o la estimación. No obstante, si linealizamos el sistema alrededor de un punto de operación nominal (posiblemente variante en el tiempo), entonces sí podemos usar herramientas de sistemas lineales para el control y la estimación. Comenzamos definiendo un vector de estado como $x=\begin{bmatrix}i_a & i_b & \omega & \theta\end{bmatrix}^T$. Con esta definición escribimos

$$
\begin{aligned}
\dot x &= \begin{bmatrix}\dot x_1 & \dot x_2 & \dot x_3 & \dot x_4\end{bmatrix}^T \\
&= f(x,u) \\
&= \begin{bmatrix} \dfrac{-R}{L}x_1+\dfrac{x_3\lambda}{L}\sin x_4+\dfrac{u_a}{L} \\[6pt] \dfrac{-R}{L}x_2-\dfrac{x_3\lambda}{L}\cos x_4+\dfrac{u_b}{L} \\[6pt] \dfrac{-3\lambda}{2J}x_1\sin x_4+\dfrac{3\lambda}{2J}x_2\cos x_4-\dfrac{Fx_3}{J}-\dfrac{T_l}{J} \\[6pt] x_3 \end{bmatrix}
\end{aligned}
\tag{1.97}
$$

Linealizamos la ecuación del sistema tomando la derivada parcial de $f(x,u)$ respecto de $x$ y de $u$, para obtener

$$
\begin{aligned}
A &= \frac{\partial f}{\partial x} \\
&= \begin{bmatrix} -R/L & 0 & \lambda s_4/L & x_3\lambda c_4/L \\ 0 & -R/L & -\lambda c_4/L & x_3\lambda s_4/L \\ -3\lambda s_4/2J & 3\lambda c_4/2J & -F/J & -3\lambda(x_1c_4+x_2s_4)/2J \\ 0 & 0 & 1 & 0 \end{bmatrix} \\
B &= \frac{\partial f}{\partial u} \\
&= \begin{bmatrix}1/L&0\\0&1/L\\0&0\\0&0\end{bmatrix}
\end{aligned}
\tag{1.98}
$$

donde $s_4=\sin x_4$ y $c_4=\cos x_4$. El sistema lineal

$$\dot{\tilde x} = A\tilde x+B\tilde u \tag{1.99}$$

describe de manera aproximada la desviación de $x$ respecto de su valor nominal $\bar x$. El sistema no lineal se simuló con los valores de control nominales $\bar u_a(t)=\sin 2\pi t$ y $\bar u_b(t)=\cos 2\pi t$. Esto dio como resultado una trayectoria de estado nominal $\bar x(t)$. Luego, los sistemas lineal y no lineal se simularon con valores de control no nominales. La Figura 1.1 muestra los resultados de las simulaciones lineal y no lineal cuando la desviación de la magnitud de control respecto de la nominal es un número positivo pequeño. Puede verse que las simulaciones dan como resultado trayectorias similares en el espacio de estados, aunque no coinciden exactamente. Si la desviación es cero, entonces las simulaciones lineal y no lineal coincidirán exactamente. A medida que aumenta la desviación respecto de la nominal, aumentará la diferencia entre las simulaciones lineal y no lineal.

**Figura 1.1.** Comparación de las simulaciones no lineal y linealizada del motor, para el Ejemplo 1.4. *(Gráfico no reproducido; muestra cuatro paneles — Velocidad, Posición, Corriente A y Corriente B — en función del tiempo, comparando las curvas "No lineal" y "Linealizada".)*

▽▽▽

## 1.4 DISCRETIZACIÓN

La mayoría de los sistemas del mundo real se describen con una dinámica en tiempo continuo, del tipo que se muestra en las Ecuaciones (1.67) o (1.83). Sin embargo, los algoritmos de estimación de estado y de control casi siempre se implementan en electrónica digital. Esto suele requerir una transformación de la dinámica en tiempo continuo a dinámica en tiempo discreto. Esta sección analiza cómo puede transformarse un sistema lineal en tiempo continuo en un sistema lineal en tiempo discreto.

Recordemos, a partir de la Ecuación (1.68), que la solución de un sistema lineal en tiempo continuo está dada por

$$x(t) = e^{A(t-t_0)}x(t_0)+\int_{t_0}^{t}e^{A(t-\tau)}Bu(\tau)\,d\tau \tag{1.100}$$

Sea $t=t_k$ (algún instante discreto de tiempo), y sea el instante inicial $t_0=t_{k-1}$ (el instante discreto de tiempo anterior). Supongamos que $A(\tau)$, $B(\tau)$ y $u(\tau)$ son aproximadamente constantes en el intervalo de integración. Obtenemos entonces

$$x(t_k) = e^{A(t_k-t_{k-1})}x(t_{k-1})+\int_{t_{k-1}}^{t_k}e^{A(t_k-\tau)}\,d\tau\,Bu(t_{k-1}) \tag{1.101}$$

Ahora definamos $\Delta t=t_k-t_{k-1}$, definamos $\alpha=\tau-t_{k-1}$, y sustituyamos $\tau$ en la ecuación anterior para obtener

$$
\begin{aligned}
x(t_k) &= e^{A\Delta t}x(t_{k-1})+\int_0^{\Delta t}e^{A(\Delta t-\alpha)}\,d\alpha\,Bu(t_{k-1}) \\
&= e^{A\Delta t}x(t_{k-1})+e^{A\Delta t}\int_0^{\Delta t}e^{-A\alpha}\,d\alpha\,Bu(t_{k-1}) \\
x_k &= F_{k-1}x_{k-1}+G_{k-1}u_{k-1}
\end{aligned}
\tag{1.102}
$$

donde $x_k$, $F_k$, $G_k$ y $u_k$ quedan definidas por la ecuación anterior. Esta es una aproximación lineal en tiempo discreto de la dinámica en tiempo continuo dada en la Ecuación (1.67). Nótese que este sistema en tiempo discreto define $x_k$ únicamente en los instantes discretos $\{t_k\}$; no dice nada sobre lo que sucede con la señal en tiempo continuo $x(t)$ entre esos instantes discretos.

La dificultad del sistema en tiempo discreto anterior radica en el cálculo de la integral de la exponencial matricial, que es necesaria para calcular la matriz $G$. Este cálculo puede simplificarse si $A$ es invertible:

$$
\begin{aligned}
\int_0^{\Delta t}e^{-A\tau}\,d\tau &= \int_0^{\Delta t}\sum_{j=0}^{\infty}\frac{(-A\tau)^j}{j!}\,d\tau \\
&= \int_0^{\Delta t}\left[I-A\tau+A^2\tau^2/2!-\cdots\right]d\tau \\
&= \left[I\tau-A\tau^2/2!+A^2\tau^3/3!-\cdots\right]_0^{\Delta t} \\
&= \left[I\Delta t-A(\Delta t)^2/2!+A^2(\Delta t)^3/3!-\cdots\right] \\
&= \left[A\Delta t-(A\Delta t)^2/2!+(A\Delta t)^3/3!-\cdots\right]A^{-1} \\
&= \left[I-e^{-A\Delta t}\right]A^{-1}
\end{aligned}
\tag{1.103}
$$

La conversión de las matrices de sistema en tiempo continuo $A$ y $B$ a las matrices de sistema en tiempo discreto $F$ y $G$ puede resumirse de la siguiente manera:

$$
\begin{aligned}
F &= e^{A\Delta t} \\
G &= F\int_0^{\Delta t}e^{-A\tau}\,d\tau\,B \\
&= F\left[I-e^{-A\Delta t}\right]A^{-1}B
\end{aligned}
\tag{1.104}
$$

donde $\Delta t$ es el tamaño de paso de discretización.

## 1.5 SIMULACIÓN

En esta sección analizamos cómo simular sistemas en tiempo continuo (ya sean lineales o no lineales) en una computadora digital. Consideramos la siguiente forma de la ecuación general del sistema, a partir de la Ecuación (1.83):

$$\dot x = f(x,u,t) \tag{1.105}$$

donde $u(t)$ es una entrada de control conocida. Para simular este sistema en una computadora, necesitamos programar una computadora que resuelva $x(t_f)$ para algún valor de $t_f$ especificado por el usuario. En otras palabras, queremos calcular

$$x(t_f) = x(t_0)+\int_{t_0}^{t_f}f[x(t),u(t),t]\,dt \tag{1.106}$$

A menudo, el instante inicial se toma como $t_0=0$, en cuyo caso tenemos la ecuación, de aspecto un poco más simple,

$$x(t_f) = x(0)+\int_0^{t_f}f[x(t),u(t),t]\,dt \tag{1.107}$$

Vemos que, para hallar la solución $x(t_f)$ de la ecuación diferencial $\dot x=f(x,u,t)$, necesitamos calcular una integral. Por lo tanto, al problema de hallar la solución $x(t_f)$ se lo suele llamar un problema de integración.

Supongamos ahora que dividimos el intervalo de tiempo $[0,t_f]$ en $L$ intervalos igualmente espaciados, de modo que $t_k=kT$ para $k=0,\cdots,L$, siendo $T=t_f/L$ el intervalo de tiempo. A partir de esto notamos que $t_f=t_L$. Con esta división del intervalo de tiempo, podemos escribir la solución de la Ecuación (1.107) como

$$
\begin{aligned}
x(t_f) &= x(t_L) \\
&= x(0)+\sum_{k=0}^{L}\int_{t_k}^{t_{k+1}}f[x(t),u(t),t]\,dt
\end{aligned}
\tag{1.108}
$$

De manera más general, para algún $n\in[0,L-1]$, podemos escribir $x(t_n)$ y $x(t_{n+1})$ como

$$
\begin{aligned}
x(t_n) &= x(0)+\sum_{k=0}^{n}\int_{t_k}^{t_{k+1}}f[x(t),u(t),t]\,dt \\
x(t_{n+1}) &= x(0)+\sum_{k=0}^{n+1}\int_{t_k}^{t_{k+1}}f[x(t),u(t),t]\,dt
\end{aligned}
\tag{1.109}
$$

lo cual significa que

$$x(t_{n+1}) = x(t_n)+\int_{t_n}^{t_{n+1}}f[x(t),u(t),t]\,dt \tag{1.110}$$

Si podemos hallar una manera de aproximar la integral del lado derecho de la ecuación anterior, podemos propagar repetidamente nuestra aproximación de $x(t)$ desde el instante $t_n$ hasta el instante $t_{n+1}$, obteniendo así una aproximación de $x(t)$ en cualquier instante $t$ deseado. El algoritmo podría verse algo así.

**Solución de la ecuación diferencial**
```
Suponer que x(0) es conocido
para t = 0 : T : tf − T
    Hallar una aproximación I(t) ≈ ∫[t a t+T] f[x(t),u(t),t] dt
    x(t+T) = x(t) + T I(t)
fin
```

En las secciones siguientes presentamos tres maneras distintas de aproximar esta integral. Las aproximaciones, en orden creciente de esfuerzo computacional y de precisión, son la integración rectangular, la integración trapezoidal, y la integración de Runge–Kutta de cuarto orden.

### 1.5.1 Integración rectangular

Si el intervalo de tiempo $(t_{n+1}-t_n)$ es pequeño, entonces $f[x(t),u(t),t]$ es aproximadamente constante en ese intervalo. Por lo tanto, la Ecuación (1.110) puede aproximarse como

$$
\begin{aligned}
x(t_{n+1}) &\approx x(t_n)+\int_{t_n}^{t_{n+1}}f[x(t_n),u(t_n),t_n]\,dt \\
&\approx x(t_n)+f[x(t_n),u(t_n),t_n]T
\end{aligned}
\tag{1.111}
$$

Por lo tanto, la Ecuación (1.109) puede aproximarse como

$$
\begin{aligned}
x(t_n) &\approx x(0)+\sum_{k=0}^{n}\int_{t_k}^{t_{k+1}}f[x(t_k),u(t_k),t_k]\,dt \\
&= x(0)+\sum_{k=0}^{n}f[x(t_k),u(t_k),t_k]T
\end{aligned}
\tag{1.112}
$$

A esto se lo llama integración de Euler, o integración rectangular, y se ilustra en la Figura 1.2. Siempre que $T$ sea suficientemente pequeño, esto da una buena aproximación de $x(t_n)$.

Esto da el siguiente algoritmo para integrar la dinámica en tiempo continuo mediante integración rectangular. El bucle temporal del algoritmo se ejecuta para $t=0,T,2T,\cdots,t_f-T$.

**Integración rectangular**
```
Suponer que x(0) es conocido
para t = 0 : T : tf − T
    Calcular f[x(t),u(t),t]
    x(t+T) = x(t) + f[x(t),u(t),t]T
fin
```

### 1.5.2 Integración trapezoidal

Al observar la Figura 1.2 surge una idea para mejorar la aproximación de $x(t)$. En lugar de aproximar cada área como un rectángulo, ¿qué pasa si aproximamos cada área como un trapecio? La Figura 1.3 muestra cómo puede implementarse un algoritmo de integración mejorado. A esto se lo llama integración de Euler modificada, o integración trapezoidal. Una comparación de las Figuras 1.2 y 1.3 muestra que la integración trapezoidal

**Figura 1.2.** Ilustración de la integración rectangular. Tenemos $\dot x=f(x)$, por lo que $x(t)$ es el área bajo la curva $f(x)$. Esta área puede aproximarse como la suma de las áreas rectangulares $A_i$. Es decir, $x(0{,}5)\approx A_1$, $x(1)\approx A_1+A_2$, $\cdots$. *(Gráfico no reproducido; muestra la curva $f(x)$ aproximada mediante rectángulos $A_1,\ldots,A_6$ sobre el intervalo de tiempo de 0 a 3.)*

**Figura 1.3.** Ilustración de la integración trapezoidal. Tenemos $\dot x=f(x)$, por lo que $x(t)$ es el área bajo la curva $f(x)$. Esta área puede aproximarse como la suma de las áreas trapezoidales $A_i$. Es decir, $x(1)\approx A_1$, $x(2)\approx A_1+A_2$, y $x(3)\approx A_1+A_2+A_3$. *(Gráfico no reproducido; muestra la curva $f(x)$ aproximada mediante trapecios $A_1$, $A_2$ y $A_3$ sobre el intervalo de tiempo de 0 a 3.)*

parece dar una mejor aproximación que la integración rectangular, aunque en la integración trapezoidal el eje temporal se divide en solo la mitad de intervalos.

Con la integración rectangular, aproximamos $f[x(t),u(t),t]$ como una constante en el intervalo $t\in[t_n,t_{n+1}]$. Con la integración trapezoidal, en cambio, aproximamos $f[x(t),u(t),t]$ como una función lineal en el intervalo $t\in[t_n,t_{n+1}]$. Es decir,

$$
f[x(t)] \approx f[x(t_n),u(t_n),t_n]+\left(\frac{f[x(t_{n+1}),u(t_{n+1}),t_{n+1}]-f[x(t_n),u(t_n),t_n]}{T}\right)(t-t_n) \qquad \text{para } t\in[t_n,t_{n+1}] \tag{1.113}
$$

Por lo tanto, la Ecuación (1.110) puede aproximarse como

$$
\begin{aligned}
x(t_{n+1}) &\approx x(t_n)+\int_{t_n}^{t_{n+1}}\Bigg\{f[x(t_n),u(t_n),t_n]+ \\
&\qquad\qquad \left(\frac{f[x(t_{n+1}),u(t_{n+1}),t_{n+1}]-f[x(t_n),u(t_n),t_n]}{T}\right)(t-t_n)\Bigg\}\,dt \\
&= x(t_n)+\left(\frac{f[x(t_n),u(t_n),t_n]+f[x(t_{n+1}),u(t_{n+1}),t_{n+1}]}{2}\right)T \\
&= x(t_n)+\frac{1}{2}\left(f[x(t_n),u(t_n),t_n]T+f[x(t_{n+1}),u(t_{n+1}),t_{n+1}]T\right)
\end{aligned}
\tag{1.114}
$$

Sin embargo, esta ecuación para aproximar $x(t_{n+1})$ tiene a $x(t_{n+1})$ en su lado derecho. ¿Cómo podemos incorporar $x(t_{n+1})$ en el lado derecho de la ecuación si todavía no conocemos $x(t_{n+1})$? La respuesta es que podemos usar, para $x(t_{n+1})$ en el lado derecho de la ecuación, la aproximación de integración rectangular de la sección anterior. Por lo tanto, la ecuación anterior puede escribirse como

$$
\begin{aligned}
\Delta x_1 &= f[x(t_n),u(t_n),t_n]T \\
\Delta x_2 &= f[x(t_{n+1}),u(t_{n+1}),t_{n+1}]T \\
&\approx f[x(t_n)+\Delta x_1,u(t_{n+1}),t_{n+1}]T \\
x(t_{n+1}) &\approx x(t_n)+\frac{1}{2}(\Delta x_1+\Delta x_2)
\end{aligned}
\tag{1.115}
$$

Esto da el siguiente algoritmo para integrar la dinámica en tiempo continuo mediante integración trapezoidal. El bucle temporal del algoritmo se ejecuta para $t=0,T,2T,\cdots,t_f-T$.

**Integración trapezoidal**
```
Suponer que x(0) es conocido
para t = 0 : T : tf − T
    Δx1 = f[x(t),u(t),t]T
    Δx2 = f[x(t)+Δx1,u(t+T),t+T]T
    x(t+T) = x(t) + (Δx1+Δx2)/2
fin
```

### 1.5.3 Integración de Runge–Kutta

A partir de las secciones anteriores, vemos que la integración rectangular implica el cálculo de un valor de función en cada paso de tiempo, y que la integración trapezoidal implica el cálculo de dos valores de función en cada paso de tiempo. Para mejorar aún más la aproximación de la integral, podemos realizar cálculos de función adicionales en cada paso de tiempo. La integración de Runge–Kutta de orden $n$ es la aproximación de una integral mediante $n$ cálculos de función en cada paso de tiempo. Por lo tanto, la integración rectangular equivale a la integración de Runge–Kutta de primer orden, y la integración trapezoidal equivale a la integración de Runge–Kutta de segundo orden.

El esquema de integración de este tipo más usado es la integración de Runge–Kutta de cuarto orden. Presentamos el algoritmo de integración de Runge–Kutta de cuarto orden (sin su deducción) de la siguiente manera:

$$
\begin{aligned}
\Delta x_1 &= f[x(t_k),u(t_k),t_k]T \\
\Delta x_2 &= f[x(t_k)+\Delta x_1/2,u(t_{k+1/2}),t_{k+1/2}]T \\
\Delta x_3 &= f[x(t_k)+\Delta x_2/2,u(t_{k+1/2}),t_{k+1/2}]T \\
\Delta x_4 &= f[x(t_k)+\Delta x_3,u(t_{k+1}),t_{k+1}]T \\
x(t_{k+1}) &\approx x(t_k)+(\Delta x_1+2\Delta x_2+2\Delta x_3+\Delta x_4)/6
\end{aligned}
\tag{1.116}
$$

donde $t_{k+1/2}=t_k+T/2$. La integración de Runge–Kutta de cuarto orden exige más cómputo que la integración rectangular o la trapezoidal, pero también ofrece una precisión mucho mayor. Esto da el siguiente algoritmo para integrar la dinámica en tiempo continuo mediante integración de Runge–Kutta de cuarto orden. El bucle temporal del algoritmo se ejecuta para $t=0,T,2T,\cdots,t_f-T$.

**Integración de Runge–Kutta de cuarto orden**
```
Suponer que x(0) es conocido
para t = 0 : T : tf − T
    t1 = t + T/2
    Δx1 = f[x(t),u(t),t]T
    Δx2 = f[x(t)+Δx1/2,u(t1),t1]T
    Δx3 = f[x(t)+Δx2/2,u(t1),t1]T
    Δx4 = f[x(t)+Δx3,u(t+T),t+T]T
    x(t+T) = x(t) + (Δx1+2Δx2+2Δx3+Δx4)/6
fin
```

La integración de Runge–Kutta fue inventada por Carl Runge, un matemático y físico alemán, en 1895. Wilhelm Kutta, un matemático y aerodinamicista alemán, la inventó y generalizó de manera independiente en 1901. También se han deducido algoritmos de integración más precisos, que a veces se usan, pero la integración de Runge–Kutta de cuarto orden generalmente se considera un buen equilibrio entre precisión y esfuerzo computacional. Puede encontrarse más información y las deducciones de los algoritmos de integración numérica en muchos textos de análisis numérico, entre ellos [Atk89].

\vspace{\baselineskip}
**EJEMPLO 1.5**

Supongamos que queremos calcular numéricamente $x(t)$ en $t=1$ a partir de la ecuación diferencial

$$\dot x = \cos t \tag{1.117}$$

con la condición inicial $x(0)=0$. Podemos integrar la ecuación analíticamente para hallar que $x(1)=\sin 1\approx 0{,}8415$. Si usamos un esquema de integración numérica, debemos elegir el tamaño de paso $T$. La Tabla 1.1 muestra el error de los métodos de integración rectangular, trapezoidal y de Runge–Kutta de cuarto orden para este ejemplo, para diversos valores de $T$. Como era de esperar, Runge–Kutta es más preciso que trapezoidal, y trapezoidal es más preciso que rectangular.

También, como se esperaba, el error de un método dado disminuye a medida que $T$ disminuye. Sin embargo, quizás la característica más destacable de la Tabla 1.1 sea *cómo* disminuye el error de integración con $T$. Podemos ver que, con la integración rectangular, cuando $T$ se reduce a la mitad, el error de integración también se reduce a la mitad. Con la integración trapezoidal, cuando $T$ se reduce a la mitad, el error de integración disminuye en un factor de cuatro. Con la integración de Runge–Kutta, cuando $T$ se reduce a la mitad, el error de integración disminuye en un factor de 16. Concluimos que (en general) el error de la integración rectangular es proporcional a $T$, el error de la integración trapezoidal es proporcional a $T^2$, y el error de la integración de Runge–Kutta es proporcional a $T^4$.

**Tabla 1.1.** Resultados del Ejemplo 1.5. Errores porcentuales al integrar numéricamente $\dot x=\cos t$ desde $t=0$ hasta $t=1$, para varios algoritmos de integración y para varios tamaños de paso $T$.

| | $T=0{,}1$ | $T=0{,}05$ | $T=0{,}025$ |
|---|---|---|---|
| Rectangular | 2,6 | 1,3 | 0,68 |
| Trapezoidal | 0,083 | 0,021 | 0,0052 |
| Runge–Kutta de cuarto orden | $3{,}5\times10^{-6}$ | $2{,}2\times10^{-7}$ | $1{,}4\times10^{-8}$ |

▽▽▽

## 1.6 ESTABILIDAD

En esta sección repasamos el concepto de estabilidad para sistemas lineales invariantes en el tiempo. Primero abordamos los sistemas en tiempo continuo, en la Sección 1.6.1, y luego los sistemas en tiempo discreto, en la Sección 1.6.2. Aquí enunciamos los resultados importantes sin demostración. El lector interesado puede consultar libros estándar de sistemas lineales para más detalles y resultados adicionales [Kai80, Bay99, Che99].

### 1.6.1 Sistemas en tiempo continuo

Consideremos el sistema lineal, en tiempo continuo, de entrada nula

$$
\begin{aligned}
\dot x &= Ax \\
y &= Cx
\end{aligned}
\tag{1.118}
$$

Las definiciones de estabilidad marginal y estabilidad asintótica son las siguientes.

**Definición 1** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es marginalmente estable si el estado $x(t)$ está acotado para todo $t$ y para todo estado inicial acotado $x(0)$.*

A la estabilidad marginal también se la llama estabilidad de Lyapunov.

**Definición 2** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es asintóticamente estable si, para todo estado inicial acotado $x(0)$,*

$$\lim_{t\to\infty}x(t) = 0 \tag{1.119}$$

Las dos definiciones anteriores muestran que un sistema es marginalmente estable si es asintóticamente estable. Es decir, la estabilidad asintótica es un subconjunto de la estabilidad marginal. La estabilidad marginal y la estabilidad asintótica son tipos de estabilidad interna. Esto se debe a que solo tienen en cuenta el estado del sistema (es decir, la condición interna del sistema) y no consideran la salida del sistema. En algunos libros de sistemas lineales se dan categorías más específicas de estabilidad interna (por ejemplo, la estabilidad uniforme y la estabilidad exponencial).

Dado que la solución de la Ecuación (1.118) está dada por

$$x(t) = \exp(At)x(0) \tag{1.120}$$

podemos enunciar el siguiente teorema.

**Teorema 1** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es marginalmente estable si y solo si*

$$\lim_{t\to\infty}\exp(At) \leq M < \infty \tag{1.121}$$

*para alguna matriz constante $M$. Esto es simplemente una manera de decir que la exponencial matricial no crece sin cota.*

La relación "menor o igual que" del teorema anterior plantea algunas dudas, porque las cantidades a ambos lados de este símbolo matemático son matrices. ¿Qué significa que una matriz sea menor que otra matriz? Puede interpretarse de varias maneras. Por ejemplo, decir que $A<B$ suele interpretarse en el sentido de que $(B-A)$ es definida positiva.⁵ En el teorema anterior, podemos usar cualquier definición razonable de la desigualdad matricial y el teorema seguirá siendo válido.

Puede enunciarse un teorema similar combinando la Definición (2) con la Ecuación (1.120).

**Teorema 2** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es asintóticamente estable si y solo si*

$$\lim_{t\to\infty}\exp(At) = 0 \tag{1.122}$$

Ahora recordemos que $\exp(At)=Q\exp(\hat At)Q^{-1}$, donde $Q$ es una matriz constante que contiene los autovectores de $A$, y $\hat A$ es la forma de Jordan de $A$. Por lo tanto, la exponencial $\exp(\hat At)$ contiene términos como $\exp(\lambda_it)$, $t\exp(\lambda_it)$, $t^2\exp(\lambda_it)$, y así sucesivamente, donde $\lambda_i$ es un autovalor de $A$. Por lo tanto, la acotación de $\exp(At)$ está relacionada con los autovalores de $A$, tal como establecen los siguientes teoremas.

**Teorema 3** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es marginalmente estable si y solo si se cumple una de las siguientes condiciones.*

1. *Todos los autovalores de $A$ tienen parte real negativa.*

2. *Todos los autovalores de $A$ tienen parte real negativa o nula, y aquellos con parte real igual a cero tienen una multiplicidad geométrica igual a su multiplicidad algebraica. Es decir, los bloques de Jordan asociados con los autovalores cuya parte real es igual a cero son de primer orden.*

**Teorema 4** *Un sistema lineal, en tiempo continuo e invariante en el tiempo, es asintóticamente estable si y solo si todos los autovalores de $A$ tienen parte real negativa.*

*⁵A veces la expresión $A<B$ significa que cada elemento de $A$ es menor que el elemento correspondiente de $B$. Sin embargo, en este libro no usaremos esa definición.*

\vspace{\baselineskip}
**EJEMPLO 1.6**

Consideremos el sistema

$$\dot x = \begin{bmatrix}0&1&0\\0&0&0\\0&0&-1\end{bmatrix}x \tag{1.123}$$

Dado que la matriz $A$ es triangular superior, sabemos que sus autovalores están en la diagonal; es decir, los autovalores de $A$ son iguales a 0, 0 y $-1$. Vemos que el sistema es asintóticamente inestable, ya que algunos de los autovalores son no negativos. Notamos también que la matriz $A$ ya está en forma de Jordan, y vemos que el bloque de Jordan correspondiente al autovalor 0 es de segundo orden. Por lo tanto, el sistema también es marginalmente inestable. La solución de este sistema es

$$
\begin{aligned}
x(t) &= \exp(At)x(0) \\
&= \begin{bmatrix}1&t&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}x(0)
\end{aligned}
\tag{1.124}
$$

El elemento de la primera fila y la segunda columna de $\exp(At)$ crece sin cota a medida que $t$ aumenta, por lo que existen algunos estados iniciales $x(0)$ que darán como resultado un $x(t)$ no acotado. Sin embargo, también existen algunos estados iniciales $x(0)$ que darán como resultado un $x(t)$ acotado. Por ejemplo, si $x(0)=\begin{bmatrix}1&0&1\end{bmatrix}^T$, entonces

$$
\begin{aligned}
x(t) &= \begin{bmatrix}1&t&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}\begin{bmatrix}1\\0\\1\end{bmatrix} \\
&= \begin{bmatrix}1\\0\\e^{-t}\end{bmatrix}
\end{aligned}
\tag{1.125}
$$

y $x(t)$ estará acotado para todo $t$. Sin embargo, esto no dice nada sobre la estabilidad del sistema; solo dice que existe algún $x(0)$ que da como resultado un $x(t)$ acotado. Si en cambio elegimos $x(0)=\begin{bmatrix}0&1&0\end{bmatrix}^T$, entonces

$$
\begin{aligned}
x(t) &= \begin{bmatrix}1&t&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}\begin{bmatrix}0\\1\\0\end{bmatrix} \\
&= \begin{bmatrix}t\\1\\0\end{bmatrix}
\end{aligned}
\tag{1.126}
$$

y $x(t)$ crece sin cota. Esto demuestra que el sistema es asintóticamente inestable y marginalmente inestable.

▽▽▽

\vspace{\baselineskip}
**EJEMPLO 1.7**

Consideremos el sistema

$$\dot x = \begin{bmatrix}0&0&0\\0&0&0\\0&0&-1\end{bmatrix}x \tag{1.127}$$

Los autovalores de $A$ son iguales a 0, 0 y $-1$. Vemos que el sistema es asintóticamente inestable, ya que algunos de los autovalores son no negativos. Para ver si el sistema es marginalmente estable, necesitamos calcular la multiplicidad geométrica del autovalor 0. (Esto puede hacerse notando que $A$ ya está en forma de Jordan, pero de todos modos desarrollaremos el ejercicio de manera más completa, a fines ilustrativos.) Al resolver la ecuación

$$(\lambda I-A)v = \begin{bmatrix}0\\0\\0\end{bmatrix} \tag{1.128}$$

(donde $\lambda=0$) para vectores $v$ no nulos, vemos que hay dos soluciones linealmente independientes, dadas por

$$v = \begin{bmatrix}1\\0\\0\end{bmatrix},\begin{bmatrix}0\\1\\0\end{bmatrix} \tag{1.129}$$

Esto muestra que la multiplicidad geométrica del autovalor 0 es igual a 2, lo cual significa que el sistema es marginalmente estable. La solución de este sistema es

$$
\begin{aligned}
x(t) &= \exp(At)x(0) \\
&= \begin{bmatrix}1&0&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}x(0)
\end{aligned}
\tag{1.130}
$$

Independientemente de $x(0)$, vemos que $x(t)$ siempre estará acotado, lo cual significa que el sistema es marginalmente estable. Nótese que $x(t)$ puede aproximarse a 0 a medida que $t$ aumenta, dependiendo del valor de $x(0)$. Por ejemplo, si $x(0)=\begin{bmatrix}0&0&-1\end{bmatrix}^T$, entonces

$$
\begin{aligned}
x(t) &= \begin{bmatrix}1&0&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}\begin{bmatrix}0\\0\\-1\end{bmatrix} \\
&= \begin{bmatrix}0\\0\\-e^{-t}\end{bmatrix}
\end{aligned}
\tag{1.131}
$$

y $x(t)$ se aproxima a 0 a medida que $t$ aumenta. Sin embargo, esto no dice nada sobre la estabilidad asintótica del sistema; solo dice que existe algún $x(0)$ que da como resultado un estado $x(t)$ que se aproxima asintóticamente a 0. Si en cambio elegimos $x(0)=\begin{bmatrix}0&1&0\end{bmatrix}^T$, entonces

$$
\begin{aligned}
x(t) &= \begin{bmatrix}1&0&0\\0&1&0\\0&0&e^{-t}\end{bmatrix}\begin{bmatrix}0\\1\\0\end{bmatrix} \\
&= \begin{bmatrix}0\\1\\0\end{bmatrix}
\end{aligned}
\tag{1.132}
$$

y $x(t)$ no se aproxima a 0. Esto demuestra que el sistema es asintóticamente inestable.

▽▽▽

### 1.6.2 Sistemas en tiempo discreto

Consideremos el sistema lineal, en tiempo discreto e invariante en el tiempo, de entrada nula

$$
\begin{aligned}
x_{k+1} &= Fx_k \\
y_k &= Hx_k
\end{aligned}
\tag{1.133}
$$

Las definiciones de estabilidad marginal (también llamada estabilidad de Lyapunov) y de estabilidad asintótica son análogas a las definiciones para sistemas en tiempo continuo dadas en la Sección 1.6.1.

**Definición 3** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es marginalmente estable si el estado $x_k$ está acotado para todo $k$ y para todo estado inicial acotado $x_0$.*

**Definición 4** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es asintóticamente estable si*

$$\lim_{k\to\infty}x_k = 0 \tag{1.134}$$

*para todo estado inicial acotado $x_0$.*

La estabilidad marginal y la estabilidad asintótica son tipos de estabilidad interna. Esto se debe a que solo tienen en cuenta el estado del sistema (es decir, la condición interna del sistema) y no consideran la salida del sistema. En algunos libros de sistemas lineales se dan categorías más específicas de estabilidad interna (por ejemplo, la estabilidad uniforme y la estabilidad exponencial).

Dado que la solución de la Ecuación (1.133) está dada por

$$x_k = A^kx_0 \tag{1.135}$$

podemos enunciar los siguientes teoremas.

**Teorema 5** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es marginalmente estable si y solo si*

$$\lim_{k\to\infty}A^k \leq M < \infty \tag{1.136}$$

*para alguna matriz constante $M$. Esto es simplemente una manera de decir que las potencias de $A$ no crecen sin cota.*

**Teorema 6** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es asintóticamente estable si y solo si*

$$\lim_{k\to\infty}A^k = 0 \tag{1.137}$$

Ahora recordemos que $A^k=Q\hat A^kQ^{-1}$, donde $Q$ es una matriz constante que contiene los autovectores de $A$, y $\hat A$ es la forma de Jordan de $A$. Por lo tanto, la matriz $\hat A^k$ contiene términos como $\lambda_i^k$, $k\lambda_i^k$, $k^2\lambda_i^k$, y así sucesivamente, donde $\lambda_i$ es un autovalor de $A$. Por lo tanto, la acotación de $A^k$ está relacionada con los autovalores de $A$, tal como establecen los siguientes teoremas.

**Teorema 7** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es marginalmente estable si y solo si se cumple una de las siguientes condiciones.*

1. *Todos los autovalores de $A$ tienen magnitud menor que uno.*

2. *Todos los autovalores de $A$ tienen magnitud menor o igual que uno, y aquellos con magnitud igual a uno tienen una multiplicidad geométrica igual a su multiplicidad algebraica. Es decir, los bloques de Jordan asociados con los autovalores cuya magnitud es igual a uno son de primer orden.*

**Teorema 8** *Un sistema lineal, en tiempo discreto e invariante en el tiempo, es asintóticamente estable si y solo si todos los autovalores de $A$ tienen magnitud menor que uno.*

## 1.7 CONTROLABILIDAD Y OBSERVABILIDAD

Los conceptos de controlabilidad y observabilidad son fundamentales para la teoría de control moderna. Estos conceptos definen qué tan bien podemos controlar un sistema (es decir, llevar el estado a un valor deseado) y qué tan bien podemos observar un sistema (es decir, determinar las condiciones iniciales después de medir las salidas). Estos conceptos también son importantes para algunos de los resultados teóricos relacionados con la estimación óptima de estado que encontraremos más adelante en este libro.

### 1.7.1 Controlabilidad

Las siguientes definiciones y teoremas dan definiciones rigurosas de controlabilidad para sistemas lineales, tanto en el caso de tiempo continuo como en el de tiempo discreto.

**Definición 5** *Un sistema en tiempo continuo es controlable si, para cualquier estado inicial $x(0)$ y cualquier instante final $t>0$, existe un control que transfiere el estado a cualquier valor deseado en el instante $t$.*

**Definición 6** *Un sistema en tiempo discreto es controlable si, para cualquier estado inicial $x_0$ y algún instante final $k$, existe un control que transfiere el estado a cualquier valor deseado en el instante $k$.*

Nótese que la definición de controlabilidad en el caso de tiempo continuo es mucho más exigente que la definición en el caso de tiempo discreto. En el caso de tiempo continuo, se exige la existencia de un control para *cualquier* instante final. En el caso de tiempo discreto, se exige la existencia de un control para *algún* instante final. En ambos casos, la controlabilidad es independiente de la ecuación de salida.

Existen varias pruebas para la controlabilidad. Los siguientes teoremas equivalentes pueden usarse para probar la controlabilidad de sistemas lineales continuos e invariantes en el tiempo.

**Teorema 9** *El sistema lineal continuo e invariante en el tiempo de $n$ estados⁶, $\dot x=Ax+Bu$, tiene la matriz de controlabilidad $P$ definida por*

$$P = \begin{bmatrix}B&AB&\cdots&A^{n-1}B\end{bmatrix} \tag{1.138}$$

*El sistema es controlable si y solo si $\rho(P)=n$.*

**Teorema 10** *El sistema lineal continuo e invariante en el tiempo de $n$ estados, $\dot x=Ax+Bu$, es controlable si y solo si el gramiano de controlabilidad definido por*

$$\int_0^t e^{A\tau}BB^Te^{A^T\tau}\,d\tau \tag{1.139}$$

*es definido positivo para algún $t\in(0,\infty)$.*

*⁶La notación sistema de n estados indica un sistema que tiene $n$ elementos en su variable de estado $x$.*

**Teorema 11** *El sistema lineal continuo e invariante en el tiempo de $n$ estados, $\dot x=Ax+Bu$, es controlable si y solo si la ecuación diferencial de Lyapunov*

$$
\begin{aligned}
W(0) &= 0 \\
\dot W &= WA^T+AW+BB^T
\end{aligned}
\tag{1.140}
$$

*tiene una solución definida positiva $W(t)$ para algún $t\in(0,\infty)$. A esto también se lo llama una ecuación de Sylvester.*

De manera similar al caso de tiempo continuo, los siguientes teoremas equivalentes pueden usarse para probar la controlabilidad de sistemas lineales discretos e invariantes en el tiempo.

**Teorema 12** *El sistema lineal discreto e invariante en el tiempo de $n$ estados, $x_k=Fx_{k-1}+Gu_{k-1}$, tiene la matriz de controlabilidad $P$ definida por*

$$P = \begin{bmatrix}G&FG&\cdots&F^{n-1}G\end{bmatrix} \tag{1.141}$$

*El sistema es controlable si y solo si $\rho(P)=n$.*

**Teorema 13** *El sistema lineal discreto e invariante en el tiempo de $n$ estados, $x_k=Fx_{k-1}+Gu_{k-1}$, es controlable si y solo si el gramiano de controlabilidad definido por*

$$\sum_{i=0}^{k}A^{k-i}BB^T(A^T)^{k-i} \tag{1.142}$$

*es definido positivo para algún $k\in(0,\infty)$.*

**Teorema 14** *El sistema lineal discreto e invariante en el tiempo de $n$ estados, $x_k=Fx_{k-1}+Gu_{k-1}$, es controlable si y solo si la ecuación de Lyapunov en diferencias*

$$
\begin{aligned}
W_0 &= 0 \\
W_{i+1} &= FW_iF^T+GG^T
\end{aligned}
\tag{1.143}
$$

*tiene una solución definida positiva $W_k$ para algún $k\in(0,\infty)$. A esto también se lo llama una ecuación de Stein.*

Nótese que los Teoremas 9 y 12 dan pruebas idénticas de controlabilidad tanto para sistemas en tiempo continuo como para sistemas en tiempo discreto. En general, estas son las pruebas de controlabilidad más simples. Pueden obtenerse pruebas de controlabilidad para sistemas lineales variantes en el tiempo generalizando los teoremas anteriores. La controlabilidad para sistemas no lineales es mucho más difícil de formalizar.

\vspace{\baselineskip}
**EJEMPLO 1.8**

El circuito RLC de la Figura 1.4 tiene la siguiente descripción de sistema

$$\begin{bmatrix}\dot v_C\\ \dot i_L\end{bmatrix} = \begin{bmatrix}-2/RC & 1/C \\ -1/L & 0\end{bmatrix}\begin{bmatrix}v_C\\i_L\end{bmatrix} + \begin{bmatrix}1/RC\\1/L\end{bmatrix}u \tag{1.144}$$

donde $v_C$ es la tensión en el capacitor, $i_L$ es la corriente que circula por el inductor, y $u$ es la tensión aplicada. Usaremos el Teorema 9 para determinar las condiciones bajo las cuales este sistema es controlable. La matriz de controlabilidad se calcula como

$$
\begin{aligned}
P &= \begin{bmatrix}B&AB\end{bmatrix} \\
&= \begin{bmatrix}1/RC & 1/LC-2/R^2C^2 \\ 1/L & -1/RLC\end{bmatrix}
\end{aligned}
\tag{1.145}
$$

A partir de esto podemos calcular el determinante de $P$ como

$$|P| = 1/R^2LC^2-1/L^2C \tag{1.146}$$

El determinante de $P$ es 0 únicamente si $R=\sqrt{L/C}$. Entonces, el sistema es controlable a menos que $R=\sqrt{L/C}$. Sería muy difícil obtener este resultado a partir de los Teoremas 10 y 11.

**Figura 1.4.** Circuito RLC para el Ejemplo 1.8. *(Diagrama no reproducido: circuito RLC con una fuente de tensión $u$, un capacitor $C$, un inductor $L$ y dos resistores $R$.)*

▽▽▽

### 1.7.2 Observabilidad

Las siguientes definiciones y teoremas dan definiciones rigurosas de observabilidad para sistemas lineales, tanto en el caso de tiempo continuo como en el de tiempo discreto.

**Definición 7** *Un sistema en tiempo continuo es observable si, para cualquier estado inicial $x(0)$ y cualquier instante final $t>0$, el estado inicial $x(0)$ puede determinarse de manera única a partir del conocimiento de la entrada $u(\tau)$ y la salida $y(\tau)$ para todo $\tau\in[0,t]$.*

**Definición 8** *Un sistema en tiempo discreto es observable si, para cualquier estado inicial $x_0$ y algún instante final $k$, el estado inicial $x_0$ puede determinarse de manera única a partir del conocimiento de la entrada $u_i$ y la salida $y_i$ para todo $i\in[0,k]$.*

Nótese que la definición de observabilidad en el caso de tiempo continuo es mucho más exigente que la definición en el caso de tiempo discreto. En el caso de tiempo continuo, el estado inicial debe poder determinarse en *cualquier* instante final. En el caso de tiempo discreto, el estado inicial debe poder determinarse en *algún* instante final. Si un sistema es observable, entonces el estado inicial puede determinarse, y si el estado inicial puede determinarse, entonces pueden determinarse todos los estados entre el instante inicial y el instante final.

Existen varias pruebas para la controlabilidad. Los siguientes teoremas equivalentes pueden usarse para probar la controlabilidad de sistemas lineales continuos e invariantes en el tiempo.

**Teorema 15** *El sistema lineal continuo e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
\dot x &= Ax+Bu \\
y &= Cx
\end{aligned}
\tag{1.147}
$$

*tiene la matriz de observabilidad $Q$ definida por*

$$Q = \begin{bmatrix}C\\CA\\ \vdots\\CA^{n-1}\end{bmatrix} \tag{1.148}$$

*El sistema es observable si y solo si $\rho(Q)=n$.*

**Teorema 16** *El sistema lineal continuo e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
\dot x &= Ax+Bu \\
y &= Cx
\end{aligned}
\tag{1.149}
$$

*es observable si y solo si el gramiano de observabilidad definido por*

$$\int_0^t e^{A^T\tau}C^TCe^{A\tau}\,d\tau \tag{1.150}$$

*es definido positivo para algún $t\in(0,\infty)$.*

**Teorema 17** *El sistema lineal continuo e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
\dot x &= Ax+Bu \\
y &= Cx
\end{aligned}
\tag{1.151}
$$

*es observable si y solo si la ecuación diferencial de Lyapunov*

$$
\begin{aligned}
W(t) &= 0 \\
-\dot W &= WA+A^TW+C^TC
\end{aligned}
\tag{1.152}
$$

*tiene una solución definida positiva $W(\tau)$ para algún $\tau\in(0,t)$. A esto también se lo llama una ecuación de Sylvester.*

De manera similar al caso de tiempo continuo, los siguientes teoremas equivalentes pueden usarse para probar la observabilidad de sistemas lineales discretos e invariantes en el tiempo.

**Teorema 18** *El sistema lineal discreto e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
x_k &= Fx_{k-1}+Gu_{k-1} \\
y_k &= Hx_k
\end{aligned}
\tag{1.153}
$$

*tiene la matriz de observabilidad $Q$ definida por*

$$Q = \begin{bmatrix}H\\HF\\ \vdots\\HF^{n-1}\end{bmatrix} \tag{1.154}$$

*El sistema es observable si y solo si $\rho(Q)=n$.*

**Teorema 19** *El sistema lineal discreto e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
x_k &= Fx_{k-1}+Gu_{k-1} \\
y_k &= Hx_k
\end{aligned}
\tag{1.155}
$$

*es observable si y solo si el gramiano de observabilidad definido por*

$$\sum_{i=0}^{k}(F^T)^iH^THF^i \tag{1.156}$$

*es definido positivo para algún $k\in(0,\infty)$.*

**Teorema 20** *El sistema lineal discreto e invariante en el tiempo de $n$ estados*

$$
\begin{aligned}
x_k &= Fx_{k-1}+Gu_{k-1} \\
y_k &= Hx_k
\end{aligned}
\tag{1.157}
$$

*es observable si y solo si la ecuación de Lyapunov en diferencias*

$$
\begin{aligned}
W_k &= 0 \\
W_i &= F^TW_{i+1}F+H^TH
\end{aligned}
\tag{1.158}
$$

*tiene una solución definida positiva $W_0$ para algún $k\in(0,\infty)$. A esto también se lo llama una ecuación de Stein.*

Nótese que los Teoremas 15 y 18 dan pruebas idénticas de observabilidad tanto para sistemas en tiempo continuo como para sistemas en tiempo discreto. En general, estas son las pruebas de observabilidad más simples. Pueden obtenerse pruebas de observabilidad para sistemas lineales variantes en el tiempo generalizando los teoremas anteriores. La observabilidad para sistemas no lineales es mucho más difícil de formalizar.

\vspace{\baselineskip}
**EJEMPLO 1.9**

El circuito RLC del Ejemplo 1.8 tiene la siguiente descripción de sistema

$$
\begin{aligned}
\begin{bmatrix}\dot v_C\\ \dot i_L\end{bmatrix} &= \begin{bmatrix}-2/RC & 1/C \\ -1/L & 0\end{bmatrix}\begin{bmatrix}v_C\\i_L\end{bmatrix} + \begin{bmatrix}1/RC\\1/L\end{bmatrix}u \\
y &= \begin{bmatrix}-1&0\end{bmatrix}\begin{bmatrix}v_C\\i_L\end{bmatrix}
\end{aligned}
\tag{1.159}
$$

donde $v_C$ es la tensión en el capacitor, $i_L$ es la corriente que circula por el inductor, y $u$ es la tensión aplicada. Usaremos el Teorema 15 para determinar las condiciones bajo las cuales este sistema es observable. La matriz de observabilidad se calcula como

$$Q = \begin{bmatrix}C\\CA\end{bmatrix} = \begin{bmatrix}-1&0\\2/RC&-1/C\end{bmatrix} \tag{1.160}$$

El determinante de la matriz de observabilidad puede calcularse como

$$|Q| = 1/C \tag{1.161}$$

El determinante de $Q$ es distinto de cero, por lo que el sistema es observable. Por otro lado, supongamos que $R=L=C=1$ y que la ecuación de salida es

$$y = \begin{bmatrix}-1&1\end{bmatrix}\begin{bmatrix}v_C\\i_L\end{bmatrix} \tag{1.162}$$

Entonces la matriz de observabilidad puede calcularse como

$$
\begin{aligned}
Q &= \begin{bmatrix}-1&1\\1&-1\end{bmatrix} \\
|Q| &= 0
\end{aligned}
\tag{1.163}
$$

Por lo tanto, el sistema es no observable. Sería muy difícil obtener este resultado a partir de los Teoremas 16 y 17.

▽▽▽

### 1.7.3 Estabilizabilidad y detectabilidad

Los conceptos de estabilizabilidad y detectabilidad están estrechamente relacionados con la controlabilidad y la observabilidad, respectivamente. Estos conceptos también están relacionados con los modos de un sistema. Los modos de un sistema son todos los estados desacoplados una vez que el sistema se transforma a la forma de Jordan. Un sistema puede transformarse a la forma de Jordan de la siguiente manera. Consideremos el sistema

$$
\begin{aligned}
\dot x &= Ax+Bu \\
y &= Cx+Du
\end{aligned}
\tag{1.164}
$$

Primero hallemos los datos propios de la matriz del sistema $A$. Supongamos que los autovectores se denotan como $v_1,\cdots,v_n$. Formemos una matriz $M$ de $n\times n$ agrupando los autovectores de la siguiente manera.

$$M = \begin{bmatrix}v_1&\cdots&v_n\end{bmatrix} \tag{1.165}$$

Definamos un nuevo sistema como

$$
\begin{aligned}
\dot{\bar x} &= M^{-1}AM\bar x+M^{-1}Bu \\
&= \bar A\bar x+\bar Bu \\
y &= CM\bar x+Du \\
&= \bar C\bar x+Du
\end{aligned}
\tag{1.166}
$$

Al nuevo sistema se lo llama la representación en forma de Jordan del sistema original. Nótese que la matriz $M$ siempre será invertible, porque los autovectores de una matriz siempre pueden elegirse de manera que sean linealmente independientes. A los dos sistemas de las Ecuaciones (1.164) y (1.166) se los llama sistemas algebraicamente equivalentes. Esto se debe a que tienen la misma entrada y la misma salida (y, por lo tanto, tienen la misma función de transferencia), pero tienen estados diferentes.

\vspace{\baselineskip}
**EJEMPLO 1.10**

Consideremos el sistema

$$
\begin{aligned}
\dot x &= Ax+Bu \\
&= \begin{bmatrix}1&1&2\\0&1&3\\0&0&-2\end{bmatrix}x + \begin{bmatrix}1\\1\\0\end{bmatrix}u \\
y &= Cx+Du \\
&= \begin{bmatrix}1&0&0\end{bmatrix}+2u
\end{aligned}
\tag{1.167}
$$

Este sistema tiene la misma función de transferencia que

$$
\begin{aligned}
\dot{\bar x} &= \bar A\bar x+\bar Bu \\
&= \begin{bmatrix}1&1&0\\0&1&0\\0&0&-2\end{bmatrix}x + \begin{bmatrix}1\\1\\0\end{bmatrix}u \\
y &= \bar C\bar x+Du \\
&= \begin{bmatrix}1&0&1\end{bmatrix}\bar x+2u
\end{aligned}
\tag{1.168}
$$

La matriz de autovectores de $A$ es

$$
\begin{aligned}
M &= \begin{bmatrix}v_1&v_2&v_n\end{bmatrix} \\
&= \begin{bmatrix}1&0&1\\0&1&3\\0&0&-3\end{bmatrix}
\end{aligned}
\tag{1.169}
$$

Nótense las equivalencias

$$
\begin{aligned}
\bar A &= M^{-1}AM \\
\bar B &= M^{-1}B \\
\bar C &= CM
\end{aligned}
\tag{1.170}
$$

El sistema en forma de Jordan tiene dos modos desacoplados. El primer modo es

$$
\begin{aligned}
\dot{\bar x}_1 &= \begin{bmatrix}1&1\\0&1\end{bmatrix}\bar x_1+\begin{bmatrix}1\\1\end{bmatrix}u \\
y_1 &= \begin{bmatrix}1&0\end{bmatrix}\bar x_1
\end{aligned}
\tag{1.171}
$$

El segundo modo es

$$
\begin{aligned}
\dot{\bar x}_2 &= -2\bar x_2+0\,u \\
y_2 &= \bar x_2
\end{aligned}
\tag{1.172}
$$

▽▽▽

**Definición 9** *Si un sistema es controlable o estable, entonces también es estabilizable. Si un sistema es no controlable o inestable, entonces es estabilizable si sus modos no controlables son estables.*

En el Ejemplo 1.10, el primer modo es inestable (ambos autovalores en $+1$) pero controlable. El segundo modo es estable (autovalor en $-2$) pero no controlable. Por lo tanto, el sistema es estabilizable.

**Definición 10** *Si un sistema es observable o estable, entonces también es detectable. Si un sistema es no observable o inestable, entonces es detectable si sus modos no observables son estables.*

En el Ejemplo 1.10, el primer modo es inestable pero observable. El segundo modo es estable y observable a la vez. Por lo tanto, el sistema es detectable.

La controlabilidad y la observabilidad fueron introducidas por Rudolph Kalman en una conferencia de 1959, cuyas actas se publicaron en una revista mexicana poco conocida en 1960 [Kal60b]. El material también se presentó en una conferencia de la IFAC en 1960 [Kal60c], y finalmente se publicó en un formato más ampliamente accesible en 1963 [Kal63].

## 1.8 RESUMEN

En este capítulo repasamos algunos de los conceptos básicos de la teoría de sistemas lineales que resultan fundamentales para muchos de los enfoques de la estimación óptima de estado. Comenzamos con un repaso del álgebra matricial y del cálculo matricial, que resulta indispensable en gran parte de la teoría de las técnicas de estimación de estado. Para obtener información adicional sobre la teoría de matrices, el lector puede consultar varios textos excelentes [Hor85, Gol89, Ber05]. Continuamos este capítulo con un repaso de los sistemas lineales y no lineales, tanto en tiempo continuo como en tiempo discreto. Consideramos el tiempo como continuo para los sistemas físicos, pero nuestros algoritmos de simulación y de estimación operan en tiempo discreto debido a la popularidad de la computación digital. Analizamos la discretización de sistemas en tiempo continuo, que es una manera de obtener una representación matemática en tiempo discreto de un sistema en tiempo continuo. El concepto de estabilidad puede usarse para indicarnos si los estados de un sistema permanecerán siempre acotados. La controlabilidad nos indica si es posible hallar una entrada de control que fuerce a los estados del sistema a alcanzar los valores que deseamos, y la observabilidad nos indica si es posible determinar el estado inicial de un sistema a partir de mediciones de la salida. La teoría del espacio de estados en general, y la teoría de sistemas lineales en particular, es una disciplina de amplio alcance que suele cubrirse en un curso de posgrado de un semestre, aunque hay material más que suficiente para llenar un curso de dos semestres. Se han escrito muchos libros de texto excelentes sobre el tema, entre ellos [Bay99, Che99, Kai00] y otros. Una sólida comprensión de los sistemas lineales brindará una base firme para estudios posteriores en áreas como la teoría de control, la teoría de la estimación, y el procesamiento de señales.

## PROBLEMAS

### Ejercicios escritos

**1.1** Halle el rango de la matriz $\begin{bmatrix}0&0\\0&0\end{bmatrix}$.

**1.2** Halle dos matrices $A$ y $B$ de $2\times 2$ tales que $A\neq B$, ni $A$ ni $B$ sean diagonales, $A\neq cB$ para ningún escalar $c$, y $AB=BA$. Halle los autovectores de $A$ y
