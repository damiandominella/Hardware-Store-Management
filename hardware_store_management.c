/*****************************************************************************************************************************************************
*     DESCRIPTION:
*     Write an ANSI C program that works as follows:
*        1) It gets a text file whose format entails a given number of rows, each one containing:
*           name of the product, product identification code (4 characters), unitary cost, number of stored pieces (separated by space or tab).
*           For instance:
*            Nail_L_2.5  N924  1.20  300
*            Hammer H610  12.50  127
*            Screwdriver S234 4.55 432
*        2) It loads data into a suitable tree data structure.
*        3) It allows the user to choose and perform the following operations:
*            (a) insertion of new input from keyboard;
*            (b) deletion of existing data, selected from keyboard;
*            (c) print on display of the sorted list of data, sorted according to one of the 4 keys, chosen by the user.
*
*     AUTHOR: Damian Ariel Dominella <d.dominella@campus.uniurb.it>
*
******************************************************************************************************************************************************/

/* Including standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definition of constants */
#define INPUT_FILE "input.txt"
#define TYPE_ID 0
#define TYPE_NAME 1
#define TYPE_QTY 2
#define TYPE_PRICE 3

/* Structures declaration */
struct article {
    char *id;
    char *name;
    int quantity;
    float price;
};

struct node {
    struct article *item;
    struct node *left, *right;
};

/* Declaration of functions */

/* Binary tree functions */
struct node* new_node(struct article *item);
struct node* insert(struct node *node, struct article *item, int type);
struct node* min_value_node(struct node* node);
struct node* search(struct node* root, char *id);
void print_tree(struct node *root);

/* Article functions */
struct article* new_article(char *id, char *name, float price, int quantity);
void print_article(struct article *item);

/* General functions */
struct node* load_data(const char file[], int type);
void print_data(struct node *root);
void clear_buffer();
int get_valid_int(char *field_name);
float get_valid_float(char *field_name);


struct node* delete_qty(struct node *node, int quantity)
{
    printf("\nqty: %d", quantity);
    struct node *temp;
    if (node == NULL)
    {
        printf("\nElement Not Found");
    }
    else if (quantity < node->item->quantity)
    {
        node->left = delete_qty(node->left, quantity);
    }
    else if (quantity > node->item->quantity)
    {
        node->right = delete_qty(node->right, quantity);
    }
    else
    {
        /* Now We can delete this node and replace with either minimum element
                   in the right sub tree or maximum element in the left subtree*/
        if (node->right && node->left)
        {
            /* Here we will replace with minimum element in the right sub tree */
            temp = min_value_node(node->right);
            node->item = temp->item;
            /* As we replaced it with some other node, we have to delete that node */
            node->right = delete_qty(node->right, temp->item->quantity);
        }
        else
        {
            /* If there is only one or zero children then we can directly
                           remove it from the tree and connect its parent to its child */
            temp = node;
            if (node->left == NULL)
                node = node->right;
            else if (node->right == NULL)
                node = node->left;
            free(temp); /* temp is longer required */
        }
    }
    return node;
}

struct node* delete_price(struct node *node, float price)
{
    printf("\nprice: %.2f", price);
    struct node *temp;
    if (node == NULL)
    {
        printf("\nElement Not Found");
    }
    else if (price < node->item->price)
    {
        node->left = delete_price(node->left, price);
    }
    else if (price > node->item->price)
    {
        node->right = delete_price(node->right, price);
    }
    else
    {
        /* Now We can delete this node and replace with either minimum element
                   in the right sub tree or maximum element in the left subtree*/
        if (node->right && node->left)
        {
            /* Here we will replace with minimum element in the right sub tree */
            temp = min_value_node(node->right);
            node->item = temp->item;
            /* As we replaced it with some other node, we have to delete that node */
            node->right = delete_price(node->right, temp->item->price);
        }
        else
        {
            /* If there is only one or zero children then we can directly
                           remove it from the tree and connect its parent to its child */
            temp = node;
            if (node->left == NULL)
                node = node->right;
            else if (node->right == NULL)
                node = node->left;
            free(temp); /* temp is longer required */
        }
    }
    return node;
}

struct node* delete_id(struct node *node, char *id)
{
    printf("\nid: %s", id);
    struct node *temp;
    if (node == NULL)
    {
        printf("\nElement Not Found");
    }
    else if (strcmp(id, node->item->id) < 0)
    {
        node->left = delete_id(node->left, id);
    }
    else if (strcmp(id, node->item->id) > 0)
    {
        node->right = delete_id(node->right, id);
    }
    else
    {
        /* Now We can delete this node and replace with either minimum element
                   in the right sub tree or maximum element in the left subtree*/
        if (node->right && node->left)
        {
            /* Here we will replace with minimum element in the right sub tree */
            temp = min_value_node(node->right);
            node->item = temp->item;
            /* As we replaced it with some other node, we have to delete that node */
            node->right = delete_id(node->right, temp->item->id);
        }
        else
        {
            /* If there is only one or zero children then we can directly
                           remove it from the tree and connect its parent to its child */
            temp = node;
            if (node->left == NULL)
                node = node->right;
            else if (node->right == NULL)
                node = node->left;
            free(temp); /* temp is longer required */
        }
    }
    return node;
}

struct node* delete_name(struct node *node, char *name)
{
    printf("\nname: %s", name);
    struct node *temp;
    if (node == NULL)
    {
        printf("\nElement Not Found");
    }
    else if (strcmp(name, node->item->name) < 0)
    {
        node->left = delete_name(node->left, name);
    }
    else if (strcmp(name, node->item->name) > 0)
    {
        node->right = delete_name(node->right, name);
    }
    else
    {
        /* Now We can delete this node and replace with either minimum element
                   in the right sub tree or maximum element in the left subtree*/
        if (node->right && node->left)
        {
            /* Here we will replace with minimum element in the right sub tree */
            temp = min_value_node(node->right);
            node->item = temp->item;
            /* As we replaced it with some other node, we have to delete that node */
            node->right = delete_name(node->right, temp->item->name);
        }
        else
        {
            /* If there is only one or zero children then we can directly
                           remove it from the tree and connect its parent to its child */
            temp = node;
            if (node->left == NULL)
                node = node->right;
            else if (node->right == NULL)
                node = node->left;
            free(temp); /* temp is longer required */
        }
    }
    return node;
}

/* Main function */
int main()
{
    printf("\n*************************\nHardware store management\n*************************\n");

    /* Initialization of 4 binary trees, one for each type of data (id, name, price, quantity) */
    struct node *root_id = load_data(INPUT_FILE, TYPE_ID);
    struct node *root_name = load_data(INPUT_FILE, TYPE_NAME);
    struct node *root_qty = load_data(INPUT_FILE, TYPE_QTY);
    struct node *root_price = load_data(INPUT_FILE, TYPE_PRICE);

    /* Check for errors during the loading of data */
    if (root_id == NULL || root_name == NULL || root_qty == NULL || root_price == NULL) {
        printf("Opening file error");
        return 1;
    }

    int choiche;

    /* Menu options */
    do {
        printf("\n");
        printf("Choose an option:\n");
        printf("1) Display items\n");
        printf("2) Insert item\n");
        printf("3) Delete item\n");
        printf("0) Exit\n\n");
        printf("Choice: ");
        scanf("%d", &choiche);
        printf("\n");

        switch (choiche) {

        case 1:
            /* Before displaying data, user must select a sort key */
            printf("Display items, ");
            printf("please choose a sort key:\n");
            printf("0) ID\n");
            printf("1) Name\n");
            printf("2) Quantity\n");
            printf("3) Price\n\n");

            if (root_id == NULL) {
                printf("--------------------------------------------------------------\n");
                printf("Data set is empty");
                printf("--------------------------------------------------------------\n");
            } else {
                int sort;
                printf("Sort key: ");
                scanf("%d", &sort);

                /* Display data of the correct tree based on user's selected sort key */
                switch (sort) {
                case TYPE_ID:
                    print_data(root_id);
                    break;

                case TYPE_NAME:
                    print_data(root_name);
                    break;

                case TYPE_QTY:
                    print_data(root_qty);
                    break;

                case TYPE_PRICE:
                    print_data(root_price);
                    break;

                default:
                    break;
                }
            }

            break;

        case 2:
            printf("Insert item");
            printf("\n-------------------------------\n");

            char id[64], name[64];
            float price;
            int quantity;

            printf("ID: ");
            /* Duplicate ID check */
            int id_exists = 1;
            while (id_exists) {
                scanf("%s", id);
                if (search(root_id, id) != NULL) {
                    clear_buffer();
                    printf("ID already exists, try again: ");
                } else {
                    id_exists = 0;
                }
            }

            printf("Name: ");
            scanf("%s", name);
            printf("Quantity: ");
            quantity = get_valid_int("Quantity");
            printf("Price: ");
            price = get_valid_float("Price");

            /* Creating new article and inserting it every binary tree */
            struct article *product = new_article(id, name, price, quantity);
            root_id = insert(root_id, product, TYPE_ID);
            root_name = insert(root_name, product, TYPE_NAME);
            root_qty = insert(root_qty, product, TYPE_QTY);
            root_price = insert(root_price, product, TYPE_PRICE);
            break;

        case 3:
            printf("Delete item");
            print_data(root_id);

            char id_to_delete[64];
            printf("\nID to delete: ");
            /* ID check */
            int id_not_exists = 1;
            while (id_not_exists) {
                scanf("%s", id_to_delete);
                if (search(root_id, id_to_delete) == NULL) {
                    clear_buffer();
                    printf("ID does not exist, try again: ");
                } else {
                    id_not_exists = 0;
                }
            }

            /* Deleting node from every binary tree */
            /* TODO: delete does not work well */
            struct node* node_to_delete = search(root_id, id_to_delete);
            /* TODO: non tira su bene l'articolo da passare ai metodi */

            printf("\nID to delete: %s", node_to_delete->item->id);
            printf("\nName to delete: %s", node_to_delete->item->name);
            printf("\nQuantity to delete: %d", node_to_delete->item->quantity);
            printf("\nPrice to delete: %.2f", node_to_delete->item->price);

            root_id = delete_id(root_id, id_to_delete);
            root_name = delete_name(root_name, node_to_delete->item->name);
            root_price = delete_price(root_price, node_to_delete->item->price);
            root_qty = delete_qty(root_qty, node_to_delete->item->quantity);
            break;

        default:
            break;

        }

    } while (choiche != 0);

    free(root_id);
    free(root_name);
    free(root_qty);
    free(root_price);

    return 0;
}

/* Implementation of functions */

/* Article functions */

/* The function acquires the data (id, name, price, quantity) and creates a new article, returning it.*/
struct article* new_article(char *id, char *name, float price, int quantity)
{
    struct article *item = (struct article *)malloc(sizeof(struct article));

    if (item != NULL) {
        item->id = malloc(strlen(id) + 1);
        if (item->id == NULL) {
            printf("\n[ERROR] Memory allocation failed, try to re-run the program");
        } else {
            strcpy(item->id, id);
            item->name = malloc(strlen(name) + 1);
            if (item->name == NULL) {
                printf("\n[ERROR] Memory allocation failed, try to re-run the program");
            } else {
                strcpy(item->name, name);
                item->price = price;
                item->quantity = quantity;
            }
        }
    } else {
        printf("\n[ERROR] Memory allocation failed, try to re-run the program");
    }

    return item;
}

/* The function acquires the item and print its data in a formatted way */
void print_article(struct article *item)
{
    printf("%-15s%-20s%-15d%-15.2f\n", item->id, item->name, item->quantity, item->price);
}

/* Binary tree functions */

/* The function acquires the item and allocates a new node with the given data and NULL left and
   right pointers. Then, the node is returned. */
struct node* new_node(struct article *item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node)); /* Allocate memory for new node */

    if (temp != NULL) {
        temp->item = item; /* Allocate memory for new node */
        temp->left = temp->right = NULL; /* Initialize left and right children as NULL */
    } else {
        printf("\n[ERROR] Memory allocation failed, try to re-run the program");
    }

    return temp;
}

/* The function acquires a node, an item and the type of data to insert.
   Then it insert the item into the correct node, and return that node. */
struct node* insert(struct node *node, struct article *item, int type)
{
    /* If the tree is empty, return a new node */
    if (node == NULL) {
        node = new_node(item);
    } else {
        /* If the tree is not empty, recur down it to find the correct node to insert the given item.
           For data comparison between strings, strcmp() is used to get the smaller value */
        switch (type) {
        case TYPE_ID:
            if (strcmp(item->id, node->item->id) < 0) {
                node->left = insert(node->left, item, type);
            }
            else if (strcmp(item->id, node->item->id) > 0) {
                node->right = insert(node->right, item, type);
            }
            break;

        case TYPE_NAME:
            if (strcmp(item->name, node->item->name) < 0) {
                node->left = insert(node->left, item, type);
            }
            else if (strcmp(item->name, node->item->name) > 0) {
                node->right = insert(node->right, item, type);
            }
            break;

        case TYPE_QTY:
            if (item->quantity < node->item->quantity) {
                node->left = insert(node->left, item, type);
            }
            else if (item->quantity > node->item->quantity) {
                node->right = insert(node->right, item, type);
            }
            break;

        case TYPE_PRICE:
            if (item->price < node->item->price) {
                node->left = insert(node->left, item, type);
            }
            else if (item->price > node->item->price) {
                node->right = insert(node->right, item, type);
            }
            break;
        default:
            break;
        }
    }

    return node;
}

/* The function acquires a node, then return the node with minimum
   key value, that will be it or it's left child. */
struct node* min_value_node(struct node* node)
{
    struct node* current = node;

    /* Loop down to find the leftmost leaf */
    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

/* The function acquires the root node and the id of the searched element,
   then search a node that has the given id. It returns the node if the element exists, NULL otherwise */
struct node* search(struct node* root, char *id)
{
    struct node* result = NULL;

    if (root != NULL) {
        if (strcmp(root->item->id, id) == 0) { /* Item is in root */
            result = root;
        } else if (strcmp(root->item->id, id) < 0) { /* Item->id is greater than root's item->id */
            result = search(root->right, id);
        } else {   /* Item->id is smaller than root's item->id */
            result = search(root->left, id);
        }
    }

    return result;
}

void print_tree(struct node *root)
{
    if (root != NULL)
    {
        print_tree(root->left);
        print_article(root->item);
        print_tree(root->right);
    }
}

/* General functions */

struct node* load_data(const char file[], int type)
{
    /* Initializing tree */
    struct node *root = NULL;

    /* Opening input file */
    FILE* f = fopen(file, "r");

    /* Opening file error */
    if (f != NULL) {
        /* Loading data from file */
        char id[64], name[64];
        float price;
        int quantity;

        while (fscanf(f, "%s %s %f %d", name, id, &price, &quantity) != EOF) {
            struct article *product = new_article(id, name, price, quantity);
            root = insert(root, product, type);
        }
        fclose(f);
    }

    return root;
}

void print_data(struct node *root)
{
    printf("\n--------------------------------------------------------------\n");
    printf("%-15s%-20s%-15s%-15s\n", "ID", "Name", "Quantity", "Price");
    printf("--------------------------------------------------------------\n");
    print_tree(root);
    printf("--------------------------------------------------------------\n");
}

void clear_buffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(char *field_name)
{
    int is_valid = 0;
    int value;
    while (!is_valid) {
        if (scanf("%d", &value) == 1) {
            is_valid = 1;
        } else {
            clear_buffer();
            printf("%s must be of type integer, try again: ", field_name);
        }
    }

    return value;
}

/* TODO: non funziona bene per i tipi float */
float get_valid_float(char *field_name)
{
    int is_valid = 0;
    float value;
    while (!is_valid) {
        if (scanf("%f", &value) == 1) {
            is_valid = 1;
        } else {
            clear_buffer();
            printf("%s must be of type float, try again: ", field_name);
        }
    }

    return value;
}