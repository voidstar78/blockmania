import java.awt.*;
import java.awt.event.*;

public class TetrisUI extends Frame {

  public static void main(String args[]) { 
    new TetrisUI(); 
  }

  public TetrisUI() {
  
    setSize(200,200);  
    setVisible(true); 

  }

  public void paint(Graphics g) {
     g.drawString("Test", 0, 0);
  
  }

}
