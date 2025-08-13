# Proyecto PEKS con PBC en Ubuntu

Este repositorio contiene la implementación del esquema **PEKS (Public Key Encryption with Keyword Search)** usando la librería **PBC** en Ubuntu.  
Este README documenta los pasos para instalar dependencias, compilar la librería PBC, configurar el entorno y ejecutar el programa PEKS.

---

## 1️⃣ Requisitos previos

Antes de compilar PEKS, necesitas instalar las herramientas de compilación y la librería GMP:

```bash
sudo apt update
sudo apt install build-essential libgmp-dev
```

## 2️⃣ Descargar y compilar PBC
```bash
cd ~/Descargas
wget https://crypto.stanford.edu/pbc/files/pbc-1.0.0.tar.gz
tar -xvf pbc-1.0.0.tar.gz
cd pbc-1.0.0
./configure --prefix=$HOME/.local
make
make install
```

## 3️⃣ Configurar variables de entorno 
```bash
export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH

Para hacerlo permanente:
echo 'export LD_LIBRARY_PATH=$HOME/.local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

## 4️⃣ Preparar archivo de parámetros de curva 
```bash
cp ~/Descargas/pbc-1.0.0/param/a.param ~/MySpace/INAOE/PEKS/}
```

## 5️⃣ Compilar tu programa PEKS 
```bash
gcc -Wall -I$HOME/.local/include/pbc -L$HOME/.local/lib -o Peks Peks.c -lpbc -lgmp
```

## 6️⃣ Ejecutar el programa 
```bash
./Peks < a.param
```

