//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <cstring>
#include "Image.h"
#include "cmath"


bool Image::load(string filename) {
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw ("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw ("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }

    return true;
}

bool Image::loadRaw(string filename) {
    ifstream in(filename);
    if (in.good()) {
        in >> w;
        in >> h;

        for (int i = 0; i < w * h; i++) {
            float r, g, b;
            in >> r >> g >> b;
            this->pixels[i].r = (unsigned char) (std::max(0.f, min(255.f, powf(r, 1 / 2.2) * 255 + 0.5f)));
            this->pixels[i].g = (unsigned char) (std::max(0.f, min(255.f, powf(g, 1 / 2.2) * 255 + 0.5f)));
            this->pixels[i].b = (unsigned char) (std::max(0.f, min(255.f, powf(b, 1 / 2.2) * 255 + 0.5f)));
        }
        in.close();
        return true;
    }
    return false;
}

bool Image::savePPM(string filename) {
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary);

    if (ofs.fail()) { return false; }

    ofs << "P6\n" << w << " " << h << "\n255\n";

    unsigned char r, g, b;

    for (int i = 0; i < w * h; ++i) {
        r = pixels[i].r;
        g = pixels[i].g;
        b = pixels[i].b;

        ofs << r << g << b;
    }

    ofs.close();

    return true;
}



void Image::filterRed() {

    for (int i = 0; i < w*h; ++i) {
        pixels[i].b=0;
        pixels[i].g=0;
    }
}

void Image::filterGreen() {

    for (int i = 0; i < w*h; ++i) {
        pixels[i].b=0;
        pixels[i].r=0;
    }

}

void Image::filterBlue() {

    for (int i = 0; i < w*h; ++i) {
        pixels[i].r=0;
        pixels[i].g=0;
    }

}

void Image::greyScale() {
    for (int i = 0; i < w * h; i++) {
        int greyscale = (0.2126 * pixels[i].r + 0.7152 * pixels[i].g + 0.0722 * pixels[i].b);

        pixels[i].b = greyscale;
        pixels[i].g = greyscale;
        pixels[i].r = greyscale;
    }
}

void Image::flipHorizontal() {

    int px1;
    int px2;
    int tempPixel[3];

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w / 2; ++x) {

            px1 = x + y * w;
            px2 = (w - 1 - x) + y * w;

            tempPixel[0] = pixels[px1].r;
            tempPixel[1] = pixels[px1].g;
            tempPixel[2] = pixels[px1].b;

            pixels[px1].r = pixels[px2].r;
            pixels[px1].g = pixels[px2].g;
            pixels[px1].b = pixels[px2].b;

            pixels[px2].r = tempPixel[0];
            pixels[px2].g = tempPixel[1];
            pixels[px2].b = tempPixel[2];

        }
    }

}

void Image::flipVertically() {

    int px1;
    int px2;
    int tempPixel[3];

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h / 2; ++y) {

            px1 = x + y * w;
            px2 = x + (h - 1 - y) * w;

            tempPixel[0] = pixels[px1].r;
            tempPixel[1] = pixels[px1].g;
            tempPixel[2] = pixels[px1].b;

            pixels[px1].r = pixels[px2].r;
            pixels[px1].g = pixels[px2].g;
            pixels[px1].b = pixels[px2].b;

            pixels[px2].r = tempPixel[0];
            pixels[px2].g = tempPixel[1];
            pixels[px2].b = tempPixel[2];

        }
    }
}

void Image::AdditionalFunction2() {

    //Change brightness
    for (int i = 0; i < w*h; ++i) {
        pixels[i].r = pixels[i].r * 0.5  ;
        pixels[i].g = pixels[i].g * 0.5 ;
        pixels[i].b = pixels[i].b * 0.5;
    }

}

void Image::AdditionalFunction3() {

    //Change saturation
    for (int i = 0; i < w * h; i++) {
        double saturation = 0.5;

        double p = sqrt((pixels[i].r) * (pixels[i].r) * .299 +
                        (pixels[i].g) * (pixels[i].g) * .587 +
                        (pixels[i].b) * (pixels[i].b) * .114);

        pixels[i].b = p + ((pixels[i].b) - p) * saturation;
        pixels[i].g = p + ((pixels[i].g) - p) * saturation;
        pixels[i].r = p + ((pixels[i].r) - p) * saturation;
    }

}

void Image::AdditionalFunction1() {

    //Invert pixels

    for (int i = 0; i < w*h; ++i) {
        pixels[i].r = 255 - pixels[i].r;
        pixels[i].g = 255 - pixels[i].g;
        pixels[i].b = 255 - pixels[i].b;
    }

}


void Image::OtherAdvancedFeature() {
    //mean filter
    for(int x = 0; x < h; ++x){
        for(int y = 0; y < w; ++y){

            int red =0 ,green = 0,blue = 0;

            for(int i = -1; i <= 1; ++i){
                for(int j = -1; j <= 1; ++j){
                    if(x + i >= 0 && x + i < h && y + j >= 0 && y + j < w){
                        red += pixels[(x + i) * w + (y + j)].r;
                        green += pixels[(x + i) * w + (y + j)].g;
                        blue += pixels[(x + i) * w + (y + j)].b;
                    }
                }
            }
            pixels[x * w + y].r = red /9;
            pixels[x * w + y].g = green/9;
            pixels[x * w + y].b = blue/9;
        }
    }
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth() {
    return w;
}

int Image::getHeight() {
    return h;
}

Rgb *Image::getImage() {
    return pixels;
}