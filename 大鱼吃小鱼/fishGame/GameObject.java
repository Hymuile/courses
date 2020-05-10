package fishGame;

import java.awt.Rectangle;
import java.util.Random;

/**
 * ��Ϸ���常��
 * @author LiuWei
 *
 */
public class GameObject {
	public int x, y;//��Ϸ���������
	public Direction dir;//��Ϸ����ķ���
	public int xSpeed, ySpeed;//��Ϸ������x,y������λ�Ƶ��ٶ�
	public int width, height;//��Ϸ����Ŀ�͸�
	public boolean live = true;//��Ϸ���������,��ʼΪtrue
	public FishFrame ff;//��Ϸ�����
	public static Random r = new Random();//�ṩ������ʹ�õ������������
	
	public GameObject() {
	}

	public GameObject(int x, int y, Direction dir, int xSpeed, int ySpeed, int width, int height, boolean live) {
		super();
		this.x = x;
		this.y = y;
		this.dir = dir;
		this.xSpeed = xSpeed;
		this.ySpeed = ySpeed;
		this.width = width;
		this.height = height;
		this.live = live;
	}
	
	/**
	 * ��õ�ǰ����ľ���,������ײ���
	 * @return
	 */
	public Rectangle getRectangle() {
		return new Rectangle(x, y, width, height);
	}
}
