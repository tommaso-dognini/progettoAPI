/** C implementation for
	Red-Black Tree Insertion
	This code is provided by
	costheta_z **/
#include <stdio.h>
#include <stdlib.h>
#define COUNT 10
// Structure to represent each
// node in a red-black tree
struct node
{
	int d;			// data
	int c;			// 1-red, 0-black
	struct node *p; // p
	struct node *r; // r-child
	struct node *l; // l child
};

// global root for the entire tree
struct node *root = NULL;

// function to perform BST insertion of a node
struct node *bst(struct node *trav,
				 struct node *temp)
{
	// If the tree is empty,
	// return a new node
	if (trav == NULL)
		return temp;

	// Otherwise recur down the tree
	if (temp->d < trav->d)
	{
		trav->l = bst(trav->l, temp);
		trav->l->p = trav;
	}
	else if (temp->d > trav->d)
	{
		trav->r = bst(trav->r, temp);
		trav->r->p = trav;
	}

	// Return the (unchanged) node pointer
	return trav;
}

// Function performing r rotation
// of the passed node
void rightRotate(struct node *temp)
{
	struct node *l = temp->l;
	temp->l = l->r;
	if (temp->l)
		temp->l->p = temp;
	l->p = temp->p;
	if (!temp->p)
		root = l;
	else if (temp == temp->p->l)
		temp->p->l = l;
	else
		temp->p->r = l;
	l->r = temp;
	temp->p = l;
}

// Function performing l rotation
// of the passed node
void leftRotate(struct node *temp)
{
	struct node *r = temp->r;
	temp->r = r->l;
	if (temp->r)
		temp->r->p = temp;
	r->p = temp->p;
	if (!temp->p)
		root = r;
	else if (temp == temp->p->l)
		temp->p->l = r;
	else
		temp->p->r = r;
	r->l = temp;
	temp->p = r;
}

// This function fixes violations
// caused by BST insertion
void fixup(struct node *root, struct node *pt)
{
	struct node *p_pt = NULL;
	struct node *grand_p_pt = NULL;

	while ((pt != root) && (pt->c != 0) && (pt->p->c == 1))
	{
		p_pt = pt->p;
		grand_p_pt = pt->p->p;

		/* Case : A
			p of pt is l child
			of Grand-p of
		pt */
		if (p_pt == grand_p_pt->l)
		{

			struct node *uncle_pt = grand_p_pt->r;

			/* Case : 1
				The uncle of pt is also red
				Only Recing required */
			if (uncle_pt != NULL && uncle_pt->c == 1)
			{
				grand_p_pt->c = 1;
				p_pt->c = 0;
				uncle_pt->c = 0;
				pt = grand_p_pt;
			}

			else
			{

				/* Case : 2
					pt is r child of its p
					l-rotation required */
				if (pt == p_pt->r)
				{
					leftRotate(p_pt);
					pt = p_pt;
					p_pt = pt->p;
				}

				/* Case : 3
					pt is l child of its p
					r-rotation required */
				rightRotate(grand_p_pt);
				int t = p_pt->c;
				p_pt->c = grand_p_pt->c;
				grand_p_pt->c = t;
				pt = p_pt;
			}
		}

		/* Case : B
			p of pt is r
			child of Grand-p of
		pt */
		else
		{
			struct node *uncle_pt = grand_p_pt->l;

			/* Case : 1
				The uncle of pt is also red
				Only Recing required */
			if ((uncle_pt != NULL) && (uncle_pt->c == 1))
			{
				grand_p_pt->c = 1;
				p_pt->c = 0;
				uncle_pt->c = 0;
				pt = grand_p_pt;
			}
			else
			{
				/* Case : 2
				pt is l child of its p
				r-rotation required */
				if (pt == p_pt->l)
				{
					rightRotate(p_pt);
					pt = p_pt;
					p_pt = pt->p;
				}

				/* Case : 3
					pt is r child of its p
					l-rotation required */
				leftRotate(grand_p_pt);
				int t = p_pt->c;
				p_pt->c = grand_p_pt->c;
				grand_p_pt->c = t;
				pt = p_pt;
			}
		}
	}
}

// Function to print inorder traversal
// of the fixated tree
void inorder(struct node *trav)
{
	if (trav == NULL)
		return;
	inorder(trav->l);
	printf("%d ", trav->d);
	inorder(trav->r);
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(struct node *root, int space)
{
	// Base case
	if (root == NULL)
		return;

	// Increase distance between levels
	space += COUNT;

	// Process r child first
	print2DUtil(root->r, space);

	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%d\n", root->d);

	// Process l child
	print2DUtil(root->l, space);
}

// Wrapper over print2DUtil()
void print2D(struct node *root)
{
	// Pass initial space count as 0
	print2DUtil(root, 0);
}

// For balancing the tree after deletion
void deleteFix(struct node *x)
{
	struct node *s;
	while (x != root && x->c == 0)
	{
		if (x == x->p->l)
		{
			s = x->p->r;
			if (s->c == 1)
			{
				s->c = 0;
				x->p->c = 1;
				leftRotate(x->p);
				s = x->p->r;
			}

			if (s->l->c == 0 && s->r->c == 0)
			{
				s->c = 1;
				x = x->p;
			}
			else
			{
				if (s->r->c == 0)
				{
					s->l->c = 0;
					s->c = 1;
					rightRotate(s);
					s = x->p->r;
				}

				s->c = x->p->c;
				x->p->c = 0;
				s->r->c = 0;
				leftRotate(x->p);
				x = root;
			}
		}
		else
		{
			s = x->p->l;
			if (s->c == 1)
			{
				s->c = 0;
				x->p->c = 1;
				rightRotate(x->p);
				s = x->p->l;
			}

			if (s->r->c == 0 && s->r->c == 0)
			{
				s->c = 1;
				x = x->p;
			}
			else
			{
				if (s->l->c == 0)
				{
					s->r->c = 0;
					s->c = 1;
					leftRotate(s);
					s = x->p->l;
				}

				s->c = x->p->c;
				x->p->c = 0;
				s->l->c = 0;
				rightRotate(x->p);
				x = root;
			}
		}
	}
	x->c = 0;
}

struct node * minimum(struct node * node)
{
    while (node->l != NULL)
    {
        node = node->l;
    }
    return node;
}

void rbTransplant(struct node *u, struct node *v)
{
	if (u->p == NULL)
	{
		root = v;
	}
	else if (u == u->p->l)
	{
		u->p->l = v;
	}
	else
	{
		u->p->r = v;
	}
	v->p = u->p;
}


void deleteNode(struct node *node, int key)
{
	struct node *z = NULL;
	struct node *y;
	struct node *x;
	while (node != NULL)
	{
		if (node->d == key)
		{
			z = node;
		}

		if (node->d <= key)
		{
			node = node->r;
		}
		else
		{
			node = node->l;
		}
	}

	if (z == NULL)
	{
		printf("Key not found in the tree\n");
		return;
	}

	y = z;
	int y_original_color = y->c;
	if (z->l == NULL)
	{
		x = z->r;
		rbTransplant(z, z->r);
	}
	else if (z->r == NULL)
	{
		x = z->l;
		rbTransplant(z, z->l);
	}
	else
	{
		y = minimum(z->r);
		y_original_color = y->c;
		x = y->r;
		if (y->p == z)
		{
			x->p = y;
		}
		else
		{
			rbTransplant(y, y->r);
			y->r = z->r;
			y->r->p = y;
		}

		rbTransplant(z, y);
		y->l = z->l;
		y->l->p = y;
		y->c = z->c;
	}

	if (y_original_color == 0)
	{
		deleteFix(x);
	}
}

// driver code
int main()
{
	int n = 12;
	int a[12] = {7, 6, 5, 4, 3, 2, 1, 10, 20, 26, 14, 18};

	for (int i = 0; i < n; i++)
	{

		// allocating memory to the node and initializing:
		// 1. c as red
		// 2. p, l and r pointers as NULL
		// 3. data as i-th value in the array
		struct node *temp = (struct node *)malloc(sizeof(struct node));
		temp->r = NULL;
		temp->l = NULL;
		temp->p = NULL;
		temp->d = a[i];
		temp->c = 1;

		// calling function that performs bst insertion of
		// this newly created node
		root = bst(root, temp);

		// calling function to preserve properties of rb
		// tree
		fixup(root, temp);
		root->c = 0;
	}

	printf("Inorder Traversal of Created Tree\n");
	printf("Root: %d \n", root->d);
	inorder(root);

	print2D(root);

	deleteNode(root, 6);

	printf("Inorder Traversal of Created Tree\n");
	printf("Root: %d \n", root->d);
	inorder(root);

	print2D(root);

	return 0;
}
