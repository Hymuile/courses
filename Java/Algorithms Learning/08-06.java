/*
 * 参考来源：https://www.cnblogs.com/dacc123/p/10574939.html
 * @author LiuWe
 * @see 使用五个栈（理论六个栈）实现队列，保证在最坏情况下模拟常数效率操作
 */
class QueueByUsingStacks<Item>
{
	private final int capacity=1005;	//栈容量
	
	/*
	 * 给head栈专用的栈，有两个游标 top 和 top_c
	*/
	private class StackHead<Item>
	{
		private Item[] s;
		private int top;
		private int top_c;
		public StackHead()
		{
			s = (Item[]) new Object[capacity];
			top=0;
			top_c=0;
		}
	}
	
	/*
	 * 普通栈，增加了出队列标记，这个出队列标记是给 head_r 专用的
	 */
	private class Stack<Item>
	{
		private Item[] s;
		private int top;
		private int dequeue;
		public Stack() 
		{
			s = (Item[]) new Object[capacity];
			top=0;
			dequeue=0;
		}
	}
	
	private int num;			// 队列已有元素数量
	private int copy_head;		// 1 表示head 正在往head_r 中倒元素。0 表示head中的元素倒空了。
	private int copy_tail;		// 1  表示tail_l中有元素，tail_l 中正在往head_l   中倒元素。0 表示tail_l中的元素倒空了。
								//copy_tail == 0 && copy_head==0 表示，head_r 需要往head_l 中倒元素了。
	private StackHead<Item> head;
	private StackHead<Item> head_l;
	private Stack<Item> tail;
	private Stack<Item> tail_l;
	private Stack<Item> head_r;
	
	public QueueByUsingStacks()
	{
		head = new StackHead<Item>();
		head_l = new StackHead<Item>();
		tail = new Stack<Item>();
		tail_l = new Stack<Item>();
		head_r = new Stack<Item>();
		copy_head=0;
		copy_tail=0;
		num=0;
	}
	
	/*
	 *  辅助函数：交换入队列栈
	 */
	private void change()
	{
		if (0 == tail_l.top)
		{
			Stack<Item> temp = tail;
			tail=tail_l;
			tail_l=temp;
		}
	}
	private void copy()
	{
	    judge();
	    //原则二
	    if(copy_tail==1)
	    {
	        head_l.s[head_l.top++] = tail_l.s[--tail_l.top];
	        head_l.top_c++;
	    }
	   //原则三
	    if(copy_head==1)
	        head_r.s[head_r.top++] = head.s[--head.top_c];
	    judge();
	    //原则四
	    if(copy_head==0&&copy_tail==0)
	    {
	        if(head_r.top > head_r.dequeue)
	        {
	            head_l.s[head_l.top++]=head_r.s[--head_r.top];
	            head_l.top_c++;
	        }
	        
	        // 在 head-c中出队列，就需要在 head-r 中标记出来。下次 head-r 需要倒元素的时候，就要注意标记的元素已经被出队列了。
	        // 判断 head_r 中是否还有元素，因为head-r中的元素有部分可能是已经被出队列的，所以通过判断top是否等于dequeue。
	        // 然后清空head_r，head_r为空，意味着head和head_l需要交换身份了，因为其中head_l被head_r倒满了，需要作为新的出队列栈。
	        if(head_r.top == head_r.dequeue)
	        {
	            head_r.top=0;
	            head_r.dequeue=0;
	            
	            head.top_c=0;
	            head.top=0;
	            
	            StackHead<Item> temp = head;
	            head = head_l;
	            head_l = temp;
	        }
	    }
	}
	
	private void Judge()
	{
	    if(copy_tail==0 && tail_l.top>0 && head_l.top==0 && head_l.top_c==0) copy_tail=1;
	    if(copy_tail==1 && tail_l.top==0) copy_tail=0; 
	    if(copy_head==0 && head.top_c>0) copy_head=1;
	    if(copy_head==1 && head.top_c==0) copy_head=0; 
	}
	
	public Item dequeue()
	{
		Item item = head.s[--head.top];
		head_r.dequeue++;
		change();
		copy();
		return item;
	}

	public void enqueue(Item item)
	{
		tail.s[tail.top++]=item;
		change();
		num++;
		copy();
	}
}
