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

/* Article functions */
struct article* new_article(char *id, char *name, float price, int quantity);
void print_article(struct article *item);

/* Binary tree functions */
struct node* load_data(const char file[], int type);
struct node* new_node(struct article *item);
struct node* insert(struct node *node, struct article *item, int type);
struct node* min_value_node(struct node* node);
struct node* delete(struct node *root, struct article *item, int type);
struct node* search(struct node* root, char *id);
void print_tree(struct node *root);

/* General functions */
void print_data(struct node *root);
void clear_buffer();
int get_valid_int(char *field_name);
float get_valid_float(char *field_name);

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

    int choice;

    /* Menu options */
    do {
        printf("\n");
        printf("Choose an option:\n");
        printf("1) Display items\n");
        printf("2) Insert item\n");
        printf("3) Delete item\n");
        printf("0) Exit\n\n");
        printf("Choice: ");
        choice = get_valid_int("Choice");
        printf("\n");

        switch (choice) {

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
            printf("\nBefore\n");
            print_tree(root_id);
            root_id = insert(root_id, product, TYPE_ID);
            printf("\nAfter\n");
            print_tree(root_id);

            printf("\n\n\n\n\n");

            printf("\nBefore\n");
            print_tree(root_name);
            root_name = insert(root_name, product, TYPE_NAME);
            printf("\nAfter\n");
            print_tree(root_name);

            printf("\n\n\n\n\n");

            printf("\nBefore\n");
            print_tree(root_qty);
            root_qty = insert(root_qty, product, TYPE_QTY);
            printf("\nAfter\n");
            print_tree(root_qty);

            printf("\n\n\n\n\n");

            printf("\nBefore\n");
            print_tree(root_price);
            root_price = insert(root_price, product, TYPE_PRICE);
            printf("\nAfter\n");
            print_tree(root_price);
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

            struct node* node_to_delete = search(root_id, id_to_delete);

            printf("\nNode to delete name:");
            print_article(node_to_delete->item);
            root_name = delete(root_name, node_to_delete->item, TYPE_NAME);

            printf("\nNode to delete price:");
            print_article(node_to_delete->item);
            root_price = delete(root_name, node_to_delete->item, TYPE_PRICE);

            printf("\nNode to delete quantity:");
            print_article(node_to_delete->item);
            root_qty = delete(root_name, node_to_delete->item, TYPE_QTY);

            printf("\nNode to delete id:");
            print_article(node_to_delete->item);
            root_id = delete(root_id, node_to_delete->item, TYPE_ID);
            break;

        default:
            printf("\nOption: %d) does not exist\n", choice);
            break;

        }

    } while (choice != 0);

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
        printf("\nELEMENT CREATED\n");
        node = new_node(item);
    } else {
        /* If the tree is not empty, recur down it to find the correct node to insert the given item.
           For data comparison between strings, strcmp() is used to get the smaller value */
        switch (type) {
        case TYPE_ID:
            printf("\nInsert ID:\n");
            if (strcmp(item->id, node->item->id) < 0) {
                printf("\nInsert to sx");
                node->left = insert(node->left, item, type);
            }
            else if (strcmp(item->id, node->item->id) > 0) {
                printf("\nInsert to dx");
                node->right = insert(node->right, item, type);
            } else {
                printf("\nNIENTE");
            }
            break;

        case TYPE_NAME:
            printf("\nInsert Name:\n");
            if (strcmp(item->name, node->item->name) < 0) {
                printf("\nInsert to sx");
                node->left = insert(node->left, item, type);
            }
            else if (strcmp(item->name, node->item->name) > 0) {
                printf("\nInsert to dx");
                node->right = insert(node->right, item, type);
            } else {
                printf("\nNIENTE");
            }
            break;

        case TYPE_QTY:
            printf("\nInsert Qty:\n");
            printf("\nItem->qty: %d", item->quantity);
            printf("\nNode->item->qty:%d", node->item->quantity);
            if (item->quantity < node->item->quantity) {
                printf("\nInsert to sx");
                node->left = insert(node->left, item, type);
            }
            else if (item->quantity > node->item->quantity) {
                printf("\nInsert to dx");
                node->right = insert(node->right, item, type);
            } else {
                printf("\nNIENTE");
            }
            break;

        case TYPE_PRICE:
            printf("\nInsert price:\n");
            printf("\nItem->price: %.2f", item->price);
            printf("\nNode->item->price:%.2f", node->item->price);
            if (item->price < node->item->price) {
                printf("\nInsert to sx");
                node->left = insert(node->left, item, type);
            }
            else if (item->price > node->item->price) {
                printf("\nInsert to dx");
                node->right = insert(node->right, item, type);
            } else {
                printf("\nNIENTE");
            }
            break;
        default:
            break;
        }
    }

    return node;
}

/* Given a non-empty binary search tree, return the node with minimum
   key value found in that tree. Note that the entire tree does not
   need to be searched. */
struct node* min_value_node(struct node *node)
{
    struct node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

/* Given a binary search tree and a key, this function deletes the key
   and returns the new root */
struct node* delete(struct node *root, struct article *item, int type)
{
    printf("\n\n\nDELETE function");
    // base case
    if (root == NULL) {
        printf("\nRoot is null");
        return root;
    }

    switch (type) {
    case TYPE_ID:
        printf("\n\n\nDelete by id\n");
        print_article(item);
        // If the key to be deleted is smaller than the root's key,
        // then it lies in left subtree
        if (strcmp(item->id, root->item->id) < 0) {
            printf("\nChiamo delete a sx");
            root->left = delete(root->left, item, type);
        }

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
        else if (strcmp(item->id, root->item->id) > 0) {
            printf("\nChiamo delete a dx");
            root->right = delete(root->right, item, type);
        }

        // if key is same as root's key, then This is the node
        // to be deleted
        else
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                printf("\nRoot->left == NULL, elimino NODO");
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                printf("\nRoot->right == NULL, elimino NODO");
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            printf("\n2 figli, vedo cosa fare");
            printf("\nPrendo figlio piu piccolo a dx\n");

            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = min_value_node(root->right);

            // Copy the inorder successor's content to this node
            printf("\nCopio l'elemento appena trovato su root->item\n");
            root->item = temp->item;
            print_article(root->item);

            printf("\nElimino partendo da root->right, l'elemento qui sopra\n");

            // Delete the inorder successor
            root->right = delete(root->right, temp->item, type);
        }
        break;
    case TYPE_NAME:
        printf("\n\n\nDelete by name\n");
        print_article(item);
        // If the key to be deleted is smaller than the root's key,
        // then it lies in left subtree
        if (strcmp(item->name, root->item->name) < 0) {
            printf("\nChiamo delete a sx");
            root->left = delete(root->left, item, type);
        }

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
        else if (strcmp(item->name, root->item->name) > 0) {
            printf("\nChiamo delete a dx");
            root->right = delete(root->right, item, type);
        }

        // if key is same as root's key, then This is the node
        // to be deleted
        else
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                printf("\nRoot->left == NULL, elimino NODO");
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                printf("\nRoot->right == NULL, elimino NODO");
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            printf("\n2 figli, vedo cosa fare");
            printf("\nPrendo figlio piu piccolo a dx\n");
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = min_value_node(root->right);

            // Copy the inorder successor's content to this node
            printf("\nCopio l'elemento appena trovato su root->item\n");
            root->item = temp->item;
            print_article(root->item);

            printf("\nElimino partendo da root->right, l'elemento qui sopra\n");
            // Delete the inorder successor
            root->right = delete(root->right, temp->item, type);
        }
        break;
    case TYPE_PRICE:
        printf("\n\n\nDelete by price\n");
        print_article(item);
        // If the key to be deleted is smaller than the root's key,
        // then it lies in left subtree
        if (item->price < root->item->price) {
            printf("\nChiamo delete a sx");
            root->left = delete(root->left, item, type);
        }

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
        else if (item->price > root->item->price) {
            printf("\nChiamo delete a dx");
            root->right = delete(root->right, item, type);
        }

        // if key is same as root's key, then This is the node
        // to be deleted
        else
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                printf("\nRoot->left == NULL, elimino NODO");
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                printf("\nRoot->right == NULL, elimino NODO");
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            printf("\n2 figli, vedo cosa fare");
            printf("\nPrendo figlio piu piccolo a dx\n");
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = min_value_node(root->right);

            // Copy the inorder successor's content to this node
            printf("\nCopio l'elemento appena trovato su root->item\n");
            root->item = temp->item;
            print_article(root->item);

            printf("\nElimino partendo da root->right, l'elemento qui sopra\n");
            // Delete the inorder successor
            root->right = delete(root->right, temp->item, type);
        }
        break;
    case TYPE_QTY:
        printf("\n\n\nDelete by price\n");
        print_article(item);
        // If the key to be deleted is smaller than the root's key,
        // then it lies in left subtree
        if (item->quantity < root->item->quantity) {
            printf("\nChiamo delete a sx");
            root->left = delete(root->left, item, type);
        }

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
        else if (item->quantity > root->item->quantity) {
            printf("\nChiamo delete a dx");
            root->right = delete(root->right, item, type);
        }

        // if key is same as root's key, then This is the node
        // to be deleted
        else
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                printf("\nRoot->left == NULL, elimino NODO");
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                printf("\nRoot->right == NULL, elimino NODO");
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            printf("\n2 figli, vedo cosa fare");
            printf("\nPrendo figlio piu piccolo a dx\n");
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = min_value_node(root->right);

            // Copy the inorder successor's content to this node
            printf("\nCopio l'elemento appena trovato su root->item\n");
            root->item = temp->item;
            print_article(root->item);

            printf("\nElimino partendo da root->right, l'elemento qui sopra\n");
            // Delete the inorder successor
            root->right = delete(root->right, temp->item, type);
        }
        break;
    }

    return root;
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