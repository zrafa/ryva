import numpy as np
import math 



def data_regularize(data, type="spheric", divs = 10):
    limits = np.array([
        [min(data[:,0]), max(data[:,0])],
        [min(data[:,1]), max(data[:,1])],
        [min(data[:,2]), max(data[:,2])]])
        
    regularized = []

    if type=="cubic":
        
        X = np.linspace(*limits[0], num = divs)
        Y = np.linspace(*limits[1], num = divs)
        Z = np.linspace(*limits[2], num = divs)

        for i in range(divs-1):
            for j in range(divs-1):
                for k in range(divs-1):
                    points_in_sector = []
                    for point in data:
                        if (point[0] >= X[i] and point[0] < X[i+1] and
                            point[1] >= Y[j] and point[1] < Y[j+1] and
                            point[2] >= Z[k] and point[2] < Z[k+1]) :
                            
                            points_in_sector.append(point)
                    if len(points_in_sector) > 0:
                        regularized.append(np.mean(np.array(points_in_sector), axis=0))

    elif type=="spheric" :
        divs_u = divs 
        divs_v = divs * 2

        center = np.array([
            0.5 * (limits[0,0] + limits[0,1]),
            0.5 * (limits[1,0] + limits[1,1]),
            0.5 * (limits[2,0] + limits[2,1])])
        d_c = data - center
    
        #spherical coordinates around center
        r_s = np.sqrt(d_c[:,0]**2. + d_c[:,1]**2. + d_c[:,2]**2.)
        d_s = np.array([
            r_s,
            np.arccos(d_c[:,2] / r_s),
            np.arctan2(d_c[:,1], d_c[:,0])]).T

        u = np.linspace(0, np.pi, num = divs_u)
        v = np.linspace(-np.pi, np.pi, num = divs_v)

        for i in range(divs_u - 1):
            for j in range(divs_v - 1):
                points_in_sector = []
                for k , point in enumerate(d_s):
                    if (point[1] >= u[i] and point[1] < u[i+1] and 
                        point[2] >= v[j] and point[2] < v[j+1]) :

                        points_in_sector.append(data[k])

                if len(points_in_sector) > 0:
                    regularized.append(np.mean(np.array(points_in_sector), axis=0))

### Other strategy of finding mean values in sectors

##                    p_sec = np.array(points_in_sector)
##                    R = np.mean(p_sec[:,0])

##                    U = (u[i] + u[i+1])*0.5
##                    V = (v[j] + v[j+1])*0.5
##                    x = R*math.sin(U)*math.cos(V)
##                    y = R*math.sin(U)*math.sin(V)
##                    z = R*math.cos(U)
##                    regularized.append(center + np.array([x,y,z]))
    

    return np.array(regularized)



# https://github.com/minillinim/ellipsoid
def ellipsoid_plot(center, radii, rotation, ax, plotAxes=False, cageColor='b', cageAlpha=0.2):
    """Plot an ellipsoid"""
        
    u = np.linspace(0.0, 2.0 * np.pi, 100)
    v = np.linspace(0.0, np.pi, 100)
    
    # cartesian coordinates that correspond to the spherical angles:
    x = radii[0] * np.outer(np.cos(u), np.sin(v))
    y = radii[1] * np.outer(np.sin(u), np.sin(v))
    z = radii[2] * np.outer(np.ones_like(u), np.cos(v))
    # rotate accordingly
    for i in range(len(x)):
        for j in range(len(x)):
            [x[i,j],y[i,j],z[i,j]] = np.dot([x[i,j],y[i,j],z[i,j]], rotation) + center

    if plotAxes:
        # make some purdy axes
        axes = np.array([[radii[0],0.0,0.0],
                         [0.0,radii[1],0.0],
                         [0.0,0.0,radii[2]]])
        # rotate accordingly
        for i in range(len(axes)):
            axes[i] = np.dot(axes[i], rotation)


        # plot axes
        for p in axes:
            X3 = np.linspace(-p[0], p[0], 100) + center[0]
            Y3 = np.linspace(-p[1], p[1], 100) + center[1]
            Z3 = np.linspace(-p[2], p[2], 100) + center[2]
            ax.plot(X3, Y3, Z3, color=cageColor)

    # plot ellipsoid
    ax.plot_wireframe(x, y, z,  rstride=4, cstride=4, color=cageColor, alpha=cageAlpha)



def ellipsoid_fit(X):
    """
    Ajusta una elipse en 2D (plano XY) y asume un valor para Z.

    Parámetros:
        X (numpy array): Matriz de datos de entrada con forma (n, 3), donde n es el número de puntos.
        x_offset (float): Offset en el eje x.
        y_offset (float): Offset en el eje y.
        z_offset (float): Offset en el eje z.

    Retorna:
        center (numpy array): Centro de la elipsoide en coordenadas originales.
        radii (numpy array): Radios de la elipsoide.
        evecs (numpy array): Vectores propios (orientación de la elipsoide).
    """
    # Vector de offset
    # offset = np.array([x_offset, y_offset, z_offset])

    # Centrar los datos restando el offset
    # X_centered = X - offset
    X_centered = X

    # Extraer componentes centrados en XY
    x = X_centered[:, 0]
    y = X_centered[:, 1]

    # Construir la matriz D para el ajuste de la elipse en 2D
    D = np.array([x * x,
                  y * y,
                  2 * x * y,
                  2 * x,
                  2 * y])

    # Resolver el sistema lineal para encontrar los parámetros de la elipse
    DT = D.conj().T
    v = np.linalg.solve(D.dot(DT), D.dot(np.ones(np.size(x))))

    # Construir la matriz A en 2D
    A = np.array([[v[0], v[2], v[3]],
                  [v[2], v[1], v[4]],
                  [v[3], v[4], -1]])

    # Calcular el centro de la elipse en coordenadas centradas
    center_centered_xy = np.linalg.solve(-A[:2, :2], [[v[3]], [v[4]]])

    # Asumir un valor para Z (usamos el promedio de los valores disponibles en Z)
    z_mean = np.mean(X_centered[:, 2]) if np.any(X_centered[:, 2]) else 0.0
    center_centered = np.vstack([center_centered_xy, [z_mean]])

    # Convertir el centro a coordenadas originales sumando el offset
    # center = center_centered + offset.reshape(3, 1)
    center = center_centered

    # Calcular los radios de la elipse en XY
    evals_xy, evecs_xy = np.linalg.eig(A[:2, :2] / -A[2, 2])
    radii_xy = np.sqrt(1. / np.abs(evals_xy))

    # Asumir un radio para Z (usamos el promedio de los radios de X e Y)
    radius_z = np.mean(radii_xy)

    # Radios de la elipsoide
    radii = np.array([radii_xy[0], radii_xy[1], radius_z])

    # Vectores propios (orientación)
    evecs = np.eye(3)  # Asumimos que no hay rotación en Z
    evecs[:2, :2] = evecs_xy

    return center, radii, evecs

