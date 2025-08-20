/*
########################################################################################################
PROGRAM: ajsWorldSeries.cpp
WRITTEN BY: Andrea Sitton
FOR: CPS 161 - Assignment #6

PURPOSE: This program allows the user to:
      (1) select an MLB baseball team from a provided list and returns the number of times and in which 
         years that team won the World Series, or 
      (2) select a year and see the name of the winning team. 
   This program accounts for team name changes over the years and covers from 1903 until 2024.
######################################################################################################## 
*/

#include <algorithm>    // Necessary for transform() on a string
#include <cctype>       // Necessary for transform()
#include <cmath>        // Necessary for ceil()
#include <fstream>
#include <iostream>
#include <limits>       // For handling wrong data type input
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

// Function prototypes
void loadTeams(vector<vector<string>> &);
void loadWinners(vector<vector<string>> &);
void sumWins(vector<vector<string>> &, vector<vector<string>>);
void getChoice(int &);
void winnerByYear(vector<vector<string>>);
void getTeam(vector<vector<string>>, int &);
void getWinYears(vector<vector<string>>, vector<vector<string>>, vector<string> &, int);
void displayResults(vector<vector<string>>, vector<string>, int);
void searchAgain(bool &);

int main()
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   // Change to an white background with red text (like a baseball)
   system("color F9");
   
   // Output formatting
   CONSOLE_FONT_INFOEX cfi;
   cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
   cfi.nFont = 0;
   cfi.dwFontSize.X = 0;
   cfi.dwFontSize.Y = 24;
   cfi.FontFamily = FF_SWISS;
   cfi.FontWeight = FW_MEDIUM;
   wcscpy(cfi.FaceName, L"Consolas");
   SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);   
   
   // Declare/initialize variables
   vector<vector<string>> teams;       // two-dimensional vector to store team information
   vector<vector<string>> winners;     // two-dimensional vector to store winning teams and years
   vector<string> years;               // vector to store the year(s) the selected team won
   int choice;                         // stores numerical value associated with user's choice
   int teamChoice;                     // stores the index value for the team the user is looking up
   bool again;                         // if user wants to run another search
      
   loadTeams(teams);
   loadWinners(winners);
   sumWins(teams, winners);
   
   do
   {
      getChoice(choice);
      
      // Search for winning team by year
      if(choice == 1)
      {
         winnerByYear(winners);
      }
      // Search for team statistics
      else if(choice == 2)
      {
         getTeam(teams, teamChoice);
         getWinYears(teams, winners, years, teamChoice);
         displayResults(teams, years, teamChoice); 
      }
      
      searchAgain(again);
   }
   while(again == true);
      
   // Terminate program execution   
   return 0;
}

/*
########################################################################################################
FUNCTION: loadTeams
PURPOSE: This function loads team information from a text file into a two-dimensional string vector.
@param teams     Reference variable for the 2D string vector used for storing team information
######################################################################################################## 
*/

void loadTeams(vector<vector<string>> &teams)
{
   string teamName,    // For transfering team name from file into vector
          teamAbbr,    // For transfering team abbreviation from file into vector
          nameKey;     // For transfering name change key from file into vector     
   
   // Declare and open teams input file
   ifstream inFile("ajsTeams.txt");
   
   while(getline(inFile, teamName))    // Loop until EOF
   {
      inFile >> teamAbbr;
      inFile >> nameKey;      // Used to link teams that won under multiple team names
      inFile.ignore();        // Ignore new line character
      
      // Add team info to new vector row
      teams.push_back({teamName, teamAbbr, nameKey, "0"});
   }
   
   inFile.close(); 
}

/*
########################################################################################################
FUNCTION: loadWinners
PURPOSE: This function loads the list of winning teams by year from a text file into a two-dimensional 
   string vector.
@param winners     Reference variable for the 2D string vector used for storing winner information
######################################################################################################## 
*/

void loadWinners(vector<vector<string>> &winners)
{
   string teamName,    // For transfering team name from file into vector
          winYear;     // For transfering win year from file into vector   
   
   // Declare and open winners input file
   ifstream inFile("ajsWorldSeriesWinners.txt");
   
   while(getline(inFile, teamName))    // Loop until EOF
   {
      inFile >> winYear;
      inFile.ignore();        // Ignore new line character
      
      // Add winner info to new vector row
      winners.push_back({teamName, winYear});
   }
   
   inFile.close(); 
}

/*
########################################################################################################
FUNCTION: sumWins
PURPOSE: This function sums the total number of wins achieved by each team listed in the teams vector. 
   The win number is stored as a string in the teams vector.
@param teams     Reference variable for the 2D string vector used for storing team information
       winners   The 2D string vector containing winning teams by year 
######################################################################################################## 
*/

void sumWins(vector<vector<string>> &teams, vector<vector<string>> winners)
{
   // Cycle through each team
   for(int teamIndex = 0; teamIndex < teams.size(); teamIndex++)
   {
      int winCount = 0;
      
      // Cycle through winner list
      for(int winIndex = 0; winIndex < winners.size(); winIndex++)
      {
         // If team name matches winning team name
         if(teams[teamIndex][0] == winners[winIndex][0])
         {
            winCount++;
         }   
      }
      
      // Modify win count in the teams vector
      teams[teamIndex][3] = to_string(winCount);
   }
}

/*
########################################################################################################
FUNCTION: getChoice
PURPOSE: This function accepts and validates the user's choice of search method.
@param choice     Reference variable for storing the user's choice as an integer
######################################################################################################## 
*/

void getChoice(int &choice)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      
   do
   {
      // Ask if user would like to search by team or by year
      cout << "\nWould you like to look up the winner for a specific year, or\n"
           << "would you like to look up the win statistics for a specific team?" << endl;
      cout << "\t\t1 = search by year\n"
           << "\t\t2 = search by team\n" << endl;
      SetConsoleTextAttribute(hConsole, 240);
      cin >> choice;
      SetConsoleTextAttribute(hConsole, 249);      
      
      // Catches non-integer input
      if(cin.fail())
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid input. An integer is required.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cin.clear();   // Clear the failbit
         // Ignore the remaining input
         cin.ignore(numeric_limits<streamsize>::max(),'\n');
      }
      else if(choice != 1 && choice != 2)
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid selection. Please try again.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
   }
   while(choice != 1 && choice != 2);
}

/*
########################################################################################################
FUNCTION: winnersByYear
PURPOSE: This function accepts a year and returns the winning team for that year, if available.
@param winners     The 2D string vector containing winning teams by year 
######################################################################################################## 
*/

void winnerByYear(vector<vector<string>> winners)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int year = 0;     
   int index = 0;       // To cycle through the winners vector
   bool found = false;  // Flag to control loop until desired year is reached
   
   do
   {  
      cout << "\nFor what year would you like to look up the winner?" << endl;
      cout << "The available data ranges from 1903 to 2024.\n" << endl;
      SetConsoleTextAttribute(hConsole, 240);
      cin >> year;
      SetConsoleTextAttribute(hConsole, 249);
      
      // Catches non-integer input
      if(cin.fail())
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid input. An integer is required.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cin.clear();   // Clear the failbit
         // Ignore the remaining input
         cin.ignore(numeric_limits<streamsize>::max(),'\n');
      }      
      else if(year < 1903 || year > 2024)
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nData is only available from 1903 to 2024." << endl;
         cout << "Please make a valid selection.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
      else if(year == 1904)
      {
         SetConsoleTextAttribute(hConsole, 242);
         cout << "\nIn 1904, there was no World Series winner because the\n"
              << "New York Giants (National League champions) declined to\n"
              << "play against the Boston Americans (American League champions).\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cout << "Please make another selection.\n" << endl;
      }
      else if(year == 1994)
      {
         SetConsoleTextAttribute(hConsole, 242);
         cout << "\nIn 1994, there was no World Series winner because the\n"
              << "series was cancelled due to a players' strike.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cout << "Please make another selection.\n" << endl;
      }            
   }
   while((year < 1903 || year > 2024) || year == 1904 || year == 1994);
   
   while(found == false)
   {
      // If the winning year of a team matches the user-ented year
      if(winners[index][1] == to_string(year))
      {
         found = true;
         SetConsoleTextAttribute(hConsole, 242);
         cout << "\nThe World Series winners in " << year << " were the "
              << winners[index][0] << "." << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
      else
         index++; 
   }
}

/*
########################################################################################################
FUNCTION: getTeam
PURPOSE: This function provides a list of teams for the user. It accepts a valid three letter 
   for one of the teams and stores the index value for that team in a reference variable.
@param teams         The 2D string vector containing team information
       teamChoice    Reference variable for storing the integer index value of the selected team
######################################################################################################## 
*/

void getTeam(vector<vector<string>> teams, int &teamChoice)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   string choice;       // Stores the three letter abbreviation for the team
   bool match = false;  // Flag to control loop until desired team is reached
   
   do
   {
      cout << "\nThe list below includes the names of every team that\n"
           << "won at least one World Series title between 1903 and 2024.\n" << endl;
      cout << "Please enter the three character abbreviation associated\n"
           << "with the team you would like to look up.\n" << endl;
      
      // Cycle through each team
      // ceil(team.size() / 2.0) dynamically splits the list into two equal columns
      for(int teamIndex = 0; teamIndex < ceil(teams.size() / 2.0); teamIndex++)
      {
         // Display the three letter abbreviation and the full team name
         cout << teams[teamIndex][1] << " = " << teams[teamIndex][0];
            
         // Add the necessary number of tabs based on the length of the
         // entries in column one so that the second column lines up properly
         if(teams[teamIndex][0].length() >= 18)
            cout << "\t";
         else if(teams[teamIndex][0].length() < 18)
            cout << "\t\t";
                  
         // This prevents an error when displaying a list with an odd number
         // of entries
         if(teamIndex != teams.size() / 2)
            cout << teams[teamIndex + ceil(teams.size() / 2.0)][1] << " = " 
                 << teams[teamIndex + ceil(teams.size() / 2.0)][0] << endl;
         else
            cout << endl;       
      }
      
      cout << endl;
      SetConsoleTextAttribute(hConsole, 240);
      cin >> choice;
      SetConsoleTextAttribute(hConsole, 249);
      // Standardizes input to all upper case if necessary for proper string comparison
      transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
      
      // Cycle through each team
      for(int teamIndex = 0; teamIndex < teams.size(); teamIndex++)
      {    
         // If the team's abbreviation matches the user-ented value
         if(teams[teamIndex][1] == choice)
         {
            match = true;
            teamChoice = teamIndex; // Save the team index number as a reference variable
            break;   // Once a match has been found, it can stop looking.
         }
      }
      
      if(match == false) // Input validation
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid selection. Please try again.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
   }
   while(match == false);   
}

/*
########################################################################################################
FUNCTION: getWinYear
PURPOSE: This function checks the chosen team against the list of winners and stores the win year in the
   years vector when there is a match. If a team has won titles under more than one team name, those 
   win years are also included.   
@param teams         The 2D string vector containing team information
       winners       The 2D string vector containing winning teams by year
       years         Reference variable for the string vector that stores winning years for the chosen
                        team
       teamChoice    The integer index value of the selected team
######################################################################################################## 
*/

void getWinYears(vector<vector<string>> teams, vector<vector<string>> winners,
         vector<string> &years, int teamChoice)
{
   if(!years.empty())
      years.clear();    // Empty years vector on subsequent runs
   
   // Cycle through winner list
   for(int winIndex = 0; winIndex < winners.size(); winIndex++)
   {
      // If team name matches winner name
      if(teams[teamChoice][0] == winners[winIndex][0])
      {
         // Add win year to vector
         years.push_back(winners[winIndex][1]);
      }
   }   
   
   // If name key indicates the team has won under other names  
   if(stoi(teams[teamChoice][2]) > 0)
   {
      // Cycle through each team
      for(int teamIndex = 0; teamIndex < teams.size(); teamIndex++)
      {
         // If the name key of the chosen team matches the name key
         // of another team AND the two teams have different names
         // to avoid duplicate values
         if(teams[teamChoice][2] == teams[teamIndex][2] &&
            teams[teamChoice][0] != teams[teamIndex][0])
         {
            // Cycle through winner list
            for(int winIndex = 0; winIndex < winners.size(); winIndex++)
            {
               // If team name matches winner name
               if(teams[teamIndex][0] == winners[winIndex][0])
               {
                  // Add win year to vector
                  years.push_back(winners[winIndex][1]);
               }
            }
         }
      }
   }
}

/*
########################################################################################################
FUNCTION: displayResults
PURPOSE: This function displays the total number of wins and the winning years for the chosen team. If 
   a team has won titles under more than one team name, the total number of wins are displayed along
   with a list of win years under each applicable team name.
@param teams         The 2D string vector containing team information
       years         The string vector that stores winning years for the chosen team
       teamChoice    The integer index value of the selected team
######################################################################################################## 
*/

void displayResults(vector<vector<string>> teams, vector<string> years, int teamChoice)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   // If name key indicates the team has won under other names
   if(stoi(teams[teamChoice][2]) > 0)
   {
      int winPosition = 0;          // To keep track of read position in years vector
      
      SetConsoleTextAttribute(hConsole, 242);
      cout << "\nThe " << teams[teamChoice][0] << " have won a total of " << years.size()
           << " World Series titles.\n" << endl;
      cout << "They won " << teams[teamChoice][3] << " World Series title(s) as the "
           << teams[teamChoice][0] << "." << endl;
      cout << "Winning year(s) as the " << teams[teamChoice][0] << ":" << endl;
      
      // Print each year chosen team won under user-selected team name
      for(int i = 0; i < stoi(teams[teamChoice][3]); i++)
      {
         cout << "\t" << years[i] << endl;
         winPosition++;
      } 
      
      // Cycle through each team
      for(int teamIndex = 0; teamIndex < teams.size(); teamIndex++)
      {
         // If the name key of the chosen team matches the name key
         // of another team AND the two teams have different names
         if(teams[teamChoice][2] == teams[teamIndex][2] &&
            teams[teamChoice][0] != teams[teamIndex][0])
         {
            cout << "\nThey also won " << teams[teamIndex][3] << " World Series "
                 << "title(s) as the " << teams[teamIndex][0] << "." << endl;         
            cout << "Winning year(s) as the " << teams[teamIndex][0] << ":" << endl;
         
            // Print each year team won under another team name
            for(int i = 0; i < stoi(teams[teamIndex][3]); i++)
            {
               cout << "\t" << years[winPosition + i] << endl;
            }
            
            // Update winPosition in case a team has won under more than two names
            winPosition += stoi(teams[teamIndex][3]);
         }
      }
      SetConsoleTextAttribute(hConsole, 249);          
   }
   
   else
   {
      SetConsoleTextAttribute(hConsole, 242);
      cout << "\nThe " << teams[teamChoice][0] << " have won a total of " << years.size()
           << " World Series title(s).\n" << endl;
      cout << "Winning year(s):" << endl;
      
      for(int i = 0; i < years.size(); i++)
         cout << "\t" << years[i] << endl;
      SetConsoleTextAttribute(hConsole, 249);          
   }   
}

/*
########################################################################################################
FUNCTION: searchAgain
PURPOSE: This function accepts and validates the user's choice to perform another search.
@param again     Reference variable that will store true if the user wants to search again, false if not
######################################################################################################## 
*/

void searchAgain(bool &again)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   char letter;
      
   do
   {
      cout << "\nDo you want perform another search? (Y/N)" << endl;
      SetConsoleTextAttribute(hConsole, 240);
      cin >> letter;
      SetConsoleTextAttribute(hConsole, 249);
      
      letter = (char)toupper(letter);  // Force character to be uppercase
   
      if(letter == 'Y')
      {
         again = true;
      }
      else if(letter == 'N')
      {
         again = false;
         cout << "\nEnding program.";
      }
      else
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid selection. Please try again.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
   }
   while (letter != 'Y' && letter != 'N');
}