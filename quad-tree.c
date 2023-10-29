/* PREDA Victor-Andrei - 311CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct rgb
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} rgb;

//the structure for the tree, left represents the leftmost node of all, left_mid
//node in the middle left, right_mid middle right and right - right of all
typedef struct tree
{
	struct tree *left;
	struct tree *right;
	struct tree *left_mid;
	struct tree *right_mid;
	rgb colours;
	int level;
	unsigned char node_type;
} tree;

typedef struct node_queue
{
	tree *node_tree;
	struct node_queue *next;
} node_queue;

typedef struct Tqueue
{
	struct node_queue *head;
	struct node_queue *tail;
} Tqueue;

//	funtion to create a queue
void create_queue(Tqueue **queue)
{
	*queue = (Tqueue *)calloc(1, sizeof(Tqueue));
	(*queue)->head = NULL;
	(*queue)->tail = NULL;
}

// function to add element to a queue
void enqueue(Tqueue **queue, tree *node_tree)
{
	if (node_tree == NULL) {
		return;
	}

	node_queue *new_node = (node_queue *)calloc(1, sizeof(node_queue));
	new_node->node_tree = node_tree;

	if ((*queue)->head == NULL)
	{
		(*queue)->head = new_node;
		(*queue)->tail = new_node;
	}
	else
	{
		(*queue)->tail->next = new_node;
		(*queue)->tail = new_node;
	}
}

//	function to remove a element from the queue
void dequeue(Tqueue **queue)
{
	if ((*queue)->head == NULL)
	{
		return;
	}

	node_queue *removed_node = (*queue)->head;
	(*queue)->head = (*queue)->head->next;
	free(removed_node);
}

// function to free the queue
void free_queue(Tqueue **queue)
{
	node_queue *node_current = (*queue)->head;
	while (node_current != NULL)
	{
		node_queue *aux = node_current;
		node_current = node_current->next;
		free(aux);
	}

	free(*queue);
}

// function for finding the colour of each node with the formulas from
// the statement
tree *check_colour(rgb **matrix, int size, tree *root, int start_column,
			 int start_line, int factor, int level)
{
	unsigned long long red = 0, green = 0, blue = 0, mean = 0;
	for (int i = start_line; i < start_line + size; i++)
	{
		for (int j = start_column; j < start_column + size; j++)
		{
			red += matrix[i][j].red;
			green += matrix[i][j].green;
			blue += matrix[i][j].blue;
		}
	}

	red = red / (size * size);
	green = green / (size * size);
	blue = blue / (size * size);

	for (int i = start_line; i < start_line + size; i++)
	{
		for (int j = start_column; j < start_column + size; j++)
		{
			mean += (red - matrix[i][j].red) * (red - matrix[i][j].red) +
					(green - matrix[i][j].green) * (green - matrix[i][j].green) +
					(blue - matrix[i][j].blue) * (blue - matrix[i][j].blue);
		}
	}

	mean = mean / (3 * size * size);

	root = (tree *)calloc(1, sizeof(tree));
	root->right = NULL;
	root->left = NULL;
	root->left_mid = NULL;
	root->right_mid = NULL;
	root->level = level;

	if (mean <= factor)
	{
		root->colours.red = (unsigned char)red;
		root->colours.blue = (unsigned char)blue;
		root->colours.green = (unsigned char)green;
		root->node_type = 1;
	}
	else
	{
		root->colours.red = 0;
		root->colours.blue = 0;
		root->colours.green = 0;
		root->node_type = 0;
	}
	return root;
}

/* recursive function for creating the tree, if the node is null I call
  the above function; after creation I check what type of node it is,
  if it is colored I return it, otherwise I recursively call the function
  for each branch, with the corresponding index: left for the upper left
  matrix, left_mid the upper right matrix, right_mid the lower left matrix
  and right the lower right matrix*/
tree *insert(rgb **matrix, int size, tree *root, int start_column,
			 int start_line, int factor, int level)
{
	if (size == 0)
	{
		return NULL;
	}
	if (root == NULL)
	{
		root = check_colour(matrix, size, root, start_column,
							start_line, factor, level);
	}
	if (root->node_type == 1)
	{
		return root;
	}

	root->left = insert(matrix, size / 2, root->left, start_column,
						start_line, factor, level + 1);

	root->left_mid = insert(matrix, size / 2, root->left_mid,
				start_column + size / 2, start_line, factor, level + 1);

	root->right_mid = insert(matrix, size / 2, root->right_mid,
		 start_column + size / 2, start_line + size / 2, factor, level + 1);

	root->right = insert(matrix, size / 2, root->right,
					 start_column, start_line + size / 2, factor, level + 1);

	return root;
}

/*  function in which I use a queue to insert all the elements from the tree
  into it and check if the node is of type 1 (colored) and I use the count
  parameter to count this type of nodes, after which I calculate the minimum
  distance from the root to a colored node to determine the largest matrix; at
  the end I calculate the maximum that represents the height of the tree */
void get_stats(tree *root, int *count, int *level, int *leaf_min_level)
{
	Tqueue *queue;

	create_queue(&queue);
	enqueue(&queue, root);

	node_queue *current_node = queue->head;

	while (current_node != NULL)
	{
		enqueue(&queue, current_node->node_tree->left);
		enqueue(&queue, current_node->node_tree->left_mid);
		enqueue(&queue, current_node->node_tree->right_mid);
		enqueue(&queue, current_node->node_tree->right);

		if (current_node->node_tree->node_type == 1)
		{
			*count += 1;
			if (current_node->node_tree->level < *leaf_min_level) {
				*leaf_min_level = current_node->node_tree->level;
			}
		}
		if (current_node->node_tree->level > *level) {
			*level = current_node->node_tree->level;
		}

		current_node = current_node->next;
	}

	free_queue(&queue);
}

/*	function for requirement 2 in which I put all the nodes in the tree in a
   queue to print them in order in the out file */
void print(tree *root, FILE *fp_out, unsigned int size_image)
{
	if (root == NULL)
	{
		return;
	}

	fwrite(&size_image, 1, sizeof(unsigned int), fp_out);
	Tqueue *queue;
	create_queue(&queue);
	enqueue(&queue, root);

	while (queue->head != NULL)
	{
		node_queue *current_node = queue->head;
		fwrite(&current_node->node_tree->node_type, 1, 1, fp_out);

		if (current_node->node_tree->node_type == 1)
		{
			fwrite(&current_node->node_tree->colours.red, 1, 1, fp_out);
			fwrite(&current_node->node_tree->colours.green, 1, 1, fp_out);
			fwrite(&current_node->node_tree->colours.blue, 1, 1, fp_out);
		}

		enqueue(&queue, current_node->node_tree->left);
		enqueue(&queue, current_node->node_tree->left_mid);
		enqueue(&queue, current_node->node_tree->right_mid);
		enqueue(&queue, current_node->node_tree->right);

		dequeue(&queue);
	}

	free_queue(&queue);
}

// function to free the quad-tree
void free_tree(tree *node)
{
	if (node != NULL)
	{
		free_tree(node->left);
		free_tree(node->left_mid);
		free_tree(node->right_mid);
		free_tree(node->right);
		free(node);
	}
}

// function similar to that of insert, less with the fact that, if the node is
//of type 1, I initialize each element of the matrix with the rgb in the node
void create_image(rgb **matrix, tree *root, unsigned int size,
				  unsigned int start_column, unsigned int start_line)
{
	if (root == NULL || size == 0)
	{
		return;
	}

	if (root->node_type == 1)
	{
		for (unsigned int i = start_line; i < start_line + size; i++)
		{
			for (unsigned int j = start_column; j < start_column + size; j++)
			{
				matrix[i][j].red = root->colours.red;
				matrix[i][j].green = root->colours.green;
				matrix[i][j].blue = root->colours.blue;
			}
		}
	}
	else
	{
		create_image(matrix, root->left, size / 2, start_column, start_line);

		create_image(matrix, root->left_mid, size / 2,
					 start_column + size / 2, start_line);

		create_image(matrix, root->right_mid, size / 2,
					 start_column + size / 2,start_line + size / 2);

		create_image(matrix, root->right, size / 2, start_column,
					 start_line + size / 2);
	}
}

// function to free the matrix
void free_matrix(rgb **matrix, unsigned int rows)
{
	for (int i = 0; i < rows; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

int power(int number, int exponent)
{
	int result = 1;
	for (int i = 0; i < exponent; i++)
	{
		result *= number;
	}
	return result;
}

/* function in which I read the elements from a file in a queue, these elements
  representing all the nodes in the tree; after I use another queue in
  which I will insert all nodes of type 0 (uncolored); when this queue will
  be empty, it means that I have finished creating the tree, this means that
  I will be able to call the image creation function (which creates the matrix)
  and insert it into the out file*/
void image_decompression(int start_column, int start_line, tree **root, FILE *fp_in,
						 FILE *fp_out)
{
	Tqueue *queue;
	Tqueue *queue_zeros;
	create_queue(&queue_zeros);
	create_queue(&queue);
	unsigned int rows;
	unsigned int finished_read = fread(&rows, 1, sizeof(unsigned int), fp_in);
	while (feof(fp_in) == 0 && finished_read != 0)
	{
		unsigned char node_type;
		finished_read = fread(&node_type, 1, sizeof(unsigned char), fp_in);
		if (finished_read != sizeof(unsigned char))
		{
			break;
		}
		tree *new_node_tree = (tree *)calloc(1, sizeof(tree));
		new_node_tree->node_type = node_type;

		if (node_type == 1)
		{
			finished_read = fread(&new_node_tree->colours, 1, sizeof(rgb), fp_in);
			if (finished_read != sizeof(rgb))
			{
				break;
			}
		}

		enqueue(&queue, new_node_tree);
	}

	*root = queue->head->node_tree;
	tree *root_aux = *root;
	dequeue(&queue);
	while (queue->head != NULL)
	{
		root_aux->left = queue->head->node_tree;
		dequeue(&queue);

		root_aux->left_mid = queue->head->node_tree;
		dequeue(&queue);

		root_aux->right_mid = queue->head->node_tree;
		dequeue(&queue);

		root_aux->right = queue->head->node_tree;
		dequeue(&queue);

		if (root_aux->left->node_type == 0)
		{
			enqueue(&queue_zeros, root_aux->left);
		}
		if (root_aux->left_mid->node_type == 0)
		{
			enqueue(&queue_zeros, root_aux->left_mid);
		}
		if (root_aux->right_mid->node_type == 0)
		{
			enqueue(&queue_zeros, root_aux->right_mid);
		}
		if (root_aux->right->node_type == 0)
		{
			enqueue(&queue_zeros, root_aux->right);
		}
		if (queue_zeros->head == NULL)
		{
			break;
		}

		root_aux = queue_zeros->head->node_tree;
		dequeue(&queue_zeros);
	}

	rgb **matrix_decompressed;
	matrix_decompressed = (rgb **)calloc(rows, sizeof(rgb *));
	for (int i = 0; i < rows; i++)
	{
		matrix_decompressed[i] = (rgb *)calloc(rows, sizeof(rgb));
	}

	fprintf(fp_out, "P6\n%u %u\n255\n", rows, rows);
	create_image(matrix_decompressed, *root, rows, start_column, start_line);

	for (int i = 0; i < rows; i++)
	{
		fwrite(matrix_decompressed[i], sizeof(rgb), rows, fp_out);
	}

	free_queue(&queue);
	free_queue(&queue_zeros);
	free_matrix(matrix_decompressed, rows);
}

// in main, I check the name of the first argument, after which I shall apply
// the operations requested by the specific statement
int main(int argc, char **argv)
{
	int start_column = 0;
	int start_line = 0;
	tree *root = NULL;

	if (strcmp(argv[1], "-d") == 0)
	{
		FILE *fp_in = fopen(argv[2], "rb");
		FILE *fp_out = fopen(argv[3], "wb");

		image_decompression(start_column, start_line, &root, fp_in, fp_out);

		fclose(fp_out);
		fclose(fp_in);
	}
	else
	{
		FILE *fp_in = fopen(argv[3], "rb");
		char buffer[3];
		int rows, columns, count;
		fread(buffer, 1, 3, fp_in);
		fscanf(fp_in, "%d %d\n%d", &rows, &columns, &count);
		fseek(fp_in, 1, SEEK_CUR);

		rgb **matrix;
		matrix = (rgb **)malloc(rows * sizeof(rgb *));
		for (int i = 0; i < rows; i++)
		{
			matrix[i] = (rgb *)malloc(columns * sizeof(rgb));
			unsigned char *row = (unsigned char *)malloc(columns * 3 *
														 sizeof(unsigned char));
			fread(row, sizeof(unsigned char), columns * 3, fp_in);

			for (int j = 0; j < columns * 3; j += 3)
			{
				matrix[i][j / 3].red = row[j];
				matrix[i][j / 3].green = row[j + 1];
				matrix[i][j / 3].blue = row[j + 2];
			}

			free(row);
		}

		int factor = atoi(argv[2]);
		root = insert(matrix, rows, root, start_column, start_line, factor, 1);
		if (strcmp(argv[1], "-c1") == 0)
		{
			FILE *fp_out = fopen(argv[4], "w");

			int levels = -1;
			int min_leaf_level = INT_MAX;
			int coloured_nodes = 0;
			get_stats(root, &coloured_nodes, &levels, &min_leaf_level);
			min_leaf_level -= 1;

			int size_big_matrix = (int)(rows / power(2, min_leaf_level));

			fprintf(fp_out, "%d\n%d\n%d\n", levels, coloured_nodes, size_big_matrix);
			fclose(fp_out);
		}
		else if (strcmp(argv[1], "-c2") == 0)
		{
			FILE *fp_out = fopen(argv[4], "wb");

			print(root, fp_out, rows);
			fclose(fp_out);
		}

		fclose(fp_in);
		free_matrix(matrix, rows);
	}

	free_tree(root);
	return 0;
}
