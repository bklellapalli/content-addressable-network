/***************Server.cpp******************
 * FILE NAME: Fileprocessor.cpp
 *
 * Created by Balakrishna on 5/11/17
 * DESCRIPTION: Definition of all Fileprocessor related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "FileProcessor.hpp"
#include "boost/filesystem.hpp"
#include <iostream>
#include <iterator>

FileProcessor::FileProcessor(std::string ipAddress, std::string port)
{
    std::string url;
    mongoc_init ();
    iov.iov_base = (void *) buf;
    iov.iov_len = sizeof buf;
    
    /* connect to localhost client */
    url = "mongodb://" + ipAddress + ":" + port + "?appname=CAN-Protocol";
    
    client = mongoc_client_new (url.c_str());
    mongoc_client_set_error_api (client, 2);
    gridfs = mongoc_client_get_gridfs (client, "CAN", "fs", &error);
}

FileProcessor::~FileProcessor()
{
    //mongoc_gridfs_destroy (gridfs);
    //mongoc_client_destroy (client);
    //mongoc_cleanup ();
}

void FileProcessor::insertFiles()
{
   
    boost::filesystem::path p("Shared");
    std::vector<boost::filesystem::directory_entry> v; // To save the file names in a vector.
    
    if(boost::filesystem::is_directory(p))
    {
        std::copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(v));
        for ( std::vector<boost::filesystem::directory_entry>::const_iterator it = v.begin(); it != v.end();  ++ it )
        {
            std::cout<< (*it).path().filename().string()<< std::endl;
        }
    }
}


void FileProcessor::readFile(std::string fileName)
{
    file = mongoc_gridfs_find_one_by_filename (gridfs, fileName.c_str(), &error);
    stream = mongoc_stream_gridfs_new (file);
    for (;;)
    {
        r = mongoc_stream_readv (stream, &iov, 1, -1, 0);
        if (r == 0) { break; }
        
        FILE *fp;
        fp = fopen( fileName.c_str() , "w" );
        fwrite (iov.iov_base, 1, r, fp);
        fclose(fp);
    }
    mongoc_stream_destroy (stream);
    mongoc_gridfs_file_destroy (file);
}

void FileProcessor::writeFile(std::string fileName)
{
    stream = mongoc_stream_file_new_for_path (fileName.c_str(), O_RDONLY, 0);
    opt.filename = fileName.c_str();
    
    /* the driver generates a file_id for you */
    file = mongoc_gridfs_create_file_from_stream (gridfs, stream, &opt);
    
    mongoc_gridfs_file_save (file);
    mongoc_gridfs_file_destroy (file);
}
