/***********
 ID:322264748
 NAME:Shani Attiya
 EX5
***********/
#define MAX_TENS 5
#define MAX_HOURS 99
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode
{
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season
{
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow
{
    char *name;
    Season *seasons;
} TVShow;

TVShow ***database = NULL;
int dbSize = 0;

char *getString(); // Function to get a dynamic string from user input
int getInt();

int validLength(char *s);
int countShows();

void shrinkDB();
void expandDB(); // Function to expand the database

void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);
void freeAll();

TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

void addShow();   // Function to add a new TV show
void addSeason(); // Function to add a new season to a TV show
void addEpisode();

void deleteShow();
void deleteSeason();
void deleteEpisode();

void printEpisode();
void printShow();
void printArray();

int positionOfSeason(char *name);
// Function Implementations are here!

//helpert functions
int validLength(char *s)
{
    int len = strlen(s);
    if (len != 8)
    {
        return 0;
    }
    if (s[2] != ':' || s[5] != ':')
    {
        return 0;
    }
    if (s[3] > MAX_TENS || s[6] > MAX_TENS) // tens place for minutes and seconds cannot exceed 5
    {
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        if (i == 2 || i == 5)
        {
            continue;
        }
        if (s[i] < '0' || s[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}
char *getString()
{
    int startSize = 10;
    int length = 0;
    char *size = malloc(startSize * sizeof(char)); // initial allocation

    if (size == NULL)
    {
        exit(1);
    }

    char c = 0;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (length == startSize - 1)
        {
            startSize *= 2;
            size = realloc(size, startSize * sizeof(char));
            if (size == NULL)
            {
                free(size);
                exit(1);
            }
        }
        size[length] = c;
        length++;
    }
    size[length] = '\0';
    return size;
}
void expandDB()
{
    int newSize = dbSize + 1;
    database = realloc(database, newSize * sizeof(TVShow **));
    if (database == NULL)
    {
        exit(1);
    }
    for (int i = 0; i < dbSize; i++)
    {
        database[i] = realloc(database[i], newSize * sizeof(TVShow *));
        if (database[i] == NULL)
        {
            exit(1);
        }
        database[i][newSize - 1] = NULL;
    }

    database[newSize - 1] = (TVShow **)malloc(newSize * sizeof(TVShow *));
    if (database[newSize - 1] == NULL)
    {
        exit(1);
    }

    for (int j = 0; j < newSize; j++)
    {
        database[newSize - 1][j] = NULL;
    }
    dbSize = newSize;
}
void shrinkDB()
{
    int newSize = dbSize - 1;
    for (int i = 0; i < newSize; i++)
    {
        database[i] = realloc(database[i], newSize * sizeof(TVShow *));
        if (database[i] == NULL)
        {
            exit(1);
        }
    }
    database = realloc(database, newSize * sizeof(TVShow **));
    if (database == NULL)
    {
        exit(1);
    }
    dbSize = newSize;
}
int countShows()
{
int counter=0;
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                counter++;
            }
        }
    }
    return counter; 
}

//FINDING FUNCTIONS
TVShow *findShow(char *name)
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL && strcmp(database[i][j]->name, name) == 0)
            {
                return database[i][j];
            }
        }
    }
    return NULL;
}
Season *findSeason(TVShow *show, char *name)
{

    if (show->seasons == NULL)
    {
        return NULL;
    }
    Season *iterator = show->seasons;
    while (iterator != NULL)
    {
        if (strcmp(iterator->name, name) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}
Episode *findEpisode(Season *season, char *name)
{if (season->episodes == NULL)
    {
        return NULL;
    }
    Episode *iterator = season->episodes;
    while (iterator != NULL)
    {
        if (strcmp(iterator->name, name) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}
//FREE FUNCTIONS
void freeShow(TVShow *showDel)
{
    if (showDel == NULL)
    {
        return;
    }
    free(showDel->name);
    Season *curSeason = showDel->seasons;
    while (curSeason != NULL)
    {
        Season *nextSeason = curSeason->next;
        freeSeason(curSeason);
        curSeason = nextSeason;
    }
    free(showDel);
}
void freeSeason(Season *seasonDel)
{
    if (seasonDel == NULL)
    {
        return;
    }
    free(seasonDel->name);
    Episode *curEpisode = seasonDel->episodes;
    while (curEpisode != NULL)
    {
        Episode *nextEpisode = curEpisode->next;
        freeEpisode(curEpisode);
        curEpisode = nextEpisode;
    }
    free(seasonDel);
}
void freeEpisode(Episode *episodeDel)
{
    if (episodeDel == NULL)
    {
        return;
    }
    free(episodeDel->name);
    free(episodeDel->length);
    free(episodeDel);
}

//ADDING FUNCTIONS
void addShow()
{
    printf("Enter the name of show:\n");
    char *showName = getString();
    if (findShow(showName) != NULL)
    {
        printf("Show already exists.\n");
        free(showName);
        return;
    }
    else
    {
        printf("Adding show: %s\n", showName);

        for (int i = 0; i < dbSize; i++)
        {
            for (int j = 0; j < dbSize; j++)
            {
                if (database[i][j] == NULL)
                {
                    database[i][j] = (TVShow *)malloc(sizeof(TVShow));
                    database[i][j]->name = showName;
                    database[i][j]->seasons = NULL;
                    printf("Show added successfully.\n");
                    return;
                }
            }
        }
    }

    expandDB();
    TVShow *newShow = (TVShow *)malloc(sizeof(TVShow));
    if (newShow == NULL)
    {
        exit(1);
    }
    newShow->name = showName;
    newShow->seasons = NULL;
    database[dbSize - 1][dbSize - 1] = newShow;
    printf("Show added successfully.\n");
}
void addSeason()
{
    printf("enter the name of the show:\n");
    char *showName = getString();

    TVShow *curShow = findShow(showName);
    if (curShow == NULL)
    {
        printf("Show does not exist.\n");
        free(showName);
        return;
    }
    free(showName);

    printf("Enter the name of season:\n");
    char *seasonName = getString();
    if (findSeason(curShow, seasonName) != NULL)
    {
        printf("Season already exists.\n");
        free(seasonName);
        return;
    }

    Season *newSeason = (Season *)malloc(sizeof(Season));
    if (newSeason == NULL)
    {
        free(seasonName);
        exit(1);
    }

    newSeason->name = seasonName;
    newSeason->episodes = NULL;
    newSeason->next = NULL;

    printf("Enter the position\n");
    char *strPos = getString();
    int numPos = 0;

    if (strPos != NULL)
    {
        numPos = atoi(strPos);
        free(strPos);
    }
    else
    {
        free(newSeason->name);
        free(newSeason);
        return;
    }

    if (numPos < 1)
    {
        printf("Invalid position.\n");
        free(newSeason->name);
        free(newSeason);
        return;
    }
    // Adding at the beginning bacause position is 1 or list is empty
    if (numPos == 1)
    {
        newSeason->next = curShow->seasons;
        curShow->seasons = newSeason;
        printf("Season added successfully.\n");
        return;
    }
    if (curShow->seasons == NULL) 
    {
        printf("List is empty, can only add at position 1.\n");
        free(newSeason->name);
        free(newSeason);
        return;
    }

    Season *iterator = curShow->seasons;
    for (int index = 0; index < numPos-2; index++)
    {
        if (iterator == NULL || iterator->next == NULL) 
        {
           printf("Position out of bounds.\n");
           free(newSeason->name);
           free(newSeason);
           return;
        }
        
        iterator = iterator->next;
    }

    if (iterator == NULL) {
        printf("Position out of bounds.\n");
        free(newSeason->name);
        free(newSeason);
        return;
    }
    newSeason->next = iterator->next;
    iterator->next = newSeason;
    
    printf("Season added successfully.\n");
}
void addEpisode()
{
  printf("enter the name of the show:\n");
    char *showName = getString();
    TVShow *curShow = findShow(showName);

    if (curShow == NULL) // check if show exists
    {
        printf("Show does not exist.\n");
        free(showName);
        return;
    }
    free(showName);

    printf("Enter the name of season:\n"); // check if season exists
    char *seasonName = getString();
    Season *curSeason = findSeason(curShow, seasonName);

    if (curSeason == NULL)
    {
        printf("Season does not exist.\n");
        free(seasonName);
        return;
    }
   free(seasonName);
      
   printf("Enter the name of the episode\n"); 
    char *episodeName = getString();
    Episode *curEpisode = findEpisode(curSeason, episodeName);
 if (curEpisode!=NULL)
  {
     printf("Episode already exists\n");
     free(episodeName);
     return;
    }
 
    Episode *newEpisode = (Episode *)malloc(sizeof(Episode));
    if (newEpisode == NULL)
    {
        free(episodeName);
        exit(1);
    }

    newEpisode->name = episodeName;
    newEpisode->next = NULL;

    printf("Enter the length (xx:xx:xx)\n");
    char *length = getString();
    if (!validLength(length))
    {
        printf("Invalid length format.\n");
        free(length);
        free(newEpisode->name);
        free(newEpisode);
        return;
    }

    newEpisode->length = length;

    printf("Enter the position\n");
    char *strPos = getString();
    int numPos = 0;

    if (strPos != NULL)
    {
        numPos = atoi(strPos);
        free(strPos);
    }
    else
    {
        free(newEpisode->length);
        free(newEpisode->name);
        free(newEpisode);
        return;
    }

    if (numPos < 1)
    {
        printf("Invalid position.\n");
        free(newEpisode->length);
        free(newEpisode->name);
        free(newEpisode);
        return;
    }
    // Adding at the beginning bacause position is 1 or list is empty
    if (numPos == 1)
    {
        newEpisode->next = curSeason->episodes;
        curSeason->episodes = newEpisode;
        printf("Episode added successfully.\n");
        return;
    }

    Episode *iterator = curSeason->episodes;
    for (int index = 0; index < numPos - 2; index++)
    {
       if (iterator == NULL || iterator->next == NULL)//checking bounds
        {
             printf("Position out of bounds.\n"); 
             free(newEpisode->length);
             free(newEpisode->name);
             free(newEpisode);
             return;
        }

        iterator = iterator->next;
    }
     
    if (iterator == NULL)  // extra check in case empty list
    {
        printf("Position out of bounds.\n");
        free(newEpisode->length);
        free(newEpisode->name);
        free(newEpisode);
        return;
    }

    newEpisode->next = iterator->next;
    iterator->next = newEpisode;

    printf("Episode added successfully.\n");

}

//DELETING FUNCTIONS
void deleteShow()
{
printf("enter the name of the show:\n");
    char *showName = getString();
    TVShow *curShow = findShow(showName);
    if (curShow == NULL)
    {
        printf("Show does not exist.\n");
        free(showName);
        return;
    }
    free(showName);
    freeShow(curShow);
for (int i = 0; i < dbSize; i++)
{
    for (int j = 0; j < database; j++)
    {
        if (database[i][j] == curShow)
     {
        database[i][j]= NULL;
        if (countShows() < (dbSize - 1) * (dbSize - 1))
        {
            shrinkDB();
        }
     }
    }
    
    
}

    printf("Show deleted successfully.\n");
    return
}
void deleteSeason(){}
void deleteEpisode(){}
//PRINTING FUNCTIONS






void addMenu()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        addShow();
        break;
    case 2:
        addSeason();
        break;
    case 3:
        addEpisode();
        break;
    }
}

void deleteMenu()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        deleteShow();
        break;
    case 2:
        deleteSeason();
        break;
    case 3:
        deleteEpisode();
        break;
    }
}

void printMenuSub()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        printShow();
        break;
    case 2:
        printEpisode();
        break;
    case 3:
        printArray();
        break;
    }
}

void mainMenu()
{
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

int main()
{
    int choice;
    do
    {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            addMenu();
            break;
        case 2:
            deleteMenu();
            break;
        case 3:
            printMenuSub();
            break;
        case 4:
            freeAll();
            break;
        }
    } while (choice != 4);
    return 0;
}
