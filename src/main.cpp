#ifndef NO_IMAGEMAGICK
#include <ImageMagick-7/Magick++.h>
#endif

#include <optional>
#include "Scene.h"

using namespace Magick;
using namespace std;

int main(int argc, char **argv) {
    optional<Scene> scene;
#ifndef NO_IMAGEMAGICK
    string outpath = "out.png";
#else
    string outpath = "out.ppm";
#endif

    if (argc > 1)
        scene = Scene(argv[1]);
    else
        scene = Scene();

    if (argc > 2)
        outpath = argv[2];
    else if (argc > 1)
#ifndef NO_IMAGEMAGICK
        outpath.replace(outpath.rfind('.')+1, 3, "png");
#else
        outpath.replace(outpath.rfind('.')+1, 3, "ppm");
#endif

    scene.value().render(outpath);

#ifndef NO_IMAGEMAGICK
    Image image;
    image.magick("PPM");
    image.read(outpath);
    image.magick("PNG");
    image.write(outpath);
#endif

    return 0;
}