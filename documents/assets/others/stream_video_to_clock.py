import serial.tools.list_ports
import numpy as np
# Import everything needed to edit video clips
import cv2 as cv
ser = serial.Serial(port="COM15", baudrate=115200)

data_frame = []
data_frame = list()

video_capture = cv.VideoCapture(0)

while(video_capture.isOpened()):
    ret, frame = video_capture.read()
    if not ret: break

    width = int(128)
    height = int(64)
    dim = (width, height)
    frame = cv.resize(frame, dim, interpolation =cv.INTER_AREA)

    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
 
    frame = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY_INV, 11, 2)

    data = np.array(frame)
    for h in range(0, 64, 1):
        for w in range(0, 128, 8):
            pixel = 0
            pixel = pixel | ( 1 << 7 if data[h][w] > 0 else 0 )
            pixel = pixel | ( 1 << 6 if data[h][w + 1] > 0 else 0 )
            pixel = pixel | ( 1 << 5 if data[h][w + 2] > 0 else 0 )
            pixel = pixel | ( 1 << 4 if data[h][w + 3] > 0 else 0 )
            pixel = pixel | ( 1 << 3 if data[h][w + 4] > 0 else 0 )
            pixel = pixel | ( 1 << 2 if data[h][w + 5] > 0 else 0 )
            pixel = pixel | ( 1 << 1 if data[h][w + 6] > 0 else 0 )
            pixel = pixel | ( 1 << 0 if data[h][w + 7] > 0 else 0 )
            data_frame.append(pixel)
    
    bytearray(data_frame)
    ser.write(data_frame)
    data_frame.clear()

    while True:
        bytesToRead = ser.inWaiting()
        if (bytesToRead > 0):
            bytesToRead == "R"
            break

    key = cv.waitKey(1)
    if key & 0xFF == ord('q') or key == 27 or 'x' == chr(key & 255):
        break

video_capture.release()
cv.destroyAllWindows()

# print(name)

# mess = ""

# def readSerial():
#     bytesToRead = ser.inWaiting()
#     if (bytesToRead > 0):
#         global mess
#         mess = mess + ser.read(bytesToRead).decode("UTF-8")
#         while ("\n" in mess):
#             print(mess)
#             mess = ""

# while True:
#     readSerial()
