# Image compression using Self Organizing Maps
 The goal is to optimize image compression in terms of memory using neural networks and more specifically using self organizing maps.
 
 The current build gives promising results for medical images after just a few minutes of training.
 
## How to compile
 The source requires libpng headers: [libpng](http://www.libpng.org/pub/png/libpng.html)
 
 The project was made using Qt Creator.
 
 Use the MSVC 2019 C++ compiler as libpng requires it.
 
## How to use
 The compiled program will take 7 lines of input:
 
	* The file name (e.g. input if the file's name is input.png, note that it will only take png files)
	* The block size i.e. k if you want the block of input to be a kxk square (the input layer will be of size k*k)
	* The output layer size p (the number of neurons of the output layer will be p*p)
	* mu and beta: 2 parameters for the training algorithm (should be in the range (0,1])
	* The number of training iterations
	* Whether or not you want to use approximation methods for computing the output (faster but suboptimal in terms of quality) 
 The program will automatically save the network locally.
 
 You can find example outputs for different parameters.

 ## Benchmarks
 
 | Comparison between JPEG and SOM | Error rate of images produced by the SOM depending on the dimensions of the network |
| ------| ------ |
| <img src="https://github.com/Froopie/SOM_image_compression/blob/master/plots/plot_target_square_grid.png" alt="drawing" width="600"/> | <img src="https://github.com/Froopie/SOM_image_compression/blob/master/plots/plot_3d_non_approx_viridis.png" alt="drawing" width="500"/> | 

| Comparison between JPEG and SOM (approximated) | Error rate of images produced by the SOM depending on the dimensions of the network (approximated) |
| ------| ------ |
| <img src="https://github.com/Froopie/SOM_image_compression/blob/master/plots/plot_target_square_approx_grid.png" alt="drawing" width="710"/> | <img src="https://github.com/Froopie/SOM_image_compression/blob/master/plots/plot_3d_approx_viridis.png" alt="drawing" width="500"/> | 

#### Computing time
p | Without approximation | With approximation
--- | --- | --- 
2 | 3 s | < 1 s 
10 | 6 s | 1 s 
20 | 24 s | 3 s 
30 | 45 s | 5 s 
49 | 120 s | 5 s 
64 | 206 s | 6 s 
