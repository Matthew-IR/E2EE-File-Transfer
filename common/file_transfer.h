#ifndef FILE_TRANSFER
#define FILE_TRANSFER

#include <string>

struct file_header {
    std::string filename;
    uint64_t file_size;
};

/** 
 * @brief Sends a file over a TCP socket
 * @param socket The connected socket
 * @param filepath The path to the file that is being sent
 * @return 0 on success, -1 on failure
*/
int send_file(int socket, const std::string& filepath);

/** 
 * @brief Receives a file over a TCP socket
 * @param socket The connected socket
 * @param output_dir The directory where the received file will be saved.
 * @return The path of the saved file on success, empty string on failure
*/
std::string receive_file(int socket, const std::string& output_dir);

#endif