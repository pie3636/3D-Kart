#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "../include/project_scene.hpp"

int main(int argc, char* argv[]) {
    Viewer viewer(1280, 720);

    initialize_scene(viewer);

    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}
