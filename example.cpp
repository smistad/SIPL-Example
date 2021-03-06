#include "SIPL/Core.hpp"
#include "SIPL/Visualization.hpp"
using namespace SIPL;

int main(int argc, char ** argv) {
    // Load image from disk and display it
    Image<color_uchar> * im = new Image<color_uchar>("images/sunset.jpg");
    im->display();

    // Remove all the green from the color image im
    for(int i = 0; i < im->getTotalSize(); i++) {
        color_uchar p = im->get(i);
        p.green = 0;
        im->set(i, p);
    }
    im->display();

    // Save it
    im->save("test.png", "png");

    // Convert image to grayscale and display it
    Image<float> * im2 = new Image<float>(im, IntensityTransformation(NORMALIZED));

    // View the image using a custom level(0.5) and window(0.25)
    im2->display(0.5, 0.25);

    // Calculate the gradient of the image and display the vector field using colors
    Image<float2> * gradient = new Image<float2>(im2->getWidth(), im2->getHeight());
    Visualization * w = new Visualization(gradient);
    w->display();
    w->setWindow(0.4);
    w->setLevel(0.2);
    for(int x = 1; x < im2->getWidth()-1; x++) {
        for(int y = 1; y < im2->getHeight()-1; y++) {
            float2 vector;
            vector.x = 0.5*(im2->get(x+1,y)-im2->get(x-1,y));
            vector.y = 0.5*(im2->get(x,y+1)-im2->get(x,y-1));
            gradient->set(x,y,vector);
        }
        if(x % 20 == 0)
            w->update(); // update the image on screen
    }

    // Load volume and display one slice on the screen
    // (Use arrow keys up and down to change the slice on screen)
    Volume<uchar> * v = new Volume<uchar>("skull.raw", 256, 256, 256);
    v->setSpacing(float3(0.5,1.0,0.5));
    Visualization * vis2 = new Visualization(v);
    vis2->setType(MIP);
    vis2->setLevel(100);
    vis2->setWindow(200);
    vis2->setTitle("Head skull CT");
    vis2->display();
    v->display();
    v->display(100, X);
    v->display(100, X, 60, 100);
    v->display(60, 100);

    // Create and show maximum intensity projection (MIP) of the volume
    v->displayMIP();
    v->displayMIP(Y, 60, 100);

    // Convert volume to another data type
    Volume<float> * v2 = new Volume<float>(v, IntensityTransformation(NORMALIZED));

    // Calculate 3D gradient of the volume
    Volume<float3> * vGradient = new Volume<float3>(v->getWidth(), v->getHeight(), v->getDepth());
    for(int x = 1; x < v->getWidth()-1; x++) {
        for(int y = 1; y < v->getHeight()-1; y++) {
            for(int z = 1; z < v->getDepth()-1; z++) {
                float3 vector;
                vector.x = 0.5*(v2->get(x+1,y,z)-v2->get(x-1,y,z));
                vector.y = 0.5*(v2->get(x,y+1,z)-v2->get(x,y-1,z));
                vector.z = 0.5*(v2->get(x,y,z+1)-v2->get(x,y,z-1));
                vGradient->set(x,y,z,vector);
            }
        }
    }
    vGradient->display();
    vGradient->save("test.raw");
}
