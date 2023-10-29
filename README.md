1 Objectives
Upon completion of this assignment, the student will be able to:
• implement and use trees to solve problems;
• understand the representation of an image;
• implement standard image processing operations;
• translate a real-life problem into a problem that utilizes quaternary trees.

2 Description
We live in a world of images; undoubtedly, we are consumers of images. Paradoxically, however, we are neither prepared nor taught to read images. When we are put in a situation to interpret them, we do so mostly intuitively because we don't have many references in this field, we don't know what we should see and how. Photography develops distinct ways of seeing the world. It can evoke or impose the representation that determines the recognition of the thing or being that has either disappeared or transformed after its fixation in black and white or color. Therefore, it represents a fractal fraction of reality.
This theme aims to introduce you to the vast world of images and proposes for analysis and implementation a method of image compression. The term "image compression" refers to a broad class of techniques and methods whose purpose is to represent a given image using as few bits as possible. With the evolution of technology and, implicitly, the increase in image quality, the need to reduce the amount of information necessary for representing an image has become evident.
The process of reconstructing the initial image from the restricted representation is called decompression. It is evident that, through decoding, one must obtain an image as close as possible to the original image.

3 Image Compression
3.1 Quadtree
The quadtree is a data structure that organizes information for multidimensional data, used in cartography, image processing, computer graphics, and so on. The structure is a tree that represents an N-dimensional space (in this case, we will analyze the case N = 2), and each node of the tree holds information for an area in space. The node has 2^N children, each representing an area 2^N times smaller than the parent's area. The children's areas are disjoint, and their union forms the parent's area. In other words, the structure we will use in this theme is a tree in which each non-terminal node will have exactly 4 descendants.

For a quadtree, the root represents a square, and its descendants represent four disjoint and congruent squares in which the initial square can be divided. As presented so far, a quadtree is infinite, but we will use this data structure to represent an image with a reduced size in the plane. For this reason, we will use only a few levels of the quadtree.

3.2 Compression Algorithm
Any square image with a size that is a power of 2 can be represented by a quadtree. The nodes at each level of the tree correspond to a subdivision of a square area of the image into four quadrants. The root of the tree is associated with the entire image, the nodes at the first level of the tree correspond to the four quadrants of the image (the order being: top-left, top-right, bottom-right, bottom-left), the nodes at the second level correspond to the quadrants of each previous quadrant, and so on. The subdivision of the image can continue until the nodes at the current level of the tree correspond to uniform square areas: if the considered square region is not uniform, it will be decomposed, by cutting, into four equal parts, and the corresponding node will become non-terminal, having four descendants. If the considered region is uniform (composed of pixels of the same type), the respective node becomes a leaf (terminal) node of the tree. Each leaf node contains information corresponding to the value of the image area it is associated with.
For a better understanding of the algorithm described in the previous paragraph, it is proposed to analyze the example below.
![image](https://github.com/victorandrei03/quad-tree/assets/117905946/10f93394-946e-468a-947a-146ee7ab6a8d)
