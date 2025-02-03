#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/filesystem.hpp>
#include <zmq.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <csignal>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace json = boost::json;
namespace fs = boost::filesystem;

// React build klasör yolu
const std::string build_path = "/home/basaran/Desktop/react-deneme/my-app/build";

// MIME türlerini belirlemek için bir harita
std::unordered_map<std::string, std::string> mime_types = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".json", "application/json"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".ico", "image/x-icon"}
};

// Dosya içeriğini okuma fonksiyonu
std::string load_file_content(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) return "";  // Dosya bulunamazsa boş string döndür
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

// Dosyanın MIME türünü belirleme
std::string get_mime_type(const std::string& path) {
    std::string ext = fs::extension(path);
    return mime_types.count(ext) ? mime_types[ext] : "application/octet-stream";
}

// React build dosyalarını sunma
void serve_static_files(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    std::string target = std::string(req.target());
    if (target == "/") target = "/index.html";  // Ana sayfa yönlendirme

    std::string file_path = build_path + target;

    if (fs::exists(file_path)) {
        std::string content = load_file_content(file_path);

        res.result(http::status::ok);
        res.set(http::field::content_type, get_mime_type(file_path));
        res.body() = content;
        res.prepare_payload();
    } else {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "404 - Not Found";
        res.prepare_payload();
    }
}
/*
// ZeroMQ kullanarak yüz tanıma servisine görüntü gönderme
std::string send_image_to_face_detection(zmq::context_t& context, const std::string& base64_image) {
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");  // Yüz tespit uygulamasına bağlan

    // Görüntüyü gönder
    socket.send(zmq::buffer(base64_image), zmq::send_flags::none);

    // Yanıtı al
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    
    return std::string(static_cast<char*>(reply.data()), reply.size());
}
*/
// ZeroMQ kullanarak bir modele görüntü gönderme
std::string send_image_to_model(zmq::context_t& context, const std::string& base64_image, const std::string& address) {
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(address);  

    // Görüntüyü gönder
    socket.send(zmq::buffer(base64_image), zmq::send_flags::none);

    // Yanıtı al
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    
    return std::string(static_cast<char*>(reply.data()), reply.size());
}

// HTTP isteklerini işleme
void handle_request(zmq::context_t& context, const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    // CORS başlıkları
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type");

    if (req.method() == http::verb::options) {
        res.result(http::status::no_content);
        res.prepare_payload();
        return;
    }
    
     if (req.method() == http::verb::post && req.target() == "/upload") {
        try {
            auto gelenImage = json::parse(req.body());
            std::string base64_image = json::value_to<std::string>(gelenImage.at("image"));
            std::cout<<"Görüntü yüz tespiti için gönderildi.";
            // Yüz tespit uygulamasına gönder (tcp://localhost:5555)
            std::string detection_result = send_image_to_model(context, base64_image, "tcp://localhost:5555");
            std::cout<<"Number of faces detected:: "+ detection_result;

            int detected_face_count = std::stoi(detection_result);

            if (detected_face_count>0) {
                // Eğer yüz tespit edildiyse, NPR modeline gönder (tcp://localhost:5554)
                std::string npr_result = send_image_to_model(context, base64_image, "tcp://localhost:5560");

                json::value response_body = { {"message", " Face detected"}, {"npr_result", npr_result} };
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = json::serialize(response_body);
            } else {
                //  Yüz bulunamazsa
                json::value response_body = { {"message", "No face detected"} };
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = json::serialize(response_body);
            }

            res.prepare_payload();
        }
        catch (const std::exception& e) {
            res.result(http::status::bad_request);
            res.body() = "Geçersiz JSON formatı.";
            res.prepare_payload();
        }
    } else {
        serve_static_files(req, res);
    }
}

// Sunucuyu başlatma
void run_server() {
    try {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "Sunucu 8080 portunda çalışıyorA...\n";

        zmq::context_t zmq_context(1);  // ZMQ için tek bir context oluştur

        while (true) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            handle_request(zmq_context, req, res);

            http::write(socket, res);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
    }
}

// Sunucu kapanırken çağrılacak fonksiyon
void stopDockerContainers() {
    std::cout << "\nStopping Docker containers..." << std::endl;
    std::system("docker stop face_detector deepfake_detector");
    std::exit(0);  // Programı güvenli şekilde sonlandır
}

// Sinyalleri yakalayacak fonksiyon
void signalHandler(int signum) {
    std::cout << "\nSignal (" << signum << ") received. Stopping server..." << std::endl;
    stopDockerContainers();
}

bool runDockerModel() {
    int status;

    // Sinyal işleyiciyi ayarla (Ctrl+C ve diğer çıkış sinyalleri için)
    std::signal(SIGINT, signalHandler);  // Ctrl+C yakala
    std::signal(SIGTERM, signalHandler); // kill veya q sinyali yakala

    // Face Detection Model
    status = std::system("docker run --rm -d --name face_detector -p 5555:5555 face_detection_app2");
    if (status == -1) {
        std::cerr << "Error: Failed to run Face Detection Docker container" << std::endl;
        return false;
    }
    std::cout << "Face Detection Docker container started successfully" << std::endl;

    // Deepfake Detection Model
    status = std::system("docker run --rm -d --gpus all --name deepfake_detector -p 5560:5560 deepfake_detector");
    if (status == -1) {
        std::cerr << "Error: Failed to run Deepfake Detection Docker container" << std::endl;
        return false;
    }
    std::cout << "Deepfake Detection Docker container started successfully" << std::endl;

    return true;
}


int main() {
   
    if(runDockerModel()){

        std::cout<<"Modeller çalıştrıldı!";        
   
        // Varsayılan tarayıcıda http://localhost:8080 adresini aç
        system("xdg-open http://localhost:8080");  // Linux
        
        run_server();
    }
    
    return 0;
}
