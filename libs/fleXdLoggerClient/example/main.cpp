/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dev
 *
 * Created on April 13, 2018, 2:02 PM
 */

#include <cstdlib>
#include <FleXdLogger.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    FLEX_LOG_NAME_INIT("Application");
    FLEX_LOG_DEBUG("Toto je ALL log");
    
    return 0;
}

