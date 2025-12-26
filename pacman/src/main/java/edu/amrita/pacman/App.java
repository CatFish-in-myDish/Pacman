package edu.amrita.pacman;

/**
 * Hello world!
 *  
 */

import javax.swing.*;

public class App {
    public static void main(String[] args) {
        // Ensure GUI creation happens on the Event Dispatch Thread (EDT)
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                JFrame frame = new JFrame("My Swing App");
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.setSize(400, 300);

                JLabel label = new JLabel("Hello, Maven Swing!", SwingConstants.CENTER);
                frame.add(label);

                frame.setVisible(true);
            }
        });
    }
}
