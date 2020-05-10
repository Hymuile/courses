package fishGame;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.util.Random;

/**
 * �з�С����
 * @author LiuWei
 *
 */
public class EnemyFish extends GameObject {
	
	public MyFish OurEnemy;//�о�С��ĵ��˾�����ҵ�С��,�����ж��Ƿ��ܳԵ���ҵ�С��
	private Image image;//С���Լ���ͼƬ
	
	/**
	 * ���صо�С���ͼƬ
	 */
	private static Random r = new Random();
	public static Image[] images = null;
	public static Toolkit tk = Toolkit.getDefaultToolkit();
	static {
		images = new Image[] {
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish1_l.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish2_l.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish3_l.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish1_r.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish2_r.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/fish3_r.gif")),
			tk.createImage(EnemyFish.class.getClassLoader().getResource("images/enemyFish/boss.gif"))
		};
	}

	public EnemyFish(int x, int y, int width, int height,
			FishFrame ff, Direction dir, int speed, MyFish OurEnemy, Image image) {
		super();
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
		this.ff = ff;
		this.dir = dir;
		this.xSpeed = speed;
		this.OurEnemy = OurEnemy;
		this.image = image;
	}
	
	/**
	 * ����С��ķ���
	 * @param g
	 */
	public void draw(Graphics g) {
		if(!live) {
			if(dir.equals(Direction.R)){
				ff.fishList_Left.remove(this);//���С�������ʹ��������Ƴ�
			}else{
				ff.fishList_Right.remove(this);//���С�������ʹ��������Ƴ�
			}
			return;//С���������ٻ���
		}
		
		g.drawImage(image, x, y, width, height, null);//�����Լ���ͼƬ
		
		move();//С��λ�ñ仯
	}
	
	/**
	 * С���ƶ��ķ���
	 */
	private void move() {
		if(dir.equals(Direction.L)) {
			x -= xSpeed;//���С��ķ����ǳ����,x����ݼ������ƶ�
		}else {
			x += xSpeed;//���������ƶ�
		}
		
		//С������߳��߽����ж�Ϊ����
		if(dir.equals(Direction.R)){
			if(x > ff.GAME_WIDTH) this.live = false;//���ҵ�С��,���x������ڴ��ڿ��������
		}else{
			if(x < 0) this.live = false;//�����С��,���x����С�ڴ��ڿ��������
		}
	}
}