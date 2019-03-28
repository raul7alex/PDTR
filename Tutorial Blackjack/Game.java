import java.io.*;
import java.awt.*;
import javax.swing.*;
import java.util.Random;
import javax.imageio.ImageIO;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.awt.event.ActionListener;


public class Game{
	public static void main(String[] args)
	{
		Main m=new Main();
	}
}
class About extends JFrame
{
	public About()
	{
		super("About" );
		crFrame();
	}
	private void crFrame()
	{
		JTextArea tf=new JTextArea("",5,10);
		tf.setEditable(false);
		tf.setBackground(new Color(224,224,224));
		
		tf.setText("BlackJack \n\n   New Game: Menu -> New \n   Change Name: Menu -> Name \n\n Rules (Short Version): \n Start with two cards , try to get as close as you can to 21 , without going over. \n Card values: \n A = 11 or 1; \n J, Q, K = 10; \n 2 -> 10 = number on card \n\n   HIT to draw a card. \n   STAY to stop and wait for computer`s turn. \n   DEAL to deal new cards. \n\n Written in Notepad++ \n https://notepad-plus-plus.org");
		
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setIconImage(new ImageIcon("./carti/pic.png").getImage());
		
		setPreferredSize(new Dimension(450, 330));
		setLayout(new BorderLayout());
		add(BorderLayout.CENTER, tf);
		
		setResizable(false);
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
	}
}
class Main extends JFrame
{
	private int plcard1,plcard2,compc1,compc2,plscore,compscore,hitv=2,comphitv=2;
	private JLabel win = new JLabel("BlackJack - Computer WINS!");
	private JLabel background,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10;
	private JLabel dc1,dc2,dc3,dc4,dc5,dc6,dc7,dc8,dc9,dc10;
	private JMenuItem aboutItem = new JMenuItem("About");
	private JMenuItem quitItem = new JMenuItem("Quit");
	private JMenuItem setName = new JMenuItem("Name");
	private JMenuItem newItem = new JMenuItem("New");
	private JLayeredPane lpane = getLayeredPane();
	private JLabel computerS = new JLabel("0");
	private JLabel playerS = new JLabel("0");
	private JButton stay = new JButton("STAY");
	private JButton deal = new JButton("DEAL");
	private JButton hit = new JButton("HIT");
	private JLabel pl=new JLabel("PLAYER");
	private Random rand = new Random();
	private JMenuItem menuItem;
	private JMenuBar menuBar;
	private	JMenu menu, help;
		
	private int compcard2;
	public Main()
	{
		super("BlackJack (21)" );
		crFrame();
	}
	
	private void crFrame()
	{
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setIconImage(new ImageIcon("./carti/pic.png").getImage());
		setPreferredSize(new Dimension(730, 460));
		
		initMenu();
		initTable();
		initCards();
		
		setResizable(false);
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
		
		  aboutItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
               About a= new About();
            }
        });
		 newItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
               newGame();
            }
        });
		
		 quitItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
              System.exit(0);
            }
        });
		
		setName.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
				setName();            }
        });
		
		 deal.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
               newGame();
            }
        });
		stay.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
				stayMethod();
            }
        });
		hit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
			   int card=0;
			   hitv++;
			   switch (hitv)
			   {
				case 3:
				{
					card = setCarte(c3,rand.nextInt(52)+2);
					c3.setVisible(true);
					plscore = plcard1 + plcard2;
					if (plcard1==11 || plcard2 == 11)
					{
						if (plscore + card>21)
							plscore-=10;
					}
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;
				}
				case 4:
				{
					card = setCarte(c4,rand.nextInt(52)+2);
					c4.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				case 5:
				{
					card = setCarte(c5,rand.nextInt(52)+2);
					c5.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				case 6:
				{
					card = setCarte(c6,rand.nextInt(52)+2);
					c6.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				case 7:
				{
					card = setCarte(c7,rand.nextInt(52)+2);
					c7.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				case 8:
				{
					card = setCarte(c8,rand.nextInt(52)+2);
					c8.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				case 9:
				{
					card = setCarte(c9,rand.nextInt(52)+2);
					c9.setVisible(true);
					if (card==11 &&  plscore + card <= 21)
						{
							playerS.setText(Integer.toString(plscore + 11));
							plscore+=11;
						}
					else if (card==11 && plscore + card > 21)
						{
							playerS.setText(Integer.toString(plscore + 1));
							plscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(plscore+card));
							plscore+=card;
						}
					
					break;

				}
				   
			   }
			   try
			   {
				Thread.sleep(100);
			   }
			   catch (Exception e)
			   {
			   }
			   checkScore(plscore,1);
            }
        });
	}
	private void stayMethod()
	{
				setCarte(dc2,compcard2);
				computerMove();
				stay.setEnabled(false);
				hit.setEnabled(false);
				
	}
	private void computerMove()
	{
		compscore=compc1 + compc2;
		if (plscore==21)
		{
			if (compscore==20)
			{
					int hitsaunu = rand.nextInt(2);			
					if (hitsaunu==0)
							{	
								compHit();
								try
								{
									Thread.sleep(100);
								}
								catch (Exception e)
								{
								}
							}
					else
						finishGame();		
			}
			else
				while (compscore<21)
				{
				
					compHit();
						try
					{
						Thread.sleep(100);
					}
					catch (Exception e)
					{
					}
				}
			
			finishGame();
		}
		else
		{
			if (compscore >= plscore && comphitv < hitv)
				finishGame();
			else if  (compscore >= plscore && comphitv == hitv)
			{
				compHit();
					try
					{
						Thread.sleep(100);
					}
					catch (Exception e)
					{
					}
			}
			else if (compscore < plscore)	
				while (compscore <21 && compscore < plscore)
				{
				
					compHit();
					try
					{
						Thread.sleep(100);
					}
					catch (Exception e)
					{
					}
				
				}
			else if (compscore == plscore && comphitv > hitv)
			{
				if (compscore<17) compHit();
			}
		}
		finishGame();
	}
	private boolean compBust()
	{
		if (compscore <21)
			return false;
		else
		{
			finishGame();
			return true;
		}
	}
	
	private void finishGame()
	{
		hit.setEnabled(false);
		stay.setEnabled(false);
		
		if ((compscore) > 21)
		{
			wins("BUST - " + pl.getText() +  " Wins!",1,plscore);
		}
		else if (compscore == plscore)
		{
			if (comphitv < hitv)
				wins(" Computer Wins!",0,compscore);
			else if (comphitv == hitv)
				wins(" Draw game!",2,compscore);
			else 
				wins (pl.getText() +  " Wins!" , 1, plscore);
		}
		else if (compscore > plscore)
		{
			wins(" Computer Wins!",0,compscore);
			computerS.setVisible(true);
		}
			
	}
	
	private void compHit()
	{
			   int card=0;
			   comphitv++;
			   switch (comphitv)
			   {
				case 3:
				{
					card = setCarte(dc3,rand.nextInt(52)+2);
					dc3.setVisible(true);
					if (compc1==11 || compc2 == 11)
					{
						if (compscore + card>21)
							compscore-=10;
					}
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;
				}
				case 4:
				{
					card = setCarte(dc4,rand.nextInt(52)+2);
					dc4.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				case 5:
				{
					card = setCarte(dc5,rand.nextInt(52)+2);
					dc5.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				case 6:
				{
					card = setCarte(dc6,rand.nextInt(52)+2);
					dc6.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				case 7:
				{
					card = setCarte(dc7,rand.nextInt(52)+2);
					dc7.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				case 8:
				{
					card = setCarte(dc8,rand.nextInt(52)+2);
					dc8.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				case 9:
				{
					card = setCarte(dc9,rand.nextInt(52)+2);
					dc9.setVisible(true);
					if (card==11 &&  compscore + card <= 21)
						{
							playerS.setText(Integer.toString(compscore + 11));
							compscore+=11;
						}
					else if (card==11 && compscore + card > 21)
						{
							playerS.setText(Integer.toString(compscore + 1));
							compscore+=1;
						}
					else
						{
							playerS.setText(Integer.toString(compscore+card));
							compscore+=card;
						}
					
					break;

				}
				   
			   }
			   checkScore(compscore,2);
    }
	
	private void checkScore(int score,int source)
	{
		if (source == 1)
		{
					if ((score) > 21)
					{
						wins("BUST - Computer Wins!",0,compscore);
					}
					else if (score == 21)
					{
						hit.setEnabled(false);
						
					}
		}
		else if (source==2)
		{
					if ((score) > 21)
					{
						wins("BUST - " + pl.getText() +  " Wins!",1,plscore);
					}
					else if (score == 21)
					{
						finishGame();
					}
			
		}
	}
	private void initMenu()
	{
		menu = new JMenu("Menu");
		help = new JMenu("Help");
		
		menuBar = new JMenuBar();
		
		menuBar.add(menu);
		menuBar.add(help);
		
		menu.add(newItem);
		menu.add(setName);
		menu.add(quitItem);
		help.add(aboutItem);
		
		setJMenuBar(menuBar);
	}
	private void initTable()
	{
		background = new JLabel(new ImageIcon("./carti/bk2.png"));
		background.setBounds(0,0,730,460);
		lpane.add(background,new Integer(1));
		
		pl.setForeground(Color.WHITE);
	}
	private void initCards()
	{
		c1 = new JLabel(new ImageIcon("./carti/spate.png"));
		c2 = new JLabel(new ImageIcon("./carti/spate.png"));
		c3 = new JLabel(new ImageIcon("./carti/spate.png"));
		c4 = new JLabel(new ImageIcon("./carti/spate.png"));
		c5 = new JLabel(new ImageIcon("./carti/spate.png"));
		c6 = new JLabel(new ImageIcon("./carti/spate.png"));
		c7 = new JLabel(new ImageIcon("./carti/spate.png"));
		c8 = new JLabel(new ImageIcon("./carti/spate.png"));
		c9 = new JLabel(new ImageIcon("./carti/spate.png"));
		
		dc1 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc2 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc3 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc4 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc5 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc6 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc7 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc8 = new JLabel(new ImageIcon("./carti/spate.png"));
		dc9 = new JLabel(new ImageIcon("./carti/spate.png"));
		
		c1.setBounds(350, 250, 100,100); // carti player  - 1
		c2.setBounds(270, 250, 100,100); // 2
		c3.setBounds(450,230,100,100); // hit card 1
		c4.setBounds(450,205,100,100); // hit card 2
		c5.setBounds(450,180,100,100); // hit card 3
		c6.setBounds(450,155,100,100); // hit card 4
		c7.setBounds(450,130,100,100); // hit card 5
		c8.setBounds(450,105,100,100); // hit card 6
		c9.setBounds(450,80,100,100); // hit card 7
		
		dc1.setBounds(270,50,100,100); //dealer card 1
		dc2.setBounds(350,50,100,100); //dealer card 1
		dc3.setBounds(170,70,100,100); // hit card 1
		dc4.setBounds(170,95,100,100); // hit card 2
		dc5.setBounds(170,120,100,100); // hit card 3
		dc6.setBounds(170,145,100,100); // hit card 4
		dc7.setBounds(170,170,100,100); // hit card 5
		dc8.setBounds(170,195,100,100); // hit card 6
		dc9.setBounds(170,220,100,100); // hit card 7
		
		pl.setBounds(335,370,50,50); //player name
		
		hit.setBounds(430,375,65,20); //buttons
		stay.setBounds(510,370,65,20);
		deal.setBounds(590,365,65,20);
		
		win.setBounds(280,150,200,100); //win text
		
		playerS.setBounds(600,150,100,100); //score player
		playerS.setForeground(Color.WHITE);
		playerS.setFont(new Font("Serif", Font.PLAIN, 20));
		
		computerS.setBounds(110,150,100,100); //score computer 
		computerS.setForeground(Color.WHITE);
		computerS.setFont(new Font("Serif", Font.PLAIN, 20));
		
		 
		win.setForeground(Color.WHITE); //win text
		win.setFont(new Font("Serif", Font.PLAIN, 16));
		
		hit.setEnabled(false);
		stay.setEnabled(false);
		deal.setEnabled(false);
		win.setVisible(false);
		
		lpane.add(c1,new Integer(2)); //lpane = layerd pane , pentru a putea suprapune jlabel peste jlabel (ex peste background poze cu carti);
		lpane.add(c2,new Integer(3));
		lpane.add(c3,new Integer(11));
		lpane.add(c4,new Integer(12));
		lpane.add(c5,new Integer(13));
		lpane.add(c6,new Integer(14));
		lpane.add(c7,new Integer(15));
		lpane.add(c8,new Integer(16));
		lpane.add(c9,new Integer(17));
		
		lpane.add(dc1,new Integer(18));
		lpane.add(dc2,new Integer(19));
		lpane.add(dc3,new Integer(20));
		lpane.add(dc4,new Integer(21));
		lpane.add(dc5,new Integer(22));
		lpane.add(dc6,new Integer(23));
		lpane.add(dc7,new Integer(24));
		lpane.add(dc8,new Integer(25));
		lpane.add(dc9,new Integer(26));
		
		lpane.add(pl,new Integer(6));
		lpane.add(hit,new Integer(7));
		lpane.add(stay,new Integer(7));
		lpane.add(deal,new Integer(8));
		lpane.add(win,new Integer(9));
		lpane.add(playerS,new Integer(10));
		lpane.add(computerS, new Integer(27));
		
		computerS.setVisible(false);
		c3.setVisible(false);
		c4.setVisible(false);
		c5.setVisible(false);
		c6.setVisible(false);
		c7.setVisible(false);
		c8.setVisible(false);
		c9.setVisible(false);
		
		dc3.setVisible(false);
		dc4.setVisible(false);
		dc5.setVisible(false);
		dc6.setVisible(false);
		dc7.setVisible(false);
		dc8.setVisible(false);
		dc9.setVisible(false);
	}
	private void newGame()
	{
		hitv=2;
		comphitv=2;
		plscore=0;
		compscore=0;
		c3.setVisible(false);
		c4.setVisible(false);
		c5.setVisible(false);
		c6.setVisible(false);
		c7.setVisible(false);
		c8.setVisible(false);
		c9.setVisible(false);
		
		dc3.setVisible(false);
		dc4.setVisible(false);
		dc5.setVisible(false);
		dc6.setVisible(false);
		dc7.setVisible(false);
		dc8.setVisible(false);
		dc9.setVisible(false);
		
		computerS.setVisible(false);
		win.setVisible(false);
		
		compcard2=rand.nextInt(52)+2;
		plcard1 = setCarte(c1,rand.nextInt(52)+2);
		plcard2 = setCarte(c2,rand.nextInt(52)+2);
		compc1 = setCarte(dc1,rand.nextInt(52)+2);
		compc2 = setCarte(dc2,compcard2);
		setCarte(dc2,0);
		
		hit.setEnabled(true);
		stay.setEnabled(true);
		deal.setEnabled(true);
		
		if (blackJack(plcard1,plcard2) && blackJack(compc1,compc2))
		{
			wins("Game ends in tie",2,21);
			compscore=compc1 + compc2;
			computerS.setVisible(true);
		}
		else if (blackJack(compc1,compc2))
			{
				wins("BlackJack - Computer WINS!",0,21);
				setCarte(dc2,compcard2);
				computerS.setText("21");
				computerS.setVisible(true);
			}
		else if (blackJack(plcard1,plcard2))
			{
				wins("BlackJack - " + pl.getText() + " WINS!",1,21);
				setCarte(dc2,compcard2);
				compscore=compc1 + compc2;
				computerS.setText(Integer.toString(compscore));
				computerS.setVisible(true);
			}
		plscore=plcard1 + plcard2;

		playerS.setText(Integer.toString(plscore));
		
	}
	
	private void wins(String s,int x,int score)
	{
		win.setText(s);
		win.setVisible(true);
		hit.setEnabled(false);
		stay.setEnabled(false);
		
		playerS.setText(Integer.toString(plscore));
		computerS.setText(Integer.toString(compscore));

		computerS.setVisible(true);
	}
	private boolean blackJack(int a , int b)
	{
		if ((a==11 && b==10) || (a==10 && b==11) || (a==11 && b==11))
			return true;
		return false;
	}
	
	private int setCarte(JLabel carte, int numar)
	{
		int returnat=0;
		switch (numar)
		{
		case 0:
			{
				ImageIcon icon = new ImageIcon("./carti/spate.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=0;
				break;
			}
		case 2: 
			{
				ImageIcon icon = new ImageIcon("./carti/2i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=2;
				break;
			}
		case 3:
			{
				ImageIcon icon = new ImageIcon("./carti/2n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=2;
				break;
			}
		case 4:
			{
				ImageIcon icon = new ImageIcon("./carti/2r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=2;
				break;
			}
		case 5:
			{
				ImageIcon icon = new ImageIcon("./carti/2t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=2;
				break;
			}
		case 6:
			{
				ImageIcon icon = new ImageIcon("./carti/3i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=3;
				break;
			}
		case 7:
			{
				ImageIcon icon = new ImageIcon("./carti/3n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=3;
				break;
			}
		case 8:
			{
				ImageIcon icon = new ImageIcon("./carti/3r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=3;
				break;
			}
		case 9:
			{
				ImageIcon icon = new ImageIcon("./carti/3t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=3;
				break;
			}
		case 10:
			{
				ImageIcon icon = new ImageIcon("./carti/4i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=4;
				break;
			}
		case 11:
			{
				ImageIcon icon = new ImageIcon("./carti/4n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=4;
				break;
			}
		case 12:
			{
				ImageIcon icon = new ImageIcon("./carti/4r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=4;
				break;
			}
		case 13:
			{
				ImageIcon icon = new ImageIcon("./carti/4t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=4;
				break;
			}
		case 14:
			{
				ImageIcon icon = new ImageIcon("./carti/5i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=5;
				break;
			}
		case 15:
			{
				ImageIcon icon = new ImageIcon("./carti/5n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=5;
				break;
			}
		case 16:
			{
				ImageIcon icon = new ImageIcon("./carti/5r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=5;
				break;
			}
		case 17:
			{
				ImageIcon icon = new ImageIcon("./carti/5t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=5;
				break;
			}
		case 18:
			{
				ImageIcon icon = new ImageIcon("./carti/6i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=6;
				break;
			}
		case 19:
			{
				ImageIcon icon = new ImageIcon("./carti/6n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=6;
				break;
			}
		case 20:
			{
				ImageIcon icon = new ImageIcon("./carti/6r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=6;
				break;
			}
		case 21:
			{
				ImageIcon icon = new ImageIcon("./carti/6t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=6;
				break;
			}
		case 22:
			{
				ImageIcon icon = new ImageIcon("./carti/7i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=7;
				break;
			}
		case 23:
			{
				ImageIcon icon = new ImageIcon("./carti/7n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=7;
				break;
			}
		case 24:
			{
				ImageIcon icon = new ImageIcon("./carti/7r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=7;
				break;
			}
		case 25:
			{
				ImageIcon icon = new ImageIcon("./carti/7t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=7;
				break;
			}
		case 26:
			{
				ImageIcon icon = new ImageIcon("./carti/8i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=8;
				break;
			}
		case 27:
			{
				ImageIcon icon = new ImageIcon("./carti/8n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=8;
				break;
			}
		case 28:
			{
				ImageIcon icon = new ImageIcon("./carti/8r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=8;
				break;
			}
		case 29:
			{
				ImageIcon icon = new ImageIcon("./carti/8t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=8;
				break;
			}
		case 30:
			{
				ImageIcon icon = new ImageIcon("./carti/9i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=9;
				break;
			}
		case 31:
			{
				ImageIcon icon = new ImageIcon("./carti/9n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=9;
				break;
			}
		case 32:
			{
				ImageIcon icon = new ImageIcon("./carti/9r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=9;
				break;
			}
		case 33:
			{
				ImageIcon icon = new ImageIcon("./carti/9t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=9;
				break;
			}
		case 34:
			{
				ImageIcon icon = new ImageIcon("./carti/10i.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 35:
			{
				ImageIcon icon = new ImageIcon("./carti/10n.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 36:
			{
				ImageIcon icon = new ImageIcon("./carti/10r.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 37:
			{
				ImageIcon icon = new ImageIcon("./carti/10t.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 38:
			{
				ImageIcon icon = new ImageIcon("./carti/ai.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=11;
				break;
			}
		case 39:
			{
				ImageIcon icon = new ImageIcon("./carti/an.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=11;
				break;
			}
		case 40:
			{
				ImageIcon icon = new ImageIcon("./carti/ar.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=11;
				break;
			}
		case 41:
			{
				ImageIcon icon = new ImageIcon("./carti/at.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=11;
				break;
			}
		case 42:
			{
				ImageIcon icon = new ImageIcon("./carti/qi.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 43:
			{
				ImageIcon icon = new ImageIcon("./carti/qn.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 44:
			{
				ImageIcon icon = new ImageIcon("./carti/qr.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 45:
			{
				ImageIcon icon = new ImageIcon("./carti/qt.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 46:
			{
				ImageIcon icon = new ImageIcon("./carti/ji.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 47:
			{
				ImageIcon icon = new ImageIcon("./carti/jn.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 48:
			{
				ImageIcon icon = new ImageIcon("./carti/jr.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 49:
			{
				ImageIcon icon = new ImageIcon("./carti/jt.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 50:
			{
				ImageIcon icon = new ImageIcon("./carti/ki.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 51:
			{
				ImageIcon icon = new ImageIcon("./carti/kn.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		case 52:
			{
				ImageIcon icon = new ImageIcon("./carti/kr.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
				
			}
		case 53:
			{
				ImageIcon icon = new ImageIcon("./carti/kt.png");
				icon.getImage().flush();
				carte.setIcon(icon);
				returnat=10;
				break;
			}
		
		}
	return returnat;
	}
	private void setName()
	{
		String nume= JOptionPane.showInputDialog(null, "Name: ").toUpperCase();
		if (nume!=null)
			pl.setText(nume);	
	}
}
