#ifndef Fileprocessor_hpp
#define Fileprocessor_hpp

#include <string>
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

class FileProcessor
{
    private:
        mongoc_gridfs_t *gridfs;
        mongoc_gridfs_file_t *file;
        mongoc_gridfs_file_opt_t opt = {0};
        mongoc_client_t *client;
        mongoc_stream_t *stream;
        bson_error_t error;
        mongoc_iovec_t iov;
    
        char buf[BUFFER_SIZE];
        ssize_t r;
    
public:
        FileProcessor(std::string ipAddress, std::string port);
        ~FileProcessor();
        void insertFiles();
        void readFile(std::string fileName);
        void writeFile(std::string fileName);
};

#endif /* Fileprocessor_hpp */
