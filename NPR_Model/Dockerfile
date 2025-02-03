FROM nvidia/cuda:12.1.1-runtime-ubuntu22.04

# Çalışma dizinini oluştur
WORKDIR /app

# GPU erişimi için çevresel değişkeni ayarla
ENV CUDA_VISIBLE_DEVICES=0

# Gerekli sistem paketlerini yükle
RUN apt-get update && apt-get install -y \
    python3 python3-pip libgl1 libglib2.0-0 && \
    apt-get clean

# pip'i güncelle
RUN python3 -m pip install --upgrade pip

# PyTorch ve diğer kütüphaneleri yükle
RUN pip install --no-cache-dir torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

# Diğer kütüphaneleri yükle
RUN pip install --no-cache-dir numpy opencv-python scikit-learn pyzmq

# OpenCV'yi doğrula (isteğe bağlı)
RUN python3 -c "import cv2; print('OpenCV versiyonu:', cv2.__version__)"

# Model ve kodları kopyala
COPY NPR /app/NPR

# Çalıştırma komutunu belirle
CMD ["python3", "detection.py", "--model_path", "NPR/NPR.pth", "--batch_size", "1"]
