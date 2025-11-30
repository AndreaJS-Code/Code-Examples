/**
 ########################################################################################################
 PROGRAM: ajsShipFleet
 FILE: Ship.java

 PURPOSE: This class stores information about Ship objects and provides the superclass structure to
    the CruiseShip and CargoShip subclasses.
 ########################################################################################################
 */

public class Ship
{
    private String shipName;            // To hold the name of the vessel
    private String yearBuilt;           // To hold the year in which the vessel was constructed

    /**
     ########################################################################################################
     MEMBER: Ship No-Arg Constructor
     PURPOSE: Creates a new Ship object with no parameters; necessary for the subclass constructors
     ########################################################################################################
     */

    public Ship()
    {

    }

    /**
     ########################################################################################################
     MEMBER: Ship Constructor
     PURPOSE: Creates a new Ship object.
     @param   shipName          The name of the ship.
     @param   yearBuilt         The year in which the ship was built.
     ########################################################################################################
     */

    public Ship(String shipName, String yearBuilt)
    {
        this.shipName = shipName;
        this.yearBuilt = yearBuilt;
    }

    /**
     ########################################################################################################
     MEMBER: setShipName
     PURPOSE: Mutator method that sets the name of the ship.
     @param   shipName          The name of the ship.
     ########################################################################################################
     */

    public void setShipName(String shipName)
    {
        this.shipName = shipName;
    }

    /**
     ########################################################################################################
     MEMBER: setYearBuilt
     PURPOSE: Mutator method that sets the year in which the ship was built.
     @param   yearBuilt          The year in which the ship was built.
     ########################################################################################################
     */

    public void setYearBuilt(String yearBuilt)
    {
        this.yearBuilt = yearBuilt;
    }

    /**
     ########################################################################################################
     MEMBER: getShipName
     PURPOSE: Accessor method that retrieves the name of the ship.
     @return  A String representing the name of the ship.
     ########################################################################################################
     */

    public String getShipName()
    {
        return shipName;
    }

    /**
     ########################################################################################################
     MEMBER: getYearBuilt
     PURPOSE: Accessor method that retrieves the year in which the ship was built.
     @return  A String representing the year in which the ship was built.
     ########################################################################################################
     */

    public String getYearBuilt()
    {
        return yearBuilt;
    }

    /**
     ########################################################################################################
     MEMBER: toString
     PURPOSE: Accessor method to print all the information related to the Ship object.
     @return  A formatted String containing the ship's name and the year it was built in columns.
     ########################################################################################################
     */

    public String toString()
    {
        String str = String.format("%-20s %-20s%n", shipName, yearBuilt);
        return str;
    }
}
