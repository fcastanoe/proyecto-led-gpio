# Proyecto LED GPIO

Este proyecto implementa un driver de dispositivo de carácter (character device) para Linux que permite controlar un LED conectado a un pin GPIO de una Raspberry Pi 5. Incluye:

- Módulo de kernel (`kernel-module/led_gpio.c`) que:
  - Registra un dispositivo de carácter.
  - Expone operaciones estándar: `open`, `release`, `read`, `write`.
  - Controla el estado de un LED en el pin BCM17.
- Aplicación de espacio de usuario en C (`user-app/led_ctl.c`) que:
  - Permite encender y apagar el LED enviando comandos al dispositivo.
- Makefiles para compilación en ambas carpetas.
- Instrucciones de instalación, compilación y uso.

---

## Contenido del repositorio

```bash
proyecto-led-gpio/
├── kernel-module/
│   ├── led_gpio.c      # Código fuente del módulo de kernel
│   └── Makefile        # Makefile para compilar el módulo
├── user-app/
│   ├── led_ctl.c       # Aplicación de usuario para controlar el LED
│   └── Makefile        # Makefile para compilar la aplicación
└── README.md           # Documentación del proyecto
```
---
## Conexiones de hardware

1. LED:

   - Ánodo (patita larga) → Resistencia de 330 Ω → GPIO17 (BCM17, pin físico 11)
   - Cátodo (patita corta) → GND (pin físico 6)

2. Pantalla → Puerto HDMI de la Raspberry Pi 5

3. Teclado → Puerto USB de la Raspberry Pi 5

---

## Requisitos previos

- Raspberry Pi OS (64-bit) actualizado.

- Herramientas de compilación:

```bash
sudo apt update
sudo apt install -y build-essential raspberrypi-kernel-headers git
```
- Editor de texto (por ejemplo, nano o vim).

- Gestor de versiones git.

---

## Compilación y carga del módulo de kernel

1. Accede al directorio del módulo:

   ```bash
   cd kernel-module
   ```

2. Compila el módulo:

   ```bash
   make
   ```
   
3. Carga el módulo en el kernel:

   ```bash
   sudo insmod led_gpio.ko
   ```

4. Verifica la carga con dmesg:

   ```bash
   dmesg | tail -n 5
   # Deberías ver un mensaje: "ledgpio: cargado major=<número>"
   ```

5. Crea el dispositivo en /dev y ajusta permisos:

   ```bash
   MAJOR=$(dmesg | tail -n1 | awk '{print $NF}')
   sudo mknod /dev/ledgpio c $MAJOR 0
   sudo chmod 666 /dev/ledgpio
   ```
---

## Compilación de la aplicación de usuario
1. Accede al directorio de la aplicación:
   
   ```bash
   cd ../user-app
   ```
2. Compila la aplicación:
   ```bash
   make
   ```
---

## Uso

- Encender LED:
  
  ```bash
  ./led_ctl on /dev/ledgpio
  ```
  
- Apagar LED:
  
  ```bash
  ./led_ctl off /dev/ledgpio
  ```
- Leer estado del LED:

  ```bash
  cat /dev/ledgpio
  # Devuelve '1' si está encendido, '0' si está apagado
  ```

---

## Desinstalación del módulo

1. Elimina el dispositivo:

   ```bash
   sudo rm /dev/ledgpio
   ```
   
2. Descarga el módulo:

   ```bash
   sudo rmmod led_gpio
   ```
   
3. Limpia los archivos compilados:

   ```bash
   cd kernel-module
   make clean
   cd ../user-app
   make clean
   ```
---

## Estructura de commits y GitHub

- Cada funcionalidad o corrección de errores debe incluirse en un commit independiente con un mensaje descriptivo.

- El repositorio remoto está alojado en GitHub: https://github.com/fcastanoe/proyecto-led-gpio

---

## Solución de problemas

- Error al compilar: Verifica que /lib/modules/$(uname -r)/build apunte a los headers correctos.

- Permisos: Asegúrate de tener permisos de escritura en /dev/ledgpio.

- Red: Si apt update falla, revisa tu conexión o espejos.
  
