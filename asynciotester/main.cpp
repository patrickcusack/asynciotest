//
//  main.cpp
//  asynciotester
//
//  Created by Patrick Cusack on 7/22/19.
//  Copyright © 2019 Patrick Cusack. All rights reserved.
//

#include <sys/types.h>
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>

using namespace std;

const int SIZE_TO_READ = 1024 * 1024;

int main(int argc, const char * argv[])
{
    // open the file
    int file = open(argv[1], O_RDONLY, 0);
    
    if (file == -1)
    {
        cout << "Can't open the files" << endl;
        return 1;
    }
    
    // make a buffer to store everything
    char* buffer = new char[SIZE_TO_READ];
    
    // create the control block structure
    aiocb cb;
    memset(&cb, 0, sizeof(aiocb));
    cb.aio_nbytes = SIZE_TO_READ;
    cb.aio_fildes = file;
    cb.aio_offset = 0;
    cb.aio_buf = buffer;
    
    // attempt to read the file
    if (aio_read(&cb) == -1)
    {
        cout << "Unable to create request!" << endl;
        close(file);
    }
    
    //cout << "Request enqueued!" << endl;
    
    // check status
    long counter = 0;
    while(aio_error(&cb) == EINPROGRESS)
    {
//        cout << "Working..." << endl;
        counter++;
    }
    
    if(aio_error(&cb) == ETIMEDOUT){
        cout << "Timeout error!!!!!" << endl;
    }
    
    // if finished try and see if we read what we needed to read
    long numBytes = aio_return(&cb);
    if (numBytes != -1)
        cout << "SUCCESS" << "\t" << counter << endl;
    else
        cout << "ERROR" << "\t" << counter << endl;
    
    // finish everything
    delete[] buffer;
    close(file);
    
    return 0;
}
