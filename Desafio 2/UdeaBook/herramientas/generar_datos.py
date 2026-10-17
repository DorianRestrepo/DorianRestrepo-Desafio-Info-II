# -*- coding: utf-8 -*-
"""
Generador de datos de prueba para UdeABook (herramienta auxiliar, NO es parte del core en C++).

Crea los cinco archivos de texto que el programa lee al iniciar:
    usuarios.txt, amistades.txt, solicitudes.txt, mensajes.txt, albumes.txt

Uso:  python generar_datos.py [carpeta_salida]      (por defecto: ../datos)

Usuarios demo (contrasena de TODOS los usuarios: udea123):
    juanito76       -> usuario principal para el video (muchos amigos, mensajes, solicitudes)
    valentina_rios  -> amiga de juanito76 con 2 albumes visibles de 6 fotos cada uno
    pruebas_udea    -> usuario con solo 2 amigos (prueba el caso de 3 o menos amigos)
"""
import os
import random
import sys
from datetime import datetime, timedelta

random.seed(2026)

SALIDA = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "..", "datos")
CONTRASENA = "udea123"
TOTAL_OBJETIVO = 1050

NOMBRES = ["ana", "juan", "maria", "carlos", "laura", "andres", "camila", "santiago", "valentina", "daniel",
           "sofia", "mateo", "isabella", "sebastian", "mariana", "felipe", "paula", "david", "natalia", "alejandro",
           "luisa", "tomas", "sara", "esteban", "manuela", "nicolas", "daniela", "julian", "gabriela", "samuel",
           "catalina", "miguel", "juliana", "diego", "carolina", "jose", "lina", "ricardo", "angela", "pablo"]
APELLIDOS = ["gomez", "rodriguez", "lopez", "martinez", "garcia", "hernandez", "perez", "sanchez", "ramirez", "torres",
             "rios", "restrepo", "alvarez", "zapata", "castro", "ospina", "vargas", "moreno", "jaramillo", "munoz",
             "cardona", "rojas", "mejia", "giraldo", "arango", "velez", "salazar", "guerra", "quintero", "henao"]
CIUDADES_CO = ["Medellin", "Bogota", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Manizales",
               "Envigado", "Bello", "Itagui", "Rionegro", "Santa Marta", "Cucuta", "Pasto", "Monteria"]
EXTRANJERO = [("Lima", "Peru"), ("Quito", "Ecuador"), ("Ciudad de Mexico", "Mexico"), ("Buenos Aires", "Argentina"),
              ("Madrid", "Espana"), ("Santiago", "Chile"), ("Panama", "Panama")]
LUGARES = ["Cartagena", "Guatape", "SantaFe", "Jardin", "Salento", "Tayrona", "SanAndres", "Bogota", "Cali",
           "Capurgana", "Villa de Leyva", "Barichara", "Minca", "Filandia", "Medellin"]
EVENTOS = ["Grados", "Cumple", "Navidad", "Semestre", "Paseo", "Feria", "Concierto", "Mundial", "Halloween", "Familia"]
MENSAJES = [
    "Hola! Como vas con el desafio de Informatica II?",
    "Nos vemos manana en el bloque 21 a las 10?",
    "Ya subiste el informe al repositorio? Recuerda los commits cada dos dias",
    "Que buenas fotos las del paseo, me encantaron",
    "Me pasas el enlace del video cuando lo tengas listo?",
    "Feliz cumpleanos!! Que la pases muy bien",
    "Vamos a almorzar en la U hoy?",
    "Tienes los apuntes de la clase de plantillas en C++?",
    "Ya viste el partido de anoche? Que golazo",
    "Recuerda que la sustentacion es presencial; lleva el portatil",
    "Gracias por la ayuda con la matriz de adyacencia; ya me funciono",
    "Te mande la solicitud de amistad de mi primo, aceptala porfa",
    "El profe dijo que no se pueden usar contenedores de la STL",
    "Estoy en la biblioteca, vienes?",
    "Que tal el viaje? Manda fotos",
    "Mañana hay parcial de calculo, nos reunimos a estudiar?".replace("ñ", "n"),
    "Ya quedo el carrusel de fotos con el modulo; super facil",
    "Saludos desde el metro, voy tarde jaja",
    "Me prestas el cargador del portatil un momento?",
    "Se cayo la red de la U otra vez...",
]


def fecha_aleatoria(inicio, fin):
    delta = fin - inicio
    return inicio + timedelta(seconds=random.randint(0, int(delta.total_seconds())))


# ----------------------------------------------------------------------------- usuarios
nombres = set(["juanito76", "valentina_rios", "pruebas_udea"])
while len(nombres) < TOTAL_OBJETIVO:
    nombre = random.choice(NOMBRES) + "_" + random.choice(APELLIDOS)
    if random.random() < 0.7:
        nombre += str(random.randint(1, 99))
    nombres.add(nombre)

usuarios = sorted(nombres)                      # El archivo va ordenado: el Id es la posicion
indice = {nombre: i for i, nombre in enumerate(usuarios)}
N = len(usuarios)

datos_usuario = []
for nombre in usuarios:
    if random.random() < 0.9:
        ciudad, pais = random.choice(CIUDADES_CO), "Colombia"
    else:
        ciudad, pais = random.choice(EXTRANJERO)
    nacimiento = fecha_aleatoria(datetime(1965, 1, 1), datetime(2008, 12, 31)).strftime("%Y-%m-%d")
    inscripcion = fecha_aleatoria(datetime(2020, 1, 1), datetime(2026, 9, 30)).strftime("%Y-%m-%d")
    correo = nombre.replace("_", ".") + "@udea.edu.co"
    datos_usuario.append((nombre, CONTRASENA, nacimiento, correo, ciudad, pais, inscripcion))

# ----------------------------------------------------------------------------- amistades
amigos = [set() for _ in range(N)]


def unir(a, b):
    if a != b:
        amigos[a].add(b)
        amigos[b].add(a)


juan = indice["juanito76"]
vale = indice["valentina_rios"]
pruebas = indice["pruebas_udea"]

resto = [i for i in range(N) if i not in (juan, pruebas)]
random.shuffle(resto)

# Grupos (comunidades) de distintos tamanos y densidades: producen amigos en comun variados
grupos = []
pos = 0
while pos < len(resto):
    tam = random.randint(30, 55)
    if len(grupos) == 0:
        tam = 55                                # Grupo denso de juanito76
    elif len(grupos) == 1:
        tam = 45                                # Grupo medio de juanito76
    grupos.append(resto[pos:pos + tam])
    pos += tam

for g, miembros in enumerate(grupos):
    p = random.choice([0.25, 0.35, 0.45, 0.55, 0.65])
    if g == 0:
        p = 0.8                                 # Amigos con 21 o mas en comun (peso 3.5)
    elif g == 1:
        p = 0.6                                 # Amigos con 11 a 20 en comun (peso 2.5)
    for i in range(len(miembros)):
        for j in range(i + 1, len(miembros)):
            if random.random() < p:
                unir(miembros[i], miembros[j])

# Conexiones entre grupos (puentes)
for u in resto:
    for _ in range(random.randint(1, 4)):
        unir(u, random.choice(resto))

# juanito76: casi todo el grupo denso + parte de un grupo medio + algunos al azar
denso = grupos[0]
if vale not in denso:
    denso.append(vale)
    for m in denso:
        if m != vale and random.random() < 0.75:
            unir(vale, m)
for m in denso:
    if random.random() < 0.85:
        unir(juan, m)
unir(juan, vale)
medio = grupos[1]
for m in random.sample(medio, 26):
    unir(juan, m)
for m in random.sample(resto[200:], 8):          # Amigos sueltos (pocos en comun, peso 1.0)
    unir(juan, m)

# pruebas_udea: solo 2 amigos
for m in random.sample(resto, 2):
    unir(pruebas, m)

parejas = sorted((a, b) for a in range(N) for b in amigos[a] if a < b)

# ----------------------------------------------------------------------------- solicitudes
solicitudes = set()


def puede_solicitar(r, d):
    return r != d and d not in amigos[r] and (r, d) not in solicitudes and (d, r) not in solicitudes


# juanito76 recibe 4 solicitudes de no-amigos
for r in random.sample([i for i in resto if i not in amigos[juan]], 4):
    if puede_solicitar(r, juan):
        solicitudes.add((r, juan))
while len(solicitudes) < 450:
    r, d = random.randrange(N), random.randrange(N)
    if d == juan or r == juan or d == pruebas:
        continue
    if puede_solicitar(r, d):
        solicitudes.add((r, d))

# ----------------------------------------------------------------------------- mensajes
mensajes = []
ahora = datetime(2026, 10, 13, 22, 0, 0)
desde = datetime(2025, 9, 1)
for e in range(N):
    lista = list(amigos[e])
    if not lista:
        continue
    for _ in range(random.randint(4, 16)):
        r = random.choice(lista)
        f = fecha_aleatoria(desde, ahora)
        leido = 1 if (ahora - f).days > 20 or random.random() < 0.6 else 0
        mensajes.append((e, r, f, leido, random.choice(MENSAJES)))

# juanito76 recibe al menos 45 mensajes repartidos en todo el ano (6+ semanas de rango) y 8 nuevos
amigos_juan = list(amigos[juan])
for k in range(45):
    e = random.choice(amigos_juan)
    f = fecha_aleatoria(datetime(2026, 1, 5), datetime(2026, 10, 12, 20, 0, 0))
    leido = 0 if k < 8 else 1
    if leido == 0:
        f = fecha_aleatoria(datetime(2026, 10, 1), datetime(2026, 10, 12, 20, 0, 0))
    mensajes.append((e, juan, f, leido, random.choice(MENSAJES)))

mensajes.sort(key=lambda m: m[2])

# ----------------------------------------------------------------------------- albumes y fotos
lineas_albumes = []


def album(dueno, nombre, privacidad, cantidad_fotos):
    lineas_albumes.append(f"A;{dueno};{nombre};{privacidad}")
    usados = set()
    for k in range(cantidad_fotos):
        while True:
            foto = random.choice(["IMG_", "DSC_", "Foto", "PXL_"]) + str(random.randint(1000, 9999))
            if foto not in usados:
                usados.add(foto)
                break
        fecha = fecha_aleatoria(datetime(2019, 1, 1), datetime(2026, 9, 30)).strftime("%Y-%m-%d")
        tam = random.randint(180_000, 4_500_000)
        candidatos = list(amigos[dueno])
        likes = random.sample(candidatos, min(len(candidatos), random.randint(0, 12)))
        lineas_albumes.append(f"F;{foto};{fecha};{tam};" + ",".join(str(x) for x in sorted(likes)))


for u in range(N):
    if u == juan:
        album(u, "Cartagena2026", "publico", 0)
        # Fotos con los nombres del ejemplo de la guia
        lineas_albumes.append("F;Malecon2612;2026-01-12;2457600;" + ",".join(str(x) for x in sorted(random.sample(list(amigos[u]), 9))))
        lineas_albumes.append("F;Bocagrande1;2026-01-13;1835008;" + ",".join(str(x) for x in sorted(random.sample(list(amigos[u]), 5))))
        album(u, "Grados2025", "amigos", 6)
        album(u, "Personales", "privado", 4)
        continue
    if u == vale:
        album(u, "Guatape2026", "publico", 6)
        album(u, "GradosUdeA", "amigos", 6)
        album(u, "Diario", "privado", 5)
        continue
    for _ in range(random.choice([0, 1, 1, 2, 2, 2, 3, 4])):
        nombre = random.choice(LUGARES + EVENTOS).replace(" ", "") + str(random.randint(2019, 2026))
        privacidad = random.choices(["publico", "amigos", "privado"], weights=[4, 4, 2])[0]
        album(u, nombre, privacidad, random.randint(0, 9))

# ----------------------------------------------------------------------------- escritura
os.makedirs(SALIDA, exist_ok=True)


def escribir(nombre, encabezado, lineas):
    with open(os.path.join(SALIDA, nombre), "w", encoding="utf-8", newline="\n") as f:
        for e in encabezado:
            f.write("# " + e + "\n")
        for linea in lineas:
            f.write(linea + "\n")


escribir("usuarios.txt",
         ["Formato: nombreUsuario;contrasena;fechaNacimiento;correo;ciudad;pais;fechaInscripcion",
          "El Id de cada usuario es su posicion (desde 0) entre las lineas de datos. Archivo ordenado por nombre."],
         [";".join(d) for d in datos_usuario])
escribir("amistades.txt", ["Formato: idUsuarioA;idUsuarioB  (cada amistad se escribe una sola vez con idA < idB)"],
         [f"{a};{b}" for a, b in parejas])
escribir("solicitudes.txt", ["Formato: idRemitente;idDestino  (solicitudes de amistad pendientes)"],
         [f"{r};{d}" for r, d in sorted(solicitudes)])
escribir("mensajes.txt", ["Formato: idEmisor;idReceptor;AAAA-MM-DD HH:MM:SS;leido(0/1);contenido"],
         [f"{e};{r};{f.strftime('%Y-%m-%d %H:%M:%S')};{l};{c}" for e, r, f, l, c in mensajes])
escribir("albumes.txt",
         ["A;idUsuario;nombreAlbum;privacidad(publico/amigos/privado)",
          "F;nombreFoto;fechaCaptura;tamanoBytes;idsQueDieronLike(separados por coma)",
          "Cada linea F pertenece al ultimo album A escrito encima de ella."],
         lineas_albumes)

total_fotos = sum(1 for l in lineas_albumes if l.startswith("F;"))
total_albumes = len(lineas_albumes) - total_fotos
print(f"Usuarios: {N}  Amistades: {len(parejas)}  Solicitudes: {len(solicitudes)}  Mensajes: {len(mensajes)}"
      f"  Albumes: {total_albumes}  Fotos: {total_fotos}")
print(f"juanito76 (id {juan}): {len(amigos[juan])} amigos | valentina_rios (id {vale}) | pruebas_udea (id {pruebas})")
