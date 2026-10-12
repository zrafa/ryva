#!/usr/bin/env python3
"""
Calibracion hard-iron + soft-iron 2D (plano horizontal) para magnetometro.

Uso:
    python3 calibrar_mag.py archivo1.txt [archivo2.txt ...]

Formato de linea:  <algo> [x, y, z] <timestamp>
No necesita saber hacia donde ibas en cada momento. Solo necesita que los datos
cubran TODAS las direcciones (idealmente girar 360 grados en el plano horizontal,
con el sensor montado igual que en uso real).

Modelo:  p_cal = M @ (p_raw - offset)      ->  rumbo = atan2(y_cal, x_cal)
  * offset = centro de la elipse que forman los puntos (hard iron)
  * M      = matriz simetrica que convierte la elipse en circulo (soft iron)
Se ajusta una elipse en el plano XY (metodo de Halir-Flusser, estable).
Z no se calibra (queda offset_z dado por --oz o el promedio de z).
"""
import re, sys
import numpy as np

LINE = re.compile(r'\[([^\]]+)\]')

def cargar(paths):
    pts = []
    for p in paths:
        for l in open(p):
            m = LINE.search(l)
            if m:
                v = [float(t) for t in m.group(1).split(',')]
                if len(v) == 3:
                    pts.append(v)
    return np.array(pts)

def ajustar_elipse(x, y):
    """Halir & Flusser (1998): elipse a x^2 + b xy + c y^2 + d x + e y + f = 0"""
    mx, my = x.mean(), y.mean()          # centrar para estabilidad numerica
    sx = np.hypot(x - mx, y - my).mean()
    X, Y = (x - mx) / sx, (y - my) / sx
    D1 = np.column_stack([X*X, X*Y, Y*Y])
    D2 = np.column_stack([X, Y, np.ones_like(X)])
    S1, S2, S3 = D1.T @ D1, D1.T @ D2, D2.T @ D2
    C1 = np.array([[0, 0, 2], [0, -1, 0], [2, 0, 0]], float)
    Tm = -np.linalg.solve(S3, S2.T)
    Mm = np.linalg.solve(C1, S1 + S2 @ Tm)
    w, v = np.linalg.eig(Mm)
    v = np.real(v)
    cond = 4 * v[0] * v[2] - v[1] ** 2
    a1 = v[:, np.argmax(cond > 0)] if np.any(cond > 0) else v[:, 0]
    a2 = Tm @ a1
    a, b, c, d, e, f = np.concatenate([a1, a2])
    # volver a coordenadas originales
    # sustituir X=(x-mx)/sx, Y=(y-my)/sx
    A = a; B = b; C = c
    D = d*sx - 2*a*mx - b*my
    E = e*sx - 2*c*my - b*mx
    F = f*sx*sx - d*sx*mx - e*sx*my + a*mx*mx + b*mx*my + c*my*my
    return A, B, C, D, E, F

def calibrar(pts, oz=None):
    x, y, z = pts[:, 0], pts[:, 1], pts[:, 2]
    A, B, C, D, E, F = ajustar_elipse(x, y)
    Q = np.array([[A, B/2], [B/2, C]])
    centro = np.linalg.solve(2*Q, -np.array([D, E]))       # offset hard iron
    k = -(F + (D*centro[0] + E*centro[1]) / 2)             # (p-o)^T Q (p-o) = k
    Qn = Q / k                                              # forma (p-o)^T Qn (p-o) = 1
    w, V = np.linalg.eigh(Qn)
    if np.any(w <= 0):
        raise RuntimeError("Ajuste invalido (no es una elipse). Faltan datos o cobertura de direcciones.")
    # M simetrica: convierte la elipse en circulo de radio R (R = radio medio)
    sqrtQ = V @ np.diag(np.sqrt(w)) @ V.T
    p = pts[:, :2] - centro
    R = np.mean(np.hypot(*(p @ sqrtQ.T).T))   # = 1 en el ajuste ideal
    # escalar para conservar la magnitud del campo (radio medio original aprox)
    radio_medio = np.mean(np.hypot(*(pts[:, :2] - centro).T))
    M2 = sqrtQ * (radio_medio / R)
    M = np.eye(3); M[:2, :2] = M2; M[2, 2] = 1.0
    off = np.array([centro[0], centro[1], z.mean() if oz is None else oz])
    return off, M

def cobertura(pts, off, M):
    c = (pts - off) @ M.T
    ang = np.degrees(np.arctan2(c[:, 1], c[:, 0])) % 360
    h, _ = np.histogram(ang, bins=12, range=(0, 360))
    return h, ang

if __name__ == '__main__':
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if not args:
        print(__doc__); sys.exit(1)
    pts = cargar(args)
    print(f"{len(pts)} muestras")
    off, M = calibrar(pts)
    h, ang = cobertura(pts, off, M)
    print("\nMuestras por sector de 30 deg (0-360):", h.tolist())
    if (h == 0).any():
        print("  AVISO: hay sectores sin datos. El ajuste puede ser poco confiable.")
    cal = (pts - off) @ M.T
    r = np.hypot(cal[:, 0], cal[:, 1])
    print(f"Radio calibrado: media {r.mean():.0f}, desvio {r.std():.0f} ({100*r.std()/r.mean():.1f}%)")
    print("\n// ---- pegar en C++ ----")
    print(f"double x_offset = {off[0]:.8f};")
    print(f"double y_offset = {off[1]:.8f};")
    print(f"double z_offset = {off[2]:.8f};")
    print("cv::Mat transformation = (cv::Mat_<double>(3, 3) <<")
    for i in range(3):
        fila = ", ".join(f"{M[i,j]: .8f}" for j in range(3))
        print(f"  {fila}{',' if i < 2 else ');'}")
