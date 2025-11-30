import javax.swing.*;
import java.awt.*;
import java.time.Year;
import java.util.ArrayList;

/**
 ########################################################################################################
 PROGRAM: ajsShipFleet
 FILE: ajsMain.java
 WRITTEN BY: Andrea Sitton
 FOR: CPS 230 - Chapter 10 Lab

 PURPOSE: This program simulates a fleet management system. Users can add new ships or view the fleet
    information from the main menu. A few of each type of ship is preloaded into the array for more
    convenient demonstration of the program's capabilities.
 ########################################################################################################
 */

public class ajsMain
{
    public static void main(String[] args)
    {
        // Declare/initialize variables
        ArrayList<Ship> fleet = new ArrayList<>(6); // ArrayList to hold fleet information

        // Load starting ships to ArrayList
        loadFleet(fleet);

        // Create custom button text
        String[] buttonsMain = {"Add New Vessel", "View Fleet", "Close"};  // Buttons for the main interface screen

        do
        {
            // Get the user's choice of actions via the buttons
            int choice = JOptionPane.showOptionDialog(null, "What would you like to do?", "Fleet Management System",
                    JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, buttonsMain, buttonsMain[1]);

            // Check to see if user closed the dialog box or chose the option to exit the program
            closeProgramCheck(choice, JOptionPane.CLOSED_OPTION, 2);

            switch (choice)
            {
                // The user wants to add a vessel to the fleet
                case 0:
                    addVessel(fleet);
                    break;

                // The user wants to display the current fleet
                case 1:
                    displayFleet(fleet);
                    break;
            }
        }
        while (true);       // Keep looping. Loop exits when program terminates.
    }

    /**
     ########################################################################################################
     METHOD: loadFleet
     PURPOSE: This method adds six Ship objects (two Ship superclass, two CruiseShip subclass, and two
        CargoShip subclass) to the ArrayList for ease of program demonstration.
     @param   fleet      ArrayList holding the Ship objects.
     ########################################################################################################
     */

    public static void loadFleet(ArrayList<Ship> fleet)
    {
        // Add initial ships to the fleet
        fleet.add(new Ship("S.S. Java", "1995"));
        fleet.add(new CruiseShip("S.S. Python", 1991));
        fleet.add(new CargoShip("S.S. Sea Plus Plus", 1985));
        fleet.add(new CargoShip("S.S. Fortran", 1957));
        fleet.add(new Ship("S.S. BASIC", "1964"));
        fleet.add(new CruiseShip("S.S. COBOL", 1959));
    }

    /**
     ########################################################################################################
     METHOD: addVessel
     PURPOSE: This method allows the user to add vessels to the fleet. Users can add regular ships, cruise
        ships, or cargo ships. Input validation makes sure that the year built (Ships), max passengers
        (CruiseShips), or cargo capacity (CargoShips) make sense.
     @param   fleet      ArrayList holding the Ship objects.
     ########################################################################################################
     */

    public static void addVessel(ArrayList<Ship> fleet)
    {
        // Create custom button text for add ship dialog box
        String[] buttonsAdd = {"Ship", "Cruise Ship", "Cargo Ship", "Return to Menu"};  // Buttons for adding a vessel to the fleet
        // Create custom button text for error dialog boxes
        String[] buttonsError = {"Go Back", "Return to Menu", "Exit Program"};

        // Get the user's choice of actions via the buttons
        int choice = JOptionPane.showOptionDialog(null, "What kind of ship would you like to add?",
                "Fleet Management System", JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, buttonsAdd, buttonsAdd[3]);

        // Check to see if user closed the dialog box
        closeProgramCheck(choice, JOptionPane.CLOSED_OPTION);
        // User wants to return to the menu
        if (choice == 3)
            return;

        // Get the name of the ship
        String shipName = JOptionPane.showInputDialog("What is the ship name?");
        // Check to see if user closed the dialog box
        // If shipName is null, the user probably closed the dialog box. The ternary operator checks for a null and substitutes
        //      the int representing JOptionPane.CLOSED_OPTION as the argument being passed to the closeProgramCheck method.
        //      If shipName is not null, sending a value of 0 ensures that the operation continues.
        closeProgramCheck((shipName == null) ? JOptionPane.CLOSED_OPTION : 0, JOptionPane.CLOSED_OPTION);

        // To keep force the user to enter a valid value before proceeding
        boolean validInput = false;

        switch (choice)
        {
            // The user wants to add a Ship object to the fleet
            case 0:
                do
                {
                    try
                    {
                        String yearBuilt = JOptionPane.showInputDialog("What year was " + shipName + " constructed?");
                        // Check to see if user closed the dialog box
                        // If yearBuilt is null, the user probably closed the dialog box. The ternary operator checks for a null and substitutes
                        //      the int representing JOptionPane.CLOSED_OPTION as the argument being passed to the closeProgramCheck method.
                        //      If yearBuilt is not null, sending a value of 0 ensures that the operation continues.
                        closeProgramCheck((yearBuilt == null) ? JOptionPane.CLOSED_OPTION : 0, JOptionPane.CLOSED_OPTION);

                        // The oldest sea vessel that is still actively sailing is the USS Constitution, which was launched in 1797.
                        // The second half of the OR statement catches years that have not occurred yet.
                        if (Integer.parseInt(yearBuilt) < 1797 || Integer.parseInt(yearBuilt) > Year.now().getValue())
                            throw new IllegalArgumentException();
                        else
                        {
                            // Create new Ship object if input is valid
                            fleet.add(new Ship(shipName, yearBuilt));
                            validInput = true;
                        }
                    } catch (IllegalArgumentException iae)
                    {
                        int exChoice = JOptionPane.showOptionDialog(null,
                                "Please enter a valid year between 1797 and the current year.", "Error",
                                JOptionPane.DEFAULT_OPTION, JOptionPane.ERROR_MESSAGE, null, buttonsError, buttonsError[0]);
                        // Check to see if user closed the dialog box or chose the option to exit the program
                        closeProgramCheck(exChoice, JOptionPane.CLOSED_OPTION, 2);
                        // If the user wants to return to the menu, break from the switch statement
                        if (exChoice == 1)
                            break;
                        // Otherwise the user will be taken back to the input screen and asked again for the year built
                    }
                }
                while (!validInput);
                break;

            // The user wants to add a CruiseShip object to the fleet
            case 1:
                do
                {
                    try
                    {
                        int maxPassengers = Integer.parseInt(JOptionPane.showInputDialog("What is the maximum number of passengers "
                                + shipName + " can accommodate?"));

                        // Can't have a maximum occupancy of 0 or a negative number
                        // The largest known cruise ship can hold ~7,600 passengers and ~2,400 crew, so the realistic maximum for now is 10,000
                        if (maxPassengers < 1 || maxPassengers > 10000) throw new IllegalArgumentException();
                        else
                        {
                            // Create new CruiseShip object if input is valid
                            fleet.add(new CruiseShip(shipName, maxPassengers));
                            validInput = true;
                        }
                    } catch (IllegalArgumentException iae)
                    {
                        int exChoice = JOptionPane.showOptionDialog(null,
                                "Maximum number of passengers must be greater than zero but less than 10,000.", "Error",
                                JOptionPane.DEFAULT_OPTION, JOptionPane.ERROR_MESSAGE, null, buttonsError, buttonsError[0]);
                        // Check to see if user closed the dialog box or chose the option to exit the program
                        closeProgramCheck(exChoice, JOptionPane.CLOSED_OPTION, 2);
                        // If the user wants to return to the menu, break from the switch statement
                        if (exChoice == 1)
                            break;
                        // Otherwise the user will be taken back to the input screen and asked again for the max number of passengers
                    }

                }
                while (!validInput);
                break;

            // The user wants to add a CargoShip object to the fleet
            case 2:
                do
                {
                    try
                    {
                        int tonnageCapacity = Integer.parseInt(JOptionPane.showInputDialog("What is the maximum cargo capacity (in tons) that "
                                + shipName + " can haul?"));

                        // A cargo ship's hauling capacity should be a positive number
                        // The largest hauling vessel - an oil tanker - had a capacity of 565,000 tons, so that's a realistic maximum
                        if (tonnageCapacity < 1 || tonnageCapacity > 565000) throw new IllegalArgumentException();
                        else
                        {
                            fleet.add(new CargoShip(shipName, tonnageCapacity));
                            validInput = true;
                        }
                    } catch (IllegalArgumentException iae)
                    {
                        int exChoice = JOptionPane.showOptionDialog(null,
                                "Cargo capacity (in tons) must greater than zero but less than 565,000.", "Error",
                                JOptionPane.DEFAULT_OPTION, JOptionPane.ERROR_MESSAGE, null, buttonsError, buttonsError[0]);
                        // Check to see if user closed the dialog box or chose the option to exit the program
                        closeProgramCheck(exChoice, JOptionPane.CLOSED_OPTION, 2);
                        // If the user wants to return to the menu, break from the switch statement
                        if (exChoice == 1)
                            break;
                        // Otherwise the user will be taken back to the input screen and asked again for the max number of passengers
                    }
                }
                while (!validInput);
                break;

            // The user wants to return to the menu
            case 3:
                // Return to menu
        }
    }

    /**
     ########################################################################################################
     METHOD: displayFleet
     PURPOSE: This method displays the current fleet of Ship objects in the array. The objects are divided
        by type and displayed in separate tables in a single dialog box. Each object type is displayed with
        the appropriate column headers.
     @param   fleet      ArrayList holding the Ship objects.
     ########################################################################################################
     */

    public static void displayFleet(ArrayList<Ship> fleet)
    {
        // Build table formatting for the different kind of Ship objects
        StringBuilder genericShips = new StringBuilder();
        StringBuilder cruiseShips = new StringBuilder();
        StringBuilder cargoShips = new StringBuilder();

        // Add titles to each subtable
        genericShips.append("Standard Ships").append("\n\n");
        cruiseShips.append("Cruise Ships").append("\n\n");
        cargoShips.append("Cargo Ships").append("\n\n");

        // Add column labels and a divider between the header and the ship information
        genericShips.append(String.format("%-20s %-20s%n", "Ship Name", "Year Built"));
        genericShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));

        cruiseShips.append(String.format("%-20s %-20s%n", "Ship Name", "Max. Occupancy"));
        cruiseShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));

        cargoShips.append(String.format("%-20s %-20s%n", "Ship Name", "Capacity (tons)"));
        cargoShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));

        // Cycle through the array, sorting ships based on type and calling their toString method to add to the correct string
        for (Ship i : fleet)
        {
            if (i instanceof CruiseShip)
                cruiseShips.append(i);
            else if (i instanceof CargoShip)
                cargoShips.append(i);
            else
                genericShips.append(i);
        }

        // End the tables with another divider
        genericShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));
        cruiseShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));
        cargoShips.append(String.format("%-20s %-20s%n", "------------------", "------------------"));

        // Set text areas
        JTextArea genericArea = new JTextArea(String.valueOf(genericShips));
        JTextArea cruiseArea = new JTextArea(String.valueOf(cruiseShips));
        JTextArea cargoArea = new JTextArea(String.valueOf(cargoShips));

        // Fetch the default background color of the dialog boxes
        Color dialogBackground = UIManager.getColor("Panel.background");

        // Set fonts for each area
        genericArea.setFont(new Font("Monospaced", Font.PLAIN, 14));
        genericArea.setBackground(dialogBackground);
        genericArea.setEditable(false);

        cruiseArea.setFont(new Font("Monospaced", Font.PLAIN, 14));
        cruiseArea.setBackground(dialogBackground);
        cruiseArea.setEditable(false);

        cargoArea.setFont(new Font("Monospaced", Font.PLAIN, 14));
        cargoArea.setBackground(dialogBackground);
        cargoArea.setEditable(false);

        // Add these customized areas to the panel layout
        JPanel panel = new JPanel(new GridLayout(1, 2));
        panel.setBackground(dialogBackground);
        panel.add(genericArea);
        panel.add(cruiseArea);
        panel.add(cargoArea);

        // Button text for dialog box
        String[] buttonsDisplay = {"Return to Menu", "Close"};

        // Display dialog box with fleet information
        int choice = JOptionPane.showOptionDialog(null, panel,
                "Fleet Overview", JOptionPane.DEFAULT_OPTION, JOptionPane.INFORMATION_MESSAGE,
                null, buttonsDisplay, buttonsDisplay[0]);

        // Check to see if user closed the dialog box or chose the option to exit the program
        closeProgramCheck(choice, JOptionPane.CLOSED_OPTION, 1);
    }

    /**
     ########################################################################################################
     METHOD: closeProgramCheck
     PURPOSE: This method checks the int returned from a dialog box against the value that corresponds with
        JOptionPane.CLOSED_OPTION. If they match, the user closed out of the dialog box. This ensures that
        the program terminates appropriately.
     @param   choice        The int value returned from the dialog box after the user has taken action.
     @param   closedOption  The int value associated with JOptionPane.CLOSED_OPTION.
     ########################################################################################################
     */

    public static void closeProgramCheck(int choice, int closedOption)
    {
        if (choice == closedOption)
            System.exit(0);
    }

    /**
     ########################################################################################################
     METHOD: closeProgramCheck
     PURPOSE: This method checks the int returned from a dialog box against the value that corresponds with
        JOptionPane.CLOSED_OPTION as well as the value associated with an "Exit Program" button. This ensures
        that the program terminates appropriately.
     @param   choice        The int value returned from the dialog box after the user has taken action.
     @param   closedOption  The int value associated with JOptionPane.CLOSED_OPTION.
     @param   closedOption2 The int value associated with the "Exit Program" button.
     ########################################################################################################
     */

    public static void closeProgramCheck(int choice, int closedOption, int closedOption2)
    {
        if  (choice == closedOption || choice == closedOption2)
            System.exit(0);
    }
}

