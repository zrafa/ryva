---

## 13.3 ENFOQUES DE ORDEN SUPERIOR

Pueden usarse técnicas de linealización más refinadas para reducir el error de linealización en el EKF, en sistemas altamente no lineales. En esta sección deduciremos e ilustraremos dos de esos enfoques: el EKF iterado y el EKF de segundo orden. También analizaremos brevemente otros enfoques, entre ellos los filtros de suma de gaussianas y los filtros de rejilla.

### 13.3.1 El filtro de Kalman extendido iterado

En esta sección analizaremos el EKF iterado. Aquí limitaremos nuestro análisis al filtrado de tiempo discreto, aunque los conceptos pueden extenderse fácilmente a filtros de tiempo continuo o híbridos.

Cuando dedujimos el EKF de tiempo discreto en la sección 13.2.3, aproximamos $h(x_k,v_k)$ expandiéndola en una serie de Taylor alrededor de $\hat{x}_k^-$, como se muestra en la ecuación (13.41):

$$
\begin{aligned}
h(x_k,v_k) &= h(\hat{x}_k^-,0)+\frac{\partial h}{\partial x}\bigg|_{\hat{x}_k^-}(x_k-\hat{x}_k^-)+\frac{\partial h}{\partial v}\bigg|_{\hat{x}_k^-}v_k \\
&= h(\hat{x}_k^-,0)+H_k(x_k-\hat{x}_k^-)+M_kv_k
\end{aligned}
\tag{13.50}
$$

A partir de esta linealización, luego escribimos las ecuaciones de actualización de la medición, como se muestra en la ecuación (13.43):

$$
\begin{aligned}
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+M_kR_kM_k^T)^{-1} \\
P_k^+ &= (I-K_kH_k)P_k^- \\
\hat{x}_k^+ &= \hat{x}_k^-+K_k[y_k-h_k(\hat{x}_k^-,0)]
\end{aligned}
\tag{13.51}
$$

La razón por la que expandimos $h(x_k)$ alrededor de $\hat{x}_k^-$ fue que esa era nuestra mejor estimación de $x_k$ antes de tener en cuenta la medición en el instante $k$. Pero después de implementar las ecuaciones del EKF discreto para obtener la estimación *a posteriori* $\hat{x}_k^+$, tenemos una mejor estimación de $x_k$. Entonces podemos reducir el error de linealización reformulando la expansión en serie de Taylor de $h(x_k)$ alrededor de nuestra nueva estimación. Si luego usamos esa nueva expansión en serie de Taylor de $h(x_k)$ y recalculamos las ecuaciones de actualización de la medición, deberíamos obtener una mejor estimación *a posteriori* de $\hat{x}_k^+$. Pero entonces podemos repetir el paso anterior; dado que tenemos una estimación aún mejor de $x_k$, podemos nuevamente reformular la expansión de $h(x_k)$ alrededor de esta estimación aún mejor, para obtener una estimación todavía mejor. Este proceso puede repetirse tantas veces como se desee, aunque en la mayoría de los problemas la mayor parte de la mejora posible se obtiene relinealizando una sola vez.

Usamos la notación $\hat{x}_{k,i}^+$ para referirnos a la estimación *a posteriori* de $x_k$ después de haber realizado $i$ relinealizaciones. Así, $\hat{x}_{k,0}$ es la estimación *a posteriori* que resulta de aplicar el EKF estándar. Del mismo modo, usamos $P_{k,i}^+$ para referirnos a la covarianza aproximada del error de estimación de $\hat{x}_{k,i}^+$, $K_{k,i}$ para referirnos a la ganancia de Kalman que se usa durante el $i$-ésimo paso de relinealización, y $H_{k,i}$ para referirnos a la matriz de derivadas parciales evaluada en $x_k=\hat{x}_{k,i}^+$.

Con esta notación, podemos describir un algoritmo para el EKF iterado de la siguiente manera. Primero, en cada instante $k$ inicializamos la estimación del EKF iterado con la estimación del EKF estándar:

$$
\begin{aligned}
\hat{x}_{k,0}^+ &= \hat{x}_k^+ \\
P_{k,0}^+ &= P_k^+
\end{aligned}
\tag{13.52}
$$

Segundo, para $i=0,1,\cdots,N$, evaluar las siguientes ecuaciones:

$$
\begin{aligned}
H_{k,i} &= \frac{\partial h}{\partial x}\bigg|_{\hat{x}_{k,i}^+} \\
K_{k,i} &= P_k^-H_{k,i}^T(H_{k,i}P_k^-H_{k,i}^T+M_kR_kM_k^T)^{-1} \\
P_{k,i+1}^+ &= (I-K_{k,i}H_{k,i})P_k^- \\
\hat{x}_{k,i+1}^+ &= \hat{x}_k^-+K_{k,i}[y_k-h_k(\hat{x}_k^-)]
\end{aligned}
\tag{13.53}
$$

Esto se hace durante tantos pasos como se desee, para mejorar la linealización. Si $N=0$, entonces el EKF iterado se reduce al EKF estándar.

Todavía debemos hacer una modificación más a las ecuaciones anteriores para obtener el filtro de Kalman iterado. Recordemos que, en la deducción del EKF, la ecuación de actualización de la medición para $\hat{x}$ se dedujo originalmente a partir de la siguiente expansión en serie de Taylor de primer orden de la ecuación de medición:

$$
\begin{aligned}
y_k &= h(x_k) \\
&\approx h(\hat{x}_k^-)+H\big|_{\hat{x}_k^-}(x_k-\hat{x}_k^-)
\end{aligned}
\tag{13.54}
$$

Para deducir la ecuación de actualización de la medición para $\hat{x}$, evaluamos el lado derecho en la estimación *a priori* $\hat{x}_k^-$ y lo restamos de $y_k$ para obtener nuestro término de corrección (el residuo):

$$
\begin{aligned}
r_k &= y_k-h(\hat{x}_k^-)-H\big|_{\hat{x}_k^-}(\hat{x}_k^--\hat{x}_k^-) \\
&= y_k-h(\hat{x}_k^-)
\end{aligned}
\tag{13.55}
$$

Con el EKF iterado, en cambio, queremos expandir la ecuación de medición alrededor de $\hat{x}_{k,i}^+$, de la siguiente manera:

$$
y_k \approx h(\hat{x}_{k,i}^+)+H\big|_{\hat{x}_{k,i}^+}(x_k-\hat{x}_{k,i}^+)
\tag{13.56}
$$

Para deducir la ecuación de actualización de la medición del EKF iterado para $\hat{x}$, evaluamos el lado derecho de la ecuación anterior en la estimación *a priori* $\hat{x}_k^-$, y lo restamos de $y_k$ para obtener nuestro término de corrección:

$$
r_k = y_k-h(\hat{x}_{k,i}^+)-H_{k,i}(\hat{x}_k^--\hat{x}_{k,i}^+)
\tag{13.57}
$$

Esto da la ecuación de actualización del EKF iterado para $\hat{x}$ como

$$
\hat{x}_{k,i+1}^+ = \hat{x}_k^-+K_{k,i}[y_k-h(\hat{x}_{k,i}^+)-H_{k,i}(\hat{x}_k^--\hat{x}_{k,i}^+)]
\tag{13.58}
$$

El EKF iterado puede entonces resumirse de la siguiente manera.

**El filtro de Kalman extendido iterado**

1. Las ecuaciones no lineales del sistema y de medición están dadas por:

$$
\begin{aligned}
x_k &= f_{k-1}(x_{k-1},u_{k-1},w_{k-1}) \\
y_k &= h_k(x_k,v_k) \\
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.59}
$$

2. Inicializar el filtro de la siguiente manera.

$$
\begin{aligned}
\hat{x}_0^+ &= E(x_0) \\
P_0^+ &= E\left[(x_0-\hat{x}_0)(x_0-\hat{x}_0)^T\right]
\end{aligned}
\tag{13.60}
$$

3. Para $k=1,2,\cdots$, realizar lo siguiente.

(a) Realizar las siguientes ecuaciones de actualización temporal:

$$
\begin{aligned}
P_k^- &= F_{k-1}P_{k-1}^+F_{k-1}^T+L_{k-1}Q_{k-1}L_{k-1}^T \\
\hat{x}_k^- &= f_{k-1}(\hat{x}_{k-1}^+,u_{k-1},0)
\end{aligned}
\tag{13.61}
$$

donde las matrices de derivadas parciales $F_{k-1}$ y $L_{k-1}$ se definen de la siguiente manera:

$$
\begin{aligned}
F_{k-1} &= \frac{\partial f_{k-1}}{\partial x}\bigg|_{\hat{x}_{k-1}^+} \\
L_{k-1} &= \frac{\partial f_{k-1}}{\partial w}\bigg|_{\hat{x}_{k-1}^+}
\end{aligned}
\tag{13.62}
$$

Hasta este punto, el EKF iterado es igual al EKF estándar de tiempo discreto.

(b) Realizar la actualización de la medición, inicializando la estimación del EKF iterado con la estimación del EKF estándar:

$$
\begin{aligned}
\hat{x}_{k,0}^+ &= \hat{x}_k^- \\
P_{k,0}^+ &= P_k^-
\end{aligned}
\tag{13.63}
$$

Para $i=0,1,\cdots,N$, evaluar las siguientes ecuaciones (donde $N$ es la cantidad deseada de iteraciones de actualización de la medición):

$$
\begin{aligned}
H_{k,i} &= \frac{\partial h}{\partial x}\bigg|_{\hat{x}_{k,i}^+} \\
M_{k,i} &= \frac{\partial h}{\partial v}\bigg|_{\hat{x}_{k,i}^+} \\
K_{k,i} &= P_k^-H_{k,i}^T(H_{k,i}P_k^-H_{k,i}^T+M_{k,i}R_kM_{k,i}^T)^{-1} \\
P_{k,i+1}^+ &= (I-K_{k,i}H_{k,i})P_k^- \\
\hat{x}_{k,i+1}^+ &= \hat{x}_k^-+K_{k,i}\left[y_k-h(\hat{x}_{k,i}^+)-H_{k,i}(\hat{x}_k^--\hat{x}_{k,i}^+)\right]
\end{aligned}
\tag{13.64}
$$

(c) La estimación final de estado *a posteriori*, y la covarianza del error de estimación, están dadas por:

$$
\begin{aligned}
\hat{x}_k^+ &= \hat{x}_{k,N+1}^+ \\
P_k^+ &= P_{k,N+1}^+
\end{aligned}
\tag{13.65}
$$

En el ejemplo 13.3 se presentará una ilustración del EKF iterado.

### 13.3.2 El filtro de Kalman extendido de segundo orden

El EKF de segundo orden es similar al EKF iterado, en el sentido de que intenta reducir el error de linealización del EKF. En el EKF iterado de la sección anterior, refinamos el punto en el que realizábamos una expansión en serie de Taylor de primer orden de la ecuación de medición $h(\cdot)$. En el EKF de segundo orden, en cambio, realizamos una expansión en serie de Taylor de segundo orden de $f(\cdot)$ y de $h(\cdot)$. El EKF de segundo orden presentado en esta sección está basado en [Ath68, Gel74].

En esta sección consideraremos el sistema híbrido, con dinámica del sistema en tiempo continuo y mediciones en tiempo discreto:

$$
\begin{aligned}
\dot{x} &= f(x,u,w,t) \\
y_k &= h(x_k,t_k)+v_k \\
w(t) &\sim (0,Q) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.66}
$$

En el EKF estándar, expandimos $f(x,u,w,t)$ usando una serie de Taylor de primer orden. En esta sección consideraremos únicamente la expansión alrededor de un $x$ nominal, sin tener en cuenta la expansión alrededor de valores nominales de $u$ y $w$. Esto se hace para poder presentar las ideas principales del EKF de segundo orden sin complicarnos demasiado con la notación. El desarrollo de esta sección puede extenderse fácilmente a expansiones de segundo orden alrededor de $u$ y $w$, una vez que se comprenda la idea principal.

La expansión de primer orden de $f(x,u,w,t)$ alrededor de $x=\hat{x}$ está dada por

$$
f(x,u,w,t) = f(\hat{x},u_0,w_0,t)+\frac{\partial f}{\partial x}\bigg|_{\hat{x}}(x-\hat{x})
\tag{13.67}
$$

En el EKF estándar, evaluamos esta expresión en $x=\hat{x}$ para obtener nuestra ecuación de actualización temporal para $\hat{x}$ como

$$
\dot{\hat{x}} = f(\hat{x},u_0,w_0,t)
\tag{13.68}
$$

En el EKF de segundo orden, expandimos $f(x,u,w,t)$ con un término adicional en la serie de Taylor:

$$
f(x,u,w,t) = f(\hat{x},u_0,w_0,t)+\frac{\partial f}{\partial x}\bigg|_{\hat{x}}(x-\hat{x})+\frac{1}{2}\sum_{i=1}^n\phi_i(x-\hat{x})^T\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}(x-\hat{x})
\tag{13.69}
$$

donde $n$ es la dimensión del vector de estado, $f_i$ es el $i$-ésimo elemento de $f(x,u,w,t)$, y el vector $\phi_i$ se define como un vector de $n\times1$ con todos ceros, excepto por un uno en el $i$-ésimo elemento. El término cuadrático de la sumatoria puede escribirse como

$$
(x-\hat{x})^T\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}(x-\hat{x}) = \mathrm{Tr}\left[\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}(x-\hat{x})(x-\hat{x})^T\right]
\tag{13.70}
$$

Dado que no conocemos el valor de $(x-\hat{x})(x-\hat{x})^T$ en la ecuación anterior, lo reemplazamos por su valor esperado, que es la covarianza del filtro de Kalman, para obtener

$$
(x-\hat{x})^T\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}(x-\hat{x}) \approx \mathrm{Tr}\left[\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}P\right]
\tag{13.71}
$$

Luego evaluamos la ecuación (13.69) en $x=\hat{x}$, y sustituimos la expresión anterior en la sumatoria, para obtener la ecuación de actualización temporal para $\hat{x}$ como

$$
\dot{\hat{x}} = f(\hat{x},u_0,w_0,t)+\frac{1}{2}\sum_{i=1}^n\phi_i\mathrm{Tr}\left[\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}P\right]
\tag{13.72}
$$

La ecuación de actualización temporal para $P$ sigue siendo la misma que en el EKF híbrido estándar, como se muestra en la ecuación (13.28):

$$
\dot{P} = FP+PF^T+LQL^T
\tag{13.73}
$$

Ahora deduciremos las ecuaciones de actualización de la medición. Supongamos que la ecuación de actualización de la medición para la estimación de estado está dada por

$$
\hat{x}_k^+ = \hat{x}_k^-+K_k\left[y_k-h(\hat{x}_k^-,t_k)\right]-\pi_k
\tag{13.74}
$$

donde $K_k$ es la ganancia de Kalman que debe determinarse, y $\pi_k$ es un término de corrección que debe determinarse. Elegiremos $\pi_k$ de modo que la estimación $\hat{x}_k^+$ sea insesgada, y luego elegiremos $K_k$ para minimizar la traza de la covarianza de la estimación.

Si definimos los errores de estimación como

$$
\begin{aligned}
e_k^- &= x_k-\hat{x}_k^- \\
e_k^+ &= x_k-\hat{x}_k^+
\end{aligned}
\tag{13.75}
$$

podemos ver, a partir de las ecuaciones (13.66) y (13.74), que

$$
e_k^+ = e_k^--K_k\left[h(x_k,t_k)-h(\hat{x}_k^-,t_k)\right]-K_kv_k+\pi_k
\tag{13.76}
$$

Ahora realizamos una expansión en serie de Taylor de segundo orden de $h(x_k,t_k)$ alrededor del punto nominal $\hat{x}_k^-$, para obtener

$$
\begin{aligned}
h(x_k,t_k) &= h(\hat{x}_k^-,t_k)+\frac{\partial h}{\partial x}\bigg|_{\hat{x}_k^-}(x_k-\hat{x}_k^-)+ \\
&\quad \frac{1}{2}\sum_{i=1}^m\phi_i(x_k-\hat{x}_k^-)^T\frac{\partial^2h(i)}{\partial x^2}\bigg|_{\hat{x}_k^-}(x_k-\hat{x}_k^-) \\[6pt]
&= h(\hat{x}_k^-,t_k)+H_k(x_k-\hat{x}_k^-)+\frac{1}{2}\sum_{i=1}^m\phi_i(x_k-\hat{x}_k^-)^T\frac{\partial^2h_i}{\partial x^2}\bigg|_{\hat{x}_k^-}(x_k-\hat{x}_k^-)
\end{aligned}
\tag{13.77}
$$

donde $H_k$ se define mediante la ecuación anterior, $m$ es la dimensión del vector de medición, y $h_i$ es el $i$-ésimo elemento de $h(x_k,t_k)$. Esto da el error de estimación *a posteriori* como

$$
e_k^+ = e_k^--K_kH_ke_k^--\frac{1}{2}K_k\sum_{i=1}^m\phi_i(e_k^-)^TD_{k,i}e_k^--K_kv_k+\pi_k
\tag{13.78}
$$

donde $D_{k,i}$ se define como

$$
D_{k,i} = \frac{\partial^2h_i}{\partial x^2}\bigg|_{\hat{x}_k^-}
\tag{13.79}
$$

Tomando el valor esperado de ambos lados de la ecuación (13.78), suponiendo que $E(e_k^-)=0$, y haciendo la misma aproximación que en la ecuación (13.71), podemos ver que, para que $E(e_k^+)=0$, debemos fijar

$$
\pi_k = \frac{1}{2}K_k\sum_{i=1}^m\phi_i\mathrm{Tr}\left[D_{k,i}P_k^-\right]
\tag{13.80}
$$

Definiendo $P_k^+$ como

$$
P_k^+ = E\left[e_k^+(e_k^+)^T\right]
\tag{13.81}
$$

y usando las ecuaciones anteriores, puede demostrarse, después de algunos cálculos algebraicos laboriosos [Ath68], que

$$
P_k^+ = (I-K_kH_k)P_k^-(I-K_kH_k)^T+K_k(R_k+\Lambda_k)K_k^T
\tag{13.82}
$$

donde la matriz $\Lambda_k$ se define como

$$
\Lambda_k = \frac{1}{4}E\left\{\left[\sum_{i=1}^m\phi_i\mathrm{Tr}\left[D_{k,i}(e_k^-(e_k^-)^T-P_k^-)\right]\right][\cdots]^T\right\}
\tag{13.83}
$$

Ahora definimos una función de costo $J_k$ que queremos minimizar, como una suma ponderada de los errores de estimación:

$$
\begin{aligned}
J_k &= E\left[(e_k^+)^TS_ke_k^+\right] \\
&= \mathrm{Tr}\left[S_kP_k^+\right]
\end{aligned}
\tag{13.84}
$$

donde $S_k$ es cualquier matriz de ponderación definida positiva. La $K_k$ que minimiza esta función de costo puede hallarse como

$$
K_k = P_k^-H_k^T(H_kP_k^-H_k^T+R_k+\Lambda_k)^{-1}
\tag{13.85}
$$

Esto da la matriz $P_k^+$ de la ecuación (13.82) como

$$
P_k^+ = P_k^--P_k^-H_k^T(H_kP_k^-H_k^T+R_k+\Lambda_k)^{-1}H_kP_k^-
\tag{13.86}
$$

Ahora necesitamos determinar cómo evaluar la matriz $\Lambda_k$ de la ecuación (13.83). Notemos que $\Lambda_k$ puede escribirse como la doble sumatoria

$$
\Lambda_k = \frac{1}{4}E\left\{\sum_{i,j=1}^m\phi_i\phi_j^T\mathrm{Tr}\left[D_{k,i}(e_k^-(e_k^-)^T-P_k^-)\right]\mathrm{Tr}\left[D_{k,j}(e_k^-(e_k^-)^T-P_k^-)\right]\right\}
\tag{13.87}
$$

El producto $\phi_i\phi_j^T$ es una matriz de $m\times m$, cuyos elementos son todos cero, excepto el elemento en la fila $i$ y la columna $j$. Por lo tanto, el elemento en la fila $i$ y la columna $j$ de $\Lambda_k$ puede escribirse como

$$
\Lambda_k(i,j) = \frac{1}{4}E\left\{\mathrm{Tr}\left[D_{k,i}(e_k^-(e_k^-)^T-P_k^-)\right]\mathrm{Tr}\left[D_{k,j}(e_k^-(e_k^-)^T-P_k^-)\right]\right\}
\tag{13.88}
$$

Esta expresión puede evaluarse con el siguiente lema [Ath68].

> **Lema 6** Supongamos que tenemos el vector aleatorio $x$, de $n$ elementos, con $x\sim N(0,P)$. Entonces
>
> $$
> \begin{aligned}
> E\left[x\,\mathrm{Tr}(Axx^T)\right] &= 0 \\
> E\left[\mathrm{Tr}(Axx^TBxx^T)\right] &= E\left[\mathrm{Tr}(Axx^T)\mathrm{Tr}(Bxx^T)\right] \\
> &= 2\,\mathrm{Tr}(APBP)+\mathrm{Tr}(AP)\mathrm{Tr}(BP)
> \end{aligned}
> $$
> *(13.89)*
>
> donde $A$ y $B$ son matrices arbitrarias de $n\times n$.

Usando este lema junto con la ecuación (13.88), podemos ver que

$$
\Lambda_k(i,j) = \frac{1}{2}\mathrm{Tr}(D_{k,i}P_k^-D_{k,j}P_k^-)
\tag{13.90}
$$

Esta ecuación, junto con las ecuaciones (13.74), (13.80), (13.82) y (13.85), especifica las ecuaciones de actualización de la medición para el EKF de segundo orden. El EKF de segundo orden puede resumirse de la siguiente manera.

**El filtro de Kalman extendido híbrido de segundo orden**

1. Las ecuaciones del sistema están dadas por:

$$
\begin{aligned}
\dot{x} &= f(x,u,w,t) \\
y_k &= h(x_k,t_k)+v_k \\
w(t) &\sim (0,Q) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.91}
$$

2. El estimador se inicializa de la siguiente manera:

$$
\begin{aligned}
\hat{x}_0^+ &= E(x_0) \\
P_0^+ &= E\left[(x_0-\hat{x}_0^+)(x_0-\hat{x}_0^+)^T\right]
\end{aligned}
\tag{13.92}
$$

3. Las ecuaciones de actualización temporal están dadas por

$$
\begin{aligned}
\dot{\hat{x}} &= f(\hat{x},u,0,t)+\frac{1}{2}\sum_{i=1}^n\phi_i\mathrm{Tr}\left[\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}}P\right] \\
\dot{P} &= FP+PF^T+LQL^T \\[6pt]
\phi_i &= \begin{bmatrix} 0 \\ \vdots \\ 0 \\ 1 \\ 0 \\ \vdots \\ 0 \end{bmatrix} \leftarrow i\text{-ésimo elemento} \\[6pt]
F &= \frac{\partial f}{\partial x}\bigg|_{\hat{x}} \\
L &= \frac{\partial f}{\partial w}\bigg|_{\hat{x}}
\end{aligned}
\tag{13.93}
$$

4. Las ecuaciones de actualización de la medición están dadas por

$$
\begin{aligned}
\hat{x}_k^+ &= \hat{x}_k^-+K_k\left[y_k-h(\hat{x}_k^-)\right]-\pi_k \\
\pi_k &= \frac{1}{2}K_k\sum_{i=1}^m\phi_i\mathrm{Tr}\left[D_{k,i}P_k^-\right] \\
D_{k,i} &= \frac{\partial^2h_i(x_k,t_k)}{\partial x^2}\bigg|_{\hat{x}_k^-} \\
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k+\Lambda_k)^{-1} \\
H_k &= \frac{\partial h(x_k,t_k)}{\partial x}\bigg|_{\hat{x}_k^-} \\
\Lambda_k(i,j) &= \frac{1}{2}\mathrm{Tr}(D_{k,i}P_k^-D_{k,j}P_k^-) \\
P_k^+ &= P_k^--P_k^-H_k^T(H_kP_k^-H_k^T+R_k+\Lambda_k)^{-1}H_kP_k^-
\end{aligned}
\tag{13.94}
$$

Notemos que, si en este algoritmo fijamos las matrices de segundas derivadas parciales como matrices nulas, se obtiene el EKF híbrido estándar.

\vspace{\baselineskip}
**EJEMPLO 13.3**

En este ejemplo comparamos el desempeño del EKF, del EKF de segundo orden y del EKF iterado, para el problema del cuerpo en caída descrito en el ejemplo 13.2. En [Wis69] se presentó una comparación similar, donde se concluyó que el EKF iterado tenía mejor desempeño en cuanto al error RMS, pero que el filtro de segundo orden tenía menor sesgo. Las ecuaciones del sistema son las mismas que se muestran en el ejemplo 13.2:

$$
\begin{aligned}
\dot{x}_1 &= x_2+w_1 \\
\dot{x}_2 &= \rho_0\exp(-x_1/k)x_2^2x_3/2-g+w_2 \\
\dot{x}_3 &= w_3
\end{aligned}
\tag{13.95}
$$

En este ejemplo cambiamos el sistema de medición, de modo que no mide la altitud del cuerpo en caída, sino que mide la distancia al dispositivo de medición. El dispositivo de medición está ubicado a una altitud $a$, y a una distancia horizontal $M$ de la línea vertical de caída del cuerpo. Por lo tanto, la ecuación de medición está dada por

$$
\begin{aligned}
y_k &= \sqrt{M^2+(x_1(t_k)-a)^2}+v_k \\
&= h(x_k)+v_k
\end{aligned}
\tag{13.96}
$$

Esto hace que el problema sea más no lineal y, por lo tanto, más difícil de estimar (es decir, en el ejemplo 13.2 teníamos un sistema no lineal, pero una medición lineal, mientras que en este ejemplo tenemos no linealidades tanto en el sistema como en las ecuaciones de medición). La matriz de derivadas parciales $F$ para los EKF se da en el ejemplo 13.2. Las demás matrices de derivadas parciales que se usan en el EKF de segundo orden están dadas por:

$$
\begin{aligned}
H &= \frac{\partial h}{\partial x} \\
&= \begin{bmatrix} (x_1-a)(M^2+(x_1-a)^2)^{-1/2} & 0 & 0 \end{bmatrix} \\[6pt]
L &= \frac{\partial f}{\partial w} \\
&= \begin{bmatrix} 1&0&0 \\ 0&1&0 \\ 0&0&1 \end{bmatrix} \\[6pt]
D_1 &= \frac{\partial^2h_1}{\partial x^2} \\
&= \begin{bmatrix} h^{-1}(1-(x_1-a)^2h^{-2}) & 0 & 0 \\ 0&0&0 \\ 0&0&0 \end{bmatrix} \\[6pt]
\frac{\partial^2f_1}{\partial x^2} &= \frac{\partial^2f_3}{\partial x^2} = \begin{bmatrix} 0&0&0 \\ 0&0&0 \\ 0&0&0 \end{bmatrix} \\[6pt]
\frac{\partial^2f_2}{\partial x^2} &= \rho_0\exp(-x_1/k)\begin{bmatrix} x_2^2x_3/2k^2 & -x_2x_3/k & -x_2^2/2k \\ -x_2x_3/k & x_3 & x_2 \\ -x_2^2/2k & x_2 & 0 \end{bmatrix}
\end{aligned}
\tag{13.97}
$$

La tabla 13.2 muestra los desempeños de los EKF (promediados sobre 20 corridas de simulación). Se observa que el EKF de segundo orden ofrece una mejora significativa respecto del EKF de primer orden para la estimación de la altitud y de la velocidad, pero, por alguna razón, en realidad ofrece un desempeño peor para la estimación del coeficiente balístico. Notemos también que el EKF iterado ofrece solo una ligera mejora respecto del EKF de primer orden, y que (como era de esperar) el EKF iterado se desempeña mejor cuando se ejecutan más iteraciones para el refinamiento de la linealización.

**Tabla 13.2** Resultados del ejemplo 13.3. Comparación de los errores de estimación de distintos enfoques de EKF para el seguimiento de un cuerpo en caída.

| Filtro | Altitud | Velocidad | Coeficiente balístico |
|---|---|---|---|
| EKF de primer orden | 758 pies | 518 pies/s | 0.091 pies³/lb/s² |
| EKF de segundo orden | 356 | 483 | 0.129 |
| EKF iterado ($N=2$) | 755 | 517 | 0.091 |
| EKF iterado ($N=3$) | 745 | 516 | 0.091 |
| EKF iterado ($N=4$) | 738 | 509 | 0.091 |
| EKF iterado ($N=5$) | 733 | 506 | 0.091 |
| EKF iterado ($N=6$) | 723 | 506 | 0.091 |

De esto concluimos que el filtro de segundo orden tiene un mejor desempeño de estimación. Sin embargo, su implementación es mucho más difícil, y requiere calcular derivadas de segundo orden. En este ejemplo, las derivadas de segundo orden pudieron obtenerse analíticamente, porque contamos con ecuaciones explícitas y analíticas del sistema y de la medición. En muchas aplicaciones, las derivadas de segundo orden no estarán disponibles analíticamente, y las aproximaciones inevitablemente estarán sujetas a error.

Estos resultados difieren de los reportados en [Wis69], donde se mostró que el EKF iterado se desempeñaba mejor que el EKF de segundo orden. Las conclusiones distintas entre este libro y [Wis69] muestran que las comparaciones entre distintos algoritmos suelen ser subjetivas. Es posible que las discrepancias se deban a diferencias en las implementaciones de los algoritmos de filtrado, a diferencias en las implementaciones de la dinámica del sistema o de la generación de ruido aleatorio, a diferencias en la manera en que se midieron los errores de estimación, o incluso a diferencias en las plataformas de cómputo que se usaron.



El filtro de segundo orden fue desarrollado inicialmente por Bass [Bas66] y Jazwinski [Jaz66]. Athans [Ath68] y Jazwinski [Jaz70] desarrollaron un filtro gaussiano de segundo orden, en el que se conservan términos de cuarto orden en las aproximaciones por serie de Taylor, y se aproximan suponiendo que las probabilidades subyacentes son gaussianas. Rolf Henriksen reportó una pequeña corrección a las deducciones originales del filtro de segundo orden [Hen82]. Aunque el filtro de segundo orden suele ofrecer un desempeño mejor que el del filtro de Kalman extendido, no puede afirmarse nada definitivo sobre su desempeño, como lo evidencia un ejemplo de un filtro de segundo orden inestable, reportado en [Kus67]. En [Sch68, Wis69, Wis70, Net78] puede encontrarse un análisis y una comparación adicionales de algunos filtros de Kalman no lineales. Una versión simplificada del filtro de segundo orden de tiempo discreto de Henriksen puede resumirse de la siguiente manera.

**El filtro de Kalman extendido de segundo orden, de tiempo discreto**

1. Las ecuaciones del sistema están dadas por:

$$
\begin{aligned}
x_{k+1} &= f(x_k,u_k,k)+w_k \\
y_k &= h(x_k,k)+v_k \\
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.98}
$$

2. El estimador se inicializa de la siguiente manera:

$$
\begin{aligned}
\hat{x}_0^+ &= E(x_0) \\
P_0^+ &= E\left[(x_0-\hat{x}_0^+)(x_0-\hat{x}_0^+)^T\right]
\end{aligned}
\tag{13.99}
$$

3. Las ecuaciones de actualización temporal están dadas por:

$$
\begin{aligned}
\hat{x}_{k+1}^- &= f(\hat{x}_k^+,u_k,k)+\frac{1}{2}\sum_{i=1}^n\phi_i\mathrm{Tr}\left[\frac{\partial^2f_i}{\partial x^2}\bigg|_{\hat{x}_k^+}P_k^+\right] \\
P_{k+1}^- &= FP_k^+F^T+Q_k \\[6pt]
\phi_i &= \begin{bmatrix} 0 \\ \vdots \\ 0 \\ 1 \\ 0 \\ \vdots \\ 0 \end{bmatrix} \leftarrow i\text{-ésimo elemento} \\[6pt]
F &= \frac{\partial f}{\partial x}\bigg|_{\hat{x}_k^+}
\end{aligned}
\tag{13.100}
$$

4. Las ecuaciones de actualización de la medición están dadas por:

$$
\begin{aligned}
\hat{x}_k^+ &= \hat{x}_k^-+K_k\left[y_k-h(\hat{x}_k^-,k)\right]-\pi_k \\
\pi_k &= \frac{1}{2}K_k\sum_{i=1}^m\phi_i\mathrm{Tr}\left[D_{k,i}P_k^-\right] \\
D_{k,i} &= \frac{\partial^2h_i(x_k,k)}{\partial x^2}\bigg|_{\hat{x}_k^-} \\
K_k &= P_k^-H_k^T(H_kP_k^-H_k^T+R_k)^{-1} \\
H_k &= \frac{\partial h(x_k,k)}{\partial x}\bigg|_{\hat{x}_k^-} \\
P_k^+ &= (I-K_kH_k)P_k^-
\end{aligned}
\tag{13.101}
$$

En [Hen82] puede encontrarse una versión más general del algoritmo anterior. De manera similar al EKF híbrido de segundo orden presentado antes en esta sección, notemos que, si en este algoritmo fijamos las matrices de derivadas parciales de segundo orden como matrices nulas, se obtiene el EKF estándar de tiempo discreto.

### 13.3.3 Otros enfoques

Hemos considerado un par de enfoques de orden superior para reducir el error de linealización del EKF. Analizamos el EKF iterado y el EKF de segundo orden, pero también existen otros enfoques. Por ejemplo, los filtros de suma de gaussianas se basan en la idea de que una función de densidad de probabilidad (fdp) no gaussiana puede aproximarse mediante una suma de fdp gaussianas. Esto es similar a la idea de que cualquier curva puede aproximarse mediante una función constante por tramos. Dado que la fdp verdadera del ruido de proceso y del ruido de medición puede aproximarse mediante una suma de $M$ fdp gaussianas, podemos ejecutar $M$ filtros de Kalman en paralelo sobre $M$ problemas de filtrado gaussiano, cada uno de ellos un filtro óptimo, y luego combinarlos para obtener una estimación aproximadamente óptima. La cantidad de filtros $M$ implica una relación de compromiso entre la exactitud de la aproximación (y, por lo tanto, la optimalidad) y el esfuerzo computacional. Esta idea se mencionó por primera vez en [Aok65], y se exploró en [Cam68, Sor71b, Als74, Kit89]. El algoritmo del filtro de suma de gaussianas presentado en [Als72] puede resumirse de la siguiente manera.

**El filtro de suma de gaussianas**

1. Las ecuaciones del sistema y de medición, de tiempo discreto y $n$ estados, están dadas por:

$$
\begin{aligned}
x_k &= f_{k-1}(x_{k-1},u_{k-1},w_{k-1}) \\
y_k &= h_k(x_k,v_k) \\
w_k &\sim (0,Q_k) \\
v_k &\sim (0,R_k)
\end{aligned}
\tag{13.102}
$$

2. Inicializar el filtro aproximando la fdp del estado inicial de la siguiente manera:

$$
\mathrm{fdp}(\hat{x}_0^+) = \sum_{i=1}^M a_{0i}N(\hat{x}_{0i}^+,P_{0i}^+)
\tag{13.103}
$$

Los coeficientes $a_{0i}$ (que son positivos y suman 1), las medias $\hat{x}_{0i}^+$, y las covarianzas $P_{0i}^+$, son elegidos por el usuario para dar una buena aproximación a la fdp del estado inicial.

3. Para $k=1,2,\cdots$, realizar lo siguiente.

(a) La estimación de estado *a priori* se obtiene ejecutando primero las siguientes ecuaciones de actualización temporal, para $i=1,\cdots,M$:

$$
\begin{aligned}
\hat{x}_{ki}^- &= f_{k-1}(\hat{x}_{k-1,i}^+,u_{k-1},0) \\
F_{k-1,i} &= \frac{\partial f_{k-1}}{\partial x_{k-1}}\bigg|_{\hat{x}_{k-1,i}^+} \\
P_{ki}^- &= F_{k-1,i}P_{k-1,i}^+F_{k-1,i}^T+Q_{k-1} \\
a_{ki} &= a_{k-1,i}
\end{aligned}
\tag{13.104}
$$

La fdp de la estimación de estado *a priori* se obtiene mediante la siguiente suma:

$$
\mathrm{fdp}(\hat{x}_k^-) = \sum_{i=1}^M a_{ki}N(\hat{x}_{ki}^-,P_{ki}^-)
\tag{13.105}
$$

(b) La estimación de estado *a posteriori* se obtiene ejecutando primero las siguientes ecuaciones de actualización de la medición, para $i=1,\cdots,M$:

$$
\begin{aligned}
H_{ki} &= \frac{\partial h_k}{\partial x_k}\bigg|_{\hat{x}_{ki}^-} \\
K_{ki} &= P_{ki}^-H_{ki}^T(H_{ki}P_{ki}^-H_{ki}^T+R_k)^{-1} \\
P_{ki}^+ &= P_{ki}^--K_{ki}H_{ki}P_{ki}^- \\
\hat{x}_{ki}^+ &= \hat{x}_{ki}^-+K_{ki}\left[y_k-h_k(\hat{x}_{ki}^-,0)\right]
\end{aligned}
\tag{13.106}
$$

Los coeficientes de ponderación $a_{ki}$ para las estimaciones individuales se obtienen de la siguiente manera:

$$
\begin{aligned}
r_{ki} &= y_k-h_k(\hat{x}_{ki}^-,0) \\
S_{ki} &= H_{ki}P_{ki}^-H_{ki}^T+R_k \\
\beta_{ki} &= \frac{\exp\left[-r_{ki}^TS_{ki}^{-1}r_{ki}/2\right]}{(2\pi)^{n/2}|S_{ki}|^{1/2}} \\
a_{ki} &= \frac{a_{k-1,i}\beta_{ki}}{\sum_{j=1}^Ma_{k-1,j}\beta_{kj}}
\end{aligned}
\tag{13.107}
$$

Notemos que el coeficiente de ponderación $a_{ki}$ se calcula usando la medición $y_k$, para obtener la confianza relativa $\beta_{ki}$ de la estimación $\hat{x}_{ki}^-$. La fdp de la estimación de estado *a posteriori* se obtiene mediante la siguiente suma:

$$
\mathrm{fdp}(\hat{x}_k^+) = \sum_{i=1}^M a_{ki}N(\hat{x}_{ki}^+,P_{ki}^+)
\tag{13.108}
$$

Este enfoque también puede extenderse al suavizado [Kit94]. Pueden seguirse enfoques similares para expandir la fdp usando funciones no gaussianas [Aok67, Sor68, Sri70, deF71, Hec71, Hec73, Mcr75, Wil81, Kit87, Kra88]. Se ha deducido un filtro relacionado para el caso en que, o bien el ruido de proceso, o bien el ruido de medición, es estrictamente gaussiano, pero el otro ruido es gaussiano con colas pesadas [Mas75, Tsa83]. Esto está motivado por la observación de que muchos casos de ruido que se dan en la naturaleza tienen fdp que son aproximadamente gaussianas, pero con colas más pesadas [Mas77].

Otro enfoque para el filtrado no lineal se llama **filtrado basado en rejilla**. En el filtrado basado en rejilla, el valor de la fdp del estado se aproxima, se almacena, se propaga y se actualiza en puntos discretos del espacio de estados [Buc69, Buc71]; [Spa88, capítulo 6]. Esto es similar al filtrado de partículas (que se analiza en el capítulo 15), salvo que en el filtrado de partículas elegimos que las partículas se distribuyan en el espacio de estados de acuerdo con la fdp del estado. El filtrado basado en rejilla no distribuye las partículas de esta manera, y por lo tanto tiene requisitos computacionales que crecen exponencialmente con la dimensión del estado. El filtrado basado en rejilla es incluso más costoso computacionalmente que el filtrado de partículas, y esto ha limitado su aplicación. Además, el filtrado de partículas es un tipo de filtrado basado en rejilla "inteligente". Esto parece augurar muy poco trabajo adicional en el filtrado basado en rejilla.

Richard Bucy propuso todavía otro enfoque para el filtrado no lineal [Buc65]. En lugar de linealizar la dinámica del sistema, calcular el filtro no lineal teóricamente óptimo, y luego linealizar ese filtro no lineal. Sin embargo, el filtro no lineal teóricamente óptimo resulta muy difícil de calcular, salvo en casos especiales.

---

## 13.4 ESTIMACIÓN DE PARÁMETROS

La teoría de estimación de estado puede usarse no solo para estimar los estados de un sistema, sino también para estimar los parámetros desconocidos de un sistema. Es posible que esto se haya sugerido por primera vez en [Kop63]. Supongamos que tenemos un modelo de sistema de tiempo discreto, pero en el que las matrices del sistema dependen de manera no lineal de un vector de parámetros desconocido $p$:

$$
\begin{aligned}
x_{k+1} &= F_k(p)x_k+G_k(p)u_k+L_k(p)w_k \\
y_k &= H_kx_k+v_k \\
p &= \text{vector de parámetros desconocido}
\end{aligned}
\tag{13.109}
$$

En este modelo, suponemos que la medición es independiente de $p$, pero esto es solo por conveniencia de notación. El análisis aquí puede extenderse fácilmente para incluir una dependencia de $y_k$ respecto de $p$. Supongamos que $p$ es un vector de parámetros constante. En realidad, no nos interesa estimar el estado, sino que nos interesa estimar $p$. Este es el caso, por ejemplo, del problema de estimación del estado de salud de motores de aeronaves [Kob03, Sim05a]. En esos artículos se supuso que se quería estimar el estado de salud de un motor de aeronave (con fines de programación del mantenimiento), pero que en realidad no interesaba estimar los estados del motor.

Para estimar el parámetro $p$, primero aumentamos el estado con el parámetro, para obtener un vector de estado aumentado $x'$:

$$
x_k' = \begin{bmatrix} x_k \\ p_k \end{bmatrix}
\tag{13.110}
$$

Si $p_k$ es constante, entonces modelamos $p_{k+1}=p_k+w_{pk}$, donde $w_{pk}$ es un pequeño término de ruido artificial que le permite al filtro de Kalman modificar su estimación de $p_k$. Nuestro modelo de sistema aumentado puede escribirse como

$$
\begin{aligned}
x_{k+1}' &= \begin{bmatrix} F_k(p_k)x_k+G_k(p_k)u_k+L_k(p_k)w_k \\ p_k+w_{pk} \end{bmatrix} \\
&= f(x_k',u_k,w_k,w_{pk}) \\[6pt]
y_k &= \begin{bmatrix} H_k & 0 \end{bmatrix}\begin{bmatrix} x_k \\ p_k \end{bmatrix}+v_k
\end{aligned}
\tag{13.111}
$$

Notemos que $f(x_k',u_k,w_k,w_{pk})$ es una función no lineal del estado aumentado $x_k'$. Por lo tanto, podemos usar un filtro de Kalman extendido (o cualquier otro filtro no lineal) para estimar $x_k'$.

\vspace{\baselineskip}
**EJEMPLO 13.4**

Este ejemplo está tomado de [Ste94]. Supongamos que tenemos un sistema de segundo orden, gobernado por las siguientes ecuaciones:

$$
\ddot{x}_1+2\zeta\omega_n\dot{x}_1+\omega_n^2x_1 = \omega_n^2w
\tag{13.112}
$$

donde $\omega_n$ es la frecuencia natural del sistema, $\zeta$ es la razón de amortiguamiento, y la entrada $w$ es ruido de media cero. Un modelo en espacio de estados para este sistema puede escribirse como

$$
\begin{aligned}
\dot{x}_1 &= x_2 \\
\dot{x}_2 &= -\omega_n^2x_1-2\zeta\omega_nx_2+\omega_n^2w \\[6pt]
\begin{bmatrix} \dot{x}_1 \\ \dot{x}_2 \end{bmatrix} &= \begin{bmatrix} 0&1 \\ -\omega_n^2&-2\zeta\omega_n \end{bmatrix}\begin{bmatrix} x_1 \\ x_2 \end{bmatrix}+\begin{bmatrix} 0 \\ \omega_n^2 \end{bmatrix}w
\end{aligned}
\tag{13.113}
$$

Supongamos que $-2\zeta\omega_n$ se conoce, pero que $\zeta$ y $\omega_n$ son desconocidos. Queremos estimar $-\omega_n^2$. Supongamos que tanto $x_1$ como $x_2$ están disponibles para la medición. Definimos el parámetro conocido como $b$; es decir, $b=-2\zeta\omega_n$. Definimos un nuevo elemento de estado, igual al parámetro que queremos estimar. Es decir, $x_3=-\omega_n^2$. Entonces formamos un modelo de sistema aumentado de la siguiente manera:

$$
\begin{aligned}
\dot{x}' &= \begin{bmatrix} x_2 \\ x_3x_1+bx_2-x_3w \\ w_p \end{bmatrix} \\
&= f(x',w') \\[6pt]
w' &= \begin{bmatrix} w \\ w_p \end{bmatrix} \\[6pt]
y &= \begin{bmatrix} 1&0&0 \\ 0&1&0 \end{bmatrix}x'+v
\end{aligned}
\tag{13.114}
$$

donde $w_p$ es un término de ruido artificial que agregamos al sistema, que le permite al filtro de Kalman modificar su estimación de $x_3$. Podemos usar un filtro de Kalman extendido para estimar el estado aumentado. Primero necesitamos hallar las matrices de derivadas parciales:

$$
\begin{aligned}
F &= \frac{\partial f}{\partial x'}\bigg|_{\hat{x}',w_0'} \\
&= \begin{bmatrix} 0&1&0 \\ x_3&b&x_1-w \\ 0&0&0 \end{bmatrix}\bigg|_{\hat{x}',w_0'} \\
&= \begin{bmatrix} 0&1&0 \\ \hat{x}_3&b&\hat{x}_1 \\ 0&0&0 \end{bmatrix} \\[6pt]
L &= \frac{\partial f}{\partial w'}\bigg|_{\hat{x}',w_0'} \\
&= \begin{bmatrix} 0&0 \\ -\hat{x}_3&0 \\ 0&1 \end{bmatrix}
\end{aligned}
\tag{13.115}
$$

El filtro de Kalman extendido de tiempo continuo puede escribirse como

$$
\begin{aligned}
\dot{\hat{x}}' &= f(\hat{x}',0)+K(y-H\hat{x}') \\
K &= PH^TR^{-1} \\
\dot{P} &= FP+PF^T+LQL^T-PH^TR^{-1}HP
\end{aligned}
\tag{13.116}
$$

La figura 13.4 ilustra los resultados de una simulación típica del filtro de Kalman extendido que se usa para estimar $-\omega_n^2$ en este sistema. Los parámetros verdaderos del sistema son $\omega_n=2$ y $\zeta=0.1$, de modo que $-\omega_n^2=-4$. Supongamos que comenzamos estimando $-\omega_n^2$ como $-8$, con una varianza de estimación inicial de 20. La figura 13.4 muestra que el error de nuestra estimación de $-\omega_n^2$ disminuye gradualmente hacia cero, y que la varianza de estimación disminuye gradualmente. En este ejemplo fijamos la varianza del ruido artificial $w_p$ en 0.1. Esto le permite al filtro de Kalman ajustar más fácilmente su estimación de $-\omega_n^2$, pero también puede impedir que el filtro converja al valor verdadero (véase el problema 13.23).



> *[Figura 13.4 — dos paneles frente a los "segundos" (0 a 100): panel superior, "error de estimación" (0 a 4), curva que decrece desde $\approx4$ hacia valores cercanos a 0; panel inferior, "varianza" (0 a 25), curva que decrece desde $\approx20$ hacia valores cercanos a 5.]*
>
> **Figura 13.4** Resultados del ejemplo 13.4. Desempeño típico de la estimación de parámetros, e incertidumbre del parámetro, para un filtro de Kalman extendido que estima $-\omega_n^2$ para un sistema de segundo orden. El error de estimación del parámetro desconocido, y su varianza, disminuyen gradualmente hacia cero.

---

## 13.5 RESUMEN

Pueden deducirse estimadores de estado óptimos para clases generales de sistemas no lineales, como se muestra en [Kus67], pero los filtros resultantes suelen tener dimensión infinita, lo cual los vuelve poco prácticos para su implementación. Pueden deducirse estimadores de estado no lineales, óptimos y de dimensión finita, para clases más restringidas de sistemas no lineales [Liu80], pero la restricción sobre las clases de sistemas aplicables es lo bastante significativa como para impedir una aplicabilidad amplia. Debido a estos factores, el filtrado de Kalman no lineal es el enfoque más difundido para la estimación de estado en sistemas no lineales.

Resulta interesante notar que las primeras aplicaciones del filtrado de Kalman fueron en problemas no lineales de estimación de órbitas [Bat62]. En [Cox64, Fri66] pueden encontrarse algunas investigaciones tempranas sobre el filtrado de Kalman no lineal. Mientras que para el filtro de Kalman lineal se dispone fácilmente de resultados de estabilidad y convergencia, para el filtrado de Kalman no lineal esos resultados son mucho más difíciles de obtener. En [Urs80] se encuentran algunos resultados de convergencia para el filtrado de Kalman no lineal. Si las no linealidades tienen cotas conocidas, entonces la ecuación de Riccati puede modificarse de una manera sencilla, para garantizar la estabilidad del EKF de tiempo continuo [Rei98]. Las condiciones necesarias para garantizar el acotamiento de la covarianza del error del EKF de tiempo discreto pueden relacionarse con la observabilidad del sistema no lineal subyacente [Dez92, Son95].
