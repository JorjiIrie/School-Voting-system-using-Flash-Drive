/*                                           PROJECT 1 (BY PAIR)

INSTRUCTION:  Student Election Voting System using List operations using USB drive as Voting Card (in C/C++)
Module 1 : Registration
Module 2:  Voting System

PREPARED BY: JEORGE IREY G. DANS                                                        BSCS-2A-M (STEM)
             JOHN DENZIL A. DIMAGUILA
*/

//HEADER FILES
#include <stdio.h> // standard input and output functions
#include <stdlib.h> // type conversion, utilities, passing
#include <string.h> // for string functions
#include <conio.h> // for user interface
#include <time.h> // for random numbers
#include <windows.h> // macros

//DEFINING PADDINGS
#define NAMELIMIT 31 //char size for names
#define STUDENTIDLIMIT 10 //char size for student ID
#define PINLIMIT 5 //char size for PIN
#define MAX 500 //max array size for the list
#define ADMINID "BSCS2A" //ADMIN ID
#define ADMINPIN "1234" //ADMIN PIN
#define STUDENTID "TUPM-" //school name for student id
#define VOTER_IND 0 //indicator used to access certain voter functions
#define CANDIDATE_IND 1 //indicator used to access certain candidate functions
#define LP 5 //left part (for output designing)
#define TP 3 //top part (for output designing)
#define RP 100 //right part (for output designing)

// STRUCTURES USED IN THE PROGRAM
typedef struct voter{ //structure for voters

	int voterIndicator; //structure member int variable for voter indicator
	char voterName[NAMELIMIT]; //structure member char variable for voter name
    char voterStudentID[STUDENTIDLIMIT]; //structure member char variable for voter student ID
	char voterPIN[PINLIMIT]; //structure member char variable for voter PIN
} VOTER; //data type name for voter structure
VOTER id; //global decalaration of id structure (for voter id reading)
VOTER voterTemp; //global declaration of temporary voter structure (for information reading)

typedef struct candidate{ //structure for candidates

    char candidateName[NAMELIMIT]; //structure member char variable for candidate name
    int candidateTally; //structure member int variable for candidate tally
} CANDIDATE; //data type name for candidate structure
CANDIDATE candidateTemp; //global declaration of temporary candidate structure (for information reading)

typedef struct list{ //list for storing the records in the databases

    VOTER votersList[MAX]; //voters structure array
    CANDIDATE candidatesList[MAX]; //candidates structure array
    int votersCount, candidatesCount; //int member variable for the last indices of the two lists
} LIST;  //data type name for the list structure
LIST l; //global declaration of voters and candidates list

void gotoxy(int x,int y){ // function to move the curson

    COORD coord; // coordinates of a character cell in a console screen buffer
    coord.X=x; // for indentation
    coord.Y=y; // for rows
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord); // This is part of the gotoxy function it looks it is setting the position of a cursor
}

// Function prototypes and their short descriptions as a comment
void headerBar(char *title, char *institution, char *section); // function for navigation bar
void borderContent(void); // function for content borders
void makeNull(); // function to make the list appear empty (reset)
int isEmpty(int ind); // function to check if the list has content inside
int isFull(int ind); // function to check if there's still room for adding in the list
int locate(); // function to locate record
void checkDatabase(); // function to check database
int mainMenu(); // function to print main menu
void readID(); // function for reading voter ID
void retrieve(int ind); // function to retrieve data from database
void save(int ind); // function to save data to database
int verifyID(); // function for verification of voter ID
int vote(); // function for the voting process
void markVoter(); // function that mark voters after voting
void loginFunction(); // function to login
int displayTally(); // function to display tally
int getInfo(int ind); // function to get the information for voter/candidate registration
void saveID(); // function to save voter ID
void add(int ind); // function for adding records to the list
int display(int ind); // function for displaying records
void edit(int ind, char nameEdit[NAMELIMIT]); // function for editing records
void del(int ind, char nameDel[NAMELIMIT]); // function for deleting records
void adminFunction(char adminID[NAMELIMIT]); // function for administrator options
void note(); //opening screen

int main(){ //main function

    char adminID[NAMELIMIT], adminPIN[PINLIMIT], ch; //local char variables

    note(); //opening screen
    system("cls"); //clears screen

    checkDatabase(); //checks for the database files
    makeNull(); //makes the list appear empty
    retrieve(VOTER_IND); //retrieve records in voters database
    retrieve(CANDIDATE_IND); //retrieve records in cadidates database


    while(1){ //infinite loop
        system("cls"); //clears screen
        switch(mainMenu()){ //switch case for the main menu
            case 1: //Login and Vote
                readID(); //read the voter id
                if(verifyID() == -1) //verifies the voter id, and if there is an error, return to main menu
                    break; //breaks from the case
                loginFunction(); //calls for the login menu and function
                break; //breaks from the case

            case 2: //Display Tally of Candidates
                system("cls"); //clears screen
                displayTally(); //displays the records of the candidates and their tallies
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 3: //Admin
                system("cls"); //clears screen
                headerBar("Verification for Administrator", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
                borderContent(); // for border

                char *subtitle = malloc(sizeof(char) * 50); // initialization
                subtitle = "Enter any key to continue"; // words to be printed
                gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
                printf("%s", subtitle); // printing

                gotoxy(LP + 33, TP + 9); //for output designing
                printf("Enter admin ID: "); //prompts for the admin ID
                scanf(" %[^\n]s", adminID); //scans for the admin ID

                if(strcasecmp(adminID, ADMINID) == 0){ //checks if input matches the ADMINID
                    gotoxy(LP + 33, TP + 11); //for output designing
                    printf("Enter admin's 4-digit PIN: "); //prompts for the admin's PIN
                    for(int i=0; i<PINLIMIT-1; i++){ //for loop for masking the PIN input with asterisks
                        ch = getch(); //scans for a character and stores in ch
                        if(ch>= 48 && ch <=57){ // to only accept numerical inputs (0-9 = 48-57 in ASCII)
                            adminPIN[i] = ch; //assigns ch to the ith element of the adminPIN
                            ch = '*'; //replaces ch with asterisk
                            printf("%c", ch); //prints asterisk to mask inputted character
                        }
                        else //if input is not numerical
                            i--; //disregard the iteration

                    }
                    if(strcmp(adminPIN, ADMINPIN) == 0){ //checks if input matches ADMINPIN
                        gotoxy(LP + 33, TP + 13); //for output designing
                        printf("Authenticated..."); //prints authentication message
                        getch(); //prompts for a keypress
                        adminFunction(adminID); //calls for the admin menu and function
                    }
                    else{ //if input doesn't match ADMINPIN
                        gotoxy(LP + 33, TP + 13); //for output designing
                        printf("[!] Incorrect PIN! Exiting..."); //incorrect PIN message
                        getch(); //prompts for a keypress
                    }
                }
                else{ //if input doesn't match ADMINID
                    gotoxy(LP + 33, TP + 13); //for output designing
                    printf("[!] Incorrect admin ID! Exiting..."); //incorrect admin ID message
                    getch(); //prompts for  keypress
                }
                break; //breaks from the case

            case 4: //Exit
                system("cls"); //clears screen
                headerBar("Closing", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
                borderContent(); // for the content border

                char *exitF = malloc(sizeof(char) * 50); // intialization
                exitF = "Your vote counts! Vote wisely always."; // words to be printed
                gotoxy((((LP + RP) / 2) - (strlen(exitF) / 2)), 26); //for output designing
                printf("%s", exitF); // printing

                gotoxy(LP + 33, TP + 13); //for output designing
                printf("Thank you for using the program!\n"); //thank you message
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                exit(0); //exit program

            default: // Invalid option
                gotoxy(LP + 33, TP + 25); //for output designing
                printf("Invalid option! "); //invalid option message
                system("pause"); //prompts for a keypress
        }
    }

}

void note(){ //opening screen

    system("cls"); //clears screen
    headerBar("Notes before voting", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // printing borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Enter any key to continue"; // words to be printed
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // priinting

    gotoxy(LP + 5, TP + 9); //for output designing
    printf("NOTES: "); // printing
    gotoxy(LP + 5, TP + 11); //for output designing
    printf("1. Ensure that your flash drive inserted in the laptop."); // printing
    gotoxy(LP + 5, TP + 13); //for output designing
    printf("2. If you do not have a file on your flash drive to save your information,"); // printing
    gotoxy(LP + 5, TP + 15); //for output designing
    printf("contact the administrator for registration."); // printing
    gotoxy(LP + 5, TP + 17); //for output designing
    printf("3. If you already have a file and your details on your flash drive, you may vote now."); // printing
    gotoxy(LP + 5, TP + 19); //for output designing
    printf("4.Vote wisely! Your vote matters! "); // printing
    gotoxy(LP + 33, TP + 25); //for output designing
    system("pause"); //prompts for a keypress

}

void makeNull(){ //makes the list appear null

    l.votersCount = -1; //sets the value of voters list's last index to -1
    l.candidatesCount = -1; //sets the value of candidates list's last index to -1
}

int isEmpty(int ind){ //checks if list is empty

    if(ind == VOTER_IND) //if indicator is for voter
        return(l.votersCount == -1); //returns if the last index of the voters list is -1, meaning the list is empty

    else if(ind == CANDIDATE_IND) //if indicator is for candidates
        return(l.candidatesCount == -1); //returns if the last index of the candidates list is -1, meaning the list is empty
}

int isFull(int ind){

    if(ind == VOTER_IND) //if indicator is for voter
        return(l.votersCount == MAX-1); //returns if the last index of the voters list is at maximum

    else if(ind == CANDIDATE_IND) //if indicator is for candidates
        return(l.candidatesCount == MAX-1); //returns if the last index of the candidates list is at maximum
}

int locate(int ind, char nameLocate[NAMELIMIT])
{
    if(ind == VOTER_IND){ //if indicator is for voter
        for(int i=0; i<=l.votersCount; i++){ //for loop for locating the name
            if(strcasecmp(l.votersList[i].voterName, nameLocate)==0) //checks if the two names are equal
                    return i; //if equal, return the index
        }
        return -1; //if none match, return -1
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        for(int i=0; i<=l.candidatesCount; i++){//for loop for locating the name
            if(strcasecmp(l.candidatesList[i].candidateName, nameLocate) == 0) //checks if the two names are equal
                    return i; //if equal, return the index
        }
        return -1; //if none match, return -1
    }
}

void checkDatabase(){ //checks for the existence of the database files

    FILE *fpVotersDatabase, *fpCandidatesDatabase; //file pointers

    headerBar("Checking Database Files", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // for borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Enter any key to continue"; // words to be printed
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

	fpVotersDatabase = fopen("Voters Database.dbf", "r"); //opens voters database in read mode
    if(!fpVotersDatabase){ //if database is not found, create new one
        gotoxy(LP + 18, TP + 9); //for output designing
		printf("[!] Voters Database is missing. Creating new database..."); //missing database message
		fpVotersDatabase = fopen("Voters Database.dbf", "w"); //opens voters databse in write mode to create new file
	}

	fpCandidatesDatabase = fopen("Candidates Database.dbf", "r"); //opens candidates database in read mode

	if(!fpCandidatesDatabase){ //if database is not found, create new one
        gotoxy(LP + 18, TP + 11); //for output designing
		printf("[!] Candidates Database is missing. Creating new database..."); //missing database message
		fpVotersDatabase = fopen("Candidates Database.dbf", "w"); //opens candidates databse in write mode to create new file
	}

    gotoxy(LP + 33, TP + 13); //for output designing
    printf("Database checking completed."); //completion message
    getch(); //prompts for a keypress

	fclose(fpVotersDatabase); //closes voters database file
	fclose(fpCandidatesDatabase); //closes candidates database file
}

int mainMenu(){ //main menu UI

    int mainMenuChoice; //int variable for user input
    headerBar("Main Menu", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // for content borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "VOTE COUNTS. VOTE WISELY"; // words to be printed
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    gotoxy(LP + 18, TP + 9); //for output designing
    printf("[1] Login and Vote"); // printing
    gotoxy(LP + 18, TP + 11); //for output designing
    printf("[2] Display Tally"); // printing
     gotoxy(LP + 18, TP + 13); //for output designing
    printf("[3] Administrator"); // printing
    gotoxy(LP + 18, TP + 15); //for output designing
    printf("[4] Exit"); // printing

    gotoxy(LP + 4, TP + 25); //for output designing
    printf("Enter your choice: "); //prompts for the choice of the user
    scanf("%d", &mainMenuChoice); //scans for the choice of the user

    return mainMenuChoice; //returns the choice of the user
}

void readID(){ //scans for the voter ID file

	FILE *fpVoterID; //file pointer

	do{ //do-while loop
		system("cls"); //clears screen
        headerBar("Reading your ID", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
        borderContent(); // for content borders

        char *subtitle = malloc(sizeof(char) * 50); // intialization
        subtitle = "Vote Counts. Vote wisely"; // words to be printed
        gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
        printf("%s", subtitle); // printing

        gotoxy(LP + 33, TP + 13); //for output designing
		printf("Please insert Voter ID..."); //message while waiting for the flash drive
		fpVoterID = fopen("D:\\Voter ID.txt", "r"); //attempt to open the voter ID file in read mode
	}while(fpVoterID == NULL); //loop reading screen until a voter ID file is detected

	system("cls"); //clears screen
    fscanf(fpVoterID, "%[^\t] %*[^\t] %s", id.voterName, id.voterPIN); //scans for the information inside the voter ID file

	fclose(fpVoterID); //closes the voter ID file
}

void retrieve(int ind) //reads the records inside the database files
{
    FILE *fpVotersDatabase, *fpCandidatesDatabase; //file pointers

    if(ind == VOTER_IND){ //if indicator is for voter
        fpVotersDatabase = fopen("Voters Database.dbf", "r"); //opens the voters database in read mode

    	fscanf(fpVotersDatabase, "%*\n"); //scans for the first blank line in the database file
    	while(!feof(fpVotersDatabase)){ //loop until EOF
            fscanf(fpVotersDatabase, "%d\t%[^\t] %[^\t] %s\n", &voterTemp.voterIndicator, voterTemp.voterName, voterTemp.voterStudentID, voterTemp.voterPIN); //scans for the voter information in the database file
            add(VOTER_IND); //adds the information to the list
    	}

        fclose(fpVotersDatabase); //closes the voters database file
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        fpCandidatesDatabase = fopen("Candidates Database.dbf", "r"); //opens the voters database in read mode

        fscanf(fpCandidatesDatabase, "%*\n"); //scans for the first blank line in the database file
        for(int i=0; !feof(fpCandidatesDatabase); i++){ //loop until EOF
            fscanf(fpCandidatesDatabase, "%[^\t] %d\n", candidateTemp.candidateName, &candidateTemp.candidateTally); //scans for the candidate information in the database file
            add(CANDIDATE_IND); //adds the information to the list
        }

        fclose(fpCandidatesDatabase); //closes the voters database file
    }
}

void save(int ind) //saves the records in the list to the database files
{
    FILE *fpVotersDatabase, *fpCandidatesDatabase; //file pointers

    if(ind == VOTER_IND){ //if indicator is for voter
        fpVotersDatabase = fopen("Voters Database.dbf", "w"); //opens the voters database file in write mode

    	for(int i=0; i<=l.votersCount; i++){ //loop until the end of the list
            fprintf(fpVotersDatabase, "%d\t%s\t%s\t%s\n", l.votersList[i].voterIndicator, l.votersList[i].voterName, l.votersList[i].voterStudentID, l.votersList[i].voterPIN); //saves the records in the list
    	}

    	fclose(fpVotersDatabase); //closes the voters database file
        return; //return to calling point
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        fpCandidatesDatabase = fopen("Candidates Database.dbf", "w"); //opens the candidates database file in write mode

        for(int i=0; i<=l.candidatesCount; i++){ //loop until the end of the list
            fprintf(fpCandidatesDatabase, "%s\t%d\n", l.candidatesList[i].candidateName, l.candidatesList[i].candidateTally); //saves the records in the list
        }

        fclose(fpCandidatesDatabase); //closes the candidates database file
        return; //return to calling point
    }
}

int verifyID(){ //verifies the voter ID in the flash drive

    headerBar("Verifying Identification", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // for borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Vote counts. Vote wisely."; // borders
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    if(isEmpty(VOTER_IND)){ //checks if the voters list is empty
            gotoxy(LP + 33, TP + 9); //for output designing
    		printf("[!] ERROR: Voters Database is empty."); //error message
            gotoxy(LP + 33, TP + 25); //for output designing
    		system("pause"); //prompts for a keypress
    		return -1; //error
    }

    for(int i=0; i<=l.votersCount; i++){ //for loop for verifying the voter ID
        if(strcmp(l.votersList[i].voterName, id.voterName) == 0){ //check if voter name is in voters database
			if(l.votersList[i].voterIndicator == 0){ //check indicator if voter has already voted or not
                if(strcmp(l.votersList[i].voterPIN, id.voterPIN) == 0){ //check if voter PIN matches the one in database
                    return 1; //voter ID matches in database
                }
                else{ //incorrect PIN
                    gotoxy(LP + 18, TP + 9); //for output designing
                    printf("[!] PIN Code is incorrect. Please check your Voter ID file."); //incorrect PIN message
                    gotoxy(LP + 33, TP + 25); //for output designing
                    system("pause"); //prompts for a keypress
                    return -1; //error
                }
			}
			else{ //indicator in database is 1 - already voted
                gotoxy(LP + 33, TP + 9); //for output designing
                printf("[!] Sorry, you have already voted!"); //error message - already voted
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                return -1; //error
			}
		}
	}
	//if for loop finishes without matching a name in the database, person is not registered
	gotoxy(LP + 11, TP + 9); //for output designing
	printf("[!] You are not a registered voter. Please contact administrator to register."); //error message -  not registered
    gotoxy(LP + 33, TP + 25); //for output designing
    system("pause"); //prompts for a keypress
    return -1; //error
}

int vote(){ //for the voting process

    int chosenCandidate, voteConfirm; //local variables

    headerBar("Voting System", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // for content borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Vote counts. Vote wisely!"; // words to be printed
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    if(isEmpty(CANDIDATE_IND)){ //checks if the candidates list is empty
        gotoxy(LP + 33, TP + 9); //for output designing
        printf("[!] ERROR: Candidates Database is empty."); //error message
        system("pause"); //prompts for a keypress
        return -1; //error
    }

    gotoxy(LP + 18, TP + 9); //for output designing
    printf("VOTING RULES:"); // print - VOTING RULES
    gotoxy(LP + 18, TP + 11); //for output designing
    printf("1. You can vote only once."); // print - You can vote only once
    gotoxy(LP + 18, TP + 13); //for output designing
    printf("2. You cannot change or edit your vote once registered."); // print - You cannot change or edit your vote once registered
    gotoxy(LP + 18, TP + 15); //for output designing
    printf("3. Please be fair and vote wisely."); // print - Please be fair and vote wisely
    gotoxy(LP + 33, TP + 25); //for output designing
    system("pause"); //prompts for a keypress

    while(1){ //infinite loop

        system("cls"); //clears screen
        headerBar("Official Website", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
        borderContent(); // for borders

        char *voteP = malloc(sizeof(char) * 50); // initialization
        voteP = "Vote counts. Vote wisely!"; // words to be printed
        gotoxy((((LP + RP) / 2) - (strlen(voteP) / 2)), 26); //for output designing
        printf("%s", voteP); // printing

        gotoxy(LP + 3 , TP + 7); //for output designing
        printf("LIST OF CANDIDATES"); // prints the header of the table

        for(int i=0; i<=l.candidatesCount; i++){ //for loop for displaying the candidates
            printf("\n\t[%d] %-28s", i+1, l.candidatesList[i].candidateName); //print candidates
        }

        gotoxy(LP + 4, TP + 25); //for output designing
        printf("Enter the number of your chosen candidate (1-%d): ", l.candidatesCount+1); //prompt for the vote
        scanf("%d", &chosenCandidate); //scans for the vote

        chosenCandidate--; //decrement to match with array index

        if(chosenCandidate>l.candidatesCount){ //check if choice is valid
            gotoxy(LP + 33, TP + 26); //for output designing
            printf("Invalid! Please select from 1-%d.", l.candidatesCount+1); //invalid message
            getch(); //prompts for a keypress
        }
        else{ //confirm vote
            system("cls"); //clears screen
            headerBar("Voting System", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
            borderContent();  // for borders

            gotoxy(LP + 18, TP + 9); //for output designing
            printf("You are voting for %s.", l.candidatesList[chosenCandidate].candidateName); //confirmation message
            gotoxy(LP + 18, TP + 11); //for output designing
            printf("Do you confirm?"); // prints the prompt
            gotoxy(LP + 18, TP + 13); //for output designing
            printf("[1] Yes"); // prints yes
            gotoxy(LP + 18, TP + 15); //for output designing
            printf("[2] No"); // prints no
            gotoxy(LP + 4, TP + 25); //for output designing
            printf("Enter your option: "); //prompts for the confirmation option
            scanf("%d", &voteConfirm); //scans for the confirmation option

            if(voteConfirm == 1) //if confirmed
                break; //break from infinite loop
            else if(voteConfirm == 2); //if not confirmed, do nothing to start from the beginning
            else{ //invalid option
                gotoxy(LP + 33, TP + 25); //for output designing
                printf("Invalid option! "); //invalid message
                system("pause"); //prompts for a keypress
            }
        }
    }

    l.candidatesList[chosenCandidate].candidateTally++; //increment the tally of the candidate chosen

    save(CANDIDATE_IND); //saves the candidates list to database
    return 1; //success
}

void markVoter() //marks the voter in the database after voting
{
	l.votersList[locate(VOTER_IND, id.voterName)].voterIndicator = 1; //mark voter as already voted

    save(VOTER_IND); //saves the voters list to the database
}

void loginFunction() //function for the voter login
{
    int loginMenuOption; //local variable

    while(1){ //infinite loop
        system("cls"); //clears screen
        headerBar("Digital Voting System", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
        borderContent(); // for content borders

        char *subtitle = malloc(sizeof(char) * 50); // initialization
        subtitle = "Vote counts. Vote wisely!"; // words to be printed
        gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
        printf("%s", subtitle); // printing subtitles

        gotoxy(LP + 18, TP + 9); //for output designing
        printf("Welcome, %s!", id.voterName); // printing
        gotoxy(LP + 18, TP + 11); //for output designing
        printf("MAIN MENU"); // printing
        gotoxy(LP + 18, TP + 13); //for output designing
        printf( "[1] Vote\n"); // printing
        gotoxy(LP + 18, TP + 15); //for output designing
        printf("[2] Exit\n\n"); // printing
        gotoxy(LP + 4, TP + 25); //for output designing
        printf("Enter your option: "); // printing
        scanf("%d", &loginMenuOption); // scanning data

        switch(loginMenuOption){ //switch case for the login menu
            case 1: //Vote
                system("cls"); //clears screen
                if(vote() == -1) //if an error occurred after voting
                    return; //return to main menu
                markVoter(); //mark voter as already voted
                system("cls"); //clears screen
                headerBar("Voting Website", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
                borderContent(); // for borders

                char *v = malloc(sizeof(char) * 50); // initialization
                v = "Vote counts. Vote wisely!"; // words to be print
                gotoxy((((LP + RP) / 2) - (strlen(v) / 2)), 26); //for output designing
                printf("%s", v); // printing

                gotoxy(LP + 18, TP + 13); //for output designing
                printf("Your vote has been registered. Thank you for voting!"); //success message
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                return; //voting finished, return to main menu

            case 2: //Exit
                return; //return to main menu

            default: //Invalid option
                gotoxy(LP + 33, TP + 25); //for output designing
                printf("Invalid option! "); //invalid message
                system("pause"); //prompts for a keypress
        }
    }
}

int displayTally(){ //displays the list of candidates and their tallies

    headerBar("Display Tally", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // navigation bar
    borderContent(); // for borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Vote Counts. Vote wisely"; // words to be printed
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    if(isEmpty(CANDIDATE_IND)){ //checks if the candidates list is empty
        gotoxy(LP + 33, TP + 9); //for output designing
        printf("[!] Candidates Database is empty."); //empty list message
        return -1; //error
    }

    gotoxy(LP + 3 , TP + 7); //for output designing
    printf("NAME OF CANDIDATE"); // printing
    gotoxy(LP + 35 , TP + 7); //for output designing
    printf("TOTAL VOTES"); // printing

    for(int i=0; i<=l.candidatesCount; i++){ //for loop for displaying the candidates with their tallies
        printf("\n\t%d.) %-28s%d", i+1, l.candidatesList[i].candidateName, l.candidatesList[i].candidateTally); //printing of information
    }

    return 1; //success
}

int getInfo(int ind){ //getting the information for voter registration

    char ch; //local vriable

    headerBar("Getting your information", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation bar
    borderContent(); // for content borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Vote Counts. Vote wisely"; // words to be print
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    if(ind == VOTER_IND){ //if indicator is for voter
        gotoxy(LP + 33, TP + 9); //for output designing
        printf("Enter your name: "); //prompts for the voter name
        scanf(" %[^\n]", voterTemp.voterName); //scans for the voter name

        if(locate(ind, voterTemp.voterName) >= 0){ //check for duplicates
            gotoxy(LP + 33, TP + 11);  //for output designing
            printf("[!] Voter already exists"); //error message
            return -1; //error
        }

        srand(time(NULL)); //use current time as seed for rand()
        sprintf(voterTemp.voterStudentID, "%s%04d", STUDENTID, rand()%9999); //format student ID
        gotoxy(LP + 33, TP + 11); //for output designing
        printf("Your student ID number is %s", voterTemp.voterStudentID); //prints the student ID

        fflush(stdin); //clears the input buffer
        gotoxy(LP + 33, TP + 13); //for output designing
        printf("Enter a 4-digit PIN: "); //prompts for a 4-digit PIN
        for(int i=0; i<PINLIMIT-1; i++){ //for loop for masking the PIN input with asterisks
            ch = getch(); //scans for a character and stores in ch
            if(ch >= 48 && ch <=57){ //to only accept numerical inputs (0-9 = 48-57 in ASCII)
                voterTemp.voterPIN[i] = ch; //assign ch to the ith element of the voter PIN in voterTemp
                ch = '*'; //replaces ch with asterisk
                printf("%c", ch); //prints asterisk to mask inputted character
            }
            else //if input is not numerical
                i--; //disregard the iteration
        }

        voterTemp.voterIndicator = 0; //set indicator for newly registered voters
        return 1; //success
    }

    if(ind == CANDIDATE_IND){ //if indicator is for candidates
        gotoxy(LP + 33, TP + 9); //for output designing
        printf("Enter the name of the candidate: "); //prompts for the candidate name
        scanf(" %[^\n]", candidateTemp.candidateName); //scans for the candidate name

        if(locate(ind, candidateTemp.candidateName) >= 0){ //check for duplicates
            gotoxy(LP + 33, TP + 11); //for output designing
            printf("[!] Candidate already exists!"); //error message
            return -1; //error
        }

        candidateTemp.candidateTally = 0; //set tally for newly registered candidates
        return 1; //success
    }
}

void saveID() //saves the voter information the the voter ID file
{
    FILE *fpVoterID; //file pointer

    fpVoterID = fopen("D://Voter ID.txt", "w"); //opens the voter ID text file in write mode

    fprintf(fpVoterID, "%s\t%s\t%s\n", l.votersList[l.votersCount].voterName, l.votersList[l.votersCount].voterStudentID, l.votersList[l.votersCount].voterPIN); //prints the information to the text file

    fclose(fpVoterID); //closes the voter ID file
}

void add(int ind){ //adds the records to the list

    if(ind == VOTER_IND){ //if indicator is for voter
        if(isFull(ind)){ //checks if the voters list is full
            gotoxy(LP + 33, TP + 9);  //for output designing
            printf("[!] Voters Database is full."); //error message
        }

        else{ //if list is not full
            l.votersCount++; //increment the last index count of the voters list
            l.votersList[l.votersCount].voterIndicator = voterTemp.voterIndicator; //add the voter indicator to the list
            strcpy(l.votersList[l.votersCount].voterName, voterTemp.voterName); //add the voter name to the list
            strcpy(l.votersList[l.votersCount].voterStudentID, voterTemp.voterStudentID); //add the voter student ID to the list
            strcpy(l.votersList[l.votersCount].voterPIN, voterTemp.voterPIN); //add the voter PIN to the list
            save(ind); //saves the list to the voter database file
        }
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        if(isFull(ind)){ //checks if the candidates list is full
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Candidates Database is full."); //error message
        }

        else{
            l.candidatesCount++; //increment the last index count of the candidates list
            strcpy(l.candidatesList[l.candidatesCount].candidateName, candidateTemp.candidateName); //add the candidate name to the list
            l.candidatesList[l.candidatesCount].candidateTally = candidateTemp.candidateTally; //add the candidate to the list
            save(ind); //saves the list to the candidates database file
        }
    }
}

int display(int ind){ //displays the records in the list

    headerBar("Display Informations", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation
    borderContent(); // borders

    char *subtitle = malloc(sizeof(char) * 50); // initialization
    subtitle = "Vote Counts. Vote wisely"; // words to be print
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing


    if(ind == VOTER_IND){ //if indicator is for voter
        if(isEmpty(ind)){ //checks if the voters list is empty
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Voters Database is empty."); //error message
            return -1; //error
        }

        gotoxy(LP + 3 , TP + 7); //for output designing
        printf("NAME"); // printing
        gotoxy(LP + 35 , TP + 7); //for output designing
        printf("STUDENT ID"); // printing
        gotoxy(LP + 55, TP + 7); //for output designing
        printf("STATUS"); // printing

        for(int i=0; i<=l.votersCount; i++) //for loop for printing the voters' records from the list
            printf("\n\t%d.) %-28s%s\t%s", i+1, l.votersList[i].voterName, l.votersList[i].voterStudentID, l.votersList[i].voterIndicator == 1 ? "Already voted" : "Not yet voted"); //prints records

        return 1; //success
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        if(isEmpty(ind)){ //checks if the candidates list is empty
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Candidates Database is empty."); //error message
            return -1; //error
        }

        gotoxy(LP + 3 , TP + 7); //for output designing
        printf("NAME"); // printing

        for(int i=0; i<=l.candidatesCount; i++) //for loop for printing the candidates' records from the list
            printf("\n\t%d.) %-28s", i+1, l.candidatesList[i].candidateName); //prints records

        return 1; //success
    }
}

void edit(int ind, char nameEdit[NAMELIMIT]){ //for editing records in the list and database
    int p; //local in variable
    char ch; //local char variable

    if(ind == VOTER_IND){ //if indicator is for voter
        p = locate(ind, nameEdit); //locates for the inputted name
        if(p < 0){ //if no record is located
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Voter doesn't exist!"); //error message
            return; //return to calling point
        }
        while(1){ //edit status and PIN
            system("cls"); //clears screen
            headerBar("Editing", "Technological University of the Philippines ", "DANS AND DIMAGUILA"); // for navigation bar
            borderContent(); // borders

            char *subtitle = malloc(sizeof(char) * 50); // initiailization
            subtitle = "Have a nice day, admin! "; // words to be print
            gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
            printf("%s", subtitle); // printing

            gotoxy(LP + 18, TP + 9); //for output designing
            printf("Enter new status (0 - Not yet voted, 1 - Already voted): "); //prompts for the new status
            scanf("%d", &l.votersList[p].voterIndicator); //scans for the new status
            if(l.votersList[p].voterIndicator == 0 || l.votersList[p].voterIndicator == 1) //if input is valid
                break; //break from the infinite loop
            else{ //if input is invalid
                gotoxy(LP + 18, TP + 11); //for output designing
                printf("[!] Invalid input! "); //invalid message
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
            }
        }
        gotoxy(LP + 18, TP + 11); //for output designing
        printf("Enter 4-digit PIN (can be old or new): "); //prompts for the old or new PIN
        for(int i=0; i<PINLIMIT-1; i++){ //for loop for masking the PIN input with asterisks
            ch = getch(); //scans for a character and stores in ch
            if(ch >= 48 && ch <=57){ //to only accept numerical inputs (0-9 = 48-57 in ASCII)
                l.votersList[p].voterPIN[i] = ch; //assignes ch to the ith element of the voter PIN in voters list
                ch = '*'; //replaces ch with asterisk
                printf("%c", ch); //prints asterisk to mask inputted character
            }
            else //if input is not numerical
                i--; //disregard the iteration
        }

        save(ind); //saves the voters list to the database file
        gotoxy(LP + 18, TP + 13); //for output designing
        printf("Voter successfully edited!"); //success message
        return; //success
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        system("cls"); //clears screen
        headerBar("Editing", "Technological University of the Philippines ", "DANS AND DIMAGUILA"); // for navigation bar
        borderContent(); // for borders

        char *subtitle = malloc(sizeof(char) * 50); // initialize border
        subtitle = "Have a nice day, admin! "; // words to be print
        gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
        printf("%s", subtitle); // printing

        p = locate(ind, nameEdit); //locates for the inputted name

        if(p < 0){ //if no record is located
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("Candidate doesn't exist!"); //error message
            return; //return to calling point
        }
        gotoxy(LP + 18, TP + 9); //for output designing
        printf("Enter new name: "); //prompts for the new name of the candidate
        scanf(" %[^\n]", l.candidatesList[p].candidateName); //scans for the new name of the candidate

        save(ind); //saves the candidates list to the database
        gotoxy(LP + 18, TP + 13); //for output designing
        printf("Candidate successfully edited!"); //success message
        return; //return to calling point
    }
}

void del(int ind, char nameDel[NAMELIMIT]){ //for deleting records in the list and database

    int p; //local variable

    system("cls"); //clears screen
    headerBar("Deleting", "Technological University of the Philippines ", "DANS AND DIMAGUILA"); // for navigation
    borderContent(); // for borders

    char *subtitle = malloc(sizeof(char) * 50); // intialization
    subtitle = "Have a nice day, admin! "; // words to be print
    gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
    printf("%s", subtitle); // printing

    if(ind == VOTER_IND){ //if indicator is for voter
        p = locate(ind, nameDel); //locates for the inputted name
        if(p < 0){ //if no record is located
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Voter doesn't exist!"); //error message
            return; //return to calling point
        }

        for(int i=p; i<l.votersCount; i++){ //for loop for deletion
            l.votersList[i] = l.votersList[i+1]; //copies the preceeding records to the suceeding ones
        }
        l.votersCount--; //decrement the last index of the voters list, meaning one record is deleted

        save(ind); //saves the voters list to the database
        gotoxy(LP + 33, TP + 13); //for output designing
        printf("Voter successfully deleted!"); //success message
    }

    else if(ind == CANDIDATE_IND){ //if indicator is for candidates
        p = locate(ind, nameDel); //locates for the inputted name

        if(p < 0){ //if no record is located
            gotoxy(LP + 33, TP + 9); //for output designing
            printf("[!] Candidate doesn't exist!"); //error message
            return; //return to calling point
        }

        for(int i=p; i<l.candidatesCount; i++){ //for loop for deletion
            l.candidatesList[i] = l.candidatesList[i+1]; //copies the preceeding records to the suceeding ones
        }
        l.candidatesCount--; //decrement the last index of the candidates list, meaning one record is deleted

        save(ind); //saves the candidates list to the database
        gotoxy(LP + 33, TP + 13); //for output designing
        printf("Candidate successfully deleted!"); //success message
    }
}

void adminFunction(char adminID[NAMELIMIT]){ //for administrative operations

    char *admiinChoice = malloc(sizeof(char) * 50); //local char pointer variable
    int adminMenuOption; //local int variable
    char name[NAMELIMIT]; //local char variable

    while(1){ //infinite loop
        system("cls"); //clears screen
        headerBar("Administrator Menu", "Technological University of the Philippines", "DANS AND DIMAGUILA"); // for navigation
        borderContent(); // for borders

        char *subtitle = malloc(sizeof(char) * 50); // initialization
        subtitle = "Good Day Admin!"; // words to be print
        gotoxy((((LP + RP) / 2) - (strlen(subtitle) / 2)), 26); //for output designing
        printf("%s", subtitle); // printing

        gotoxy(LP + 18, TP + 6); //for output designing
        printf("SETTINGS FOR VOTERS: "); // printing
        gotoxy(LP + 18, TP + 9); //for output designing
        printf("[1] Register a voter "); // printing
        gotoxy(LP + 18, TP + 11); //for output designing
        printf("[2] Edit a voter "); // printing
        gotoxy(LP + 18, TP + 13); //for output designing
        printf("[3] Delete a voter "); // printing
        gotoxy(LP + 18, TP + 15); //for output designing
        printf("[4] Display all voters "); // printing

        admiinChoice = "SETTINGS FOR CANDIDATES:";  // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 6); //for output designing
        printf("%s", admiinChoice); // printing
        admiinChoice = "[5] Add a candidate "; // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 9); //for output designing
        printf("%s", admiinChoice); // printing
        admiinChoice = "[6] Edit a candidate "; // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 11); //for output designing
        printf("%s", admiinChoice); // printing
        admiinChoice = "[7] Delete a candidate "; // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 13); //for output designing
        printf("%s", admiinChoice); // printing
        admiinChoice = "[8] Display all candidate "; // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 15); //for output designing
        printf("%s", admiinChoice); // printing
        admiinChoice = "[9] Exit administrator menu "; // words to be printed
        gotoxy(RP - (20 + strlen(admiinChoice)), TP + 17); //for output designing
        printf("%s", admiinChoice); // printing

        gotoxy(LP + 4, TP + 25); //for output designing
        printf("Enter your choice: "); // printing
        scanf("%d", &adminMenuOption); // scanning the inputted choice
        getchar(); // waiting for key

        switch(adminMenuOption){ //switch case for admin menu
            case 1: //Register Voters
                system("cls"); //clears screen
                if(getInfo(VOTER_IND) == 1){ //if no errors occurred after getting information
                    add(VOTER_IND); //proceed to adding the information to the voters list
                    saveID(); //save the voter ID text file after voter registration
                    gotoxy(LP + 33, TP + 17); //for output designing
                    printf("Voter successfully registered!"); //success message
                    gotoxy(LP + 33, TP + 18); //for output designing
                    printf("See you on the next election!"); //success message
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 2: //Edit Voters
                system("cls"); //clears screen
                if(display(VOTER_IND) == 1){ //if no errors occurred after displaying records
                    gotoxy(LP + 4, TP + 25); //for output designing
                    printf("Enter the name of the voter you want to edit: "); //prompts for the name to be edited
                    scanf(" %[^\n]", name); //scans for the name to be edited
                    edit(VOTER_IND, name); //calls for the edit function to edit the record
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 3: //Delete Voters
                system("cls"); //clears screen
                if(display(VOTER_IND) == 1){ //if no errors occurred after displaying records
                    gotoxy(LP + 4, TP + 25); //for output designing
                    printf("Enter the name of the voter you want to delete: "); //prompts for the name to be deleted
                    scanf(" %[^\n]", name); //scans for the name to be deleted
                    del(VOTER_IND, name); //calls for the delete function to delete the record
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 4: //Display Voters
                system("cls"); //clears screen
                display(VOTER_IND); //display the records in voters list
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 5: //Register Candidates
                system("cls"); //clears screen
                if(getInfo(CANDIDATE_IND) == 1){ //if no errors occurred after getting information
                    add(CANDIDATE_IND); //proceed to adding the information to the candidateslist
                    printf("Candidate successfully registered!"); //success message
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 6: //Edit Candidates
                system("cls"); //clears screen
                if(display(CANDIDATE_IND) == 1){ //if no errors occurred after displaying records
                    gotoxy(LP + 4, TP + 25); //for output designing
                    printf("Enter the name of the candidate you want to edit: "); //prompts for the name to be edited
                    scanf(" %[^\n]", name); //scans for the name to be edited
                    edit(CANDIDATE_IND , name); //calls for the edit function to edit record
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 7: //Delete Candidates
                system("cls"); //clears screen
                if(display(CANDIDATE_IND) == 1){ //if no errors occurred after displaying records
                    gotoxy(LP + 4, TP + 25); //for output designing
                    printf("Enter the name of the candidate you want to delete: "); //prompts for the name to be deleted
                    scanf(" %[^\n]", name); //scans for the name to be deleted
                    del(CANDIDATE_IND, name); //calls for the delete function to delete the record
                }
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 8: //Display Candidates
                system("cls"); //clears screen
                display(CANDIDATE_IND); //display the records in candidates list
                gotoxy(LP + 33, TP + 25); //for output designing
                system("pause"); //prompts for a keypress
                break; //breaks from the case

            case 9: //Exit
                return; //return to main menu

            default:
                gotoxy(LP + 33, TP + 25); //for output designing
                printf("Invalid option! "); //invalid message
                system("pause"); //prompts for a keypress
        }
    }
}

void borderContent(void){

     // Left and Right Borders
    for(int i = TP + 5; i < 30; i++){
        gotoxy(LP, i); //for output designing
        printf("%c", 186); // printing left borders
    }

    for(int i = TP + 5; i < 30; i++){
        gotoxy(RP, i); //for output designing
        printf("%c", 186); // printing right borders
    }

    // Bottom Borders
    for(int i = LP; i < RP; i++){
        gotoxy(i, 26); //for output designing
        printf("%c", 196); // printing first bottom borders
    }

    for(int i = LP; i < RP; i++){
        gotoxy(i, 30); //for output designing
        printf("%c", 205); // for second bottom borders
    }

    // Corners
    gotoxy(LP, TP + 4); //for output designing
    printf("%c", 199); // printing
    gotoxy(RP, TP + 4); //for output designing
    printf("%c", 182); // printing
    gotoxy(LP, TP + 27); //for output designing
    printf("%c", 200); // printing
    gotoxy(RP, TP + 27); //for output designing
    printf("%c", 188); // printing
    gotoxy(LP, 26); //for output designing
    printf("%c", 199); // printing
    gotoxy(RP, 26); //for output designing
    printf("%c", 182); // printing

}

void headerBar(char *title, char *institution, char *section){

    system("COLOR 7C"); // changing the background color into GRAY and font color into RED

    // Left and Right Borders
    for(int i = TP; i < 7; i++){
        gotoxy(LP, i); //for output designing
        printf("%c", 186); // printing left par
    }

    for(int i = TP; i < 7; i++){
        gotoxy(RP, i); //for output designing
        printf("%c", 186); // printing right part
    }

    // Top and Bottom Borders
    for(int i = LP; i < 100; i++){
        gotoxy(i, 2); //for output designing
        printf("%c", 205); // printing left borders
    }

    for(int i = LP; i < 100; i++){
        gotoxy(i, 7); //for output designing
        printf("%c", 196);  // printing bottom
    }

    // Corners
    gotoxy(5, 2); //for output designing
    printf("%c", 201); // printing
    gotoxy(100, 2); //for output designing
    printf("%c", 187); // printing
    gotoxy(5, 7); //for output designing
    printf("%c", 211); // printing
    gotoxy(100, 7); //for output designing
    printf("%c", 189); // printing


    // Texts
    gotoxy((((LP + RP) / 2) - (strlen(institution) / 2)), TP + 1); //for output designing
    printf("%s", institution); // printing the given institution
    gotoxy((((LP + RP) / 2) - (strlen(section) / 2)), TP + 2); //for output designing
    printf("%s", section); // printing name of the author
    gotoxy((((LP + RP) / 2) - (strlen(title) / 2)), 7); //for output designing
    for(int i = 0; i < strlen(title); i++){
        for(int j = 0; j < 10000000; j++); // looping for the effects in title
        printf("%c", title[i]);//219
    }
}

