package fishGame;

import java.util.Random;
/**
 * ����Ϸ���ڵ�������С����߳���,ע���뿴AddFishFromright��
 * @author LiuWei
 *
 */
public class AddFishFromLeft implements Runnable {
	private FishFrame ff;
	private Random r = new Random();
	
	public AddFishFromLeft(FishFrame ff) {
		this.ff = ff;
	}

	/**
	 * ���÷ֳ���500ʱ��ʱС��̫�����һ��boss������Ϸ
	* ��Ϸ�ɸĽ�Ϊ�ؿ�ģʽ��ÿһ�شﵽ���ٷ�����Ϊ�ϸ񣬴�ʱδʵ�ִ˹���
	*/
	public void run() {
		while(ff.myFish.scores < 500){
			int width = getWidthByMyFish();
			int position = getPositionByWidth(width);
			int speed = getSpeedByMyFish();
			EnemyFish ef1 = new EnemyFish(0 - 300, position, width, width,
					ff, Direction.R, speed, ff.myFish, EnemyFish.images[r.nextInt(3) + 3]);
			ff.fishList_Left.add(ef1);
			try {
				Thread.sleep(1500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		addBoss();
	}

	private int getPositionByWidth(int width) {
		return (r.nextInt(ff.GAME_HEIGHT - width - 31) + 31);
	}

	private void addBoss() {
		int radius = FishFrame.GAME_HEIGHT - 100;
		int speed = 5;
		EnemyFish ef = new EnemyFish(0 - FishFrame.GAME_WIDTH, 50, radius, radius,
				ff, Direction.R, speed, ff.myFish, EnemyFish.images[EnemyFish.images.length-1]);
		ff.fishList_Left.add(ef);
	}

	private int getWidthByMyFish() {
		//�Ȼ�ȡ��ǰMyFish�Ļ���
		int score = ff.myFish.scores;//���ݻ�������С��Ĵ�С
		if(score >= 0 && score <50) {
			return r.nextInt(90) + 6;
		}else if(score >= 50 && score < 100) {
			return r.nextInt(127) + 9;
		}else if(score >= 100 && score < 200){
			return r.nextInt(160) + 12;
		}else{
			return r.nextInt(230) + 20;
		}
	}

	private int getSpeedByMyFish() {
		//�Ȼ�ȡ��ǰMyFish�Ļ���
		int score = ff.myFish.scores;//���ݻ�������С����ٶ�
		if(score >= 0 && score <50) {
			return r.nextInt(5) + 1;
		}else if(score >= 50 && score < 100) {
			return r.nextInt(9) + 1;
		}else if(score >= 100 && score < 200){
			return r.nextInt(13) + 1;
		}else{
			return r.nextInt(18) + 1;
		}
	}
}