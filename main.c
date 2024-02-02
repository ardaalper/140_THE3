#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "the3.h"


int main(int argc, char** argv)
{
    Cache* cache;
    Media media1;
    Media video1;
    /* Create the cache */
    cache = createCache();
    printCache(cache);
    /* Add new media */
    media1.name = "newVideo";
    media1.size = 300;
    
    video1.name = "video1";
    video1.size = 200;

    addMediaRequest(cache, video1, "tr");
    printCache(cache);
    
    deleteMedia(cache, "video2");

    
    
    return 0;
}