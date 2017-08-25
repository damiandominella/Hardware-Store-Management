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

/* Binary tree types */
#define TYPE_ID 0
#define TYPE_NAME 1
#define TYPE_QTY 2
#define TYPE_PRICE 3

#define ID_LENGTH 4

/* Structures declaration */

/* Article structure */
struct article {
    char *id;
    char *name;
    int quantity;
    float price;
};

/* Node of binary tree structure */
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
struct node* search_id(struct node* root, char *id);
struct node* search_name(struct node* root, char *id);
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
        choice = get_valid_int("Choice"); /* Acquiring a valid integer using get_valid_int() function */
        printf("\n");

        switch (choice) {

        case 1:
            /* Before displaying data, user must select a sort key, but if the binary tree is empty, this step can be skipped */
            if (root_id == NULL) {
                printf("--------------------------------------------------------------\n");
                printf("\nData set is empty");
                printf("--------------------------------------------------------------\n");
            } else {
                printf("Display items, ");
                printf("please choose a sort key\n");
                printf("0) ID\n");
                printf("1) Name\n");
                printf("2) Quantity\n");
                printf("3) Price\n\n");

                /* Acquiring sort key and validating it */
                int sort_key;
                printf("Sort key: ");
                do {
                    sort_key = get_valid_int("Sort key");
                    if (sort_key < 0 || sort_key > 3) {
                        printf("Sort key %d) does not exists, try again: ", sort_key);
                    }
                } while (sort_key < 0 || sort_key > 3);

                /* Display data from the correct tree based on user's selected sort key */
                printf("\nData sorted by field: ");
                switch (sort_key) {
                case TYPE_ID:
                    printf("ID");
                    print_data(root_id);
                    break;

                case TYPE_NAME:
                    printf("Name");
                    print_data(root_name);
                    break;

                case TYPE_QTY:
                    printf("Quantity");
                    print_data(root_qty);
                    break;

                case TYPE_PRICE:
                    printf("Price");
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

            /* Acquision of the new record to insert in binary tree, ID and name fields are unique values
               so they can't be duplicate, that's the reason of the following checks */
            printf("ID: ");
            int id_exists = 1;
            while (id_exists) {
                scanf("%s", id);
                if (search_id(root_id, id) != NULL) {
                    clear_buffer();
                    printf("A record with ID: %s already exists, try again: ", id);
                } else if (strlen(id) > ID_LENGTH || strlen(id) < ID_LENGTH) {
                    clear_buffer();
                    printf("ID length must be of %d characters: ", ID_LENGTH);
                } else {
                    id_exists = 0;
                }
            }

            printf("Name: ");
            /* Duplicate name check */
            int name_exists = 1;
            while (name_exists) {
                scanf("%s", name);
                if (search_name(root_name, name) != NULL) {
                    clear_buffer();
                    printf("A record with name: %s already exists, try again: ", name);
                } else {
                    name_exists = 0;
                }
            }

            printf("Quantity: ");
            quantity = get_valid_int("Quantity");
            printf("Price: ");
            price = get_valid_float("Price");

            /* Creating new article and inserting it every binary tree */
            struct article *item = new_article(id, name, price, quantity);

            if (item != NULL) {
                root_id = insert(root_id, item, TYPE_ID);
                root_name = insert(root_name, item, TYPE_NAME);
                root_qty = insert(root_qty, item, TYPE_QTY);
                root_price = insert(root_price, item, TYPE_PRICE);

                printf("\nRecord inserted successfully");
                print_data(root_id);
            } else {
                choice = 0; /* Memory allocation error, exit the program setting the choice = 0 */
            }

            break;

        case 3:
            printf("Delete item");
            print_data(root_id); /* Displaying data to ease the user to select the ID */

            /* Checking if the selected ID exists */
            char id_to_delete[64];
            printf("\nID to delete: ");
            int id_not_exists = 1;
            while (id_not_exists) {
                scanf("%s", id_to_delete);
                if (search_id(root_id, id_to_delete) == NULL) {
                    clear_buffer();
                    printf("ID does not exist, try again: ");
                } else {
                    id_not_exists = 0;
                }
            }

            /* Searching the node to delete by the selected ID and deleting that node from every binary tree */
            struct node* node_to_delete = search_id(root_id, id_to_delete);

            root_name = delete(root_name, node_to_delete->item, TYPE_NAME);
            root_price = delete(root_price, node_to_delete->item, TYPE_PRICE);
            root_qty = delete(root_qty, node_to_delete->item, TYPE_QTY);
            root_id = delete(root_id, node_to_delete->item, TYPE_ID);

            printf("\nRecord deleted successfully\n");
            break;

        default:
            if (choice != 0) printf("\nOption: %d) does not exist\n", choice);
            break;

        }

    } while (choice != 0);

    /* Memory de-allocation */
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
    /* Allocating memory for the new item */
    struct article *item = (struct article *)malloc(sizeof(struct article));

    /* Checking for memory allocation errors */
    if (item != NULL) {
        item->id = malloc(strlen(id) + 1);
        if (item->id == NULL) {
            item = NULL;
            printf("\n[ERROR] Memory allocation failed, try to re-run the program");
        } else {
            strcpy(item->id, id);
            item->name = malloc(strlen(name) + 1);
            if (item->name == NULL) {
                item = NULL;
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

/* The function acquires the input file where the data is stored and the binary tree type to insert that data */
struct node* load_data(const char file[], int type)
{
    /* Initializing tree */
    struct node *root = NULL;

    /* Opening input file */
    FILE* f = fopen(file, "r");

    /* Opening file error */
    if (f != NULL) {
        char id[64], name[64];
        float price;
        int quantity;

        /* Loading data from file */
        while (fscanf(f, "%s %s %f %d", name, id, &price, &quantity) != EOF) {
            /* Creating new article and inserting it in binary tree */
            struct article *item = new_article(id, name, price, quantity);
            if (item != NULL) {
                root = insert(root, item, type);
            } else {
                root = NULL;
                break; /* Memory allocation failed */
            }
        }
        fclose(f);
    }

    return root;
}

/* The function acquires the item and allocates a new node with the given data.
It also initialize the node left and right pointers as NULL. Then, the node is returned. */
struct node* new_node(struct article *item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node)); /* Allocate memory for new node */

    if (temp != NULL) {
        temp->item = item; /* Storing the item in the node */
        temp->left = temp->right = NULL; /* Initialize left and right child as NULL */
    } else {
        printf("\n[ERROR] Memory allocation failed, try to re-run the program");
    }

    return temp;
}

/* The function acquires a node, an item and the type of data to insert.
   Then it insert the item into the correct node, and return that node. */
struct node* insert(struct node *node, struct article *item, int type)
{
    /* If the node is NULL, it is the node where to insert the item */
    if (node == NULL) {
        node = new_node(item);
    } else {
        /* If the tree is not empty, recur down it to find the correct node to insert the given item.
           For data comparison between strings, strcmp() is used to get the smaller value */

        /* The switch statement is used to compare the correct item value */
        switch (type) {
        /* The logic behind the insert function is quite simple:
        if the value to insert is smaller than the current node value then use recursion to insert the value in his left child.
        Otherwise insert it in his right child */

        /* ID and Name are unique values, so < and > are enough for values comparyson, because duplicate values are not allowed. */
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

        /* Quantity and Price fields allow duplicate values, so <= and > are required for values comparyson. */
        case TYPE_QTY:
            if (item->quantity <= node->item->quantity) {
                node->left = insert(node->left, item, type);
            }
            else if (item->quantity > node->item->quantity) {
                node->right = insert(node->right, item, type);
            }
            break;

        case TYPE_PRICE:
            if (item->price <= node->item->price) {
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

/* The function acquires a node and return his smallest child */
struct node* min_value_node(struct node *node)
{
    struct node* temp = node;

    /* Loop down the node until the leftmost child is found*/
    while (temp->left != NULL) {
        temp = temp->left;
    }

    return temp;
}

/* The function acquires a node, an item and the type of data to delete.
   Then it delete the item from the correct tree, and return that tree. */
struct node* delete(struct node *root, struct article *item, int type)
{
    /* Simplest case, if root is null, do nothing and return it */
    if (root == NULL) {
        return root;
    }

    /* The switch statement is used to compare the correct item value */
    switch (type) {

    /* The logic behind the delete is the same for every field, with a little difference for Price and Quantity.*/

    case TYPE_ID:
        /* If the key to be deleted is smaller than the root's key, then it lies in left subtree */
        if (strcmp(item->id, root->item->id) < 0) {
            root->left = delete(root->left, item, type);
        }
        /* If the key to be deleted is greater than the root's key, then it lies in right subtree */
        else if (strcmp(item->id, root->item->id) > 0) {
            root->right = delete(root->right, item, type);
        }
        /* If key is same as root's key, then this is the node to be deleted */
        else
        {
            /* Node with only one child or no child */
            if (root->left == NULL)
            {
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            /* Node with two children: get the smallest child the right subtree */
            struct node* temp = min_value_node(root->right);

            /* Copy the found node item to this node */
            root->item = temp->item;

            /* Deleting the found node */
            root->right = delete(root->right, temp->item, type);
        }
        break;
    case TYPE_NAME:
        if (strcmp(item->name, root->item->name) < 0) {
            root->left = delete(root->left, item, type);
        } else if (strcmp(item->name, root->item->name) > 0) {
            root->right = delete(root->right, item, type);
        } else {
            if (root->left == NULL) {
                struct node *temp = root->right;
                free(root);
                return temp;
            } else if (root->right == NULL) {
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            struct node* temp = min_value_node(root->right);
            root->item = temp->item;
            root->right = delete(root->right, temp->item, type);
        }
        break;
    case TYPE_PRICE:
        if (item->price < root->item->price) {
            root->left = delete(root->left, item, type);
        } else if (item->price > root->item->price) {
            root->right = delete(root->right, item, type);
        } else {
            /* Since price field allows duplicate values, an ID comparyson must be done to delete the right item */
            if (strcmp(item->id, root->item->id) == 0) {
                if (root->left == NULL) {
                    struct node *temp = root->right;
                    free(root);
                    return temp;
                } else if (root->right == NULL) {
                    struct node *temp = root->left;
                    free(root);
                    return temp;
                }

                struct node* temp = min_value_node(root->right);
                root->item = temp->item;
                root->right = delete(root->right, temp->item, type);
            } else {
                /* If ID is not the same, then delete the left child of that node (since insert on left child is conditioned by <= ) */
                root->left = delete(root->left, item, type);
            }
        }
        break;
    case TYPE_QTY:
        if (item->quantity < root->item->quantity) {
            root->left = delete(root->left, item, type);
        } else if (item->quantity > root->item->quantity) {
            root->right = delete(root->right, item, type);
        } else {
            /* Since quantity field allows duplicate values, an ID comparyson must be done to delete the right item */
            if (strcmp(item->id, root->item->id) == 0) {
                if (root->left == NULL)
                {
                    struct node *temp = root->right;
                    free(root);
                    return temp;
                } else if (root->right == NULL) {
                    struct node *temp = root->left;
                    free(root);
                    return temp;
                }

                struct node* temp = min_value_node(root->right);
                root->item = temp->item;
                root->right = delete(root->right, temp->item, type);
            } else {
                /* If ID is not the same, then delete the left child of that node (since insert on left child is conditioned by <= ) */
                root->left = delete(root->left, item, type);
            }
        }
        break;
    }

    return root;
}

/* The function acquires the root node and the id of the searched element,
   then search a node that has the given id. It returns the node if the element exists, NULL otherwise */
struct node* search_id(struct node* root, char *id)
{
    struct node* result = NULL;

    if (root != NULL) {
        if (strcmp(root->item->id, id) == 0) { /* Item is in root */
            result = root;
        } else if (strcmp(root->item->id, id) < 0) { /* Item->id is greater than root's item->id */
            result = search_id(root->right, id);
        } else {   /* Item->id is smaller than root's item->id */
            result = search_id(root->left, id);
        }
    }

    return result;
}

/* The function acquires the root and the name of the searched element,
   then search a node that has the given name. It returns the node if the element exists, NULL otherwise */
struct node* search_name(struct node* root, char *name)
{
    struct node* result = NULL;

    if (root != NULL) {
        if (strcmp(root->item->name, name) == 0) { /* Item is in root */
            result = root;
        } else if (strcmp(root->item->name, name) < 0) { /* Item->id is greater than root's item->id */
            result = search_name(root->right, name);
        } else {   /* Item->id is smaller than root's item->id */
            result = search_name(root->left, name);
        }
    }

    return result;
}

/* The function acquires the root and print its data in order */
void print_tree(struct node *root)
{
    if (root != NULL)
    {
        /* Use recursion to elaborate the left child */
        print_tree(root->left);
        /* Elaborate node item */
        print_article(root->item);
        /* Use recursion to elaborate the right child */
        print_tree(root->right);
    }
}

/* General functions */

/* The function acquires the root and print its data in a formatted way */
void print_data(struct node *root)
{
    printf("\n--------------------------------------------------------------\n");
    printf("%-15s%-20s%-15s%-15s\n", "ID", "Name", "Quantity", "Price");
    printf("--------------------------------------------------------------\n");
    print_tree(root);
    printf("--------------------------------------------------------------\n");
}

/* Clear buffer function to avoid scanf errors */
void clear_buffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(char *field_name)
{
    int is_valid = 0;
    int value;
    char term;
    while (!is_valid) {
        if (scanf("%d%c", &value, &term) != 2 || term != '\n') {
            clear_buffer();
            printf("%s must be of type integer, try again: ", field_name);
        }
        else {
            is_valid = 1;
        }
    }

    return value;
}

float get_valid_float(char *field_name)
{
    int is_valid = 0;
    float value;
    char term;
    while (!is_valid) {
        if (scanf("%f%c", &value, &term) != 2 || term != '\n') {
            clear_buffer();
            printf("%s must be of type float, try again: ", field_name);
        } else {
            is_valid = 1;
        }
    }

    return value;
}