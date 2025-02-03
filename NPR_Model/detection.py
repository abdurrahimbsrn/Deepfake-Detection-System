import zmq
import base64
import os
import torch
import cv2
import numpy as np
import random
from PIL import Image
from torchvision import transforms
from networks.resnet import resnet50
from options.test_options import TestOptions

# Rastgelelikleri sabitle
def seed_torch(seed=1029):
    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)  # Çoklu GPU için
    torch.backends.cudnn.benchmark = False
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.enabled = False

seed_torch(100)

# Modeli yükle
opt = TestOptions().parse(print_options=False)
print(f'Model yükleniyor: {opt.model_path}')

model = resnet50(num_classes=1)
checkpoint = torch.load(opt.model_path, map_location='cpu')

# Model ağırlıklarını yükle
from collections import OrderedDict
new_state_dict = OrderedDict()
for k, v in checkpoint['model'].items():
    label = k[7:]  # "module." önekini kaldır
    new_state_dict[label] = v

model.load_state_dict(new_state_dict, strict=True)
model.cuda()
model.eval()

# Base64 string'ini görüntüye çevirme fonksiyonu
def base64_to_image(base64_str):
    base64_str = base64_str.split(',')[1]  # Base64 başlığını kaldır
    img_data = base64.b64decode(base64_str)
    np_arr = np.frombuffer(img_data, np.uint8)
    img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

    if img is None:
        print("Görüntü decode edilemedi.")
        return None

    # OpenCV (BGR) -> PIL (RGB) dönüşümü
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = Image.fromarray(img)  # NumPy dizisini PIL görüntüsüne çevir

    return img

# Görüntüyü modele uygun şekilde dönüştür
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

def process_image(image):
    img = transform(image).unsqueeze(0).cuda()
    with torch.no_grad():
        output = model(img).sigmoid()
    return output.item()

# ZeroMQ ile Sunucu
context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5560")

print("ZeroMQ sunucu başlatıldı, bağlantı bekleniyor...")

while True:
    # Base64 formatındaki görüntüyü al
    base64_image = socket.recv_string()
    print("Görüntü alındı, işleniyor...")

    # Base64'ü görüntüye çevir
    image = base64_to_image(base64_image)
    
    # Modeli çalıştır ve tahmini al
    result = process_image(image)
    result=result*100
    result=result-100
    result=result*(-1)
    
    # Sonucu gönder
    response = f"{result:.2f}"
    socket.send_string(response)
    print("Sonuç gönderildi.")
