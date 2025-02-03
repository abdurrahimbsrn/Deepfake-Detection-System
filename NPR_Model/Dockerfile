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
CMD ["python3", "NPR/test3.py", "--model_path", "NPR/NPR.pth", "--batch_size", "32"]

#
#FROM nvidia/cuda:12.1.1-runtime-ubuntu22.04
#
## Çalışma dizinini oluştur
#WORKDIR /app
#
## Gerekli paketleri yükle
#RUN apt-get update && apt-get install -y \
#    python3 python3-pip libgl1 libglib2.0-0
#
## PyTorch, scikit-learn ve diğer gerekli kütüphaneleri yükle
#RUN pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
#RUN pip install numpy opencv-python scikit-learn
#
## Model ve kodları kopyala
#COPY NPR /app/NPR
#
## Çalıştırma komutunu belirle
#CMD ["python3", "NPR/test2.py", "--model_path", "NPR/NPR.pth", "--batch_size", "32"]
#
###############################################################3
#
#FROM nvidia/cuda:12.1.1-runtime-ubuntu22.04
#
## Çalışma dizinini oluştur
#WORKDIR /app
#
## Gerekli sistem paketlerini yükle
#RUN apt-get update && apt-get install -y \
#    python3 python3-pip libgl1 libglib2.0-0 && \
#    apt-get clean
#
## pip'i güncelle
#RUN python3 -m pip install --upgrade pip
#
## PyTorch ve diğer kütüphaneleri yükle
#RUN pip install --no-cache-dir torch torchvision torchaudio -f https://download.pytorch.org/whl/cu121.html
#RUN pip install --no-cache-dir numpy opencv-python scikit-learn
#RUN pip install --no-cache-dir pyzmq
#
## Model ve kodları kopyala
#COPY NPR /app/NPR
#
## Çalıştırma komutunu belirle
#CMD ["python3", "NPR/test2.py", "--model_path", "NPR/NPR.pth", "--batch_size", "32"]
#
#