import pygame
import serial
import time

# Inicializar pygame
pygame.init()
pygame.joystick.init()

# Verificar si hay joystick conectado
if pygame.joystick.get_count() == 0:
    print("No joystick connected")
else:
    joystick1 = pygame.joystick.Joystick(0)
    joystick1.init()
    print("Joystick connected:", joystick1.get_name(), " - Player 1")

    joystick2 = None
    if pygame.joystick.get_count() > 1:
        joystick2 = pygame.joystick.Joystick(1)
        joystick2.init()
        print("Joystick connected:", joystick2.get_name(), " - Player 2")

# Configurar el puerto serie
try:
    arduino = serial.Serial('COM3', 9600)  # Reemplaza 'COM3' con el puerto de tu Arduino
    time.sleep(2)  # Esperar a que la conexión se establezca
    print("Conexión establecida con el Arduino")
except serial.SerialException as e:
    print(f"No se pudo abrir el puerto: {e}")
    exit()

def map_value(value, original_min, original_max, new_min, new_max):
    # Mapea los valores del joystick al rango de 5 a 60
    original_span = original_max - original_min
    new_span = new_max - new_min

    value_scaled = float(value - original_min) / float(original_span)

    return int(new_min + (value_scaled * new_span))

control_enabled = True

while True:
    try:
        pygame.event.pump()
        
        # Detectar si se ha presionado el botón X del joystick
        buttons = joystick1.get_numbuttons()
        for i in range(buttons):
            if joystick1.get_button(i) == 1:
                if i == 0:  # El botón X del DualShock 4 es el 0
                    control_enabled = not control_enabled
                    print(f"Control {'habilitado' if control_enabled else 'deshabilitado'}")
                    time.sleep(0.5)  # Para evitar múltiples detecciones rápidas

        if control_enabled:
            # Leer los valores de los ejes analógicos
            axis_left_x_1 = joystick1.get_axis(0)  # Eje X del joystick1 izquierdo
            axis_right_x_1 = joystick1.get_axis(2)  # Eje X del joystick1 derecho
            
            # Mapear los valores de -1 a 1 a 5 a 60 para los servos
            servo1_value = map_value(axis_left_x_1, -1, 1, 0, 90)
            servo2_value = map_value(axis_right_x_1, -1, 1, 5, 90)
            
            # Enviar los valores al Arduino
            if joystick2 is None:
                servo3_value = 85
                servo4_value = 10
            else:
                axis_left_x_2 = joystick2.get_axis(0)  # Eje X del joystick2 izquierdo
                axis_right_x_2 = joystick2.get_axis(2)  # Eje X del joystick2 derecho
                servo1_value = map_value(axis_left_x_2, -1, 1, 5, 90)
                servo2__value = map_value(axis_right_x_2, -1, 1, 5, 90)

            data_to_send = f'{servo1_value},{servo2_value},{servo3_value},{servo4_value}\n'
            arduino.write(data_to_send.encode())
            
            # Leer y mostrar los datos recibidos del Arduino
            if arduino.in_waiting > 0:
                received_data = arduino.readline().decode().strip()
                print(f"Recibido del Arduino: {received_data}")
            
            # Imprimir los valores enviados
            print(f"Enviado: {data_to_send.strip()}")
        
        time.sleep(0.1)
    except Exception as e:
        print(f"Error al escribir en el puerto serie: {e}")
