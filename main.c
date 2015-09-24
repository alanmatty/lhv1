#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct product_t {
    char name[50];
    char description[128];
    char shelf[9];
    int price;
    int amount;
    struct product_t *next;
} product;


typedef struct undo_t {
    int type; // add = 1, edit = 2, delete = 3
    int id;
    struct product_t data;
} undo;



undo *saveUndo(int type, product *head, int id);
product *addProductById(product *head, int id);

product *head = NULL;
product *last = NULL;
undo *undo_d = NULL;

bool confirm(char *q)
{
    char input[25];
    
    
    while( true )
    {
        printf("%s", q);
        scanf("%s", input);
        
        if( strcasecmp(input, "y") == 0 )
            return true;
        else if( strcasecmp(input, "n") == 0 )
            return false;
        else
            printf("Felaktig inmatning: '%s'\n", input);
    }
    
}

bool validateShelf(char *shelf)
{
    unsigned long length = strlen(shelf);
    
    // make sure that the first letter is a alphabet
    if( !isalpha(shelf[0]) || !isdigit(shelf[1]) )
        return false;
    
    // and the rest should be digits
    for(int i = 1; i < length; i++)
    {
        if( !isdigit(shelf[i]) )
            return false;
    }
    
    return true;
}

product *updateLast(product *head)
{
    product *current = head;
    product *previous = NULL;
    
    while( current != NULL )
    {
        previous = current;
        current = current->next;
    }
    
    return previous;
}

int getProductIdByName(product *head, char name[50])
{
    product *current = head;
    
    int i = 1;
    
    while( current != NULL )
    {
        if( strcasecmp(current->name, name) == 0)
        {
            return i;
        }
        
        current = current->next;
        i++;
    }
    
    return 0;
}


product *searchProductByName(product *head, char name[50])
{
    product *current = head;
    
    while( current != NULL )
    {
        if( strcasecmp(current->name, name) == 0)
        {
            return current;
        }
        
        current = current->next;
    }
    
    return NULL;
}


product *undo_data(product *head)
{
    
    return head;
    
}


product *searchProductById(product *head, int id)
{
    product *current = head;
    int i = 1;
    
    while( current != NULL )
    {
        if( i == id )
        {
            return current;
        }
        
        current = current->next;
        i++;
    }
    
    
    return current;
}


void showProducts(product *start, bool productInfo)
{
    
    
    product *current = start;
    int i = 1;
    
    
    //printf("%s", search.description);
    printf("\nVaror\n---------------------------------\n");
    if( start == NULL )
    {
        printf("Inga produkter att visa!\n");
        printf("---------------------------------\n\n");
    }
    else
    {
        while( current != NULL )
        {
            printf("%d. %s\n", i, current->name);
            
            current = current->next;
            i++;
        }
        
        if( productInfo == true )
        {
            char id[4];
            int input;
            
            printf("Välj produkt: ");
            while( true )
            {
                fgets(id, 3, stdin);
                
                if( sscanf(id, "%d", &input) == 1 )
                {
                    product *search = searchProductById(start, input);
                    printf("---------------------------------\n");
                    printf("Name: %s\n", search->name);
                    printf("Description: %s\n", search->description);
                    printf("Price: %d SEK\n", search->price);
                    printf("Lagerhylla: %s\n", search->shelf);
                    printf("Antal: %d\n", search->amount);
                    printf("---------------------------------\n\n");
                    break;
                }
            }
        }
        
    }
}




bool productNameExist(product *head, char *name)
{
    product *current = head;
    
    while( current != NULL )
    {
        if( strcasecmp(current->name, name) == 0 )
        {
            return true;
        }
        
        current = current->next;
        
    }
    
    return false;
}

bool isShelfAvailable(product *head, char *name, char *shelf)
{
    product *current = head;
    
    while( current != NULL )
    {
        if( strcasecmp(current->name, name) != 0 && strcasecmp(current->shelf, shelf) == 0 )
        {
            return false;
        }
        
        current = current->next;
    }
    
    return true;
}


product *deleteProduct(product *head, int id)
{
    
    int num = id;
    
    if( id == 0 )
    {
        
        showProducts(head, false);
        printf("Ta bort ID: ");
        scanf("%d", &num);
    }
    
    product *current = head;
    product *previous = NULL;
    
    int i = 1;
    
    product *search = searchProductById(head, num);
    
    if( id == 0 )
    {
        
        if( search != NULL )
        {
            printf("---------------------------------\n");
            printf("Name: %s\n", search->name);
            printf("Description: %s\n", search->description);
            printf("Price: %d SEK\n", search->price);
            printf("Storage Shelf: %s\n", search->shelf);
            printf("Amount: %d \n", search->amount);
            printf("---------------------------------\n");
            
            if( confirm("Ta bort varan? (Y/n): ") == false )
            {
                printf("Borttagningen avbröts!\n");
                return head;
            }
        }
        else
        {
            printf("Produkten existerar inte!\n");
            return head;
        }
    }
    
    
    while( current != NULL )
    {
        previous = current;
        current = current->next;
        
        
        
        if( num == i && previous != NULL )
        {
            undo_d = saveUndo(3, previous, num);
            head = previous->next;
            free(previous);
            
            break;
        }
        
        else if( (num - 1) == i )
        {
            undo_d = saveUndo(3, current, num);
            
            previous->next = current->next;
            free(current);
            break;
        }
        
        i++;
    }
    
    if( id == 0 )
        printf("Varan har blivit borttagen! \n\n");
    
    return head;
    
}

product *editProduct(product *head, int id)
{
    
    showProducts(head, false);
    product *current = head;
    int num;
    
    if( id == 0 )
    {
        printf("Redigera ID: ");
        scanf("%d", &num);
    }
    else
        num = id;
    
    
    product *search = searchProductById(current, num);
    
    if( search != NULL )
    {
        puts("[N]anmn");
        puts("[B]eskrivning");
        puts("[P]ris");
        puts("[L]agerhylla");
        puts("An[t]al");
        
        char inp[4];
        char name[25];
        char description[128];
        char shelf[9];
        int price;
        int amount;
        
        
        while( true )
        {
            printf("\nVälj eller [a]vbryt: ");
            scanf("%s", inp);
            
            if( strcasecmp(inp, "n") == 0 )
            {
                printf("Nuvarande namn: %s\n", search->name);
                printf("--------------------\n");
                printf("Nytt namn: ");
                scanf(" %[^\n]s", name);
                
                undo_d = saveUndo(2, head, num);
                
                strcpy(search->name, name);
                break;
            }
            else if( strcasecmp(inp, "b") == 0 )
            {
                printf("Nuvarande beskrivning: %s\n", search->description);
                printf("--------------------\n");
                printf("Ny beskrivning: ");
                scanf(" %[^\n]s", description);
                
                undo_d = saveUndo(2, head, num);
                
                strcpy(search->description, description);
                break;
            }
            else if( strcasecmp(inp, "p") == 0 )
            {
                printf("Nuvarande pris: %d\n", search->price);
                printf("--------------------\n");
                printf("Nytt pris: ");
                scanf("%d", &price);
                
                undo_d = saveUndo(2, head, num);
                
                search->price = price;
                break;
            }
            else if( strcasecmp(inp, "l") == 0 )
            {
                printf("Nuvarande lagerhylla: %s\n", search->shelf);
                printf("--------------------\n");
                printf("Flytta till lagerhylla: ");
                
                while( true )
                {
                    scanf("%s", shelf);
                    
                    if( validateShelf(shelf) == true )
                    {
                        undo_d = saveUndo(2, head, num);
                        strcpy(search->shelf, shelf);
                        break;
                    }
                    
                    printf("Fel lagerhylla, försök igen: ");
                    
                }
                
                break;
            }
            else if( strcasecmp(inp, "t") == 0 )
            {
                printf("Nuvarande antal: %d\n", search->amount);
                printf("--------------------\n");
                printf("Ny antal: ");
                scanf("%d", &amount);
                
                undo_d = saveUndo(2, head, num);
                
                
                search->amount = amount;
                break;
            }
            else
            {
                printf("Felaktig inmatning: ");
            }
        }
        
        
        
        
    }
    else
    {
        printf("Produkten existerar inte!\n");
    }
    
    return head;
}


undo *saveUndo(int type, product *head, int id)
{
    
    free(undo_d);
    undo *undo_d = malloc(sizeof(undo));
    
    undo_d->type = type;
    undo_d->id = id;
    undo_d->data = *head;
    
    return undo_d;
}

product *makeUndo(product *head, undo *undo_d)
{
    
    if( undo_d == NULL )
        return head;
    
    product *current = head;
    if( undo_d->type == 1 ) // add
    {
        head = deleteProduct(head, undo_d->id);
        printf("Undo add. \n", undo_d->data.name);
        
    }
    else if( undo_d->type == 2 ) // edit
    {
        product *search = searchProductById(current, undo_d->id);
        
        strcpy(search->name, undo_d->data.name);
        strcpy(search->description, undo_d->data.description);
        search->price = undo_d->data.price;
        strcpy(search->shelf, undo_d->data.shelf);
        search->amount = undo_d->data.amount;
        
        printf("Undo edit. \n");
    }
    else if( undo_d->type == 3 ) // delete
    {
        
        current = addProductById(current, undo_d->id);
        printf("Undo deletion. \n");
    }
    
    undo_d->type = 0;
    
    return current;
}

product *addProductById(product *head, int id)
{
    product *current = head;
    
    int i = 1;
    
    
    
    if( current == NULL )
    {
        
        current = malloc(sizeof(product));
        
        strcpy(current->name, undo_d->data.name);
        strcpy(current->description, undo_d->data.description);
        current->price = undo_d->data.price;
        strcpy(current->shelf, undo_d->data.shelf);
        current->amount = undo_d->data.amount;
    }
    else if( current != NULL )
    {
        while( current != NULL )
        {
            if( (id - 1) == i || id == 1 )
            {
                product *add = malloc(sizeof(product));
                
                strcpy(add->name, undo_d->data.name);
                strcpy(add->description, undo_d->data.description);
                add->price = undo_d->data.price;
                strcpy(add->shelf, undo_d->data.shelf);
                add->amount = undo_d->data.amount;
                
                if( id == 1 )
                {
                    
                    add->next = current;
                    return add;
                    
                }
                else
                {
                    add->next = current->next;
                    current->next = add;
                    
                    return head;
                }
                
                
            }
            
            current = current->next;
            i++;
        }
    }
    
    
    
    return current;
}

product *addProduct(product *previous)
{
    product *new = malloc(sizeof(product));
    
    char name[25];
    char tmp_name[25];
    
    char description[128];
    char tmp_description[128];
    
    char shelf[9];
    char tmp_shelf[9];
    
    char price[25];
    int tmp_price;
    
    printf("\nAnge namn: ");
    while( true )
    {
        fgets(name, 24, stdin);
        
        if( sscanf(name, "%[^\n]s", tmp_name) == 1 )
        {
            break;
        }
    }
    
    printf("Ange beskrivning: ");
    while( true )
    {
        fgets(description, 127, stdin);
        
        if( sscanf(description, "%[^\n]s", tmp_description) == 1 )
        {
            break;
        }
    }
    
    while( true )
    {
        printf("Ange pris (SEK): ");
        fgets(price, 24, stdin);
        if( sscanf(price, "%d", &tmp_price) == 1 )
        {
            break;
        }
        else
        {
            printf("Felaktig inmatning: %s", price);
        }
    }
    
    while( true )
    {
        printf("Lagerhylla (e.g A25): ");
        fgets(shelf, 8, stdin);
        sscanf(shelf, "%s", tmp_shelf);
        
        if( validateShelf(tmp_shelf) == true )
        {
            if( isShelfAvailable(head, tmp_name, tmp_shelf) == true)
                break;
            else
            {
                printf("En annan produkt finns på denna lagerhylla\n");
            }
        }
        else
            printf("Felaktig inmatning:\n");
        
    }
    
    if( confirm("Vill du lägga till produkten? (Y/n): ") == true )
    {
        
        strcpy(new->name, tmp_name);
        strcpy(new->description, tmp_description);
        new->price = tmp_price;
        strcpy(new->shelf, tmp_shelf);
        new->amount = 1;
        
        
        printf("\nProdukten har lagts till! \n-------------------------------\n");
        printf("Namn: %s\n", new->name);
        printf("Beskrivning: %s\n", new->description);
        printf("Pris: %d SEK\n", new->price);
        printf("Lagerhylla: %s\n", new->shelf);
        printf("Antal: %d\n", new->amount);
        printf("-------------------------------\n\n");
        
        
        if( previous != NULL )
        {
            previous->next = new;
        }
        
        
        
        return new;
    }
    else
    {
        free(new);
        
        printf("-------------------------------\n");
        printf("Lägg till operationen avbröts!\n");
        printf("-------------------------------\n");
        
        return previous;
    }
    
}

void freeMem(product *head)
{
    product *current = head;
    product *tmp = NULL;
    while( current != NULL )
    {
        tmp = current->next;
        free(current);
        current = tmp;
    }
}


int main(void) {
    
    // input
    char input[20];
    
    puts("Välkommen till lagerhantering 1.0");
    puts("=================================");
    puts("[L]ägga till en vara");
    puts("[T]a bort en vara");
    puts("[R]edigera en vara");
    puts("Ån[g]ra senaste ändringen");
    puts("Lista [h]ela varukatalogen");
    puts("[A]vsluta \n");
    
    
    while( true )
    {
        printf("Vad vill du göra: ");
        scanf("%s", input);
        
        if( strcasecmp(input, "l") == 0 )
        {
            // one->data = *head;
            
            if( head == NULL )
            {
                head = addProduct(NULL);
                last = head;
            }
            else
            {
                last = addProduct(last);
            }
            
            if( last != NULL )
            {
                int id = getProductIdByName(head, last->name);
                undo_d = saveUndo(1, head, id);
            }
            
        }
        else if( strcasecmp(input, "h") == 0)
        {
            showProducts(head, true);
        }
        else if( strcasecmp(input, "t") == 0 )
        {
            head = deleteProduct(head, 0);
            last = updateLast(head);
        }
        else if( strcasecmp(input, "r") == 0 )
        {
            head = editProduct(head, 0);
        }
        else if( strcasecmp(input, "a") == 0 )
        {
            printf("Avslutar...\n");
            break;
        }
        else if(strcasecmp(input, "g") == 0 )
        {
            head = makeUndo(head, undo_d);
            last = updateLast(head);
        }
        else
        {
            printf("Felaktig inmatning: %s\n", input);
        }
        
    }
    
    freeMem(head);
    printf("Avlustad.\n");
    
    return 0;
}
