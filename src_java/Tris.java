import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;

public class Tris {

  private static int ERR_NONE = 0;
  private static int ERR_INPUT = -1;
  private static int ERR_INITINPUT = -2;

  private static String sCommandInput;

  private static BufferedReader br = null;
  // Buffer to hold standard input strings.

  public static String sGetNextCommand() {
    String sInput = "";
    char[] ch = new char[128];

    // Wait for a command from standard input.
    try {
      sInput = br.readLine();
    } catch (Exception e) {
      System.err.println("Error reading from standard input: " + e);
      System.exit(ERR_INPUT);
    }

    return sInput;
  }

  public static void main(String[] args) {

    // Construct a buffered reader for the standard input.
    try {
      br = new BufferedReader(new InputStreamReader(System.in));
    } catch (Exception e) {
      System.err.println("Error initializing standard input reader: " + e);
      System.exit(ERR_INITINPUT);
    }

    TrisBoard tb = new TrisBoard();

    while (true) {
      System.out.println("\n\n\n" + tb.toString());
      System.out.print("COMMAND: ");
      String s = sGetNextCommand();
      if ( (s != null) && (s.length() > 0) ) {
        switch (s.toUpperCase().charAt(0)) {
          case 'Q':
          case 'X':  // EXIT or QUIT
            // Close the standard input buffer.
            try {
              br.close();
            } catch (Exception e) {
              System.err.println("Error closing buffered input reader: " + e);
            }
            System.exit(ERR_NONE);
            break;
          case 'S':  // MOVE LEFT
            tb.bMoveLeft();
            break;
          case 'D':  // MOVE RIGHT
            tb.bMoveRight();
            break;
          case 'F':  // ROTATE RIGHT
            tb.bRotate(1);
            break;
          case 'A':  // ROTATE LEFT
            tb.bRotate(-1);
            break;
          default:  // UNRECOGNIZED COMMAND
            break;
        }
      }

      if (!tb.bMoveDownOne()) {
        // The current piece could not be moved down,
        //   start a new block at the top.
        tb.startNewBlock();
        if (tb.bBlockCollision()) {
          System.out.println("Game Over");
          System.exit(0);
        }
        tb.clearFullRows();
      }

    }  // end main program loop

  }  // end method main(String[])

}  // end class Tris

