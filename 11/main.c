/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 11

   Topic: Linked list

   file name: main.c

   Date: May 21th, 2016

   Objective: Create a linked-list based on the input from a file.
              Do the following operations against the created
              linked-list:

              1) Reverse the list
              2) Delete the node, identified by the user

              Print the list after removing elements.

              Here is the sample input numbers:

              49, 17, 56, 85, 12, 97, 33, 71, 19, 62, 38, 84, 51,
              29, 77, 65, 44, 99, 4, 47, 67, 41, 23, 88, 73, 8, 10,
              100, 25, 91, 58, 45, 22, 15, 35, 95, 60, 20, 7, 50

              Here is the sample input by the user for deletion:

              71, 51, 38, 5, 0, 25, 42, 32 and 47
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

struct node {
	struct node *next;
	struct node *prev;
	int data;
};

struct list {
	struct node *head;
	struct node *tail;
};

static int xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	if (os != stdout) {
		va_end(ap);
		va_start(ap, fmt);
		vfprintf(os, fmt, ap);
	}
	va_end(ap);

	return ret;
}

static struct node *new_node(const int data)
{
	struct node *n;

	n = malloc(sizeof(*n));
	if (n == NULL)
		return NULL;

	n->next = n->prev = NULL;
	n->data = data;

	return n;
}

static int add_node(struct list *l, const int data)
{
	struct node *n;

	n = new_node(data);
	if (n == NULL)
		return -1;

	if (l->head == NULL)
		l->head = n;
	else
		l->tail->next = n;
	l->tail = n;

	return 0;
}

static void free_node(struct node *n)
{
	free(n);
}

static void delete_list(struct list *l)
{
	struct node *n, *next;

	for (n = l->head, next = n; next != l->tail; n = next) {
		next = n->next;
		free_node(n);
	}
}

static void print_list(const struct list *l, FILE *os)
{
	struct node *n;

	for (n = l->head; n != NULL; n = n->next)
		xprintf(os, "%d ", n->data);
	xprintf(os, "\n");
}

static void create_list(FILE *is, struct list *l)
{
	int i;

	while (fscanf(is, "%d,", &i) != EOF)
		add_node(l, i);
}

static void process(FILE *is, FILE *os)
{
	struct list l = { .head = NULL, .tail = NULL };

	xprintf(os, "\nLinked-list operations\n");
	xprintf(os, "======================\n\n");

	create_list(is, &l);
	xprintf(os, "1) Original list\n\n");
	print_list(&l, os);

	delete_list(&l);
}

int main()
{
	const char *input_file = "input.txt";
	const char *output_file = "output.txt";
	FILE *is, *os = NULL;

	is = fopen(input_file, "r");
	if (is == NULL) {
		fprintf(stderr, "Can't open '%s' file to read.\n",
				input_file);
		goto err;
	}

	os = fopen(output_file, "w");
	if (os == NULL) {
		fprintf(stderr, "Can't open '%s' file to write.\n",
				output_file);
		goto err;
	}

	process(is, os);

err:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);
}