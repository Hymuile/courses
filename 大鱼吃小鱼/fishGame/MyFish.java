package fishGame;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.util.List;

public class MyFish extends GameObject{
	
	boolean bU = false, bR = false, bD = false, bL = false;//�ĸ�����������µĲ�����
	public int scores = 0;//��ҵ���Ϸ����
	
	/**
	 * �����ҷ�С���ͼƬ
	 */
	private static Toolkit tk = Toolkit.getDefaultToolkit();//�õ�ϵͳ�Ĺ��߰�
	private static Image myFish_left = null;//�����ͼƬ
	private static Image myFish_right = null;//���ҵ�ͼƬ
	
	//�ھ�̬����������ɶ�ͼƬ�ļ���
	static{
		myFish_left = tk.createImage(MyFish.class.getClassLoader().getResource("images/myFish/myfish_left.gif"));
		myFish_right = tk.createImage(MyFish.class.getClassLoader().getResource("images/myFish/myfish_right.gif"));
	}

	public MyFish(int x, int y, FishFrame ff) {//�вι��캯��
		super();
		this.x = x;
		this.y = y;
		this.xSpeed = 5;//��ʼ�����ٶ�Ϊ5
		this.ySpeed = 5;//��ʼ�����ٶ�Ϊ5
		this.dir = Direction.STOP;//��ʼ״̬�ҷ�С��ͣ����Ļ����
		this.width = 40;
		this.height = 40;
		this.ff = ff;
	}

	public void draw(Graphics g) {//ÿ���ƶ����»����ҷ�С��
		if(!live) return;//����ҷ�С���Ƿ���
		if(dir == Direction.U || dir == Direction.RU || dir == Direction.R || dir == Direction.RD){
			g.drawImage(myFish_right, x, y, width, height, null);
		}else{
			g.drawImage(myFish_left, x, y, width, height, null);
		}
		move();//ÿ��һ�ξ͸ı��������
	}

	/**
	 * ���¼�����ķ���ı�
	 * @param e �������e
	 */
	public void keyPressed(KeyEvent e) {
		int key = e.getKeyCode();
		switch(key) {
		case KeyEvent.VK_UP :
			bU = true;
			break;
		case KeyEvent.VK_RIGHT :
			bR = true;
			break;
		case KeyEvent.VK_DOWN :
			bD = true;
			break;
		case KeyEvent.VK_LEFT :
			bL = true;
			break;
		}
		getDirection();//����㵱ǰ�ķ���
	}
	
	/**
	 * �ͷż�������ĸ����򲼶�ֵ����Ϊfalse
	 * @param e
	 */
	public void keyRelease(KeyEvent e) {
		int key = e.getKeyCode();
		switch(key) {
		case KeyEvent.VK_UP :
			bU = false;
			break;
		case KeyEvent.VK_RIGHT :
			bR = false;
			break;
		case KeyEvent.VK_DOWN :
			bD = false;
			break;
		case KeyEvent.VK_LEFT :
			bL = false;
			break;
		}
		getDirection();//����㵱ǰ�ķ���
	}

	/**
	 * �����ĸ�����Ĳ������ж�С��ķ���
	 */
	private void getDirection() {
		if(bU && !bR && !bD && !bL) dir = Direction.U;
		else if(bU && bR && !bD && !bL) dir = Direction.RU;
		else if(!bU && bR && !bD && !bL) dir = Direction.R;
		else if(!bU && bR && bD && !bL) dir = Direction.RD;
		else if(!bU && !bR && bD && !bL) dir = Direction.D;
		else if(!bU && !bR && bD && bL) dir = Direction.LD;
		else if(!bU && !bR && !bD && bL) dir = Direction.L;
		else if(bU && !bR && !bD && bL) dir = Direction.LU;
		else if(!bU && !bR && !bD && !bL) dir = Direction.STOP;
	}
	
	/**
	 * �ƶ�,���ı�����
	 */
	private void move() {
		switch(dir) {
		case U :
			y -= ySpeed;
			break;
		case RU :
			y -= ySpeed;
			x += xSpeed;
			break;
		case R :
			x += xSpeed;
			break;
		case RD :
			y += ySpeed;
			x += xSpeed;
			break;
		case D :
			y += ySpeed;
			break;
		case LD :
			y += ySpeed;
			x -= xSpeed;
			break;
		case L :
			x -= xSpeed;
			break;
		case LU :
			y -= ySpeed;
			x -= xSpeed;
			break;
		}
		
		//��ֹ�����
		if(x < 2) x = 2;
		if(y < 31) y = 31;
		if(x > FishFrame.GAME_WIDTH - width) x = FishFrame.GAME_WIDTH - width;
		if(y > FishFrame.GAME_HEIGHT - height) y = FishFrame.GAME_HEIGHT - height;
	}

	/**
	 * ͨ������С�����ײ����ж��Ƿ�Ե�С��
	 * @param ef
	 * @return
	 */
	public boolean eatFish(EnemyFish ef) {
		if(this.live && ef.live && this.getRectangle().intersects(ef.getRectangle()) 
				&& this.width > ef.width) {
			ef.live = false;
			int radius = r.nextInt(3) + 3;
			this.width += radius;
			this.height += radius;
			this.scores += r.nextInt(9);
			return true;
		}
		return false;
	}
	
	/**
	 * �Ե�����С��
	 * @param list
	 * @return
	 */
	public boolean eatFishs(List<EnemyFish> list) {
		for(EnemyFish e : list) {
			if(this.eatFish(e)) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * ��ҵ�С����Ա��Ե�
	 * @param list
	 * @return
	 */
	public boolean beEaten(List<EnemyFish> list) {
		for(EnemyFish e : list) {
			if(this.live && e.live && this.getRectangle().intersects(e.getRectangle()) 
					&& this.width <= e.width){
				this.live = false;
				return true;
			}
		}
		return false;
	}
}



