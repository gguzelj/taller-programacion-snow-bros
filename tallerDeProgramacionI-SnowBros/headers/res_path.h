#ifndef RES_PATH_H_
#define RES_PATH_H_

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

/*
 * Get the resource path for resources located in resources
 * It's assumed the project directory is structured like:
 * headers/
 *  ... .h
 * resources/
 *  chapelco.jpeg
 *
 * Paths returned will be .../res/chapelco.jpeg
 */
std::string getResourcePath(const std::string &filename = ""){
        //We need to choose the path separator properly based on which
        //platform we're running on, since Windows uses a different
        //separator than most systems
#ifdef _WIN32
        const char PATH_SEP = '\\';
#else
        const char PATH_SEP = '/';
#endif
        //This will hold the base resource path: .../project/resources/
        std::string baseRes;
        if (baseRes.empty()){
                //SDL_GetBasePath will return NULL if something went wrong in retrieving the path
                char *basePath = SDL_GetBasePath();
                if (basePath){
                        baseRes = basePath;
                        SDL_free(basePath);
                }
                else {
                        std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
                        return "";
                }
                //We replace the folder where the binaries are allocated with resources/ to get the the resource path
                size_t pos = baseRes.rfind("Debug");
                baseRes = baseRes.substr(0, pos+5) + PATH_SEP + "resources" + PATH_SEP;
        }
        //If we want a specific subdirectory path in the resource directory
        //append it to the base path. This would be something like project/resources/images/
        return filename.empty() ? baseRes : baseRes + filename;
}

#endif /* RES_PATH_H_ */
