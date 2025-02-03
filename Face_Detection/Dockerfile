# Python 3.9 tabanlı bir imaj kullanın
FROM python:3.9

# Gerekli sistem kütüphanelerini yükleyin
RUN apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    build-essential \
    python3-dev \
    libsm6 \
    libxext6 \
    libxrender-dev \
    && rm -rf /var/lib/apt/lists/*

# Çalışma dizinini ayarlayın
WORKDIR /app

# Gerekli Python kütüphanelerini yükleyin
RUN pip install --no-cache-dir \
    opencv-python \
    numpy \
    pyzmq \
    retina-face \
    tf-keras

# face_detection.py dosyasını konteynerin içine kopyalayın
COPY face_detection.py /app/

# face_detection.py dosyasını çalıştırın
CMD ["python", "face_detection.py"]
