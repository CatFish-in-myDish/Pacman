package edu.amrita.elements;

/**
 * Represents a ghost in pacman
 * All 4 ghosts should implement this interface along with some required class members
 */
public interface Ghost {

    /**
     * Gets the name of the ghost
     * @return String The name of the ghost in String format
     */
    String getGhostName();

    /**
      * A shortest path algorithm should be implemented for moveGhost() method to reach the player
      * Each ghost should implement a different path finding algorithm.
      * 
      * @param targetPosX X coordinate value of the player in integer
      * @param targetPosY Y coordinate value of the player in integer
      * @param map 2d grid map 
     */
    void moveGhost(int targetPosX, int targetPosY, int[][] map);

    /**
     * Gives the current x coordinate value of the ghost
     * 
     * @return The x position of the ghost
     */
    int getPositionX();

    /**
     * Gives the current y coordinate value of the ghost
     * 
     * @return The y position of the ghost
     */
    int getPositionY();

    /**
     * Sets the ghosts position
     * 
     * @param x x coordinate of the ghost 
     * @param y y coordinate of the ghost
     */
    void setPosition(int x, int y);

    /**
     * Updates the ghosts position in the game, internally calls moveGhost, getPositionX, getPositionY and setPosition functions
     */
    void update();

}