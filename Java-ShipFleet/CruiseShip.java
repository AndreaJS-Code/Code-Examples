/**
 ########################################################################################################
 PROGRAM: ajsShipFleet
 FILE: CruiseShip.java

 PURPOSE: This class stores information about CruiseShip objects. CruiseShip is a subclass of the Ship
    superclass.
 ########################################################################################################
 */

public class CruiseShip extends Ship
{
    private int maxPassengers;          // To hold the maximum number of passengers the cruise ship can hold

    /**
     ########################################################################################################
     MEMBER: CruiseShip Constructor
     PURPOSE: Creates a new CruiseShip object.
     @param   shipName          The name of the ship.
     @param   maxPassengers     The maximum number of passengers that the cruise ship can accommodate.
     ########################################################################################################
     */

    public CruiseShip(String shipName, int maxPassengers)
    {
        setShipName(shipName);
        this.maxPassengers = maxPassengers;
    }

    /**
     ########################################################################################################
     MEMBER: setMaxPassengers
     PURPOSE: Mutator method that sets the maximum number of passengers that the cruise ship can accommodate.
     @param   maxPassengers          The maximum number of passengers that the cruise ship can accommodate.
     ########################################################################################################
     */

    public void setMaxPassengers(int maxPassengers)
    {
        this.maxPassengers = maxPassengers;
    }

    /**
     ########################################################################################################
     MEMBER: getMaxPassengers
     PURPOSE: Accessor method that retrieves the maximum number of passengers that the cruise ship can
        accommodate.
     @return  An int representing the maximum number of passengers that the cruise ship can accommodate.
     ########################################################################################################
     */

    public int getMaxPassengers()
    {
        return maxPassengers;
    }

    /**
     ########################################################################################################
     MEMBER: toString
     PURPOSE: Accessor method to print all the information related to the CruiseShip object. Overrides the
        toString method in the Ship superclass. maxPassengers is printed with comma separators.
     @return  A formatted String containing the ship's name and maximum passengers in columns.
     ########################################################################################################
     */

    @Override
    public String toString()
    {
        String str = String.format("%-20s %-20s%n", getShipName(), String.format("%,d",maxPassengers));
        return str;
    }
}
