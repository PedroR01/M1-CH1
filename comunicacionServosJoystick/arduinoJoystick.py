import pygame
import serial
import serial.tools.list_ports

# Lista de puertos disponibles
ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)

arduino = None

try:
    # Configurar el puerto serie
    arduino = serial.Serial('COM3', 9600)  # Reemplaza 'COM3' con el puerto de tu Arduino
    print("Conexión establecida con el Arduino")
except serial.SerialException as e:
    print(f"No se pudo abrir el puerto: {e}")

# Inicializar pygame
pygame.init()
pygame.joystick.init()

# Verificar si hay joystick conectado
if pygame.joystick.get_count() == 0:
    print("No joystick connected")
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print("Joystick connected:", joystick.get_name())

while arduino != None:
    pygame.event.pump()
    
    # Leer los valores de los ejes analógicos
    axis_left_x = joystick.get_axis(0)
    axis_left_y = joystick.get_axis(1)
    axis_right_x = joystick.get_axis(2)
    axis_right_y = joystick.get_axis(3)
    
    # Mapear los valores de -1 a 1 a 5 a 70 para los servos
    servo_left_x_1 = int((axis_left_x + 1) * 32.5 + 5)
    servo_left_y_1 = int((axis_left_y + 1) * 32.5 + 5)
    servo_right_x_1 = int((axis_right_x + 1) * 32.5 + 5)
    servo_right_y_1 = int((axis_right_y + 1) * 32.5 + 5)
    
    # Imprimir los valores en la consola de Python
    print(f"Joystick Izquierdo X: {axis_left_x}, Joystick Izquierdo Y: {axis_left_y}")
    print(f"Servo Izquierdo X1: {servo_left_x_1}, Servo Izquierdo Y1: {servo_left_y_1}")
    
    print(f"Joystick Derecho X: {axis_right_x}, Joystick Derecho Y: {axis_right_y}")
    print(f"Servo Derecho X1: {servo_right_x_1}, Servo Derecho Y1: {servo_right_y_1}")
    
    # Enviar los valores al Arduino solo si la conexión es exitosa
    if arduino is not None:
        try:
            arduino.write(f'{servo_left_x_1},{servo_left_y_1},{servo_right_x_1},{servo_right_y_1}\n'.encode())
        except Exception as e:
            print(f"Error al escribir en el puerto serie: {e}")
    
    pygame.time.wait(100)
