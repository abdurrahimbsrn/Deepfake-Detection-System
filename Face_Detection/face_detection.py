import zmq
import base64
import numpy as np
import cv2
from retinaface import RetinaFace

def base64_to_image(base64_str):
    base64_str = base64_str.split(',')[1]
    print("Görüntü Geldi")
    img_data = base64.b64decode(base64_str)
    np_arr = np.frombuffer(img_data, np.uint8)
    img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

    # Hata ayıklama bilgileri
    if img is None:
        print("Görüntü decode edilmedi.")
    return img

def detect_faces(image):
    faces = RetinaFace.detect_faces(image)
    return len(faces)

def main():
    print("RetinaFace çalışıyor!")
    context = zmq.Context()
    socket = context.socket(zmq.REP)  # REP soketi oluşturuyoruz
    socket.bind("tcp://*:5555")  # API sunucusunun bağlanacağı port

    while True:
        # API sunucusundan görüntüyü al
        base64_image = socket.recv_string()
        print("Görüntü alındı.")

        # Base64'ü görüntüye dönüştür
        image = base64_to_image(base64_image)

        # Yüzleri tespit et
        face_count = detect_faces(image)

        # Sonucu döndür
        response = f"{face_count}"
        socket.send_string(response)
        print("Yanıt gönderildi.")
        print("Tespit edilen yüz sayısı: ", response)

if __name__ == "__main__":
    main()
