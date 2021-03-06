//
// Created by bknun on 1/8/2017.
//
#include "file.h"
#include<stdio.h>
#include <fstream>

bool file::exists(const char *file)
{
    std::ifstream infile(file);
    return infile.good();
}

bool is_whitespace(char c)
{
    return ((c ==' ') || (c =='\n') ||
            (c =='\r') || (c =='\t') ||
            (c =='\b') || (c =='\v') ||
            (c =='\f')) ;
}

bool file::empty(const char *file)
{
    stream data;
    read_alltext(file, data);
    for(long i = 0; i < data.size(); i++)
    {
        if(!is_whitespace(data.at(i)))
            return false;
    }

    return true;
}

int file::write(const char *f, stream& data)
{
    try {
        FILE* fp=NULL;

        remove( f );
        fp = fopen(f,"wb+");
        if(fp == 0)
            return 1;  // could not open file

        unsigned int p=0;
        do {
            fputc( data.at(p++), fp );
        }while(p<data.size());

        fclose(fp);
        return 0;
    }
    catch(std::bad_alloc& ba){
        return -1;
    }
}

int64_t file_size(FILE *fp)
{
    int64_t len, cur;
    cur = ftell( fp );            /* remember where we are */
    fseek( fp, 0L, SEEK_END );    /* move to the end */
    len = ftell( fp );            /* get position there */
    fseek( fp, cur, SEEK_SET );   /* return back to original position */

    return ( len );
}

void file::read_alltext(const char *f, stream& _out)
{
    if(!exists(f))
        return;

    try {

        FILE* fp=NULL;
        int64_t len;

        fp = fopen(f,"rb");
        if(fp == 0)
            return;  // could not open file


        len = file_size(fp);
        if(len == -1) 1;
        char c;

        do {
            c = getc(fp);
            if(c==EOF) {
                break;
            } else {
                _out << c;
            }
        }while(len--);
        fclose(fp);
    }
    catch(std::bad_alloc& ba){
        return;
    }
    catch(std::exception& e){
        return;
    }

}

bool file::endswith(string ext, string file) {
    size_t ext_t = ext.length();
    stringstream extension;
    for(size_t i = file.length() - ext_t; i < file.length(); i++){
        extension << file.at(i);
    }

    return extension.str() == ext;
}

void file::stream::_push_back(char _C) {
    if(_Data == NULL)
        begin();

    if(sp>=_ds) {
        _ds+=STREAM_CHUNK;
        void* tptr=realloc(_Data, _ds*sizeof(char));
        if(tptr == NULL) {
            throw std::bad_alloc();
        }
        _Data=(char* )tptr;
    }

    _Data[sp++]=_C;
}

char file::stream::at(stream_t _X) {
    if(_X>=sp ||_X<0) {
        stringstream _s;
        _s << "stream::at() _X: " << _X << " >= size: " << _ds;
        throw std::out_of_range(_s.str());
    }
    return _Data[_X];
}

string file::stream::to_str() {
    string s ="";
    for(unsigned long i=0; i < size(); i++) {
        s+=_Data[i];
    }
    return s;
}

int file::write(const char *f, string data)
{
    try {
        FILE* fp=NULL;

        remove( f );
        fp = fopen(f,"wb+");
        if(fp == 0)
            return 1;  // could not open file

        unsigned int p=0;
        do {
            fputc( data.at(p++), fp );
        }while(p<data.size());

        fclose(fp);
        return 0;
    }
    catch(std::bad_alloc& ba){
        return -1;
    }
}
