#ifndef NETWORK_H
#define NETWORK_H
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <io.h>
#include <random>
using namespace std;

struct Node{
    vector<float> w;
    Node(int sz, mt19937 *rng) {
        w = vector<float>(sz);
        uniform_real_distribution<float> distrib(0,255);
        for (int i = 0; i < sz; i++)
            w[i] = distrib(*rng);
    }
};

struct network{
    mt19937 *gen;
    float mu, beta;
    int counter;
    int n, p, idi, idj;
    vector<float> E;
    vector<vector<Node>> X;
    network(){}
    network(int nn, int pp, mt19937 *rng, float mmu, float mbeta, int cnt) { // O(np^2)
        idi = idj = -1;
        counter = cnt;
        E = vector<float>(nn);
        mu = mmu; beta = mbeta;
        p = pp; n = nn;
        gen = rng;
        for (int i = 0; i < p; i++) {
            vector<Node> temp;
            for (int j = 0; j < p; j++) {
                Node a = Node(n, rng);
                temp.emplace_back(a);
            }
            X.emplace_back(temp);
        }
    }
    void set_input(vector<float> &pixels) { // O(n)
        for (int i = 0; i < n; i++)
                E[i] = pixels[i];
    }
    float dist(int i, int j) { // O(n)
        float dis = 0;
        int l = 0;
        for (float k : X[i][j].w) {
            dis += (k-E[l])*(k-E[l]);
            ++l;
        }
        return dis;
    }
    void distc(int i, int j) { // O(n)
        float dis = 0;
        int l = 0;
        for (float k : X[i][j].w)
            dis += (k-E[l])*(k-E[l]);
            ++l;
    }
    void update_weights(int i, int j) { // O(n)
        for (int x = 0; x < n; x++)
            X[i][j].w[x] += mu*(E[x]-X[i][j].w[x]);
        for (int f = -1; f < 2; f++)
            for (int g = -1; g < 2; g++) {
                if (abs(g)+abs(f) != 1 || max(i+f,j+g) >= p || min(i+f,j+g) < 0) continue;
                for (int x = 0; x < n; x++)
                    X[i+f][j+g].w[x] += beta*(E[x]-X[i+f][j+g].w[x]);
            }
    }
    void train() { // O(np^2)
        float mini = 1e18;
        idi = -1; idj = -1;
        for (int i = 0; i < p; i++)
            for (int j = 0; j < p; j++) {
                float dis = dist(i,j);
                if (mini > dis) {
                    mini = dis;
                    idi = i;
                    idj = j;
                }
            }
        mu = max(mu*exp(-0.00005*counter),0.01);
        beta = max(beta*exp(-0.00005*counter),0.01);
        if (counter % 100 == 0)
            mu = 0.5, beta = 0.5;
        update_weights(idi, idj);
        counter++;
    }
    void compute_output() { // O(np^2)
        float mini = 1e18;
        idi = -1; idj = -1;
        for (int i = 0; i < p; i++)
            for (int j = 0; j < p; j++) {
                float dis = dist(i,j);
                if (mini > dis) {
                    mini = dis;
                    idi = i;
                    idj = j;
                }
            }
    }
    void compute_output_approx() { // O(np)
        float mini = 1e18;
        idi = -1; idj = -1;
        int cn = 0;
        uniform_int_distribution<> distrib(0,p-1);
        while (1) {
            ++cn;
            int i = distrib(*gen);
            int j = distrib(*gen);
            float dis = dist(i,j);
            if (mini > dis) {
                mini = dis;
                idi = i;
                idj = j;
            }
            if (mini <= n*n || cn > p*5)
                break;
        }
    }
    float err() { // O(n)
        float ans = 0;
        for (int i = 0; i < n; i++)
            ans += sqrt((X[idi][idj].w[i]-E[i])*(X[idi][idj].w[i]-E[i]));
        return ans;
    }
    void parse(string name) {
        freopen(name.c_str(),"r",stdin);
        cin >> counter;
        for (int i = 0; i < p; i++)
            for (int j = 0; j < p; j++)
                for (int k = 0; k < n; k++)
                    cin >> X[i][j].w[k];
        fclose(stdin);
    }
    void save(string name) {
        int stdout_dupfd;
        FILE *temp_out;
        stdout_dupfd = _dup(1);
        temp_out = fopen(name.c_str(),"w");
        _dup2(_fileno(temp_out), 1);
        cout << counter << endl;
        for (int i = 0; i < p; i++)
            for (int j = 0; j < p; j++) {
                for (int k = 0; k < n; k++)
                    cout << X[i][j].w[k] << ' ';
                cout << endl;
             }
        fclose(temp_out);
        _dup2(stdout_dupfd, 1);
        _close(stdout_dupfd);
    }
};


#endif // NETWORK_H
