package fishGame;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.util.List;

public class MyFish extends GameObject{
	
	boolean bU = false, bR = false, bD = false, bL = false;//四个方向键被按下的布尔量
	public int scores = 0;//玩家的游戏积分
	
	/**
	 * 加载我方小鱼的图片
	 */
	private static Toolkit tk = Toolkit.getDefaultToolkit();//拿到系统的工具包
	private static Image myFish_left = null;//朝左的图片
	private static Image myFish_right = null;//朝右的图片
	
	//在静态代码区中完成对图片的加载
	static{
		myFish_left = tk.createImage(MyFish.class.getClassLoader().getResource("images/myFish/myfish_left.gif"));
		myFish_right = tk.createImage(MyFish.class.getClassLoader().getResource("images/myFish/myfish_right.gif"));
	}

	public MyFish(int x, int y, FishFrame ff) {//有参构造函数
		super();
		this.x = x;
		this.y = y;
		this.xSpeed = 5;//初始横向速度为5
		this.ySpeed = 5;//初始纵向速度为5
		this.dir = Direction.STOP;//初始状态我方小鱼停在屏幕中央
		this.width = 40;
		this.height = 40;
		this.ff = ff;
	}

	public void draw(Graphics g) {//每次移动重新画出我方小鱼
		if(!live) return;//检测我方小鱼是否存活
		if(dir == Direction.U || dir == Direction.RU || dir == Direction.R || dir == Direction.RD){
			g.drawImage(myFish_right, x, y, width, height, null);
		}else{
			g.drawImage(myFish_left, x, y, width, height, null);
		}
		move();//每画一次就改变鱼的坐标
	}

	/**
	 * 按下键盘鱼的方向改变
	 * @param e 输入参数e
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
		getDirection();//获得鱼当前的方向
	}
	
	/**
	 * 释放键盘鱼的四个方向布尔值设置为false
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
		getDirection();//获得鱼当前的方向
	}

	/**
	 * 根据四个方向的布尔量判断小鱼的方向
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
	 * 移动,即改变坐标
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
		
		//防止鱼出界
		if(x < 2) x = 2;
		if(y < 31) y = 31;
		if(x > FishFrame.GAME_WIDTH - width) x = FishFrame.GAME_WIDTH - width;
		if(y > FishFrame.GAME_HEIGHT - height) y = FishFrame.GAME_HEIGHT - height;
	}

	/**
	 * 通过两条小鱼的碰撞检测判断是否吃掉小鱼
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
	 * 吃掉多条小鱼
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
	 * 玩家的小鱼可以被吃掉
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



