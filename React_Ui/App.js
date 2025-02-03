import React, { useState } from 'react';
import axios from 'axios';
import { Button, Typography, Box, CircularProgress, Alert, Paper } from '@mui/material';
import CloudUploadIcon from '@mui/icons-material/CloudUpload';
import DescriptionIcon from '@mui/icons-material/Description';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';
import ErrorIcon from '@mui/icons-material/Error';

// toBase64 fonksiyonu
const toBase64 = (file) =>
  new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = () => resolve(reader.result);
    reader.onerror = (error) => reject(error);
  });

function App() {
  const [selectedFile, setSelectedFile] = useState(null);
  const [previewUrl, setPreviewUrl] = useState(null);
  const [response, setResponse] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const handleFileChange = (event) => {
    const file = event.target.files[0];
    setSelectedFile(file);
    setError(null);
    setPreviewUrl(URL.createObjectURL(file));
  };

  const handleUpload = async () => {
    if (!selectedFile) {
      setError('Please select a photo!');
      return;
    }

    try {
      setLoading(true);
      setError(null);

      const base64Image = await toBase64(selectedFile);
      const res = await axios.post('http://localhost:8080/upload', { image: base64Image });

      setResponse(res.data);
    } catch (error) {
      console.error('Upload error:', error);
      setError('An error occurred while uploading the photo. Please try again.');
    } finally {
      setLoading(false);
    }
  };

  // JSON çıktısını düz metne dönüştürme ve koşula göre sonuç belirleme
  const formatResponse = (response) => {
    if (!response) return null;

    const { message, npr_result } = response;
    const nprValue = parseFloat(npr_result); // npr_result'u sayıya çevir

    if (nprValue >= 50.0) {
      return `${message}, this image is ${npr_result}% likely to be REAL.`;
    } else {
      return `${message}, this image is ${100 - npr_result}% likely to be FAKE.`;
    }
  };

  // Analiz sonucuna göre renk belirleme
  const getResultColor = (npr_result) => {
    const nprValue = parseFloat(npr_result);

    if (nprValue < 0) {
      return '#757575'; // Geçersiz sonuç için gri renk
    } else if (nprValue >= 50.0) {
      return '#2e7d32'; // Gerçek sonuç için koyu yeşil
    } else {
      return '#d32f2f'; // Sahte sonuç için koyu kırmızı
    }
  };

  return (
    <Box
      sx={{
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        minHeight: '100vh',
        background: 'linear-gradient(135deg, #2A004E, #1D24CA)',
        padding: 4,
      }}
    >
      <Paper
        elevation={5}
        sx={{
          padding: 6,
          borderRadius: 3,
          bgcolor: 'white',
          width: '100%',
          maxWidth: 800,
          textAlign: 'center',
        }}
      >
        <Typography
          variant="h3"
          gutterBottom
          sx={{
            color: '#6a11cb',
            fontWeight: 'bold',
            background: 'linear-gradient(135deg, #2A004E, #1D24CA)',
            WebkitBackgroundClip: 'text',
            WebkitTextFillColor: 'transparent',
          }}
        >
          Deepfake Detection System
        </Typography>

        <Typography variant="body1" sx={{ marginBottom: 4, color: '#555', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
          <DescriptionIcon sx={{ marginRight: 1, color: '#6a11cb' }} />
          This application analyzes whether the photos you upload are Deepfake or not. Please select a photo and click the "Upload and Analyze" button.
        </Typography>

        <Box sx={{ marginBottom: 4 }}>
          <input
            type="file"
            onChange={handleFileChange}
            accept="image/*"
            style={{ display: 'none' }}
            id="upload-button"
          />
          <label htmlFor="upload-button">
            <Button
              variant="contained"
              component="span"
              startIcon={<CloudUploadIcon />}
              sx={{
                bgcolor: '#6a11cb',
                '&:hover': { bgcolor: '#2575fc' },
                padding: '12px 24px',
                fontSize: '16px',
                borderRadius: '8px',
                textTransform: 'none',
              }}
            >
              Select Photo
            </Button>
          </label>
        </Box>

        {previewUrl && (
          <Box
            component="img"
            src={previewUrl}
            alt="Fotoğraf Önizleme"
            sx={{
              width: 300,
              height: 300,
              borderRadius: 2,
              objectFit: 'cover',
              marginBottom: 4,
              boxShadow: '0px 4px 15px rgba(0, 0, 0, 0.1)',
              border: '3px solid #6a11cb',
            }}
          />
        )}
        <br />
        <Button
          variant="contained"
          color="primary"
          onClick={handleUpload}
          disabled={loading}
          sx={{
            marginBottom: 4,
            bgcolor: '#6a11cb',
            '&:hover': { bgcolor: '#2575fc' },
            padding: '12px 24px',
            fontSize: '16px',
            borderRadius: '8px',
            textTransform: 'none',
          }}
        >
          {loading ? <CircularProgress size={24} sx={{ color: 'white' }} /> : 'Upload and Analyze'}
        </Button>

        {error && (
          <Alert
            severity="error"
            sx={{ marginBottom: 4, borderRadius: '8px', alignItems: 'center' }}
            icon={<ErrorIcon sx={{ color: '#d32f2f' }} />}
          >
            {error}
          </Alert>
        )}

        {response && (
          <Box
            sx={{
              marginTop: 4,
              padding: 3,
              borderRadius: 2,
              bgcolor: '#f5f5f5',
              boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.1)',
              maxWidth: 500,
              textAlign: 'center',
              border: `2px solid ${getResultColor(response.npr_result)}`, // Çerçeve rengi
              mx: 'auto',
            }}
          >
            <Typography variant="h6" gutterBottom sx={{ color: getResultColor(response.npr_result), display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
              <CheckCircleIcon sx={{ marginRight: 1, color: getResultColor(response.npr_result) }} />
              Analysis Result
            </Typography>
            <Typography variant="body1" sx={{ marginBottom: 1, color: getResultColor(response.npr_result), textAlign: 'center' }}>
              {formatResponse(response)}
            </Typography>
          </Box>
        )}
      </Paper>
    </Box>
  );
}

export default App;