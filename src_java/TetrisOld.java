import java.awt.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;


public class Tetris extends Applet implements KeyListener {

  public void init() {
             addKeyListener( this );
          }

  private static BufferedReader br = null;


          public void keyPressed( KeyEvent e ) { }
          public void keyReleased( KeyEvent e ) { }

  public void keyTyped( KeyEvent e ) {
    char c = e.getKeyChar();
    if ( c != KeyEvent.CHAR_UNDEFINED ) {
//      s = s + c;
//      repaint();
      e.consume();
    }
    System.out.println("you pressed " + c);
  }

  public static String sGetNextCommand() {

    String sInput = "";

    char[] ch = new char[128];

    // Wait for a command from standard input.

    try {
/*
      if (br.available() > 0) {
      System.out.println("BR is ready");
        int i = br.read(ch, 0, ch.length);
  System.out.println("read: " + ch + "("+i+")");
        if (i != -1)
          sInput = new String(ch);
      }
*/

      sInput = br.readLine();
    } catch (Exception e) {
      System.err.println("Error reading from standard input: " + e);
      System.exit(-1);
    }

    return sInput;
  }

  public static void main(String[] args) {

/*
    Frame frame = new Frame("Tetris");
    TextArea textArea = new TextArea(40, 40);
    frame.add(textArea);
    frame.show();
*/

      /*

    JFrame frame = new JFrame("SwingApplication");
    frame.addWindowListener(
      new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          System.exit(0);
        }
      }
    );

    JPanel pane = new JPanel();
    pane.setBorder(BorderFactory.createEmptyBorder(30, 30, 10, 30));
    pane.setLayout(new GridLayout(0, 1));

    //    pane.add(button);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                numClicks++;
                label.setText(labelPrefix + numClicks);
            }
        });
       
    frame.getContentPane().add(pane, BorderLayout.CENTER);

                //Finish setting up the frame, and show it.
                frame.pack();
                frame.setVisible(true);
      */
    /*******************************************/

    // Construct a buffered reader of the standard input.
    try {
      br = new BufferedReader(new InputStreamReader(System.in));
    } catch (Exception e) {
      System.err.println("Error initializing standard input reader: " + e);
      System.exit(-1);
    }

    TetrisBoard tb = new TetrisBoard();

    while (true) {
      String s = "\n\n\n";
//      textArea.setText(tb.toString());
      System.out.println(s + tb);
      System.out.print("COMMAND: ");
      s = sGetNextCommand();
      if ( (s != null) && (s.length() > 0) ) {
        char c = s.toUpperCase().charAt(0);
        switch (c) {
          case 'Q':
          case 'X':
            // Close the standard input buffer.
            try {
              br.close();
            } catch (Exception e) {
              System.err.println("Error closing buffered input reader: " + e);
            }
            System.exit(0);
            break;
          case 'S':
            tb.bMoveLeft();
            break;
          case 'D':
            tb.bMoveRight();
            break;
          case 'F':
            tb.bRotate(1);
            break;
          case 'A':
            tb.bRotate(-1);
            break;
          default:
            break;
        }
      } else {
        if (!tb.bMoveDownOne()) {
          tb.startNewBlock();
          if (tb.bBlockCollision()) {
            System.out.println("Game Over");
            System.exit(0);
          }
          tb.clearFullRows();
        }
      }
    }
  }

}
