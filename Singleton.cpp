//
//  Singleton.cpp
//  OpenGL
//
//  Created by 0 on 04/02/2020.
//  Copyright © 2020 0. All rights reserved.
//
#pragma once

template<typename T>
class Singleton {
    protected :
    Singleton() {};
    
    public :
    static T *GetInstance() {
        
        if(instance == nullptr)
            instance = new T;
        return instance;
    };
    
    private :
    static T *instance;
    
};

template<typename T>
T * Singleton<T>::instance = nullptr;
