#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <cstring> // For strerror
#include <cerrno>  // For errno

#include "file_transfer.h"
#include "shared.h"

int send_file(int socket, const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    file_header header;
    header.file_size = std::filesystem::file_size(filepath);
    header.filename = std::filesystem::path(filepath).filename().string();

    // Send filename length
    uint16_t name_length = header.filename.size();
    if (send(socket, &name_length, sizeof(name_length), 0) < 0) {
        std::cerr << "Failed sending filename length: " << strerror(errno) << std::endl;
        return -1;
    };

    // Send filename
    if (send(socket, header.filename.c_str(), name_length, 0) < 0) {
        std::cerr << "Failed sending filename: " << strerror(errno) << std::endl;
        return -1;
    };

    // Send filesize
    if (send(socket, &header.file_size, sizeof(header.file_size), 0) < 0) {
        std::cerr << "Failed sending filesize: " << strerror(errno) << std::endl;
        return -1;
    };

    std::vector<char> buffer(BUFFER_SIZE);

    std::cout << "Reading file: " << header.filename << std::endl;
    std::int16_t chunk = 1;
    while (file) {

        file.read(buffer.data(), buffer.size());
        std::streamsize bytes_read = file.gcount();

        if (bytes_read > 0) {

            if (bytes_read < buffer.size()) {
                std::fill(buffer.begin() + bytes_read, buffer.end(), 0);
            }

            if (send(socket, buffer.data(), bytes_read, 0) > 0) {
                std::cout << "Chunk sent: " << chunk << std::endl;
                chunk += 1;
            } else {
                std::cerr << "Error sending chunk: " << chunk << " " << strerror(errno) << std::endl;
            };
        }

    }
};

std::string receive_file(int socket, const std::string& output_dir) {
    // Recieve filename length
    uint16_t name_length;
    if (recv(socket, &name_length, sizeof(name_length), 0) <= 0) {
        std::cerr << "Failed receiving filename length: " << strerror(errno) << std::endl;
        return "";
    };

    // Recieve filename
    std::vector<char> filename_buffer(name_length);
    if (recv(socket, filename_buffer.data(), name_length, 0) <= 0) {   
        std::cerr << "Failed receiving filename: " << strerror(errno) << std::endl;
        return "";
    };
    
    std::string filename(filename_buffer.begin(), filename_buffer.end());

    // Recieve filesize
    uint64_t filesize;
    if (recv(socket, &filesize, sizeof(uint64_t), 0) <= 0) {
        std::cerr << "Failed receiving file size: " << strerror(errno) << std::endl;
        return "";
    };

    // File to save the data


    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open output file: " << strerror(errno) << std::endl;
        return "";
    };

    
    std::vector<char> buffer(BUFFER_SIZE);

    
    uint64_t bytes_received = 0;
    while (bytes_received < filesize) {
        size_t bytes_remaining = std::min((uint64_t)buffer.size(), filesize - bytes_received);

        ssize_t bytes_in_packet = recv(socket, buffer.data(), bytes_remaining, 0);

        if (bytes_in_packet <= 0) {
            std::cerr << "Failed to receive packet: " << strerror(errno) << std::endl;
            file.close();
            return "";
        }

        file.write(buffer.data(), bytes_in_packet);
        bytes_received += bytes_in_packet;
    };

    std::cout << "File received successfully" << std::endl;
    return output_dir;
};