#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <ctype.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#define cimg_use_png
#include "CImg.h"
#include "Network.h"

using namespace std;
using namespace cimg_library;

int myrandom (int i) { return std::rand()%i;}

inline bool exists_test (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    string file;
    cout << "File name: ";
    cin >> file;
    if (file == "0")
        file = "input.png";
    else file += ".png";
    int bsz, outsz, d = 1;
    cout << "Block size: ";
    cin >> bsz;
    cout << "Output layer size: ";
    cin >> outsz;
    float mu, beta;
    cout << "mu: ";
    cin >> mu;
    cout << "beta: ";
    cin >> beta;
    int iter;
    cout << "Iterations: ";
    cin >> iter;
    cout << "Approximate output? (y/n): ";
    string approx;
    cin >> approx;
    bool approximate = tolower(approx[0]) == 'y';
    random_device rd;
    mt19937 rng(rd());
    CImg<float> input;
    input.load(file.c_str());
    int h = input.height();
    int w = input.width();

    network net(bsz*bsz,outsz,&rng,mu,beta,0);
    stringstream stream;
    stream << fixed << setprecision(2) << mu << ',' << beta;
    string mubeta = stream.str();
    string network = to_string(bsz)+"x"+to_string(bsz)+","+to_string(outsz)+","+mubeta;

    if (exists_test(network)) {
        cout << "Detected saved network" << endl;
        net.parse(network);
    }

    vector<vector<float>> pixels;
    float mx = 0;
    for (int i = 0; i < h; i++) {
        vector<float> line;
        for (int j = 0; j < w; j++) {
            mx = max(mx, input(j,i,0,0));
            line.push_back(input(j,i,0,0));}
        pixels.push_back(line);
    }
    if (mx > 255) d = 256;
    vector<vector<float>> inputs;
    for (int i = 0; i < h; i += bsz) {
        if (i+bsz > h) i = h-bsz;
        vector<float> E;
        for (int j = 0; j < w; j += bsz) {
            if (j+bsz > w) j = w-bsz;
            for (int f = 0; f < bsz; f++)
                for (int g = 0; g < bsz; g++)
                    E.push_back(pixels[i+f][j+g]/d);
            inputs.push_back(E);
            E.clear();
        }
    }
    auto svin = inputs;
    int c=1,tot=iter;
    time_t start;
    start = time(NULL);
    while (iter--) {
        _Random_shuffle1(inputs.begin(),inputs.end(),myrandom);
        for (auto E : inputs) {
            net.set_input(E);
            net.train();
        }
        time_t cur = time(NULL);
        cout << c++ << '/' << tot << ", time elapsed: " << difftime(cur,start) << 's' << endl;
        if (iter%3 == 0) {
            net.save(network);
            cout << "Network saved." << endl;
        }
    }
    CImg<float> img(w,h);
    int ii = 0;
    float error = 0;
    for (int i = 0; i < h; i += bsz) {
        if (i+bsz > h) i = h-bsz;
        for (int j = 0; j < w; j += bsz) {
            if (j+bsz > w) j = w-bsz;
            net.set_input(svin[ii++]);
            if (approximate) net.compute_output_approx();
            else net.compute_output();
            error += net.err();
            int idi = net.idi;
            int idj = net.idj;
            for (int f = 0; f < bsz; f++)
                for (int g = 0; g < bsz; g++)
                    img(j+g,i+f) = net.X[idi][idj].w[f*bsz+g];
        }
    }
    error /= (ii-1)*bsz*bsz;
    cout << error << endl;
    input.display();
    img.display();
    system("pause");
    return 0;
}
