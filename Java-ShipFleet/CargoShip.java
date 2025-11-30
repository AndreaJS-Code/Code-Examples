/**
 ########################################################################################################
 PROGRAM: ajsShipFleet
 FILE: CargoShip.java

 PURPOSE: This class stores information about CargoShip objects. CargoShip is a subclass of the Ship
    superclass.
 ########################################################################################################
 */
public class CargoShip extends Ship
{
    private int tonnageCapacity;            // To hold the cargo capacity (in tonnage) of the cargo ship

    /**
     ########################################################################################################
     MEMBER: CargoShip Constructor
     PURPOSE: Creates a new CargoShip object.
     @param   shipName          The name of the ship.
     @param   tonnageCapacity   The cargo capacity (in tonnage) of the ship.
     ########################################################################################################
     */

    public CargoShip(String shipName, int tonnageCapacity)
    {
        setShipName(shipName);
        this.tonnageCapacity = tonnageCapacity;
    }

    /**
     ########################################################################################################
     MEMBER: setTonnageCapacity
     PURPOSE: Mutator method that sets the cargo capacity (in tonnage) of the ship.
     @param   tonnageCapacity          The cargo capacity (in tonnage) of the ship.
     ########################################################################################################
     */

    public void setTonnageCapacity(int tonnageCapacity)
    {
        this.tonnageCapacity = tonnageCapacity;
    }

    /**
     ########################################################################################################
     MEMBER: getTonnageCapacity
     PURPOSE: Accessor method that retrieves the cargo capacity (in tonnage) of the ship.
     @return  An int representing the cargo capacity (in tonnage) of the ship.
     ########################################################################################################
     */

    public int getTonnageCapacity()
    {
        return tonnageCapacity;
    }

    /**
     ########################################################################################################
     MEMBER: toString
     PURPOSE: Accessor method to print all the information related to the CargoShip object. Overrides the
     toString method in the Ship superclass. tonnageCapacity is printed with comma separators.
     @return  A formatted String containing the ship's name and tonnage capacity in columns.
     ########################################################################################################
     */

    @Override
    public String toString()
    {
        String str = String.format("%-20s %-20s%n", getShipName(), String.format("%,d",tonnageCapacity));
        return str;
    }
}
