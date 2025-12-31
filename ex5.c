/***********
 ID:322264748
 NAME:Shani Attiya
 EX5
***********/
#define MAX_TENS '5'
#define MAX_HOURS '99'
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
int countShows();                  // Function to count the number of shows in the database
int countSeasons(TVShow *show);    // Function to count the number of seasons in a show
int countEpisodes(Season *season); // Function to count the number of episodes in a season

void shrinkDB(); // Function to shrink the database
void expandDB(); // Function to expand the database
void bubbleSort();

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

TVShow *getShowFromUser();
int positionOfSeason(char *name);
// Function Implementations are here!

// helpert functions
TVShow *getShowFromUser()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *curShow = findShow(showName);

    free(showName);

    if (curShow == NULL)
    {
        printf("Show not found.\n");
        return NULL; // if fail
    }

    return curShow; // success
}
int validLength(char *str)
{
    int len = strlen(str);
    if (len != 8)
    {
        return 0;
    }
    if (str[2] != ':' || str[5] != ':')
    {
        return 0;
    }
    if (str[3] > MAX_TENS || str[6] > MAX_TENS) // tens place for minutes and seconds cannot exceed 5
    {
        return 0;
    }

    return 1;
}
char *getString()
{
    int startSize = 10;
    int length = 0;
    char *size = (char *)calloc(startSize, sizeof(char)); // initial allocation

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
            char *temp = realloc(size, startSize * sizeof(char));
            if (temp == NULL)
            {
                free(size);
                exit(1);
            }
            size = temp;
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

    database[newSize - 1] = (TVShow **)calloc(newSize, sizeof(TVShow *));
    if (database[newSize - 1] == NULL)
    {
        exit(1);
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
void bubbleSort()
{
    int count = 0;
    int dbToal = dbSize * dbSize;

    // creating 1 dim array to make my life easier
    TVShow **tempArr = (TVShow **)calloc(dbToal, sizeof(TVShow *));

    if (tempArr == NULL) // making sure malloc didn't fail
    {
        exit(1);
    }

    // puting all elements in this 1dim array
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {

                tempArr[count] = database[i][j];
                count++;
            }

            database[i][j] = NULL;
        }
    }
    // sorting!
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++) // count-i is basically eliminating how many cells are left
        {
            if (tempArr[j] != NULL && tempArr[j + 1] != NULL)
            {
                if (strcmp(tempArr[j]->name, tempArr[j + 1]->name) > 0)
                {
                    TVShow *temp = tempArr[j];
                    tempArr[j] = tempArr[j + 1];
                    tempArr[j + 1] = temp;
                }
            }
        }
    }
    // now back to the original!

    int track = 0;
    for (int row = 0; row < dbSize; row++)
    {
        for (int col = 0; col < dbSize; col++)
        {
            if (track < count)
            {
                database[row][col] = tempArr[track];
                track++;
            }
            else
            {

                database[row][col] = NULL;
            }
        }
    }
    free(tempArr);
    return;
}

// COUNTING FUNCTIONS
int countShows()
{
    int counter = 0;
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
int countSeasons(TVShow *show)
{
    int counter = 0;
    Season *iterator = show->seasons;
    while (iterator != NULL)
    {
        counter++;
        iterator = iterator->next;
    }
    return counter;
}
int countEpisodes(Season *season)
{
    int counter = 0;
    Episode *iterator = season->episodes;
    while (iterator != NULL)
    {
        counter++;
        iterator = iterator->next;
    }
    return counter;
}

// FINDING FUNCTIONS
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

    if (show == NULL || show->seasons == NULL)
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
{
    if (season->episodes == NULL)
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

// FREE FUNCTIONS
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
void freeAll()
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                freeShow(database[i][j]);
            }
        }
        free(database[i]);
    }
    free(database);
}
// ADDING FUNCTIONS
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

    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == NULL)
            {
                database[i][j] = (TVShow *)calloc(1, sizeof(TVShow));
                if (database[i][j] == NULL)
                {
                    exit(1);
                }

                database[i][j]->name = showName;
                database[i][j]->seasons = NULL;

                printf("Show added successfully.\n");
                bubbleSort();
                return;
            }
        }
    }

    expandDB();

    TVShow *newShow = (TVShow *)calloc(1, sizeof(TVShow));
    if (newShow == NULL)
    {
        exit(1);
    }

    newShow->name = showName;
    newShow->seasons = NULL;

    database[dbSize - 1][dbSize - 1] = newShow;

    printf("Show added successfully.\n");
    bubbleSort();
}
void addSeason()
{
    TVShow *curShow = getShowFromUser();
    if (curShow == NULL)
        return;

    printf("Enter the name of the season:\n");
    char *seasonName = getString();
    Season *curSeason = findSeason(curShow, seasonName);

    if (curSeason != NULL)
    {
        printf("Season already exists.\n");
        free(seasonName);

        return;
    }

    Season *newSeason = (Season *)calloc(1, sizeof(Season));
    if (newSeason == NULL)
    {
        freeSeason(curShow->seasons);

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
        printf("Position 2: %d\n", numPos);
        return;
    }

    // Adding at the beginning bacause position is atleast 0 or list is empty
    if (numPos == 0 || curShow->seasons == NULL) // recent
    {
        newSeason->next = curShow->seasons;
        curShow->seasons = newSeason;
        printf("Season added successfully.\n");
        return;
    }

    Season *iterator = curShow->seasons;
    for (int index = 0; index < numPos; index++)
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

    if (iterator == NULL)
    {
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
    TVShow *curShow = getShowFromUser();
    if (curShow == NULL)
        return;

    printf("Enter the name of season:\n"); // check if season exists
    char *seasonName = getString();
    Season *curSeason = findSeason(curShow, seasonName);

    if (curSeason == NULL)
    {
        printf("Season does not exist.\n");
        free(seasonName);
        return;
    }

    printf("Enter the name of the episode\n");
    char *episodeName = getString();

    Episode *curEpisode = findEpisode(curSeason, episodeName);
    if (curEpisode != NULL)
    {
        printf("Episode already exists\n");
        char *trashLength = getString();
        char *trashPos = getString();
        free(trashLength);
        free(trashPos);
        free(episodeName);
        return;
    }

    Episode *newEpisode = (Episode *)malloc(sizeof(Episode));
    if (newEpisode == NULL)
    {
        free(newEpisode);
        exit(1);
    }

    newEpisode->name = episodeName;
    newEpisode->next = NULL;

    printf("Enter the length (xx:xx:xx)\n");
    char *length = getString();
    while (!validLength(length))
    {
        printf("Invalid length, enter again:\n");
        free(length);
        length = getString();
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

    // Adding at the beginning bacause position is 1 or list is empty
    if (numPos == 0 || curSeason->episodes == NULL) // recent
    {
        newEpisode->next = curSeason->episodes;
        curSeason->episodes = newEpisode;
        printf("Episode added successfully.\n");
        return;
    }

    Episode *iterator = curSeason->episodes;
    for (int index = 0; index < numPos; index++)
    {
        if (iterator == NULL || iterator->next == NULL) // checking bounds
        {
            printf("Position out of bounds.\n");
            free(newEpisode->length);
            free(newEpisode->name);
            free(newEpisode);
            return;
        }

        iterator = iterator->next;
    }

    if (iterator == NULL) // extra check in case empty list
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

// DELETING FUNCTIONS
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
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == curShow)
            {
                database[i][j] = NULL;
                if (countShows() < (dbSize - 1) * (dbSize - 1))
                {
                    shrinkDB();
                }
            }
        }
    }

    printf("Show deleted successfully.\n");
    return;
}
void deleteSeason()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    if (!showName)
    {
        return;
    }

    TVShow *curShow = findShow(showName);
    free(showName);

    if (!curShow)
    {
        printf("Show does not exist.\n");
        return;
    }

    printf("Enter the name of the season:\n");
    char *seasonName = getString();
    if (!seasonName)
        return;

    Season *iterator = curShow->seasons;
    Season *prev = NULL;

    if (!iterator)
    {
        printf("Season not found.\n");
        free(seasonName);
        return;
    }

    if (strcmp(iterator->name, seasonName) == 0)
    {
        // head
        curShow->seasons = iterator->next;
        iterator->next = NULL;
        freeSeason(iterator);
        free(seasonName);
        printf("Season deleted successfully.\n");
        return;
    }

    while (iterator != NULL && strcmp(iterator->name, seasonName) != 0)
    {
        prev = iterator;
        iterator = iterator->next;
    }

    if (!iterator)
    {
        printf("Season not found.\n");
        free(seasonName);
        return;
    }

    prev->next = iterator->next;
    iterator->next = NULL;
    freeSeason(iterator);
    free(seasonName);
    printf("Season deleted successfully.\n");
}
void deleteEpisode()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    if (!showName)
    {
        return;
    }

    TVShow *curShow = findShow(showName);
    free(showName);

    if (!curShow)
    {
        printf("Show does not exist.\n");
        return;
    }

    printf("Enter the name of season:\n");
    char *seasonName = getString();
    if (!seasonName)
    {
        return;
    }
    Season *curSeason = findSeason(curShow, seasonName);
    free(seasonName);

    if (!curSeason)
    {
        printf("Season does not exist.\n");
        return;
    }

    printf("Enter the name of the episode\n");
    char *episodeName = getString();
    if (!episodeName)
    {
        return;
    }

    Episode *curEpisode = findEpisode(curSeason, episodeName);
    if (!curEpisode)
    {
        printf("Episode does not exist\n");
        free(episodeName);
        return;
    }

    Episode *iterator = curSeason->episodes;
    Episode *prev = NULL;

    if (!iterator)
    {
        printf("Episode not found.\n");
        free(episodeName);
        return;
    }

    // if head==episodeName
    if (strcmp(iterator->name, episodeName) == 0)
    {
        curSeason->episodes = iterator->next;
        iterator->next = NULL;
        freeEpisode(iterator);
        free(episodeName);
        printf("Episode deleted successfully.\n");
        return;
    }

    // Iterating the list
    while (iterator != NULL && strcmp(iterator->name, episodeName) != 0)
    {
        prev = iterator;
        iterator = iterator->next;
    }

    if (iterator == NULL) // episode not found
    {
        printf("Episode not found.\n");
        free(episodeName);
        return;
    }

    prev->next = iterator->next;
    iterator->next = NULL;
    freeEpisode(iterator);
    free(episodeName);
    printf("Episode deleted successfully.\n");
}

// PRINTING FUNCTIONS
void printShow()
{
    TVShow *curShow = getShowFromUser();
    if (curShow == NULL)
        return;

    printf("Show Name: %s\n", curShow->name);

    Season *CurSeason = curShow->seasons;
    printf("Seasons:\n");

    int count = 0;
    while (CurSeason != NULL)
    {
        printf("    Season %d: %s\n", count, CurSeason->name);

        if (CurSeason->episodes != NULL)
        {
            printf(" Episodes:\n");
            Episode *curEpisode = CurSeason->episodes;
            int epCount = 0;

            while (curEpisode != NULL)
            {
                printf("        Episode %d: %s, Length:(%s)\n",
                       epCount, curEpisode->name, curEpisode->length);
                curEpisode = curEpisode->next;
                epCount++;
            }
        }

        CurSeason = CurSeason->next;
        count++;
    }

    return;
}
/*void printSeason()
{
    char *showName = getString();
    TVShow *curShow = findShow(showName);
    if (curShow == NULL)
    {
        printf("Show does not exist.\n");
        free(showName);
        return;
    }
    free(showName);
    Season *CurSeason = curShow->seasons;
    while (CurSeason != NULL)
    {
        printf("Season Name: %s\n", CurSeason->name);
        Episode *curEpisode = CurSeason->episodes;
        int episodeCount = 0;
        while (curEpisode != NULL)
        {
            printf("  Episode %d: %s, Length: %s\n",episodeCount ,curEpisode->name, curEpisode->length);
            curEpisode = curEpisode->next;
            episodeCount++;
        }
        CurSeason = CurSeason->next;
    }
}*/
void printEpisode()
{
    TVShow *curShow = getShowFromUser();
    if (curShow == NULL)
        return;

    printf("Enter the name of season:\n");
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
    if (curEpisode == NULL)
    {
        printf("Episode does not exist\n");
        free(episodeName);
        return;
    }
    else
    {
        printf("Name: %s \n", episodeName);
        printf("Length: %s \n", curEpisode->length);
    }
    free(episodeName);
}
void printArray()
{
    int showCount = countShows();
    if (showCount == 0)
    {
        printf("No TV shows in the database.\n");
        return;
    }
    printf("[%d]", dbSize);
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == NULL)
            {

                printf("[NULL]");
            }
            else
            {
                printf("[%s]", database[i][j]->name);
            }
        }
    }
}

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
