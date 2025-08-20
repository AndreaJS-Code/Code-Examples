import javax.swing.JOptionPane; //Necessary to use dialog boxes
import javax.swing.JLabel; //For adding the input dialog message
import javax.swing.JPanel; //For custom dialog panel
import javax.swing.JTextField; //To define a input box
import java.util.ArrayList; //To collect the list of test scores from the user

/**PROGRAM: TestAverageAndGradeAJS.java
WRITTEN BY: Andrea Sitton
FOR: CPS 121 Chapter 5 Lab

PURPOSE: This program accepts test values from the user, determines
   the letter grade associated with each test score, and then provides 
   the average value of the provided test scores.*/

public class TestAverageAndGradeAJS 
{
   public static void main(String[] args)
   {
      //Initialize variables
      ArrayList<Integer> scores = new ArrayList<Integer>(5); //ArrayList for storing test scores, starting size of 5
      int count = 0; //Counts the number of scores entered
      int input = -1; //Dialog box input, initial value used to stop program if window is closed
      
      //Create custom button text
      Object[] buttonsNew = {"Start Over", "Close"}; //Buttons for a new calculation or closing the program
      Object[] buttonsNext = {"Add Next Score", "Calculate"}; //Buttons for adding another value or calculating average
      Object[] buttonsError = {"Add Next Score", "Start Over", "Close"}; //Buttons for returning to value entry after error
      
      //Create custom panel for input dialog
      JPanel panel = new JPanel();
      panel.add(new JLabel("Enter test score.")); //This is the input dialog message
      JTextField textField = new JTextField(3);
      panel.add(textField); //This adds a text field for the user's input
        
      //**** Collect user-provided test scores ****      
      do
      {
         input = JOptionPane.showOptionDialog(null, panel, "Test Score Evaluator", JOptionPane.YES_NO_OPTION, 
            JOptionPane.QUESTION_MESSAGE, null, buttonsNext, null); //Stores user's button presses
         if (input == -1) //If the dialog box is closed without input, the execution is stopped
            System.exit(0);
                  
         //Parse user input into integer
         try
         {
            if (Integer.parseInt(textField.getText()) < 0 || Integer.parseInt(textField.getText()) > 100) throw new IllegalArgumentException();
               //Throws an exception if the user-entered value is a non-integer or if the integer is out of the acceptable range
            else
            {  
               scores.add(Integer.parseInt(textField.getText())); //Adds the user input from the text field into the ArrayList
               count++; //Increments to count number of test scores entered
               textField.setText(""); //Clears user data from text field in preparation to accept new value
            }
         }
         catch (IllegalArgumentException iae) //This catches non-integer input or input outside the acceptable range and returns an error
         {
            int choice = JOptionPane.showOptionDialog(panel, "Enter a valid integer between 0 and 100.", "Error", JOptionPane.YES_NO_CANCEL_OPTION,
               JOptionPane.ERROR_MESSAGE, null, buttonsError, null);
            if (choice == JOptionPane.YES_OPTION)
               continue; //This allows the user to correct the illegal argument and continue adding values
            else if (choice == JOptionPane.NO_OPTION)
               main(new String[0]); //If the user wants to start over, the main method is called again
            else
               System.exit(0); //If the user doesn't want to perform another calculation, execution is stopped 
         }
      }
      while (input == JOptionPane.YES_OPTION); //Loop continues so long as user has more values to enter
      
      //Send scores to determineGrade method and store returned letter grades into a String ArrayList
      ArrayList<String> letterGrades = new ArrayList<String>(determineGrade(scores));
      
      //Send scores to calcAverage method and store return average as a double
      double testAverage = (calcAverage(scores));

      //Format and store test score average
      ArrayList<Integer> intAverageScore = new ArrayList<Integer>(); //To match the required input parameters of determineGrade
      intAverageScore.add((int)Math.round(testAverage)); //Stores test score average into ArrayList as a rounded integer

      //Send test score average to determineGrade method and store returned letter grade into String ArrayList     
      ArrayList<String> averageLetter = new ArrayList<String>(determineGrade(intAverageScore));
         //To match the return type of the determineGrade method
      
      //Format ArrayLists as a series of comma separated values for display purposes
      String testScoresCSV = scores.toString();
      String letterGradesCSV = String.join(", ", letterGrades);
      
      //Format test average to display with one decimal place
      String averageScore = String.format("%.1f",testAverage);
      
      //Display results
      int choice = JOptionPane.showOptionDialog(panel, "Entered test scores: \n        " + testScoresCSV 
         + ".\n Corresponding letter grades: \n        [" + letterGradesCSV + "].\n\n The average of " 
         + count + " scores is " + averageScore + ", which corresponds to a letter grade of " 
         + averageLetter.get(0) + ".", "Test Score Evaluator Results", JOptionPane.YES_NO_OPTION,
         + JOptionPane.INFORMATION_MESSAGE, null, buttonsNew, null);      
      if (choice == JOptionPane.YES_OPTION)
         main(new String[0]); //If the user wants to perform another calculation, the main method is called again
      else
         System.exit(0); //If the user doesn't want to perform another calculation, execution is stopped   
   }
   
   /**
   METHOD: determineGrade
   PURPOSE: This method accepts the ArrayList of user-entered scores, determines the letter grade
      associated with each score, and stores these letter grades in an separate ArrayList.
   @param scores The Integer ArrayList containing user-entered values
   @return The letter grades in a String ArrayList
   */
   
   public static ArrayList<String> determineGrade(ArrayList<Integer> scores)
   {
      //Initialize variables
      ArrayList<String> letterGrades = new ArrayList<String>(5); //String ArrayList for storing letter grades
           
      for(int i : scores) //Cycles through the ArrayList values
      {
         if (i < 60)
            letterGrades.add("F");
         else if (i <= 69)
            letterGrades.add("D");
         else if (i <= 79)
            letterGrades.add("C");
         else if (i <= 89)
            letterGrades.add("B");
         else if (i <= 100)
            letterGrades.add("A");
         else //This should never get triggered if my exception catch statements work properly
            letterGrades.add("INVALID ENTRY");
      }
     
      return letterGrades;
   }
   
   /**
   METHOD: calcAverage
   PURPOSE: This method accepts the ArrayList of user-entered scores, averages the values, and
      returns the numerical average.
   @param scores The Integer ArrayList containing user-entered values
   @return The average of the user-entered test scores as a double
   */
   
   public static double calcAverage(ArrayList<Integer> scores)
   {
      //Initialize variables
      double testAverage = 0;
      double totalPoints = 0;
      int count = 0;
      
      for(int i : scores) //Cycles through the ArrayList values
      {
         totalPoints += i; //Adds score to total points
         count++; //Increments to count number of test scores entered
      }
      
      testAverage = totalPoints/count; //Divide total points by number of tests entered
      
      return testAverage;      
   }
   
}
