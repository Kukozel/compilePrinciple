//
//  HandleFiles.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/5/21.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "HandleFiles.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string handleFile::readFileIntoString_forLexical(std::string filename)
{
    std::ifstream ifile(filename);
    std::ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch)){
        if(ch=='\n'||ch=='\t')
            continue;
        buf.put(ch);
    }
    return buf.str();
}

std::string handleFile::readFileIntoString_forSyntactic(string filename){
    std::ifstream ifile(filename);
    std::ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch)){
        if(ch=='\n'||ch=='\t'||ch==' ')
            continue;
        buf.put(ch);
    }
    return buf.str();
}



