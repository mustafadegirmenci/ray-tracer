typedef struct {
    unsigned char *image;
    int width;
    int height;
} RenderResult;

typedef unsigned char RGB[3];

RenderResult render();
