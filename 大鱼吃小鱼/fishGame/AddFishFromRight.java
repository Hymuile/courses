package fishGame;

import java.util.Random;

/**
 * ����Ϸ���ڵ��ұ����С����߳���
 * @author LiuWei
 *
 */
public class AddFishFromRight implements Runnable {
	private FishFrame ff;
	private Random r = new Random();
	
	public AddFishFromRight(FishFrame ff) {
		this.ff = ff;
	}

	public void run() {
		while(ff.myFish.scores < 500){
			int width = getWidthByMyFish();//ͨ����ҵ�С����ȷ������С��Ĵ�С
			int speed = getSpeedByMyFish();//ͨ����ҵ�С����ȷ������С����ٶ�
			int position = getPositionByWidth(width);
			EnemyFish ef1 = new EnemyFish(ff.GAME_WIDTH + 300, position, width, width,
					ff, Direction.L, speed, ff.myFish, EnemyFish.images[r.nextInt(3)]);
			ff.fishList_Right.add(ef1);
			try {
				Thread.sleep(1500);//ÿ��1.5s���ұ����һ��С��
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	/**
	 * ͨ��С��Ŀ�ȷ���С��ĳ���λ��,Ϊ���ǲ���С���������Ϸ���ڵı߿���
	 * @param width
	 * @return
	 */
	private int getPositionByWidth(int width) {
		return (r.nextInt(ff.GAME_HEIGHT - width - 31) + 31);
	}
	/**
	 * ͨ����ҵ�С�㷵�صо�С��Ŀ��,��ҵ�С��̫Сʱ,�о�С��Ҳ����̫��
	 * @return
	 */
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

	/**
	 * ͨ����ҵ�С�㷵�صо�С����ٶ�,��ҵ�С��̫Сʱ,�о�С��Ҳ����̫��
	 * @return
	 */
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