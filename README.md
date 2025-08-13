# Proyecto PEKS con PBC en Ubuntu

Este repositorio contiene la implementación del esquema **PEKS (Public Key Encryption with Keyword Search)** usando la librería **PBC** en Ubuntu. Este README documenta los pasos para instalar dependencias, compilar la librería PBC, configurar el entorno y ejecutar el programa PEKS.

---

## 1️⃣ Requisitos previos

Antes de compilar PEKS, necesitas instalar las herramientas de compilación y la librería GMP.

```bash
# Instalar dependencias (build-essential y gmp)
sudo apt update
sudo apt install build-essential libgmp-dev

# Descargar y descomprimir la librería PBC
cd ~/Descargas
wget [https://crypto.stanford.edu/pbc/files/pbc-1.0.0.tar.gz](https://crypto.stanford.edu/pbc/files/pbc-1.0.0.tar.gz)
tar -xvf pbc-1.0.0.tar.gz
cd pbc-1.0.0

# Configurar, compilar e instalar PBC en un directorio local
./configure --prefix=$HOME/.local
make
make install

# Verificar que los archivos de la librería se instalaron correctamente
ls ~/.local/include/pbc/pbc.h
ls ~/.local/lib/libpbc.so

# Exportar la ruta de la librería para la sesión actual
export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH

# Hacer la variable de entorno permanente
echo 'export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc

# Copiar el archivo de parámetros 'a.param' al directorio del proyecto
# Asegúrate de que la ruta de destino exista. Ejemplo: mkdir -p ~/MySpace/INAOE/PEKS/
cp ~/Descargas/pbc-1.0.0/param/a.param ~/MySpace/INAOE/PEKS/

# Comando de compilación
gcc -Wall -I$HOME/.local/include/pbc -L$HOME/.local/lib -o Peks Peks.c -lpbc -lgmp

# Ejecutar el programa pasando los parámetros de la curva
./Peks < a.param

