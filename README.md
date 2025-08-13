Claro, aquí tienes el código mejorado para tu README.md, con una estructura clara, títulos para cada paso y explicaciones concisas.

Este formato es mucho más fácil de leer y seguir.

Markdown

# Proyecto PEKS con PBC en Ubuntu

Este repositorio contiene la implementación del esquema **PEKS (Public Key Encryption with Keyword Search)** usando la librería **PBC** en Ubuntu. Este README documenta los pasos para instalar dependencias, compilar la librería PBC, configurar el entorno y ejecutar el programa PEKS.

---

## 1️⃣ Paso 1: Instalar Requisitos Previos

Antes de empezar, necesitas las herramientas de compilación (`build-essential`) y la librería de manejo de números grandes (`libgmp-dev`).

```bash
# Actualizar lista de paquetes e instalar dependencias
sudo apt update
sudo apt install build-essential libgmp-dev
2️⃣ Paso 2: Descargar y Compilar la Librería PBC
Ahora, descargaremos y compilaremos la librería criptográfica PBC desde la fuente.

2.1. Descargar y Descomprimir
Navega a tu carpeta de descargas, obtén el archivo y descomprímelo.

Bash

# Navegar, descargar y descomprimir
cd ~/Descargas
wget [https://crypto.stanford.edu/pbc/files/pbc-1.0.0.tar.gz](https://crypto.stanford.edu/pbc/files/pbc-1.0.0.tar.gz)
tar -xvf pbc-1.0.0.tar.gz
cd pbc-1.0.0
2.2. Configurar, Compilar e Instalar
Instalaremos la librería en un directorio local (~/.local) para no requerir permisos de superusuario (sudo).

Bash

# Configurar para instalación local, compilar e instalar
./configure --prefix=$HOME/.local
make
make install
2.3. Verificar la Instalación
Comprueba que los archivos de la librería y de cabecera se hayan instalado en la ubicación correcta.

Bash

# Verificar la existencia de los archivos clave
ls ~/.local/include/pbc/pbc.h
ls ~/.local/lib/libpbc.so
3️⃣ Paso 3: Configurar Variables de Entorno
Para que el sistema encuentre la librería libpbc.so al momento de ejecutar programas, debes añadir su ruta a la variable LD_LIBRARY_PATH.

Bash

# Exportar la ruta para la sesión actual de la terminal
export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH
Para que este cambio sea permanente y no tengas que repetirlo cada vez que abres una terminal, agrégalo a tu .bashrc:

Bash

# Hacer el cambio permanente y recargar la configuración
echo 'export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
4️⃣ Paso 4: Preparar Archivo de Parámetros
Nuestro programa PEKS necesita un archivo con los parámetros de la curva elíptica para funcionar. Copiaremos uno de los archivos de ejemplo de PBC a nuestro directorio de trabajo.

Bash

# Copiar el archivo 'a.param' al directorio del proyecto
# (Asegúrate de que la ruta de destino exista)
# Ejemplo: mkdir -p ~/MySpace/INAOE/PEKS/
cp ~/Descargas/pbc-1.0.0/param/a.param ~/MySpace/INAOE/PEKS/
5️⃣ Paso 5: Compilar el Programa PEKS
Ahora, compila tu código fuente (Peks.c) en un archivo ejecutable. Este comando le dice al compilador dónde encontrar las librerías y cabeceras de PBC.

Bash

# Comando de compilación
gcc -Wall -I$HOME/.local/include/pbc -L$HOME/.local/lib -o Peks Peks.c -lpbc -lgmp
6️⃣ Paso 6: Ejecutar el Programa
Finalmente, ejecuta tu programa compilado. Es importante redirigir el archivo de parámetros a.param a la entrada estándar del programa, ya que así está diseñado para leer la configuración.

Bash

# Ejecutar el programa pasando los parámetros de la curva
./Peks < a.param
✅ Resultado Esperado
Si todo salió bien, deberías ver una salida en la terminal que confirme que las claves se generaron, se creó el criptograma y la prueba de búsqueda fue exitosa.

Plaintext

1. Generando claves...
Claves generadas exitosamente.
...
