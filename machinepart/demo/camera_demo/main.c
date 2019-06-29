#include "camera.h"
#include "log.h"

#include "assert.h"

int test_camera_ctx() {
    struct camera_ctx cam;
    get_frame_from_camera(&cam);
    return 0;
}

int main(int argc, char** argv){
    test_camera_ctx();
    return 0;
}
