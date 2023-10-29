   **1. Objectives**</br>
  Upon completion of this assignment, the student will be able to: </br>
• implement and use trees to solve problems; </br>
• understand the representation of an image; </br>
• implement standard image processing operations; </br>
• translate a real-life problem into a problem that utilizes quaternary trees. </br>
    
   **2. Description** </br>
  We live in a world of images; undoubtedly, we are consumers of images. Paradoxically, however, we are neither prepared nor taught to read images. When we are put in a situation to interpret them, we do so mostly intuitively because we don't have many references in this field, we don't know what we should see and how. Photography develops distinct ways of seeing the world. It can evoke or impose the representation that determines the recognition of the thing or being that has either disappeared or transformed after its fixation in black and white or color. Therefore, it represents a fractal fraction of reality.</br>
  This theme aims to introduce you to the vast world of images and proposes for analysis and implementation a method of image compression. The term "image compression" refers to a broad class of techniques and methods whose purpose is to represent a given image using as few bits as possible. With the evolution of technology and, implicitly, the increase in image quality, the need to reduce the amount of information necessary for representing an image has become evident.</br>
The process of reconstructing the initial image from the restricted representation is called decompression. It is evident that, through decoding, one must obtain an image as close as possible to the original image.</br>
   **3. Image Compression**</br>
  **3.1 Quadtree**</br>
  The quadtree is a data structure that organizes information for multidimensional data, used in cartography, image processing, computer graphics, and so on. The structure is a tree that represents an N-dimensional space (in this case, we will analyze the case N = 2), and each node of the tree holds information for an area in space. The node has 2^N children, each representing an area 2^N times smaller than the parent's area. The children's areas are disjoint, and their union forms the parent's area. In other words, the structure we will use in this theme is a tree in which each non-terminal node will have exactly 4 descendants.</br>
  For a quadtree, the root represents a square, and its descendants represent four disjoint and congruent squares in which the initial square can be divided. As presented so far, a quadtree is infinite, but we will use this data structure to represent an image with a reduced size in the plane. For this reason, we will use only a few levels of the quadtree. </br>
  **3.2 Compression Algorithm**</br>
  Any square image with a size that is a power of 2 can be represented by a quadtree. The nodes at each level of the tree correspond to a subdivision of a square area of the image into four quadrants. The root of the tree is associated with the entire image, the nodes at the first level of the tree correspond to the four quadrants of the image (the order being: top-left, top-right, bottom-right, bottom-left), the nodes at the second level correspond to the quadrants of each previous quadrant, and so on. The subdivision of the image can continue until the nodes at the current level of the tree correspond to uniform square areas: if the considered square region is not uniform, it will be decomposed, by cutting, into four equal parts, and the corresponding node will become non-terminal, having four descendants. If the considered region is uniform (composed of pixels of the same type), the respective node becomes a leaf (terminal) node of the tree. Each leaf node contains information corresponding to the value of the image area it is associated with.</br>
  For a better understanding of the algorithm described in the previous paragraph, it is proposed to analyze the example below.</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/10f93394-946e-468a-947a-146ee7ab6a8d)</br>

   **4 Image Decompression**</br>
  To reconstruct the original image from its tree representation, it is sufficient to select the terminal nodes whose values correspond to the object's pixels. The depth at which a leaf is placed in the tree contains information about the size of the corresponding square area in the image. The position of the leaf relative to the nodes at the same level that have the same predecessor is directly determined by the rule of allocating the quadrants of an area to the descendant nodes of the tree (this allocation rule must be maintained for the entire tree).</br>

   **5 File Formats**</br>
  In this theme, we will use color images. There are several available formats for representing color images. Regardless of the format, for a color image, we will have a stored pixel matrix in which each pixel of the image is composed of a triplet of numerical values, called color channels: red, green, and blue (RGB). These values are used to describe the amount of specific color for each pixel. Each channel can have a value ranging from 0 to 255, where 0 represents the absence of the color, and 255 represents the maximum amount of color possible in a particular channel. By combining the values from the three channels, a wide range of colors is obtained, allowing for a wide variety of color images.</br>

For simplicity, we have chosen to use the PPM format within this theme.</br>

   **5.1 PPM File**</br>
 A file in PPM format contains a header in text format that includes: on the first line, the type of the file (in the case of the images used in the test, it will be type P6); on the second line, two natural numbers (width and height), separated by a space, which describe the dimensions of the image; on the third line, there is a natural number representing the maximum value that a color can take (in the case of the tests used, the value is 255); and finally, the actual image in binary format.</br>

   **5.1.1 The Actual Image**</br>
 This section is represented by a pixel matrix that occupies the largest part of the file. The number of elements in the array is equal to the product of the number of pixels per line (width) and the number of pixels per column (height), with the array organized in rows and columns, starting from the top line of the image, leftmost pixel.</br>

 **5.2 Compressed File**</br>
 The compressed file will contain the information resulting from the compression process:</br>

-image_size - of type unsigned int - specifying the size of the image (as a reminder, we are dealing with square images).</br>
  For each node in the level traversal applied to the compression tree, we will write the following information to the compressed file:</br>
If the node is an internal node:</br>
-node_type - of type unsigned char - which will have the value 0 in this case.</br>
If the node is a leaf node:</br>
-node_type - of type unsigned char - which will have the value 1 in this case.</br>
-value_for_red - of type unsigned char - indicating the value of the component responsible for the Red color for the pixels in the area described by that node.</br>
-value_for_green - of type unsigned char - indicating the value of the component responsible for the Green color for the pixels in the area described by that node.</br>
-value_for_blue - of type unsigned char - indicating the value of the component responsible for the Blue color for the pixels in the area described by that node.</br>
To better understand how the descendants of a node will appear, we propose the following graphical representation of such a subdivision in which the 4 child nodes are clearly depicted.</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/4a09eae2-dbb6-4222-a5b3-27b8897e453c)</br>

   **6 Requirements**</br>
  **6.1 Requirement 1**</br>
To begin with, we will construct the compression tree and determine some statistics based on it. Thus, we will first read the image from the PPM file and then build the compression tree for it.</br>
To determine when a block has reached a point where it can be represented in the quadtree compression as a leaf node, in other words, it does not need to be further divided into four equal-sized zones, we will calculate the average color of the block. This involves finding the arithmetic mean for each channel (RED, GREEN, and BLUE) of the values in the pixel submatrix corresponding to the block. We will consider this submatrix to start with the element at coordinates (x, y), where x represents the index of the row, and y represents the index of the column. Furthermore, any analyzed submatrix must be square, and we will consider it to have a size of size × size.</br>

To calculate the arithmetic mean of the values in the pixel submatrix corresponding to a specific block, we can use the following formulas:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/95319705-04e8-40a2-9c6b-ef714ba7b652)</br>
After determining the average color, a similarity score for the respective block is calculated using the following formula:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/c9c6faa9-ad1c-430f-97a2-5d20f15f7fbe)</br>
where red, green, blue represent the components for the average color. If the obtained score is less than or equal to the imposed threshold, then there will be no need for further division.</br>

After constructing the compression tree, we want to determine the following information:</br>

-The number of levels in the quadtree.</br>
-The number of blocks in the image for which the pixel similarity score is less than or equal to the provided factor.</br>
-The size of the side of the square for the largest area in the image that remains undivided.</br>

   **6.2 Requirement 2**</br>
For the second requirement of the assignment, you need to compress an image in PPM format using the compression algorithm detailed in the previous section of the statement. In solving this requirement, it is mandatory to implement the quadtree compression. You will use the method described in the previous requirement to construct the tree.</br>

Once this compression tree is built, it will be traversed at each level, and the compression file will be generated according to the format described in Section 5.2.</br>

   **6.3 Requirement 3**</br>
In this requirement, you need to reconstruct the initial image from a file resulting from the compression using the described decompression algorithm. Additionally, it is mandatory to use the quadtree structure to solve this requirement.</br>

In this case, you will have to reconstruct the compression tree from the available traversal in the compressed file and generate the image based on the tree. The binary file from which you start will have the format described in Section 5.2.</br>

   **7 Example**</br>
For a better understanding of the requirements of this assignment, consider the following example created for the image below. The images you will work with in this assignment will not have separate areas (the white contours that appear in the image below for each square), as shown in the example. Initially, you will need to read the image and build the quadtree compression that models it, which will be the structure you work with in the following requirements.</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/02a72440-1f6c-4e88-811a-313b423c9dc3)</br>

   **7.1 Requirement 1**</br>
Let's assume that each square in the previous image has dimensions of 4 × 4. The number of levels in the compression tree is 4, the number of blocks in the image for which the pixel similarity score is less than or equal to the provided factor is 16, and the square describing the largest undivided area in the image has a side length of 16.</br>

The content of the file "quadtree.out" is as follows:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/5642b3fe-2438-415d-8b8f-3eef4bcc4107)</br>

   **7.2 Requirement 2**</br>
After reading the image and constructing the quadtree, we can perform the compression of the image. For this, we will apply the level-order traversal algorithm for the quadtree.</br>

The order in which the nodes appear for the respective tree in the result of the level-order traversal is as follows:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/120c073b-1908-4e37-a3c0-d7c3c4d3fb5f)</br>

For the tree above, we present the values we will write to the binary file for each level:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/a41e26d2-f180-49a0-a32b-3565ec97ead9)</br>

In this representation, we have specified the level index at the beginning and then the values. To make it easier to follow, we have delimited the four values for leaf nodes (node_type, red, green, blue) with { and }.</br>

Important Note:</br>
In the prompt, we provided an example of values in a formatted text format for ease of reading. In your implementation, you must adhere to the specifications in Section 5.2 and keep in mind that you are working with a binary file. Therefore, you will only write the relevant values, considering the specified data types, without any spaces or additional processing.</br>

Important Note:</br>
The file resulting from the compression must be a binary file. Therefore, the information will be written to the file using the fwrite function, considering the data types indicated in Section 5.2.</br>

Starting from the following image, we will see how the provided threshold affects the quadtree and, implicitly, the compression operation.</br>

![image](https://github.com/victorandrei03/quad-tree/assets/117905946/6deb1ffb-c328-46be-801f-ff5321478842)</br>

If the provided threshold is 0, then the compression tree will be as follows:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/1f0e7977-84a9-4e52-bfb4-2ac8376dad48)</br>

For this image, the image after decompression will be:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/e1931ace-32b8-4c50-8a85-ef3953d2174a)</br>

If the provided threshold is greater, then the compression tree might look like the one below. Each leaf node will contain the average color of the block.</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/6396bbb0-e764-4b1a-a527-361baddb0a6b)</br>

For this image, the image after decompression will be:</br>
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/cea83690-8297-41a8-8e81-41db801dc0d8)</br>

   **7.3 Requirement 3**</br>
To fulfill this requirement, you will need to read the binary file representing the compression of an image. This file contains the information necessary for constructing the associated quadtree of the image. As you read the contents of the file, you can create a quadtree. With this structure modeling an image, you simply need to form the image based on the quadtree.</br>

Important Note:</br>
You must adhere to the structure of a PPM file, as described in section 5.1.</br>

   **8. Clarifications**</br>
Your program will receive, as command line arguments, the names of the input and output files, along with an option in the following way:</br>
./quadtree [-c1 factor | -c2 factor | -d] [input_file] [output_file]</br>
where:</br>

-c1 factor indicates that the program will solve requirement 1 (factor is the threshold set for the compression tree).</br>
-c2 factor indicates that the program will solve requirement 2 (factor is the threshold set for the compression tree).</br>
-d indicates that the program will solve requirement 3.</br>
input_file represents the name of the first input file (which contains the image).</br>
output_file represents the name of the output file where the result of the program's execution will be written, depending on the received command.</br>

















