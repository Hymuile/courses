package fishGame;

import java.awt.Color;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class FishFrame extends Frame {
	public static void main(String[] args) {
		FishFrame fishFrame = new FishFrame();//�����������
		fishFrame.launchFrame();//������Ϸ
	}
	
	public static final int GAME_WIDTH = 1200;//��Ϸ���ڵĿ�
	public static final int GAME_HEIGHT = 800;//��Ϸ���ڵĸ�
	private Image offScreenImage = null;//��paint�������л�ͼǰ����ͼ��,�Ա�һ�λ���,����˫��������
	public static Random r = new Random();//����һ�������������
	private boolean isStart = false;//�ж���Ϸ�Ƿ�ʼ�ı���
	MyFish myFish = new MyFish(GAME_WIDTH/2, GAME_HEIGHT/2, this);//����һ���ҵ�С��
	List<EnemyFish> fishList_Left = new ArrayList<EnemyFish>();//װС��ļ���
	List<EnemyFish> fishList_Right = new ArrayList<EnemyFish>();//װС��ļ���

	/**
	 * ����ͼƬ
	 */
	public void paint(Graphics g) {
		/*
		 * ͨ��isStart�����ж���Ϸ�Ƿ�ʼ,û�п�ʼ�Ļ�������ӭ����
		 */
		if(!isStart){
			draw_welcome(g);
			return;
		}
		
		//�����ҷ�С��,����ҷ�С��Ե��˵о�С�����GOOD������ʾ
		myFish.draw(g);
		if(myFish.eatFishs(fishList_Left) || myFish.eatFishs(fishList_Right)){
			for(int i = 0; i < 5; i++){
				g.drawString("GOOD!", myFish.x, myFish.y);
			}
		}
		
		//�����ҷ�С����Ա��Ե�
		myFish.beEaten(fishList_Left);
		myFish.beEaten(fishList_Right);
		
		//�����з�С��
		for(int i = 0; i < fishList_Left.size(); i++) {
			EnemyFish e = fishList_Left.get(i);
			e.draw(g);
		}
		
		for(int i = 0; i < fishList_Right.size(); i++) {
			EnemyFish e = fishList_Right.get(i);
			e.draw(g);
		}
		
		//������ǰС�������͵�ǰ��һ�������
		g.setFont(new Font("����", Font.BOLD, 25));
		g.drawString("С������ : " + (fishList_Left.size()+fishList_Right.size()), 10, 50);
		g.drawString("���� : " + myFish.scores, 10, 70);
		if(myFish.scores > 500) {
			g.drawString("����Ϊ������ܱ��һ������Ա�������? Too Young! Too Simple!", 200, 350);
		}
		
		//���ҷ�С������ʱд��GAME OVER����
		if(!myFish.live){
			g.setColor(Color.RED);
			g.setFont(new Font("����", Font.BOLD, 70));
			g.drawString("����Ц����GG!!!", 300, 400);
		}
	}
	
	/**
	 * ������ӭ����
	 * @param g �������g
	 */
	private void draw_welcome(Graphics g) {
		g.setColor(Color.RED);
		g.setFont(new Font("����", Font.BOLD, 75));
		g.drawString("�����С��!!!", 100, 150);
		g.setFont(new Font("����", Font.BOLD, 40));
		g.setColor(Color.YELLOW);
		g.drawString("�淨:", 200, 350);
		g.drawString("ʹ�� �ϡ��¡����� �� �ƶ�,", 200, 400);
		g.drawString("�Ա��Լ����С����,", 200, 450);
		g.drawString("��ܱ��Լ���������.", 200, 500);
		g.drawString("����вʵ�!", 200, 600);
		g.drawString("���� �س� �����ɿ�ʼ...", 200, 650);
	}
	
	/**
	 * ��Ϸ��ʼ��
	 */
	private void launchFrame() {
		this.setLocation(50, 50);//������Ϸ���ڵ�λ��
		this.setSize(GAME_WIDTH, GAME_HEIGHT);//������Ϸ���ڵĿ��
		this.setVisible(true);//������Ϸ���ڿɼ�
		this.setResizable(false);//������Ϸ���ڴ�С���ɸı�
		this.setTitle("FishGame");//������Ϸ���ڱ���
		
		this.addWindowListener(new WindowAdapter() {//��Ӵ��ڼ���
			public void windowClosing(WindowEvent e) {//�����ڲ���������ڹر��¼�
				System.exit(0);//���ڹر���Ϸ�˳�
			}
		});
		this.addKeyListener(new KeyMonitorForStart());//��Ӽ��̼���,����Ϸ�Ƿ�ʼ
		this.addKeyListener(new KeyMonitor());//��Ӽ��̼���
	
		new Thread(new RepaintThread()).start();//�����ػ��̶߳�������ػ�
	
		new Thread(new AddFishFromLeft(this)).start();//��������ƶ���С��
		new Thread(new AddFishFromRight(this)).start();//��������ƶ���С��
		
		//ͨ����ǰ���ָı��ҷ�С����ƶ��ٶ�
		changeMyFishSpeedByScores();
	}

	/**
	 * ���ݻ��ָı���΢���С����ٶ�
	 */
	private void changeMyFishSpeedByScores() {
		//�Ȼ�ȡ��ǰMyFish�Ļ���
		int score = this.myFish.scores;//���ݻ�������MyFish���ٶ�
		if(score >= 0 && score <50) {
			addSpeed();
		}else if(score >= 50 && score < 100) {
			addSpeed();
		}else if(score >= 100 && score < 200){
			addSpeed();
		}else{
			addSpeed();
			addSpeed();
		}
	}
	/**
	 * ����ٶ�
	 */
	private void addSpeed() {
		myFish.xSpeed++;
		myFish.ySpeed++;
	}

	/**
	 * ������Ϸ����
	 */
	private void drawBackground(Graphics g) {
		Toolkit tk = Toolkit.getDefaultToolkit();//�õ���ǰϵͳĬ�ϵ�Toolkit
		Image background = tk.getImage(this.getClass()
				.getClassLoader().getResource("images/background/sea.jpg"));
		g.drawImage(background, 0, 0, null);
	}

	/**
	 * ����˫��������
	 */
	public void update(Graphics g) {
		if(null == offScreenImage) {//�ж�offScreenImage�Ƿ�Ϊ��,Ϊ���򴴽�һ��׼��ͼƬ
			offScreenImage = this.createImage(GAME_WIDTH, GAME_HEIGHT);
		}
		Graphics og = offScreenImage.getGraphics();
		drawBackground(og);
		paint(og);//ִ��paint����,��Ҫ����ͼƬ���������offScreenImage��
		g.drawImage(offScreenImage, 0, 0, null);//����Ļ�ϻ���offScreenImage
	}

	/**
	 * �ػ����߳���
	 * @author LiuWei
	 *
	 */
	class RepaintThread implements Runnable{
		public void run() {
			while(true) {
				repaint();//�����̺߳�����ػ�
				try {
					Thread.sleep(30);//ÿ��50�����ػ�һ��
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * ���̼����ڲ���,���ڸı����λ��,���㶯����
	 * @author LiuWei
	 *
	 */
	class KeyMonitor extends KeyAdapter{
		public void keyPressed(KeyEvent e) {
			myFish.keyPressed(e);
		}

		public void keyReleased(KeyEvent e) {
			myFish.keyRelease(e);
		}
	}
	
	/**
	 * ���̼���,�����Ƿ����˻س���,���»س�������isStartΪtrue,��Ϸ��ʼ
	 * @author LiuWei
	 *
	 */
	class KeyMonitorForStart extends KeyAdapter{
		public void keyPressed(KeyEvent e) {
			int key = e.getExtendedKeyCode();
			if(key == KeyEvent.VK_ENTER){
				isStart = true;
			}
		}
	}
	
	
}