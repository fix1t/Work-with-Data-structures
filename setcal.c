 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int size;           //number of items
    char **item;        //item array
    char **item_b;
    bool valid;
    bool is_set;
} set_t;

typedef struct {
    FILE*fp;
    int len;
    set_t *data;        //data line
} line_arr;

void line_arr_ctor(line_arr *a);

void item_arr_ctor(set_t *s);

void item_ctor(const char *tmp_a,const char *tmp_b, line_arr *a);

void line_arr_inc(line_arr *a);

void *item_arr_inc(set_t *item_structure);

void item_arr_dtor(set_t *data);

void line_arr_dtor(line_arr *a);

bool banned_words(char *item);

void is_valid_u(set_t *set1);

void is_valid(set_t *set,set_t *universum);

void alloc_error(line_arr *a);

void usage();

void check_txt(const char* file);

void not_valid(line_arr *a);

/* C functions */
void call_function(char *command, line_arr *a);

void empty(set_t set);

void card(set_t set);

void complement(set_t universum, set_t set);

void s_union(set_t set_1, set_t set_2); 

void intersect(set_t set_1, set_t set_2);

void domain(set_t relation);

void codomain(set_t relation);

void injective(set_t relation, set_t set_1, set_t set_2);

void surjective(set_t relation, set_t set_1, set_t set_2);

void bijective(set_t relation, set_t set_1, set_t set_2);

void minus(set_t  set_1, set_t set_2);

void subseteq(set_t  set_1, set_t set_2);

void subset(set_t  set_1, set_t set_2); 

void equals (set_t  set_1, set_t set_2); 

void reflexive(set_t relation, set_t universum);

void symmetric(set_t relation);

void antisymmetric(set_t relation); 

void transitive(set_t relation); 

bool function(set_t relation);


int main(int argc, char const *argv[])
{
    //check correct starting input
    if (argc == 2)
       check_txt(argv[1]); 
    else
        usage();

    
    line_arr a;                                         //create line array
    line_arr_ctor(&a);
    a.fp = fopen(argv[1] ,"r");                         //file pointer

    //variabales used in reading cycle
    char c = '\0';                                       //initializing char
    char temp_a[31];
    char temp_b[31];
    for (int i = 0; i < 32; i++)                        //init temp array
    {
        temp_a[i]='\0';
        temp_b[i]='\0';
    }
        
    int U_count =0,C_count =0, S_count = 0, R_count =0;
    while (c != EOF)
    {
        if ((U_count + C_count + S_count + R_count) > 1000 )    //if max line length is exceeded
            not_valid(&a);
        
        c = fgetc(a.fp);
        
        switch (c)
        {
// case universum 
        case 'U':
            
            if (++U_count != 1)                     //if there is more then one universum 
                not_valid(&a);
            if (S_count || C_count || R_count)      //check order
                not_valid(&a);
                
            //continue to create line as set ...

            line_arr_inc(&a);
            a.data[a.len-1].is_set=true;
            putchar(c);
            c = fgetc(a.fp);
            
            if (c == '\n')                          //if its an emty set break
            {   
                putchar(c);
                a.data[a.len-1].is_set = true;
                break;
            }

            if (c != ' ')                           //after capital letter must follow ' '
                    not_valid(&a);



            while (c != EOF)                        //read items
            {
                c = fgetc(a.fp);    
                
                for (int i = 0; temp_a[i] != '\0'; i++)                 //shortened string reset
                    temp_a[i]='\0';
                
                for (int i = 0; c != ' ' && c != '\n' && c != EOF; i++) //save chars until a word is over
                {
                    if (i>29)                                           //if input is longer than 30 - > not valid input
                        not_valid(&a);
                    temp_a[i] = c;
                    c = fgetc(a.fp); 
                }

                if (temp_a[0] != '\0')
                {
                    printf(" %s",temp_a);                               //if input is not empt print &
                    item_ctor(temp_a,NULL,&a);                          //construct item in item array
                }
                
                if (c == '\n')                                          //break line, continue to check validity
                {
                    putchar(c);
                    break;
                }
            }
            is_valid_u(&a.data[0]);                     //check validity of universsum
                
            if(a.data[a.len-1].size != 0)               //if not valid, crash program
            {
                if (!a.data[a.len-1].valid)
                    not_valid(&a);
            }

            break;

// case set
        case 'S':
            S_count++;
            if (!U_count || R_count || C_count)     //check the order
                not_valid(&a);
            
            line_arr_inc(&a);
            a.data[a.len-1].is_set = true;
            putchar(c);
            c = fgetc(a.fp);
            
            if (c == '\n')                          //if its an emty set break
            {   
                putchar(c);
                break;
            }

            if (c != ' ')                           //after capital letter must follow ' '
                    not_valid(&a);


            while (c != EOF)                        //read items
            {
                c = fgetc(a.fp);    
                
                for (int i = 0; temp_a[i] != '\0'; i++)                 //shortened string reset
                    temp_a[i]='\0';
                
                for (int i = 0; c != ' ' && c != '\n' && c != EOF; i++) //save chars until a word is over
                {
                    if (i>29)                                           //if input is longer than 30 - > not valid input
                        not_valid(&a);
                    temp_a[i] = c;
                    c = fgetc(a.fp); 
                }

                if (temp_a[0] != '\0')
                {
                    printf(" %s",temp_a);                               //if input is not empt print &
                    item_ctor(temp_a,NULL,&a);                          //construct item in item array
                }
                
                if (c == '\n')                                          //break line, continue to check validity
                {
                    putchar(c);
                    break;
                }
            }
            
            is_valid(&a.data[0], &a.data[a.len-1]);                     //check validity of universsum
                
            if(a.data[a.len-1].size != 0)                               //if not valid, crash program        
            {
                if (!a.data[a.len-1].valid)
                    not_valid(&a);
            }
            break;
// case relation
        case 'R':
            R_count++;
            if (!U_count || C_count)                        //check the order
                not_valid(&a);
          
            line_arr_inc(&a);                               //realloc line array & set it up                
            a.data[a.len-1].is_set = false;
            putchar(c);
            c = fgetc(a.fp);
 
            if (c == '\n')                                  //if empty break
            {
                putchar(c);
                break;
            }
            
            if (c != ' ')                                   //after capital letter must follow ' '
                not_valid(&a);

            while (c != EOF)                                //read items
            {
                for (int i = 0; temp_a[i]!= '\0' && temp_b[i] !='\0'; i++)                            //temp arrs set up
                {
                    temp_a[i]='\0';
                    temp_b[i]='\0';
                }
                
                if (c == '(')                                           //relation - (x y) - starts with '(' , save without brackets 
                {
                    for (int i = 0; (c = fgetc(a.fp)) != ' '; i++)      //get character & save char, until word ends
                    {
                        if (i>29)                                       //max length exceeded, crash 
                            not_valid(&a);
                        temp_a[i]=c;
                    }

                    for (int i = 0; (c = fgetc(a.fp)) != ')'; i++)      //until ')' read another word
                    {
                        if (c == ' ')                                   //forgive ' ' between words - if it is in the word - check_valid will uncover 
                        {
                            i--;
                            continue;
                        }
                        if (i>29)
                            not_valid(&a);
                        temp_b[i]=c;
                    }

                    item_ctor(temp_a,temp_b,&a);                        //construct item in item array

                    printf(" (%s %s)",a.data[a.len-1].item[a.data[a.len-1].size-1],a.data[a.len-1].item_b[a.data[a.len-1].size-1]);
                }                                                       //print from memory
                
                if ((c=fgetc(a.fp)) == '\n')                            //new line - break 
                {
                    putchar(c);
                    break;
                }
            }
            is_valid(&a.data[0], &a.data[a.len-1]);                     //check validity
            if (!a.data[a.len-1].valid)                                 //if valid = false, crash
                not_valid(&a);
            break;
// case command
        case 'C':
            C_count++;
            if (!U_count || !(S_count || R_count))                      //check the order
                not_valid(&a);
            c = fgetc(a.fp);
            if (c == '\n')                                              //command line cannot be empty
                not_valid(&a);
            
            if (c != ' ')                                               //' ' must follow 
                not_valid(&a);  

            char command[20];                                       
            for (int i = 0; i < 30; i++)
                command[i]='\0';
            fscanf(a.fp,"%s",command);                              //read command
            call_function(command,&a);                              //after functio '\n' or EOF follows, else wrong input                     
            c=fgetc(a.fp);
            if (c == '\n'  || c == EOF)
                break;
            not_valid(&a);           
            break;                         
        

        default:
        if(c  == EOF)
        {
        
            if (!U_count || !(S_count || R_count))          //if no U or S/R was entered
                not_valid(&a);
            if (U_count && !(S_count || R_count))           //if U entered but no S/R
                not_valid(&a);
            if (C_count && !(S_count || R_count))           //if C entered but no S/R
                not_valid(&a);

            //free allocated memory & close txt file
            line_arr_dtor(&a);
            fclose(a.fp);
            return 0;
        }    
        //some other character was entered at the beggining of the line
        not_valid(&a);
        break;
        }
    }
}


// FUNCTIONS

// MAIN FUNCTIONS = Array set up, Dynamic memory allocation

    // initiate line array
    void line_arr_ctor(line_arr *a)
    {
        a->len = 0;
        a->data = NULL;
    }
    // initiate item array
    void item_arr_ctor(set_t *s)
    {
        s->size=0;
        s->item=NULL;
        s->item_b=NULL;
        s->valid=false;
        s->is_set=false;
    }
    // increases line array by 1, if unsuccessful send to function alloc_error
    void line_arr_inc(line_arr *a)
    {
        set_t *p = realloc(a->data, (a->len+1)*sizeof(set_t));
        if (p == NULL)
                alloc_error(a);
        a->data = p;
        item_arr_ctor(&a->data[a->len]);                    //set up item array 
        a->len++;                                           //length ++
    }
    // increases item arrays (both a and b) by 1, returns NULL if unsuccessful
    void *item_arr_inc(set_t *item_array)
    {
        item_array->size++;                                                             //increase size
        char **p = realloc(item_array->item, (item_array->size)*sizeof(char**));    //increase item array
        if (p == NULL)
                return NULL;
        if (!item_array->is_set)
        {
            char **k = realloc(item_array->item_b, (item_array->size)*sizeof(char**));  //increase item array for item_b   
            if (k == NULL)
                return NULL;
            item_array->item_b=k;
        }
        return item_array->item=p;
    }
    // adds item(s) to an array
    // calls item_arr_inc to reallocate memory for new item, than copies temp items to allocated memory
    void item_ctor(const char *tmp_a,const char *tmp_b, line_arr *a)
    {
        if(item_arr_inc(&a->data[a->len-1]) == NULL)                    //increase item array and set it up
            alloc_error(a);
            
        int i = a->data[a->len-1].size;                                 //set i to be the size of item array(clarifies code)

        char * p = malloc((strlen(tmp_a)+1)*sizeof(char));              //malloc array for item
        if (p==NULL)
            alloc_error(a);

        strcpy(p, tmp_a);
        a->data[a->len-1].item[i-1] = p;

        if (tmp_b != NULL)                                              //if its relation malloc array for item b, NULL
        {
            char * k = malloc((strlen(tmp_b)+1)*sizeof(char));
            if (k == NULL)
                alloc_error(a);
            strcpy(k, tmp_b);
            a->data[a->len-1].item_b[i-1] = k;
        }
    }

//FUNCTIONS TO CLEAR MEMORY
    //frees items
    void item_arr_dtor(set_t *data)
    {
        for (int i = 0; i < data->size; i++)
            {
                free(data->item[i]);
                if (!data->is_set)
                    free(data->item_b[i]);
            }        
    }
    //free item array and line array
    void line_arr_dtor(line_arr *a)
    {
        for (int i = 0; i < a->len; i++)
            {
                item_arr_dtor(&a->data[i]);
                free(a->data[i].item);
                free(a->data[i].item_b);
            }
        free(a->data);
    }


//FUCTIONS to patch errors
    //universum cannot cotnain more than one same item, item cannot be named true/false or any name of programs function
    void is_valid_u(set_t *set)
    {
        for (int i = 0; i < set->size; i++)
        {
            if (banned_words(set->item[i]))                     //check for banned words
            {
                set->valid = false;
                return ;
            }
            
            for (int j = 0; set->item[i][j]; j++)               //item can only contain letters aA-zZ
            {
            if (  !((set->item[i][j] >= 'A' && set->item[i][j] <= 'Z') || (set->item[i][j] >= 'a' && set->item[i][j] <= 'z'))  )
                {
                set->valid = false;
                return ;
                } 
            }
            
            for (int k = i+1; k < set->size; k++)               //check for duplicates
            {
                if(!strcmp(set->item[i],set->item[k]))
                {
                    set->valid = false;
                    return;
                }
            }
        }
        set->valid = true;
    }

    //checks validity of a set
    //set mus only contain items declared in universum
    void is_valid(set_t *universum,set_t *set)
    {
        int count=0;
        for (int i = 0; i < set->size; i++)
        {
            
            for (int j = 0; j < universum->size; j++)
            {
                    if (set->is_set)                                                //when its set, compare item to universum
                    {
                        if (!strcmp(set->item[i],universum->item[j]))
                            count++;
                    }
                    else                                                            //when its relation, compare both item arrays
                    {
                        if (!strcmp(set->item_b[i],universum->item[j]))
                            count++;
                        if (!strcmp(set->item[i],universum->item[j]))
                            count++;
                    }
            }
        }
    //check if items are from universum and are not repeating
        if (set->is_set)                                                //if set, check if count == size(if every item found its image in universum)
        {
            if (count == set->size && set->size <= universum->size)     //also check if items are not repeating -> size <= universum.size
                set->valid = true;
        }
        else                                                            //if relation, check if count == 2*size
        {
            if (count == 2*set->size)
            {
                for (int i = 0; i < set->size; i++)
                {
                    for (int j = i+1; j < set->size; j++)
                    {
                        if(!strcmp(set->item[i],set->item[j]))
                        {
                            if (!strcmp(set->item_b[i],set->item_b[j]))
                                return;
                        }
                    }
                }
                set->valid = true;
            }         
        }
    }

    //checks universum items for banned words
    //returns true if item contains banned words
    bool banned_words(char *item)
    {
        if(!(strcmp(item,"true")))
            return true;
        if(!(strcmp(item,"TRUE")))
            return true;
        if(!(strcmp(item,"false")))
            return true;
        if(!(strcmp(item,"FALSE")))
            return true;
        if(!(strcmp(item,"empty")))
            return true;
        if(!(strcmp(item,"card")))
            return true;
        if(!(strcmp(item,"complement")))
            return true;
        if(!(strcmp(item,"union")))
            return true;
        if(!(strcmp(item,"subseteq")))
            return true;
        if(!(strcmp(item,"intersect")))
            return true;
        if(!(strcmp(item,"minus")))
            return true;
        if(!(strcmp(item,"subset")))
            return true;
        if(!(strcmp(item,"equals")))
            return true;
        if(!(strcmp(item,"relexive")))
            return true;
        if(!(strcmp(item,"symmetric")))
            return true;
        if(!(strcmp(item,"antisymmetric")))
            return true;
        if(!(strcmp(item,"transitive")))
            return true;
        if(!(strcmp(item,"function")))
            return true;
        if(!(strcmp(item,"domain")))
            return true;
        if(!(strcmp(item,"codomain")))
            return true;
        if(!(strcmp(item,"injective")))
            return true;
        if(!(strcmp(item,"surjective")))
            return true;
        if(!(strcmp(item,"bijective")))
            return true;
        return false;
    }

    //when alloc NULL is returned print error msg, close txt file and exit
    void alloc_error(line_arr *a)
    {
        fprintf(stderr,"\nError. Unable to allocate memory.");
        fclose(a->fp);
        line_arr_dtor(a);
        exit(-1);
    }

    //if line entered uncorrectly crash
    void not_valid(line_arr *a)
    {
        fprintf(stderr,"Error. Unexpected input");
        line_arr_dtor(a);
        fclose(a->fp);
        exit(-1);
    }
    //starting arguments entered uncorrectly 
    void usage()
    {
        fprintf(stderr, "Error. Program must start with txt file.");
        exit(-1);
    }
    //chcek arguemnt if its a txt file
    void check_txt(const char *file)
    {
        int i;
        for (i = 0; file[i]!='\0'; i++);
        if (file[i-1] == 't')
            if (file[i-2] == 'x')
                if (file[i-3] == 't')
                    if (file[i-4] == '.')
                        return;
        usage();
    }

//PERFORMANCE FUNCTIONS
//Command functions
//Calls function, based on function input scans the needed parameters, checks if parameters are valid and goes to correct function 
    void call_function(char *command, line_arr *a)
    {
        int line=0,line2=0,line3=0;
        if(!(strcmp(command,"empty")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set)
                    not_valid(a);
                empty(a->data[line-1]);
                return;
            } 
        if(!(strcmp(command,"card")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set)
                    not_valid(a);
                card(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"complement")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set)
                    not_valid(a);
                complement(a->data[0],a->data[line-1]);
                return;           
            } 
        if(!(strcmp(command,"union")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set|| !a->data[line2-1].is_set)
                    not_valid(a);
                s_union(a->data[line-1],a->data[line2-1]);
                return;            
            } 
        if(!(strcmp(command,"intersect")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set || !a->data[line2-1].is_set)
                    not_valid(a);
                intersect(a->data[line-1],a->data[line2-1]);
                return;            
            } 
        if(!(strcmp(command,"minus")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set || !a->data[line2-1].is_set)
                    not_valid(a);

                minus(a->data[line-1],a->data[line2-1]);
                return;            
            } 
        if(!(strcmp(command,"subseteq")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set || !a->data[line2-1].is_set)
                    not_valid(a);

                subseteq(a->data[line-1],a->data[line2-1]);
                return;            
            } 
        if(!(strcmp(command,"subset")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set || !a->data[line2-1].is_set)
                    not_valid(a);

                subset(a->data[line-1],a->data[line2-1]);    
                return;            
            } 
        if(!(strcmp(command,"equals")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF)
                    not_valid(a);
                
                if (!a->data[line-1].is_set || !a->data[line2-1].is_set )
                    not_valid(a);

                equals(a->data[line-1],a->data[line2-1]);
                return;            
            } 

        if(!(strcmp(command,"reflexive")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);

                reflexive(a->data[line-1],a->data[0]);
                return;            
            } 
        if(!(strcmp(command,"symmetric")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);

                symmetric(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"antisymmetric")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);

                antisymmetric(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"transitive")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);
                transitive(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"function")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);
                if(function(a->data[line-1]))
                    printf("true");
                else
                    printf("false");
                return;            
            } 
        if(!(strcmp(command,"domain")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);

                domain(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"codomain")))
            {
                fscanf(a->fp,"%d",&line);
                if (a->len < line || line < 1 || line == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set)
                    not_valid(a);
                codomain(a->data[line-1]);
                return;            
            } 
        if(!(strcmp(command,"injective")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                fscanf(a->fp,"%d",&line3);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF || a->len < line3 || line3 < 1 || line3 == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set || !a->data[line2-1].is_set || !a->data[line3-1].is_set)
                    not_valid(a);

                injective(a->data[line-1],a->data[line2-1],a->data[line3-1]);

                return;            
            } 
        if(!(strcmp(command,"surjective")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                fscanf(a->fp,"%d",&line3);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF || a->len < line3 || line3 < 1 || line3 == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set || !a->data[line2-1].is_set || !a->data[line3-1].is_set)
                    not_valid(a);

                surjective(a->data[line-1],a->data[line2-1],a->data[line3-1]);

                return;            
            } 
        if(!(strcmp(command,"bijective")))
            {
                fscanf(a->fp,"%d",&line);
                fscanf(a->fp,"%d",&line2);
                fscanf(a->fp,"%d",&line3);
                if (a->len < line || line < 1 || line == EOF || a->len < line2 || line2 < 1 || line2 == EOF || a->len < line3 || line3 < 1 || line3 == EOF)
                    not_valid(a);
                
                if (a->data[line-1].is_set || !a->data[line2-1].is_set || !a->data[line3-1].is_set)
                    not_valid(a);
                    
                bijective(a->data[line-1],a->data[line2-1],a->data[line3-1]);

                return;            
            } 
        not_valid(a);
    }

/* FUNCTIONS ON SETS */
    void empty(set_t set)
    {
        if(set.size == 0) // if the size of the set is zero, the set is empty
        {
            printf("true\n");
        } else 
        {
            printf("false\n");
        }
    }

    void card(set_t set)
    {
        printf("%d\n", set.size); // print the size of given set == the number of items in it
    }

    void complement(set_t universum, set_t set)
    {
        printf("S");

        int different;
        for(int i = 0; i < universum.size; i++) // checks each item from the universum
        {
            different = 1; // assumes the item is different than any in the given set
            for(int j = 0; j < set.size; j++)
            {
                if(strcmp(universum.item[i], set.item[j]) == 0) // if it finds the item we are checking in the given set...
                {
                    different = 0; // ...it changes the value to zero...
                    break;
                }
            }
            if(different == 1)
            {
                printf(" %s", universum.item[i]); // ...and prints the item
            }
        }

        printf("\n");
    }


    void s_union(set_t set_1, set_t set_2) // since union is a data type, the function name must be a little different
    {
        printf("S");
        for(int i = 0; i < set_1.size; i++)
        {
            printf(" %s", set_1.item[i]); // prints every item in the set_1
        }

        int different;
        for(int j = 0; j < set_2.size; j++) // we are checking every item from set_2...
        {
            different = 1;
            for(int k = 0; k < set_1.size; k++) // ...to every item from set_1
            {
                if(strcmp(set_2.item[j], set_1.item[k]) == 0) // if the item in both sets is the same, change value of different to 0
                {
                    different = 0;
                    break;
                }
            }
            if(different == 1)
            {
                printf(" %s", set_2.item[j]); // if the item from set_2 is different, print it
            }
        }

        printf("\n");
    }

    void intersect(set_t set_1, set_t set_2)
    {
        printf("S");
        
        for(int i = 0; i < set_1.size; i++)
        {
            for(int j = 0; j < set_2.size; j++)
            {
                if(strcmp(set_1.item[i], set_2.item[j]) == 0) // goes through every item from set_1 and set_2 and if it finds same item...
                {
                    printf(" %s", set_1.item[i]); // ...it prints it
                    break;
                }
            }
        }
        printf("\n");
    }

    void minus(set_t  set_1, set_t set_2) //rozdil mnozin
    {
        printf("S");//vypis 'set'
        for(int i=0;i<set_1.size;i++)//cyklus pro indexy prvniho retezce
            {
                bool is_in_both = false;//nastaveni promene existence v obou mnozinach; na zacatku cyklu

                for(int j=0;j<set_2.size;j++)//cyklus pro indexy druheho retezce
                    {
                        if(strcmp(set_1.item[i], set_2.item[j]) == 0)//podminka porovnani dvou prvku; projde pokud jsou stejne
                        {
                            is_in_both = true;//nastaveni promene existence v obou mnozinach na true
                            break;//prvek nalezen netreba prochazet zbytek cyklu
                        }
                    }

                if(is_in_both != true)//podminka existence v obou mnozinach;projde pokud nebyl nalezen stejny prvek
                {
                    printf(" %s", set_1.item[i]);//vypise prvek
                }

            }
        printf("\n");//konec radku
    }

    void subseteq(set_t  set_1, set_t set_2) //odpoved jestli je A podmnozinou B
    {
        bool subset;//nastaveni promene jestli je podmnozinou

        for(int i=0;i<set_1.size;i++)//cyklus pro indexy prvniho retezce
            {
                subset = false;//nastaveni promene jestli je podmnozinou na false; na zacatku kazdeho retezce, tedy pro kazdy prvek z A
                for(int j=0;j<set_2.size;i++)//cyklus pro indexy druheho retezce
                {
                    if (strcmp(set_1.item[i], set_2.item[j]) == 0)//podminka porovnani dvou prvku; projde pokud jsou stejne
                    {
                        subset = true;//nastaveni promene jestli je podmnozinou na true; "pro tento prvek je true"
                        break;//netreba
                    }
                }
                if (subset == false)//K prvku z A nebyl nalezen prvek z B, tim padem se subset nezmenil
                {
                    printf("false\n");//vypis
                    break;//neni potreba kontrolovat pro dalsi
                }
                if(i==set_1.size)//stane se na posledni smycce
                {
                    printf("true\n");//vypis
                }
            }
    }

    void subset(set_t  set_1, set_t set_2) //odpoved jestli je A vlastni podmnozina B
    {
        int count;//pocet stejnych prvku
        bool subset;//nastaveni promene jestli je podmnozinou

    if(set_1.size==set_2.size)//podminka jestli jsou mnoziny stejne velke
        {
        for(int i=0;i<set_1.size;i++)//cyklus pro indexy prvniho retezce
            {
                subset = false;//nastaveni promene jestli je podmnozinou na false; na zacatku kazdeho retezce, tedy pro kazdy prvek z A
                for(int j=0;j<set_2.size;i++)//cyklus pro indexy druheho retezce
                {
                    if (strcmp(set_1.item[i], set_2.item[j]) == 0)//podminka porovnani dvou prvku; projde pokud jsou stejne
                    {
                        subset = true;//nastaveni promene jestli je podmnozinou na true; "pro tento prvek je true"
                        count++;//nalezen stejny prvek -> pocet se zvysi
                        break;
                    }
                }
                if ((subset == false) || (count = set_2.size))//K prvku z A nebyl nalezen prvek z B nebo jsou vsechny prvky stejne
                {
                    printf("false\n");//vypis
                    break;//neni potreba kontrolovat pro dalsi
                }
                if(i==set_1.size)//stane se na posledni smycce
                {
                    printf("true\n");//vypis
                }
            }
        }
        else
        {
            printf("false\n");//vypis
        }
    }

    void equals (set_t  set_1, set_t set_2) //odpoved jestli jsou si mnoziny rovny
    {
        int count;//pocet stejnych prvku
        bool subset;//nastaveni promene jestli je podmnozinou

    if(set_1.size==set_2.size)//podminka jestli jsou mnoziny stejne velke
        {
        for(int i=0;i<set_1.size;i++)//cyklus pro indexy prvniho retezce
            {
                subset = false;//nastaveni promene jestli je podmnozinou na false; na zacatku kazdeho retezce, tedy pro kazdy prvek z A
                for(int j=0;j<set_2.size;i++)//cyklus pro indexy druheho retezce
                {
                    if (strcmp(set_1.item[i], set_2.item[j]) == 0)//podminka porovnani dvou prvku; projde pokud jsou stejne
                    {
                        subset = true;//nastaveni promene jestli je podmnozinou na true; "pro tento prvek je true"
                        count++;//nalezen stejny prvek -> pocet se zvysi
                        break;
                    }
                }
                if (subset == false)//K prvku z A nebyl nalezen prvek z B
                {
                    printf("false\n");
                    break;//konec pokud ne
                }
            if (count==set_2.size)//stane se na posledni smycce
                {
                    printf("true\n");//vypis
                }
            }

        }
    }




/* FUNCTIONS ON RELATIONS */

    void reflexive(set_t relation, set_t universum)//odpoved jestli je relace reflexivni
        {
            bool reflexive;//nastaveni promene reflexivity
            int count;//nastaveni promene poctu reflexivnich prvku
            for(int i=0;i<universum.size;i++)//cyklus pro indexy prvkù v univerzu
                {
                    reflexive=false;//nastaveni promene reflexivity na false
                    for(int j=0;j<relation.size;i++)//cyklus pro indexy prvkù v relacích
                    {
                        if(universum.item[i]==relation.item[j])//podmínka prvku v univerzu
                        {
                            if(relation.item[j]==relation.item_b[j])//podmínka stejných prvkù v relaci
                                {
                                    reflexive=true;//nastaveni reflexivity prvku na true
                                    count++;//nasel se dalsi reflexini prvke; zvysi se pocitatel
                                    break;
                                }
                        }
                    }
                    if (reflexive==false)//pokud zustal na false nenasli jsme prvek
                    {
                        printf("false");//vypis
                        break;//konec pokud ne
                    }
                    if (count==universum.size)//stane se na posledni smycce
                    {
                        printf("frue");//vypis
                        break;//konec pokud ano
                    }
                }
        }

    void symmetric(set_t relation)//odpoved jestli je relace symetricka
        {
            bool symmetric;//nastaveni promene symetricnosti
            int count;//nastani promene pocitatele
            for(int i=0;i<relation.size;i++)//cyklus pro indexy prvniho prvku relace
                {
                    symmetric=false;//symetricnost se nastavi na false na zacatku kazdeho cyklu
                    for(int j=0;j<relation.size;i++)//cyklus pro indexy druheho prvku relace
                        {
                        if(relation.item[i]==relation.item_b[j])//podminka rovnajiciho se prvku u jine relace
                            {
                            if(relation.item[j]==relation.item_b[i])//podminka rovnajicich se prvku u prohozene relace
                                {
                                    symmetric=true;//symetricnost se nastavi na true
                                    count++;//nasel se dalsi symetricky prvek; zvysi se pocitatel
                                    break;
                                }
                            }
                        }
                    if(symmetric==false)//pokud zustal na false nenasli jsme prvek
                    {
                        printf("false");//vypis
                        break;
                    }
                    if(count==relation.size)
                    {
                        printf("true");//vypis
                        break;
                    }
                }
        }
    void antisymmetric(set_t relation) //odpoved jestli je relace antisymetricka
        {
            bool symmetric;//nastaveni promene symetricnosti
            for(int i=0;i<relation.size;i++)//cyklus pro indexy prvniho prvku relace
                {
                    symmetric=true;//symetricnost se nastavi na false na zacatku kazdeho cyklu
                    for(int j=0;j<relation.size;i++)//cyklus pro indexy druheho prvku relace
                {
                    {
                        if(relation.item[i]!=relation.item_b[i])
                        {
                            if(relation.item[i]==relation.item_b[j])//podminka rovnajiciho se prvku u jine relace
                                {
                                    if(relation.item[j]==relation.item_b[i])//podminka rovnajicich se prvku u prohozene relace
                                        {
                                            symmetric=false
                                            ;//symetricnost se nastavi na true
                                            break;
                                        }
                                }
                        }
                    }
                    if (symmetric==false)//pokud se nastavil na false nasli jsme symetricky prvek
                    {
                        printf("false");//vypis
                        break;
                    }
                    if(i==relation.size-1)
                    {
                        printf("true");//vypis
                        break;
                    }
                }
            }
        }
    void transitive(set_t relation) //odpoved jestli je relace tranzitivni
        {
            bool transitive;//nastaveni promene symetricnosti
            for(int i=0;i<relation.size;i++)//cyklus pro indexy prvniho prvku relace
                {
                    transitive=false;//symetricnost se nastavi na false na zacatku kazdeho cyklu
                    for(int j=0;j<relation.size;i++)//cyklus pro indexy druheho prvku relace
                    {
                        for(int k=0;k<relation.size;i++)
                        {
                            if(relation.item_b[i]==relation.item[j])
                                {
                                    if(relation.item[i]==relation.item[k])//podminka rovnajiciho se prvku u jine relace
                                        {
                                            if(relation.item_b[k]==relation.item_b[j])//podminka rovnajicich se prvku u prohozene relace
                                            {
                                                transitive=true;//symetricnost se nastavi na true
                                                break;
                                            }
                                        }
                                }
                        }
                        if (transitive==false)//pokud se nenastavil na true nenasli jsme transitivni prvek
                        {
                            printf("false");//vypis
                            break;
                        }
                        if(i==relation.size-1)
                        {
                            printf("true");//vypis
                            break;
                        }
                    }
            }
        }

        bool function(set_t relation)//vraci True nebo False podle toho jestli je funkce
        {
            for(int i=0;i<relation.size;i++)//cyklus pro indexy prvni relace
            {
                for(int j=0;j<relation.size;j++)//cyklus pro indexy druhe relace
                {
                        if(relation.item[i]==relation.item[j])//pokud prvni prvek je stejny
                        {
                            if(relation.item_b[i]!=relation.item_b[j])//pokud je druhy prvek jiny
                            {
                                return false;//vraci false
                            }
                        }
                }
                    if (i==relation.size-1)//stane se na posledni smycce
                    {
                        return true;//vraci true
                    }
            }
        return true;
        }

    void domain(set_t relation)
    {
        printf("S");

        bool same;
        for(int i = 0; i < relation.size; i++) // goes through every "first item" from relation
        {
            same = false;
            for(int j = 0; j < i; j++) // checks if the curent item was alredy printed
            {
                if(strcmp(relation.item[i], relation.item[j]) == 0)
                {
                    same = true; // changes the value of same and breaks
                    break;
                }
            }
            if(!same)
            {
                printf(" %s", relation.item[i]); // prints only, if the item wasn't alredy printed
            }
        }

        printf("\n");
    }

    void codomain(set_t relation)
    {
        printf("S");

        bool same;
        for(int i = 0; i < relation.size; i++) // goes through every "second item" from relation
        {
            same = false;
            for(int j = 0; j < i; j++) // checks if the curent item was alredy printed
            {
                if(strcmp(relation.item_b[i], relation.item_b[j]) == 0)
                {
                    same = true; // changes the value of same and breaks
                    break;
                }
            }
            if(!same)
            {
                printf(" %s", relation.item_b[i]); // prints only, if the item wasn't alredy printed
            }
        }

        printf("\n");
    }


    void injective(set_t relation, set_t set_1, set_t set_2)
    {
        if(function(relation) == false) // checks if the relation is function
        {
            printf("false\n");
            return;
        }

        bool is_in;
        for(int i = 0; i < relation.size; i++)
        {
            is_in = false;
            for(int j = 0; j < set_1.size; j++)
            {
                if(strcmp(relation.item[i], set_1.item[j]) == 0) // checks if every "first item" from relation is in the set A
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }
        for(int k = 0; k < relation.size; k++)
        {
            is_in = false;
            for(int l = 0; l < set_2.size; l++)
            {
                if(strcmp(relation.item_b[k], set_2.item[l]) == 0) // checks if every "second item" from relation is in the set B
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }

        // injective means, that every "a" has different "b" => every "b" (second item from relation) must be there only once and every "first item" must be used
        if(set_1.size != relation.size) // checks that every item from A is present
        {
            printf("false\n");
            return;
        }
        int same;
        for(int m = 0; m < relation.size; m++)
        {
            same = 0;
            for(int n = 0; n < relation.size; n++)
            {
                if(strcmp(relation.item_b[m], relation.item_b[n]) == 0) // if it finds same string in the item_b array..
                {
                    same++; // ...it adds to this variable (by logic, this variable will always be at least 1)
                }
            }
            if(same != 1) // if there is different number of same items than 1, print false and return
            {
                printf("false\n");
                return;
            }
        }

        printf("true\n");
    }

    void surjective(set_t relation, set_t set_1, set_t set_2)
    {
        if(function(relation) == false) // checks if the relation is function
        {
            printf("false\n");
            return;
        }

        bool is_in;
        for(int i = 0; i < relation.size; i++)
        {
            is_in = false;
            for(int j = 0; j < set_1.size; j++)
            {
                if(strcmp(relation.item[i], set_1.item[j]) == 0) // checks if every "first item" from relation is in the set A
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }
        for(int k = 0; k < relation.size; k++)
        {
            is_in = false;
            for(int l = 0; l < set_2.size; l++)
            {
                if(strcmp(relation.item_b[k], set_2.item[l]) == 0) // checks if every "second item" from relation is in the set B
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }

        // surjective means, that every item from set B has to be in relation with some item from A
        // that means that every item from B must be a "second item" in the given relation
        int same;
        for(int m = 0; m < set_2.size; m++)
        {
            same = 0;
            for(int n = 0; n < relation.size; n++)
            {
                if(strcmp(set_2.item[m], relation.item_b[n]) == 0) // checks if every the item is the same
                {
                    same++;
                }
            }
            if(same == 0) // there must be at least one "same item" in the relation, otherwise it is not surjective
            {
                printf("false\n");
                return;
            }
        }

        printf("true\n");
        
    }

    void bijective(set_t relation, set_t set_1, set_t set_2)
    {
        if(function(relation) == false) // checks if the relation is function
        {
            printf("false\n"); 
            return;
        }
        
        bool is_in;
        for(int i = 0; i < relation.size; i++)
        {
            is_in = false;
            for(int j = 0; j < set_1.size; j++)
            {
                if(strcmp(relation.item[i], set_1.item[j]) == 0) // checks if every "first item" from relation is in the set A
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }
        for(int k = 0; k < relation.size; k++)
        {
            is_in = false;
            for(int l = 0; l < set_2.size; l++)
            {
                if(strcmp(relation.item_b[k], set_2.item[l]) == 0) // checks if every "second item" from relation is in the set B
                {
                    is_in = true;
                }
            }
            if(!is_in)
            {
                printf("false\n"); // if not, return false
                return;
            }
        }

        // bijective means, that the function is "perfect" - every item has its counterpart from the other set =>
        // => the function must be injective AND surjective
        
        // checks if it is injective:
        if(set_1.size != relation.size) // checks that every item from A is present
        {
            printf("false\n");
            return;
        }
        int same_i;
        for(int m = 0; m < relation.size; m++)
        {
            same_i = 0;
            for(int n = 0; n < relation.size; n++)
            {
                if(strcmp(relation.item_b[m], relation.item_b[n]) == 0)
                {
                    same_i++;
                }
            }
            if(same_i != 1)
            {
                printf("false\n"); // the condition is broken => it is not injective => return false
                return; 
            }
        }

        // checks if it is surjective:
        int same_s;
        for(int m = 0; m < set_2.size; m++)
        {
            same_s = 0;
            for(int n = 0; n < relation.size; n++)
            {
                if(strcmp(set_2.item[m], relation.item_b[n]) == 0)
                {
                    same_s++;
                }
            }
            if(same_s == 0)
            {
                printf("false\n"); // the condition is broken => it is not surjective => return false
                return;
            }
        }

        printf("true\n"); // all conditions passed, it is bijective, print true
    }