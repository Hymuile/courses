import java.util.Iterator;
/*
 *@author LiuWe 
 *@see 定容栈
 */
class FixedCapacityStack<Item> implements Iterable<Item>
{
	private Item[] a = (Item[])new Object[1];
	private int N=0;
	public boolean isEmpty() 				{return N == 0;}
	public int size()						{return N;}
	public void push(Item item)	
	{
		if (N==a.length) 
			resize(2*a.length);
		a[N++]=item;
	}
	private void resize(int max)
	{
		Item[] temp= (Item[]) new Object[max];
		for(int i=0;i<N;i++)
			temp[i]=a[i];
		a=temp;
	}
	public Item pop()
	{
		Item item=a[--N];
		a[N]=null;
		if(N>0 && N==a.length/4)
			resize(a.length/2);
		return item;
	}
	public Iterator<Item> iterator()	
	{	return new ReverseArrayIterator();}
	private class ReverseArrayIterator implements Iterator<Item>
	{
		private int i=N;
		public boolean hasNext() 	{return i>0;}
		public Item next() 			{return a[--i];}
		public void remove() 		{	}
	}
}


/*
 * @author LiuWe
 * @see 下压堆栈
 */
class Stack<Item> implements Iterable<Item>
{
	private Node first;
	private int N;
	private class Node
	{
		Item item;
		Node next;
	}
	public Stack(){
		N=0;
		first=new Node();
	}
	public boolean isEmpty() 	{return first==null;}
	public int size() 			{return N;}
	public void push(Item item)
	{
		Node oldfirst = first;
		first=new Node();
		first.item=item;
		first.next=oldfirst;
		N++;
	}
	public Item pop() 
	{
		Item item=first.item;
		first=first.next;
		N--;
		return item;
	}
	
	public Iterator<Item> iterator()	
	{	return new ListIterator();}
	private class ListIterator implements Iterator<Item>
	{
		private Node current=first;
		public boolean hasNext() 	{return current!=null;}
		public Item next() 
		{
			Item item=current.item;
			current=current.next;
			return item;
		}
		public void remove() 		{	}
	}
}

/*
*@author LiuWe
*@see 先进先出队列
*/
class Queue<Item> implements Iterable<Item>
{
	private Node first;
	private Node last;
	private int N;
	private class Node
	{
		Item item;
		Node next;
	}
	public boolean isEmpty() 	{return first==null;}
	public int size() 			{return N;}
	public void enqueue(Item item)
	{
		Node oldlast=last;
		last= new Node();
		last.item=item;
		last.next=null;
		//已经处理过last，接下来处理first
		if(isEmpty())	first=last;
		else			oldlast.next=last;
		N++;
	}
	public Item dequeue()
	{
		Item item=first.item;
		first=first.next;
		//已经处理过first，接下来处理last
		if(isEmpty())	last=null;
		
		N--;
		return item;
	}
	public Iterator<Item> iterator()	
	{	return new ListIterator();}
	private class ListIterator implements Iterator<Item>
	{
		private Node current=first;
		public boolean hasNext() 	{return current!=null;}
		public Item next() 
		{
			Item item=current.item;
			current=current.next;
			return item;
		}
		public void remove() 		{	}
	}
}

